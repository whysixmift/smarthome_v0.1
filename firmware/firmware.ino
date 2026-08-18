/*
 * ESP32 Smart Home 4-Channel Relay Controller Firmware
 * Board: ESP32 Dev Module
 * Framework: Arduino IDE / PlatformIO
 *
 * KiCad Schematic Pinout:
 * - Relay 1: GPIO 25
 * - Relay 2: GPIO 23
 * - Relay 3: GPIO 19
 * - Relay 4: GPIO 18
 * - Status LED: GPIO 2
 * - Switch 1..4: GPIO 13, 12, 27, 14
 * - I2C: SDA GPIO 21, SCL GPIO 22
 */

#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include <Preferences.h>
#include <ArduinoOTA.h>
#include <ArduinoJson.h>
#include <Wire.h>

#define RELAY_1_PIN      25
#define RELAY_2_PIN      23
#define RELAY_3_PIN      19
#define RELAY_4_PIN      18
#define NUM_RELAYS       4

#define STATUS_LED_PIN   2
#define SWITCH_1_PIN     13
#define SWITCH_2_PIN     12
#define SWITCH_3_PIN     27
#define SWITCH_4_PIN     14

#define I2C_SDA_PIN      21
#define I2C_SCL_PIN      22

AsyncWebServer server(80);
Preferences preferences;

const uint8_t relayPins[NUM_RELAYS]  = { RELAY_1_PIN, RELAY_2_PIN, RELAY_3_PIN, RELAY_4_PIN };
const uint8_t switchPins[NUM_RELAYS] = { SWITCH_1_PIN, SWITCH_2_PIN, SWITCH_3_PIN, SWITCH_4_PIN };

bool relayStates[NUM_RELAYS]  = { false, false, false, false };
bool switchStates[NUM_RELAYS] = { HIGH, HIGH, HIGH, HIGH };

void setRelay(uint8_t index, bool state) {
    if (index >= NUM_RELAYS) return;
    relayStates[index] = state;
    digitalWrite(relayPins[index], state ? HIGH : LOW);
    preferences.begin("relays", false);
    String key = "r" + String(index);
    preferences.putBool(key.c_str(), state);
    preferences.end();
}

void toggleRelay(uint8_t index) {
    if (index >= NUM_RELAYS) return;
    setRelay(index, !relayStates[index]);
}

void setup() {
    Serial.begin(115200);
    pinMode(STATUS_LED_PIN, OUTPUT);

    for (int i = 0; i < NUM_RELAYS; i++) {
        pinMode(relayPins[i], OUTPUT);
        digitalWrite(relayPins[i], LOW);
        pinMode(switchPins[i], INPUT_PULLUP);
    }

    preferences.begin("relays", false);
    for (int i = 0; i < NUM_RELAYS; i++) {
        String key = "r" + String(i);
        setRelay(i, preferences.getBool(key.c_str(), false));
    }
    preferences.end();

    Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);
    WiFi.mode(WIFI_AP_STA);
    WiFi.softAP("ESP32-SmartHome-AP", "12345678");

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(200, "text/plain", "ESP32 Smart Home Controller Ready");
    });

    server.on("/api/relay/toggle", HTTP_POST, [](AsyncWebServerRequest *request) {
        if (request->hasParam("id")) {
            int id = request->getParam("id")->value().toInt();
            toggleRelay(id);
        }
        request->send(200, "application/json", "{\"status\":\"ok\"}");
    });

    server.begin();
    ArduinoOTA.begin();
}

void loop() {
    ArduinoOTA.handle();
    for (int i = 0; i < NUM_RELAYS; i++) {
        bool reading = digitalRead(switchPins[i]);
        if (reading != switchStates[i]) {
            switchStates[i] = reading;
            if (reading == LOW) {
                toggleRelay(i);
            }
        }
    }
}
