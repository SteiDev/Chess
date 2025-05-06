#ifndef __PIECEMOVES_H__
#define __PIECEMOVES_H__

#include "../include/typedefs.h"
#include "../include/chessboardlogic.h"

// [Piece movement related macros]

// used for checking the return value of CheckPieceOwner
#define INVALID_OWNER -1
#define VALID_OWNER 0

// used for checking the return value of the particular piece moves related functions
#define INVALID_MOVE -1
#define VALID_MOVE 0

void MovePiece(playerPiece_t*** restrict board_ptr, const u16 fromX, const u16 fromY, const u16 toX, const u16 toY);
// Checks if the selected piece is the same color as the current player; returns INVALID_OWNER if they don't match, VALID_OWNER otherwise
i8 CheckPieceOwner(playerPiece_t** restrict mainBoard, const u16 fromX, const u16 fromY, const __color_t currentTurnColor);
// Checks if a pawn can move to the desired location; returns INVALID_MOVE if it can't, VALID_MOVE otherwise
i8 __CheckValidMove_Pawn__(playerPiece_t** restrict mainBoard, const u16 fromX, const u16 fromY, const u16 toX, const u16 toY);
// Checks if a knight can move to the desired location; returns INVALID_MOVE if it can't, VALID_MOVE otherwise
i8 __CheckValidMove_Knight__(const u16 fromX, const u16 fromY, const u16 toX, const u16 toY, const u16 boardX, const u16 boardY);
// Checks if a bishop can move to the desired location; returns INVALID_MOVE if it can't, VALID_MOVE otherwise
i8 __CheckValidMove_Bishop__(playerPiece_t** restrict mainBoard, const u16 fromX, const u16 fromY, const u16 toX, const u16 toY, const u16 boardX, const u16 boardY);
// Checks if a rook can move to the desired location; returns INVALID_MOVE if it can't, VALID_MOVE otherwise
i8 __CheckValidMove_Rook__(playerPiece_t** restrict mainBoard, const u16 fromX, const u16 fromY, const u16 toX, const u16 toY, const u16 boardX, const u16 boardY);
// Checks if a queen can move to the desired location; returns INVALID_MOVE if it can't, VALID_MOVE otherwise
i8 __CheckValidMove_Queen__(playerPiece_t** restrict mainBoard, const u16 fromX, const u16 fromY, const u16 toX, const u16 toY, const u16 boardX, const u16 boardY);
// Checks if a king can move to the desired location; returns INVALID_MOVE if it can't, VALID_MOVE otherwise
i8 __CheckValidMove_King__(const u16 fromX, const u16 fromY, const u16 toX, const u16 toY);

#endif // __PIECEMOVES_H__