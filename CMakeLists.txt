cmake_minimum_required(VERSION 3.12)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(PICO_BOARD pico_w)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_C_STANDARD 17)

SET(ProjectName VerdantAbode)

include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)
include($ENV{FREERTOS_KERNEL_PATH}/portable/ThirdParty/GCC/RP2040/FreeRTOS_Kernel_import.cmake)

project(${ProjectName})
pico_sdk_init()
#add_subdirectory(include)
add_subdirectory(src)
add_subdirectory(lib)
