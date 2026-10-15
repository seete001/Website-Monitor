#include "Logger.h"
#include "WebsiteChecker.h"
#include "Config.h"
#include "CurlGlobal.h"

#include <iostream>
#include <exception>
#include <chrono>
#include <thread>
#include <string>
#include <mutex>
#include <csignal>
#include <condition_variable>

volatile std::sig_atomic_t stopRequested = 0;

void handleSignal(int)
{
    stopRequested = 1;
}

int main()
{
    std::signal(SIGINT, handleSignal);
    std::signal(SIGTERM, handleSignal);

    try
    {
        Config config = loadConfig("config.conf");

        CurlGlobal curl;

        Logger logger(config.log_file);
        logger.log("Monitoring started: " + config.url);

        WebsiteChecker checker(config.url, config.timeout);

        std::mutex statusMutex;
        std::condition_variable statusChanged;

        std::string status;
        bool newRes = false;
        bool checkerFinished = false;

        std::thread checkerThread([&]()
        {
            try
            {
                while (!stopRequested)
                {
                    const std::string result =
                        checker.check() ? "UP" : "DOWN";

                    {
                        std::lock_guard<std::mutex> lock(statusMutex);
                        status = result;
                        newRes = true;
                    }

                    statusChanged.notify_one();

                    for (int i = 0;
                         i < config.intervalSeconds && !stopRequested;
                         ++i)
                    {
                        std::this_thread::sleep_for(
                            std::chrono::seconds(1));
                    }
                }
            }
            catch (const std::exception& e)
            {
                std::cerr << "Checker error: " << e.what() << '\n';
            }

            {
                std::lock_guard<std::mutex> lock(statusMutex);
                checkerFinished = true;
            }

            statusChanged.notify_one();
        });

        std::thread loggerThread([&]()
        {
            while (true)
            {
                std::string result;

                {
                    std::unique_lock<std::mutex> lock(statusMutex);

                    statusChanged.wait(lock, [&]()
                    {
                        return newRes || checkerFinished;
                    });

                    if (newRes)
                    {
                        result = status;
                        newRes = false;
                    }
                    else if (checkerFinished)
                    {
                        break;
                    }
                }

                if (!result.empty())
                {
                    logger.log(result);
                }
            }
        });

        checkerThread.join();
        loggerThread.join();

        logger.log("Monitoring stopped");
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    }

    return 0;
}
