#ifndef CURL_GLOBAL_H
#define CURL_GLOBAL_H

class CurlGlobal
{
    public:
        CurlGlobal();
        ~CurlGlobal();

        CurlGlobal(const CurlGlobal&) = delete;
        CurlGlobal& operator=(const CurlGlobal&) = delete;
};

#endif