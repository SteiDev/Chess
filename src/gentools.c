// This is where general utility functions should go

#include "../include/gentools.h"
#include "../include/typedefs.h"
#include "../include/graphics.h"

#include "raylib.h"

#include <stdio.h>
#include <stdlib.h>

#ifdef OS_UNIX
    #include <unistd.h>
#elif defined OS_WINDOWS
    #include <synchapi.h>
#endif

void Exception_SendWarning(const u8* restrict warningMsg){
    fprintf(stderr, "%s", warningMsg);
    #ifdef EXIT_ON_WARNING
    exit(EXIT_FAILURE);
    #endif
}

void Exception_CheckCriticalOperation(void* restrict general_ptr, const u8* restrict errorMsg){
    if(general_ptr == NULL){
        fprintf(stderr, "%s", errorMsg);
        #ifdef EXIT_ON_FATAL_ERROR
        exit(EXIT_FAILURE);
        #endif
    }
}

i8 Exception_CheckBounds(const u16 currentX, const u16 currentY, const u16 maxX, const u16 maxY, const bool supressWarnings){
    if((currentX >= maxX) || (currentY >= maxY)){
        if(supressWarnings != true){
            Exception_SendWarning("Warning: trying to set pieces out bounds (CheckBounds)\n");
        }
        return OUT_OF_BOUNDS;
    }else{
        return IN_BOUNDS;
    }
}

void Exception_CheckLoadedTexture(pieceTextures_t inptexture) {
    // This function may have some C++ stuff
    if (inptexture.rook.id == 0) {
    TraceLog(LOG_WARNING, "Failed to load rook texture!");
    Exception_SendWarning("Warning: Failed to load rook texture (Exception_CheckLoadedTexture)\n");
    }

    if (inptexture.knight.id == 0) {
        TraceLog(LOG_WARNING, "Failed to load knight texture!");
        Exception_SendWarning("Warning: Failed to load knight texture (Exception_CheckLoadedTexture)\n");
    }

    if (inptexture.bishop.id == 0) {
        TraceLog(LOG_WARNING, "Failed to load bishop texture!");
        Exception_SendWarning("Warning: Failed to load bishop texture (Exception_CheckLoadedTexture)\n");
    }

    if (inptexture.queen.id == 0) {
        TraceLog(LOG_WARNING, "Failed to load queen texture!");
        Exception_SendWarning("Warning: Failed to load queen texture (Exception_CheckLoadedTexture)\n");
    }

    if (inptexture.king.id == 0) {
        TraceLog(LOG_WARNING, "Failed to load king texture!");
        Exception_SendWarning("Warning: Failed to load king texture (Exception_CheckLoadedTexture)\n");
    }

    if (inptexture.pawn.id == 0) {
        TraceLog(LOG_WARNING, "Failed to load pawn texture!");
        Exception_SendWarning("Warning: Failed to load pawn texture (Exception_CheckLoadedTexture)\n");
    }
}

void Sys_Clear(void){
    #ifdef OS_UNIX
        system(UNIX_CLEAR);
    #elif defined OS_WINDOWS
        system(WIN_CLEAR);
    #endif
}

void Sys_Sleep(const u32 seconds){
    #ifdef OS_UNIX
    sleep(seconds);
    #elif defined OS_WINDOWS
    Sleep(seconds * 1000);
    #endif
}