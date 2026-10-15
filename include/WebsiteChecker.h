#ifndef WEBSITE_CHECKER_H
#define WEBSITE_CHECKER_H

#include <string>

class WebsiteChecker
{
    public:
        WebsiteChecker(const std::string& url, long timeout);

        bool check();

    private:
        std::string url;
        long timeout;
};

#endif