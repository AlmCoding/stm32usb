cmake_minimum_required(VERSION 3.20)
project(usb_device C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

file(GLOB_RECURSE ${PROJECT_NAME}_SOURCES "${CMAKE_CURRENT_LIST_DIR}/*.c")

# Create the static library target
add_library(${PROJECT_NAME} STATIC ${${PROJECT_NAME}_SOURCES})

# Set the include directories for the usb_device library
target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/App
    ${CMAKE_CURRENT_LIST_DIR}/Target
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/App
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    cmsis_driver
    hal_driver
    cubemx_core
    tf
    usb_device_lib
)

add_custom_command(TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMAND ${ARM_NONE_EABI_OBJDUMP} -h -S lib${PROJECT_NAME}.a > ${PROJECT_NAME}.list
)
