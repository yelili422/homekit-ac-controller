#include "wifi.h"

const char *ssid = "SSID";
const char *password = "password";


void wifi_init() {
    WiFi.mode(WIFI_STA);
    WiFi.setAutoReconnect(true);
    WiFi.begin(ssid, password);

    Serial.println("Wifi connecting...");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println();
    Serial.printf("Wifi connected, IP: %s\n", WiFi.localIP().toString().c_str());
}
