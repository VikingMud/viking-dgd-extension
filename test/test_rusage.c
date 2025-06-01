/* Test rusage() kfun */

void test() {
    mixed *usage;
    int i;
    string result;
    
    write_file("/test_results.txt", "\n--- Testing rusage() ---\n");
    
    /* Call rusage() */
    usage = rusage();
    
    if (!usage) {
        write_file("/test_results.txt", "ERROR: rusage() returned nil\n");
        return;
    }
    
    if (sizeof(usage) != 14) {
        write_file("/test_results.txt", "ERROR: rusage() returned array of size " + sizeof(usage) + ", expected 14\n");
        return;
    }
    
    write_file("/test_results.txt", "SUCCESS: rusage() returned array of 14 elements\n");
    
    /* Display the values */
    write_file("/test_results.txt", "User time: " + usage[0] + " seconds, " + usage[1] + " microseconds\n");
    write_file("/test_results.txt", "System time: " + usage[2] + " seconds, " + usage[3] + " microseconds\n");
    write_file("/test_results.txt", "Max RSS: " + usage[4] + "\n");
    write_file("/test_results.txt", "Page faults (no I/O): " + usage[8] + "\n");
    write_file("/test_results.txt", "Page faults (I/O): " + usage[9] + "\n");
    write_file("/test_results.txt", "Voluntary context switches: " + usage[12] + "\n");
    write_file("/test_results.txt", "Involuntary context switches: " + usage[13] + "\n");
}