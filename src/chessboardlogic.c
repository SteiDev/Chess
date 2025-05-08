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

    Sys_Clear();

    return mainBoard;
}

void Board_Print(playerPiece_t** inpboard, const u16 Xsize, const u16 Ysize){
    u8 rowIndex = '1' + Ysize - 1;
    u8 columnIndex;
    
    printf("  ");
    for(columnIndex = 'a'; columnIndex < 'a' + Xsize; ++columnIndex){
        printf("%c  ", columnIndex);
    }
    printf("\n");
    for(u16 Yindex = Ysize - 1; Yindex != __UINT16_MAX__; --Yindex){ // underflow by design
        printf("%c ", rowIndex);
        for(u16 Xindex = 0; Xindex < Xsize; ++Xindex){
            printf("%c", inpboard[Yindex][Xindex].pieceColor);
            printf("%c ", inpboard[Yindex][Xindex].pieceType);
        }
        printf("%c", rowIndex);
        --rowIndex;
        printf("\n");
    }
    printf("  ");
    for(columnIndex = 'a'; columnIndex < 'a' + Xsize; ++columnIndex){
        printf("%c  ", columnIndex);
    }
    printf("\n");
}

void Board_Settings_LoadGeneral(const u8* restrict ini_filename, u16* XBoardCoord, u16* YBoardCoord, bool* restrict skip_loading_screen){
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

    Sys_Clear();
}

void Board_Free(playerPiece_t*** main_board_ptr, const u16 YBoardCoord){
    for(u16 freeIndex = 0; freeIndex < YBoardCoord; ++freeIndex){
        free((*main_board_ptr)[freeIndex]);
    }
    free((*main_board_ptr));
}

void Board_Turn_ReadAndValidate(playerPiece_t** restrict mainBoard, const __color_t currentColor, u16* fromCoordsX, u16* fromCoordsY, u16* toCoordsX, u16* toCoordsY, const u16 XBoardCoord, const u16 YBoardCoord){
    u8 turnColor[__TURN_COLOR_MAX_SIZE__];
    u8 aux_fromCoordsX = 0;
    u8 aux_toCoordsX = 0;
    i8 validPieceMove = 0;

    switch(currentColor){
        case white:
            strcpy(turnColor, "White");
            break;
        case black:
            strcpy(turnColor, "Black");
            break;
        default:
            Exception_SendWarning("Warning: Invalid Color Turn (Board_Turn)\n");
    }

    printf("\n%s's Turn\n\n", turnColor);
    
    do{

        do{
            do{
                printf("Enter the location of a piece: ");
                scanf(" %c %hu", &aux_fromCoordsX, fromCoordsY);
        
                *fromCoordsX = toupper(aux_fromCoordsX);
                *fromCoordsX = UPPERLETTER_TO_NUM(*fromCoordsX);
                *fromCoordsX = (*fromCoordsX) - 1;  // ajust for chessboard indexing from 1
                *fromCoordsY = (*fromCoordsY) - 1;
        
            }while(Exception_CheckBounds(*fromCoordsX, *fromCoordsY, XBoardCoord, YBoardCoord, false) != IN_BOUNDS);
        }while(Piece_CheckOwner(mainBoard, *fromCoordsX, *fromCoordsY, currentColor) != VALID_OWNER);
        
        do{
            printf("Enter the location to move the piece to: ");
            scanf(" %c %hu", &aux_toCoordsX, toCoordsY);
    
            *toCoordsX = toupper(aux_toCoordsX);
            *toCoordsX = UPPERLETTER_TO_NUM(*toCoordsX);
            *toCoordsX = (*toCoordsX) - 1;
            *toCoordsY = (*toCoordsY) - 1;
    
        }while(Exception_CheckBounds(*toCoordsX, *toCoordsY, XBoardCoord, YBoardCoord, false) != IN_BOUNDS);
    
        switch(mainBoard[*fromCoordsY][*fromCoordsX].pieceType){
            case pawn:
                validPieceMove = __CheckValidMove_Pawn__(mainBoard, *fromCoordsX, *fromCoordsY, *toCoordsX, *toCoordsY);
                break;
            case knight:
                validPieceMove = __CheckValidMove_Knight__(*fromCoordsX, *fromCoordsY, *toCoordsX, *toCoordsY, XBoardCoord, YBoardCoord);
                break;
            case bishop:
                validPieceMove = __CheckValidMove_Bishop__(mainBoard, *fromCoordsX, *fromCoordsY, *toCoordsX, *toCoordsY, XBoardCoord, YBoardCoord);
                break;
            case rook:
                validPieceMove = __CheckValidMove_Rook__(mainBoard, *fromCoordsX, *fromCoordsY, *toCoordsX, *toCoordsY, XBoardCoord, YBoardCoord);
                break;
            case queen:
                validPieceMove = __CheckValidMove_Queen__(mainBoard, *fromCoordsX, *fromCoordsY, *toCoordsX, *toCoordsY, XBoardCoord, YBoardCoord);
                break;
            case king:
                validPieceMove = __CheckValidMove_King__(*fromCoordsX, *fromCoordsY, *toCoordsX, *toCoordsY);
                break;
            default:    
                printf("Unimplemented (Board_Turn_Read)\n");
                validPieceMove = INVALID_MOVE;
        }
    }while(validPieceMove != VALID_MOVE);
}

void Board_Turn_Counter(u16* restrict turnCount){
    printf("Turn %u\n", *turnCount);
    *turnCount = (*turnCount) + 1;
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