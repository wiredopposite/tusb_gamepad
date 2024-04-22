#include "Gamepad.h"
#include "board_config.h"

void reset_buttons(Gamepad *gamepad) 
{
    memset(&(gamepad->buttons), 0, sizeof(GamepadButtons));
    memset(&(gamepad->analog_buttons), 0, sizeof(GamepadAnalogButtons));
}

void reset_triggers(Gamepad *gamepad) 
{
    memset(&(gamepad->triggers), 0, sizeof(GamepadTriggers));
}

void reset_joysticks(Gamepad *gamepad) 
{
    memset(&(gamepad->joysticks), 0, sizeof(GamepadJoysticks));
}

void reset_pad(Gamepad *gamepad) 
{
    memset(&(gamepad->buttons), 0, sizeof(GamepadButtons));
    memset(&(gamepad->analog_buttons), 0, sizeof(GamepadAnalogButtons));
    memset(&(gamepad->triggers), 0, sizeof(GamepadTriggers));
    memset(&(gamepad->joysticks), 0, sizeof(GamepadJoysticks));
}

void reset_rumble(Gamepad *gamepad) 
{
    memset(&(gamepad->rumble), 0, sizeof(GamepadRumble));
}

void initialize_gamepad(Gamepad *gamepad) 
{
    gamepad->use_analog_buttons = false;
    
    gamepad->reset_pad       = reset_pad;
    gamepad->reset_buttons   = reset_buttons;
    gamepad->reset_triggers  = reset_triggers;
    gamepad->reset_joysticks = reset_joysticks;
    gamepad->reset_rumble    = reset_rumble;

    gamepad->reset_pad(gamepad);
    gamepad->reset_rumble(gamepad);
}

Gamepad *gamepad(int idx) 
{
    static Gamepad gamepads[MAX_GAMEPADS];
    static bool initialized[MAX_GAMEPADS] = {false};

    if (idx < 0 || idx >= MAX_GAMEPADS)
    {
        return NULL;
    }

    if (!initialized[idx]) 
    {
        initialize_gamepad(&gamepads[idx]);
        initialized[idx] = true;
    }

    return &gamepads[idx];
}