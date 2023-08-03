cmake_minimum_required(VERSION 3.20)
project(freertos C ASM)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

file(GLOB_RECURSE ${PROJECT_NAME}_SOURCES "${CMAKE_CURRENT_LIST_DIR}/*.c")

# Create the static library target
add_library(${PROJECT_NAME} STATIC ${${PROJECT_NAME}_SOURCES})

# Set the include directories for the usb_device library
target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/Source/include
    ${CMAKE_CURRENT_LIST_DIR}/Source/CMSIS_RTOS_V2
    ${CMAKE_CURRENT_LIST_DIR}/Source/portable/GCC/ARM_CM4F
    ${CMAKE_CURRENT_LIST_DIR}/Source/portable/MemMang
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    cmsis_driver
    cubemx_core
)
