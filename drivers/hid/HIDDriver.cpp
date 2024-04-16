/*
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: Copyright (c) 2024 OpenStickCommunity (gp2040-ce.info)
 */
#include "utilities/scaling.h"

#include "usbd/drivers/hid/HIDDriver.h"
#include "usbd/drivers/shared/driverhelper.h"
#include "usbd/descriptors/HIDDescriptors.h"

// Magic byte sequence to enable PS button on PS3
static const uint8_t ps3_magic_init_bytes[8] = {0x21, 0x26, 0x01, 0x07, 0x00, 0x00, 0x00, 0x00};

static bool hid_control_xfer_cb(uint8_t rhport, uint8_t stage, tusb_control_request_t const * request)
{
	if ( request->bmRequestType == 0xA1 &&
		request->bRequest == HID_REQ_CONTROL_GET_REPORT &&
		request->wValue == 0x0300 ) {
		return tud_control_xfer(rhport, request, (void *) ps3_magic_init_bytes, sizeof(ps3_magic_init_bytes));
	} else {
		return hidd_control_xfer_cb(rhport, stage, request);
	}
}

void HIDDriver::initialize() 
{
	hidReport = {
		.square_btn = 0, .cross_btn = 0, .circle_btn = 0, .triangle_btn = 0,
		.l1_btn = 0, .r1_btn = 0, .l2_btn = 0, .r2_btn = 0,
		.select_btn = 0, .start_btn = 0, .l3_btn = 0, .r3_btn = 0, .ps_btn = 0, .tp_btn = 0,
		.direction = 0x08,
		.l_x_axis = HID_JOYSTICK_MID,
		.l_y_axis = HID_JOYSTICK_MID,
		.r_x_axis = HID_JOYSTICK_MID,
		.r_y_axis = HID_JOYSTICK_MID,
		.right_axis = 0x00, .left_axis = 0x00, .up_axis = 0x00, .down_axis = 0x00,
		.triangle_axis = 0x00, .circle_axis = 0x00, .cross_axis = 0x00, .square_axis = 0x00,
		.l1_axis = 0x00, .r1_axis = 0x00, .l2_axis = 0x00, .r2_axis = 0x00
	};

	class_driver = {
	#if CFG_TUSB_DEBUG >= 2
		.name = "HID",
	#endif
		.init = hidd_init,
		.reset = hidd_reset,
		.open = hidd_open,
		.control_xfer_cb = hid_control_xfer_cb,
		.xfer_cb = hidd_xfer_cb,
		.sof = NULL
	};
}

// Generate HID report from gamepad and send to TUSB Device
void HIDDriver::process(int idx, Gamepad * gamepad, uint8_t * outBuffer) 
{
   if (gamepad->buttons.up) {
        if (gamepad->buttons.right) {
            hidReport.direction = HID_HAT_UPRIGHT;
        } else if (gamepad->buttons.left) {
            hidReport.direction = HID_HAT_UPLEFT;
        } else {
            hidReport.direction = HID_HAT_UP;
        }
    } else if (gamepad->buttons.down) {
        if (gamepad->buttons.right) {
            hidReport.direction = HID_HAT_DOWNRIGHT;
        } else if (gamepad->buttons.left) {
            hidReport.direction = HID_HAT_DOWNLEFT;
        } else {
            hidReport.direction = HID_HAT_DOWN;
        }
    } else if (gamepad->buttons.left) {
        hidReport.direction = HID_HAT_LEFT;
    } else if (gamepad->buttons.right) {
        hidReport.direction = HID_HAT_RIGHT;
    } else {
        hidReport.direction = HID_HAT_NOTHING;
    }

    hidReport.cross_btn     = gamepad->buttons.a      ? 1 : 0;
    hidReport.circle_btn    = gamepad->buttons.b      ? 1 : 0;
    hidReport.square_btn    = gamepad->buttons.x      ? 1 : 0;
    hidReport.triangle_btn  = gamepad->buttons.y      ? 1 : 0;
    hidReport.l1_btn        = gamepad->buttons.lb     ? 1 : 0;
    hidReport.r1_btn        = gamepad->buttons.rb     ? 1 : 0;
    hidReport.l2_btn        = gamepad->triggers.l > 0 ? 1 : 0;
    hidReport.r2_btn        = gamepad->triggers.r > 0 ? 1 : 0;
    hidReport.select_btn    = gamepad->buttons.back   ? 1 : 0;
    hidReport.start_btn     = gamepad->buttons.start  ? 1 : 0;
    hidReport.l3_btn        = gamepad->buttons.l3     ? 1 : 0;
    hidReport.r3_btn        = gamepad->buttons.r3     ? 1 : 0;
    hidReport.ps_btn        = gamepad->buttons.sys    ? 1 : 0;
    hidReport.tp_btn        = gamepad->buttons.misc   ? 1 : 0;

    hidReport.cross_axis    = gamepad->buttons.a  ? 0xFF : 0x00;
    hidReport.circle_axis   = gamepad->buttons.b  ? 0xFF : 0x00;
    hidReport.square_axis   = gamepad->buttons.x  ? 0xFF : 0x00;
    hidReport.triangle_axis = gamepad->buttons.y  ? 0xFF : 0x00;
    hidReport.l1_axis       = gamepad->buttons.lb ? 0xFF : 0x00;
    hidReport.r1_axis       = gamepad->buttons.rb ? 0xFF : 0x00;

    hidReport.l2_axis = gamepad->triggers.l;
    hidReport.r2_axis = gamepad->triggers.r;

    hidReport.l_x_axis = scale_int16_to_uint8(gamepad->joysticks.lx, false);
    hidReport.l_y_axis = scale_int16_to_uint8(gamepad->joysticks.ly, true);
    hidReport.r_x_axis = scale_int16_to_uint8(gamepad->joysticks.rx, false);
    hidReport.r_y_axis = scale_int16_to_uint8(gamepad->joysticks.ry, true);

	// Wake up TinyUSB device
	if (tud_suspended())
		tud_remote_wakeup();

	void * report = &hidReport;
	uint16_t report_size = sizeof(hidReport);
	if (memcmp(last_report, report, report_size) != 0)
	{
		// HID ready + report sent, copy previous report
		if (tud_hid_n_ready((uint8_t)idx) && tud_hid_n_report((uint8_t)idx, 0, report, report_size) == true ) 
        {
			memcpy(last_report, report, report_size);
		}
	}
}

// tud_hid_get_report_cb
uint16_t HIDDriver::get_report(uint8_t report_id, hid_report_type_t report_type, uint8_t *buffer, uint16_t reqlen) 
{
    memcpy(buffer, &hidReport, sizeof(HIDReport));
	return sizeof(HIDReport);
}

// Only PS4 does anything with set report
void HIDDriver::set_report(uint8_t report_id, hid_report_type_t report_type, uint8_t const *buffer, uint16_t bufsize) {}

// Only XboxOG and Xbox One use vendor control xfer cb
bool HIDDriver::vendor_control_xfer_cb(uint8_t rhport, uint8_t stage, tusb_control_request_t const *request) 
{
    return false;
}

const uint16_t * HIDDriver::get_descriptor_string_cb(uint8_t index, uint16_t langid) 
{
	const char *value = (const char *)hid_string_descriptors[index];
	return getStringDescriptor(value, index); // getStringDescriptor returns a static array
}

const uint8_t * HIDDriver::get_descriptor_device_cb() 
{
    return hid_device_descriptor;
}

const uint8_t * HIDDriver::get_hid_descriptor_report_cb(uint8_t itf) 
{
    return hid_report_descriptor;
}

const uint8_t * HIDDriver::get_descriptor_configuration_cb(uint8_t index) 
{
    return hid_configuration_descriptor;
}

const uint8_t * HIDDriver::get_descriptor_device_qualifier_cb() 
{
	return nullptr;
}

void HIDDriver::update_rumble(int idx, Gamepad * gamepad)
{
    
}