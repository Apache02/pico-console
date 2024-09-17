add_library(task_console INTERFACE)

file(
        GLOB COMMANDS
        ${CMAKE_CURRENT_LIST_DIR}/commands/*.cpp
)

target_sources(
        task_console INTERFACE
        console_task.cpp
        Console.cpp
        Packet.cpp

        ${COMMANDS}
)

target_link_libraries(
        task_console INTERFACE
        pico_stdlib
        pico_unique_id
        hardware_clocks
        hardware_adc
)
