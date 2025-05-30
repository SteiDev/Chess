#include "../include/pieceMoves.h"
#include "../include/chessboardlogic.h"
#include "../include/gentools.h"
#include "../include/typedefs.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>

void Piece_ToggleSpecialMove(playerPiece_t*** board_ptr, const u16Vect2 from, const u16Vect2 to) {
    (*board_ptr)[from.y][from.x].usedSpecialMove = false;
    (*board_ptr)[to.y][to.x].usedSpecialMove = true;
}

bool Piece_CheckOwnerDestination(playerPiece_t** restrict board, const u16Vect2 from, const u16Vect2 to) {
    if(board[from.y][from.x].pieceColor != board[to.y][to.x].pieceColor) {
        return true;
    } else {
        return false;
    }
}

static void __MovePiece__(playerPiece_t*** restrict board_ptr, const u16Vect2 from, const u16Vect2 to) {
    (*board_ptr)[to.y][to.x].pieceType = (*board_ptr)[from.y][from.x].pieceType;
    (*board_ptr)[to.y][to.x].pieceColor = (*board_ptr)[from.y][from.x].pieceColor;

    (*board_ptr)[from.y][from.x].pieceType = piece_NONE;
    (*board_ptr)[from.y][from.x].pieceColor = color_NONE;

    Piece_ToggleSpecialMove(board_ptr, from, to);
}

void Piece_Move(playerPiece_t*** restrict board_ptr, const u16Vect2 from, const u16Vect2 to, bool* restrict doCastling) {
    if((*doCastling) == false) {
        __MovePiece__(board_ptr, from, to);
    } else {
        *doCastling = false;
        Piece_MoveCastling(board_ptr, from, to);
    }
}

bool Piece_ISOwnerValid(playerPiece_t** restrict mainBoard, const u16Vect2 from, const __color_t currentTurnColor) {
    if(mainBoard[from.y][from.x].pieceColor == currentTurnColor){
        return true;
    }else{
        return false;
    }
}

bool __CheckValidMove_Pawn__(playerPiece_t** restrict mainBoard, const u16Vect2 from, const u16Vect2 to) {
    switch(mainBoard[from.y][from.x].pieceColor){
        case white:
            if((from.x == to.x) && ((to.y == from.y + 1) || (to.y == from.y + 2 && mainBoard[from.y][from.x].usedSpecialMove == false)) && (mainBoard[to.y][to.x].pieceColor == color_NONE)){
                return true;
            }else if(((from.x -1 == to.x) || (from.x +1 == to.x)) && (to.y == from.y + 1) && (mainBoard[to.y][to.x].pieceColor == black)){
                return true;
            }else{
                return false;
            }
            break;
        case black:
            if((from.x == to.x) && ((to.y == from.y - 1 || (to.y == from.y - 2 && mainBoard[from.y][from.x].usedSpecialMove == false))) &&  (mainBoard[to.y][to.x].pieceColor == color_NONE)){
                return true;
            }else if(((from.x -1 == to.x) || (from.x +1 == to.x)) && (to.y == from.y - 1) && (mainBoard[to.y][to.x].pieceColor == white)){
                return true;
            }else{
                return false;
            }
            break;
        default:
            Exception_SendWarning("Warning: Invalid pawn move color (__CheckValidMove_Pawn__)\n");
    }
    return false;
}

// redundant checkbounds
bool __CheckValidMove_Knight__(const u16Vect2 from, const u16Vect2 to, const u16Vect2 boardSize) {
    if(from.x > to.x){
        if(from.y > to.y){
            // - -
            if((Exception_CheckBounds(from.x - 1, from.y - 2, boardSize.x, boardSize.y, true) != OUT_OF_BOUNDS) || (Exception_CheckBounds(from.x - 2, from.y - 1, boardSize.x, boardSize.y, true) != OUT_OF_BOUNDS)){
                return true;
            }
        }else{
            // - +
            if((Exception_CheckBounds(from.x - 1, from.y + 2, boardSize.x, boardSize.y, true) != OUT_OF_BOUNDS) || (Exception_CheckBounds(from.x - 2, from.y + 1, boardSize.x, boardSize.y, true) != OUT_OF_BOUNDS)){
                return true;
            }
        }
    }else if(from.x < to.x){
        if(from.y > to.y){
            // + -
            if((Exception_CheckBounds(from.x + 1, from.y - 2, boardSize.x, boardSize.y, true) != OUT_OF_BOUNDS) || (Exception_CheckBounds(from.x + 2, from.y - 1, boardSize.x, boardSize.y, true) != OUT_OF_BOUNDS)){
                return true;
            }
        }else{
            // + +
            if((Exception_CheckBounds(from.x + 1, from.y + 2, boardSize.x, boardSize.y, true) != OUT_OF_BOUNDS) || (Exception_CheckBounds(from.x + 2, from.y + 1, boardSize.x, boardSize.y, true) != OUT_OF_BOUNDS)){
                return true;
            }
        }
    }
    return false;
}

// redo at some point
bool __CheckValidMove_Bishop__(playerPiece_t** restrict mainBoard, const u16Vect2 from, const u16Vect2 to, const u16Vect2 boardSize) {
    i32 stepX = 0;
    i32 stepY = 0;
    u16 boardIndexX = 0;
    u16 boardIndexY = 0;

    // Check if the move is diagonal (i.e., the absolute differences in X and Y are equal)
    if (abs(from.x - to.x) != abs(from.y - to.y)) {
        return false; // Bishops must move diagonally
    }

    // Ensure the destination (to.x, to.y) is within the bounds of the board
    if (to.x >= boardSize.x || to.y >= boardSize.y) {
        return false; // Destination is out of bounds
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
            return false; // Obstacle found in the path
        }
        boardIndexX += stepX;
        boardIndexY += stepY;
    }

    // Final destination square check
    if (mainBoard[to.y][to.x].pieceColor == color_NONE || mainBoard[to.y][to.x].pieceColor != mainBoard[from.y][from.x].pieceColor) {
        return true; // Valid move: target is either empty or occupied by opponent
    }

    return false; // Invalid move: target is occupied by a piece of the same color
}

// redo at some point
bool __CheckValidMove_Rook__(playerPiece_t** restrict mainBoard, const u16Vect2 from, const u16Vect2 to, const u16Vect2 boardSize) {
    i32 stepX = 0;
    i32 stepY = 0;
    u16 boardIndexX = 0;
    u16 boardIndexY = 0;

    // Rook must move either horizontally or vertically
    if (from.x != to.x && from.y != to.y) {
        return false; // Rook moves in straight lines (horizontally or vertically)
    }

    // Ensure the destination (to.x, to.y) is within the bounds of the board
    if (to.x >= boardSize.x || to.y >= boardSize.y) {
        return false; // Destination is out of bounds
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
                return false; // Obstacle found in the path
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
                return false; // Obstacle found in the path
            }
            boardIndexY += stepY;
        }
    }

    // Final destination square check
    if (mainBoard[to.y][to.x].pieceColor == color_NONE || mainBoard[to.y][to.x].pieceColor != mainBoard[from.y][from.x].pieceColor) {
        return true; // Valid move: target is either empty or occupied by opponent
    }

    return false; // Invalid move: target is occupied by a piece of the same color
}

bool __CheckValidMove_Queen__(playerPiece_t** restrict mainBoard, const u16Vect2 from, const u16Vect2 to, const u16Vect2 boardSize) {
    if((__CheckValidMove_Bishop__(mainBoard, from, to, boardSize) == true) || (__CheckValidMove_Rook__(mainBoard, from, to, boardSize) == true)){
        return true;
    }else{
        return false;
    }
}

bool __CheckValidMove_King__(const u16Vect2 from, const u16Vect2 to) {
    if((to.x == from.x + 1) || (to.x == from.x - 1)){
        if((to.y == from.y + 1) || (to.y == from.y - 1)){
            return true;
        }    
    }
    return false;
}

bool Piece_CheckCastling(playerPiece_t** restrict mainBoard, const u16Vect2 from, const u16Vect2 to) {
    u16 startIndex = 0;
    u16 stopIndex = 0;

    if((mainBoard[from.y][from.x].pieceType == king && mainBoard[to.y][to.x].pieceType == rook) || (mainBoard[from.y][from.x].pieceType == rook && mainBoard[to.y][to.x].pieceType == king))
    if(mainBoard[from.y][from.x].usedSpecialMove == false || mainBoard[to.y][to.x].usedSpecialMove == false) {
        if(from.y == to.y) {
            startIndex = Util_u16Min(from.x, to.x) + 1;
            stopIndex = Util_u16Max(from.x, to.x);

            if(startIndex >= stopIndex) {
                return false;
            }

            for(u16 searchIndex = startIndex; searchIndex < stopIndex; ++searchIndex) {
                if(mainBoard[from.y][searchIndex].pieceType != piece_NONE) {
                    return false;
                }
            }

            return true;
        }
    }

    return false;
}

void Piece_MoveCastling(playerPiece_t*** restrict board_ptr, const u16Vect2 from, const u16Vect2 to) {
    u16 kingX = 0;
    u16 rookX = 0;
    u16 newKingX = 0;
    u16 newRookX = 0;
    
    // identify king and rook
    if((*board_ptr)[from.y][from.x].pieceType == king) {
        kingX = from.x;
        rookX = to.x;
    } else {
        kingX = to.x;
        rookX = from.x;
    }

    if(rookX > kingX) {
        // kingside castling
        newKingX = kingX + 2;
        newRookX = kingX + 1;
    } else {
        // queenside castling
        newKingX = kingX - 2;
        newRookX = kingX - 1;
    }

    __MovePiece__(board_ptr, (u16Vect2){.x = kingX, .y = from.y}, (u16Vect2){.x = newKingX, .y = from.y});
    __MovePiece__(board_ptr, (u16Vect2){.x = rookX, .y = from.y}, (u16Vect2){.x = newRookX, .y = from.y});
}

bool Piece_CheckPawnPromotion(playerPiece_t** restrict board, const u16Vect2 from, const u16Vect2 to, const u16Vect2 boardSize) {
    if(board[from.y][from.x].usedSpecialMove != false) {
        if(to.y == 0 || to.y == boardSize.y - 1) {
            return true;
        }
    }

    return false;
}