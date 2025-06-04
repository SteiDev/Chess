#include "../include/gameStage.h"
#include "../include/chessboardlogic.h"
#include "../include/menus.h"
#include "../include/graphics.h"
#include "../include/gentools.h"

#include <stdlib.h>

void Game_Preload(const u8* configFilename, u16Vect2* boardSize_ptr, i32* fpsMax_ptr, i32Vect2* windowSize_ptr, bool* showCoordinates_ptr, f32Vect2* menuButtonOffset_ptr, playerPiece_t*** mainBoard_ptr, u32Vect2* boardPixelSize_ptr, f32Vect2* offset_ptr, pieceTextures_t* whiteTextures_ptr, pieceTextures_t* blackTextures_ptr) {
    Board_Settings_LoadGeneral(configFilename, boardSize_ptr, fpsMax_ptr, windowSize_ptr, showCoordinates_ptr);

    *menuButtonOffset_ptr = Menu_CalculateButtonOffset(*windowSize_ptr);

    *mainBoard_ptr = Board_Init(*boardSize_ptr);

    *boardPixelSize_ptr = Graphics_CalculateBoardPixels(*boardSize_ptr);
    *offset_ptr = Graphics_CalculateOffset(*windowSize_ptr, *boardPixelSize_ptr);

    Graphics_InitRaylib(*windowSize_ptr, *fpsMax_ptr);
    Graphics_LoadTextures(whiteTextures_ptr, blackTextures_ptr);
}

void Game_Exit(pieceTextures_t* whiteTextures_ptr, pieceTextures_t* blackTextures_ptr, playerPiece_t*** mainBoard_ptr, const u16 boardSizeY) {
    Graphics_UnloadTextures(whiteTextures_ptr, blackTextures_ptr);
    CloseWindow();
    Board_Free(mainBoard_ptr, boardSizeY);
    exit(EXIT_SUCCESS);
}

void Game_LoadAtContinueExistingGame(bool* loadPreviousGame_ptr, menu_t* curMenu_ptr) {
    *loadPreviousGame_ptr = true;
                
    *curMenu_ptr = inGame;
}

void Game_StartMenuLoop(menu_t* curMenu_ptr, menu_t* selectedMenu_ptr, const f32Vect2 menuButtonOffset) {
    while(*curMenu_ptr == start && WindowShouldClose() == false) {
        BeginDrawing();
        ClearBackground(BROWN);

        Menu_PrintButtons(menuButtonOffset);
        *selectedMenu_ptr = Menu_GetSelectedMenu(menuButtonOffset);

        if(*selectedMenu_ptr != menu_NONE) {
            *curMenu_ptr = *selectedMenu_ptr;
            *selectedMenu_ptr = menu_NONE;
        }

        EndDrawing();
    }
}

void Game_LoadBeforeGameLoop(const bool loadPreviousGame, u8** layoutName_ptr, PGN_Data* currentPGN_ptr, playerPiece_t*** mainBoard_ptr, const u16Vect2 boardSize, u16* turnCounter_ptr, __color_t* currentCol_ptr, u16* moveCount_ptr) {
    #ifdef DEBUG
        Debug_ResetDumpFile();
    #endif

    if(loadPreviousGame == false) {
        *layoutName_ptr = "config/layout.ini";
        Read_PGN_Header(currentPGN_ptr, "readMetadataPGN.txt");
        Board_Save_PGN_Header(currentPGN_ptr, "saves/game.pgn");
    } else {
        *layoutName_ptr = "saves/gamesave.ini";
    }

    Board_LoadPlacement(*layoutName_ptr, mainBoard_ptr, boardSize, turnCounter_ptr, currentCol_ptr, moveCount_ptr);

}

// void Game_MainGameLoop(const u16 turnCounter, const u32Vect2 boardPixelSize, const i32Vect2 windowSize, const i32 fpsMax, 
//                         const u16Vect2 boardSize, const u16Vect2 from, const u16Vect2 to, const f32Vect2 offset, u8* layoutName, const bool loadPreviousGame, 
//                         const bool validFrom, const bool validTo, const bool highlightSelection, const bool undoSelection, const bool showCoordinates, const __color_t currentCol,
//                         const gameState_t gameState, const boardTheme_t boardTheme) {
//     while(WindowShouldClose() == false) {
//         BeginDrawing();
//         ClearBackground(RAYWHITE);
//         Graphics_DrawBoard(boardSize, offset, boardTheme);
                    
//         Graphics_DrawPieces(mainBoard, boardSize, whiteTextures, blackTextures, offset);
//         Graphics_PrintTurn(turnCounter, offset, currentCol);

//         if(showCoordinates != false) {
//             Graphics_PrintCoordinates(boardSize, boardPixelSize, offset);
//         }

//         // To highlight the current selected piece
//         if(highlightSelection != false) {
//             DrawRectangle(offset.x + from.x * TILE_SIZE, offset.y + (boardSize.y - from.y - 1) * TILE_SIZE, TILE_SIZE, TILE_SIZE, ColorAlpha(YELLOW, 0.5f));
//         }
//         switch(gameState) {
//             case checkFrom:
//                 validFrom = Board_GetFrom(mainBoard, boardSize, &from, currentCol, &highlightSelection, offset);
//                 if(validFrom != false) {
//                     gameState = checkTo;
//                 }
//                 break;

//             case checkTo:
//                 undoSelection = Board_UndoSelection(boardSize, offset);
//                 validTo = Board_GetTo(mainBoard, boardSize, from, &to, offset, &doCastling);

//                 if(undoSelection != false) {
//                     gameState = checkFrom;
//                     highlightSelection = false;
//                 }
//                 if(validTo != false) {
//                     gameState = move;
//                 }
//                 break;

//             case move:
//                 #ifdef DEBUG
//                     Debug_DumpVariables(turnCounter, boardPixelSize, windowSize, fpsMax, boardSize, from, to, offset, layoutName, loadPreviousGame, validFrom, validTo, highlightSelection, undoSelection, showCoordinates, currentCol, gameState);
//                 #endif
//                 u8 moveStr[16];
//                 FormatMoveString(moveStr, from, to, mainBoard);
//                 if(currentPGN.moveCount < MAX_MOVES){
//                     strcpy(currentPGN.moves[currentPGN.moveCount], moveStr);
//                     currentPGN.moveCount++;
//                 }
//                 doPawnPromotion = Piece_CheckPawnPromotion(mainBoard, from, to, boardSize);
//                 Piece_Move(&mainBoard, from, to, &doCastling);
//                 Board_Turn_NextColor(&currentCol);
//                 ++turnCounter;

//                 if(doPawnPromotion == true) {
//                     gameState = promotePawn;
//                     doPawnPromotion = false;
//                     pawnPromotionPos = Graphics_MapBoardToMouse(to, boardSize.y, offset);
                        
//                     if(currentCol == white) { /* The colors are inverted because this happens after the colors are changed */
//                         pawnPromoteTextures = blackTextures;
//                     } else {
//                         pawnPromoteTextures = whiteTextures;
//                     }
//                 } else {
//                     gameState = checkFrom;
//                     Board_Save_SaveGame(mainBoard, boardSize, turnCounter, currentCol, currentPGN.moveCount, "saves/gamesave.ini");
//                     Board_Save_Moves_To_PGN(&currentPGN, currentPGN.moveCount - 1, "saves/game.pgn");
//                 }

//                 highlightSelection = false;
//                 break;

//             case promotePawn:
//                 Graphics_PawnPromotionPopup(pawnPromotionPos, pawnPromoteTextures);

//                 /* Here should be a function that checks if the player clicks inside the popup window and which piece is selected 
//                 and there should be another function that actually changes the piece to the selected one on the board    */
                            
//                 /* If a piece was chosen pawnUpgradeSelected should be changed to true */

//                 if(pawnUpgradeSelected != false) {
//                     Board_Save_SaveGame(mainBoard, boardSize, turnCounter, currentCol, currentPGN.moveCount, "saves/gamesave.ini");
//                     Board_Save_Moves_To_PGN(&currentPGN, currentPGN.moveCount - 1, "saves/game.pgn");
//                 }

//                 /* Currently not implemented so it should skip this state */

//                 gameState = checkFrom;
//                 break;

//             default:
//                 Exception_SendWarning("Warning: Invalid game state (main)\n");
//         }
//         EndDrawing();
//     }
// }