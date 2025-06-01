/* Test http_request() kfun */

/* Helper function to find substring */
int strstr(string haystack, string needle) {
    int i, len_h, len_n;
    
    len_h = strlen(haystack);
    len_n = strlen(needle);
    
    for (i = 0; i <= len_h - len_n; i++) {
        if (haystack[i..i+len_n-1] == needle) {
            return i;
        }
    }
    return -1;
}

void test() {
    mapping response, headers;
    string url, body;
    
    write_file("/test_results.txt", "\n--- Testing http_request() ---\n");
    
    /* Test 1: Simple GET request */
    url = "http://httpbin.org/get";
    headers = ([ ]);
    body = "";
    
    response = http_request(url, "GET", headers, body);
    
    if (!response) {
        write_file("/test_results.txt", "ERROR: http_request() GET returned nil\n");
        return;
    }
    
    write_file("/test_results.txt", "SUCCESS: http_request() GET returned response\n");
    write_file("/test_results.txt", "  Status: " + response["status"] + "\n");
    write_file("/test_results.txt", "  Headers received: " + sizeof(response["headers"]) + " headers\n");
    write_file("/test_results.txt", "  Body length: " + strlen(response["body"]) + " bytes\n");
    
    /* Test 2: POST request with data */
    url = "http://httpbin.org/post";
    headers = ([ "Content-Type": "application/json" ]);
    body = "{\"test\": \"data\", \"number\": 42}";
    
    response = http_request(url, "POST", headers, body);
    
    if (!response) {
        write_file("/test_results.txt", "ERROR: http_request() POST returned nil\n");
    } else {
        write_file("/test_results.txt", "SUCCESS: http_request() POST returned response\n");
        write_file("/test_results.txt", "  Status: " + response["status"] + "\n");
        write_file("/test_results.txt", "  Body contains our data: " + 
                   (strstr(response["body"], "\"test\": \"data\"") >= 0 ? "YES" : "NO") + "\n");
    }
    
    /* Test 3: Custom headers */
    url = "http://httpbin.org/headers";
    headers = ([ 
        "X-Custom-Header": "Viking-DGD-Test",
        "User-Agent": "Viking-DGD-Extension/1.0"
    ]);
    body = "";
    
    response = http_request(url, "GET", headers, body);
    
    if (!response) {
        write_file("/test_results.txt", "ERROR: http_request() with custom headers returned nil\n");
    } else {
        write_file("/test_results.txt", "SUCCESS: http_request() with custom headers returned response\n");
        write_file("/test_results.txt", "  Status: " + response["status"] + "\n");
        write_file("/test_results.txt", "  Custom header reflected: " +
                   (strstr(response["body"], "Viking-DGD-Test") >= 0 ? "YES" : "NO") + "\n");
    }
    
    /* Test 4: Test other HTTP methods */
    url = "http://httpbin.org/delete";
    response = http_request(url, "DELETE", ([ ]), "");
    write_file("/test_results.txt", "DELETE request status: " + 
               (response ? response["status"] : "FAILED") + "\n");
    
    url = "http://httpbin.org/put";
    response = http_request(url, "PUT", ([ ]), "test data");
    write_file("/test_results.txt", "PUT request status: " + 
               (response ? response["status"] : "FAILED") + "\n");
}