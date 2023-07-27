cmake_minimum_required(VERSION 3.20)
project(hal_driver C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

#message("CMAKE_CURRENT_LIST_DIR = ${CMAKE_CURRENT_LIST_DIR}")
#message("CMAKE_SOURCE_DIR = ${CMAKE_SOURCE_DIR}")

file(GLOB HAL_DRIVER_SOURCES "${CMAKE_CURRENT_LIST_DIR}/Src/*.c")
#message("HAL_DRIVER_SOURCES = ${HAL_DRIVER_SOURCES}")

# Create the static library target
add_library(hal_driver STATIC ${HAL_DRIVER_SOURCES})

# Set the include directories for the hal_driver library
target_include_directories(hal_driver PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/Inc
    ${CMAKE_SOURCE_DIR}/Drivers/CMSIS/Device/ST/STM32F3xx/Include
    ${CMAKE_SOURCE_DIR}/Drivers/CMSIS/Include
)

# Set the include directories for the hal_driver library
#target_include_directories(hal_driver PRIVATE
#    ${CMAKE_SOURCE_DIR}/Core/Inc
#)

target_link_libraries(hal_driver PRIVATE
    cmsis_driver
    cubemx_core
)
