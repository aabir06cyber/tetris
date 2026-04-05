#include"common.h"
#include"input_control.h"
#include"logic.h"
#include"renderer.h"
#include "raylib.h"
int offset_x=100;    // ***Defined in tetris_main.c by Srijato
int offset_y=50;    // ***Defined in tetris_main.c by Srijato

#define timeDelay 500 //timeDelay is set to 500 ms
#define GameState enum GameState
float frame_time = 60;

double lastTime=0;

GameState gstate;
GameStats gamestats;

void handleGravity(){
    double currentTime = GetTime() * 1000.0;
    if((currentTime-lastTime)>=timeDelay){//after a certain time the tetromino falls a certain distance 
        if(isValidMove(current_piece,0,1,current_piece.rotation)){//check if block can move one down
            current_piece.y++;//y increases downwards 
        }
        else{
            LockPiece(current_piece);
            ClearFullRows();
            SpawnNext();
            CheckGameOver();
        }
        lastTime = currentTime;
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
    InitWindow(1000,1000,"Tetris");//initializes the tetris window 
    SetTargetFPS(frame_time);//frames per second

    InitBoard();
    InitStartPiece();
    
    lastTime = GetTime()*1000.0;
    while(!WindowShouldClose()){
    switch(gstate){//logic part
    case MENU:
        MenuInputControl();
        break;
    case PLAYING:
        PlayerInputControl();
        handleGravity();
        break;
    case PAUSED:
        PlayerInputControl();
        break;
    case GAMEOVER:
        MenuInputControl(); //returns to menu when gameover 
        break;
    }
    renderFrame();//UI part refreshes every frame
}
CloseWindow();
}










