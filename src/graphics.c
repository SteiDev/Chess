#include "../include/graphics.h"
#include "../include/typedefs.h"
#include "../include/gameTexts.h"
#include "../include/chessboardlogic.h"
#include "../include/gentools.h"

#include "raylib.h"

static void __Graphics_DrawPiece__(const playerPiece_t currentPiece, const u16 Xcoord, const u16 Ycoord, const pieceTextures_t whitePieces, const pieceTextures_t blackPieces) {
    const i32 textureX = Xcoord * SQUARE_SIZE;
    const i32 textureY = Ycoord * SQUARE_SIZE;
    Texture2D loadedTexture = {0};
    Color loadedColor = {0};

    switch(currentPiece.pieceColor) {
        case white:
            loadedColor = WHITE;
            break;
        case black:
            loadedColor = BLACK;
            break;
        default:
        Exception_SendWarning("Warning: Invalid piece texture color (__Graphics_DrawPiece__)\n");
        return;
    }

    switch(currentPiece.pieceType) {
        case rook:
            if(currentPiece.pieceColor == white) {
                loadedTexture = whitePieces.rook;
            } else {
                loadedTexture = blackPieces.rook;
            }
            break;
        case knight:
            if(currentPiece.pieceColor == white) {
                loadedTexture = whitePieces.knight;
            } else {
                loadedTexture = blackPieces.knight;
            }
            break;
        case bishop:
            if(currentPiece.pieceColor == white) {
                loadedTexture = whitePieces.bishop;
            } else {
                loadedTexture = blackPieces.bishop;
            }
            break;
        case queen:
            if(currentPiece.pieceColor == white) {
                loadedTexture = whitePieces.queen;
            } else {
                loadedTexture = blackPieces.queen;
            }
            break;
        case king:
            if(currentPiece.pieceColor == white) {
                loadedTexture = whitePieces.king;
            } else {
                loadedTexture = blackPieces.king;
            }
            break;
        case pawn:
            if(currentPiece.pieceColor == white) {
                loadedTexture = whitePieces.pawn;
            } else {
                loadedTexture = blackPieces.pawn;
            }
            break;
        default:
        Exception_SendWarning("Warning: Invalid piece texture type (__Graphics_DrawPiece__)\n");
    }

    DrawTexture(loadedTexture, textureX, textureY, loadedColor);
}

void Graphics_InitRaylib(const u16 screenWidth, const u16 screenHeight, const u16 targetFPS) {
    InitWindow(screenWidth, screenHeight, WINDOW_TITLE);
    SetTargetFPS(targetFPS);
}

void Graphics_DrawBoard(const u16 boardSizeX, const u16 boardSizeY) {
    Color color = {0};
    Vector2 position = {0};

    for (u16 row = 0; row < boardSizeY; row++) {
        for (u16 col = 0; col < boardSizeX; col++) {
            if((row + col) % 2 == 0){
                color = DARKGRAY;
            } else {
                color = LIGHTGRAY;
            }
            position.x = col * SQUARE_SIZE;
            position.y = row * SQUARE_SIZE;
            DrawRectangleV(position, (Vector2){SQUARE_SIZE, SQUARE_SIZE}, color);
        }
    }
}

void Graphics_LoadTextures(pieceTextures_t* whitePieces_ptr, pieceTextures_t* blackPieces_ptr) {
    whitePieces_ptr->rook = LoadTexture(TEXTURE_WHITE_ROOK); // here is the crash
    whitePieces_ptr->knight = LoadTexture(TEXTURE_WHITE_KNIGHT);
    whitePieces_ptr->bishop = LoadTexture(TEXTURE_WHITE_BISHOP);
    whitePieces_ptr->queen = LoadTexture(TEXTURE_WHITE_QUEEN);
    whitePieces_ptr->king = LoadTexture(TEXTURE_WHITE_KING);
    whitePieces_ptr->pawn = LoadTexture(TEXTURE_WHITE_PAWN);

    blackPieces_ptr->rook = LoadTexture(TEXTURE_BLACK_ROOK);
    blackPieces_ptr->knight = LoadTexture(TEXTURE_BLACK_KNIGHT);
    blackPieces_ptr->bishop = LoadTexture(TEXTURE_BLACK_BISHOP);
    blackPieces_ptr->queen = LoadTexture(TEXTURE_BLACK_QUEEN);
    blackPieces_ptr->king = LoadTexture(TEXTURE_BLACK_KING);
    blackPieces_ptr->pawn = LoadTexture(TEXTURE_BLACK_PAWN);

    Exception_CheckLoadedTexture(*whitePieces_ptr);
    Exception_CheckLoadedTexture(*blackPieces_ptr);
}

void Graphics_UnloadTextures(pieceTextures_t* whitePieces_ptr, pieceTextures_t* blackPieces_ptr) {
    UnloadTexture(whitePieces_ptr->rook);
    UnloadTexture(whitePieces_ptr->knight);
    UnloadTexture(whitePieces_ptr->bishop);
    UnloadTexture(whitePieces_ptr->queen);
    UnloadTexture(whitePieces_ptr->king);
    UnloadTexture(whitePieces_ptr->pawn);

    UnloadTexture(blackPieces_ptr->rook);
    UnloadTexture(blackPieces_ptr->knight);
    UnloadTexture(blackPieces_ptr->bishop);
    UnloadTexture(blackPieces_ptr->queen);
    UnloadTexture(blackPieces_ptr->king);
    UnloadTexture(blackPieces_ptr->pawn);    
}

void Graphics_DrawPieces(playerPiece_t** restrict mainBoard, const u16 boardSizeX, const u16 boardSizeY, const pieceTextures_t whitePieces, const pieceTextures_t blackPieces) {
    for(u16 Yindex = 0; Yindex < boardSizeY; ++Yindex) {
        for(u16 Xindex = 0; Xindex < boardSizeX; ++Xindex) {
            if(mainBoard[Yindex][Xindex].pieceColor != color_NONE) {
                __Graphics_DrawPiece__(mainBoard[Yindex][Xindex], Xindex, Yindex, whitePieces, blackPieces);
            }
        }
    }
}