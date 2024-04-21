#ifndef _TUSB_GAMEPAD_H_
#define _TUSB_GAMEPAD_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "inputmodes.h"
#include "Gamepad.h"
#include "board_config.h"

void init_tusb_gamepad(enum InputMode mode);
void tusb_gamepad_task();

#ifdef __cplusplus
}
#endif

#endif // _TUSB_GAMEPAD_H_