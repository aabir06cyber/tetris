#pragma once
#include "common.h"

// Input Processing
void PlayerInputControl();  // Handle player input for moving, rotating, and dropping pieces (gstate == PLAYING)
void MenuInputControl();    // Handle input for navigating the menu and starting the game (gstate == MENU/GAMEOVER)