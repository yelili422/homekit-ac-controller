#pragma once

#include <EEPROM.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <ir_Coolix.h>
#include "homekit_accessory.h"

class ACController
{
public:
    ACController(uint16_t pin_ir_led);

    // Sync the A/C state with the new state by sending IR commands.
    void sync(homekit_ac_state_t new_state);

private:
    IRCoolixAC ir;
    homekit_ac_state_t current_state;
};
