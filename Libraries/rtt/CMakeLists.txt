cmake_minimum_required(VERSION 3.20)
project(rtt C ASM)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

#message("CMAKE_CURRENT_LIST_DIR = ${CMAKE_CURRENT_LIST_DIR}")
#message("CMAKE_SOURCE_DIR = ${CMAKE_SOURCE_DIR}")

# Add the source files for rtt
set(RTT_SOURCES 
    ${CMAKE_CURRENT_LIST_DIR}/RTT/SEGGER_RTT.c
    ${CMAKE_CURRENT_LIST_DIR}/RTT/SEGGER_RTT_printf.c
)

# Create the static library target
add_library(rtt STATIC ${RTT_SOURCES})

# Set the include directories for the rtt library
target_include_directories(rtt PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/.
    ${CMAKE_CURRENT_LIST_DIR}/Config
    #${CMAKE_SOURCE_DIR}/Libraries
)
