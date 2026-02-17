
# lets try out the library thingy
add_library(i2c-driver
    i2c.cpp
)

target_include_directories(i2c-driver
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)
