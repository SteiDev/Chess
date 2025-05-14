#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__

#include "../include/typedefs.h"
#include "../include/chessboardlogic.h"

#include "raylib.h"

/** * @brief #### Size of a chessboard square */
#define TILE_SIZE 60

#define TURN_TEXT_MAX_LENGTH 50
#define NUMBERS_TEXT_MAX_LENGTH 5
#define LETTERS_TEXT_MAX_LENGTH 3

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 640
#define TARGET_FPS 60

/** * @brief #### Path the to white rook png file */
#define TEXTURE_WHITE_ROOK "textures/pieces_png/white_rook.png"
/** * @brief #### Path the to white knight png file */
#define TEXTURE_WHITE_KNIGHT "textures/pieces_png/white_knight.png"
/** * @brief #### Path the to white bishop png file */
#define TEXTURE_WHITE_BISHOP "textures/pieces_png/white_bishop.png"
/** * @brief #### Path the to white queen png file */
#define TEXTURE_WHITE_QUEEN "textures/pieces_png/white_queen.png"
/** * @brief #### Path the to white king png file */
#define TEXTURE_WHITE_KING "textures/pieces_png/white_king.png"
/** * @brief #### Path the to white pawn png file */
#define TEXTURE_WHITE_PAWN "textures/pieces_png/white_pawn.png"

/** * @brief #### Path the to black rook png file */
#define TEXTURE_BLACK_ROOK "textures/pieces_png/black_rook.png"
/** * @brief #### Path the to black knight png file */
#define TEXTURE_BLACK_KNIGHT "textures/pieces_png/black_knight.png"
/** * @brief #### Path the to black bishop png file */
#define TEXTURE_BLACK_BISHOP "textures/pieces_png/black_bishop.png"
/** * @brief #### Path the to black queen png file */
#define TEXTURE_BLACK_QUEEN "textures/pieces_png/black_queen.png"
/** * @brief #### Path the to black king png file */
#define TEXTURE_BLACK_KING "textures/pieces_png/black_king.png"
/** * @brief #### Path the to black pawn png file */
#define TEXTURE_BLACK_PAWN "textures/pieces_png/black_pawn.png"

/**
 * @brief #### Piece textures
 * @brief Contains all piece textures for a single color.
 * @param rook The 2D texture of the rook.
 * @param knight The 2D texture of the knight.
 * @param bishop The 2D texture of the bishop.
 * @param queen The 2D texture of the queen.
 * @param king The 2D texture of the king.
 * @param pawn The 2D texture of the pawn.
 */
typedef struct pieceTextures_t {
    Texture2D rook;
    Texture2D knight;
    Texture2D bishop;
    Texture2D queen;
    Texture2D king;
    Texture2D pawn;
} pieceTextures_t;

/**
 * @brief #### Initialise Raylib
 * @brief Initialises the tools necessary for Raylib.
 * @param screenWidth Width of the created window.
 * @param screenHeight Height of the created window.
 * @param targetFPS The Maximum FPS for the created window.
 */
void Graphics_InitRaylib(const u16 screenWidth, const u16 screenHeight, const u16 targetFPS);

/**
 * @brief #### Draw graphic chessboard
 * @brief Draws the graphics for the chessboard alone (without pieces).
 * @param boardSizeX The X coordinates (number of lines) of the board.
 * @param boardSizeY The Y coordinates (number of columns) of the board.
 */
void Graphics_DrawBoard(const u16 boardSizeX, const u16 boardSizeY, const f32 offsetX, const f32 offsetY);

/**
 * @brief #### Load chesspieces textures
 * @brief Loads and check the textures for the black and white pieces into the GPU memory(VRAM).
 * @note This function can throw exceptions
 * @param whitePieces_ptr Pointer to the variable containing the textures for the white pieces.
 * @param blackPieces_ptr Pointer to the variable containing the textures for the black pieces.
 */
void Graphics_LoadTextures(pieceTextures_t* whitePieces_ptr, pieceTextures_t* blackPieces_ptr);

/**
 * @brief #### Unload chesspieces textures
 * @brief Unloads the textures for the black and white pieces into the GPU memory(VRAM).
 * @param whitePieces_ptr Pointer to the variable containing the textures for the white pieces.
 * @param blackPieces_ptr Pointer to the variable containing the textures for the black pieces.
 */
void Graphics_UnloadTextures(pieceTextures_t* whitePieces_ptr, pieceTextures_t* blackPieces_ptr);

/**
 * @brief #### Draw chesspieces over the board
 * @brief Draws all pieces using loaded textures. 
 * @note This function can throw exceptions
 * @param mainBoard The main board.
 * @param boardSizeX The X coordinates (number of lines) of the board.
 * @param boardSizeX The Y coordinates (number of columns) of the board.
 * @param whitePieces The variable containing the textures for the white pieces.
 * @param blackPieces The variable containing the textures for the black pieces.
 */
void Graphics_DrawPieces(playerPiece_t** restrict mainBoard, const u16 boardSizeX, const u16 boardSizeY, const pieceTextures_t whitePieces, const pieceTextures_t blackPieces, const f32 offsetX, const f32 offsetY);

Vector2 Graphics_MapMouseToBoard(Vector2 mousePos, const u16 boardSizeY, const f32 offsetX, const f32 offsetY);
void Graphics_CalculateBoardPixels(const u16 boardSizeX, const u16 boardSizeY, u32* outputBoardPixelWidth, u32* outputBoardPixelHeight);
void Graphics_PrintTurn(const u16 turnCounter, const f32 offsetX, const f32 offsetY, const __color_t currentColor);
void Graphics_CalculateOffset(const i32 screenWidth, const i32 screenHeight, const u32 boardPixelWidth, const u32 boardPixelHeight, f32* outputOffsetX, f32* outputOffsetY);
void Graphics_PrintScale(const u16 boardSizeX, const u16 boardSizeY, const u32 boardPixelWidth, const u32 boardPixelHeight, const f32 offsetX, const f32 offsetY);

#endif  // end of __GRAPHICS_H__