#include "common.h"
#include "logic.h"
#include "raylib.h"
#define L KEY_LEFT //for shifting the block to left
#define R KEY_RIGHT //foor shifting the blok too right
#define C KEY_UP   // foor rotating the block clock wise
#define CC KEY_Z   // foor rotating thr block counterclockwise
#define D KEY_DOWN //Soft drop
#define S KEY_SPACE //for hard drop 

int DASR =15;
int dasrotTimer = 0;

int dasTimer = 0;
int arrTimer = 0;

int DAS = 20; //delay before repeaating movement (frames) [Delayed Auto Shift]
int ARR = 3; // repeat speed (frames) [Auto Repeat Rate]

void PlayerInputControl(){
// Move left
if(IsKeyPressed(L)){
    dasTimer = 0;
    arrTimer = 0;
    if(isValidMove(current_piece,-1,0,current_piece.rotation)){
        current_piece.x--;
    }
}
if(IsKeyDown(L)){
    dasTimer++;
    if(dasTimer >= DAS){ //increment until dasTimer crosses DAS threshold
        arrTimer++;
        if (arrTimer >= ARR) { //increment until arrTimer crosses ARR threshold
            if(isValidMove(current_piece,-1,0,current_piece.rotation)){
                current_piece.x--;
            } 
            arrTimer = 0; //reset arrTimer
        }
    }
}
if(IsKeyReleased(L)){
    dasTimer = 0;
    arrTimer = 0;
}
// Move right

if(IsKeyPressed(R)){
    dasTimer = 0;
    arrTimer = 0;
if(isValidMove(current_piece,1,0,current_piece.rotation)){
    current_piece.x++;
}
}
if(IsKeyDown(R)){
    dasTimer++;
    if(dasTimer>=DAS){
        arrTimer++;
        if(arrTimer>=ARR){
        if(isValidMove(current_piece,1,0,current_piece.rotation)){
                current_piece.x++; 
        }
        arrTimer =0;
    }
    
}
}
if(IsKeyReleased(R)){
arrTimer = 0;
dasTimer = 0;

}
// ROtate clockwise
if(IsKeyPressed(C)){
    SRS(&current_piece,1);
}
if(IsKeyDown(C)){
    dasrotTimer++;
    if(dasrotTimer>=DASR){
        SRS(&current_piece,1);
        dasrotTimer=0;
    }
    }
if(IsKeyReleased(C)){
dasrotTimer = 0;
}
// Rotate counterclockwise
/*if(IsKeyPressed(CC)){
    SRS(&current_piece,-1);
}*/
if(IsKeyPressed(CC)){
    SRS(&current_piece,-1);
}
if(IsKeyDown(CC)){
    dasrotTimer++;
    if(dasrotTimer>=DASR){
        SRS(&current_piece,-1);
        dasrotTimer=0;}
    }
if(IsKeyReleased(CC)){
dasrotTimer = 0;
}
// Soft drop
/*
if(IsKeyPressed(D)){
    if(isValidMove(current_piece,0,1,current_piece.rotation)){
        current_piece.y++;
    }
*/
if(IsKeyPressed(D)){
    dasTimer = 0;
    arrTimer = 0;
    if(isValidMove(current_piece,0,1,current_piece.rotation)){
        current_piece.y++;
    }
}
if(IsKeyDown(D)){
    dasTimer++;
    if(dasTimer >= DAS){ //increment until dasTimer crosses DAS threshold
        arrTimer++;
        if (arrTimer >= ARR) { //increment until arrTimer crosses ARR threshold
            if(isValidMove(current_piece,0,1,current_piece.rotation)){
                current_piece.y++;
            } 
            arrTimer = 0; //reset arrTimer
        }
    }
}
if(IsKeyReleased(D)){
    dasTimer = 0;
    arrTimer = 0;
}

// HARD DROP
if(IsKeyPressed(S)){
while(isValidMove(current_piece,0,1,current_piece.rotation)){
    current_piece.y++;
}

}
LockPiece(current_piece);
ClearFullRows();
SpawnNext();

 if(IsKeyPressed(KEY_P)){// to Pause
        gstate = PAUSED;
    }
if(gstate == PAUSED && IsKeyPressed(KEY_ENTER)){// to resume
         gstate = PLAYING;

    }

}

//

void MenuInputControl(){
    if(IsKeyPressed(KEY_ENTER)){// to start
        gstate = PLAYING;
    }
    if(IsKeyPressed(KEY_R)){ //Game is being restarted
        InitBoard();
        gamestats.score = gamestats.lines_cleared = 0 ;
        gamestats.level = 1;
        InitStartPiece();
        gstate = PLAYING;
    }
    if(IsKeyPressed(KEY_Q)){
        gstate = GAMEOVER;
        DrawGameStatistics();
        if(IsKeyPressed(KEY_ENTER)) gstate = MENU;
        //display score and go to menu after acknowledgement
    }
}
