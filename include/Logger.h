#ifndef LOGGER_H
#define LOGGER_H

#include <string>


class Logger
{
    public:
        explicit Logger(const std::string& filename);

        void log(const std::string& message);

    private:
        std::string filename;
};

#endif