#include "../include/typedefs.h"
#include "../include/chessboardlogic.h"
#include "../include/gentools.h"
#include "../include/graphics.h"

#include "raylib.h"

#include <stdio.h>
#include <errno.h>

#define DEBUG 1

#define SLEEP_SECONDS 1
#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 640
#define TARGET_FPS 60

i32 main(){
    playerPiece_t** mainBoard = NULL;
    u16 Xsize = 0;
    u16 Ysize = 0;
    u16 fromXCoords = 0;
    u16 fromYCoords = 0;
    u16 toXCoords = 0;
    u16 toYCoords = 0;
    u16 turnCounter = 1;  // starts at turn 1
    u8* layoutName = NULL;
    bool skip_loading_screen = false;    
    bool loadPreviousGame = false;
    bool clicked = false;
    __color_t currentCol = white;  // white starts first
    Vector2 mousePos;
    pieceTextures_t whiteTextures = {0};
    pieceTextures_t blackTextures = {0};
    
    Board_Settings_LoadGeneral("config/table_config.ini", &Xsize, &Ysize, &skip_loading_screen);
    
    mainBoard = Board_Init(Xsize, Ysize);
    
    // needs to be improved
    loadPreviousGame = Board_Save_LoadGame("Load previous game? (y/n)");
    if(loadPreviousGame != true){
        layoutName = "config/layout.ini";
    }else{
        layoutName = "saves/gamesave.ini";
    }
    
    Board_LoadPlacement(layoutName, &mainBoard, Xsize, Ysize);
    
    Graphics_InitRaylib(WINDOW_WIDTH, WINDOW_HEIGHT, TARGET_FPS);
    Graphics_LoadTextures(&whiteTextures, &blackTextures); // crashes here

    while(WindowShouldClose() == false){
        mousePos = GetMousePosition();
        clicked = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
        
        BeginDrawing();
        ClearBackground(RAYWHITE);
        Graphics_DrawBoard(Xsize, Ysize);

        // Board_Print(mainBoard, Xsize, Ysize);
        // printf("\n");

        Graphics_DrawPieces(mainBoard, Xsize, Ysize, whiteTextures, blackTextures);

        // Board_Turn_Counter(&turnCounter);
        // Board_Turn_ReadAndValidate(mainBoard, currentCol, &fromXCoords, &fromYCoords, &toXCoords, &toYCoords, Xsize, Ysize);
        // Board_Turn_Move(&mainBoard, fromXCoords, fromYCoords, toXCoords, toYCoords);
        // Board_Turn_NextColor(&currentCol);
        Sys_Sleep(SLEEP_SECONDS);
        // Sys_Clear();

        // this might change later
        Board_Save_SaveGame(mainBoard, Xsize, Ysize, "saves/gamesave.ini");

        EndDrawing();
    }
    
    Graphics_UnloadTextures(&whiteTextures, &blackTextures);
    CloseWindow();
    Board_Free(&mainBoard, Ysize);

    #ifdef DEBUG
    perror("\nErrors");
    #endif
}