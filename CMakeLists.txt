# Set the minimum required version of CMake
cmake_minimum_required(VERSION 3.20)

set(CMAKE_TOOLCHAIN_FILE "toolchain.cmake")

project(stm C ASM)
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

#message("CMAKE_CURRENT_LIST_DIR = ${CMAKE_CURRENT_LIST_DIR}")
#message("CMAKE_SOURCE_DIR = ${CMAKE_SOURCE_DIR}")

# Core
file(GLOB ${PROJECT_NAME}_CORE_SOURCES "${CMAKE_SOURCE_DIR}/Core/Src/*.c")
message("${PROJECT_NAME}_CORE_SOURCES = ${${PROJECT_NAME}_CORE_SOURCES}")
set( ${PROJECT_NAME}_STARTUP_ASM "${CMAKE_SOURCE_DIR}/Core/Startup/startup_stm32f303vctx.s")

# Drivers - HAL
file(GLOB ${PROJECT_NAME}_HAL_SOURCES "${CMAKE_SOURCE_DIR}/Drivers/STM32F3xx_HAL_Driver/Src/*.c")
message("${PROJECT_NAME}_HAL_SOURCES = ${${PROJECT_NAME}_HAL_SOURCES}")

# Middlewares - FreeRTOS
file(GLOB_RECURSE ${PROJECT_NAME}_FREERTOS_SOURCES "${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/*.c")
message("${PROJECT_NAME}_FREERTOS_SOURCES = ${${PROJECT_NAME}_FREERTOS_SOURCES}")

# Middlewares - USB
file(GLOB_RECURSE ${PROJECT_NAME}_USB_LIB_SOURCES "${CMAKE_SOURCE_DIR}/Middlewares/ST/STM32_USB_Device_Library/*.c")
message("${PROJECT_NAME}_USB_LIB_SOURCES = ${${PROJECT_NAME}_USB_LIB_SOURCES}")

# USB_DEVICE
file(GLOB_RECURSE ${PROJECT_NAME}_USB_DEVICE_SOURCES "${CMAKE_SOURCE_DIR}/USB_DEVICE/*.c")
message("${PROJECT_NAME}_USB_DEVICE_SOURCES = ${${PROJECT_NAME}_USB_DEVICE_SOURCES}")

# Create the static library target
add_library(${PROJECT_NAME} STATIC 
    ${${PROJECT_NAME}_CORE_SOURCES}
    ${${PROJECT_NAME}_HAL_SOURCES}
    ${${PROJECT_NAME}_FREERTOS_SOURCES}
    ${${PROJECT_NAME}_USB_LIB_SOURCES}
    ${${PROJECT_NAME}_USB_DEVICE_SOURCES}
    ${${PROJECT_NAME}_STARTUP_ASM}
)

# Set the include directories for the hal_driver library
target_include_directories(${PROJECT_NAME} PUBLIC
    # Core
    ${CMAKE_SOURCE_DIR}/Core/Inc
    # Drivers - CMSIS
    ${CMAKE_SOURCE_DIR}/Drivers/CMSIS/Include
    ${CMAKE_SOURCE_DIR}/Drivers/CMSIS/Device/ST/STM32F3xx/Include
    # Drivers - HAL
    ${CMAKE_SOURCE_DIR}/Drivers/STM32F3xx_HAL_Driver/Inc
    # Middlewares - FreeRTOS
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/include
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang
    # Middlewares - USB
    ${CMAKE_SOURCE_DIR}/Middlewares/ST/STM32_USB_Device_Library/Core/Inc
    ${CMAKE_SOURCE_DIR}/Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc
    # USB_DEVICE
    ${CMAKE_SOURCE_DIR}/USB_DEVICE/App
    ${CMAKE_SOURCE_DIR}/USB_DEVICE/Target
    # App
    ${CMAKE_SOURCE_DIR}/App
)

add_custom_command(TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMAND ${ARM_NONE_EABI_OBJDUMP} -h -S lib${PROJECT_NAME}.a > ${PROJECT_NAME}.list
)

add_subdirectory(Libraries/rtt)
add_subdirectory(Libraries/tf)
add_subdirectory(Proto)
add_subdirectory(App)

#add_subdirectory(Drivers/CMSIS)
#add_subdirectory(Drivers/STM32F3xx_HAL_Driver)
#add_subdirectory(Middlewares/Third_Party/FreeRTOS)
#add_subdirectory(Core)
#add_subdirectory(Middlewares/ST/STM32_USB_Device_Library)
#add_subdirectory(USB_DEVICE)
