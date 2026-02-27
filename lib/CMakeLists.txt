# lib/CMakeLists.txt

set(I2C_LIB "driver_i2c")
add_library(${I2C_LIB}
    driver/i2c.cpp
)
target_link_libraries(${I2C_LIB}
    hardware_i2c
    pico_stdlib
)
# not sure if this is needed. until further notice
# target_include_directories(${I2C_LIB}
#     PUBLIC
#     ${CMAKE_CURRENT_SOURCE_DIR}
# )


set(ROM_LIB "driver_rom")
add_library(${ROM_LIB}
    driver/rom.cpp
)
target_link_libraries(${ROM_LIB}
    PUBLIC
    hardware_i2c
    pico_stdlib

    driver_i2c
)

# NOTE: ROM_LIB is PUBLIC(line 24), because we want to automatically link
#       I2C_LIB to anything that includes ROM_LIB, and prevent double linking
