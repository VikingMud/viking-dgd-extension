/*
 * Viking DGD Extension - HTTP Client Functions
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "viking_ext.h"
#include "kfun_http.h"

/* CURL callback for response data */
size_t viking_curl_write_cb(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    struct http_response *resp = (struct http_response *)userp;
    
    char *ptr = realloc(resp->data, resp->size + realsize + 1);
    if (!ptr) {
        return 0;
    }
    
    resp->data = ptr;
    memcpy(&(resp->data[resp->size]), contents, realsize);
    resp->size += realsize;
    resp->data[resp->size] = 0;
    
    return realsize;
}

/* CURL callback for headers */
size_t viking_curl_header_cb(char *buffer, size_t size, size_t nitems, void *userdata) {
    size_t realsize = size * nitems;
    struct http_response *resp = (struct http_response *)userdata;
    
    /* Parse header line */
    char *header = malloc(realsize + 1);
    if (header) {
        memcpy(header, buffer, realsize);
        header[realsize] = 0;
        
        /* Remove trailing newline */
        char *newline = strchr(header, '\r');
        if (newline) *newline = 0;
        newline = strchr(header, '\n');
        if (newline) *newline = 0;
        
        /* Add to header list if not empty */
        if (strlen(header) > 0) {
            resp->headers = curl_slist_append(resp->headers, header);
        }
        
        free(header);
    }
    
    return realsize;
}

/*
 * http_request(string url, string method, mapping headers, string body)
 * Make an HTTP request and return response as a mapping
 */
void kf_http_request(LPC_frame f, int nargs, LPC_value retval) {
    LPC_value arg, temp, key, val;
    LPC_string url_str, method_str, body_str;
    LPC_mapping headers_map, result_map, response_headers;
    LPC_dataspace data;
    CURL *curl;
    CURLcode res;
    struct http_response response = {0};
    struct curl_slist *request_headers = NULL;
    long http_code = 0;
    int i;
    
    /* Get URL */
    arg = lpc_frame_arg(f, nargs, 0);
    if (lpc_value_type(arg) != LPC_TYPE_STRING) {
        lpc_runtime_error(f, "Bad argument 1 for kfun http_request");
        return;
    }
    url_str = lpc_string_getval(arg);
    
    /* Get method */
    arg = lpc_frame_arg(f, nargs, 1);
    if (lpc_value_type(arg) != LPC_TYPE_STRING) {
        lpc_runtime_error(f, "Bad argument 2 for kfun http_request");
        return;
    }
    method_str = lpc_string_getval(arg);
    
    /* Get headers mapping */
    arg = lpc_frame_arg(f, nargs, 2);
    if (lpc_value_type(arg) != LPC_TYPE_MAPPING) {
        lpc_runtime_error(f, "Bad argument 3 for kfun http_request");
        return;
    }
    headers_map = lpc_mapping_getval(arg);
    
    /* Get body */
    arg = lpc_frame_arg(f, nargs, 3);
    if (lpc_value_type(arg) != LPC_TYPE_STRING) {
        lpc_runtime_error(f, "Bad argument 4 for kfun http_request");
        return;
    }
    body_str = lpc_string_getval(arg);
    
    /* Initialize CURL */
    curl = curl_easy_init();
    if (!curl) {
        return; /* nil */
    }
    
    data = lpc_frame_dataspace(f);
    temp = lpc_value_temp(data);
    
    /* Set URL */
    curl_easy_setopt(curl, CURLOPT_URL, lpc_string_text(url_str));
    
    /* Set method */
    char *method = lpc_string_text(method_str);
    if (strcmp(method, "POST") == 0) {
        curl_easy_setopt(curl, CURLOPT_POST, 1L);
    } else if (strcmp(method, "PUT") == 0) {
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
    } else if (strcmp(method, "DELETE") == 0) {
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
    } else if (strcmp(method, "HEAD") == 0) {
        curl_easy_setopt(curl, CURLOPT_NOBODY, 1L);
    }
    
    /* Set request headers */
    for (i = 0; i < lpc_mapping_size(headers_map); i++) {
        key = lpc_mapping_enum(headers_map, i);
        if (lpc_value_type(key) == LPC_TYPE_STRING) {
            val = lpc_mapping_index(headers_map, key);
            if (lpc_value_type(val) == LPC_TYPE_STRING) {
                char header_buf[1024];
                snprintf(header_buf, sizeof(header_buf), "%s: %s",
                         lpc_string_text(lpc_string_getval(key)),
                         lpc_string_text(lpc_string_getval(val)));
                request_headers = curl_slist_append(request_headers, header_buf);
            }
        }
    }
    if (request_headers) {
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, request_headers);
    }
    
    /* Set body */
    if (lpc_string_length(body_str) > 0) {
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, lpc_string_text(body_str));
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, lpc_string_length(body_str));
    }
    
    /* Set callbacks */
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, viking_curl_write_cb);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, viking_curl_header_cb);
    curl_easy_setopt(curl, CURLOPT_HEADERDATA, &response);
    
    /* Follow redirects */
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_MAXREDIRS, 10L);
    
    /* Timeout */
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30L);
    
    /* Perform request */
    res = curl_easy_perform(curl);
    
    if (res == CURLE_OK) {
        /* Get HTTP status code */
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
        
        /* Create result mapping */
        result_map = lpc_mapping_new(data);
        
        /* Add status code */
        lpc_string_putval(temp, lpc_string_new(data, "status", 6));
        key = lpc_value_temp2(data);
        lpc_int_putval(key, http_code);
        lpc_mapping_assign(data, result_map, temp, key);
        
        /* Add response headers */
        response_headers = lpc_mapping_new(data);
        struct curl_slist *header = response.headers;
        while (header) {
            char *colon = strchr(header->data, ':');
            if (colon) {
                *colon = 0;
                char *value = colon + 1;
                while (*value == ' ') value++;
                
                lpc_string_putval(temp, lpc_string_new(data, header->data, strlen(header->data)));
                lpc_string_putval(key, lpc_string_new(data, value, strlen(value)));
                lpc_mapping_assign(data, response_headers, temp, key);
            }
            header = header->next;
        }
        lpc_string_putval(temp, lpc_string_new(data, "headers", 7));
        lpc_mapping_putval(key, response_headers);
        lpc_mapping_assign(data, result_map, temp, key);
        
        /* Add response body */
        lpc_string_putval(temp, lpc_string_new(data, "body", 4));
        lpc_string_putval(key, lpc_string_new(data, response.data ? response.data : "", 
                                               response.data ? response.size : 0));
        lpc_mapping_assign(data, result_map, temp, key);
        
        /* Return the mapping */
        lpc_mapping_putval(retval, result_map);
    }
    
    /* Cleanup */
    curl_easy_cleanup(curl);
    if (request_headers) {
        curl_slist_free_all(request_headers);
    }
    if (response.headers) {
        curl_slist_free_all(response.headers);
    }
    if (response.data) {
        free(response.data);
    }
}