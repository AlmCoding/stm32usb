# toolchain.cmake

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR ARM)
set(CMAKE_CROSSCOMPILING 1)

set(CMAKE_C_COMPILER "C:/ST/STM32CubeIDE_1.12.0/STM32CubeIDE/plugins/com.st.stm32cube.ide.mcu.externaltools.gnu-tools-for-stm32.10.3-2021.10.win32_1.0.200.202301161003/tools/bin/arm-none-eabi-gcc.exe")
set(CMAKE_CXX_COMPILER "C:/ST/STM32CubeIDE_1.12.0/STM32CubeIDE/plugins/com.st.stm32cube.ide.mcu.externaltools.gnu-tools-for-stm32.10.3-2021.10.win32_1.0.200.202301161003/tools/bin/arm-none-eabi-g++.exe")
#set(CMAKE_C_COMPILER "arm-none-eabi-gcc.exe")
#set(CMAKE_CXX_COMPILER "arm-none-eabi-g++.exe")
set(CMAKE_ASM_COMPILER "${CMAKE_C_COMPILER}")

set(ARM_NONE_EABI_OBJDUMP "C:/ST/STM32CubeIDE_1.12.0/STM32CubeIDE/plugins/com.st.stm32cube.ide.mcu.externaltools.gnu-tools-for-stm32.10.3-2021.10.win32_1.0.200.202301161003/tools/bin/arm-none-eabi-objdump.exe")
set(ARM_NONE_EABI_OBJCOPY "C:/ST/STM32CubeIDE_1.12.0/STM32CubeIDE/plugins/com.st.stm32cube.ide.mcu.externaltools.gnu-tools-for-stm32.10.3-2021.10.win32_1.0.200.202301161003/tools/bin/arm-none-eabi-objcopy.exe")
set(ARM_NONE_EABI_SIZE "C:/ST/STM32CubeIDE_1.12.0/STM32CubeIDE/plugins/com.st.stm32cube.ide.mcu.externaltools.gnu-tools-for-stm32.10.3-2021.10.win32_1.0.200.202301161003/tools/bin/arm-none-eabi-size.exe")
#set(ARM_NONE_EABI_OBJDUMP "arm-none-eabi-objdump.exe")
#set(ARM_NONE_EABI_OBJCOPY "arm-none-eabi-objcopy.exe")
#set(ARM_NONE_EABI_SIZE "arm-none-eabi-size.exe")

# Skip the compiler test
set(CMAKE_C_COMPILER_WORKS 1)
set(CMAKE_CXX_COMPILER_WORKS 1)

# Add C and C++ flags as needed for your specific target and toolchain
# For example:
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c17 -g3 -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard -fdata-sections -ffunction-sections -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -g3 -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard -fdata-sections -ffunction-sections -Wall")
#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -T${CMAKE_SOURCE_DIR}/STM32F303VCTX_FLASH.ld,-mcpu=cortex-m4,-mthumb,-mfpu=fpv4-sp-d16,-mfloat-abi=hard,-specs=nano.specs,-lc,-lm,-lnosys,-Wl,-Map=${PROJECT_NAME}.map,--cref,--gc-sections")
#-T${CMAKE_SOURCE_DIR}/STM32F303VCTX_FLASH.ld,-mcpu=cortex-m4,-mthumb,-mfpu=fpv4-sp-d16,-mfloat-abi=hard,-specs=nano.specs,-lc,-lm,-lnosys,-Wl,-Map=${PROJECT_NAME}.map,--cref,--gc-sections

# Add the required defines
add_definitions(
    -DDEBUG
    -DSTM32F303xC
    -DUSE_HAL_DRIVER
    -DRENAME_CUBEMX_MAIN
    -DHIDE_CUBEMX_FREERTOS
    -DSEGGER_RTT_MODE_DEFAULT=SEGGER_RTT_MODE_BLOCK_IF_FIFO_FULL
)

# Enable verbose output
#set(CMAKE_VERBOSE_MAKEFILE ON CACHE BOOL "ON")
