cmake_minimum_required(VERSION 3.20)
project(freertos C ASM)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

#message("CMAKE_CURRENT_LIST_DIR = ${CMAKE_CURRENT_LIST_DIR}")
#message("CMAKE_SOURCE_DIR = ${CMAKE_SOURCE_DIR}")

file(GLOB_RECURSE FREERTOS_SOURCES "${CMAKE_CURRENT_LIST_DIR}/*.c")

# Create the static library target
add_library(freertos STATIC ${FREERTOS_SOURCES})

# Set the include directories for the usb_device library
target_include_directories(freertos PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/Source/include
    ${CMAKE_CURRENT_LIST_DIR}/Source/CMSIS_RTOS_V2
    ${CMAKE_CURRENT_LIST_DIR}/Source/portable/GCC/ARM_CM4F
    ${CMAKE_CURRENT_LIST_DIR}/Source/portable/MemMang
)

#target_include_directories(freertos PRIVATE
#    ${CMAKE_SOURCE_DIR}/Core/Inc
#)

target_link_libraries(freertos PRIVATE
    cmsis_driver
    cubemx_core
)

