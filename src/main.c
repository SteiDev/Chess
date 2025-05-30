#include "../include/typedefs.h"
#include "../include/chessboardlogic.h"
#include "../include/gentools.h"
#include "../include/graphics.h"
#include "../include/pieceMoves.h"

#include "raylib.h"

#include <stdio.h>
#include <string.h>

#define DEBUG 1

i32 main() {
    playerPiece_t** mainBoard = NULL;
    u16 turnCounter = 1;  // starts at turn 1
    u32 boardPixelWidth = 0;
    u32 boardPixelHeight = 0;
    i32 screenWidth = 0;
    i32 screenHeight = 0;
    i32 fpsMax = 0;
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
    __color_t currentCol = white;  // white starts first
    pieceTextures_t whiteTextures = {0};
    pieceTextures_t blackTextures = {0};
    gameState_t gameState = checkFrom;
    boardTheme_t boardTheme = THEME_CLASSIC;
    PGN_Data currentPGN = {0};

    Board_Settings_LoadGeneral("config/table_config.ini", &boardSize, &fpsMax, &screenWidth, &screenHeight, &showCoordinates);
    
    mainBoard = Board_Init(boardSize);
    
    // needs to be improved
    loadPreviousGame = Board_Save_LoadGame("Load previous game? (y/n)");
    if(loadPreviousGame == false) {
        layoutName = "config/layout.ini";
        Read_PGN_Header(&currentPGN, "readMetadataPGN.txt");
        Board_Save_PGN_Header(&currentPGN, "saves/game.pgn");
    } else {
        layoutName = "saves/gamesave.ini";
    }
    
    Graphics_CalculateBoardPixels(boardSize, &boardPixelHeight, &boardPixelWidth);
    offset = Graphics_CalculateOffset(screenWidth, screenHeight, boardPixelWidth, boardPixelHeight);
    Board_LoadPlacement(layoutName, &mainBoard, boardSize, &turnCounter, &currentCol, &currentPGN.moveCount);
    Graphics_InitRaylib(screenWidth, screenHeight, fpsMax);
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
            Graphics_PrintCoordinates(boardSize, boardPixelWidth, boardPixelHeight, offset);
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
                validTo = Board_GetTo(mainBoard, boardSize, from, &to, offset);

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
                    Debug_DumpVariables(turnCounter, boardPixelWidth, boardPixelHeight, screenWidth, screenHeight, fpsMax, boardSize, from, to, offset, layoutName, loadPreviousGame, validFrom, validTo, highlightSelection, undoSelection, showCoordinates, currentCol, gameState);
                #endif
                Piece_Move(&mainBoard, from, to);

                u8 moveStr[16];
                FormatMoveString(moveStr, from, to, mainBoard);
                if(currentPGN.moveCount < MAX_MOVES){
                    strcpy(currentPGN.moves[currentPGN.moveCount], moveStr);
                    currentPGN.moveCount++;
                }

                Board_Turn_NextColor(&currentCol);
                ++turnCounter;
                Board_Save_SaveGame(mainBoard, boardSize, turnCounter, currentCol, currentPGN.moveCount, "saves/gamesave.ini");
                Board_Save_Moves_To_PGN(&currentPGN, currentPGN.moveCount - 1, "saves/game.pgn");
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