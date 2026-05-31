#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

const int pinCLK = 19;
const int pinDT = 18;
const int pinSW = 23;
const int pinCup = 4;

const int pump1 = 25;
const int pump2 = 26;
const int pump3 = 27;

struct Cocktail {
    int id;
    String name;
    String description;
};

struct RecipeStep {
    int cocktail_id;
    int pump_number;
    unsigned long pour_time_ms;
};

struct OrderLog {
    int order_id;
    int cocktail_id;
    String source;
    unsigned long timestamp_ms;
};

enum State {
    MENU,
    POURING,
    PAUSED,
    DONE,
    ERROR_NO_CUP
};

const int totalMenuItems = 6;
extern Cocktail local_db_cocktails[totalMenuItems];

const int totalRecipeSteps = 9;
extern RecipeStep local_db_recipes[totalRecipeSteps];

const int maxLogEntries = 50;
extern OrderLog local_db_orders_log[maxLogEntries];
extern int totalOrdersMade;

extern State currentState;
extern int currentMenuItem;
extern int activeCocktailID;
extern bool screenNeedsUpdate;
extern String currentOrderSource;

void logOrderToCSV(int orderId, int cocktailId, String cocktailName, String source);

#endif