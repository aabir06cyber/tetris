#pragma once
#include "common.h"

// Board Rendering
void DrawBoard();   // Draw the active 20x10 board

// Piece Rendering (UI)
void DrawTetromino(Tetromino piece); // Generic function for drawing any tetromino
void DrawActivePiece(); // Draw the current falling piece
void DrawNextPiecePreview();    // Draw the next piece in a side panel
void DrawGhost();   // Draw a semi-transparent prediction of where the piece might land

// State Overlays and Feedback
void GameStatsDisplay();    // Display the current score, level, lines cleared, etc. info.
void DrawMenu();    // Draw the MENU, the start screen
void DrawPausedState(); // Pause game and draw a light text overlay
void DrawGameOver();    // Draw the game over screen, display final score

/* *** BeginDrawing() and EndDrawing() [from Raylib] will be called in tetrs_main.c, not in renderer.c *** */
