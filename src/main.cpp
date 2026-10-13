#include "Logger.h"
#include "WebsiteChecker.h"
#include "Config.h"
#include "CurlGlobal.h"



#include <iostream>
#include <exception>
#include <chrono>
#include <fstream>
#include <thread>
#include <string>
#include <mutex>
#include <csignal>


/**
 * Handling SIGTERM and SIGINT
 */
volatile std::sig_atomic_t stopRequested = 0;

void handleSignal(int)
{
    stopRequested = 1;
}

/**
 * Main function
 */
int main()
{

    // Register signal handlers
    std::signal(SIGINT, handleSignal);
    std::signal(SIGTERM, handleSignal);

    try
    {
        /**
         * 
         * Initializing Configuration, Curl, Logger, WebsiteChecker
         * 
         */
        Config config = loadConfig("config.conf");

        CurlGlobal curl;
        
        Logger logger(config.log_file);
        logger.log("Monitoring started: " + config.url);
        
        WebsiteChecker checker(config.url, config.timeout);
        
        // Setup status statusMutex and newRes
        std::string status;
        std::mutex statusMutex;
        bool newRes = false;
        bool checkerFinished;

        /**
         * 
         * First Thread for Requesting to the web
         * and update the status to log
         * 
         */
        std::thread checkerThread([&]()
        {
            while(!stopRequested)
            {
                std::string result = checker.check() ? "UP" : "DOWN";

                {
                    std::lock_guard<std::mutex> lock(statusMutex);
                    
                    status = result;
                    newRes = true;
                }

                for(int i = 0; i < config.intervalSeconds && !stopRequested;++i)
                {
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                }
                {
                    std::lock_guard<std::mutex> lock(statusMutex);
                    checkerFinished = true;
                }
            }
        });

        /**
         * 
         * Second Thread to log the updated result
         * 
         */
        std::thread loggerThread([&]()
        {
            while(true)
            {
                std::string result;
                bool finished;

                {
                    // To prevent both threads accessing the status at the same time
                    std::lock_guard<std::mutex> lock(statusMutex);
                    
                    if(newRes) // If there is an update to the result
                    {
                        result = status;
                        newRes = false;
                    }

                    finished = checkerFinished && !newRes;
                }

                if(!result.empty())
                {
                    logger.log(result);
                }

                if(finished) break;
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }
        });

        // Starting and joining the threads
        checkerThread.join();
        loggerThread.join();

        logger.log("Monitoring stopped");
    }

    // Error handling
    catch(const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
