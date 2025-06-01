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
    int passed, failed;
    
    passed = 0;
    failed = 0;
    
    write_file("/test_results.txt", "\n--- Testing http_request() ---\n");
    
    /* Test 1: Simple GET request */
    write_file("/test_results.txt", "\nTest 1: GET request\n");
    url = "http://httpbin.org/get";
    response = http_request(url, "GET", ([ ]), "");
    
    if (!response) {
        write_file("/test_results.txt", "  ERROR: http_request() returned nil\n");
        failed++;
    } else {
        write_file("/test_results.txt", "  Status: " + response["status"] + "\n");
        if (response["status"] == 200) {
            write_file("/test_results.txt", "  ✓ Got 200 OK\n");
            passed++;
        } else {
            write_file("/test_results.txt", "  ✗ Expected status 200\n");
            failed++;
        }
        
        if (response["body"] && strlen(response["body"]) > 0) {
            write_file("/test_results.txt", "  Body length: " + strlen(response["body"]) + " bytes\n");
            if (strstr(response["body"], "\"url\"") >= 0) {
                write_file("/test_results.txt", "  ✓ Body contains expected JSON\n");
                passed++;
            } else {
                write_file("/test_results.txt", "  ✗ Body missing expected JSON\n");
                failed++;
            }
        } else {
            write_file("/test_results.txt", "  ✗ No body received\n");
            failed++;
        }
    }
    
    /* Test 2: POST request with data */
    write_file("/test_results.txt", "\nTest 2: POST request with JSON\n");
    url = "http://httpbin.org/post";
    headers = ([ "Content-Type": "application/json" ]);
    body = "{\"test\": \"data\", \"number\": 42}";
    
    response = http_request(url, "POST", headers, body);
    
    if (!response) {
        write_file("/test_results.txt", "  ERROR: http_request() POST returned nil\n");
        failed++;
    } else {
        write_file("/test_results.txt", "  Status: " + response["status"] + "\n");
        if (response["status"] == 200) {
            write_file("/test_results.txt", "  ✓ Got 200 OK\n");
            passed++;
        } else {
            write_file("/test_results.txt", "  ✗ Expected status 200\n");
            failed++;
        }
        
        /* Check if our posted data is echoed back */
        if (response["body"]) {
            if (strstr(response["body"], "\"test\": \"data\"") >= 0) {
                write_file("/test_results.txt", "  ✓ Posted data found in response body\n");
                passed++;
            } else {
                write_file("/test_results.txt", "  ✗ Posted data not echoed back\n");
                failed++;
            }
        }
    }
    
    /* Test 3: Custom headers */
    write_file("/test_results.txt", "\nTest 3: Custom headers\n");
    url = "http://httpbin.org/headers";
    headers = ([ 
        "X-Custom-Header": "Viking-DGD-Test",
        "User-Agent": "Viking-DGD-Extension/1.0"
    ]);
    
    response = http_request(url, "GET", headers, "");
    
    if (response && response["status"] == 200) {
        write_file("/test_results.txt", "  ✓ Got response with custom headers\n");
        passed++;
        if (strstr(response["body"], "Viking-DGD-Test") >= 0) {
            write_file("/test_results.txt", "  ✓ Custom header reflected in response body\n");
            passed++;
        } else {
            write_file("/test_results.txt", "  ✗ Custom header not found in body\n");
            failed++;
        }
    } else {
        write_file("/test_results.txt", "  ✗ Failed to send custom headers\n");
        failed++;
    }
    
    /* Test 4: Different HTTP methods */
    write_file("/test_results.txt", "\nTest 4: Other HTTP methods\n");
    
    response = http_request("http://httpbin.org/delete", "DELETE", ([ ]), "");
    if (response && response["status"] == 200) {
        write_file("/test_results.txt", "  ✓ DELETE request successful\n");
        passed++;
    } else {
        write_file("/test_results.txt", "  ✗ DELETE request failed\n");
        failed++;
    }
    
    response = http_request("http://httpbin.org/put", "PUT", ([ ]), "test data");
    if (response && response["status"] == 200) {
        write_file("/test_results.txt", "  ✓ PUT request successful\n");
        passed++;
        /* Also check if PUT data was received */
        if (response["body"] && strstr(response["body"], "test data") >= 0) {
            write_file("/test_results.txt", "  ✓ PUT data found in response body\n");
            passed++;
        } else {
            write_file("/test_results.txt", "  ✗ PUT data not found in response\n");
            failed++;
        }
    } else {
        write_file("/test_results.txt", "  ✗ PUT request failed\n");
        failed++;
    }
    
    /* Test 5: Error handling */
    write_file("/test_results.txt", "\nTest 5: Error handling\n");
    response = http_request("http://httpbin.org/status/404", "GET", ([ ]), "");
    if (response && response["status"] == 404) {
        write_file("/test_results.txt", "  ✓ 404 error handled correctly\n");
        passed++;
    } else {
        write_file("/test_results.txt", "  ✗ 404 not handled properly\n");
        failed++;
    }
    
    /* Summary */
    write_file("/test_results.txt", "\n--- HTTP Test Summary ---\n");
    write_file("/test_results.txt", "Passed: " + passed + " tests\n");
    write_file("/test_results.txt", "Failed: " + failed + " tests\n");
    write_file("/test_results.txt", "Total: " + (passed + failed) + " tests\n");
}