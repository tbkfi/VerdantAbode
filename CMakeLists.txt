# [CMAKE] Feature set
cmake_minimum_required(VERSION 3.12)
include(FetchContent)
cmake_policy(SET CMP0169 OLD)


# [PICO-SDK] Download, Include
set(PICO_BOARD pico_w)

FetchContent_Declare(
	pico_sdk
	GIT_REPOSITORY https://github.com/raspberrypi/pico-sdk.git
	GIT_TAG 2.2.0
)
FetchContent_GetProperties(pico_sdk)

if(NOT pico_sdk_POPULATED)
	message(STATUS "[FetchContent] DL: Pico SDK ...")
	FetchContent_Populate(pico_sdk)
endif()
set(PICO_SDK_PATH ${pico_sdk_SOURCE_DIR})
include(${PICO_SDK_PATH}/external/pico_sdk_import.cmake)


# [PROJECT] General Settings
set(PROJECT_NAME ${PROJECT_NAME})
project(${PROJECT_NAME} C CXX ASM)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_C_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


# [PICO-SDK] Initialise
#add_subdirectory(${PICO_SDK_PATH} pico_sdk_build)
pico_sdk_init()
pico_add_extra_outputs(${PROJECT_NAME})


# [FreeRTOS Kernel] Download, Include
add_library(freertos_config INTERFACE)
target_include_directories(freertos_config SYSTEM
	INTERFACE
		include
)

FetchContent_Declare(
	freertos_kernel
	GIT_REPOSITORY https://github.com/FreeRTOS/FreeRTOS-Kernel.git
	GIT_TAG V11.2.0
)
FetchContent_GetProperties(freertos_kernel)

if(NOT freertos_kernel_POPULATED)
	message(STATUS "[FetchContent] DL: FreeRTOS Kernel ...")
	FetchContent_Populate(freertos_kernel)
endif()
set(FREERTOS_KERNEL_PATH ${freertos_kernel_SOURCE_DIR})
include(FreeRTOS_Kernel_import.cmake)


# [PROJECT] H/C
add_executable(${PROJECT_NAME}
	main.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(${PROJECT_NAME} PRIVATE
	pico_stdlib
	freertos_kernel
	freertos_kernel_port
)
