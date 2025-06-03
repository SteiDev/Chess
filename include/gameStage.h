#ifndef __GAMESTATE_H__
#define __GAMESTATE_H__

#include "../include/typedefs.h"
#include "../include/chessboardlogic.h"
#include "../include/graphics.h"

/* Load before the game properly starts */
void Game_Preload(const u8* configFilename, u16Vect2* boardSize_ptr, i32* fpsMax_ptr, i32Vect2* windowSize_ptr, bool* showCoordinates_ptr, f32Vect2* menuButtonOffset_ptr, playerPiece_t*** mainBoard_ptr, u32Vect2* boardPixelSize_ptr, f32Vect2* offset_ptr, pieceTextures_t* whiteTextures_ptr, pieceTextures_t* blackTextures_ptr);

void Game_Exit(pieceTextures_t* whiteTextures_ptr, pieceTextures_t* blackTextures_ptr, playerPiece_t*** mainBoard_ptr, const u16 boardSizeY);

#endif  // end of __GAMESTATE_H__