#include "../include/graphics.h"
#include "../include/typedefs.h"
#include "../include/chessboardlogic.h"
#include "../include/gentools.h"

#include "raylib.h"

#include <stdio.h>
#include <math.h>

#define TURN_TEXT_MAX_LENGTH 50
#define NUMBERS_TEXT_MAX_LENGTH 6
#define LETTERS_TEXT_MAX_LENGTH 3

#define POPUP_BUTTON_X 50
#define POPUP_BUTTON_Y 40
#define POPUP_BUTTON_SPACING 40
#define POPUP_BUTTON_COUNT 4

static void __Graphics_DrawPiece__(const playerPiece_t currentPiece, const u16 Xcoord, const u16 Ycoord, const pieceTextures_t whitePieces, const pieceTextures_t blackPieces, const f32Vect2 offset) {
    f32 textureX = 0;
    f32 textureY = 0;
    Texture2D loadedTexture = {0};

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
    textureX = offset.x + Xcoord * TILE_SIZE + (TILE_SIZE - loadedTexture.width) / 2;
    textureY = offset.y + Ycoord * TILE_SIZE + (TILE_SIZE - loadedTexture.height) / 2;

    DrawTexture(loadedTexture, textureX, textureY, WHITE);
}

void Graphics_InitRaylib(const i32Vect2 screenSize, const i32 targetFPS) {
    InitWindow(screenSize.x, screenSize.y, WINDOW_TITLE);
    SetTargetFPS(targetFPS);
}

void Graphics_DrawBoard(const u16Vect2 boardSize, const f32Vect2 offset, const boardTheme_t theme) {
    Color color = {0};
    f32Vect2 position = {0};

    for (u16 row = 0; row < boardSize.y; ++row) {
        for (u16 col = 0; col < boardSize.x; ++col) {
            if((row + col) % 2 == 0){
                color = theme.evenTile;
            } else {
                color = theme.oddTile;
            }
            position.x = offset.x + col * TILE_SIZE;
            position.y = offset.y + row * TILE_SIZE;
            DrawRectangleV(position, (f32Vect2){TILE_SIZE, TILE_SIZE}, color);
        }
    }
}

void Graphics_LoadTextures(pieceTextures_t* whitePieces_ptr, pieceTextures_t* blackPieces_ptr) {
    whitePieces_ptr->rook = LoadTexture(TEXTURE_WHITE_ROOK);
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

void Graphics_DrawPieces(playerPiece_t** restrict mainBoard, const u16Vect2 boardSize, const pieceTextures_t whitePieces, const pieceTextures_t blackPieces, const f32Vect2 offset) {
    for(u16 Yindex = 0; Yindex < boardSize.y; ++Yindex) {
        for(u16 Xindex = 0; Xindex < boardSize.x; ++Xindex) {
            if(mainBoard[Yindex][Xindex].pieceColor != color_NONE) {
                __Graphics_DrawPiece__(mainBoard[Yindex][Xindex], Xindex, (boardSize.y - 1) - Yindex, whitePieces, blackPieces, offset); // mirror the board
            }
        }
    }
}

static f32Vect2 Graphics_ConvertToTile(f32Vect2 mousePos, const f32Vect2 offset) {
    mousePos.x = (mousePos.x - offset.x) / TILE_SIZE;
    mousePos.y = (mousePos.y - offset.y) / TILE_SIZE;

    mousePos.x = floorf(mousePos.x);
    mousePos.y = floorf(mousePos.y);

    return mousePos;
}

static f32Vect2 Graphics_FlipAxis (f32Vect2 mousePos, const u16 boardSizeY) {
    mousePos.y = boardSizeY - 1 - mousePos.y; // -1 to ajust for the 0 array indexing in C

    return mousePos;
}

f32Vect2 Graphics_MapMouseToBoard(f32Vect2 mousePos, const u16 boardSizeY, const f32Vect2 offset) {
    mousePos = Graphics_ConvertToTile(mousePos, offset);
    mousePos = Graphics_FlipAxis(mousePos, boardSizeY);

    return mousePos;
}

static f32Vect2 Graphics_ConvertToPoint(f32Vect2 boardPos, const f32Vect2 offset) {
    boardPos.x = (boardPos.x / TILE_SIZE) + offset.x;
    boardPos.y = (boardPos.y / TILE_SIZE) + offset.y;

    return boardPos;
}

f32Vect2 Graphics_MapBoardToMouse(const u16Vect2 boardPos, const u16 boardSizeY, const f32Vect2 offset) {
    f32Vect2 fboardPos = (f32Vect2){(f32)boardPos.x, (f32)boardPos.y};

    f32Vect2 mousePos = Graphics_FlipAxis(fboardPos, boardSizeY);
    mousePos = Graphics_ConvertToPoint(fboardPos, offset);


    return mousePos;
}

void Graphics_PrintTurn(const u16 turnCounter, const f32Vect2 offset, const __color_t currentColor) {
    u8* colorText = NULL;
    u8 finalText[TURN_TEXT_MAX_LENGTH] = {0};
    i32 textPosX = 0;
    i32 textPosY = 0;
    Color playerColor = WHITE;

    switch(currentColor) {
        case white:
            colorText = "White";
            playerColor = GRAY;
            break;
        case black:
            colorText = "Black";
            playerColor = BLACK;
            break;
        default:
            Exception_SendWarning("Warning: Invalid current player color (Graphics_PrintTurn)\n");
            colorText = "color_NONE";
            playerColor = RED;
    }

    snprintf(finalText, sizeof(finalText), "Turn %hu (%s's turn)", turnCounter, colorText);

    textPosX = offset.x + 4;
    textPosY = offset.y - TILE_SIZE;
    
    DrawText(finalText, textPosX, textPosY, 25, playerColor);
}

u32Vect2 Graphics_CalculateBoardPixels(const u16Vect2 boardSize) {
    return (u32Vect2){.x = TILE_SIZE * boardSize.x, .y = TILE_SIZE * boardSize.y};
}

f32Vect2 Graphics_CalculateOffset(const i32Vect2 screenSize, const u32Vect2 boardPixelSize) {
    return (f32Vect2){.x = (screenSize.x - boardPixelSize.x) / 2, .y = (screenSize.y - boardPixelSize.y) / 2};
}

void Graphics_PrintCoordinates(const u16Vect2 boardSize, const u32Vect2 boardPixelSize, const f32Vect2 offset) {
    u8 numberText[NUMBERS_TEXT_MAX_LENGTH] = {0};
    u8 letterText[LETTERS_TEXT_MAX_LENGTH] = {0};
    u8 letterVal = 'a';
    u16 numberVal = 1;
    i32 textWidthNumber = 0;
    i32 textWidthLetter = 0;
    i32 posYNumber = 0;
    i32 posXLetter = 0;

    for (u16 currentY = 0; currentY < boardSize.y; ++currentY) {
        snprintf(numberText, sizeof(numberText), "%hu", numberVal);

        textWidthNumber = MeasureText(numberText, USUAL_FONT_SIZE);

        posYNumber = offset.y + ((boardSize.y - 1 - currentY) * TILE_SIZE) + TILE_SIZE / 2 - USUAL_FONT_SIZE / 2;

        DrawText(numberText, offset.x - textWidthNumber - 5, posYNumber, USUAL_FONT_SIZE, DARKGRAY);
        DrawText(numberText, offset.x + boardPixelSize.x + 5, posYNumber, USUAL_FONT_SIZE, DARKGRAY);

        ++numberVal;
    }

    for (u16 x = 0; x < boardSize.x; ++x) {
        snprintf(letterText, sizeof(letterText), "%c", letterVal);

        textWidthLetter = MeasureText(letterText, USUAL_FONT_SIZE);
        posXLetter = offset.x + (x * TILE_SIZE) + TILE_SIZE / 2 - textWidthLetter / 2;

        DrawText(letterText, posXLetter, offset.y + boardPixelSize.y + 5, USUAL_FONT_SIZE, DARKGRAY);
        DrawText(letterText, posXLetter, offset.y - USUAL_FONT_SIZE - 5, USUAL_FONT_SIZE, DARKGRAY);

        ++letterVal;
    }
}

void Graphics_PawnPromotionPopup(f32Vect2 position, const pieceTextures_t textures) {
    Texture2D popupTextures[] = {textures.rook, textures.knight, textures.bishop, textures.queen};

    for(u8_EXPLICIT buttonIndex = 0; buttonIndex < POPUP_BUTTON_COUNT; ++buttonIndex) {
        DrawRectangle((i32)position.x, (i32)position.y,POPUP_BUTTON_X, POPUP_BUTTON_Y, GRAY);
        DrawTexture(popupTextures[buttonIndex], (i32)position.x, (i32)position.y, WHITE);
        position.x += POPUP_BUTTON_SPACING;
    }
}