#ifndef __HTTPCLIENT_H
#define __HTTPCLIENT_H

#include <string>

char *http_get (const char *url);
std::string http_get (const std::string& url);

#endif
