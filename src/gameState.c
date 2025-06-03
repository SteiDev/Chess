#include "../include/gameStage.h"
#include "../include/chessboardlogic.h"
#include "../include/menus.h"
#include "../include/graphics.h"

void Game_Preload(const u8* configFilename, u16Vect2* boardSize_ptr, i32* fpsMax_ptr, i32Vect2* windowSize_ptr, bool* showCoordinates_ptr, f32Vect2* menuButtonOffset_ptr, playerPiece_t*** mainBoard_ptr, u32Vect2* boardPixelSize_ptr) {
    Board_Settings_LoadGeneral(configFilename, boardSize_ptr, fpsMax_ptr, windowSize_ptr, showCoordinates_ptr);

    *menuButtonOffset_ptr = Menu_CalculateButtonOffset(*windowSize_ptr);

    *mainBoard_ptr = Board_Init(*boardSize_ptr);

    boardPixelSize = Graphics_CalculateBoardPixels(boardSize);
}