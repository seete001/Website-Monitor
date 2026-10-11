cmake_minimum_required(VERSION 3.16)

project(WebsiteMonitor LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

add_executable(website-monitor
    src/main.cpp
    src/Config.cpp
)