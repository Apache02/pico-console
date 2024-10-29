pico_sdk_init()

add_subdirectory(shell)
add_subdirectory(commands)

add_executable(
        pico-console
        main.cpp
        usb/usb_descriptors.c
        usb/usb_task.cpp
        shell_task.cpp
        handlers.cpp
)

pico_add_extra_outputs(pico-console)

include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/config
        ${CMAKE_CURRENT_SOURCE_DIR}/usb
)

target_link_libraries(
        pico-console
        pico_stdlib
        pico_stdio_usb
        pico_unique_id
        tinyusb_device
        FreeRTOS-Kernel
        FreeRTOS-Kernel-Heap4
        shell
        commands
)

add_deploy_target(pico-console "${CMAKE_CURRENT_BINARY_DIR}/pico-console.uf2")
