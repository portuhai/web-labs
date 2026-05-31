#include "Config.h"
#include "BarLogics.h"
#include "DisplayManager.h"
#include "WebServerManager.h"

void setup() {
    Serial.begin(115200);
    initBarHardware();
    if (!initDisplay()) {
        Serial.println(F("SSD1306 allocation failed"));
        for (;;);
    }
    updateMenuDisplay();
    initWebServer();
}

void loop() {
    handleWebServer();
    handleEncoderMovement();
    checkButtonClicks();
    processButtonAction();
    runStateMachine();
}