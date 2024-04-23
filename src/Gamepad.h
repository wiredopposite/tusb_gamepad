#ifndef _GAMEPAD_H_
#define _GAMEPAD_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <string.h> 
#include <stdbool.h>

// typedef struct 
// {
// 	bool up 	;
// 	bool down 	;
// 	bool left 	;
// 	bool right 	;
// 	bool a		;
// 	bool b 		;
// 	bool x 		;
// 	bool y 		;
// 	bool l3		;
// 	bool r3 	;
// 	bool back 	;
// 	bool start 	;
// 	bool rb 	;
// 	bool lb 	;
// 	bool sys 	;
// 	bool misc	;
// } GamepadButtons;

// #define GP_BUTTON_UP    0x0001
// #define GP_BUTTON_DOWN  0x0002
// #define GP_BUTTON_LEFT  0x0004
// #define GP_BUTTON_RIGHT 0x0008
// #define GP_BUTTON_A     0x0010
// #define GP_BUTTON_B     0x0020
// #define GP_BUTTON_X     0x0040
// #define GP_BUTTON_Y     0x0080
// #define GP_BUTTON_L3    0x0100
// #define GP_BUTTON_R3    0x0200
// #define GP_BUTTON_BACK  0x0400
// #define GP_BUTTON_START 0x0800
// #define GP_BUTTON_LB    0x1000
// #define GP_BUTTON_RB    0x2000
// #define GP_BUTTON_SYS   0x4000
// #define GP_BUTTON_MISC  0x8000

typedef struct 
{
	uint16_t up 	: 1;
	uint16_t down 	: 1;
	uint16_t left 	: 1;
	uint16_t right 	: 1;
	uint16_t a		: 1;
	uint16_t b 		: 1;
	uint16_t x 		: 1;
	uint16_t y 		: 1;
	uint16_t l3		: 1;
	uint16_t r3 	: 1;
	uint16_t back 	: 1;
	uint16_t start 	: 1;
	uint16_t lb 	: 1;
	uint16_t rb 	: 1;
	uint16_t sys 	: 1;
	uint16_t misc	: 1;
} __attribute__((packed, aligned(2))) GamepadButtons;

typedef struct 
{
	uint8_t up;
	uint8_t down;
	uint8_t left;
	uint8_t right;
	uint8_t a;
	uint8_t b;
	uint8_t x;
	uint8_t y;
	uint8_t lb;
	uint8_t rb;
} GamepadAnalogButtons;

typedef struct 
{
	uint8_t l;
	uint8_t r;
} GamepadTriggers;

typedef struct 
{
	int16_t lx;
	int16_t ly;
	int16_t rx;
	int16_t ry;
} GamepadJoysticks;

typedef struct 
{
	uint8_t l;
	uint8_t r;
} GamepadRumble;

typedef struct Gamepad Gamepad;

typedef void (*GamepadResetPadFunc)(Gamepad*);
typedef void (*GamepadResetButtonsFunc)(Gamepad*);
typedef void (*GamepadResetTriggersFunc)(Gamepad*);
typedef void (*GamepadResetJoysticksFunc)(Gamepad*);
typedef void (*GamepadResetRumbleFunc)(Gamepad*);

struct Gamepad 
{
	GamepadButtons 	 	 buttons;
	GamepadAnalogButtons analog_buttons;
	GamepadTriggers  	 triggers;
	GamepadJoysticks 	 joysticks;
    GamepadRumble 	 	 rumble;

	bool enable_analog_buttons;

    GamepadResetPadFunc 			reset_pad;
	GamepadResetButtonsFunc 		reset_buttons;
	GamepadResetTriggersFunc 		reset_triggers;
	GamepadResetJoysticksFunc 		reset_joysticks;
    GamepadResetRumbleFunc  		reset_rumble;
};

// void reset_pad(Gamepad *gamepad);
// void reset_buttons(Gamepad *gamepad);
// void reset_triggers(Gamepad *gamepad);
// void reset_joysticks(Gamepad *gamepad);
// void reset_rumble(Gamepad *gamepad);
// void initialize_gamepad(Gamepad *gamepad);
Gamepad *gamepad(int idx);

#ifdef __cplusplus
}
#endif

#endif // _GAMEPAD_H_
