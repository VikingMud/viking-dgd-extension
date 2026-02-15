# Viking DGD Extension

A DGD extension module that provides additional kernel functions (kfuns) for Viking MUD.

## Kfuns

| Kfun | Prototype | Description |
|------|-----------|-------------|
| `rusage()` | `int*` (14-element array) | System resource usage via `getrusage()` |
| `argon2_hash(string, int, int)` | returns `string` | Argon2id password hashing |
| `argon2_verify(string, string)` | returns `int` | Argon2id password verification |

## Requirements

- [DGD](https://github.com/dworkin/dgd) (Dworkin's Game Driver)
- libsodium (`libsodium-dev` on Debian/Ubuntu, `libsodium` on macOS via Homebrew)
- C compiler with C99 support

## Viking MUD Installation

### 1. Build DGD with Viking flags

This extension repo includes a Makefile target that builds upstream DGD with all flags Viking needs. No source patches or fork required.

```bash
# Clone upstream DGD (if not already present)
git clone https://github.com/dworkin/dgd.git ~/code/dgd

# Build DGD with Viking flags
cd ~/code/viking-dgd-extension
make dgd
```

This runs `make` in the upstream DGD source with these defines:

| Flag | Purpose |
|------|---------|
| `-DLARGENUM` | 64-bit LPC integers (default is 32-bit) |
| `-DSLASHSLASH` | Allow `//` comments in LPC source |
| `-DUINDEX_TYPE=uint32_t` | 4-byte object index (required by LARGENUM) |
| `-DUINDEX_MAX=UINT32_MAX` | Max value for 4-byte object index |

The output binary is at `~/code/dgd/bin/dgd`. Set `DGD_SRC` to override the DGD source location:

```bash
make dgd DGD_SRC=/path/to/dgd/src
```

### 2. Build the extension

```bash
make extension
```

This creates `viking_ext.so` (Linux) or `viking_ext.dylib` (macOS).

The extension **must** be compiled with `-DLARGENUM` to match DGD's 64-bit integer size. The Makefile handles this automatically.

### 3. Install

```bash
cp ~/code/dgd/bin/dgd ~/viking-mud/bin/dgd
cp viking_ext.so ~/viking-mud/bin/
```

Or use the install target:

```bash
make install INSTALL_DIR=~/viking-mud
```

### 4. Configure DGD

Add the modules line to your DGD config file (e.g. `etc/config.viking`):

```
modules = ([ "/usr/local/viking/mud/bin/viking_ext.so" : "" ]);
```

### 5. Server file layout

```
/usr/local/viking/mud/
├── bin/
│   ├── dgd              # DGD binary (built with Viking flags)
│   └── viking_ext.so    # Extension module
├── etc/config.viking    # DGD config (references extension via modules)
└── lib/                 # MUD library
```

## Docker Development

The `viking-ansible` repo has Dockerfile and docker-compose.yml configured for local development. The extension source is mounted read-only into the container at `/home/drake/viking-ext`.

To build the extension inside the container (avoids libsodium version mismatch):

```bash
docker exec viking-local bash -c 'cd /home/drake/viking-ext && make clean && make extension'
docker exec viking-local bash -c 'cp /home/drake/viking-ext/viking_ext.so /usr/local/viking/mud/bin/'
```

## Testing

```bash
# Set DGD binary path (or add dgd to your PATH)
export DGD_BIN=/path/to/dgd

# Build extension first
make extension

# Run tests
./run_tests.sh
```

Tests run a DGD instance with `test.dgd` config, execute LPC test scripts, and write results to `test/test_results.txt`.

## API Reference

### rusage()

Returns a 14-element `int*` array with system resource usage (values in milliseconds for CPU times):

| Index | Field |
|-------|-------|
| `[0]` | User CPU time (ms) |
| `[1]` | System CPU time (ms) |
| `[2]` | Maximum resident set size (KB) |
| `[3]` | Integral shared memory size |
| `[4]` | Page reclaims (soft page faults) |
| `[5]` | Page faults (hard page faults) |
| `[6]` | Swaps |
| `[7]` | Block input operations |
| `[8]` | Block output operations |
| `[9]` | Messages sent |
| `[10]` | Messages received |
| `[11]` | Signals received |
| `[12]` | Voluntary context switches |
| `[13]` | Involuntary context switches |

### argon2_hash(string password, int ops_limit, int mem_limit)

Hashes a password using Argon2id (via libsodium).

- `password` - The password to hash
- `ops_limit` - Operations limit (e.g., 3 for interactive, 4 for moderate)
- `mem_limit` - Memory limit in bytes (e.g., 67108864 for 64MB)

Returns a string containing the encoded Argon2id hash, or `nil` on failure.

### argon2_verify(string password, string hash)

Verifies a password against an Argon2id hash.

- `password` - The password to verify
- `hash` - The encoded Argon2id hash string

Returns `1` if the password matches, `0` otherwise.

## License

This project is licensed under the same terms as DGD.
