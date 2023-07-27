cmake_minimum_required(VERSION 3.20)
project(tf C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

#message("CMAKE_CURRENT_LIST_DIR = ${CMAKE_CURRENT_LIST_DIR}")
#message("CMAKE_SOURCE_DIR = ${CMAKE_SOURCE_DIR}")

set(TF_SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/TinyFrame.c
    ${CMAKE_CURRENT_LIST_DIR}/utilities/payload_builder.c
    ${CMAKE_CURRENT_LIST_DIR}/utilities/payload_parser.c
)

include_directories(demo/simple)

add_library(tf STATIC ${TF_SOURCES})

# Set the include directories for the hal_driver library
target_include_directories(tf PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/.
)

target_include_directories(tf PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/utilities
    ${CMAKE_SOURCE_DIR}/Libraries
)