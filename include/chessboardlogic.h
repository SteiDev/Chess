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

/**
 * @brief #### Playable piece type
 * @brief Contains all attributes that a chess piece should have in order to be playable.
 * @param pieceType The type of the piece.
 * @param pieceColor the color of the piece.
 */
typedef struct playerPiece_t{
    __chesspiece_t pieceType;
    __color_t pieceColor;
    bool usedSpecialMove;  // for pawn double move and castling
}playerPiece_t;

typedef enum gameState_t {
    checkFrom,
    checkTo,
    move,
    promotePawn
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
playerPiece_t** Board_Init(const u16Vect2 boardSize);

/**
 * @brief #### Load general settings from an ini file
 * @brief Loads general game settings from an ini file. Should always be used before initialising the board.
 * @note This function can throw exceptions
 * @param ini_filename The filename for the ini file.
 * @param XBoardCoord Pointer to the X coordinates (number of lines) of the board.
 * @param YBoardCoord Pointer to the Y coordinates (number of columns) of the board.
 * @param skip_loading_screen Skip or not the loading screens.
 */
NONNULL void Board_Settings_LoadGeneral(const u8* restrict ini_filename, u16Vect2* restrict boardCoord, i32* restrict fpsMax_ptr, i32Vect2* restrict screenSize_ptr, bool* showCoordinates_ptr);

/**
 * @brief #### Loads the placement of the board from an ini file
 * @brief Loads the placement of the chess pieces from the board from an ini file.
 * @note This function can throw exceptions
 * @param ini_filename The filename for the ini file.
 * @param XBoardCoord The X coordinates (number of lines) of the board.
 * @param YBoardCoord The Y coordinates (number of columns) of the board.
 */
NONNULL void Board_LoadPlacement(const u8* restrict ini_filename, playerPiece_t*** main_board_ptr, const u16Vect2 boardCoord);

/**
 * @brief #### Free the allocated heap memory of the main board
 * @brief Frees the allocated help memory of the main board. The board should not be used after this point.
 * @note This function can throw exceptions
 * @param main_board_ptr Pointer to the main board.
 * @param YBoardCoord The Y coordinates (number of columns) of the board.
 */
NONNULL void Board_Free(playerPiece_t*** main_board_ptr, const u16 YBoardCoord);

// Turn related functions

/**
 * @brief #### Change the color to the next color
 * @brief Change the color to the next color, according to the order of players.
 * @note This function can throw exceptions
 * @param color_ptr Pointer to the color of the current player.
 */
NONNULL void Board_Turn_NextColor(__color_t* restrict color_ptr);

/**
 * @brief #### Save the current state of the game
 * @brief Saves the current state of the table to an ini file.
 * @note This function can throw exceptions
 * @param mainBoard The main board.
 * @param XBoardCoord The X coordinates (number of lines) of the board.
 * @param YBoardCoord The Y coordinates (number of columns) of the board.
 * @param filename Name of the file containing the game save.
 */
NONNULL void Board_Save_SaveGame(playerPiece_t** restrict mainBoard, const u16Vect2 boardSize, u8* restrict filename);

NONNULL bool Board_Save_LoadGame(u8* restrict loadPrompt);
bool Board_IsInsideBoard(const f32Vect2 mousePos, const u16Vect2 boardSize);
NONNULL bool Board_GetFrom(playerPiece_t** restrict mainBoard, const u16Vect2 boardSize, u16Vect2* restrict from_ptr, const __color_t currentPlayerCol, bool* restrict highlightSelection, const f32Vect2 offset);
NONNULL bool Board_GetTo(playerPiece_t** restrict mainBoard, const u16Vect2 boardSize, const u16Vect2 from, u16Vect2* restrict to_ptr, const f32Vect2 offset, bool* restrict doCastling);
bool Board_UndoSelection(const u16Vect2 boardSize, const f32Vect2 offset);

#endif // __CHESSBOARDLOGIC_H__