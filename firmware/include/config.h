#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// ==========================================
// HARDWARE PIN ASSIGNMENTS (KiCad Schematic)
// ==========================================

// Relay Driver Outputs (via ULN2003A)
#define RELAY_1_PIN      25  // Channel 1 Relay & Status LED D6
#define RELAY_2_PIN      23  // Channel 2 Relay & Status LED D7
#define RELAY_3_PIN      19  // Channel 3 Relay & Status LED D8
#define RELAY_4_PIN      18  // Channel 4 Relay & Status LED D9
#define NUM_RELAYS       4

// Status & Indicators
#define STATUS_LED_PIN   2   // System Status LED D5
#define BOOT_BUTTON_PIN  0   // Tactile Boot Switch SW2

// External Wall Switch / Digital Inputs
#define SWITCH_1_PIN     13  // Switch Input 1
#define SWITCH_2_PIN     12  // Switch Input 2
#define SWITCH_3_PIN     27  // Switch Input 3
#define SWITCH_4_PIN     14  // Switch Input 4

// I2C Expansion Header (J4)
#define I2C_SDA_PIN      21  // Data
#define I2C_SCL_PIN      22  // Clock

// Auxiliary Expansion GPIO
#define EXTRA_IO_PIN     17  // General Purpose IO

// ==========================================
// DEFAULT NETWORK & SYSTEM SETTINGS
// ==========================================

#define HOSTNAME_PREFIX     "SmartHome-ESP32"
#define DEFAULT_AP_SSID     "ESP32-SmartHome-AP"
#define DEFAULT_AP_PASS     "12345678"

#define HTTP_PORT           80
#define MQTT_PORT           1883
#define DEFAULT_MQTT_TOPIC  "smarthome/esp32"

#define DEBOUNCE_DELAY_MS   50   // Switch debounce time
#define RECONNECT_INTERVAL  5000 // Wi-Fi / MQTT reconnect timer (ms)

#endif // CONFIG_H
