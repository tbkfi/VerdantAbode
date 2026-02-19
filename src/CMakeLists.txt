
add_compile_options(
	-Wall
	-Wpedantic
)

add_executable(${ProjectName}
	main.cpp
)

add_compile_options(
	-fdiagnostics-color=always 	# colored output
	-fdiagnostics-format=json
	-fno-diagnostics-show-caret 	# cleaner log output
	-O0
	-g	# produce debug symbols
)

pico_enable_stdio_usb(${ProjectName} 0)
pico_enable_stdio_uart(${ProjectName} 1)

target_link_libraries(${ProjectName}
	PRIVATE
	driver_i2c
	hardware_i2c
	pico_stdlib
)
