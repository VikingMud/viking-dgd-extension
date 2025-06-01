#!/bin/bash

# Viking DGD Extension Test Runner

echo "=== Viking DGD Extension Test Runner ==="

# Check if DGD binary is specified or in PATH
if [ -z "$DGD_BIN" ]; then
    DGD_BIN=$(which dgd)
    if [ -z "$DGD_BIN" ]; then
        echo "ERROR: DGD binary not found"
        echo "Please set DGD_BIN environment variable or add dgd to your PATH"
        exit 1
    fi
fi

if [ ! -f "$DGD_BIN" ]; then
    echo "ERROR: DGD binary not found at $DGD_BIN"
    exit 1
fi

# Check if extension module exists
if [ ! -f "viking_ext.dylib" ] && [ ! -f "viking_ext.so" ]; then
    echo "ERROR: Viking extension module not found"
    echo "Please run 'make' first to build the extension"
    exit 1
fi

# Create necessary directories
mkdir -p test/tmp

# Clean up any previous test results
rm -f test/test_results.txt

# Run DGD with test configuration
echo "Starting DGD with test configuration..."
echo "Test results will be written to test/test_results.txt"
echo ""

# Run DGD in the background
$DGD_BIN test.dgd &
DGD_PID=$!

# Give DGD time to start and run tests
sleep 3

# Kill DGD
kill $DGD_PID 2>/dev/null

# Display test results
if [ -f "test/test_results.txt" ]; then
    echo "=== Test Results ==="
    cat test/test_results.txt
else
    echo "ERROR: No test results found"
    echo "DGD may have failed to start or load the extension"
fi