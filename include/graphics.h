#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__

#include "raylib.h"
#include "../include/typedefs.h"

#define SQUARE_SIZE 60

// Calls important function in order to start the graphical interface
void Graphics_InitRaylib(const u16 screenWidth, const u16 screenHeight, const u16 targetFPS);

// Draws the main board
void Graphics_DrawBoard(const u16 boardSizeX, const u16 boardSizeY);

#endif  // end of __GRAPHICS_H__