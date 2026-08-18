#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Screen dimensions
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Pins matching schematic
#define RELAY_1_PIN   25
#define RELAY_2_PIN   23
#define RELAY_3_PIN   19
#define RELAY_4_PIN   18

#define BUTTON_PREV_PIN 13 // SW3
#define BUTTON_SEL_PIN  12 // SW4
#define BUTTON_NEXT_PIN 27 // SW5

#define STATUS_LED_PIN 2
#define I2C_SDA_PIN    21
#define I2C_SCL_PIN    22

// Hardware state
const uint8_t relayPins[4] = { RELAY_1_PIN, RELAY_2_PIN, RELAY_3_PIN, RELAY_4_PIN };
bool relayStates[4] = { false, false, false, false };

int currentSelectedRelay = 0;
int currentMenuPage = 0; // 0: Main Status, 1: Relay Control

// FreeRTOS task handles
TaskHandle_t xDisplayTaskHandle = NULL;
TaskHandle_t xButtonTaskHandle  = NULL;

// Helper functions
void updateDisplay();
void toggleRelay(int index);

// Task 1: OLED Display Update Loop
void displayTask(void *pvParameters) {
    while (true) {
        updateDisplay();
        vTaskDelay(pdMS_TO_TICKS(150));
    }
}

// Task 2: Physical Buttons Read & Debounce Loop
void buttonTask(void *pvParameters) {
    bool lastPrev = HIGH;
    bool lastSel  = HIGH;
    bool lastNext = HIGH;

    while (true) {
        bool readingPrev = digitalRead(BUTTON_PREV_PIN);
        bool readingSel  = digitalRead(BUTTON_SEL_PIN);
        bool readingNext = digitalRead(BUTTON_NEXT_PIN);

        // Prev button press (SW3)
        if (lastPrev == HIGH && readingPrev == LOW) {
            if (currentMenuPage == 0) {
                currentMenuPage = 1;
            } else {
                currentSelectedRelay = (currentSelectedRelay + 3) % 4;
            }
            vTaskDelay(pdMS_TO_TICKS(50));
        }

        // Next button press (SW5)
        if (lastNext == HIGH && readingNext == LOW) {
            if (currentMenuPage == 0) {
                currentMenuPage = 1;
            } else {
                currentSelectedRelay = (currentSelectedRelay + 1) % 4;
            }
            vTaskDelay(pdMS_TO_TICKS(50));
        }

        // Select button press (SW4) -> Toggle Relay
        if (lastSel == HIGH && readingSel == LOW) {
            if (currentMenuPage == 0) {
                currentMenuPage = 1;
            } else {
                toggleRelay(currentSelectedRelay);
            }
            vTaskDelay(pdMS_TO_TICKS(50));
        }

        lastPrev = readingPrev;
        lastSel  = readingSel;
        lastNext = readingNext;

        vTaskDelay(pdMS_TO_TICKS(20));
    }
}

void toggleRelay(int index) {
    if (index < 0 || index >= 4) return;
    relayStates[index] = !relayStates[index];
    digitalWrite(relayPins[index], relayStates[index] ? HIGH : LOW);
    
    Serial.print("Relay ");
    Serial.print(index + 1);
    Serial.println(relayStates[index] ? " ON" : " OFF");
}

void updateDisplay() {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);

    if (currentMenuPage == 0) {
        // Page 0: Main Overview
        display.setCursor(16, 0);
        display.println("Smart Home PCB");
        display.drawLine(0, 10, 128, 10, SSD1306_WHITE);

        display.setCursor(0, 16);
        display.print("R1: "); display.print(relayStates[0] ? "ON " : "OFF");
        display.setCursor(68, 16);
        display.print("R2: "); display.print(relayStates[1] ? "ON " : "OFF");

        display.setCursor(0, 28);
        display.print("R3: "); display.print(relayStates[2] ? "ON " : "OFF");
        display.setCursor(68, 28);
        display.print("R4: "); display.print(relayStates[3] ? "ON " : "OFF");

        display.setCursor(0, 42);
        display.print("Uptime: ");
        display.print(millis() / 1000);
        display.print("s");

        display.setCursor(0, 54);
        display.println("[Press SEL for Menu]");
    } else {
        // Page 1: Relay Control Menu
        display.setCursor(20, 0);
        display.println("Control Relays");
        display.drawLine(0, 10, 128, 10, SSD1306_WHITE);

        for (int i = 0; i < 4; i++) {
            int y = 14 + (i * 10);
            if (i == currentSelectedRelay) {
                display.setCursor(0, y);
                display.print(">");
            }
            display.setCursor(12, y);
            display.print("Relay "); display.print(i + 1);
            display.setCursor(80, y);
            display.print(relayStates[i] ? "[ ON ]" : "[OFF ]");
        }

        display.setCursor(0, 56);
        display.print("< PREV   TOGGLE   NEXT >");
    }

    display.display();
}

void setup() {
    Serial.begin(115200);
    delay(200);

    Serial.println("Smart Home PCB");
    Serial.println("Firmware: 0.1");
    Serial.println("Starting...");

    // Relays
    for (int i = 0; i < 4; i++) {
        pinMode(relayPins[i], OUTPUT);
        digitalWrite(relayPins[i], LOW);
    }
    Serial.println("Relay OK");

    // Buttons
    pinMode(BUTTON_PREV_PIN, INPUT_PULLUP);
    pinMode(BUTTON_SEL_PIN,  INPUT_PULLUP);
    pinMode(BUTTON_NEXT_PIN, INPUT_PULLUP);
    Serial.println("Buttons OK");

    // Status LED
    pinMode(STATUS_LED_PIN, OUTPUT);
    digitalWrite(STATUS_LED_PIN, HIGH);

    // I2C & OLED
    Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println("OLED Warning: 0x3C not found");
    } else {
        Serial.println("OLED OK");
    }

    Serial.println("Ready");

    // Create FreeRTOS tasks
    xTaskCreate(displayTask, "DisplayTask", 2048, NULL, 1, &xDisplayTaskHandle);
    xTaskCreate(buttonTask,  "ButtonTask",  2048, NULL, 2, &xButtonTaskHandle);
}

void loop() {
    // Empty - work handled by FreeRTOS tasks
    vTaskDelay(pdMS_TO_TICKS(1000));
}
