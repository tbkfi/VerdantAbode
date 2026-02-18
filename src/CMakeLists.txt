
add_compile_options(
    -Wall
    -Wpedantic
)

add_executable(${ProjectName}
	main.cpp
)

target_link_libraries(${ProjectName}
	PRIVATE
	i2c-driver
    hardware_i2c
    pico_stdlib
)
