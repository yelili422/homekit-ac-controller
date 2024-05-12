
# Homekit A/C Remote Controller

A simple Homekit AC remote controller made by ESP-8266.

## Supported devices

- Midea(Coolix protocol)

## Development

### 1. Install Dependencies
```shell
arduino-cli config init
arduino-cli config add board_manager.additional_urls "http://arduino.esp8266.com/stable/package_esp8266com_index.json"
arduino-cli core install esp8266:esp8266
arduino-cli lib install IRremoteESP8266 HomeKit-ESP8266 "DHT sensor library"
```

### 2. Compile and Upload
```shell
arduino-cli compile --fqbn esp8266:esp8266:nodemcuv2 HomekitACController.ino
arduino-cli upload -p /dev/cu.usbserial-8310 --fqbn esp8266:esp8266:nodemcuv2
```

