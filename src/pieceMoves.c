#include "../include/pieceMoves.h"
#include "../include/chessboardlogic.h"
#include "../include/gentools.h"
#include "../include/typedefs.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>

#define __CAN_MOVE__ 0
#define __CAN_MOVE_AND_CAPTURE__ 1
#define __CANNOT_MOVE__ 2

#define __LOST_COLOR_MAX_SIZE__ 25
#define __LOST_PIECE_MAX_SIZE__ 25

i8 __CheckTile__(playerPiece_t** restrict board, const u16Vect2 from, const u16Vect2 to) {
    if(board[from.y][from.x].pieceColor == board[to.y][to.x].pieceColor){
        return __CANNOT_MOVE__;
    }else if(board[to.y][to.x].pieceColor == color_NONE){
        return __CAN_MOVE__;
    }else{
        return __CAN_MOVE_AND_CAPTURE__;
    }
}

void __MovePiece__(playerPiece_t*** restrict board_ptr, const u16Vect2 from, const u16Vect2 to, const bool capturePiece) {
    u8 lostColor[__LOST_COLOR_MAX_SIZE__];
    u8 lostPiece[__LOST_PIECE_MAX_SIZE__];

    if(capturePiece == true){
        switch((*board_ptr)[to.y][to.x].pieceColor){
            case white:
                strcpy(lostColor, "White");
                break;
            case black:
                strcpy(lostColor, "Black");
                break;
            default:
                Exception_SendWarning("Warning: Invalid lost color (__MovePiece__)\n");
        }
        switch((*board_ptr)[to.y][to.x].pieceType){
            case pawn:
                strcpy(lostPiece, "Pawn");
                break;
            case rook:
                strcpy(lostPiece, "Rook");
                break;
            case bishop:
                strcpy(lostPiece, "Bishop");
                break;
            case knight:
                strcpy(lostPiece, "Knight");
                break;
            case queen:
                strcpy(lostPiece, "Queen");
                break;
            case king:
                strcpy(lostPiece, "King");
                break;
            default:
            Exception_SendWarning("Warning: Invalid lost piece (__MovePiece__)\n");
        }

        printf("%s lost a %s!\n", lostColor, lostPiece);
    }

    (*board_ptr)[to.y][to.x].pieceType = (*board_ptr)[from.y][from.x].pieceType;
    (*board_ptr)[to.y][to.x].pieceColor = (*board_ptr)[from.y][from.x].pieceColor;

    (*board_ptr)[from.y][from.x].pieceType = piece_NONE;
    (*board_ptr)[from.y][from.x].pieceColor = color_NONE;
}

void __Piece_CheckAndMove__(playerPiece_t*** restrict board_ptr, const u16Vect2 from, const u16Vect2 to) {
    bool capturePiece = false;

    if(__CheckTile__((*board_ptr), from, to) == __CANNOT_MOVE__){
        printf("Invalid Move: try again!\n");
    }else{
        if(__CheckTile__((*board_ptr), from, to) == __CAN_MOVE_AND_CAPTURE__){
            capturePiece = true;
        }else{
            capturePiece = false;
        }
        __MovePiece__(board_ptr, from, to, capturePiece);
    }

}

void Piece_Move(playerPiece_t*** restrict board_ptr, const u16Vect2 from, const u16Vect2 to) {
    __Piece_CheckAndMove__(board_ptr, from, to);   
}

bool Piece_ISOwnerValid(playerPiece_t** restrict mainBoard, const u16Vect2 from, const __color_t currentTurnColor) {
    if(mainBoard[from.y][from.x].pieceColor == currentTurnColor){
        return true;
    }else{
        return false;
    }
}

i8 __CheckValidMove_Pawn__(playerPiece_t** restrict mainBoard, const u16Vect2 from, const u16Vect2 to) {
    switch(mainBoard[from.y][from.x].pieceColor){
        case white:
            if((from.x == to.x) && (to.y == from.y + 1) && (mainBoard[to.y][to.x].pieceColor == color_NONE)){
                return VALID_MOVE;
            }else if(((from.x -1 == to.x) || (from.x +1 == to.x)) && (to.y == from.y + 1) && (mainBoard[to.y][to.x].pieceColor == black)){
                return VALID_MOVE;
            }else{
                return INVALID_MOVE;
            }
            break;
        case black:
            if((from.x == to.x) && (to.y == from.y - 1) && (mainBoard[to.y][to.x].pieceColor == color_NONE)){
                return VALID_MOVE;
            }else if(((from.x -1 == to.x) || (from.x +1 == to.x)) && (to.y == from.y - 1) && (mainBoard[to.y][to.x].pieceColor == white)){
                return VALID_MOVE;
            }else{
                return INVALID_MOVE;
            }
            break;
        default:
            Exception_SendWarning("Warning: Invalid pawn move color (__CheckValidMove_Pawn__)\n");
    }
    return INVALID_MOVE;
}

i8 __CheckValidMove_Knight__(const u16Vect2 from, const u16Vect2 to, const u16Vect2 boardSize) {
    if(from.x > to.x){
        if(from.y > to.y){
            // - -
            if((Exception_CheckBounds(from.x - 1, from.y - 2, boardSize.x, boardSize.y, true) != OUT_OF_BOUNDS) || (Exception_CheckBounds(from.x - 2, from.y - 1, boardSize.x, boardSize.y, true) != OUT_OF_BOUNDS)){
                return VALID_MOVE;
            }
        }else{
            // - +
            if((Exception_CheckBounds(from.x - 1, from.y + 2, boardSize.x, boardSize.y, true) != OUT_OF_BOUNDS) || (Exception_CheckBounds(from.x - 2, from.y + 1, boardSize.x, boardSize.y, true) != OUT_OF_BOUNDS)){
                return VALID_MOVE;
            }
        }
    }else if(from.x < to.x){
        if(from.y > to.y){
            // + -
            if((Exception_CheckBounds(from.x + 1, from.y - 2, boardSize.x, boardSize.y, true) != OUT_OF_BOUNDS) || (Exception_CheckBounds(from.x + 2, from.y - 1, boardSize.x, boardSize.y, true) != OUT_OF_BOUNDS)){
                return VALID_MOVE;
            }
        }else{
            // + +
            if((Exception_CheckBounds(from.x + 1, from.y + 2, boardSize.x, boardSize.y, true) != OUT_OF_BOUNDS) || (Exception_CheckBounds(from.x + 2, from.y + 1, boardSize.x, boardSize.y, true) != OUT_OF_BOUNDS)){
                return VALID_MOVE;
            }
        }
    }
    return INVALID_MOVE;
}

// redo at some point
i8 __CheckValidMove_Bishop__(playerPiece_t** restrict mainBoard, const u16Vect2 from, const u16Vect2 to, const u16Vect2 boardSize) {
    i32 stepX = 0;
    i32 stepY = 0;
    u16 boardIndexX = 0;
    u16 boardIndexY = 0;

    // Check if the move is diagonal (i.e., the absolute differences in X and Y are equal)
    if (abs(from.x - to.x) != abs(from.y - to.y)) {
        return INVALID_MOVE; // Bishops must move diagonally
    }

    // Ensure the destination (to.x, to.y) is within the bounds of the board
    if (to.x >= boardSize.x || to.y >= boardSize.y) {
        return INVALID_MOVE; // Destination is out of bounds
    }

    // Determine the direction and check for obstacles in the diagonal path
    if(to.x > from.x){
        stepX = 1;
    }else{
        stepX = -1;
    }

    if(to.y > from.y){
        stepY = 1;
    }else{
        stepY = -1;
    }

    boardIndexX = from.x + stepX;
    boardIndexY = from.y + stepY;

    while (boardIndexX != to.x && boardIndexY != to.y) {
        if (mainBoard[boardIndexY][boardIndexX].pieceColor != color_NONE) {
            return INVALID_MOVE; // Obstacle found in the path
        }
        boardIndexX += stepX;
        boardIndexY += stepY;
    }

    // Final destination square check
    if (mainBoard[to.y][to.x].pieceColor == color_NONE || mainBoard[to.y][to.x].pieceColor != mainBoard[from.y][from.x].pieceColor) {
        return VALID_MOVE; // Valid move: target is either empty or occupied by opponent
    }

    return INVALID_MOVE; // Invalid move: target is occupied by a piece of the same color
}

// redo at some point
i8 __CheckValidMove_Rook__(playerPiece_t** restrict mainBoard, const u16Vect2 from, const u16Vect2 to, const u16Vect2 boardSize) {
    i32 stepX = 0;
    i32 stepY = 0;
    u16 boardIndexX = 0;
    u16 boardIndexY = 0;

    // Rook must move either horizontally or vertically
    if (from.x != to.x && from.y != to.y) {
        return INVALID_MOVE; // Rook moves in straight lines (horizontally or vertically)
    }

    // Ensure the destination (to.x, to.y) is within the bounds of the board
    if (to.x >= boardSize.x || to.y >= boardSize.y) {
        return INVALID_MOVE; // Destination is out of bounds
    }

    // If moving horizontally (same row)
    if (from.y == to.y) {
        // Check horizontal path from from.x to to.x
        if(to.x > from.x){
            stepX = 1;
        }else{
            stepX = -1;
        }
        boardIndexX = from.x + stepX;

        while (boardIndexX != to.x) {
            if (mainBoard[from.y][boardIndexX].pieceColor != color_NONE) {
                return INVALID_MOVE; // Obstacle found in the path
            }
            boardIndexX += stepX;
        }
    }
    // If moving vertically (same column)
    else if (from.x == to.x) {
        // Check vertical path from from.y to to.y
        if(to.y > from.y){
            stepY = 1;
        }else{
            stepY = -1;
        }
        boardIndexY = from.y + stepY;

        while (boardIndexY != to.y) {
            if (mainBoard[boardIndexY][from.x].pieceColor != color_NONE) {
                return INVALID_MOVE; // Obstacle found in the path
            }
            boardIndexY += stepY;
        }
    }

    // Final destination square check
    if (mainBoard[to.y][to.x].pieceColor == color_NONE || mainBoard[to.y][to.x].pieceColor != mainBoard[from.y][from.x].pieceColor) {
        return VALID_MOVE; // Valid move: target is either empty or occupied by opponent
    }

    return INVALID_MOVE; // Invalid move: target is occupied by a piece of the same color
}

i8 __CheckValidMove_Queen__(playerPiece_t** restrict mainBoard, const u16Vect2 from, const u16Vect2 to, const u16Vect2 boardSize) {
    if((__CheckValidMove_Bishop__(mainBoard, from, to, boardSize) == VALID_MOVE) || (__CheckValidMove_Rook__(mainBoard, from, to, boardSize) == VALID_MOVE)){
        return VALID_MOVE;    
    }else{
        return INVALID_MOVE;
    }
}

i8 __CheckValidMove_King__(const u16Vect2 from, const u16Vect2 to) {
    if((to.x == from.x + 1) || (to.x == from.x - 1)){
        if((to.y == from.y + 1) || (to.y == from.y - 1)){
            return VALID_MOVE;
        }    
    }
    return INVALID_MOVE;
}