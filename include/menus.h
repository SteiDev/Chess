#ifndef __MENUS_H__
#define __MENUS_H__

#include "../include/typedefs.h"

typedef enum menu_t {
    start,
    inGame,
    loadGame,
    quit,
    watch,
    menu_NONE
} menu_t;

f32Vect2 Menu_CalculateButtonOffset(const i32Vect2 windowSize);
void Menu_PrintButtons(const f32Vect2 buttonOffset);
menu_t Menu_GetSelectedMenu(const f32Vect2 buttonOffset); 

#endif  // end of __MENUS_H__