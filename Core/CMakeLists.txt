cmake_minimum_required(VERSION 3.20)
project(cubemx_core C ASM)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

#message("CMAKE_CURRENT_LIST_DIR = ${CMAKE_CURRENT_LIST_DIR}")
#message("CMAKE_SOURCE_DIR = ${CMAKE_SOURCE_DIR}")

file(GLOB CUBEMX_CORE_SOURCES "${CMAKE_CURRENT_LIST_DIR}/Src/*.c")
set(CUBEMX_CORE_STARTUP "${CMAKE_CURRENT_LIST_DIR}/Startup/startup_stm32f303vctx.s")

# Create the static library target
add_library(cubemx_core STATIC ${CUBEMX_CORE_SOURCES} ${CUBEMX_CORE_STARTUP})

# Set the include directories for the cubemx_core library
target_include_directories(cubemx_core PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/Inc
)

target_link_libraries(cubemx_core PRIVATE
    cmsis_driver
    hal_driver
    freertos
    usb_device_lib
    usb_device
)
