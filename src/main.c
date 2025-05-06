#include "../include/typedefs.h"
#include "../include/chessboardlogic.h"
#include "../include/gentools.h"

#include <stdio.h>
#include <errno.h>

#define UNIMPLEMENTED
#define DEBUG 1

#define SLEEP_SECONDS 1

i32 main(){
    playerPiece_t** mainBoard = NULL;
    u16 Xsize = 0;
    u16 Ysize = 0;
    u16 fromXCoords = 0;
    u16 fromYCoords = 0;
    u16 toXCoords = 0;
    u16 toYCoords = 0;
    u16 turnCounter = 1;  // starts at turn 1
    bool skip_loading_screen = false;
    __color_t currentCol = white;  // white starts first

    Board_Settings_General("config/table_config.ini", &Xsize, &Ysize, &skip_loading_screen);

    mainBoard = Board_Init(Xsize, Ysize);

    Board_Placement("config/layout.ini", &mainBoard, Xsize, Ysize);

    while(true){
        Board_Print(mainBoard, Xsize, Ysize);
        printf("\n");
        Board_Turn_Counter(&turnCounter);
        Board_Turn_Read(mainBoard, currentCol, &fromXCoords, &fromYCoords, &toXCoords, &toYCoords, Xsize, Ysize);
        Board_Turn_Move(&mainBoard, fromXCoords, fromYCoords, toXCoords, toYCoords);
        Board_Turn_NextColor(&currentCol);
        Sys_Sleep(SLEEP_SECONDS);
        Sys_Clear();
    }
    

    Board_Free(&mainBoard, Ysize);

    #ifdef DEBUG
    perror("\nErrors");
    #endif
}