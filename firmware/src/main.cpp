#include <Arduino.h>

// Hardware Pin Definitions (Matching PCB Schematic)
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

// FreeRTOS Task Handles
TaskHandle_t xInputTaskHandle  = NULL;
TaskHandle_t xStatusTaskHandle = NULL;

void toggleRelay(int index) {
    if (index < 0 || index >= 4) return;
    relayStates[index] = !relayStates[index];
    digitalWrite(relayPins[index], relayStates[index] ? HIGH : LOW);
    
    Serial.print("Relay ");
    Serial.print(index + 1);
    Serial.println(relayStates[index] ? " ON" : " OFF");
}

// FreeRTOS Task 1: Read Switch Inputs with Debounce
void inputTask(void *pvParameters) {
    while (true) {
        for (int i = 0; i < 4; i++) {
            bool reading = digitalRead(switchPins[i]);
            if (reading != switchStates[i]) {
                switchStates[i] = reading;
                if (reading == LOW) { // Switch pressed (pull-up active low)
                    toggleRelay(i);
                }
            }
        }
        vTaskDelay(pdMS_TO_TICKS(30));
    }
}

// FreeRTOS Task 2: Heartbeat Status LED Blink
void statusTask(void *pvParameters) {
    while (true) {
        digitalWrite(STATUS_LED_PIN, HIGH);
        vTaskDelay(pdMS_TO_TICKS(500));
        digitalWrite(STATUS_LED_PIN, LOW);
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

void setup() {
    Serial.begin(115200);
    delay(200);

    Serial.println("Smart Home PCB");
    Serial.println("Firmware: 0.1");
    Serial.println("Starting...");

    // Initialize Relays (Default SAFE OFF State)
    for (int i = 0; i < 4; i++) {
        pinMode(relayPins[i], OUTPUT);
        digitalWrite(relayPins[i], LOW);
    }
    Serial.println("Relay OK");

    // Initialize Switch Inputs
    for (int i = 0; i < 4; i++) {
        pinMode(switchPins[i], INPUT_PULLUP);
    }
    Serial.println("Switches OK");

    // Initialize Status LED
    pinMode(STATUS_LED_PIN, OUTPUT);
    digitalWrite(STATUS_LED_PIN, LOW);

    Serial.println("Ready");

    // Create FreeRTOS tasks
    xTaskCreate(inputTask,  "InputTask",  2048, NULL, 2, &xInputTaskHandle);
    xTaskCreate(statusTask, "StatusTask", 1024, NULL, 1, &xStatusTaskHandle);
}

void loop() {
    vTaskDelay(pdMS_TO_TICKS(1000));
}
