# Symbols
file build/src/VerdantAbode.elf

# Plugins
python import freertos_gdb

# Connection
target remote localhost:3333
monitor halt
load

# Start
break main
monitor reset halt
continue
