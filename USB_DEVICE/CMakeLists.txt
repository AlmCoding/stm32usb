cmake_minimum_required(VERSION 3.20)
project(usb_device C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

#message("CMAKE_CURRENT_LIST_DIR = ${CMAKE_CURRENT_LIST_DIR}")
#message("CMAKE_SOURCE_DIR = ${CMAKE_SOURCE_DIR}")

file(GLOB_RECURSE USB_DEVICE_SOURCES "${CMAKE_CURRENT_LIST_DIR}/*.c")

# Create the static library target
add_library(usb_device STATIC ${USB_DEVICE_SOURCES})

# Set the include directories for the usb_device library
target_include_directories(usb_device PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/App
    ${CMAKE_CURRENT_LIST_DIR}/Target
)

target_include_directories(usb_device PRIVATE
    ${CMAKE_SOURCE_DIR}/App
)

target_link_libraries(usb_device PRIVATE
    cmsis_driver
    hal_driver
    cubemx_core
    tf
    usb_device_lib
)
