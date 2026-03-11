add_executable(
	${PROJECT_NAME}
	main.cpp
	util.cpp

	# FreeRTOS tasks
	task/blinky.cpp
	task/parser.cpp
	task/controller.cpp
	task/local_inputs.cpp
	task/oled.cpp
	task/sdp610.cpp
	task/gmp252.cpp
	task/oled.cpp
    task/mio.cpp
	task/hmp60.cpp

	# Parser Actions
	action/action_local_inputs.cpp
	#action/action_wifi_setup.cpp
	action/action_sdp610.cpp
	action/action_gmp252.cpp
	action/action_hmp60.cpp

	# Libraries
	modbus/uart.cpp
	modbus/nanomodbus.c
	modbus/modbus_register.cpp
	modbus/modbus_client.cpp
	modbus/ringbuffer.cpp

	# SSD1306
	../include/display/PicoI2CBus.cpp
	../include/display/PicoI2CDevice.cpp
	../include/display/ssd1306/framebuf.cpp
	../include/display/ssd1306/mono_vlsb.cpp
	../include/display/ssd1306/ssd1306.cpp
)

target_include_directories(
# Combined namespace, avoid naming collisions
# by using prefixes!
# 
# E.g.
# src/task/sdp610.cpp
# src/action/action_sdp610.cpp
	${PROJECT_NAME} PRIVATE
	${CMAKE_CURRENT_LIST_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/../include
	${CMAKE_CURRENT_SOURCE_DIR}/../include/task
	${CMAKE_CURRENT_SOURCE_DIR}/../include/action
	${CMAKE_CURRENT_SOURCE_DIR}/../include/modbus
	${CMAKE_CURRENT_SOURCE_DIR}/../include/display          # ssd1306
	${CMAKE_CURRENT_SOURCE_DIR}/../include/display/ssd1306  # ssd1306
	${CMAKE_CURRENT_SOURCE_DIR}/../include/display/fonts    # ssd1306
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
