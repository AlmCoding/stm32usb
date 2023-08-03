cmake_minimum_required(VERSION 3.20)
project(cubemx_core C ASM)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

file(GLOB  ${PROJECT_NAME}_SOURCES  "${CMAKE_CURRENT_LIST_DIR}/Src/*.c")
set( ${PROJECT_NAME}_STARTUP "${CMAKE_CURRENT_LIST_DIR}/Startup/startup_stm32f303vctx.s")

# Create the static library target
add_library(${PROJECT_NAME} STATIC ${${PROJECT_NAME}_SOURCES} ${${PROJECT_NAME}_STARTUP})

# Set the include directories for the cubemx_core library
target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/Inc
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    cmsis_driver
    hal_driver
    freertos
    usb_device_lib
    usb_device
)

add_custom_command(TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMAND ${ARM_NONE_EABI_OBJDUMP} -h -S lib${PROJECT_NAME}.a > ${PROJECT_NAME}.list
)
