#include "homekit_accessory.h"

homekit_ac_state_t homekit_state = {
    .active = ACTIVE_INACTIVE,
    .current_temperature = 0,
    .target_temperature = 25,
    .current_state = AC_CURRENT_STATE_INACTIVE,
    .target_state = AC_TARGET_STATE_COOLING,
    .fan = {
        .swing_mode = SWING_MODE_DISABLED,
        .rotation_speed = 0.0,
        .current_fan_state = FAN_CURRENT_STATE_IDLE,
        .target_fan_state = FAN_TARGET_STATE_AUTO,
    },
    .current_humidity = 50,
};

// Called to identify this accessory. See HAP section 6.7.6 Identify Routine
// Generally this is called when paired successfully or click
// the "Identify Accessory" button in Home APP.
void accessory_identify(homekit_value_t _value)
{
    printf("accessory identify\n");
}

void set_active(homekit_value_t value)
{
    homekit_state.active = value.uint8_value;
}

homekit_value_t get_active()
{
    return HOMEKIT_UINT8(homekit_state.active);
}

homekit_value_t get_current_state()
{
    if (homekit_state.active == ACTIVE_INACTIVE)
    {
        return HOMEKIT_UINT8(AC_CURRENT_STATE_INACTIVE);
    }

    return HOMEKIT_UINT8(homekit_state.current_state);
}

void set_target_state(homekit_value_t value)
{
    // NOTE:
    // We can't get the current state from the IR controller, so we have to
    // assume the current state is the same as the target state.
    switch (value.uint8_value)
    {
    case AC_TARGET_STATE_HEATING:
        homekit_state.current_state = AC_CURRENT_STATE_HEATING;
        homekit_state.target_state = AC_TARGET_STATE_HEATING;
        break;
    case AC_TARGET_STATE_AUTO:
    case AC_TARGET_STATE_COOLING:
        homekit_state.current_state = AC_CURRENT_STATE_COOLING;
        homekit_state.target_state = AC_TARGET_STATE_COOLING;
        break;
    default:
        break;
    }
}

homekit_value_t get_target_state()
{
    return HOMEKIT_UINT8(homekit_state.target_state);
}

homekit_value_t get_current_temperature()
{
    return HOMEKIT_FLOAT(homekit_state.current_temperature);
}

void set_target_temperature(homekit_value_t value)
{
    homekit_state.target_temperature = value.float_value;
}

homekit_value_t get_target_temperature()
{
    return HOMEKIT_FLOAT(homekit_state.target_temperature);
}

void set_fan_rotation_speed(homekit_value_t value)
{
    homekit_state.fan.rotation_speed = value.float_value;

    if (homekit_state.fan.rotation_speed < 0.1)
    {
        set_fan_target_state(HOMEKIT_UINT8(FAN_TARGET_STATE_AUTO));
        return;
    }

    set_fan_target_state(HOMEKIT_UINT8(FAN_TARGET_STATE_MANUAL));
}

homekit_value_t get_fan_rotation_speed()
{
    return HOMEKIT_FLOAT(homekit_state.fan.rotation_speed);
}

void set_fan_swing_mode(homekit_value_t value)
{
    homekit_state.fan.swing_mode = value.uint8_value;
}

homekit_value_t get_fan_swing_mode()
{
    if (homekit_state.active == ACTIVE_INACTIVE)
    {
        return HOMEKIT_UINT8(SWING_MODE_DISABLED);
    }

    return HOMEKIT_UINT8(homekit_state.fan.swing_mode);
}

homekit_value_t get_fan_current_state()
{
    return HOMEKIT_UINT8(homekit_state.fan.current_fan_state);
}

void set_fan_target_state(homekit_value_t value)
{
    homekit_state.fan.target_fan_state = value.uint8_value;

    // Updating the current fan state for display in the Home app.
    switch (homekit_state.fan.target_fan_state)
    {
    case FAN_TARGET_STATE_AUTO:
        homekit_state.fan.current_fan_state = FAN_CURRENT_STATE_IDLE;
        break;
    case FAN_TARGET_STATE_MANUAL:
        homekit_state.fan.current_fan_state = FAN_CURRENT_STATE_BLOWING;
        break;
    default:
        break;
    }
}

homekit_value_t get_fan_target_state()
{
    return HOMEKIT_UINT8(homekit_state.fan.target_fan_state);
}

homekit_value_t get_current_humidity()
{
    return HOMEKIT_FLOAT(homekit_state.current_humidity);
}

homekit_value_t get_humidity_active()
{
    return HOMEKIT_BOOL(true);
}

homekit_value_t get_humidity_fault()
{
    return HOMEKIT_UINT8(0);
}

homekit_characteristic_t cha_active = HOMEKIT_CHARACTERISTIC_(
    ACTIVE, ACTIVE_INACTIVE,
    .setter = set_active,
    .getter = get_active, );

// format: float; HAP section 9.35; min 0, max 100, step 0.1, unit celsius
homekit_characteristic_t cha_current_temperature = HOMEKIT_CHARACTERISTIC_(
    CURRENT_TEMPERATURE, (float)0,
    .getter = get_current_temperature, );

homekit_characteristic_t cha_ac_current_state = HOMEKIT_CHARACTERISTIC_(
    CURRENT_HEATER_COOLER_STATE, AC_CURRENT_STATE_INACTIVE,
    .getter = get_current_state, );

homekit_characteristic_t cha_ac_target_state = HOMEKIT_CHARACTERISTIC_(
    TARGET_HEATER_COOLER_STATE, AC_TARGET_STATE_AUTO,
    .setter = set_target_state,
    .getter = get_target_state, );

// format: float; HAP section 9.20; min 10, max 35, step 0.1, unit celsius
homekit_characteristic_t cha_cooling_threshold = HOMEKIT_CHARACTERISTIC_(
    COOLING_THRESHOLD_TEMPERATURE, 25,
    .setter = set_target_temperature,
    .getter = get_target_temperature, );

// format: float; HAP section 9.42; min 0, max 25, step 0.1, unit celsius
homekit_characteristic_t cha_heating_threshold = HOMEKIT_CHARACTERISTIC_(
    HEATING_THRESHOLD_TEMPERATURE, 25,
    .setter = set_target_temperature,
    .getter = get_target_temperature, );

// format: float; HAP section 9.81; min 0, max 100, step 1, unit percentage
homekit_characteristic_t cha_fan_rotation_speed = HOMEKIT_CHARACTERISTIC_(
    ROTATION_SPEED, (float)0,
    .setter = set_fan_rotation_speed,
    .getter = get_fan_rotation_speed, );

homekit_characteristic_t cha_fan_active = HOMEKIT_CHARACTERISTIC_(
    ACTIVE, ACTIVE_ACTIVE,
    .setter = set_active,
    .getter = get_active, );

homekit_characteristic_t cha_fan_swing_mode = HOMEKIT_CHARACTERISTIC_(
    SWING_MODE, SWING_MODE_DISABLED,
    .setter = set_fan_swing_mode,
    .getter = get_fan_swing_mode, );

homekit_characteristic_t cha_fan_current_state = HOMEKIT_CHARACTERISTIC_(
    CURRENT_FAN_STATE, FAN_CURRENT_STATE_INACTIVE,
    .getter = get_fan_current_state, );

homekit_characteristic_t cha_fan_target_state = HOMEKIT_CHARACTERISTIC_(
    TARGET_FAN_STATE, FAN_TARGET_STATE_AUTO,
    .setter = set_fan_target_state,
    .getter = get_fan_target_state, );

homekit_characteristic_t cha_humidity = HOMEKIT_CHARACTERISTIC_(
    CURRENT_RELATIVE_HUMIDITY, 0,
    .getter = get_current_humidity, );

homekit_characteristic_t cha_humidity_active = HOMEKIT_CHARACTERISTIC_(
    STATUS_ACTIVE, true,
    .getter = get_humidity_active, );

homekit_characteristic_t cha_humidity_fault = HOMEKIT_CHARACTERISTIC_(
    STATUS_FAULT, 0,
    .getter = get_humidity_fault, );

// Heater Cooler (HAP section 8.18)
//
// required:
//   - Active
//   - Current Temperature
//   - Current Heater Cooler State
//   - Target Heater Cooler State
//
// optional:
//   - Name
//   - Rotation Speed
//   - Temperature Display Units
//   - Swing Mode
//   - Cooling Threshold Temperature
//   - Heating Threshold Temperature
//   - Lock Physical Controls
homekit_accessory_t *accessories[] = {
    HOMEKIT_ACCESSORY(.id=1, .category=homekit_accessory_category_air_conditioner, .services=(homekit_service_t*[]){
        HOMEKIT_SERVICE(ACCESSORY_INFORMATION, .characteristics=(homekit_characteristic_t*[]){
            HOMEKIT_CHARACTERISTIC(NAME, "AC"),
            HOMEKIT_CHARACTERISTIC(MANUFACTURER, "Arduino"),
            HOMEKIT_CHARACTERISTIC(SERIAL_NUMBER, "0034114"),
            HOMEKIT_CHARACTERISTIC(MODEL, "ESP8266"),
            HOMEKIT_CHARACTERISTIC(FIRMWARE_REVISION, "0.1"),
            HOMEKIT_CHARACTERISTIC(IDENTIFY, accessory_identify),
            NULL
        }),
        HOMEKIT_SERVICE(HEATER_COOLER, .primary=true, .characteristics=(homekit_characteristic_t*[]){
            HOMEKIT_CHARACTERISTIC(NAME, "AC"),
            &cha_active,
            &cha_current_temperature,
            &cha_ac_current_state,
            &cha_ac_target_state,
            &cha_cooling_threshold,
            &cha_heating_threshold,
            // &cha_fan_rotation_speed,
            NULL
        }),
        HOMEKIT_SERVICE(FAN, .characteristics=(homekit_characteristic_t*[]) {
            HOMEKIT_CHARACTERISTIC(NAME, "AC Fan"),
            &cha_fan_active,
            &cha_fan_rotation_speed,
            &cha_fan_swing_mode,
            &cha_fan_current_state,
            &cha_fan_target_state,
            NULL
        }),
        HOMEKIT_SERVICE(HUMIDITY_SENSOR, .characteristics=(homekit_characteristic_t*[]){
            HOMEKIT_CHARACTERISTIC(NAME, "AC Humidity Sensor"),
            &cha_humidity,
            &cha_humidity_active,
            &cha_humidity_fault,
            NULL
        }),
        NULL
    }),
    NULL
};

homekit_server_config_t homekit_config = {
    .accessories = accessories,
    .password = "111-11-111"};
