cmake_minimum_required(VERSION 3.13)

include (pico_sdk_import.cmake)

project(pico_examples C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_subdirectory(lib/Config)
add_subdirectory(lib/LCD)
add_subdirectory(lib/Fonts)
add_subdirectory(lib/GUI)

include_directories(./lib/Config)
include_directories(./lib/GUI)
include_directories(./lib/LCD)

add_executable(
    hello_world
    ./src/main.c
)

target_link_libraries(hello_world 
    pico_stdlib
    pico_cyw43_arch_none     
    hardware_adc
    LCD
    GUI 
    Fonts 
    Config
)

pico_enable_stdio_usb(hello_world 1)
pico_enable_stdio_uart(hello_world 0)


pico_add_extra_outputs(hello_world)