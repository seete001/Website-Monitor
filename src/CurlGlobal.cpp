#include "CurlGlobal.h"

#include <curl/curl.h>
#include <stdexcept>

CurlGlobal::CurlGlobal()
{
    if(curl_global_init(CURL_GLOBAL_DEFAULT) != CURLE_OK)
    {
        throw std::runtime_error("Could not initialize libcurl");
    }
}

CurlGlobal::~CurlGlobal()
{
    curl_global_cleanup();
}


