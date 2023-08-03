cmake_minimum_required(VERSION 3.20)
project(cmsis_driver C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_library(cmsis_driver INTERFACE)

target_include_directories(cmsis_driver INTERFACE 
    ${CMAKE_CURRENT_LIST_DIR}/Include
    ${CMAKE_CURRENT_LIST_DIR}/Device/ST/STM32F3xx/Include
)
