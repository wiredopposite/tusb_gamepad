#ifndef _GAMEPAD_H_
#define _GAMEPAD_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <string.h> 
#include <stdbool.h>

typedef struct 
{
	bool up 	;
	bool down 	;
	bool left 	;
	bool right 	;
	bool a		;
	bool b 		;
	bool x 		;
	bool y 		;
	bool l3		;
	bool r3 	;
	bool back 	;
	bool start 	;
	bool rb 	;
	bool lb 	;
	bool sys 	;
	bool misc	;
} GamepadButtons;

// typedef struct 
// {
// 	uint16_t up 	: 1;
// 	uint16_t down 	: 1;
// 	uint16_t left 	: 1;
// 	uint16_t right 	: 1;
// 	uint16_t a		: 1;
// 	uint16_t b 		: 1;
// 	uint16_t x 		: 1;
// 	uint16_t y 		: 1;
// 	uint16_t l3		: 1;
// 	uint16_t r3 	: 1;
// 	uint16_t back 	: 1;
// 	uint16_t start 	: 1;
// 	uint16_t rb 	: 1;
// 	uint16_t lb 	: 1;
// 	uint16_t sys 	: 1;
// 	uint16_t misc	: 1;
// } __attribute__((packed, aligned(2))) GamepadButtons;

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
typedef void (*GamepadResetRumbleFunc)(Gamepad*);

struct Gamepad 
{
	GamepadButtons 	 buttons;
	GamepadTriggers  triggers;
	GamepadJoysticks joysticks;
    GamepadRumble 	 rumble;

    GamepadResetPadFunc reset_pad;
    GamepadResetRumbleFunc reset_rumble;
};

void reset_pad(Gamepad *gamepad);
void reset_rumble(Gamepad *gamepad);
void initialize_gamepad(Gamepad *gamepad);
Gamepad *gamepad(int idx);

#ifdef __cplusplus
}
#endif

#endif // _GAMEPAD_H_
