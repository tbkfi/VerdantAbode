# lib/CMakeLists.txt

# NOTE: ROM_LIB is PUBLIC(line 24), because we want to automatically link
#       I2C_LIB to anything that includes ROM_LIB, and prevent double linking

# NOTE: target_link_libraries is needed to expose include/ directory

# NOTE: project config must be linked as a library!

set(I2C_LIB "driver_i2c")
add_library(${I2C_LIB}
    driver/i2c.cpp
)
target_link_libraries(${I2C_LIB}
    hardware_i2c
    pico_stdlib

    project_config  # ! <-- importando !!
)
target_include_directories(${I2C_LIB}
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

set(ROM_LIB "driver_rom")
add_library(${ROM_LIB}
    driver/rom.cpp
)
target_link_libraries(${ROM_LIB}
    PUBLIC
    hardware_i2c
    pico_stdlib

    driver_i2c
    project_config  # ! <-- importando !!
)

set(NET_LIB "driver_net")
add_library(${NET_LIB}
    driver/wifi.cpp
)
target_include_directories(${I2C_LIB}
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_libraries(${NET_LIB}
    PUBLIC
    pico_cyw43_arch_lwip_sys_freertos
    pico_lwip_mbedtls
    pico_mbedtls

    project_config  # ! <-- importando !!
)

