#ifndef CONFIG_H
#define CONFIG_H

#include <string>

struct Config
{
    std::string url;
    int intervalSeconds;
};

Config loadConfig(const std::string& filename);

#endif