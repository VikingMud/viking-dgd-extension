/* Minimal driver object - only contains required functions */

void initialize() {
    write_file("/test_results.txt", "Driver initialized\n");
    write_file("/test_results.txt", "\n=== Testing Viking Extension ===\n");
    
    /* Run tests */
    call_other("/test_rusage", "test");
    call_other("/test_crypto", "test");
    call_other("/test_http", "test");
    
    write_file("/test_results.txt", "\n=== All tests completed ===\n");
}

/* Required by DGD for call_other to work */
object call_object(string path) {
    return compile_object(path);
}