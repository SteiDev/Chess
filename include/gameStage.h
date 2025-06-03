#ifndef __GAMESTATE_H__
#define __GAMESTATE_H__

#include "../include/typedefs.h"

void Game_Preload(const u8* configFilename, u16* boardSize_ptr, i32* fpsMax_ptr, i32Vect2* windowSize_ptr, bool* showCoordinates_ptr);

#endif  // end of __GAMESTATE_H__