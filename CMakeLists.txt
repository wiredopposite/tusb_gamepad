set(LIB_NAME tusb_gamepad)

set(LIB_SRC ${CMAKE_CURRENT_SOURCE_DIR}/src)

set(executables
    ${LIB_SRC}/usbdriver.cpp
    ${LIB_SRC}/drivermanager.cpp
    ${LIB_SRC}/Gamepad.c
    ${LIB_SRC}/tusb_gamepad.cpp
    ${LIB_SRC}/drivers/shared/driverhelper.h
    ${LIB_SRC}/drivers/shared/scaling.cpp
    ${LIB_SRC}/drivers/dinput/DInputDriver.cpp
    ${LIB_SRC}/drivers/hid/HIDDriver.cpp
    ${LIB_SRC}/drivers/ps3/PS3Driver.cpp
    ${LIB_SRC}/drivers/psclassic/PSClassicDriver.cpp
    ${LIB_SRC}/drivers/switch/SwitchDriver.cpp
    ${LIB_SRC}/drivers/xinput/XInputDriver.cpp
    ${LIB_SRC}/drivers/xboxog/XboxOriginalDriver.cpp
    ${LIB_SRC}/drivers/xboxog/xid/xid.c
    ${LIB_SRC}/drivers/xboxog/xid/xid_driver.c
    ${LIB_SRC}/drivers/xboxog/xid/xid_gamepad.c
    ${LIB_SRC}/drivers/xboxog/xid/xid_remote.c
    ${LIB_SRC}/drivers/xboxog/xid/xid_steelbattalion.c
    ${LIB_SRC}/drivers/usbserial/USBSerialDriver.cpp
    ${LIB_SRC}/utilities/log.cpp
)

if(IDF_TARGET)
    set(requires tinyusb_src)

    idf_component_register( SRCS ${executables}
                            INCLUDE_DIRS ${LIB_SRC} ${LIB_SRC}/descriptors
                            REQUIRES ${requires})

elseif(PICO_SDK_VERSION_STRING)
    list(APPEND executables
        ${LIB_SRC}/drivers/uartbridge/UARTBridgeDriver.cpp
        ${LIB_SRC}/drivers/uartbridge/uart_bridge.c
        ${LIB_SRC}/drivers/uartbridge/uart_bridge_task.cpp)

    add_library(${LIB_NAME} ${executables})
    
    target_include_directories(${LIB_NAME} PUBLIC 
        ${LIB_SRC}
        ${LIB_SRC}/descriptors)

    set(LINK_LIBS 
        pico_stdlib
        pico_multicore
        hardware_flash
        hardware_uart
        hardware_pio
        hardware_sync
        tinyusb_device
        tinyusb_board)

    if(TARGET pico_pio_usb)
        list(APPEND LINK_LIBS pico_pio_usb)
    elseif(TARGET tinyusb_pico_pio_usb)
        list(APPEND LINK_LIBS tinyusb_pico_pio_usb)
    endif()

    target_link_libraries(${LIB_NAME} ${LINK_LIBS})

    add_compile_definitions(PICO_BOOT_STAGE2_CHOOSE_GENERIC_03H=1 PICO_XOSC_STARTUP_DELAY_MULTIPLIER=64)
    target_compile_options(${LIB_NAME} PRIVATE -Wno-unused-function -Wno-unused-variable -Wno-array-bounds)
        
else()
    message(FATAL_ERROR "Unsupported platform, Pico-SDK or ESP-IDF required.")
endif()

