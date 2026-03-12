# Tell CMake where to find the executable source file
add_executable(${ProjectName} 
    main.cpp
    wifi.cpp
)

# Create map/bin/hex/uf2 files
pico_add_extra_outputs(${ProjectName})

# Link to pico_stdlib (gpio, time, etc. functions)
target_link_libraries(${ProjectName} 
        pico_stdlib
        hardware_gpio
        FreeRTOS-Kernel-Heap4
        hardware_spi
        pico_cyw43_arch_lwip_poll
        hardware_adc
        pico_lwip_http
)
target_include_directories(${ProjectName} PRIVATE ${CMAKE_CURRENT_LIST_DIR})

pico_add_extra_outputs(${ProjectName})

# Disable uart output, enable usb output
pico_enable_stdio_usb(${ProjectName} 1)
pico_enable_stdio_uart(${ProjectName} 0)
