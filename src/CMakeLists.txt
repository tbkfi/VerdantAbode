
add_executable(${ProjectName}
	main.cpp
)

target_link_libraries(${ProjectName}
	PRIVATE
	i2c-driver
)
