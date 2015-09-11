//
// Created by pranaykumar on 11/09/15.
//
#include <iostream>
#include <curl/curl.h>

#include "fetchData.h"
using namespace std;

string data;


size_t callback(char* buf, size_t size, size_t nmemb, void* up)
{   //callback must have this declaration
    //buf is a pointer to the data that curl has for us
    //size*nmemb is the size of the buffer

    for (int c = 0; c<size*nmemb; c++)
    {
        data.push_back(buf[c]);
    }
    return size*nmemb; //tell curl how many bytes we handled
}



std::string fetchData(std::string url) {
    CURL *curl;
    curl_global_init(CURL_GLOBAL_ALL); //pretty obvious
    curl = curl_easy_init();

// Set remote URL.
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

// Don't bother trying IPv6, which would increase DNS resolution time.
// curl_easy_setopt(curl, CURLOPT_IPRESOLVE, CURL_IPRESOLVE_V4);

// Don't wait forever, time out after 10 seconds.
//curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10);

// Follow HTTP redirects if necessary.
// curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

// Response information.
    long httpCode(0);
//std::unique_ptr<std::string> httpData(new std::string());

// Hook up data handling function.
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &callback);

// Hook up data container (will be passed as the last parameter to the
// callback handling function).  Can be any pointer type, since it will
// internally be passed as a void pointer.
//  curl_easy_setopt(curl, CURLOPT_WRITEDATA, httpData.get());

// Run our HTTP GET command, capture the HTTP response code, and clean up.
    curl_easy_perform(curl);
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);
    curl_easy_cleanup(curl);

    if (httpCode == 200)
        return data;
    else
        return "NULL";
}