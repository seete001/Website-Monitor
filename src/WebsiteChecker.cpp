#include "WebsiteChecker.h"

#include <curl/curl.h>
#include <iostream>

WebsiteChecker::WebsiteChecker(const std::string& url, long timeout) : url(url), timeout(timeout)
{
    // Constructor
}

bool WebsiteChecker::check()
{
    // Initialize libcurl
    CURL* curl = curl_easy_init();

    // Error handling
    if(curl == nullptr)
    {
        throw std::runtime_error("Could not initialize libcurl");
        return false;
    }


    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());                       // Setting URL
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout);                       // Setting timeout
    curl_easy_setopt(curl, CURLOPT_NOBODY, 1L);                             // Asking for just HEAD

    CURLcode result = curl_easy_perform(curl);                              // Request sent

    long status_code = 0;

    if(result == CURLE_OK)
    {
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &status_code);      // Setting the status code
    }

    curl_easy_cleanup(curl);                                                // Releasing the resources used by libcurl

    if(result == CURLE_OK && status_code >= 200 && status_code < 400)
    {
        return true;
    }

    return false;
}