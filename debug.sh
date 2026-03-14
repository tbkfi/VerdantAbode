#!/bin/bash
# Start the debug server,
# can be detached to bg shell
openocd -f interface/cmsis-dap.cfg \
	-f target/rp2040.cfg \
	-c "adapter speed 5000" \
	>> gdb-server.log
