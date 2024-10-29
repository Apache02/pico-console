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

add_subdirectory(src)
