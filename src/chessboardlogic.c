// This is where functions related to the backend logic of the game board
// should go

#include "../include/chessboardlogic.h"
#include "../include/gameTexts.h"
#include "../include/typedefs.h"
#include "../include/gentools.h"
#include "../include/pieceMoves.h"

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <ctype.h>

#define __CONFIG_INI_MAX_SIZE__ 120
#define __TURN_COLOR_MAX_SIZE__ 50

playerPiece_t** Board_Init(const u16 boardSizeX, const u16 boardSizeY){
    playerPiece_t** mainBoard = NULL;

    printf(LOADING_MESSAGE_BOARD_EN);

    mainBoard = (playerPiece_t**)malloc(sizeof(playerPiece_t*) * boardSizeY);
    if(mainBoard == NULL){
        #ifdef EXIT_ON_FATAL_ERROR
        fprintf(stderr, "Fatal Error: Main board allocation failed (Board_Init)\n");
        exit(EXIT_FAILURE);
        #endif
    }else{
        for(u16 allocIndex = 0; allocIndex < boardSizeY; ++allocIndex){
            mainBoard[allocIndex] = (playerPiece_t*)malloc(sizeof(playerPiece_t) * boardSizeX);
            Exception_CheckNULLPtr((playerPiece_t*)mainBoard[allocIndex], "Fatal Error: board tiles allocation failed (Board_Init)\n");
        }
    }

    for(u16 Yindex = 0; Yindex < boardSizeY; ++Yindex){
        for(u16 Xindex = 0; Xindex < boardSizeX; ++Xindex){
            mainBoard[Yindex][Xindex].pieceColor = color_NONE;
            mainBoard[Yindex][Xindex].pieceType = piece_NONE;
        }
    }

    return mainBoard;
}

void Board_Settings_LoadGeneral(const u8* restrict ini_filename, u16* XBoardCoord, u16* YBoardCoord, bool* restrict skip_loading_screen, i32* restrict fpsMax_ptr, i32* screenWidth_ptr, i32* screenHeight_ptr, bool* showCoordinates_ptr){
    i8 skip_loading_screen_buffer = 0;
    FILE* ini_ptr = NULL;
    u8 ini_filecontent[__CONFIG_INI_MAX_SIZE__] = {0};

    if((ini_filename == NULL) || (XBoardCoord == NULL) || (YBoardCoord == NULL) || (skip_loading_screen == NULL)){
        #ifdef EXIT_ON_FATAL_ERROR
        fprintf(stderr, "Error: Invalid Board_Settings_General arguments\n");
        exit(EXIT_FAILURE);
        #endif
    }
    
    ini_ptr = fopen(ini_filename, "r");
    Exception_CheckNULLPtr((FILE*)ini_ptr, "Fatal Error: Config Ini could not be opened! (Board_Settings_General)\n");

    // check the ini file format
    // make an algorithm to fallback on default values if the file is not correct
    if(feof(ini_ptr) == 0 && fgets(ini_filecontent, __CONFIG_INI_MAX_SIZE__, ini_ptr) != NULL){
        if(strcmp(ini_filecontent, "[general]\n") != 0){  // fgets also gets the newline terminator \n
            Exception_SendWarning("Warning: Incompatible Ini file content (Board_Settings_General)\n");
        }else{
            if(feof(ini_ptr) == 0 && fgets(ini_filecontent, __CONFIG_INI_MAX_SIZE__, ini_ptr) != NULL){
                sscanf(ini_filecontent, "table_size = %hux%hu", XBoardCoord, YBoardCoord);
            }else{
                Exception_SendWarning("Warning: Incompatible Ini file content (Board_Settings_General)\n");
            }
            if(feof(ini_ptr) == 0 && fgets(ini_filecontent, __CONFIG_INI_MAX_SIZE__, ini_ptr) != NULL){
                sscanf(ini_filecontent, "show_loading_screen = %hhd", &skip_loading_screen_buffer);  // still no bool format specifier in 2025
                if(skip_loading_screen_buffer == 0){
                    *skip_loading_screen = false;
                }else{
                    *skip_loading_screen = true;
                }
            }else{
                Exception_SendWarning("Warning: Incompatible Ini file content (Board_Settings_General)\n");
            }
            if(feof(ini_ptr) == 0 && fgets(ini_filecontent, __CONFIG_INI_MAX_SIZE__, ini_ptr) != NULL){
                sscanf(ini_filecontent, "maxFps = %d", fpsMax_ptr);
            }else{
                Exception_SendWarning("Warning: Incompatible Ini file content (Board_Settings_General)\n");
            }
            if(feof(ini_ptr) == 0 && fgets(ini_filecontent, __CONFIG_INI_MAX_SIZE__, ini_ptr) != NULL){
                sscanf(ini_filecontent, "screenWidth = %d", screenWidth_ptr);
            }else{
                Exception_SendWarning("Warning: Incompatible Ini file content (Board_Settings_General)\n");
            }
            if(feof(ini_ptr) == 0 && fgets(ini_filecontent, __CONFIG_INI_MAX_SIZE__, ini_ptr) != NULL){
                sscanf(ini_filecontent, "screenHeight = %d", screenHeight_ptr);
            }else{
                Exception_SendWarning("Warning: Incompatible Ini file content (Board_Settings_General)\n");
            }
            if(feof(ini_ptr) == 0 && fgets(ini_filecontent, __CONFIG_INI_MAX_SIZE__, ini_ptr) != NULL){
                sscanf(ini_filecontent, "showCoordinates = %hhu", (u8*)showCoordinates_ptr);
            }else{
                Exception_SendWarning("Warning: Incompatible Ini file content (Board_Settings_General)\n");
            }
        }
    }else{
        Exception_SendWarning("Warning: Incompatible Ini file format (Board_Settings_General)\n");
    }

    fclose(ini_ptr);
}

void Board_LoadPlacement(const u8* restrict ini_filename, playerPiece_t*** main_board_ptr, const u16 XBoardCoord, const u16 YBoardCoord){
    FILE* ini_ptr = NULL;
    playerPiece_t piece_Placement_Buffer = {piece_NONE, color_NONE};
    u16 XBoard_Placement_Buffer = 0;
    u16 YBoard_Placement_Buffer = 0;
    u8 ini_filecontent[__CONFIG_INI_MAX_SIZE__] = {0};

    printf(LOADING_MESSAGE_PIECES_EN);

    Exception_CheckNULLPtr((u8*)ini_filename, "Type Error: Invalid ini filename (Board_Placement)\n");
    
    ini_ptr = fopen(ini_filename, "r");
    Exception_CheckNULLPtr((FILE*)ini_ptr, "Fatal Error: Config Ini could not be opened! (Board_Placement)\n");
    
    if(feof(ini_ptr) == 0 && fgets(ini_filecontent, __CONFIG_INI_MAX_SIZE__, ini_ptr) != NULL){
        if(strcmp(ini_filecontent, "[layout]\n") != 0){  // fgets also gets the newline terminator \n
            Exception_SendWarning("Warning: Incompatible Ini file content (Board_Placement)\n");
        }else{
            while(feof(ini_ptr) == 0 && fgets(ini_filecontent, __CONFIG_INI_MAX_SIZE__, ini_ptr) != NULL){
                sscanf(ini_filecontent, "piece = %c[%hu][%hu], color = %c", (u8*)&piece_Placement_Buffer.pieceType, &YBoard_Placement_Buffer, &XBoard_Placement_Buffer, (u8*)&piece_Placement_Buffer.pieceColor);
                if(Exception_CheckBounds(XBoard_Placement_Buffer, YBoard_Placement_Buffer, XBoardCoord, YBoardCoord, false) == IN_BOUNDS){
                    (*main_board_ptr)[YBoard_Placement_Buffer][XBoard_Placement_Buffer].pieceColor = piece_Placement_Buffer.pieceColor;
                    (*main_board_ptr)[YBoard_Placement_Buffer][XBoard_Placement_Buffer].pieceType = piece_Placement_Buffer.pieceType;
                }
            }
        }
    }

    fclose(ini_ptr);
}

void Board_Free(playerPiece_t*** main_board_ptr, const u16 YBoardCoord){
    for(u16 freeIndex = 0; freeIndex < YBoardCoord; ++freeIndex){
        free((*main_board_ptr)[freeIndex]);
    }
    free((*main_board_ptr));
}

void Board_Turn_NextColor(__color_t* restrict color_ptr){
    switch(*color_ptr){
        case white:
            *color_ptr = black;
            break;
        case black: 
            *color_ptr = white;
            break;
        default:
            Exception_SendWarning("Warning: Invalid turn color (Board_Turn_NextColor)\n");
    }
}

void Board_Save_SaveGame(playerPiece_t** restrict mainBoard, const u16 XBoardCoord, const u16 YBoardCoord, u8* restrict filename){
    FILE* saveFile_ptr = NULL;

    saveFile_ptr = fopen(filename, "w");
    Exception_CheckNULLPtr((FILE*)saveFile_ptr, "Fatal Error: Game save failed (Board_Save_SaveGame)\n");

    fprintf(saveFile_ptr, "[layout]\n");

    for(u16 boardIndexY = 0; boardIndexY < YBoardCoord; ++boardIndexY){
        for(u16 boardIndexX = 0; boardIndexX < XBoardCoord; ++boardIndexX){
            if(mainBoard[boardIndexY][boardIndexX].pieceColor != color_NONE){
                fprintf(saveFile_ptr, "piece = %c[%hu][%hu], color = %c\n", mainBoard[boardIndexY][boardIndexX].pieceType, boardIndexY, boardIndexX, mainBoard[boardIndexY][boardIndexX].pieceColor);
            }
        }
    }
    
    fclose(saveFile_ptr);
}

// quick and dirty implementation that probably needs to be fixed later
bool Board_Save_LoadGame(u8* restrict loadPrompt){
    u8 keyboardLetter = 0;

    printf("%s\n", loadPrompt);
    scanf(" %c", &keyboardLetter);

    keyboardLetter = toupper(keyboardLetter);

    if(keyboardLetter != 'Y'){
        return false;
    }else{
        return true;
    }
}

bool Board_IsInsideBoard(const Vector2 mousePos, const u16 boardSizeX, const u16 boardSizeY) {
    if((mousePos.x < 0 || mousePos.x >= boardSizeX) || (mousePos.y < 0 || mousePos.y >= boardSizeY)) {
        return false;
    } else {
        return true;
    }
}

bool Board_GetFrom(playerPiece_t** restrict mainBoard, const u16 boardSizeX, const u16 boardSizeY, u16* fromX_ptr, u16* fromY_ptr, const __color_t currentPlayerCol, bool* restrict highlightSelection, const f32 offsetX, const f32 offsetY) {
    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT) != false) {
        Vector2 mousePos = Graphics_MapMouseToBoard(GetMousePosition(), boardSizeY, offsetX, offsetY);
        if(Board_IsInsideBoard(mousePos, boardSizeX, boardSizeY) != false) {
            *fromX_ptr = (u16)mousePos.x;
            *fromY_ptr = (u16)mousePos.y;
            if(mainBoard[*fromY_ptr][*fromX_ptr].pieceColor == currentPlayerCol) {
                    *highlightSelection = true;
                    return true;
                } else {
                    *highlightSelection = false;
                    return false;
                }
            } else {
                *highlightSelection = false;
                return false;
            }
    } else {
        return false;
    }
}

bool Board_GetTo(playerPiece_t** restrict mainBoard, const u16 boardSizeX, const u16 boardSizeY, const u16 fromX, const u16 fromY, u16* toX_ptr, u16* toY_ptr, const f32 offsetX, const f32 offsetY) {
    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT) != false) {
        Vector2 mousePos = Graphics_MapMouseToBoard(GetMousePosition(), boardSizeY, offsetX, offsetY);
        i8 validPieceMove = INVALID_MOVE;
        if(Board_IsInsideBoard(mousePos, boardSizeX, boardSizeY) != false) {
            *toX_ptr = (u16)mousePos.x;
            *toY_ptr = (u16)mousePos.y;

            switch(mainBoard[fromY][fromX].pieceType){
                case pawn:
                    validPieceMove = __CheckValidMove_Pawn__(mainBoard, fromX, fromY, *toX_ptr, *toY_ptr);
                    break;
                case knight:
                    validPieceMove = __CheckValidMove_Knight__(fromX, fromY, *toX_ptr, *toY_ptr, boardSizeX, boardSizeY);
                    break;
                case bishop:
                    validPieceMove = __CheckValidMove_Bishop__(mainBoard, fromX, fromY, *toX_ptr, *toY_ptr, boardSizeX, boardSizeY);
                    break;
                case rook:
                    validPieceMove = __CheckValidMove_Rook__(mainBoard, fromX, fromY, *toX_ptr, *toY_ptr, boardSizeX, boardSizeY);
                    break;
                case queen:
                    validPieceMove = __CheckValidMove_Queen__(mainBoard, fromX, fromY, *toX_ptr, *toY_ptr, boardSizeX, boardSizeY);
                    break;
                case king:
                    validPieceMove = __CheckValidMove_King__(fromX, fromY, *toX_ptr, *toY_ptr);
                    break;
                default:    
                    printf("Unimplemented (Board_Turn_Read)\n");
                    validPieceMove = INVALID_MOVE;
            }
            if(validPieceMove == VALID_MOVE) {
                return true;
            } else {
                return false;
            }
        } else {
            return false;
        }
    } else {
        return false;
    }
}

bool Board_UndoSelection(const u16 boardSizeX, const u16 boardSizeY, const f32 offsetX, const f32 offsetY) {
    if(IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) != false) {
        Vector2 mousePos = Graphics_MapMouseToBoard(GetMousePosition(), boardSizeY, offsetX, offsetY);
        if(Board_IsInsideBoard(mousePos, boardSizeX, boardSizeY) != false) {
            return true;
        } else {
            return false;
        }
    } else {
        return false;
    }
}