set(executables
    ${CMAKE_CURRENT_SOURCE_DIR}/src/usbdriver.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/drivermanager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Gamepad.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/tusb_gamepad.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/drivers/shared/driverhelper.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/drivers/shared/scaling.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/drivers/dinput/DInputDriver.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/drivers/hid/HIDDriver.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/drivers/ps3/PS3Driver.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/drivers/psclassic/PSClassicDriver.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/drivers/switch/SwitchDriver.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/drivers/xinput/XInputDriver.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/drivers/xboxog/XboxOriginalDriver.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/drivers/xboxog/xid/xid.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/drivers/xboxog/xid/xid_driver.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/drivers/xboxog/xid/xid_gamepad.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/drivers/xboxog/xid/xid_remote.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/drivers/xboxog/xid/xid_steelbattalion.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/drivers/usbserial/USBSerialDriver.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/utilities/log.cpp
)

if(IDF_TARGET)
    set(requires tinyusb_src)

    idf_component_register( SRCS ${executables}
                            INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/src ${CMAKE_CURRENT_SOURCE_DIR}/src/descriptors
                            REQUIRES ${requires})

elseif(PICO_SDK_VERSION_STRING)
    list(APPEND executables
        ${CMAKE_CURRENT_SOURCE_DIR}/src/drivers/uartbridge/UARTBridgeDriver.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/drivers/uartbridge/uart_bridge.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/drivers/uartbridge/esp32_prog_task.cpp)

    add_library(tusb_gamepad ${executables})

    target_include_directories(tusb_gamepad PUBLIC 
                                ${CMAKE_CURRENT_SOURCE_DIR}/src 
                                ${CMAKE_CURRENT_SOURCE_DIR}/src/descriptors)

    target_link_libraries(  pico_multicore
                            pico_stdlib
                            hardware_flash
                            hardware_uart
                            hardware_pio
                            hardware_sync
                            tinyusb_device
                            tinyusb_board
                            )
else()
    message(FATAL_ERROR "Unsupported platform, RP2040 or ESP-IDF required.")
endif()

