cmake_minimum_required(VERSION 3.20)
project(usb_device_lib C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

file(GLOB_RECURSE USB_DEVICE_LIB_SOURCES "${CMAKE_CURRENT_LIST_DIR}/*.c")

# Create the static library target
add_library(usb_device_lib STATIC ${USB_DEVICE_LIB_SOURCES})

# Set the include directories for the usb_device_lib library
target_include_directories(usb_device_lib PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/Core/Inc
    ${CMAKE_CURRENT_LIST_DIR}/Class/CDC/Inc
)

target_link_libraries(usb_device_lib PRIVATE
    cmsis_driver
    hal_driver
    cubemx_core
    usb_device
)
