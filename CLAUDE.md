# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

Viking DGD Extension is a C extension module for DGD (Dworkin's Game Driver) that provides additional kernel functions (kfuns) for Viking MUD. The extension adds cryptographic, HTTP, and system resource monitoring capabilities to LPC code running in DGD.

## Architecture

### Core Components

**Extension Entry Point (viking_ext.c)**
- Registers all kfuns with DGD via `lpc_ext_init()`
- Initializes libsodium and CURL on module load
- Defines function prototypes using DGD's type system (e.g., `LPC_TYPE_STRING`, `LPC_TYPE_INT`)

**DGD Extension Interface (lpc_ext.c, lpc_ext.h)**
- Standard DGD extension interface boilerplate
- Provides `ext_init()` callback that DGD calls when loading the module
- Handles function table initialization and callback registration
- Not modified for new kfuns - all changes go in individual kfun_*.c files

**Kernel Functions**
- `kfun_rusage.c` - System resource usage via `getrusage()`
- `kfun_crypto.c` - Argon2id password hashing using libsodium
- `kfun_http.c` - HTTP client using libcurl

### Adding New Kfuns

To add a new kfun:
1. Create `kfun_<name>.c` with function implementation
2. Add function declaration to `viking_ext.h`
3. Add prototype string and entry to `viking_kfuns[]` table in `viking_ext.c`
4. Update `SOURCES` in `Makefile`

### DGD Type System

Prototypes use DGD type constants:
- `LPC_TYPE_INT`, `LPC_TYPE_STRING`, `LPC_TYPE_MAPPING`
- `LPC_TYPE_ARRAY_OF(type)` for arrays
- Prototype arrays must be null-terminated

## Commands

**Build**
```bash
make
```
Builds `viking_ext.dylib` (macOS) or `viking_ext.so` (Linux). Checks for libsodium and libcurl dependencies first.

**Clean**
```bash
make clean
```

**Run Tests**
```bash
export DGD_BIN=/path/to/dgd  # Optional if dgd is in PATH
./run_tests.sh
```
- Tests run in background DGD instance using `test.dgd` config
- Results written to `test/test_results.txt`
- DGD loads `test/obj/driver.c` which calls test objects
- Tests located in `test/test_*.c` (LPC code)

**Run Single Test**
Modify `test/obj/driver.c` to comment out unneeded `call_other()` lines, then run `./run_tests.sh`

## Dependencies

- **libsodium** - Argon2 password hashing
- **libcurl** - HTTP requests
- **DGD** - Game driver (test runner requires DGD binary)

Install on macOS: `brew install libsodium curl`
Install on Ubuntu/Debian: `sudo apt-get install libsodium-dev libcurl4-openssl-dev`

## DGD Configuration

The extension is loaded via DGD config file:
```
modules = ([ "/path/to/viking_ext.dylib" : "" ]);
```

Test configuration is in `test.dgd` - points to `test/` directory as DGD root.
