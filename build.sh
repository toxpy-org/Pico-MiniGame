#!/bin/bash
if [[ "$1" == "-r" ]]
then
    rm -rf build
fi

if [ ! -e build ] 
then
    mkdir build
fi

cd build

# Download Pico SDK at 
# https://github.com/raspberrypi/pico-sdk.git
export PICO_SDK_PATH="/PATH/TO/PICO-SDK"

# Change this to `pico` if using Pi Pico and not the Pi Pico W. 
cmake -DPICO_BOARD=pico_w ..

make