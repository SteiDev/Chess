// This is where general utility functions should go

#include "../include/gentools.h"
#include "../include/typedefs.h"
#include "../include/graphics.h"

#include "raylib.h"

#include <stdio.h>
#include <stdlib.h>

void Exception_SendWarning(const u8* restrict warningMsg){
    fprintf(stderr, "%s", warningMsg);
    #ifdef EXIT_ON_WARNING
    exit(EXIT_FAILURE);
    #endif
}

void Exception_CheckNULLPtr(void* restrict general_ptr, const u8* restrict errorMsg){
    if(general_ptr == NULL){
        fprintf(stderr, "%s", errorMsg);
        #ifdef EXIT_ON_FATAL_ERROR
        exit(EXIT_FAILURE);
        #endif
    }
}

i8 Exception_CheckBounds(const u16 currentX, const u16 currentY, const u16 maxX, const u16 maxY, const bool suppressWarnings){
    if((currentX >= maxX) || (currentY >= maxY)){
        if(suppressWarnings == false){
            Exception_SendWarning("Warning: trying to set pieces out bounds (CheckBounds)\n");
        }
        return OUT_OF_BOUNDS;
    }else{
        return IN_BOUNDS;
    }
}

void Exception_CheckLoadedTexture(pieceTextures_t inptexture) {
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

void Debug_ResetDumpFile(void) {
    FILE* dumpFile = fopen("debug/vardump.txt", "w");
    if(dumpFile == NULL) {
        Exception_SendWarning("Warning: Dump file cannot be opened (Debug_ResetDumpFile)\n");
    }

    fclose(dumpFile);
}

void Debug_DumpVariables(const u16 turnCounter, const u32 boardPixelWidth, const u32 boardPixelHeight, const i32 screenWidth, const i32 screenHeight, const i32 fpsMax, 
                        const u16Vect2 boardSize, const u16Vect2 from, const u16Vect2 to, const f32Vect2 offset, u8* layoutName, const bool loadPreviousGame, 
                        const bool validFrom, const bool validTo, const bool highlightSelection, const bool undoSelection, const bool showCoordinates, const __color_t currentCol,
                        const gameState_t gameState) {
    FILE* dumpFile = fopen("debug/vardump.txt", "a+w");
    if(dumpFile != NULL) {
        fprintf(dumpFile, "turnCounter: %hu\nboardPixelWidth: %u\nboardPixelHeight: %u\nscreenWidth: %d\nscreenHeight: %d\nfpsMax: %d\n", turnCounter, boardPixelWidth, boardPixelHeight, screenWidth, screenHeight, fpsMax);
        fprintf(dumpFile, "boardSize.x: %hu\nboardSize.y: %hu\nfrom.x: %hu\nfrom.y: %hu\nto.x: %hu\nto.y: %hu\noffset.x: %f\noffset.y: %f\nlayoutName: %s\nloadPreviousGame: %hhu\n", boardSize.x, boardSize.y, from.x, from.y, to.x, to.y, offset.x, offset.y, layoutName, loadPreviousGame);
        fprintf(dumpFile, "validFrom: %hhu\nvalidTo: %hhu\nhighlightSelection: %hhu\nundoSelection: %hhu\nshowCoordinates: %hhu\ncurrentCol: %d\n", validFrom, validTo, highlightSelection, undoSelection, showCoordinates, currentCol);
        fprintf(dumpFile, "gameState: %d\n\n", gameState);
    } else {
        Exception_SendWarning("Warning: Dump file cannot be opened (Debug_DumpVariables)\n");
    }

    fclose(dumpFile);
}