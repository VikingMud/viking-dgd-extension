/* Test argon2_hash() and argon2_verify() kfuns */

void test() {
    string password, hash, hash2;
    int result;
    
    write_file("/test_results.txt", "\n--- Testing argon2_hash() and argon2_verify() ---\n");
    
    /* Test 1: Hash a password with default parameters */
    password = "test_password123";
    write_file("/test_results.txt", "Calling argon2_hash with password: " + password + "\n");
    hash = argon2_hash(password, 3, 67108864);  /* Use libsodium defaults */
    
    if (!hash) {
        write_file("/test_results.txt", "ERROR: argon2_hash() returned nil\n");
        return;
    }
    
    write_file("/test_results.txt", "SUCCESS: argon2_hash() returned hash: " + hash + "\n");
    
    /* Test 2: Verify correct password */
    result = argon2_verify(password, hash);
    if (result == 1) {
        write_file("/test_results.txt", "SUCCESS: argon2_verify() correctly verified password\n");
    } else {
        write_file("/test_results.txt", "ERROR: argon2_verify() failed to verify correct password\n");
    }
    
    /* Test 3: Verify incorrect password */
    result = argon2_verify("wrong_password", hash);
    if (result == 0) {
        write_file("/test_results.txt", "SUCCESS: argon2_verify() correctly rejected wrong password\n");
    } else {
        write_file("/test_results.txt", "ERROR: argon2_verify() incorrectly accepted wrong password\n");
    }
    
    /* Test 4: Hash with custom parameters */
    hash2 = argon2_hash(password, 2, 65536);  /* opsLimit=2, memLimit=65536 */
    
    if (!hash2) {
        write_file("/test_results.txt", "ERROR: argon2_hash() with custom params returned nil\n");
    } else {
        write_file("/test_results.txt", "SUCCESS: argon2_hash() with custom params returned hash: " + hash2 + "\n");
        
        /* Verify the custom hash */
        result = argon2_verify(password, hash2);
        if (result == 1) {
            write_file("/test_results.txt", "SUCCESS: argon2_verify() verified custom param hash\n");
        } else {
            write_file("/test_results.txt", "ERROR: argon2_verify() failed on custom param hash\n");
        }
    }
    
    /* Test 5: Different passwords should produce different hashes */
    if (hash != hash2) {
        write_file("/test_results.txt", "SUCCESS: Different parameters produced different hashes\n");
    } else {
        write_file("/test_results.txt", "WARNING: Same password with different params produced same hash\n");
    }
}