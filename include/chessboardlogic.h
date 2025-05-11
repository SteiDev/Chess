#ifndef __CHESSBOARDLOGIC_H__
#define __CHESSBOARDLOGIC_H__

#include "../include/typedefs.h"

#include "raylib.h"

// [Piece related macros]


// [Piece related structs]

/**
 * @brief #### Chess pieces
 * @brief Contains all types of pieces.
 */
typedef enum __chesspiece_t{
    piece_NONE = '-',
    pawn = 'p',
    bishop = 'b',
    knight = 'k',
    rook = 'r',
    queen = 'q',
    king = 'K'
}__chesspiece_t;

/**
 * @brief #### Chess colors
 * @brief Contains all colors for pieces.
 */
typedef enum __color_t{
    color_NONE = '-',
    white = 'w',
    black = 'b'
}__color_t;

// Defines the struct necessary to contain every special attributes of any piece type
// [[UNIMPLEMENTED]]
typedef struct __pieceAttributes_t{
   bool __pawn_canMoveTwice__;  // for the ability to move 2 tiles for pawns if they havent been moved
}__pieceAttributes_t;

/**
 * @brief #### Playable piece type
 * @brief Contains all attributes that a chess piece should have in order to be playable.
 * @param pieceType The type of the piece.
 * @param pieceColor the color of the piece.
 */
typedef struct playerPiece_t{
    __chesspiece_t pieceType;
    __color_t pieceColor;
}playerPiece_t;

typedef enum gameState_t {
    checkFrom,
    checkTo,
    move
} gameState_t;

// [Board related functions]

// General board functions

/**
 * @brief #### Initialise the main board
 * @brief Allocates heap memory for the board and sets the default values (piece_NONE and color_NONE) for each tile.
 * @note This function can throw exceptions
 * @param boardSizeX The X coordinates (number of lines) of the board.
 * @param boardSizeY The Y coordinates (number of columns) of the board.
 * @return The initialised board.
 */
playerPiece_t** Board_Init(const u16 boardSizeX, const u16 boardSizeY);

/**
 * @brief #### Load general settings from an ini file
 * @brief Loads general game settings from an ini file. Should always be used before initialising the board.
 * @note This function can throw exceptions
 * @param ini_filename The filename for the ini file.
 * @param XBoardCoord Pointer to the X coordinates (number of lines) of the board.
 * @param YBoardCoord Pointer to the Y coordinates (number of columns) of the board.
 * @param skip_loading_screen Skip or not the loading screens.
 */
void Board_Settings_LoadGeneral(const u8* restrict ini_filename, u16* XBoardCoord, u16* YBoardCoord, bool* restrict skip_loading_screen, i32* restrict fpsMax_ptr, i32* screenWidth_ptr, i32* screenHeight_ptr);

/**
 * @brief #### Loads the placement of the board from an ini file
 * @brief Loads the placement of the chess pieces from the board from an ini file.
 * @note This function can throw exceptions
 * @param ini_filename The filename for the ini file.
 * @param XBoardCoord The X coordinates (number of lines) of the board.
 * @param YBoardCoord The Y coordinates (number of columns) of the board.
 */
void Board_LoadPlacement(const u8* restrict ini_filename, playerPiece_t*** main_board_ptr, const u16 XBoardCoord, const u16 YBoardCoord);

/**
 * @brief #### Free the allocated heap memory of the main board
 * @brief Frees the allocated help memory of the main board. The board should not be used after this point.
 * @note This function can throw exceptions
 * @param main_board_ptr Pointer to the main board.
 * @param YBoardCoord The Y coordinates (number of columns) of the board.
 */
void Board_Free(playerPiece_t*** main_board_ptr, const u16 YBoardCoord);

// Turn related functions

/**
 * @brief #### Change the color to the next color
 * @brief Change the color to the next color, according to the order of players.
 * @note This function can throw exceptions
 * @param color_ptr Pointer to the color of the current player.
 */
void Board_Turn_NextColor(__color_t* restrict color_ptr);

/**
 * @brief #### Save the current state of the game
 * @brief Saves the current state of the table to an ini file.
 * @note This function can throw exceptions
 * @param mainBoard The main board.
 * @param XBoardCoord The X coordinates (number of lines) of the board.
 * @param YBoardCoord The Y coordinates (number of columns) of the board.
 * @param filename Name of the file containing the game save.
 */
void Board_Save_SaveGame(playerPiece_t** restrict mainBoard, const u16 XBoardCoord, const u16 YBoardCoord, u8* restrict filename);

bool Board_Save_LoadGame(u8* restrict loadPrompt);
bool Board_IsInsideBoard(const Vector2 mousePos, const u16 boardSizeX, const u16 boardSizeY);
bool Board_GetFrom(playerPiece_t** restrict mainBoard, const u16 boardSizeX, const u16 boardSizeY, u16* fromX_ptr, u16* fromY_ptr, const __color_t currentPlayerCol, bool* restrict highlightSelection, const f32 offsetX, const f32 offsetY);
bool Board_GetTo(playerPiece_t** restrict mainBoard, const u16 boardSizeX, const u16 boardSizeY, const u16 fromX, const u16 fromY, u16* toX_ptr, u16* toY_ptr, const f32 offsetX, const f32 offsetY);
bool Board_UndoSelection(const u16 boardSizeX, const u16 boardSizeY, const f32 offsetX, const f32 offsetY);

#endif // __CHESSBOARDLOGIC_H__