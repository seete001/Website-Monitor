#include "Config.h"

#include <fstream>
#include <stdexcept>

Config loadConfig(const std::string& filename)
{
    Config settings = {"", 60, 15, ""};
    
    std::ifstream file(filename);

    if(!file.is_open())
    {
        throw std::runtime_error("Could not open the log file");
    }

    std::string line;

    while(std::getline(file, line))
    {
        std::size_t seperator = line.find("=");

        if(seperator == std::string::npos)
        {
            continue;
        }

        std::string key = line.substr(0, seperator);
        std::string value = line.substr(seperator + 1);

        if(key == "url")
        {
            settings.url = value;
        }
        else if(key == "interval_seconds")
        {
            settings.intervalSeconds = std::stoi(value);
        }
        else if(key == "timeout_seconds")
        {
            settings.timeout = std::stoi(value);
        }
        else if(key == "log_file")
        {
            settings.log_file = value;
        }
    }

    if(settings.url.empty() || settings.log_file.empty() || 
        settings.intervalSeconds <= 0 || settings.timeout <= 0)
        {
            throw std::runtime_error("Invalid Configuration");
        }
    
    return settings;
}