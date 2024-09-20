cmake_minimum_required(VERSION 3.13)

# specified board definitions
list(APPEND PICO_BOARD_HEADER_DIRS ${CMAKE_CURRENT_LIST_DIR}/src/boards)
set(PICO_BOARD pico)

# use config.cmake
if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/config.cmake)
    include(config.cmake)
endif ()


include(pico_sdk_import.cmake)
include(scripts/functions.cmake)
include(FreeRTOS_Kernel_import.cmake)

project(pico-console C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
pico_sdk_init()


add_subdirectory(src/console)

add_executable(
        pico-console
        src/main.cpp
        src/usb/usb_descriptors.c
        src/usb/usb_task.cpp
        src/msc_disk.c
)

pico_add_extra_outputs(pico-console)

include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/src/config
        ${CMAKE_CURRENT_SOURCE_DIR}/src/usb
)

target_link_libraries(
        pico-console
        pico_stdlib
        pico_stdio_usb
        pico_unique_id
        tinyusb_device
        FreeRTOS-Kernel
        FreeRTOS-Kernel-Heap4
        task_console
)

add_deploy_target(pico-console "${CMAKE_CURRENT_BINARY_DIR}/pico-console.uf2")
