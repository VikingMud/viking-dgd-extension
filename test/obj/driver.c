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

/* Called when compilation errors occur */
void compile_error(string file, int line, string err) {
    write_file("/test_results.txt", "Compile error in " + file + ":" + line + " - " + err + "\n");
}

/* Called when runtime errors occur */
void runtime_error(string err, int caught, mixed *trace) {
    write_file("/test_results.txt", "Runtime error: " + err + "\n");
    if (!caught) {
        int i;
        for (i = 0; i < sizeof(trace); i++) {
            write_file("/test_results.txt", "  " + trace[i][0] + ":" + trace[i][1] + " in " + trace[i][2] + "\n");
        }
    }
}

/* Called when DGD receives an interrupt signal */
void interrupt() {
    /* Clean shutdown - tests are already complete */
}