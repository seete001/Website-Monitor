#include "Config.h"

#include <iostream>
#include <exception>

int main()
{
    std::cout << "Website monitoring is running ..." << std::endl;

    try
    {
        Config settings = loadConfig("config.conf");

        std::cout << "URL: " << settings.url << std::endl;
        std::cout << "Checktime: " << settings.intervalSeconds << " seconds" << std::endl;
    
        return 0;
    }
    catch(const std::exception& error)
    {
        std::cerr << "Error: " << error.what() << std::endl;
        return 1;    
    }
}
