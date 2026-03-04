add_executable(
	${PROJECT_NAME}
	main.cpp
	util.cpp

	blinky.cpp
	sdp610.cpp
)

target_include_directories(
	${PROJECT_NAME} PRIVATE
	${CMAKE_CURRENT_LIST_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/../include
)

target_link_libraries(
	${PROJECT_NAME} 
	pico_stdlib 
	hardware_gpio
	hardware_i2c
	FreeRTOS-Kernel-Heap4
)

pico_add_extra_outputs(${PROJECT_NAME})
pico_enable_stdio_usb(${PROJECT_NAME} 0)
pico_enable_stdio_uart(${PROJECT_NAME} 1)
