
# lets try out the library thingy
set(I2C_LIB "i2c-driver")

add_library(${I2C_LIB}
    driver-i2c/i2c.cpp
)

target_link_libraries(${I2C_LIB}
    hardware_i2c
)

target_include_directories(${I2C_LIB}
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)
