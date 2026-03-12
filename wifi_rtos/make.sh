#!/bin/bash 
# 1. Change into the build directory

cd build
# 3. Run Make to compile the project
# '-j' uses all available CPU cores for faster compilation.
echo "Running Make..."
make -j

# 4. Return to the original directory
cd ..

