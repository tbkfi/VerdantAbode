#!/bin/bash

# Is Ninja available
if command -v ninja > /dev/null 2>&1; then
	CUSTOM_OPTS="-G Ninja"
fi

mkdir -p ./build && cd ./build && cmake $CUSTOM_OPTS .. && cmake --build . -j$(nproc)
