#ifndef BAR_LOGICS_H
#define BAR_LOGICS_H

#include "Config.h"

void initBarHardware();
void handleEncoderMovement();
void checkButtonClicks();
void processButtonAction();
void runStateMachine();
void allPumpsOff();
void startPouringSequence(int cocktailID, String source);
String getCocktailNameByID(int id);
void finishAndLogCocktail();
void IRAM_ATTR readEncoderISR();


#endif