# TODO
drv_uart_t uart1;
drv_i2c_t i2c0;
drv_i2c_t i2c1;

## Member functions

Consider using overloads to accept different formats when sending data.

### UART
```
.read(char* string)
.write(char* string)
```

### I2C
```
.read()
.write()
```
