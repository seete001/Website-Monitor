# Website Monitor

A lightweight C++ website monitoring tool that periodically checks website availability using libcurl and logs the results. It runs as a systemd user service and automatically restarts if it crashes. Uni Project!

## Features

- Periodically checks website availability.
- Uses HTTP status codes to determine whether a website is reachable.
- Logs monitoring results with timestamps.
- Runs in the background as a systemd service.
- Automatically restarts after unexpected failures.
- Supports graceful shutdown.

## Requirements

- Ubuntu/Debian Linux
- C++17-compatible compiler (`g++`)
- libcurl development library
- systemd

The installation script installs the required compilation dependencies.

## Installation

Clone or download the repository, then enter the project directory:

cd Website-monitor


Make the installation script executable:

chmod +x install.sh


Run the installer:

./install.sh


The installer compiles the application, generates the systemd service file using the project's actual installation path, and enables and starts the service.

## Configuration

Edit `config.conf` to configure the website, request timeout, monitoring interval, and log file.

Example:

url=https://google.com 
timeout=10 
intervalSeconds=60 
log_file=logs/website.log


Use the parameter names and format expected by your version of `Config.cpp`.

Restart the service after changing the configuration:

systemctl --user restart website-monitor


## Service Management

Check the service status:

systemctl --user status website-monitor


Stop monitoring:

systemctl --user stop website-monitor


Start monitoring:

systemctl --user start website-monitor


Restart the service:

systemctl --user restart website-monitor


Enable automatic startup for your user service:

systemctl --user enable website-monitor


## Logs

View the monitoring log:

tail -f logs/website.log


View service logs:

journalctl --user -u website-monitor -f


## Project Structure
``` text
Website-monitor/ 
├── include/ 			# Header files 
├── src/ 		        # C++ source files 
│   │ 
│   ├── main.cpp 
│   │ 
│   ├── Config.cpp 
│   │ 
│   ├── CurlGlobal.cpp 
│   │ 
│   ├── Logger.cpp 
│   │ 
│   └── WebsiteChecker.cpp 
├── bin/ 	        	# Compiled executable 
├── logs/ 			# Monitoring logs 
├── config.conf 		# Monitoring configuration 
├── website-monitor.service 	# Service template 
├── install.sh 			# Installation script 
└── README.md
```
