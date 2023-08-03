cmake_minimum_required(VERSION 3.20)
project(exp_app C CXX ASM)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add the source files for the exp_app
set( ${PROJECT_NAME}_SOURCES 
    ${CMAKE_CURRENT_LIST_DIR}/main.cpp
    # app 
    ${CMAKE_CURRENT_LIST_DIR}/app/gpio_srv/GpioService.cpp
    ${CMAKE_CURRENT_LIST_DIR}/app/uart_srv/UartService.cpp
    # driver
    ${CMAKE_CURRENT_LIST_DIR}/driver/tf/FrameDriver.cpp 
    ${CMAKE_CURRENT_LIST_DIR}/driver/tf/TF_integration.cpp
    # hal
    ${CMAKE_CURRENT_LIST_DIR}/hal/gpio/Gpio.cpp 
    ${CMAKE_CURRENT_LIST_DIR}/hal/gpio/GpioIrq.cpp 
    ${CMAKE_CURRENT_LIST_DIR}/hal/uart/Uart.cpp 
    ${CMAKE_CURRENT_LIST_DIR}/hal/uart/UartIrq.cpp 
    # os
    ${CMAKE_CURRENT_LIST_DIR}/os/builder.cpp 
    ${CMAKE_CURRENT_LIST_DIR}/os/hook.cpp 
    ${CMAKE_CURRENT_LIST_DIR}/os/mutex.cpp 
    ${CMAKE_CURRENT_LIST_DIR}/os/queue.cpp 
    ${CMAKE_CURRENT_LIST_DIR}/ os/task.cpp 
    ${CMAKE_CURRENT_LIST_DIR}/os/timer.cpp 
    ${CMAKE_CURRENT_LIST_DIR}/os/msg/msg_broker.cpp
    # srv
    ${CMAKE_CURRENT_LIST_DIR}/srv/debug.cpp 
    ${CMAKE_CURRENT_LIST_DIR}/srv/Stopwatch.cpp 
    ${CMAKE_CURRENT_LIST_DIR}/srv/Timeout.cpp
    # task
    ${CMAKE_CURRENT_LIST_DIR}/task/ctrlTask.cpp
    ${CMAKE_CURRENT_LIST_DIR}/task/gpioTask.cpp
    ${CMAKE_CURRENT_LIST_DIR}/task/uartTask.cpp
)

set(EXECUTABLE ${PROJECT_NAME}.out)

# Add the executable target for exp_app
add_executable(${EXECUTABLE} ${${PROJECT_NAME}_SOURCES})

target_include_directories(${EXECUTABLE} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/.
    ${CMAKE_SOURCE_DIR}/Libraries
)

# Link the libraries with the exp_app target
target_link_libraries(${EXECUTABLE} PUBLIC 
    stm
    rtt
    tf
    proto
)

target_link_options(${EXECUTABLE} PUBLIC
    -T${CMAKE_SOURCE_DIR}/STM32F303VCTX_FLASH.ld
    -mcpu=cortex-m4
    -mthumb
    -mfpu=fpv4-sp-d16
    -mfloat-abi=hard
    --specs=nosys.specs
    -lc
    -lm
    -lnosys
    -Wl,-Map=${PROJECT_NAME}.map,--cref
    -Wl,--gc-sections
)

# Print executable size
add_custom_command(TARGET ${EXECUTABLE}
    POST_BUILD
    COMMAND arm-none-eabi-size.exe ${EXECUTABLE}
)

# Create hex file
add_custom_command(TARGET ${EXECUTABLE}
    POST_BUILD
    COMMAND arm-none-eabi-objcopy.exe -O ihex ${EXECUTABLE} ${PROJECT_NAME}.hex
    COMMAND arm-none-eabi-objcopy.exe -O binary ${EXECUTABLE} ${PROJECT_NAME}.bin
)
