#ifndef __CHESSBOARDLOGIC_H__
#define __CHESSBOARDLOGIC_H__

#include "../include/typedefs.h"


// [Piece related macros]

// Converts an upper board letter to its corresponding number 
#define UPPERLETTER_TO_NUM(letter)  ((letter) - '@')
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
   bool __pawn_canMoveTwice__;  // for the ability to move 2 tiles for pawns if they havent been moved
}__pieceAttributes_t;

// Defines the struct necessary to contain all attributes of a piece
typedef struct playerPiece_t{
    __chesspiece_t pieceType;
    __color_t pieceColor;
}playerPiece_t;

// [Board related functions]

// General board functions

// Initializes the main chessboard with default values as the return value and allocates memory on the heap for the board; if the initialization fails, exit() will be used
playerPiece_t** Board_Init(const u16 boardSizeX, const u16 boardSizeY);

// Prints the current board and the letter-number coordinates; WARNING: if the table coordinates are too large, the coordinates will show up incorrectly
void Board_Print(playerPiece_t** restrict inpboard, const u16 Xsize, const u16 Ysize);

// Imports general game settings from table_config.ini; should always be used before initializing the board as the ini file also contains the size of the table
void Board_Settings_LoadGeneral(const u8* restrict ini_filename, u16* XBoardCoord, u16* YBoardCoord, bool* restrict skip_loading_screen);

// Imports the placement of the chess pieces from layout.ini
void Board_LoadPlacement(const u8* restrict ini_filename, playerPiece_t*** main_board_ptr, const u16 XBoardCoord, const u16 YBoardCoord);

// Frees the dynamically allocated memory; the freed board should not be used after this point
void Board_Free(playerPiece_t*** main_board_ptr, const u16 YBoardCoord);

// Turn related functions

// Reads the move positions (from and to) and checks if they are valid, if they are not the function will ask the user again for the positions until they are valid
void Board_Turn_ReadAndValidate(playerPiece_t** restrict mainBoard, const __color_t currentColor, u16* fromCoordsX, u16* fromCoordsY, u16* toCoordsX, u16* toCoordsY, const u16 XBoardCoord, const u16 YBoardCoord);

// Moves the piece from a position to another and checks an enemy piece is captured
void Board_Turn_Move(playerPiece_t*** restrict board_ptr, const u16 fromX, const u16 fromY, const u16 toX, const u16 toY);

// Prints the current turn and then increments the turn count by 1
void Board_Turn_Counter(u16* restrict turnCount);

// Changes color_ptr to the next color, according to the order of the players
void Board_Turn_NextColor(__color_t* restrict color_ptr);

// 
void Board_Save_SaveGame(playerPiece_t** restrict mainBoard, const u16 XBoardCoord, const u16 YBoardCoord, u8* restrict filename);

bool Board_Save_LoadGame(u8* restrict loadPrompt);

#endif // __CHESSBOARDLOGIC_H__