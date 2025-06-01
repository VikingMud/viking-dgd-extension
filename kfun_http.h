/*
 * Viking DGD Extension - HTTP Client Header
 */

#ifndef KFUN_HTTP_H
#define KFUN_HTTP_H

#include <curl/curl.h>

/* HTTP response buffer */
struct http_response {
    char *data;
    size_t size;
    struct curl_slist *headers;
};

/* CURL callbacks */
size_t viking_curl_write_cb(void *contents, size_t size, size_t nmemb, void *userp);
size_t viking_curl_header_cb(char *buffer, size_t size, size_t nitems, void *userdata);

#endif /* KFUN_HTTP_H */