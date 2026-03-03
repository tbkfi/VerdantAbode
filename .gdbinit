file build/src/VerdantAbode.elf
target remote localhost:3333
monitor halt
load
break main
monitor reset halt
continue
