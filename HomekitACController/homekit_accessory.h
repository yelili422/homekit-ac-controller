#pragma once

#include <stdint.h>
#include <homekit/homekit.h>
#include <homekit/characteristics.h>

typedef struct
{
    float rotation_speed;
    uint8_t swing_mode;
    uint8_t current_fan_state;
    uint8_t target_fan_state;
} homekit_fan_state_t;

// typedef struct
// {
//     uint8_t slate_type;
//     uint8_t current_slat_state;
//     uint8_t swing_mode;
// } homekit_slat_state_t;

typedef struct
{
    uint8_t active;
    // current_temperature is the actual temperature of the room.
    float current_temperature;
    // target_temperature is the temperature setting of the A/C.
    float target_temperature;

    // NOTE:
    // The current_state and the target_state are not the same type
    // in homekit, so we have to keep them separate and sync them manually.
    uint8_t current_state;
    uint8_t target_state;

    homekit_fan_state_t fan;

    float current_humidity;
} homekit_ac_state_t;

inline void display_homekit_state(homekit_ac_state_t state)
{
    printf("AC State: active=%d, current_temp=%.1f, target_temp=%.1f, current_state=%d, target_state=%d\n",
           state.active, state.current_temperature, state.target_temperature, state.current_state, state.target_state);
    printf("Fan State: rotation_speed=%.1f, swing_mode=%d, current_fan_state=%d, target_fan_state=%d\n",
           state.fan.rotation_speed, state.fan.swing_mode, state.fan.current_fan_state, state.fan.target_fan_state);
}

#define ACTIVE_INACTIVE 0
#define ACTIVE_ACTIVE 1

#define AC_CURRENT_STATE_INACTIVE 0
#define AC_CURRENT_STATE_IDLE 1
#define AC_CURRENT_STATE_HEATING 2
#define AC_CURRENT_STATE_COOLING 3

#define AC_TARGET_STATE_AUTO 0
#define AC_TARGET_STATE_HEATING 1
#define AC_TARGET_STATE_COOLING 2

#define FAN_CURRENT_STATE_INACTIVE 0
#define FAN_CURRENT_STATE_IDLE 1
#define FAN_CURRENT_STATE_BLOWING 2

#define FAN_TARGET_STATE_MANUAL 0
#define FAN_TARGET_STATE_AUTO 1

#define SLATE_TYPE_HORIZONTAL 0
#define SLATE_TYPE_VERTICAL 1

#define SLATE_CURRENT_STATE_FIXED 0
#define SLATE_CURRENT_STATE_JAMMED 1
#define SLATE_CURRENT_STATE_SWINGING 2

#define SWING_MODE_DISABLED 0
#define SWING_MODE_ENABLED 1

extern homekit_server_config_t homekit_config;
extern homekit_ac_state_t homekit_state;
