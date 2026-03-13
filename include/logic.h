#pragma once
#include "common.h"
#include <stdbool.h>

// Board Management
void InitBoard();
bool CheckGameOver();

// Piece Management
bool isValidMove(Tetromino piece, int dx, int dy, int dr);  // Check if piece can move by (dx, dy) and rotate to dr (0-3)
void SRS(Tetromino *piece, int direction);  // Super Rotation System for wall kicks (direction: 1 CW, -1 CCW)
void LockPiece(Tetromino piece);    // Locks piece into the board
void InitStartPiece();  //
void SpawnNext();   // Spawns the next piece and updates the current piece

// Row Management
int ClearFullRows();    // Returns number of rows cleared