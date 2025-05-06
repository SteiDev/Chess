#ifndef __CHESSBOARDLOGIC_H__
#define __CHESSBOARDLOGIC_H__

#include "../include/typedefs.h"


// [Piece related macros]

// Converts a board letter 
#define LETTER_TO_NUM(letter)  ((letter) - '@')
// A = 65
// @ = 64


// [Piece related structs]

// Defines the struct necessary to contain every present chesspiece type
typedef enum __chesspiece_t{
    piece_NONE = '-',
    pawn = 'p',
    bishop = 'b',
    knight = 'k',
    rook = 'r',
    queen = 'q',
    king = 'K'
}__chesspiece_t;

// Defines the struct necessary to contain every present chesspiece color
typedef enum __color_t{
    color_NONE = '-',
    white = 'w',
    black = 'b'
}__color_t;

// Defines the struct necessary to contain every special attributes of any piece type
// [[UNIMPLEMENTED]]
typedef struct __pieceAttributes_t{
   bool __pawn_canMove2__;
}__pieceAttributes_t;

// Defines the struct necessary to contain all attributes of a piece
typedef struct playerPiece_t{
    __chesspiece_t pieceType;
    __color_t pieceColor;
}playerPiece_t;

// [Board related functions]

// General board functions

// Initializes the main chessboard with default values as the return value; if the initialization fails, exit() will be used
playerPiece_t** Board_Init(const u16 boardSizeX, const u16 boardSizeY);
void Board_Print(playerPiece_t** restrict inpboard, const u16 Xsize, const u16 Ysize);
void Board_Settings_General(const u8* restrict ini_filename, u16* XBoardCoord, u16* YBoardCoord, bool* restrict skip_loading_screen);
void Board_Placement(const u8* restrict ini_filename, playerPiece_t*** main_board_ptr, const u16 XBoardCoord, const u16 YBoardCoord);
void Board_Free(playerPiece_t*** main_board_ptr, const u16 YBoardCoord);

// Turn related functions


void Board_Turn_Read(playerPiece_t** restrict mainBoard, const __color_t currentColor, u16* fromCoordsX, u16* fromCoordsY, u16* toCoordsX, u16* toCoordsY, const u16 XBoardCoord, const u16 YBoardCoord);
void Board_Turn_Move(playerPiece_t*** restrict board_ptr, const u16 fromX, const u16 fromY, const u16 toX, const u16 toY);
void Board_Turn_Counter(u16* restrict turnCount);

// 
void Board_Turn_NextColor(__color_t* restrict color_ptr);

#endif // __CHESSBOARDLOGIC_H__