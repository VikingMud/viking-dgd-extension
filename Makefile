# Viking DGD Extension Makefile

# Detect OS
UNAME_S := $(shell uname -s)

# Compiler settings
CC = cc
CXX = c++
CFLAGS = -fPIC -O2 -Wall -DLARGENUM -I. $(shell pkg-config --cflags libsodium)
CXXFLAGS = $(CFLAGS) -std=c++11

# Platform-specific settings
ifeq ($(UNAME_S),Darwin)
    LIBEXT = .dylib
    LDFLAGS = -dynamiclib -undefined dynamic_lookup
else
    LIBEXT = .so
    LDFLAGS = -shared
endif

# Library flags (must come after object files for --as-needed)
LDLIBS = $(shell pkg-config --libs libsodium)

# Target
TARGET = viking_ext$(LIBEXT)

# Source files
SOURCES = lpc_ext.c viking_ext.c kfun_rusage.c kfun_crypto.c

# Object files
OBJECTS = $(SOURCES:.c=.o)

# Install directory (override with INSTALL_DIR=/path)
INSTALL_DIR ?= $(HOME)/viking-mud

# --- Viking DGD build (from upstream dgd source) ---
DGD_SRC ?= $(HOME)/code/dgd/src
DGD_DEFINES = -DLARGENUM -DSLASHSLASH '-DUINDEX_TYPE=uint32_t' '-DUINDEX_MAX=UINT32_MAX'
DGD_DEBUG = -g

# --- Targets ---

all: dgd extension

# Build upstream DGD with Viking flags
dgd:
	$(MAKE) -C $(DGD_SRC) DEFINES="$(DGD_DEFINES)" DEBUG="$(DGD_DEBUG)" install

dgd-clean:
	$(MAKE) -C $(DGD_SRC) clean

# Build the extension shared library
extension: check-deps $(TARGET)

check-deps:
	@pkg-config --exists libsodium || (echo "Error: libsodium not found. Please install it." && exit 1)

$(TARGET): $(OBJECTS)
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)

install: dgd extension
	cp $(DGD_SRC)/../bin/dgd $(INSTALL_DIR)/bin/dgd
	cp $(TARGET) $(INSTALL_DIR)/bin/viking_ext$(LIBEXT)

.PHONY: all dgd dgd-clean extension check-deps clean install
