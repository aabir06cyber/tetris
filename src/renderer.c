#include "renderer.h"
#include "common.h"
#include "raylib.h"
#include <stdio.h>

// ------------------------------------------------------------
// SHAPE DATA
// Each tetromino has 4 rotation states
// Each rotation is a 4x4 matrix
// 1 = block exists, 0 = empty
// ------------------------------------------------------------

/*int shapes[7][4][4][4] = {
    // I
    {
        {
            {0,0,0,0},
            {1,1,1,1},
            {0,0,0,0},
            {0,0,0,0}
        },
        {
            {0,1,0,0},
            {0,1,0,0},
            {0,1,0,0},
            {0,1,0,0}
        },
        {
            {0,0,0,0},
            {1,1,1,1},
            {0,0,0,0},
            {0,0,0,0}
        },
        {
            {0,1,0,0},
            {0,1,0,0},
            {0,1,0,0},
            {0,1,0,0}
        }
    },

    // J
    {
        {
            {1,0,0,0},
            {1,1,1,0},
            {0,0,0,0},
            {0,0,0,0}
        },
        {
            {0,1,1,0},
            {0,1,0,0},
            {0,1,0,0},
            {0,0,0,0}
        },
        {
            {0,0,0,0},
            {1,1,1,0},
            {0,0,1,0},
            {0,0,0,0}
        },
        {
            {0,1,0,0},
            {0,1,0,0},
            {1,1,0,0},
            {0,0,0,0}
        }
    },

    // L
    {
        {
            {0,0,1,0},
            {1,1,1,0},
            {0,0,0,0},
            {0,0,0,0}
        },
        {
            {0,1,0,0},
            {0,1,0,0},
            {0,1,1,0},
            {0,0,0,0}
        },
        {
            {0,0,0,0},
            {1,1,1,0},
            {1,0,0,0},
            {0,0,0,0}
        },
        {
            {1,1,0,0},
            {0,1,0,0},
            {0,1,0,0},
            {0,0,0,0}
        }
    },

    // O
    {
        {
            {0,1,1,0},
            {0,1,1,0},
            {0,0,0,0},
            {0,0,0,0}
        },
        {
            {0,1,1,0},
            {0,1,1,0},
            {0,0,0,0},
            {0,0,0,0}
        },
        {
            {0,1,1,0},
            {0,1,1,0},
            {0,0,0,0},
            {0,0,0,0}
        },
        {
            {0,1,1,0},
            {0,1,1,0},
            {0,0,0,0},
            {0,0,0,0}
        }
    },

    // S
    {
        {
            {0,1,1,0},
            {1,1,0,0},
            {0,0,0,0},
            {0,0,0,0}
        },
        {
            {0,1,0,0},
            {0,1,1,0},
            {0,0,1,0},
            {0,0,0,0}
        },
        {
            {0,1,1,0},
            {1,1,0,0},
            {0,0,0,0},
            {0,0,0,0}
        },
        {
            {0,1,0,0},
            {0,1,1,0},
            {0,0,1,0},
            {0,0,0,0}
        }
    },

    // T
    {
        {
            {0,1,0,0},
            {1,1,1,0},
            {0,0,0,0},
            {0,0,0,0}
        },
        {
            {0,1,0,0},
            {0,1,1,0},
            {0,1,0,0},
            {0,0,0,0}
        },
        {
            {0,0,0,0},
            {1,1,1,0},
            {0,1,0,0},
            {0,0,0,0}
        },
        {
            {0,1,0,0},
            {1,1,0,0},
            {0,1,0,0},
            {0,0,0,0}
        }
    },

    // Z
    {
        {
            {1,1,0,0},
            {0,1,1,0},
            {0,0,0,0},
            {0,0,0,0}
        },
        {
            {0,0,1,0},
            {0,1,1,0},
            {0,1,0,0},
            {0,0,0,0}
        },
        {
            {1,1,0,0},
            {0,1,1,0},
            {0,0,0,0},
            {0,0,0,0}
        },
        {
            {0,0,1,0},
            {0,1,1,0},
            {0,1,0,0},
            {0,0,0,0}
        }
    }
};*/

//colors for each pieces
//color is the datatype of the array piceceOlors which has 7 colors in it 
// in raylib color is the struct type aldready defined for us to use 
///create the custom color 
//Color mycolor={255,0,0}
// in this each segement refers to a color 


Color pieceColors[7] = {
    SKYBLUE,   // I
    BLUE,      // J
    ORANGE,    // L
    YELLOW,    // O
    GREEN,     // S
    PURPLE,    // T
    RED        // Z
};

//introduced in the common.h
/*typedef struct
{
    int x;  // X position of the tetromino on the grid
    int y;  // Y position of the tetromino on the grid
    enum TetrominoType type; // Type of the tetromino   
    int rotation;   // Rotation state (0-3)
} Tetromino;*/
//piececellexist check whether there is a piece or not in the row or column
//it takes the struct tetromino and the rrow and col in the loop
//as the arguments
//since tetrominotype is a enum datatype so type is alreadyy indexed 
//takes the rotation states and finally tells us whether it is 1 or 0 on the
//basis whether there is a tetromino block or not 
int PieceCellExists(Tetromino piece, int row, int col)
{
    return shapes[piece.type][piece.rotation][row][col];
}

//drawing each block 
//here x and y tells us from which coordinate in the board we shoudl start drawing 
//seperated this as a drawblock to use it in the drawboard fcn 
//here tile_size is acuallty mentioned in the common.h which is 40 so 
//drawrectangle(100,100,40,40,RED)
//means it will draw from 100px from the left side of the screeen and 40px down from the top most of the screen 

void DrawBlock(int x, int y, Color color)
{
DrawRectangle(x, y, tile_size, tile_size, color);
DrawRectangleLines(x, y, tile_size, tile_size, BLACK);
}

//drawing the visible part only that is of 20 ros and 10 cols 
//total_height=40;
//gridrows=20;
//visible part is 20
//viscols=10
//used offset_x and offset_y in the board to make it look pretty  
//if we havent used the offset thing then t he board would have started in the left most corenr og ht screen which would appear ugly
//total board size i meand the visible board is 
//40*10=400
//20*40=800

void DrawBoard()
{
    for (int row = totalheight - gridrows; row < totalheight; row++)
    {
        for (int col = 0; col < viscols; col++)
        {
            int screenX = offset_x + col * tile_size;
            int screenY = offset_y + (row - (totalheight - gridrows)) * tile_size;

            if (board[row][col] != 0)
            {
                DrawBlock(screenX, screenY, pieceColors[board[row][col] - 1]);
            }
            else
            {
                DrawRectangleLines(screenX, screenY, tile_size, tile_size, DARKGRAY);
            }
        }
    }
}
//it is the drawtetromino function which will draw our 
//current piece,next piece,ghost piece,
//piece.x and piece,y is the board column and board row we have given 
//here screenX and ScreenY are the pizel postions of the game window


void DrawTetromino(Tetromino piece)
{
    Color color = pieceColors[piece.type];

    for (int row = 0; row < 4; row++)
    {
        for (int col = 0; col < 4; col++)
        {
            if (PieceCellExists(piece, row, col))
            {
                int boardX = piece.x + col;
                int boardY = piece.y + row;

                // Only draw visible rows
                if (boardY >= totalheight - gridrows && boardY < totalheight)
                {
                    int screenX = offset_x + boardX * tile_size;
                    int screenY = offset_y + (boardY - (totalheight - gridrows)) * tile_size;

                    DrawBlock(screenX, screenY, color);
                }
            }
        }
    }
}


void DrawActivePiece()
{
    DrawTetromino(current_piece);
}

//here we are actually drawing a box that  will show box which tetromino will come next
//the thing that we have included in the coomon.h
//extern tetrromino current_piece does not create additional memory of the variable but it says thte compiler thatt somewhere esle there is a varibale called current_piece
//this allows us to access it in multiple files 
void DrawNextPiecePreview()
{   //preview X and previewY are the pixels where the box will appear
    int previewX = offset_x + viscols * tile_size + 60;
    int previewY = offset_y + 100;
    //DrawrectangleLines(X,Y,width,height,color)
    DrawText("NEXT", previewX, previewY - 40, 30, WHITE);
    DrawRectangleLines(previewX - 10, previewY - 10, 180, 180, WHITE);

    Color color = pieceColors[next_piece.type];

    for (int row = 0; row < 4; row++)
    {
        for (int col = 0; col < 4; col++)
        {
            if (shapes[next_piece.type][0][row][col])
            {
                int x = previewX + col * tile_size;
                int y = previewY + row * tile_size;
                DrawBlock(x, y, color);
            }
        }
    }
}


int CheckCollision(Tetromino piece)
{
    for (int row = 0; row < 4; row++)
    {
        for (int col = 0; col < 4; col++)
        {
            if (PieceCellExists(piece, row, col))
            {
                int newX = piece.x + col;
                int newY = piece.y + row;

                if (newX < 0 || newX >= viscols || newY >= totalheight)
                    return 1;

                if (newY >= 0 && board[newY][newX] != 0)
                    return 1;
            }
        }
    }
    return 0;
}

//next we are going to construct ghost piece 
void DrawGhost()
{
    Tetromino ghost = current_piece; // it copies all the properties of the current_piece

    while (!CheckCollision(ghost)) //it checks whether there will be any collision at that particular row
    {
        ghost.y++;//if not then it goes down
    }
    ghost.y--;//now imageine therer was a collision at some x row so now x is an invalid postion so we need to subtract 1 from x 
    //it wilkl be x-1

    Color ghostColor = Fade(pieceColors[ghost.type], 0.3f); // it makes the tetromino piece 30 percent visible or transparent

    for (int row = 0; row < 4; row++)
    {
        for (int col = 0; col < 4; col++)
        {
            if (PieceCellExists(ghost, row, col))
            {
                int boardX = ghost.x + col;
                int boardY = ghost.y + row;

                if (boardY >= totalheight - gridrows && boardY < totalheight)
                {
                    int screenX = offset_x + boardX * tile_size;
                    int screenY = offset_y + (boardY - (totalheight - gridrows)) * tile_size;

                    DrawRectangle(screenX, screenY, tile_size, tile_size, ghostColor);
                    DrawRectangleLines(screenX, screenY, tile_size, tile_size, LIGHTGRAY);
                }
            }
        }
    }
}

void GameStatsDisplay()
{
    int statsX = offset_x + viscols * tile_size + 60;
    int statsY = offset_y + 350;

    DrawText(TextFormat("Score: %d", gamestats.score), statsX, statsY, 28, WHITE);
    DrawText(TextFormat("Level: %d", gamestats.level), statsX, statsY + 50, 28, WHITE);
    DrawText(TextFormat("Lines: %d", gamestats.lines_cleared), statsX, statsY + 100, 28, WHITE);
}


void DrawMenu()
{
    DrawText("TETRIS", 280, 120, 70, YELLOW);
    DrawText("Press ENTER to Start", 220, 250, 35, WHITE);
    DrawText("Arrow Keys to Move", 230, 320, 28, LIGHTGRAY);
    DrawText("UP to Rotate", 280, 360, 28, LIGHTGRAY);
    DrawText("P to Pause", 300, 400, 28, LIGHTGRAY);
}


void DrawPausedState()
{
    DrawRectangle(0, 0, 900, 900, Fade(BLACK, 0.5f));
    DrawText("PAUSED", 300, 300, 60, YELLOW);
    DrawText("Press P to Resume", 240, 380, 35, WHITE);
}


void DrawGameOver()
{
    DrawRectangle(0, 0, 900, 900, Fade(BLACK, 0.7f));
    DrawText("GAME OVER", 220, 220, 60, RED);
    DrawText(TextFormat("Final Score: %d", gamestats.score), 250, 320, 35, WHITE);
    DrawText("Press ENTER to Restart", 190, 400, 35, YELLOW);
}
