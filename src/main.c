#include "../include/typedefs.h"
#include "../include/chessboardlogic.h"
#include "../include/gentools.h"
#include "../include/graphics.h"
#include "../include/pieceMoves.h"

#include "raylib.h"

#include <stdio.h>

#define DEBUG 1

i32 main() {
    playerPiece_t** mainBoard = NULL;
    u16 turnCounter = 1;  // starts at turn 1
    i32 fpsMax = 0;
    u32Vect2 boardPixelSize = {0};
    i32Vect2 windowSize = {0};
    u16Vect2 boardSize = {0};
    u16Vect2 from = {0};
    u16Vect2 to = {0};
    f32Vect2 offset = {0};
    u8* layoutName = NULL;
    bool loadPreviousGame = false;
    bool validFrom = false;
    bool validTo = false;
    bool highlightSelection = false;
    bool undoSelection = false;
    bool showCoordinates = false;
    bool doCastling = false;
    __color_t currentCol = white;  // white starts first
    pieceTextures_t whiteTextures = {0};
    pieceTextures_t blackTextures = {0};
    gameState_t gameState = checkFrom;
    boardTheme_t boardTheme = THEME_CLASSIC;

    Board_Settings_LoadGeneral("config/table_config.ini", &boardSize, &fpsMax, &windowSize, &showCoordinates);
    
    mainBoard = Board_Init(boardSize);
    
    // needs to be improved
    loadPreviousGame = Board_Save_LoadGame("Load previous game? (y/n)");
    if(loadPreviousGame == false) {
        layoutName = "config/layout.ini";
    } else {
        layoutName = "saves/gamesave.ini";
    }
    
    boardPixelSize = Graphics_CalculateBoardPixels(boardSize);
    offset = Graphics_CalculateOffset(windowSize, boardPixelSize);
    Board_LoadPlacement(layoutName, &mainBoard, boardSize);
    Graphics_InitRaylib(windowSize, fpsMax);
    Graphics_LoadTextures(&whiteTextures, &blackTextures);

    #ifdef DEBUG
    Debug_ResetDumpFile();
    #endif

    while(WindowShouldClose() == false) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        Graphics_DrawBoard(boardSize, offset, boardTheme);
        
        Graphics_DrawPieces(mainBoard, boardSize, whiteTextures, blackTextures, offset);
        Graphics_PrintTurn(turnCounter, offset, currentCol);

        if(showCoordinates != false) {
            Graphics_PrintCoordinates(boardSize, boardPixelSize, offset);
        }

        // To highlight the current selected piece
        if(highlightSelection != false) {
            DrawRectangle(offset.x + from.x * TILE_SIZE, offset.y + (boardSize.y - from.y - 1) * TILE_SIZE, TILE_SIZE, TILE_SIZE, ColorAlpha(YELLOW, 0.5f));
        }
        switch(gameState) {
            case checkFrom:
                validFrom = Board_GetFrom(mainBoard, boardSize, &from, currentCol, &highlightSelection, offset);
                if(validFrom != false) {
                    gameState = checkTo;
                }
                break;
            case checkTo:
                undoSelection = Board_UndoSelection(boardSize, offset);
                validTo = Board_GetTo(mainBoard, boardSize, from, &to, offset, &doCastling);

                if(undoSelection != false) {
                    gameState = checkFrom;
                    highlightSelection = false;
                }
                if(validTo != false) {
                    gameState = move;
                }
                break;
            case move:
                #ifdef DEBUG
                    Debug_DumpVariables(turnCounter, boardPixelSize, windowSize, fpsMax, boardSize, from, to, offset, layoutName, loadPreviousGame, validFrom, validTo, highlightSelection, undoSelection, showCoordinates, currentCol, gameState);
                #endif
                Piece_Move(&mainBoard, from, to, &doCastling);
                Board_Turn_NextColor(&currentCol);
                ++turnCounter;
                Board_Save_SaveGame(mainBoard, boardSize, "saves/gamesave.ini");
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
    Board_Free(&mainBoard, boardSize.y);
}