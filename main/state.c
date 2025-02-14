#include <stdlib.h>
#include <stdbool.h>

#include "state.h"
#include "sensors.h"

keyboard_state_t device_state = 0;
volatile keyboard_state_t bt_state = KEYBOARD_STATE_BT_UNCONNECTED;

void update_state(keyboard_system_command_t command)
{
    jumper_states_t jumpers = read_jumpers();

    keyboard_state_t sensor_state = jumpers.calibration ? KEYBOARD_STATE_SENSOR_CALIBRATION : KEYBOARD_STATE_SENSOR_NORMAL;

    keyboard_state_t logging_state = jumpers.enhanced_logging ? KEYBOARD_STATE_SENSOR_LOGGING : 0;

    keyboard_state_t paused_state = (device_state & KEYBOARD_STATE_PAUSED);

    switch (command)
    {
    case KEYBOARD_COMMAND_OFF:
        paused_state = KEYBOARD_STATE_PAUSED;
        break;
    case KEYBOARD_COMMAND_ON:
        paused_state = 0;
        break;
    default:
        break;
    }

    keyboard_state_t new_state = bt_state | sensor_state | logging_state | paused_state;

    device_state = new_state;
}

void update_bt_state(keyboard_state_t new_state)
{
    switch (new_state)
    {
    case KEYBOARD_STATE_BT_UNCONNECTED:
    case KEYBOARD_STATE_BT_CONNECTED:
    case KEYBOARD_STATE_BT_PASSKEY_ENTRY:
        bt_state = new_state;
        break;
    default:
        break;
    }
}

void handle_keyboard_command(keyboard_system_command_t);
