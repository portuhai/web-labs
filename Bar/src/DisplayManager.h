#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

#include "Config.h"

bool initDisplay();
void updateMenuDisplay();
void showPouringDisplay();
void showPausedDisplay();
void showErrorDisplay();
void showReadyDisplay();

#endif