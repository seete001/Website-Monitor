#!/bin/bash

# To stopp the script when program stops or crashes
set -e

# Create bin dir if does not exist
mkdir -p bin

# Compile the project
g++ -std=c++17 -Wall -Wextra -Wpedantic \
    -Iinclude src/*.cpp \
    -o bin/website-monitor \
    -lcurl -pthread

# Run the program
exec ./bin/website-monitor
