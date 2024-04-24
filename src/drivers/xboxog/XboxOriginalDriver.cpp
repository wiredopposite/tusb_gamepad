#include "drivers/xboxog/XboxOriginalDriver.h"
#include "drivers/xboxog/xid/xid.h"
#include "drivers/shared/driverhelper.h"

void XboxOriginalDriver::initialize() 
{
    xboxog_report = {
        .zero = 0,
        .bLength = sizeof(XboxOriginalReport),
        .dButtons = 0,
        .reserved = 0,
        .A = 0,
        .B = 0,
        .X = 0,
        .Y = 0,
        .BLACK = 0,
        .WHITE = 0,
        .L = 0,
        .R = 0,
        .leftStickX = 0,
        .leftStickY = 0,
        .rightStickX = 0,
        .rightStickY = 0,
    };

    // Copy XID driver to local class driver
    memcpy(&class_driver, xid_get_driver(), sizeof(usbd_class_driver_t));
}

void XboxOriginalDriver::process(int idx, Gamepad * gamepad, uint8_t * outBuffer) 
{
    (void)idx;
    (void)outBuffer;

	xboxog_report.dButtons = 0
		| (gamepad->buttons.up    ? XID_DUP    : 0)
		| (gamepad->buttons.down  ? XID_DDOWN  : 0)
		| (gamepad->buttons.left  ? XID_DLEFT  : 0)
		| (gamepad->buttons.right ? XID_DRIGHT : 0)
		| (gamepad->buttons.start ? XID_START  : 0)
		| (gamepad->buttons.back  ? XID_BACK   : 0)
		| (gamepad->buttons.l3    ? XID_LS     : 0)
		| (gamepad->buttons.r3    ? XID_RS     : 0)
	;

    if (gamepad->enable_analog_buttons)
    {
        xboxog_report.A     = gamepad->analog_buttons.a ;
        xboxog_report.B     = gamepad->analog_buttons.b ;
        xboxog_report.X     = gamepad->analog_buttons.x ;
        xboxog_report.Y     = gamepad->analog_buttons.y ;
        xboxog_report.BLACK = gamepad->analog_buttons.rb;
        xboxog_report.WHITE = gamepad->analog_buttons.lb;
    }
    else
    {
        xboxog_report.A     = gamepad->buttons.a  ? 0xFF : 0;
        xboxog_report.B     = gamepad->buttons.b  ? 0xFF : 0;
        xboxog_report.X     = gamepad->buttons.x  ? 0xFF : 0;
        xboxog_report.Y     = gamepad->buttons.y  ? 0xFF : 0;
        xboxog_report.BLACK = gamepad->buttons.rb ? 0xFF : 0;
        xboxog_report.WHITE = gamepad->buttons.lb ? 0xFF : 0;
    }

    xboxog_report.L = gamepad->triggers.l;
    xboxog_report.R = gamepad->triggers.r;

	xboxog_report.leftStickX = gamepad->joysticks.lx;
	xboxog_report.leftStickY = gamepad->joysticks.ly;
	xboxog_report.rightStickX = gamepad->joysticks.rx;
	xboxog_report.rightStickY = gamepad->joysticks.ry;

	if (tud_suspended())
		tud_remote_wakeup();

    uint8_t xIndex = xid_get_index_by_type(0, XID_TYPE_GAMECONTROLLER);

	if (memcmp(last_report, &xboxog_report, sizeof(XboxOriginalReport)) != 0) 
    {
        if ( xid_send_report(xIndex, &xboxog_report, sizeof(XboxOriginalReport)) == true ) 
        {
            memcpy(last_report, &xboxog_report, sizeof(XboxOriginalReport));
        }
    }

    xid_get_report(xIndex, &xboxog_out_report, sizeof(XboxOriginalOutReport));
}

uint16_t XboxOriginalDriver::get_report(uint8_t report_id, hid_report_type_t report_type, uint8_t *buffer, uint16_t reqlen) 
{
    (void)report_id;
    (void)report_type;
    (void)reqlen;

    memcpy(buffer, &xboxog_report, sizeof(XboxOriginalReport));
	return sizeof(XboxOriginalReport);
}

void XboxOriginalDriver::set_report(uint8_t report_id, hid_report_type_t report_type, uint8_t const *buffer, uint16_t bufsize) 
{
    (void)report_id;
    (void)report_type;
    (void)bufsize;
    (void)buffer;
}

bool XboxOriginalDriver::vendor_control_xfer_cb(uint8_t rhport, uint8_t stage, tusb_control_request_t const *request) 
{
    return class_driver.control_xfer_cb(rhport, stage, request);
}

const uint16_t * XboxOriginalDriver::get_descriptor_string_cb(uint8_t index, uint16_t langid) 
{
    (void)langid;

	const char *value = (const char *)xboxoriginal_string_descriptors[index];
	return getStringDescriptor(value, index); // getStringDescriptor returns a static array
}

const uint8_t * XboxOriginalDriver::get_descriptor_device_cb() 
{
    return xboxoriginal_device_descriptor;
}

const uint8_t * XboxOriginalDriver::get_hid_descriptor_report_cb(uint8_t itf) 
{
    (void)itf;

    return nullptr;
}

const uint8_t * XboxOriginalDriver::get_descriptor_configuration_cb(uint8_t index) 
{
    (void)index;

    return xboxoriginal_configuration_descriptor;
}

const uint8_t * XboxOriginalDriver::get_descriptor_device_qualifier_cb() 
{
	return nullptr;
}

void XboxOriginalDriver::update_rumble(int idx, Gamepad * gamepad)
{
    (void)idx;

    if (xboxog_out_report.zero == 0 && 
        xboxog_out_report.bLength == sizeof(XboxOriginalOutReport))
    {
        gamepad->rumble.l = xboxog_out_report.lValue >> 8;
        gamepad->rumble.r = xboxog_out_report.rValue >> 8;
    }
}