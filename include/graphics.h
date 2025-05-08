#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__

#include "../include/typedefs.h"
#include "../include/chessboardlogic.h"

#include "raylib.h"

#define SQUARE_SIZE 60

#define TEXTURE_WHITE_ROOK "textures/pieces_png/white_rook.png"
#define TEXTURE_WHITE_KNIGHT "textures/pieces_png/white_knight.png"
#define TEXTURE_WHITE_BISHOP "textures/pieces_png/white_bishop.png"
#define TEXTURE_WHITE_QUEEN "textures/pieces_png/white_queen.png"
#define TEXTURE_WHITE_KING "textures/pieces_png/white_king.png"
#define TEXTURE_WHITE_PAWN "textures/pieces_png/white_pawn.png"

#define TEXTURE_BLACK_ROOK "textures/pieces_png/black_rook.png"
#define TEXTURE_BLACK_KNIGHT "textures/pieces_png/black_knight.png"
#define TEXTURE_BLACK_BISHOP "textures/pieces_png/black_bishop.png"
#define TEXTURE_BLACK_QUEEN "textures/pieces_png/black_queen.png"
#define TEXTURE_BLACK_KING "textures/pieces_png/black_king.png"
#define TEXTURE_BLACK_PAWN "textures/pieces_png/black_pawn.png"


// Stores all necessary data types to store all image textures for a color of pieces
typedef struct pieceTextures_t {
    Texture2D rook;
    Texture2D knight;
    Texture2D bishop;
    Texture2D queen;
    Texture2D king;
    Texture2D pawn;
} pieceTextures_t;


// Calls important function in order to start the graphical interface
void Graphics_InitRaylib(const u16 screenWidth, const u16 screenHeight, const u16 targetFPS);

// Draws the main board
void Graphics_DrawBoard(const u16 boardSizeX, const u16 boardSizeY);

// Loads the textures for the white and black pieces in the GPU memory(VRAM)
void Graphics_LoadTextures(pieceTextures_t* whitePieces_ptr, pieceTextures_t* blackPieces_ptr);

// Unloads the textures for the white and black pieces from the GPU memory(VRAM)
void Graphics_UnloadTextures(pieceTextures_t* whitePieces_ptr, pieceTextures_t* blackPieces_ptr);

// Draws the pieces from the main board
void Graphics_DrawPieces(playerPiece_t** restrict mainBoard, const u16 boardSizeX, const u16 boardSizeY, const pieceTextures_t whitePieces, const pieceTextures_t blackPieces);

#endif  // end of __GRAPHICS_H__