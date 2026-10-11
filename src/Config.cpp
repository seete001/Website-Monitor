#include "Config.h"

#include <fstream>
#include <stdexcept>
#include<string>

Config loadConfig(const std::string& filename)
{
    Config settings{"", 60};

    std::ifstream file(filename);

    if(!file)
    {
        throw std::runtime_error("Could not open config file");
    }

    std::string line;

    while(std::getline(file, line))
    {
        const std::size_t seperator = line.find('=');

        if(seperator == std::string::npos)
        {
            continue;
        }

        const std::string key = line.substr(0, seperator);
        const std::string value = line.substr(seperator + 1);

        if(key == "url")
        {
            settings.url = value;
        }
        else if(key == "interval_secons")
        {
            settings.intervalSeconds = std::stoi(value);
        }
    }

    if(settings.url.empty() || settings.intervalSeconds < 1)
    {
        throw std::runtime_error("Invalid config");
    }

    return settings;
}