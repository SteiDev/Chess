#include "../include/graphics.h"
#include "../include/typedefs.h"
#include "../include/gameTexts.h"

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