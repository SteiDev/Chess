#ifndef __GAMESTATE_H__
#define __GAMESTATE_H__

#include "../include/typedefs.h"
#include "../include/chessboardlogic.h"
#include "../include/graphics.h"
#include "../include/menus.h"

#define DEBUG 1

/* Load before the game properly starts */
void Game_Preload(const u8* configFilename, u16Vect2* boardSize_ptr, i32* fpsMax_ptr, i32Vect2* windowSize_ptr, bool* showCoordinates_ptr, f32Vect2* menuButtonOffset_ptr, playerPiece_t*** mainBoard_ptr, u32Vect2* boardPixelSize_ptr, f32Vect2* offset_ptr, pieceTextures_t* whiteTextures_ptr, pieceTextures_t* blackTextures_ptr);

void Game_StartMenuLoop(menu_t* curMenu_ptr, menu_t* selectedMenu_ptr, const f32Vect2 menuButtonOffset);

void Game_LoadAtContinueExistingGame(bool* loadPreviousGame_ptr, menu_t* curMenu_ptr);

void Game_LoadBeforeGameLoop(const bool loadPreviousGame, u8** layoutName_ptr, PGN_Data* currentPGN_ptr, playerPiece_t*** mainBoard_ptr, const u16Vect2 boardSize, u16* turnCounter_ptr, __color_t* currentCol_ptr, u16* moveCount_ptr);

/* Load before exiting the game */
void Game_Exit(pieceTextures_t* whiteTextures_ptr, pieceTextures_t* blackTextures_ptr, playerPiece_t*** mainBoard_ptr, const u16 boardSizeY);

#endif  // end of __GAMESTATE_H__