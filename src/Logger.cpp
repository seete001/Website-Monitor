#include "Logger.h"

#include <fstream>
#include <stdexcept>
#include <ctime>
#include <iomanip>



Logger::Logger(const std::string&filename) : filename(filename)
{
    // Constructor while creating
}

void Logger::log(const std::string& message)
{
    // Opening the file
    std::ofstream file(filename, std::ios::app);

    // Handle the error
    if(!file.is_open())
    {
        throw std::runtime_error("Could not open the log file");
    }

    std::time_t now = std::time(nullptr);
    std::tm* local_time = std::localtime(&now);

    file << "[";

    if(local_time != nullptr)
    {
        file << std::put_time(local_time, "%Y-%m-%d %H:%M:%S");
    }
    else
    {
        file << "unknown time";
    }

    file << "] " << message << "\n";
    file.flush();
}
