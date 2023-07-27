# Set the minimum required version of CMake
cmake_minimum_required(VERSION 3.20)

set(CMAKE_TOOLCHAIN_FILE "toolchain.cmake")

# Set the project name
project(EmbeddedProject CXX C ASM)

add_subdirectory(Drivers/CMSIS)
add_subdirectory(Middlewares/Third_Party/FreeRTOS)
add_subdirectory(Drivers/STM32F3xx_HAL_Driver)
add_subdirectory(Core)
add_subdirectory(Libraries/rtt)
add_subdirectory(Libraries/tf)
add_subdirectory(Proto)
add_subdirectory(Middlewares/ST/STM32_USB_Device_Library)
add_subdirectory(USB_DEVICE)
add_subdirectory(App)
