#include <Arduino.h>
#include <arduino_homekit_server.h>
#include <DHT.h>
#include "wifi.h"
#include "homekit_accessory.h"
#include "ac_ir.h"
#include "button.h"

#define FLASH_BUTTON_PIN 0
#define D0 16
#define D1 5 // I2C Bus SCL (clock)
#define D2 4 // I2C Bus SDA (data)
#define D3 0
#define D4 2  // Same as "LED_BUILTIN", but inverted logic
#define D5 14 // SPI Bus SCK (clock)
#define D6 12 // SPI Bus MISO
#define D7 13 // SPI Bus MOSI
#define D8 15 // SPI Bus SS (CS)
#define D9 3  // RX0 (Serial console)
#define D10 1 // TX0 (Serial console)

#define DHT_TYPE DHT11

const uint16_t EEPROM_OFFSET = 1410;
const uint16_t HOMEKIT_STATE_VALID_ADDR = EEPROM_OFFSET;
const uint16_t HOMEKIT_STATE_ADDR = EEPROM_OFFSET + sizeof(uint8_t);

const uint8_t HOMEKIT_STATE_MAGIC = 0x35;

const uint16_t PIN_IR_LED = D2;
const uint16_t PIN_DHT = D4;

ACController ac(PIN_IR_LED);
DHT dht(PIN_DHT, DHT_TYPE);
Button hk_reset_btn(FLASH_BUTTON_PIN);

static uint32_t next_sync_millis = 0;

void update_current_temperature()
{
    // Reading temperature or humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    float h = dht.readHumidity();
    // Read temperature as Celsius (the default)
    float t = dht.readTemperature();

    // Check if any reads failed and exit early (to try again).
    if (isnan(h) || isnan(t))
    {
        Serial.println(F("!!! Failed to read from DHT sensor!"));
        return;
    }

    Serial.print(F("Humidity: "));
    Serial.print(h);
    Serial.print(F("%  Temperature: "));
    Serial.print(t);
    Serial.print(F("°C "));
    Serial.println();

    homekit_state.current_temperature = t;
    homekit_state.current_humidity = h;
}

void reset_homekit_storage()
{
    Serial.println("Resetting HomeKit storage");
    homekit_storage_reset();
}

void setup()
{
    Serial.begin(115200);

    hk_reset_btn.on_click(reset_homekit_storage);

    wifi_init();
    arduino_homekit_setup(&homekit_config);

    Serial.println("A/C controller is ready.");

    dht.begin();
    delay(2000);
}

void loop()
{
    hk_reset_btn.loop();

    arduino_homekit_loop();

    if (!homekit_is_paired())
    {
        return;
    }

    const uint32_t t = millis();
    if (t > next_sync_millis)
    {
        // show homekit info every 5 seconds
        next_sync_millis = t + 5 * 1000;
        Serial.printf("HomeKit clients: %d\n", arduino_homekit_connected_clients_count());

        // Sync A/C settings with the IR controller
        ac.sync(homekit_state);

        update_current_temperature();
    }
}
