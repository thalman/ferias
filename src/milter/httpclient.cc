#include "httpclient.h"

#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

struct httpMemoryBuffer {
    char *buffer;
    size_t size;
};
 
static size_t http_memory_write(void *ptr, size_t size, size_t nmemb, void *userdata)
{
    if (!userdata) return -1;
    size_t totalsize = size * nmemb;
    if (!totalsize) return 0;

    struct httpMemoryBuffer *memory = (struct httpMemoryBuffer *)userdata;
    char *mem = NULL;
    if (memory->buffer) {
        // check limit here?
        char *mem = (char *) realloc (memory->buffer, totalsize + memory->size + 1);
        if (!mem) return -1;
    } else {
        char *mem = (char *) malloc (totalsize + 1);
        if (!mem) return -1;
    }
    memory->buffer = mem;
    memcpy (&memory->buffer[memory->size], ptr, totalsize);
    memory->size += totalsize;
    memory->buffer[memory->size] = '\x00';
    return totalsize;
}

char *http_get (const char *url)
{
    //curl_global_init(CURL_GLOBAL_DEFAULT);
    struct httpMemoryBuffer buffer;
    buffer.size = 0;
    buffer.buffer = NULL;
    CURLcode res;

    CURL *curl = curl_easy_init ();
    if(curl) {
        curl_easy_setopt (curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, http_memory_write);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);

        res = curl_easy_perform(curl);

        curl_easy_cleanup(curl);
        if(CURLE_OK != res) {
            if (buffer.buffer) free (buffer.buffer);
            return NULL;
        }
    }
    return buffer.buffer;
    // curl_global_cleanup();
}

std::string http_get (const std::string& url)
{
    if (url.empty()) return "";
    char *body = http_get (url.c_str());
    if (!body) return "";
    std::string result = body;
    free (body);
    return result;
}

