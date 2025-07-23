cmake_minimum_required(VERSION 3.11)

project(
        DC20_CRC8
        VERSION 0.1
        DESCRIPTION "DC20_CRC8"
        LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set the output directory for executable files
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_executable(main main.cpp)
