#include "common.h"
#include "logic.h"
#include <string.h>
#include <stdlib.h>

// Global Variables for Current and Next Pieces (important to be defined here as they are used across multiple files)
Tetromino current_piece, next_piece;

//  STEP 1: Create the Shape Directory [4D] for each rotation state (CW) of Tetrominoes
//  Def: [number][states][y-coord/rows][x-coord/cols]
const int shapes[7][4][4][4] = {
    // Piece: I
    {
        // State 0, 1, 2, 3 respectively
        {{0,0,0,0}, {1,1,1,1}, {0,0,0,0}, {0,0,0,0}},
        {{0,0,1,0}, {0,0,1,0}, {0,0,1,0}, {0,0,1,0}},
        {{0,0,0,0}, {0,0,0,0}, {1,1,1,1}, {0,0,0,0}},
        {{0,1,0,0}, {0,1,0,0}, {0,1,0,0}, {0,1,0,0}}
    },
    // Piece: O
    {
        // State 0, 1, 2, 3 respectively
        {{0,0,0,0}, {0,1,1,0}, {0,1,1,0}, {0,0,0,0}},
        {{0,0,0,0}, {0,1,1,0}, {0,1,1,0}, {0,0,0,0}},
        {{0,0,0,0}, {0,1,1,0}, {0,1,1,0}, {0,0,0,0}},
        {{0,0,0,0}, {0,1,1,0}, {0,1,1,0}, {0,0,0,0}}
    },
    // Piece: T
    {
        // States 0, 1, 2, 3 respectively
        {{0,0,0,0}, {0,1,0,0}, {1,1,1,0}, {0,0,0,0}},
        {{0,0,0,0}, {0,1,0,0}, {0,1,1,0}, {0,1,0,0}},
        {{0,0,0,0}, {0,0,0,0}, {1,1,1,0}, {0,1,0,0}},
        {{0,0,0,0}, {0,1,0,0}, {1,1,0,0}, {0,1,0,0}}
    },
    // Piece: J
    {
        // States 0, 1, 2, 3 respectively
        {{1,0,0,0}, {1,1,1,0}, {0,0,0,0}, {0,0,0,0}},
        {{0,1,1,0}, {0,1,0,0}, {0,1,0,0}, {0,0,0,0}},
        {{0,0,0,0}, {1,1,1,0}, {0,0,1,0}, {0,0,0,0}},
        {{0,1,0,0}, {0,1,0,0}, {1,1,0,0}, {0,0,0,0}}
    },
    // Piece: L
    {
        // States 0, 1, 2, 3 respectively
        {{0,0,1,0}, {1,1,1,0}, {0,0,0,0}, {0,0,0,0}},
        {{0,1,0,0}, {0,1,0,0}, {0,1,1,0}, {0,0,0,0}},
        {{0,0,0,0}, {1,1,1,0}, {1,0,0,0}, {0,0,0,0}},
        {{1,1,0,0}, {0,1,0,0}, {0,1,0,0}, {0,0,0,0}}
    },
    // Piece: S
    {
        // States 0, 1, 2, 3 respectively
        {{0,1,1,0}, {1,1,0,0}, {0,0,0,0}, {0,0,0,0}},
        {{0,1,0,0}, {0,1,1,0}, {0,0,1,0}, {0,0,0,0}},
        {{0,0,0,0}, {0,1,1,0}, {1,1,0,0}, {0,0,0,0}},
        {{1,0,0,0}, {1,1,0,0}, {0,1,0,0}, {0,0,0,0}}
    },
    // Piece:Z
    {
        // States 0, 1, 2, 3 respectively
        {{1,1,0,0}, {0,1,1,0}, {0,0,0,0}, {0,0,0,0}},
        {{0,0,1,0}, {0,1,1,0}, {0,1,0,0}, {0,0,0,0}},
        {{0,0,0,0}, {1,1,0,0}, {0,1,1,0}, {0,0,0,0}},
        {{0,1,0,0}, {1,1,0,0}, {1,0,0,0}, {0,0,0,0}}
    }
};

// Define the OFFSET Arrays/Tables for SRS
// Def: [rotation_state][offset_no][coord(x,y)]
const int offsetSRS_JLSTZ [4][5][2] =
{
    {{0,0}, {0,0}, {0,0}, {0,0}, {0,0}},
    {{0,0}, {1,0}, {1,-1}, {0,2}, {1,2}},
    {{0,0}, {0,0}, {0,0}, {0,0}, {0,0}},
    {{0,0}, {-1,0}, {-1,-1}, {0,2}, {-1,2}}
};

const int offsetSRS_I [4][5][2] = 
{
    {{0,0}, {-1,0}, {2,0}, {-1,0}, {2,0}},
    {{-1,0}, {0,0}, {0,0}, {0,1}, {0,-2}},
    {{-1,1}, {1,1}, {-2,1}, {1,0}, {-2,0}},
    {{0,1}, {0,1}, {0,1}, {0,-1}, {0,2}}
};

// STEP 2: Board Initialization
int board[totalheight][viscols];
void InitBoard()
{
    memset(board, 0, sizeof(board));    // Clear the board by setting all cells to 0
}

// STEP 3: Movement Sensing
bool isValidMove(Tetromino piece, int dx, int dy, int dr)
{
    int nextX = piece.x + dx;   // Future x-coord.
    int nextY = piece.y + dy;   // Future y-coord.
    // Collision Checks
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            int tetrogridX = nextX + j, tetrogridY = nextY + i; // Calculate future placement for each cell in 4x4 grid
            if (shapes[piece.type][dr][i][j] != 0 && tetrogridY >= 0)  // Different states are handled using "dr"
            {
                // Wall Collision Check (Sides)
                if (tetrogridX < 0 || tetrogridX > 9)
                    return false;
                // Floor Collison Check
                else if (tetrogridY >= 40)
                    return false;
                // Overlapping Check
                else if (board[tetrogridY][tetrogridX] != 0)
                    return false;
            }
        }
    }
    return true;
}

// STEP 4: SRS - Super Rotation System Implementation
// Test 5 sequences ("Kicks") , for each of these check validity with "isValidMove()"
void SRS(Tetromino *piece, int direction)
{
    int curr_state = piece->rotation;
    int intended = (direction == 1) ? (curr_state + 1) % 4 : (curr_state + 3) % 4;  // Intended rotation state
    int dx = 0, dy = 0;
    if (piece->type != I && piece->type != O)   // For JLSTZ tetrominos
    {
        for(int i = 0; i < 5; i++)
        {
            dx = offsetSRS_JLSTZ[curr_state][i][0] - offsetSRS_JLSTZ[intended][i][0];   // X-kick
            dy = offsetSRS_JLSTZ[curr_state][i][1] - offsetSRS_JLSTZ[intended][i][1];   // Y-kick
            if (isValidMove(*piece, dx, dy, intended))  // Check if this adjustment is valid, if yes, update current
            {
                piece->x = piece->x + dx;
                piece->y = piece->y + dy;
                piece->rotation = intended;
                return;
            }
        }
    }
    else if (piece->type == I)  // For I tetromino
    {
        for(int i = 0; i < 5; i++)
        {
            dx = offsetSRS_I[curr_state][i][0] - offsetSRS_I[intended][i][0];   // X-kick
            dy = offsetSRS_I[curr_state][i][1] - offsetSRS_I[intended][i][1];   // Y-kick
            if (isValidMove(*piece, dx, dy, intended))  // Validity check of adjustment
            {
                piece->x = piece->x + dx;
                piece->y = piece->y + dy;
                piece->rotation = intended;
                return;
            }
        }
    }
    else if (piece->type == O)  // Kicks are not useful
        return;
}

// STEP 5: Locking the piece
void LockPiece(Tetromino piece)
{
    int curr_x = piece.x, curr_y = piece.y;
    int boardX = 0, boardY = 0; // Store the actual burning coordinates on the board for each cell of the piece
    /* Loop through the 4x4 grid of the piece and mark the occupied cells on the board
    with piece type + 1 (to differentiate from empty cells) */
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            if (shapes[piece.type][piece.rotation][i][j] == 1)
            {
                boardX = curr_x + j;    // Important!!!
                boardY = curr_y + i;
                if (boardY < 20)    // If any part of the piece is above the visible area when locked, it's game over
                {
                    gstate = GAMEOVER;  // Global "enum GameState" type variable is set to GAMEOVER
                    return;
                }
                board[boardY][boardX] = piece.type + 1;
                // While colouring, just subtract 1 to get original piece type and colour accordingly [Drawing]
            }
        }
    }
}

// STEP 6: Clear the fully-filled rows
int ClearFullRows()
{
    int cleared = 0;    // Local variable to keep track
    for(int i = totalheight - 1; i >= 20; i--)
    {
        int cell_count = 0;
        for(int j = 0; j < 10; j++)
        {
            if (board[i][j] == 0)
                break;
            else
                cell_count++;
        }
        if (cell_count == 10)
        {
            cleared += 1;
            // SHIFT DOWN ROWS AFTER CLEARING
            for(int sdr = i; sdr >= 0; sdr--)  // "sdr" = "shift down row"
            {
                for(int sdc = 0; sdc < 10; sdc++)   // "sdc" = "shift down column"
                {
                    board[sdr][sdc] = board[sdr - 1][sdc];  // SHIFT DOWN the row
                    board[sdr - 1][sdc] = 0;    // Set the previous row to zero, effectively "RESET THE ROW"
                }
            }
            i += 1;
            // or memcpy(board[sdr], board[sdr - 1], sizeof(board[sdr]));
            // i += 1; (ANYONE for "sdc for loop", EITHER THE ABOVE FOR LOOP OR THIS ONE-LINER TO SHIFT DOWN)
        }
    }
    gamestats.lines_cleared += cleared; // Update total lines cleared in global game statistics
    // Update score based on the number of lines cleared at once, and also update the level based on total lines cleared
    if (cleared == 1) gamestats.score += 100 * gamestats.level;
    else if (cleared == 2) gamestats.score += 300 * gamestats.level;
    else if (cleared == 3) gamestats.score += 800 * gamestats.level;
    gamestats.level = (gamestats.lines_cleared / 10) + 1;   // Level increases every 10 lines cleared
    return cleared;
}

// STEP 7: Generate Random Pieces & Spawn the Next Piece
int GenerateRandomPieceType()
{
    static int bag[7] = {0,1,2,3,4,5,6};
    static int bagindex = 0;
    // ***Srijato calls srand(time(NULL)); in tetris_main.c at the start
    // Implement the FISHER-YATES SHUFFLE (or KNUTH SHUFFLE) - The Gold Standard for Randomization
    if (bagindex == 0 || bagindex >= 7) // Shuffle only when bag is completely empty [7] (or first use [0])
    {
        bagindex = 0;
        for(int i = 6; i > 0; i--)
        {
            int r_index = rand() % (i + 1);
            // Swap elements
            int temp = bag[i];
            bag[i] = bag[r_index];
            bag[r_index] = temp; 
        }
    }
    return bag[bagindex++]; // Return the randomly selected piece type
}

void InitStartPiece()
{
    current_piece.type = GenerateRandomPieceType();
    next_piece.type = GenerateRandomPieceType();
    current_piece.rotation = 0;
    current_piece.x = 3;
    current_piece.y = 18;
}

void SpawnNext()
{
    current_piece = next_piece;
    current_piece.rotation = 0;
    current_piece.x = 3;
    current_piece.y = 18;
    next_piece.type = GenerateRandomPieceType();
}

bool CheckGameOver()
{
    if (!isValidMove(current_piece,0,0,0))
    {
        gstate = GAMEOVER;
        return true;
    }
    return false;
}
