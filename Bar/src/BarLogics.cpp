#include "BarLogics.h"
#include "DisplayManager.h"
#include <LittleFS.h>

Cocktail local_db_cocktails[totalMenuItems] = {
    {1, "Whiskey-Cola", "Classic club mix"},
    {2, "Pure Juice", "100 percent Apple juice"},
    {3, "Strong Mix", "All pumps together"},
    {4, "Double Whiskey", "Double shot of alcohol"},
    {5, "Sweet Cola", "Pure cola drink"},
    {6, "Apple Custom", "Juice with cola splash"}};

RecipeStep local_db_recipes[totalRecipeSteps] = {
    {1, 1, 3000}, {1, 2, 5000}, {2, 3, 6000}, {3, 99, 4000}, {4, 1, 6000}, {5, 2, 7000}, {6, 3, 4000}, {6, 2, 2000}};

OrderLog local_db_orders_log[maxLogEntries];
int totalOrdersMade = 0;

State currentState = MENU;
int currentMenuItem = 0;
int activeCocktailID = -1;
bool screenNeedsUpdate = true;
String currentOrderSource = "Encoder";

volatile bool encoderMoved = false;
volatile int encoderDirection = 0;
unsigned long lastButtonPress = 0;
int buttonClickCount = 0;
unsigned long firstClickTime = 0;

unsigned long actionStartTime = 0;
unsigned long totalPourTime = 0;
unsigned long elapsedTime = 0;
int currentStepIndex = 0;

void IRAM_ATTR readEncoderISR() {
    if (digitalRead(pinDT) != digitalRead(pinCLK)) {
        encoderDirection = 1;
    } else {
        encoderDirection = -1;
    }
    encoderMoved = true;
}

void initBarHardware() {
    pinMode(pinCLK, INPUT);
    pinMode(pinDT, INPUT);
    pinMode(pinSW, INPUT_PULLUP);
    pinMode(pinCup, INPUT);

    pinMode(pump1, OUTPUT);
    pinMode(pump2, OUTPUT);
    pinMode(pump3, OUTPUT);
    allPumpsOff();

    attachInterrupt(digitalPinToInterrupt(pinCLK), readEncoderISR, RISING);
}

void activatePumps(int pump) {
    if (pump == 1)
        digitalWrite(pump1, HIGH);
    else if (pump == 2)
        digitalWrite(pump2, HIGH);
    else if (pump == 3)
        digitalWrite(pump3, HIGH);
    else if (pump == 99) {
        digitalWrite(pump1, HIGH);
        digitalWrite(pump2, HIGH);
        digitalWrite(pump3, HIGH);
    }
}

void allPumpsOff() {
    digitalWrite(pump1, LOW);
    digitalWrite(pump2, LOW);
    digitalWrite(pump3, LOW);
}

bool findNextRecipeStep() {
    int stepsFound = 0;
    for (int i = 0; i < totalRecipeSteps; i++) {
        if (local_db_recipes[i].cocktail_id == activeCocktailID) {
            if (stepsFound == currentStepIndex) {
                totalPourTime = local_db_recipes[i].pour_time_ms;
                activatePumps(local_db_recipes[i].pump_number);
                return true;
            }
            stepsFound++;
        }
    }
    return false;
}

void startPouringSequence(int cocktailID, String source) {
    activeCocktailID = cocktailID;
    currentStepIndex = 0;
    elapsedTime = 0;
    currentOrderSource = source;
    currentState = POURING;
    screenNeedsUpdate = true;
    actionStartTime = millis();
    findNextRecipeStep();
}

void handleEncoderMovement() {
    if (!encoderMoved || currentState != MENU)
        return;

    currentMenuItem += encoderDirection;
    if (currentMenuItem >= totalMenuItems)
        currentMenuItem = 0;
    else if (currentMenuItem < 0)
        currentMenuItem = totalMenuItems - 1;

    encoderMoved = false;
    updateMenuDisplay();
}

void checkButtonClicks() {
    if (digitalRead(pinSW) == LOW) {
        if (millis() - lastButtonPress > 250) {
            if (buttonClickCount == 0) {
                firstClickTime = millis();
            }
            buttonClickCount++;
            lastButtonPress = millis();
        }
    }
}

void executeSingleClick() {
    if (currentState == MENU) {
        if (digitalRead(pinCup) == HIGH) {
            int id = local_db_cocktails[currentMenuItem].id;
            startPouringSequence(id, "Encoder");
        } else {
            currentState = ERROR_NO_CUP;
            actionStartTime = millis();
            screenNeedsUpdate = true;
        }
    }
}

void printSerialLogs() {
    Serial.println("\n===== LOCAL DB ORDERS LOG =====");
    if (totalOrdersMade == 0) {
        Serial.println("Log is empty. No orders made yet.");
    } else {
        for (int i = 0; i < totalOrdersMade; i++) {
            Serial.print("Order #");
            Serial.print(local_db_orders_log[i].order_id);
            Serial.print(" | Cocktail ID: ");
            Serial.print(local_db_orders_log[i].cocktail_id);
            Serial.print(" | Src: ");
            Serial.print(local_db_orders_log[i].source);
            Serial.print(" | Time: ");
            Serial.print(local_db_orders_log[i].timestamp_ms / 1000.0, 1);
            Serial.println(" s");
        }
    }
    Serial.println("==================================\n");
}

void processButtonAction() {
    if (buttonClickCount > 0 && (millis() - firstClickTime > 400)) {
        if (buttonClickCount == 1) {
            executeSingleClick();
        } else if (buttonClickCount >= 2) {
            printSerialLogs();
        }
        buttonClickCount = 0;
    }
}

void handlePouringState(bool glassPresent) {
    if (screenNeedsUpdate) {
        showPouringDisplay();
        screenNeedsUpdate = false;
    }
    if (!glassPresent) {
        allPumpsOff();
        elapsedTime += (millis() - actionStartTime);
        currentState = PAUSED;
        screenNeedsUpdate = true;
        return;
    }
    if (millis() - actionStartTime + elapsedTime >= totalPourTime) {
        allPumpsOff();
        elapsedTime = 0;
        currentStepIndex++;
        if (!findNextRecipeStep()) {
            finishAndLogCocktail();
        } else {
            actionStartTime = millis();
        }
    }
}

void handleErrorState() {
    if (screenNeedsUpdate) {
        showErrorDisplay();
        screenNeedsUpdate = false;
    }
    if (millis() - actionStartTime >= 3000) {
        currentState = MENU;
        updateMenuDisplay();
    }
}

void handleDoneState() {
    if (screenNeedsUpdate) {
        showReadyDisplay();
        screenNeedsUpdate = false;
    }
    if (millis() - actionStartTime >= 4000) {
        currentState = MENU;
        updateMenuDisplay();
    }
}

void runStateMachine() {
    bool glassPresent = (digitalRead(pinCup) == HIGH);

    switch (currentState) {
        case ERROR_NO_CUP:
            handleErrorState();
            break;
        case POURING:
            handlePouringState(glassPresent);
            break;
        case PAUSED:
            if (screenNeedsUpdate) {
                showPausedDisplay();
                screenNeedsUpdate = false;
            }
            if (glassPresent) {
                currentState = POURING;
                actionStartTime = millis();
                screenNeedsUpdate = true;
                findNextRecipeStep();
            }
            break;
        case DONE:
            handleDoneState();
            break;
        case MENU:
            break;
    }
}

String getCocktailNameByID(int id) {
    for (int j = 0; j < totalMenuItems; j++) {
        if (local_db_cocktails[j].id == id)
            return local_db_cocktails[j].name;
    }
    return "Unknown";
}

void logOrderToCSV(int orderId, int cocktailId, String cocktailName, String source) {
    File logFile = LittleFS.open("/orders.csv", "a");
    if (!logFile) {
        Serial.println("No orders.csv");
        return;
    }

    logFile.print(String(orderId) + ",");
    logFile.print(String(cocktailId) + ",");
    logFile.print(cocktailName + ",");
    logFile.print(source + ",");
    logFile.println(String(millis())); 

    logFile.close();
    Serial.println("Success");
}

void finishAndLogCocktail() {
    allPumpsOff();
    currentState = DONE;
    actionStartTime = millis();
    screenNeedsUpdate = true;

    int newOrderId = totalOrdersMade + 1;
    String cocktailName = getCocktailNameByID(activeCocktailID);

    if (totalOrdersMade < maxLogEntries) {
        local_db_orders_log[totalOrdersMade].order_id = newOrderId;
        local_db_orders_log[totalOrdersMade].cocktail_id = activeCocktailID;
        local_db_orders_log[totalOrdersMade].source = currentOrderSource;
        local_db_orders_log[totalOrdersMade].timestamp_ms = millis();
        totalOrdersMade++;
        Serial.println("Success");
    } else {
        Serial.println("Only file will accept logs");
    }

    logOrderToCSV(newOrderId, activeCocktailID, cocktailName, currentOrderSource);
}