
# set(I2C_LIB "driver_i2c")
# add_library(${I2C_LIB}
#     driver/i2c.cpp
# )
# target_link_libraries(${I2C_LIB}
#     PRIVATE
#     hardware_i2c
#     pico_stdlib
# )
# target_include_directories(${I2C_LIB}
#     PUBLIC
#     ${CMAKE_CURRENT_SOURCE_DIR}
# )
# 
set(GEN_DRIVERS "driver_rom")
add_library(${GEN_DRIVERS}
    driver/rom.cpp
    driver/i2c.cpp
)
target_link_libraries(${GEN_DRIVERS}
     hardware_i2c
     pico_stdlib
)
	
