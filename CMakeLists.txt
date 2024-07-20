cmake_minimum_required(VERSION 3.11)

project(
        DC20_CRC8
        VERSION 0.1
        DESCRIPTION "DC20_CRC8"
        LANGUAGES C
)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Set the output directory for executable files
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_executable(main main.c)
