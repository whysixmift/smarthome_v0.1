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
 * - Switch Inputs 1..4 (Terminal Block J3): GPIO 13, 12, 27, 14
 */

#include <Arduino.h>

#define RELAY_1_PIN   25
#define RELAY_2_PIN   23
#define RELAY_3_PIN   19
#define RELAY_4_PIN   18

#define SWITCH_1_PIN  13
#define SWITCH_2_PIN  12
#define SWITCH_3_PIN  27
#define SWITCH_4_PIN  14

#define STATUS_LED_PIN 2

const uint8_t relayPins[4]  = { RELAY_1_PIN, RELAY_2_PIN, RELAY_3_PIN, RELAY_4_PIN };
const uint8_t switchPins[4] = { SWITCH_1_PIN, SWITCH_2_PIN, SWITCH_3_PIN, SWITCH_4_PIN };

bool relayStates[4]  = { false, false, false, false };
bool switchStates[4] = { HIGH, HIGH, HIGH, HIGH };

void toggleRelay(int index) {
    if (index < 0 || index >= 4) return;
    relayStates[index] = !relayStates[index];
    digitalWrite(relayPins[index], relayStates[index] ? HIGH : LOW);
    
    Serial.print("Relay ");
    Serial.print(index + 1);
    Serial.println(relayStates[index] ? " ON" : " OFF");
}

void setup() {
    Serial.begin(115200);
    delay(200);

    Serial.println("Smart Home PCB");
    Serial.println("Firmware: 0.1");

    for (int i = 0; i < 4; i++) {
        pinMode(relayPins[i], OUTPUT);
        digitalWrite(relayPins[i], LOW);

        pinMode(switchPins[i], INPUT_PULLUP);
    }

    pinMode(STATUS_LED_PIN, OUTPUT);
    digitalWrite(STATUS_LED_PIN, LOW);

    Serial.println("Ready");
}

void loop() {
    for (int i = 0; i < 4; i++) {
        bool reading = digitalRead(switchPins[i]);
        if (reading != switchStates[i]) {
            switchStates[i] = reading;
            if (reading == LOW) {
                toggleRelay(i);
            }
        }
    }
    delay(30);
}
