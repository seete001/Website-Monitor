#!/bin/bash

# Stop if any command fails
set -e

# Project configuration
SERVICENAME="website-monitor"
PROJECT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SERVICEDIR="$HOME/.config/systemd/user"
SERVICE_FILE="$SERVICEDIR/$SERVICENAME.service"

echo "========================================"
echo "       Website Monitor Installer"
echo "========================================"

echo
echo "Project directory: $PROJECT_DIR"
echo "Service directory: $SERVICEDIR"

# 1. Install dependencies
echo
echo "[1/6] Installing dependencies..."

sudo apt update
sudo apt install -y g++ libcurl4-openssl-dev

# 2. Create project directories
echo
echo "[2/6] Creating project directories..."

mkdir -p "$PROJECT_DIR/bin"
mkdir -p "$PROJECT_DIR/logs"

# 3. Compile the project
echo
echo "[3/6] Compiling the project..."

if ! compgen -G "$PROJECT_DIR/src/*.cpp" > /dev/null; then
    echo "ERROR: No C++ source files found in $PROJECT_DIR/src/"
    exit 1
fi

g++ -std=c++17 -Wall -Wextra -Wpedantic \
    -I "$PROJECT_DIR/include" \
    "$PROJECT_DIR"/src/*.cpp \
    -o "$PROJECT_DIR/bin/$SERVICENAME" \
    -lcurl -pthread

echo "Compilation successful!"

# 4. Generate the systemd service file
echo
echo "[4/6] Creating systemd service..."

mkdir -p "$SERVICEDIR"

cat > "$SERVICE_FILE" <<EOF
[Unit]
Description=Website Monitor
After=network-online.target

[Service]
Type=simple
WorkingDirectory=$PROJECT_DIR
ExecStart=$PROJECT_DIR/bin/$SERVICENAME
Restart=on-failure
RestartSec=5

[Install]
WantedBy=default.target
EOF

echo "Service file created at:"
echo "$SERVICE_FILE"

# 5. Reload systemd and enable the service
echo
echo "[5/6] Configuring systemd..."

systemctl --user daemon-reload
systemctl --user enable "$SERVICENAME"

# 6. Start the service
echo
echo "[6/6] Starting Website Monitor..."

systemctl --user restart "$SERVICENAME"

echo
echo "========================================"
echo " Installation completed successfully!"
echo "========================================"

echo
echo "Service status:"
systemctl --user status "$SERVICENAME" --no-pager -l || true

echo
echo "Useful commands:"
echo "  Check status: systemctl --user status $SERVICENAME"
echo "  View logs:    journalctl --user -u $SERVICENAME -f"
echo "  Stop:         systemctl --user stop $SERVICENAME"
echo "  Restart:      systemctl --user restart $SERVICENAME"
echo "  Disable:      systemctl --user disable --now $SERVICENAME"
