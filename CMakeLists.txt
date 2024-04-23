set(LIB_NAME tusb_gamepad)

set(LIB_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

set(executables
    ${LIB_ROOT}/src/usbdriver.cpp
    ${LIB_ROOT}/src/drivermanager.cpp
    ${LIB_ROOT}/src/Gamepad.c
    ${LIB_ROOT}/src/tusb_gamepad.cpp
    ${LIB_ROOT}/src/drivers/shared/driverhelper.h
    ${LIB_ROOT}/src/drivers/shared/scaling.cpp
    ${LIB_ROOT}/src/drivers/dinput/DInputDriver.cpp
    ${LIB_ROOT}/src/drivers/hid/HIDDriver.cpp
    ${LIB_ROOT}/src/drivers/ps3/PS3Driver.cpp
    ${LIB_ROOT}/src/drivers/psclassic/PSClassicDriver.cpp
    ${LIB_ROOT}/src/drivers/switch/SwitchDriver.cpp
    ${LIB_ROOT}/src/drivers/xinput/XInputDriver.cpp
    ${LIB_ROOT}/src/drivers/xboxog/XboxOriginalDriver.cpp
    ${LIB_ROOT}/src/drivers/xboxog/xid/xid.c
    ${LIB_ROOT}/src/drivers/xboxog/xid/xid_driver.c
    ${LIB_ROOT}/src/drivers/xboxog/xid/xid_gamepad.c
    ${LIB_ROOT}/src/drivers/xboxog/xid/xid_remote.c
    ${LIB_ROOT}/src/drivers/xboxog/xid/xid_steelbattalion.c
    ${LIB_ROOT}/src/drivers/usbserial/USBSerialDriver.cpp
    ${LIB_ROOT}/src/utilities/log.cpp
)

if(IDF_TARGET)
    set(requires tinyusb_src)

    idf_component_register( SRCS ${executables}
                            INCLUDE_DIRS ${LIB_ROOT}/src ${LIB_ROOT}/src/descriptors
                            REQUIRES ${requires})

elseif(PICO_SDK_VERSION_STRING)
    list(APPEND executables
        ${LIB_ROOT}/src/drivers/uartbridge/UARTBridgeDriver.cpp
        ${LIB_ROOT}/src/drivers/uartbridge/uart_bridge.c
        ${LIB_ROOT}/src/drivers/uartbridge/uart_bridge_task.cpp)

        add_library(${LIB_NAME} ${executables})
        
        target_include_directories(${LIB_NAME} PUBLIC 
            ${LIB_ROOT}/src
            ${LIB_ROOT}/src/descriptors)

        target_link_libraries(${LIB_NAME} 
            pico_stdlib
            pico_multicore
            hardware_flash
            hardware_uart
            hardware_pio
            hardware_sync
            tinyusb_device
            tinyusb_board
            tinyusb_pico_pio_usb)
        
else()
    message(FATAL_ERROR "Unsupported platform, Pico-SDK or ESP-IDF required.")
endif()

