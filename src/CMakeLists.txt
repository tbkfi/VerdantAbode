
add_executable(${PROJECT_NAME}
    main.cpp
)

add_compile_options(
	-Wall
	-Wpedantic
	-fdiagnostics-color=always
	-fdiagnostics-format=json
	-fno-diagnostics-show-caret
	-O0
    -g
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
    project_config

	driver_rom
	hardware_i2c
	pico_stdlib
    FreeRTOS-Kernel-Heap4
)

pico_add_extra_outputs(${PROJECT_NAME})
pico_enable_stdio_usb(${PROJECT_NAME} 0)
pico_enable_stdio_uart(${PROJECT_NAME} 1)

