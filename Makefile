# Viking DGD Extension Makefile

# Detect OS
UNAME_S := $(shell uname -s)

# Compiler settings
CC = cc
CXX = c++
CFLAGS = -fPIC -O2 -Wall -I. $(shell pkg-config --cflags libsodium libcurl)
CXXFLAGS = $(CFLAGS) -std=c++11

# Platform-specific settings
ifeq ($(UNAME_S),Darwin)
    LIBEXT = .dylib
    LDFLAGS = -dynamiclib -undefined dynamic_lookup
else
    LIBEXT = .so
    LDFLAGS = -shared
endif

# Add library flags
LDFLAGS += $(shell pkg-config --libs libsodium libcurl)

# Target
TARGET = viking_ext$(LIBEXT)

# Source files
SOURCES = lpc_ext.c viking_ext.c kfun_rusage.c kfun_crypto.c kfun_http.c

# Object files
OBJECTS = $(SOURCES:.c=.o)

# Build rules
all: check-deps $(TARGET)

check-deps:
	@pkg-config --exists libsodium || (echo "Error: libsodium not found. Please install it." && exit 1)
	@pkg-config --exists libcurl || (echo "Error: libcurl not found. Please install it." && exit 1)

$(TARGET): $(OBJECTS)
	$(CC) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)

install: $(TARGET)
	@echo "Extension built successfully: $(TARGET)"
	@echo "Add the following to your DGD config file:"
	@echo "    modules = ([ \"$(shell pwd)/$(TARGET)\" : \"\" ]);"

.PHONY: all clean install check-deps