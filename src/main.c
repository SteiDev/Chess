#include "../include/typedefs.h"
#include "../include/chessboardlogic.h"
#include "../include/gentools.h"
#include "../include/graphics.h"
#include "../include/pieceMoves.h"

#include "raylib.h"

#include <stdio.h>
#include <errno.h>

#define DEBUG 1

i32 main() {
    playerPiece_t** mainBoard = NULL;
    u16 Xsize = 0;
    u16 Ysize = 0;
    u16 fromXCoords = 0;
    u16 fromYCoords = 0;
    u16 toXCoords = 0;
    u16 toYCoords = 0;
    u16 turnCounter = 1;  // starts at turn 1
    i32 screenWidth = 0;
    i32 screenHeight = 0;
    i32 fpsMax = 0;
    u8* layoutName = NULL;
    bool skip_loading_screen = false;    
    bool loadPreviousGame = false;
    bool validFrom = false;
    bool validTo = false;
    bool highlightSelection = false;
    bool undoSelection = false;
    __color_t currentCol = white;  // white starts first
    pieceTextures_t whiteTextures = {0};
    pieceTextures_t blackTextures = {0};
    gameState_t gameState = checkFrom;
    
    Board_Settings_LoadGeneral("config/table_config.ini", &Xsize, &Ysize, &skip_loading_screen, &fpsMax, &screenWidth, &screenHeight);
    
    mainBoard = Board_Init(Xsize, Ysize);
    
    // needs to be improved
    loadPreviousGame = Board_Save_LoadGame("Load previous game? (y/n)");
    if(loadPreviousGame != true){
        layoutName = "config/layout.ini";
    }else{
        layoutName = "saves/gamesave.ini";
    }
    
    Board_LoadPlacement(layoutName, &mainBoard, Xsize, Ysize);
    Graphics_InitRaylib(screenWidth, screenHeight, fpsMax);
    Graphics_LoadTextures(&whiteTextures, &blackTextures);

    while(WindowShouldClose() == false) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        Graphics_DrawBoard(Xsize, Ysize, X_OFFSET, Y_OFFSET);
        
        Graphics_DrawPieces(mainBoard, Xsize, Ysize, whiteTextures, blackTextures, X_OFFSET, Y_OFFSET);
        Graphics_PrintTurn(turnCounter);

        if(highlightSelection != false) {
            DrawRectangle((fromXCoords + X_OFFSET) * TILE_SIZE, (Ysize - fromYCoords - 1 + Y_OFFSET) * TILE_SIZE, TILE_SIZE, TILE_SIZE, ColorAlpha(YELLOW, 0.5f));
        }
        switch(gameState) {
            case checkFrom:
                validFrom = Board_GetFrom(mainBoard, Xsize, Ysize, &fromXCoords, &fromYCoords, currentCol, &highlightSelection, X_OFFSET, Y_OFFSET);
                if(validFrom != false) {
                    gameState = checkTo;
                }
                break;
            case checkTo:
                undoSelection = Board_UndoSelection(Xsize, Ysize, X_OFFSET, Y_OFFSET);
                validTo = Board_GetTo(mainBoard, Xsize, Ysize, fromXCoords, fromYCoords, &toXCoords, &toYCoords, X_OFFSET, Y_OFFSET);

                if(undoSelection != false) {
                    gameState = checkFrom;
                    highlightSelection = false;
                }
                if(validTo != false) {
                    gameState = move;
                }
                break;
            case move:
                Piece_Move(&mainBoard, fromXCoords, fromYCoords, toXCoords, toYCoords);
                Board_Turn_NextColor(&currentCol);
                ++turnCounter;
                Board_Save_SaveGame(mainBoard, Xsize, Ysize, "saves/gamesave.ini");
                gameState = checkFrom;
                highlightSelection = false;
                break;
            default:
            Exception_SendWarning("Warning: Invalid game state (main)\n");
        }

        EndDrawing();
    }
    
    Graphics_UnloadTextures(&whiteTextures, &blackTextures);
    CloseWindow();
    Board_Free(&mainBoard, Ysize);

    #ifdef DEBUG
    perror("\nErrors");
    #endif
}
