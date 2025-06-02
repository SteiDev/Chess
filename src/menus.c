#include "../include/menus.h"

#include "raylib.h"

#define BUTTON_X 250
#define BUTTON_Y 100
#define BUTTON_COUNT 3
#define BUTTON_Y_SPACING BUTTON_Y * 1.5

#define BUTTON_COL_START GREEN
#define BUTTON_COL_LOAD_GAME YELLOW
#define BUTTON_COL_QUIT RED

#define BUTTON_TEXT_START "Start"
#define BUTTON_TEXT_LOAD_GAME "Load an existing game"
#define BUTTON_TEXT_QUIT "Quit game"

#define BUTTON_TEXT_FONT_SIZE 20
#define BUTTON_TEXT_COLOR BLACK

static f32Vect2 __Menu_CalculateTextOffset__(const u8* buttonText, f32 currentY) {
    const i32 textX = MeasureText(buttonText, BUTTON_TEXT_FONT_SIZE);
    f32Vect2 textOffset = {0};

    textOffset.x = (BUTTON_X - textX) / 2;
    textOffset.y = currentY + (BUTTON_Y / 2);

    return textOffset;
}

f32Vect2 Menu_CalculateButtonOffset(const i32Vect2 windowSize) {
    f32Vect2 buttonOffset = {0};

    buttonOffset.x = (windowSize.x - BUTTON_X) / 2;
    buttonOffset.y = (windowSize.y - BUTTON_Y) / BUTTON_COUNT;

    return buttonOffset;
}

void Menu_PrintButtons(const f32Vect2 buttonOffset) {
    f32 currentY = buttonOffset.y;
    f32Vect2 textPos = {0};
    const u8* buttonText[BUTTON_COUNT] = {BUTTON_TEXT_START, BUTTON_TEXT_LOAD_GAME, BUTTON_TEXT_QUIT};
    const Color buttonColors[BUTTON_COUNT] = {BUTTON_COL_START, BUTTON_COL_LOAD_GAME, BUTTON_COL_QUIT};

    for(u8_EXPLICIT buttonIdx = 0; buttonIdx < BUTTON_COUNT; ++buttonIdx) {
        textPos = __Menu_CalculateTextOffset__(buttonText[buttonIdx], currentY);
        textPos.x += buttonOffset.x;

        DrawRectangle(buttonOffset.x, currentY, BUTTON_X, BUTTON_Y, buttonColors[buttonIdx]);
        DrawText(buttonText[buttonIdx], textPos.x, textPos.y, BUTTON_TEXT_FONT_SIZE, BUTTON_TEXT_COLOR);

        currentY += BUTTON_Y_SPACING;
    }   
}

currentMenu_t Menu_GetSelectedMenu(const f32Vect2 buttonOffset) {
    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT) != false) {
        f32Vect2 mousePos = GetMousePosition();

        if(mousePos.x >= buttonOffset.x && mousePos.x <= buttonOffset.x + BUTTON_X) {
            /* now check for each button Y */
        }
    }

    return menu_NONE;
}