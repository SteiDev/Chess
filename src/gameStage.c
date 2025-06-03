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