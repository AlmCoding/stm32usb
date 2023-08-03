cmake_minimum_required(VERSION 3.20)
project(hal_driver C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

file(GLOB ${PROJECT_NAME}_SOURCES "${CMAKE_CURRENT_LIST_DIR}/Src/*.c")
message("HAL_DRIVER_SOURCES = ${HAL_DRIVER_SOURCES}")

# Create the static library target
add_library(${PROJECT_NAME} STATIC ${${PROJECT_NAME}_SOURCES})

# Set the include directories for the hal_driver library
target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/Inc
    ${CMAKE_SOURCE_DIR}/Drivers/CMSIS/Device/ST/STM32F3xx/Include
    ${CMAKE_SOURCE_DIR}/Drivers/CMSIS/Include
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    cmsis_driver
    cubemx_core
)

add_custom_command(TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMAND ${ARM_NONE_EABI_OBJDUMP} -h -S lib${PROJECT_NAME}.a > ${PROJECT_NAME}.list
)
