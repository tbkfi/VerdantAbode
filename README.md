# Green House CO2 System

## Hardware

## Project Structure

### Software Hierarchy
```
Project sources are split into headers files .h/.hpp and source files .c/.cpp.
All sources for drivers reside under lib/drivers, and headers to use are
defined under include/drivers. To use them:

// driver-xxx.cpp
#include "driver/xxx"
...
// write your silly ass code

Linking allothis:
// lib/cmakelists
add_library(driver_xxx)
... # define the stuff that cmake needs

// cmakelists
target_link_libraries($PROJECT_NAME
    driver_xxx
)

```
# TLDR:
put your:
- sources: lib/driver
- headers: include/driver

### Data Formats and Backend

## Building

```
Project can be built and flashed using helper scripts in the following order:
1. setup.sh
2. build.sh
3. flash.sh
```

Alternatively initialise the repository manually (and all submodules!), then
create a "build" directory, and run `cmake .. && make` from within it.
