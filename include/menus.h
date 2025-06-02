#ifndef __MENUS_H__
#define __MENUS_H__

#include "../include/typedefs.h"

typedef enum currentMenu_t {
    start,
    inGame,
    loadGame,
    quit,
    menu_NONE
} currentMenu_t;

f32Vect2 Menu_CalculateButtonOffset(const i32Vect2 windowSize);
void Menu_PrintButtons(const f32Vect2 buttonOffset);
currentMenu_t Menu_GetSelectedMenu(const f32Vect2 buttonOffset); 

#endif  // end of __MENUS_H__