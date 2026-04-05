#pragma once
#define viscols 10  // Grid Rows
#define gridrows 20  // Visible Height
#define totalheight 40  // Total Height of Board (20-cells buffer)

#define tile_size 40    // 40px of each tetromino

extern int offset_x;    // ***Defined in tetris_main.c by Srijato
extern int offset_y;    // ***Defined in tetris_main.c by Srijato

// Game State Management
enum GameState {MENU, PLAYING, PAUSED, GAMEOVER};
extern enum GameState gstate;   // Current Game State

// Tetromino Definitions
enum TetrominoType {I, J, L, O, S, T, Z};

typedef struct
{
    int x;  // X position of the tetromino on the grid
    int y;  // Y position of the tetromino on the grid
    enum TetrominoType type; // Type of the tetromino
    int rotation;   // Rotation state (0-3)
} Tetromino;

// Game statistics structure
typedef struct
{
    int score;
    int level;
    int lines_cleared;
} GameStats;
extern GameStats gamestats;  // Global game statistics

extern int board[totalheight][viscols];
extern const int shapes[7][4][4][4];
extern Tetromino current_piece; // Current falling piece
extern Tetromino next_piece;    // Next piece in queue
extern float frame_time;