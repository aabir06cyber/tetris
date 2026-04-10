#include"common.h"
#include"input_control.h"
#include"logic.h"
#include"renderer.h"
#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int offset_x=100;    // ***Defined in tetris_main.c by Srijato
int offset_y=50;    // ***Defined in tetris_main.c by Srijato

double timeDelay = 250.00; //timeDelay is set to 250 ms
#define GameState enum GameState
float frame_time = 90;  // Game Engine runs at 90 FPS (~ 11.1 ms per frame)
static int lockdelay = 0;

double lastTime=0;

GameState gstate;
GameStats gamestats;

void handleGravity(){
    int lastlevel = gamestats.level;
    double currentTime = GetTime() * 1000.0;
    if ((currentTime - lastTime) >= timeDelay){//after a certain time the tetromino falls a certain distance 
        if (isValidMove(current_piece,0,1,current_piece.rotation))  //check if block can move one down
        {
            current_piece.y++;//y increases downwards
            lockdelay = 0;  // reset lockdelay if user makes any valid move before locking
        }
        else
        {
            if (lockdelay >= 2)    // lock after 0.5sec (0.25s x 2 = 0.5) [lock delay]
            {
                LockPiece(current_piece);
                ClearFullRows();
                SpawnNext();
                CheckGameOver();
            }
        }
        lastTime = currentTime;
        lockdelay += 1; // increase lockdelay by 1
        if (gamestats.level > lastlevel)    // decrease timeDelay for each level increase by factor of 0.75
            timeDelay = 0.75 * timeDelay;
        if (timeDelay < 11.1)   // Set baseline limit for timeDelay (@ 11.1 ms AS THE LOWEST ALLOWED)
            timeDelay = 11.1;
    }
}

void renderFrame(){//i will define it now
    BeginDrawing();//starts a new frame for drawing
    ClearBackground(BLACK);//turns the entire background black

    switch(gstate){
        case MENU:
            DrawMenu();
            break;
        case PLAYING:
            DrawBoard();
            DrawActivePiece();
            DrawGhost();
            DrawNextPiecePreview();
            GameStatsDisplay();
            break;
        case PAUSED:
            DrawBoard();
            DrawActivePiece();
            DrawPausedState();
            break;
        case GAMEOVER:
            DrawGameOver();
            break;
    }
    EndDrawing();
    }

int main(){
    srand(time(NULL));
    InitWindow(1000,1000,"Tetris");//initializes the tetris window 
    SetTargetFPS(frame_time);//frames per second

    InitBoard();
    InitStartPiece();
    
    lastTime = GetTime()*1000.0;
    int scoresaved = 0; // false
    while(!WindowShouldClose()){
    switch(gstate){//logic part
    case MENU:
        MenuInputControl();
        break;
    case PLAYING:
        PlayerInputControl();
        handleGravity();
        //scoresaved = 0; // reset for next play
        break;
    case PAUSED:
        PlayerInputControl();
        break;
    case GAMEOVER:
        MenuInputControl(); //returns to menu when gameover
        /*// Write to record file (not working properly)
        if (scoresaved == 0)
        {
            time_t savetime1 = time(NULL);
            struct tm *savetime2 = localtime(&savetime1);
            char savetime3[22];
            strftime(savetime3, 20, "%d-%m-%Y   %H:%M:%S", savetime2);
            FILE *rec = fopen("resources/record.txt", "a");
            fprintf(rec, "%s    %d      %d\n", savetime3, gamestats.score, gamestats.level);
            fclose(rec);
            scoresaved = 1;
        }*/
        break;
    }
    renderFrame();//UI part refreshes every frame
}
CloseWindow();
}










