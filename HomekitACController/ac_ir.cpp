#include "ac_ir.h"

const int DATA_START = 4096 - 64;

uint8_t convert_target_state(uint8_t homekit_mod)
{
    switch (homekit_mod)
    {
    case AC_TARGET_STATE_HEATING:
        return kCoolixHeat;
    case AC_TARGET_STATE_COOLING:
        return kCoolixCool;
    case AC_TARGET_STATE_AUTO:
    default:
        return kCoolixAuto;
    }
}

uint8_t convert_fan_speed(float homekit_fan_speed)
{
    uint8_t fan_speed = map(homekit_fan_speed, 0, 100, 0, 3);
    switch (fan_speed)
    {
    case 0:
        return kCoolixFanMin;
    case 1:
        return kCoolixFanMed;
    case 2:
    case 3:
        return kCoolixFanMax;
    default:
        return kCoolixFanAuto;
    }
}

ACController::ACController(uint16_t pin_ir_led) : ir(pin_ir_led)
{
    current_state = homekit_state;

    printf("Initializing ir controller at pin %d\n", pin_ir_led);
    ir.begin();
}

void ACController::sync(homekit_ac_state_t homekit_state)
{
    printf("Syncing A/C with latest state...\n");
    printf("Current state: \n");
    display_homekit_state(current_state);
    printf("Target state: \n");
    display_homekit_state(homekit_state);

    if (current_state.active != homekit_state.active)
    {
        homekit_state.active ? ir.on() : ir.off();
        goto ir_send;
    }

    if (current_state.target_temperature != homekit_state.target_temperature)
    {
        ir.on();
        ir.setTemp(homekit_state.target_temperature);
        goto ir_send;
    }

    if (current_state.target_state != homekit_state.target_state)
    {
        ir.on();
        ir.setMode(convert_target_state(homekit_state.target_state));
        goto ir_send;
    }

    if (current_state.fan.rotation_speed != homekit_state.fan.rotation_speed)
    {
        ir.on();
        ir.setMode(convert_target_state(homekit_state.target_state));
        ir.setFan(convert_fan_speed(homekit_state.fan.rotation_speed), false);
        goto ir_send;
    }

    if (current_state.fan.target_fan_state != homekit_state.fan.target_fan_state)
    {
        if (homekit_state.fan.target_fan_state == FAN_TARGET_STATE_AUTO)
        {
            ir.on();
            ir.setMode(convert_target_state(homekit_state.target_state));
            ir.setFan(kCoolixFanAuto, false);
            goto ir_send;
        }
    }

    // homekit_state.fan.swing_mode == AC_TARGET_STATE_AUTO ? ir.setSwing(kCoolixSwing) : ir.setSwing(kCoolixOff);

    return;

ir_send:
    current_state = homekit_state;
    printf("Sending IR command...\n");
    ir.send();
}
