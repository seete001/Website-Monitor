#!/bin/bash

# To stopp the script when program stops or crashes
set -e

SERVICENAME="website-monitor"
SERVICEDIR="$HOME/.config/systemd/user"


echo "Installing dependencies..."
sudo apt update
sudo apt install -y g++ libcurl4-openssl-dev

echo "Creating project directories..."
mkdir -p bin logs

echo "Compiling the project..."
g++ -std=c++17 -Wall -Wextra -Wpendantic \  
    -I include src/*cpp \
    -o bin/$SERVICENAME \
    -lcurl -pthread

echo "Creating systemd service..."
mkdir -p "$SERVICEDIR"
cp website-monitor.service "SERVICEDIR/"

echo "Reloading systemd..."
systemctl --user daemon-reload

echo "Enabling and starting the service..."
systemctl --user enable--now "$SERVICENAME"

echo "Installation successfuly completed!"
systemctl --user status "$SERVICENAME" --no-pager