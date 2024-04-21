#include "Gamepad.h"
#include "board_config.h"

static Gamepad gamepad_array[MAX_GAMEPADS];
static bool initialized[MAX_GAMEPADS] = {0}; 

void reset_pad(Gamepad *gamepad) 
{
    memset(&(gamepad->buttons), 0, sizeof(GamepadButtons));
    memset(&(gamepad->triggers), 0, sizeof(GamepadTriggers));
    memset(&(gamepad->joysticks), 0, sizeof(GamepadJoysticks));
}

void reset_rumble(Gamepad *gamepad) 
{
    gamepad->rumble.l = 0;
    gamepad->rumble.r = 0;
}

void initialize_gamepad(Gamepad *gamepad) 
{
    gamepad->reset_pad = reset_pad;
    gamepad->reset_rumble = reset_rumble;
}

Gamepad *gamepad(int idx) 
{
    if (idx < 0 || idx >= MAX_GAMEPADS)
    {
        return NULL;
    }

    if (!initialized[idx]) 
    {
        initialize_gamepad(&gamepad_array[idx]);
        initialized[idx] = true;
    }

    return &gamepad_array[idx];
}