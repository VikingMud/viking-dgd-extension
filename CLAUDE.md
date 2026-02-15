# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

Viking DGD Extension is a C extension module for DGD (Dworkin's Game Driver) that provides additional kernel functions (kfuns) for Viking MUD. The extension adds cryptographic and system resource monitoring capabilities to LPC code running in DGD.

This repo also wraps the upstream DGD build with Viking-specific compile flags, eliminating the need for a separate `viking-dgd` fork.

## Architecture

### Core Components

**Extension Entry Point (viking_ext.c)**
- Registers all kfuns with DGD via `lpc_ext_init()`
- Initializes libsodium on module load
- Defines function prototypes using DGD's type system (e.g., `LPC_TYPE_STRING`, `LPC_TYPE_INT`)

**DGD Extension Interface (lpc_ext.c, lpc_ext.h)**
- Standard DGD extension interface boilerplate
- Provides `ext_init()` callback that DGD calls when loading the module
- Handles function table initialization and callback registration
- Not modified for new kfuns - all changes go in individual kfun_*.c files

**Kernel Functions**
- `kfun_rusage.c` - System resource usage via `getrusage()`
- `kfun_crypto.c` - Argon2id password hashing using libsodium

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

### LARGENUM

Both DGD and the extension must be compiled with `-DLARGENUM` so that `LPC_int` is `int64_t`. The Makefile handles this for both the `dgd` and `extension` targets.

## Commands

**Build everything (DGD + extension)**
```bash
make
```

**Build only the extension**
```bash
make extension
```
Builds `viking_ext.dylib` (macOS) or `viking_ext.so` (Linux). Checks for libsodium dependency first.

**Build only DGD**
```bash
make dgd
```
Builds upstream DGD (`~/code/dgd/src`) with Viking flags: `-DLARGENUM -DSLASHSLASH -DUINDEX_TYPE=uint32_t -DUINDEX_MAX=UINT32_MAX`.

**Clean**
```bash
make clean      # clean extension objects
make dgd-clean  # clean DGD build
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

Install on macOS: `brew install libsodium`
Install on Ubuntu/Debian: `sudo apt-get install libsodium-dev`

## DGD Configuration

The extension is loaded via DGD config file:
```
modules = ([ "/path/to/viking_ext.so" : "" ]);
```

Test configuration is in `test.dgd` - points to `test/` directory as DGD root.
