#include "../include/pieceMoves.h"
#include "../include/chessboardlogic.h"
#include "../include/gentools.h"
#include "../include/typedefs.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>

#define __MISMATCHED_PIECE__ 1
#define __CORRECT_PIECE__ 0

#define __CAN_MOVE__ 0
#define __CAN_MOVE_AND_CAPTURE__ 1
#define __CANNOT_MOVE__ 2

#define __LOST_COLOR_MAX_SIZE__ 25
#define __LOST_PIECE_MAX_SIZE__ 25

#define SLEEP_SECONDS_AFTER_CAPTURE 1

i8 __CheckTile__(playerPiece_t** restrict board, const u16 fromX, const u16 fromY, const u16 toX, const u16 toY){
    if(board[fromY][fromX].pieceColor == board[toY][toX].pieceColor){
        return __CANNOT_MOVE__;
    }else if(board[toY][toX].pieceColor == color_NONE){
        return __CAN_MOVE__;
    }else{
        return __CAN_MOVE_AND_CAPTURE__;
    }
}

void __MovePiece__(playerPiece_t*** restrict board_ptr, const u16 fromX, const u16 fromY, const u16 toX, const u16 toY, const bool capturePiece){
    u8 lostColor[__LOST_COLOR_MAX_SIZE__];
    u8 lostPiece[__LOST_PIECE_MAX_SIZE__];

    if(capturePiece == true){
        switch((*board_ptr)[toY][toX].pieceColor){
            case white:
                strcpy(lostColor, "White");
                break;
            case black:
                strcpy(lostColor, "Black");
                break;
            default:
                SendWarning("Warning: Invalid lost color (__MovePiece__)\n");
        }
        switch((*board_ptr)[toY][toX].pieceType){
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
            SendWarning("Warning: Invalid lost piece (__MovePiece__)\n");
        }

        printf("%s lost a %s!\n", lostColor, lostPiece);
        Sys_Sleep(SLEEP_SECONDS_AFTER_CAPTURE);
    }

    (*board_ptr)[toY][toX].pieceType = (*board_ptr)[fromY][fromX].pieceType;
    (*board_ptr)[toY][toX].pieceColor = (*board_ptr)[fromY][fromX].pieceColor;

    (*board_ptr)[fromY][fromX].pieceType = piece_NONE;
    (*board_ptr)[fromY][fromX].pieceColor = color_NONE;
}

void __CheckAndMove__(playerPiece_t*** restrict board_ptr, const u16 fromX, const u16 fromY, const u16 toX, const u16 toY){
    bool capturePiece = false;

    if(__CheckTile__((*board_ptr), fromX, fromY, toX, toY) == __CANNOT_MOVE__){
        printf("Invalid Move: try again!\n");
    }else{
        if(__CheckTile__((*board_ptr), fromX, fromY, toX, toY) == __CAN_MOVE_AND_CAPTURE__){
            capturePiece = true;
        }else{
            capturePiece = false;
        }
        __MovePiece__(board_ptr, fromX, fromY, toX, toY, capturePiece);
    }

}

void MovePiece(playerPiece_t*** restrict board_ptr, const u16 fromX, const u16 fromY, const u16 toX, const u16 toY){
    __CheckAndMove__(board_ptr, fromX, fromY, toX, toY);   
}

i8 CheckPieceOwner(playerPiece_t** restrict mainBoard, const u16 fromX, const u16 fromY, const __color_t currentTurnColor){
    if(mainBoard[fromY][fromX].pieceColor != currentTurnColor){
        printf("Invalid Piece Selected: select a piece of your own color!\n");
        return INVALID_OWNER;
    }else{
        return VALID_OWNER;
    }
}

i8 __CheckValidMove_Pawn__(playerPiece_t** restrict mainBoard, const u16 fromX, const u16 fromY, const u16 toX, const u16 toY){
    switch(mainBoard[fromY][fromX].pieceColor){
        case white:
            if((fromX == toX) && (toY == fromY + 1)){
                return VALID_MOVE;
            }else{
                return INVALID_MOVE;
            }
            break;
        case black:
            if((fromX == toX) && (toY == fromY - 1)){
                return VALID_MOVE;
            }else{
                return INVALID_MOVE;
            }
            break;
        default:
            SendWarning("Warning: Invalid pawn move color (__CheckValidMove_Pawn__)\n");
    }
    return INVALID_MOVE;
}

i8 __CheckValidMove_Knight__(const u16 fromX, const u16 fromY, const u16 toX, const u16 toY, const u16 boardX, const u16 boardY){
    if(fromX > toX){
        if(fromY > toY){
            // - -
            if((CheckBounds(fromX - 1, fromY - 2, boardX, boardY, true) != OUT_OF_BOUNDS) || (CheckBounds(fromX - 2, fromY - 1, boardX, boardY, true) != OUT_OF_BOUNDS)){
                return VALID_MOVE;
            }
        }else{
            // - +
            if((CheckBounds(fromX - 1, fromY + 2, boardX, boardY, true) != OUT_OF_BOUNDS) || (CheckBounds(fromX - 2, fromY + 1, boardX, boardY, true) != OUT_OF_BOUNDS)){
                return VALID_MOVE;
            }
        }
    }else if(fromX < toX){
        if(fromY > toY){
            // + -
            if((CheckBounds(fromX + 1, fromY - 2, boardX, boardY, true) != OUT_OF_BOUNDS) || (CheckBounds(fromX + 2, fromY - 1, boardX, boardY, true) != OUT_OF_BOUNDS)){
                return VALID_MOVE;
            }
        }else{
            // + +
            if((CheckBounds(fromX + 1, fromY + 2, boardX, boardY, true) != OUT_OF_BOUNDS) || (CheckBounds(fromX + 2, fromY + 1, boardX, boardY, true) != OUT_OF_BOUNDS)){
                return VALID_MOVE;
            }
        }
    }
    return INVALID_MOVE;
}

// redo at some point
i8 __CheckValidMove_Bishop__(playerPiece_t** restrict mainBoard, const u16 fromX, const u16 fromY, const u16 toX, const u16 toY, const u16 boardX, const u16 boardY) {
    i32 stepX = 0;
    i32 stepY = 0;
    u16 boardIndexX = 0;
    u16 boardIndexY = 0;

    // Check if the move is diagonal (i.e., the absolute differences in X and Y are equal)
    if (abs(fromX - toX) != abs(fromY - toY)) {
        return INVALID_MOVE; // Bishops must move diagonally
    }

    // Ensure the destination (toX, toY) is within the bounds of the board
    if (toX >= boardX || toY >= boardY) {
        return INVALID_MOVE; // Destination is out of bounds
    }

    // Determine the direction and check for obstacles in the diagonal path
    if(toX > fromX){
        stepX = 1;
    }else{
        stepX = -1;
    }

    if(toY > fromY){
        stepY = 1;
    }else{
        stepY = -1;
    }

    boardIndexX = fromX + stepX;
    boardIndexY = fromY + stepY;

    while (boardIndexX != toX && boardIndexY != toY) {
        if (mainBoard[boardIndexY][boardIndexX].pieceColor != color_NONE) {
            return INVALID_MOVE; // Obstacle found in the path
        }
        boardIndexX += stepX;
        boardIndexY += stepY;
    }

    // Final destination square check
    if (mainBoard[toY][toX].pieceColor == color_NONE || mainBoard[toY][toX].pieceColor != mainBoard[fromY][fromX].pieceColor) {
        return VALID_MOVE; // Valid move: target is either empty or occupied by opponent
    }

    return INVALID_MOVE; // Invalid move: target is occupied by a piece of the same color
}

// redo at some point
i8 __CheckValidMove_Rook__(playerPiece_t** restrict mainBoard, const u16 fromX, const u16 fromY, const u16 toX, const u16 toY, const u16 boardX, const u16 boardY){
    i32 stepX = 0;
    i32 stepY = 0;
    u16 boardIndexX = 0;
    u16 boardIndexY = 0;

    // Rook must move either horizontally or vertically
    if (fromX != toX && fromY != toY) {
        return INVALID_MOVE; // Rook moves in straight lines (horizontally or vertically)
    }

    // Ensure the destination (toX, toY) is within the bounds of the board
    if (toX >= boardX || toY >= boardY) {
        return INVALID_MOVE; // Destination is out of bounds
    }

    // If moving horizontally (same row)
    if (fromY == toY) {
        // Check horizontal path from fromX to toX
        if(toX > fromX){
            stepX = 1;
        }else{
            stepX = -1;
        }
        boardIndexX = fromX + stepX;

        while (boardIndexX != toX) {
            if (mainBoard[fromY][boardIndexX].pieceColor != color_NONE) {
                return INVALID_MOVE; // Obstacle found in the path
            }
            boardIndexX += stepX;
        }
    }
    // If moving vertically (same column)
    else if (fromX == toX) {
        // Check vertical path from fromY to toY
        if(toY > fromY){
            stepY = 1;
        }else{
            stepY = -1;
        }
        boardIndexY = fromY + stepY;

        while (boardIndexY != toY) {
            if (mainBoard[boardIndexY][fromX].pieceColor != color_NONE) {
                return INVALID_MOVE; // Obstacle found in the path
            }
            boardIndexY += stepY;
        }
    }

    // Final destination square check
    if (mainBoard[toY][toX].pieceColor == color_NONE || mainBoard[toY][toX].pieceColor != mainBoard[fromY][fromX].pieceColor) {
        return VALID_MOVE; // Valid move: target is either empty or occupied by opponent
    }

    return INVALID_MOVE; // Invalid move: target is occupied by a piece of the same color
}

i8 __CheckValidMove_Queen__(playerPiece_t** restrict mainBoard, const u16 fromX, const u16 fromY, const u16 toX, const u16 toY, const u16 boardX, const u16 boardY){
    if((__CheckValidMove_Bishop__(mainBoard, fromX, fromY, toX, toY, boardX, boardY) == VALID_MOVE) || (__CheckValidMove_Rook__(mainBoard, fromX, fromY, toX, toY, boardX, boardY) == VALID_MOVE)){
        return VALID_MOVE;    
    }else{
        return INVALID_MOVE;
    }
}

i8 __CheckValidMove_King__(const u16 fromX, const u16 fromY, const u16 toX, const u16 toY){
    if((toX == fromX + 1) || (toX == fromX - 1)){
        if((toY == fromY + 1) || (toY == fromY - 1)){
            return VALID_MOVE;
        }    
    }
    return INVALID_MOVE;
}