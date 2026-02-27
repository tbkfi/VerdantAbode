cmake_minimum_required(VERSION 3.17)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_C_STANDARD 23)

SET(PROJECT_NAME "VerdantAbode")
set(PICO_BOARD pico_w)


# dir is different. maybe needed
#SET(FREERTOS_KERNEL_PATH "${CMAKE_CURRENT_SOURCE_DIR}/module/freertos-kernel") 

option(FETCH_FROM_GIT "Compile against local or remote repositories" ON) #ON by default
if(FETCH_FROM_GIT)
    set(PICO_SDK_FETCH_FROM_GIT on)
    SET(PICO_SDK_PATH "${CMAKE_CURRENT_SOURCE_DIR}/module/pico-sdk")
    include("${CMAKE_CURRENT_SOURCE_DIR}/module/pico_sdk_import.cmake")
	include("${CMAKE_CURRENT_SOURCE_DIR}/module/freertos-kernel/portable/ThirdParty/GCC/RP2040/FreeRTOS_Kernel_import.cmake")
    message(">> Building against remote repositories")
else()
    include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)
    include($ENV{FREERTOS_KERNEL_PATH}/portable/ThirdParty/GCC/RP2040/FreeRTOS_Kernel_import.cmake)
    message(">> Building against local repositories")
endif(FETCH_FROM_GIT)
unset(FETCH_FROM_GIT CACHE) # <---- this is the important!!

project(${PROJECT_NAME} C CXX ASM)

pico_sdk_init()

# incorrect -- SET(FREERTOS_KERNEL_PATH ${CMAKE_CURRENT_SOURCE_DIR}/module/freertos-kernel/FreeRTOS-KernelV10.6.2)
# incorrect -- SET(FREERTOS_KERNEL_PATH ${CMAKE_CURRENT_SOURCE_DIR}/module/freertos-kernel/portable/ThirdParty/GCC/RP2040/FreeRTOS-KernelV10.6.2)

add_subdirectory(src)
add_subdirectory(lib)

# target_include_directories(${PROJECT_NAME} 
#     PUBLIC
#     include
#     include/namespace
# )

include_directories(include)
#include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")
#include($ENV{FREERTOS_KERNEL_PATH}/portable/ThirdParty/GCC/RP2040/FreeRTOS_Kernel_import.cmake)
#include(module/pico_sdk_import.cmake)
#include(${_PICO_SDK_PATH}/pico_sdk_init.cmake)
