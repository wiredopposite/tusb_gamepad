#pragma once

#include <stdint.h>
#include "tusb.h"
#include "board_config.h"

#define HID_ENDPOINT_SIZE 64

/**************************************************************************
 *
 *  Endpoint Buffer Configuration
 *
 **************************************************************************/

#define ENDPOINT0_SIZE	64

#define GAMEPAD_INTERFACE	0
#define GAMEPAD_ENDPOINT	1
#define GAMEPAD_SIZE		64

#define LSB(n) (n & 255)
#define MSB(n) ((n >> 8) & 255)

#define DINPUT_HAT_UP        0x00
#define DINPUT_HAT_UPRIGHT   0x01
#define DINPUT_HAT_RIGHT     0x02
#define DINPUT_HAT_DOWNRIGHT 0x03
#define DINPUT_HAT_DOWN      0x04
#define DINPUT_HAT_DOWNLEFT  0x05
#define DINPUT_HAT_LEFT      0x06
#define DINPUT_HAT_UPLEFT    0x07
#define DINPUT_HAT_NOTHING   0x08

#define DINPUT_JOYSTICK_MIN 0x00
#define DINPUT_JOYSTICK_MID 0x80
#define DINPUT_JOYSTICK_MAX 0xFF

typedef struct __attribute((packed, aligned(1)))
{
	// digital buttons, 0 = off, 1 = on
    // uint8_t report_id;
    // uint8_t padding;

	uint8_t square_btn : 1;
    uint8_t cross_btn : 1;
	uint8_t circle_btn : 1;
    uint8_t triangle_btn : 1;

	uint8_t l1_btn : 1;
	uint8_t r1_btn : 1;
	uint8_t l2_btn : 1;
	uint8_t r2_btn : 1;

	uint8_t select_btn : 1;
	uint8_t start_btn : 1;
	uint8_t l3_btn : 1;
	uint8_t r3_btn : 1;
	
	uint8_t ps_btn : 1;
	uint8_t tp_btn : 1;
//	uint8_t l2_btn_alt : 1;
	
//	uint8_t r2_btn_alt : 1;
	uint8_t : 2;

	// digital direction, use the dir_* constants(enum)
	// 8 = center, 0 = up, 1 = up/right, 2 = right, 3 = right/down
	// 4 = down, 5 = down/left, 6 = left, 7 = left/up

	// uint8_t direction;
    uint8_t direction : 4;
    uint8_t : 4;

	// left and right analog sticks, 0x00 left/up, 0x80 middle, 0xff right/down

	uint8_t l_x_axis;
	uint8_t l_y_axis;
	uint8_t r_x_axis;
	uint8_t r_y_axis;

	// Gonna assume these are button analog values for the d-pad.
	// NOTE: NOT EVEN SURE THIS IS RIGHT, OR IN THE CORRECT ORDER
	uint8_t right_axis;
	uint8_t left_axis;
	uint8_t up_axis;
	uint8_t down_axis;

	// button axis, 0x00 = unpressed, 0xff = fully pressed

	uint8_t triangle_axis;
	uint8_t circle_axis;
	uint8_t cross_axis;
	uint8_t square_axis;

	uint8_t l1_axis;
	uint8_t r1_axis;
	uint8_t l2_axis;
	uint8_t r2_axis;
} DInputReport;

// struct DInputLed {
// 	uint8_t time_enabled; /* the total time the led is active (0xff means forever) */
// 	uint8_t duty_length;  /* how long a cycle is in deciseconds (0 means "really fast") */
// 	uint8_t enabled;
// 	uint8_t duty_off; /* % of duty_length the led is off (0xff means 100%) */
// 	uint8_t duty_on;  /* % of duty_length the led is on (0xff mean 100%) */
// } __attribute__((packed));

// struct DInputRumble {
// 	uint8_t padding;
// 	uint8_t right_duration; /* Right motor duration (0xff means forever) */
// 	uint8_t right_motor_on; /* Right (small) motor on/off, only supports values of 0 or 1 (off/on) */
// 	uint8_t left_duration;    /* Left motor duration (0xff means forever) */
// 	uint8_t left_motor_force; /* left (large) motor, supports force values from 0 to 255 */
// } __attribute__((packed));

// struct DInputOutReport {
// 	struct DInputRumble rumble;
// 	uint8_t padding[4];
// 	uint8_t leds_bitmap; /* bitmap of enabled LEDs: LED_1 = 0x02, LED_2 = 0x04, ... */
// 	struct DInputLed led[4];    /* LEDx at (4 - x) */
// 	struct DInputLed _reserved; /* LED5, not actually soldered */
// } __attribute__((packed));

static const uint8_t dinput_string_language[]     = { 0x09, 0x04 };
static const uint8_t dinput_string_manufacturer[] = "SHANWAN";
static const uint8_t dinput_string_product[]      = "2In1 USB Joystick";
static const uint8_t dinput_string_version[]      = "1.0";

static const uint8_t *dinput_string_descriptors[] __attribute__((unused)) =
{
	dinput_string_language,
	dinput_string_manufacturer,
	dinput_string_product,
	dinput_string_version
};

static const uint8_t dinput_device_descriptor[] =
{
    0x12,        // bLength
    0x01,        // bDescriptorType (Device)
    0x10, 0x01,  // bcdUSB 1.10
    0x00,        // bDeviceClass (Use class information in the Interface Descriptors)
    0x00,        // bDeviceSubClass 
    0x00,        // bDeviceProtocol 
    0x40,        // bMaxPacketSize0 64
    0x63, 0x25,  // idVendor 0x2563
    0x75, 0x05,  // idProduct 0x0575
    0x00, 0x02,  // bcdDevice 4.00
    0x01,        // iManufacturer (String Index)
    0x02,        // iProduct (String Index)
    0x00,        // iSerialNumber (String Index)
    0x01,        // bNumConfigurations 1
};

static const uint8_t dinput_report_descriptor[] =
{
    0x05, 0x01,        // Usage Page (Generic Desktop Ctrls)
    0x09, 0x05,        // Usage (Game Pad)
    0xA1, 0x01,        // Collection (Application)
    0x15, 0x00,        //   Logical Minimum (0)
    0x25, 0x01,        //   Logical Maximum (1)
    0x35, 0x00,        //   Physical Minimum (0)
    0x45, 0x01,        //   Physical Maximum (1)
    0x75, 0x01,        //   Report Size (1)
    0x95, 0x0D,        //   Report Count (13)
    0x05, 0x09,        //   Usage Page (Button)
    0x19, 0x01,        //   Usage Minimum (0x01)
    0x29, 0x0D,        //   Usage Maximum (0x0D)
    0x81, 0x02,        //   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0x95, 0x03,        //   Report Count (3)
    0x81, 0x01,        //   Input (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0x05, 0x01,        //   Usage Page (Generic Desktop Ctrls)
    0x25, 0x07,        //   Logical Maximum (7)
    0x46, 0x3B, 0x01,  //   Physical Maximum (315)
    0x75, 0x04,        //   Report Size (4)
    0x95, 0x01,        //   Report Count (1)
    0x65, 0x14,        //   Unit (System: English Rotation, Length: Centimeter)
    0x09, 0x39,        //   Usage (Hat switch)
    0x81, 0x42,        //   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,Null State)
    0x65, 0x00,        //   Unit (None)
    0x95, 0x01,        //   Report Count (1)
    0x81, 0x01,        //   Input (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0x26, 0xFF, 0x00,  //   Logical Maximum (255)
    0x46, 0xFF, 0x00,  //   Physical Maximum (255)
    0x09, 0x30,        //   Usage (X)
    0x09, 0x31,        //   Usage (Y)
    0x09, 0x32,        //   Usage (Z)
    0x09, 0x35,        //   Usage (Rz)
    0x75, 0x08,        //   Report Size (8)
    0x95, 0x04,        //   Report Count (4)
    0x81, 0x02,        //   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0x06, 0x00, 0xFF,  //   Usage Page (Vendor Defined 0xFF00)
    0x09, 0x20,        //   Usage (0x20)
    0x09, 0x21,        //   Usage (0x21)
    0x09, 0x22,        //   Usage (0x22)
    0x09, 0x23,        //   Usage (0x23)
    0x09, 0x24,        //   Usage (0x24)
    0x09, 0x25,        //   Usage (0x25)
    0x09, 0x26,        //   Usage (0x26)
    0x09, 0x27,        //   Usage (0x27)
    0x09, 0x28,        //   Usage (0x28)
    0x09, 0x29,        //   Usage (0x29)
    0x09, 0x2A,        //   Usage (0x2A)
    0x09, 0x2B,        //   Usage (0x2B)
    0x95, 0x0C,        //   Report Count (12)
    0x81, 0x02,        //   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0x0A, 0x21, 0x26,  //   Usage (0x2621)
    0x95, 0x08,        //   Report Count (8)
    0xB1, 0x02,        //   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    0x0A, 0x21, 0x26,  //   Usage (0x2621)
    0x91, 0x02,        //   Output (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    0x26, 0xFF, 0x03,  //   Logical Maximum (1023)
    0x46, 0xFF, 0x03,  //   Physical Maximum (1023)
    0x09, 0x2C,        //   Usage (0x2C)
    0x09, 0x2D,        //   Usage (0x2D)
    0x09, 0x2E,        //   Usage (0x2E)
    0x09, 0x2F,        //   Usage (0x2F)
    0x75, 0x10,        //   Report Size (16)
    0x95, 0x04,        //   Report Count (4)
    0x81, 0x02,        //   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0xC0,              // End Collection
};

static const uint8_t dinput_hid_descriptor[] =
{
    0x09,        // bLength
    0x21,        // bDescriptorType (HID)
    0x10, 0x01,  // bcdHID 1.10
    0x00,        // bCountryCode
    0x01,        // bNumDescriptors
    0x22,        // bDescriptorType[0] (HID)
    0x89, 0x00,  // wDescriptorLength[0] 137
};

enum { ITF_NUM_HID1, ITF_NUM_HID2, ITF_NUM_HID3, ITF_NUM_HID4, ITF_NUM_TOTAL };

#define CONFIG_TOTAL_LEN                                                       \
    (TUD_CONFIG_DESC_LEN + (TUD_HID_DESC_LEN * MAX_GAMEPADS))

#define EPNUM_HID1 0x81
#define EPNUM_HID2 0x82
#define EPNUM_HID3 0x83
#define EPNUM_HID4 0x84

uint8_t const dinput_configuration_descriptor[] = {
    // Config number, interface count, string index, total length, attribute, power in mA
    TUD_CONFIG_DESCRIPTOR(1,
                        ITF_NUM_TOTAL,
                        0,
                        CONFIG_TOTAL_LEN,
                        TUSB_DESC_CONFIG_ATT_REMOTE_WAKEUP,
                        500),

    // Interface number, string index, protocol, report descriptor len, EP In address, size & polling interval
    TUD_HID_DESCRIPTOR(ITF_NUM_HID1,
                    0,
                    HID_ITF_PROTOCOL_NONE,
                    sizeof(dinput_report_descriptor),
                    EPNUM_HID1,
                    CFG_TUD_HID_EP_BUFSIZE,
                    1),
#if (MAX_GAMEPADS > 1)
    TUD_HID_DESCRIPTOR(ITF_NUM_HID2,
                    0,
                    HID_ITF_PROTOCOL_NONE,
                    sizeof(dinput_report_descriptor),
                    EPNUM_HID2,
                    CFG_TUD_HID_EP_BUFSIZE,
                    1),
#endif
#if (MAX_GAMEPADS > 2)
    TUD_HID_DESCRIPTOR(ITF_NUM_HID3,
                    0,
                    HID_ITF_PROTOCOL_NONE,
                    sizeof(dinput_report_descriptor),
                    EPNUM_HID3,
                    CFG_TUD_HID_EP_BUFSIZE,
                    1),
#endif
#if (MAX_GAMEPADS > 3)
    TUD_HID_DESCRIPTOR(ITF_NUM_HID4,
                    0,
                    HID_ITF_PROTOCOL_NONE,
                    sizeof(dinput_report_descriptor),
                    EPNUM_HID4,
                    CFG_TUD_HID_EP_BUFSIZE,
                    1)
#endif
};

//     static const uint8_t dinput_configuration_descriptor[] =
//     {
//         0x09,        // bLength
//         0x02,        // bDescriptorType (Configuration)
//         0x29, 0x00,  // wTotalLength 41
//         0x01,        // bNumInterfaces
//         0x01,        // bConfigurationValue
//         0x00,        // iConfiguration (String Index)
//         0x80,        // bmAttributes
//         0xFA,        // bMaxPower 500mA

//         0x09,        // bLength
//         0x04,        // bDescriptorType (Interface)
//         0x00,        // bInterfaceNumber 0
//         0x00,        // bAlternateSetting
//         0x02,        // bNumEndpoints 2
//         0x03,        // bInterfaceClass
//         0x00,        // bInterfaceSubClass
//         0x00,        // bInterfaceProtocol
//         0x00,        // iInterface (String Index)

//         0x09,        // bLength
//         0x21,        // bDescriptorType (HID)
//         0x10, 0x01,  // bcdHID 1.10
//         0x00,        // bCountryCode
//         0x01,        // bNumDescriptors
//         0x22,        // bDescriptorType[0] (HID)
//         0x89, 0x00,  // wDescriptorLength[0] 137

//         0x07,        // bLength
//         0x05,        // bDescriptorType (Endpoint)
//         0x02,        // bEndpointAddress (OUT/H2D)
//         0x03,        // bmAttributes (Interrupt)
//         0x20, 0x00,  // wMaxPacketSize 32
//         0x0A,        // bInterval 10 (unit depends on device speed)

//         0x07,        // bLength
//         0x05,        // bDescriptorType (Endpoint)
//         0x81,        // bEndpointAddress (IN/D2H)
//         0x03,        // bmAttributes (Interrupt)
//         0x20, 0x00,  // wMaxPacketSize 32
//         0x0A,        // bInterval 10 (unit depends on device speed)
//     };



