#include "DisplayManager.h"
#include "BarLogics.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
bool initDisplay() {
    return display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
}

void updateMenuDisplay() {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(15, 0);
    display.println("- WIFI SMART BAR -");
    
    int startItem = (currentMenuItem / 3) * 3;
    for (int i = startItem; i < startItem + 3; i++) {
        if (i >= totalMenuItems)
            break;
        display.setCursor(5, 16 + ((i - startItem) * 15));
        if (i == currentMenuItem)
            display.print("> ");
        else
            display.print("  ");
        display.println(local_db_cocktails[i].name);
    }
    display.display();
}

void showPouringDisplay() {
    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);
    display.setCursor(10, 5);
    display.print("Src: ");
    display.println(currentOrderSource);
    display.setCursor(5, 22);
    display.println(getCocktailNameByID(activeCocktailID));
    
    display.setTextSize(2);
    display.setCursor(10, 42);
    display.println("POURING...");
    display.display();
}

void showPausedDisplay() {
    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(2);
    display.setCursor(25, 10);
    display.println("PAUSED");
    display.setTextSize(1);
    display.setCursor(10, 38);
    display.println("Cup removed");
    display.display();
}

void showErrorDisplay() {
    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(2);
    display.setCursor(25, 10);
    display.println("ERROR");
    display.setTextSize(1);
    display.setCursor(15, 38);
    display.println("No cup detected");
    display.display();
}

void showReadyDisplay() {
    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(2);
    display.setCursor(30, 15);
    display.println("READY");
    display.setTextSize(1);
    display.setCursor(10, 45);
    display.println("Logged into local DB");
    display.display();
}