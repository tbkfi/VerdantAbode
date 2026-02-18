cmake_minimum_required(VERSION 3.20)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_C_STANDARD 23)

SET(PROJECT_NAME "VerdantAbode")

# Pico-SDK and FreeRTOS
SET(PICO_SDK_PATH "${CMAKE_CURRENT_SOURCE_DIR}/module/pico-sdk")
SET(FREERTOS_KERNEL_PATH "${CMAKE_CURRENT_SOURCE_DIR}/module/freertos-kernel")
include(${PICO_SDK_PATH}/external/pico_sdk_import.cmake)
include(${FREERTOS_KERNEL_PATH}/portable/ThirdParty/GCC/RP2040/FreeRTOS_Kernel_import.cmake)

project(${PROJECT_NAME})
pico_sdk_init()

# Subdirectories
add_subdirectory(src)
