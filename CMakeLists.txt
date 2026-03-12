cmake_minimum_required(VERSION 4.0)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_C_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_BUILD_TYPE Debug)
add_compile_options(-O0 -g)

# Suppressions
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-volatile") # FreeRTOS volatile usage


SET(PROJECT_NAME "VerdantAbode")

# Pico-SDK and FreeRTOS
SET(PICO_SDK_PATH "${CMAKE_CURRENT_SOURCE_DIR}/module/pico-sdk")
SET(FREERTOS_KERNEL_PATH "${CMAKE_CURRENT_SOURCE_DIR}/module/freertos-kernel")
include(${PICO_SDK_PATH}/external/pico_sdk_import.cmake)
include(${FREERTOS_KERNEL_PATH}/portable/ThirdParty/GCC/RP2040/FreeRTOS_Kernel_import.cmake)

project(${PROJECT_NAME} C CXX ASM)
pico_sdk_init()

# Subdirectories
add_subdirectory(src)
