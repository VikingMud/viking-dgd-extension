# Viking DGD Extension

A DGD extension module that provides additional kernel functions (kfuns) for Viking MUD.

## Features

This extension provides the following kfuns:

- **`rusage()`** - Returns system resource usage information as a 14-element array
- **`argon2_hash(string password, int ops_limit, int mem_limit)`** - Hash passwords using Argon2id
- **`argon2_verify(string password, string hash)`** - Verify passwords against Argon2id hashes
- **`http_request(string url, string method, mapping headers, string body)`** - Make HTTP requests

## Requirements

- DGD (Dworkin's Game Driver)
- libsodium (for Argon2 password hashing)
- libcurl (for HTTP requests)
- C compiler with C99 support

## Building

1. Install dependencies:
   ```bash
   # macOS
   brew install libsodium curl

   # Ubuntu/Debian
   sudo apt-get install libsodium-dev libcurl4-openssl-dev
   ```

2. Build the extension:
   ```bash
   make
   ```

   This will create `viking_ext.dylib` (macOS) or `viking_ext.so` (Linux).

## Usage

1. Add the extension to your DGD configuration file:
   ```
   modules = ([ "/path/to/viking_ext" : "" ]);
   ```

2. Use the kfuns in your LPC code:
   ```lpc
   /* Get system resource usage */
   mixed *usage = rusage();
   write("User CPU time: " + usage[0] + " seconds\n");

   /* Hash a password */
   string hash = argon2_hash("mypassword", 3, 67108864);
   
   /* Verify a password */
   if (argon2_verify("mypassword", hash)) {
       write("Password correct!\n");
   }

   /* Make an HTTP request */
   mapping response = http_request("https://api.example.com/data", "GET", ([ ]), "");
   write("Status: " + response["status"] + "\n");
   ```

## Testing

Run the test suite:
```bash
# Set DGD binary path (or add dgd to your PATH)
export DGD_BIN=/path/to/dgd

# Run tests
./run_tests.sh
```

See [test/test_results.txt](test/test_results.txt) for example test output.

## API Reference

### rusage()
Returns an array with 14 elements containing system resource usage:
- `[0]` - User CPU time (seconds)
- `[1]` - User CPU time (microseconds)
- `[2]` - System CPU time (seconds)
- `[3]` - System CPU time (microseconds)
- `[4]` - Maximum resident set size
- `[5]` - Integral shared memory size
- `[6]` - Integral unshared data size
- `[7]` - Integral unshared stack size
- `[8]` - Page reclaims (soft page faults)
- `[9]` - Page faults (hard page faults)
- `[10]` - Swaps
- `[11]` - Block input operations
- `[12]` - Block output operations
- `[13]` - Voluntary context switches
- `[14]` - Involuntary context switches

### argon2_hash(string password, int ops_limit, int mem_limit)
Hashes a password using Argon2id.
- `password` - The password to hash
- `ops_limit` - Operations limit (e.g., 3 for interactive, 4 for moderate)
- `mem_limit` - Memory limit in bytes (e.g., 67108864 for 64MB)

Returns a string containing the Argon2id hash.

### argon2_verify(string password, string hash)
Verifies a password against an Argon2id hash.
- `password` - The password to verify
- `hash` - The Argon2id hash to verify against

Returns 1 if the password matches, 0 otherwise.

### http_request(string url, string method, mapping headers, string body)
Makes an HTTP request.
- `url` - The URL to request
- `method` - HTTP method (GET, POST, PUT, DELETE, HEAD)
- `headers` - Mapping of request headers
- `body` - Request body (for POST/PUT)

Returns a mapping with:
- `status` - HTTP status code
- `headers` - Response headers as a mapping
- `body` - Response body as a string

## License

This project is licensed under the same terms as DGD.