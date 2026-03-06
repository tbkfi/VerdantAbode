# lib/CMakeLists.txt

# NOTE: ROM_LIB is PUBLIC, because we want to automatically link
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
    ${CMAKE_SOURCE_DIR}/include
)
##############################################################################
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
target_include_directories(${ROM_LIB}
    PUBLIC
    ${CMAKE_SOURCE_DIR}/include
)
##############################################################################
set(UART_LIB "driver_uart")
add_library(${UART_LIB}
    driver/uart.cpp
    ringbuffer.cpp
)
target_link_libraries(${UART_LIB}
    PUBLIC
    project_config
    hardware_gpio
)
target_include_directories(${UART_LIB}
    PUBLIC
    ${CMAKE_SOURCE_DIR}/include
)
##############################################################################

# add_library(nanomodbus
#     modbus/nanomodbus.h
#     modbus/nanomodbus.c
# )

##############################################################################
add_library(modbus
    modbus/modbus_client.cpp
    modbus/modbus_register.cpp
    modbus/nanomodbus.c
)
target_include_directories(modbus
    PUBLIC
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include/modbus
)
target_link_libraries(modbus
    PUBLIC
    pico_stdlib
    driver_uart
)
##############################################################################
add_library(tasks
    task/gmp252.cpp
    task/sdp610.cpp
)
target_link_libraries(tasks
    PUBLIC
    modbus
    FreeRTOS-Kernel-Heap4
    project_config
    ${I2C_LIB}
)
target_include_directories(tasks
    PUBLIC
    ${CMAKE_SOURCE_DIR}/include
)
##############################################################################
#  set(NET_LIB "driver_net")
#  add_library(${NET_LIB}
#      driver/wifi.cpp
#  )
#  target_include_directories(${I2C_LIB}
#      PUBLIC
#      ${CMAKE_CURRENT_SOURCE_DIR}/include
#  )
#  target_link_libraries(${NET_LIB}
#      PUBLIC
#      pico_cyw43_arch_lwip_sys_freertos
#      pico_lwip_mbedtls
#      pico_mbedtls
#  
#      project_config  # ! <-- importando !!
#  )

