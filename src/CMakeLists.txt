add_executable(
	${PROJECT_NAME}
	main.cpp
	util.cpp

	# FreeRTOS tasks
	task/blinky.cpp
	task/local_inputs.cpp
	task/controller.cpp
	task/parser.cpp
	task/sdp610.cpp
	task/gmp252.cpp

	# Parser Actions
	action/action_local_inputs.cpp
	action/action_sdp610.cpp
	action/action_gmp252.cpp

	# Libraries
	modbus/uart.cpp
	modbus/nanomodbus.c
	modbus/modbus_register.cpp
	modbus/modbus_client.cpp
	modbus/ringbuffer.cpp
)

target_include_directories(
	${PROJECT_NAME} PRIVATE
	${CMAKE_CURRENT_LIST_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/../include
	${CMAKE_CURRENT_SOURCE_DIR}/../include/task
	${CMAKE_CURRENT_SOURCE_DIR}/../include/action
	${CMAKE_CURRENT_SOURCE_DIR}/../include/modbus
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
