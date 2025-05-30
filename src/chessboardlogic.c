// This is where functions related to the backend logic of the game board
// should go

#include "../include/chessboardlogic.h"
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

playerPiece_t** Board_Init(const u16Vect2 boardSize) {
    playerPiece_t** mainBoard = NULL;

    mainBoard = (playerPiece_t**)malloc(sizeof(playerPiece_t*) * boardSize.y);
    if(mainBoard == NULL){
        #ifdef EXIT_ON_FATAL_ERROR
        fprintf(stderr, "Fatal Error: Main board allocation failed (Board_Init)\n");
        exit(EXIT_FAILURE);
        #endif
    }else{
        for(u16 allocIndex = 0; allocIndex < boardSize.y; ++allocIndex){
            mainBoard[allocIndex] = (playerPiece_t*)malloc(sizeof(playerPiece_t) * boardSize.x);
            Exception_CheckNULLPtr((playerPiece_t*)mainBoard[allocIndex], "Fatal Error: board tiles allocation failed (Board_Init)\n");
        }
    }

    for(u16 Yindex = 0; Yindex < boardSize.y; ++Yindex){
        for(u16 Xindex = 0; Xindex < boardSize.x; ++Xindex){
            mainBoard[Yindex][Xindex].pieceColor = color_NONE;
            mainBoard[Yindex][Xindex].pieceType = piece_NONE;
            mainBoard[Yindex][Xindex].usedSpecialMove = false;
        }
    }

    return mainBoard;
}

NONNULL void Board_Settings_LoadGeneral(const u8* restrict ini_filename, u16Vect2* restrict boardCoord, i32* restrict fpsMax_ptr, i32Vect2* restrict screenSize_ptr, bool* showCoordinates_ptr) {
    FILE* ini_ptr = NULL;
    u8 ini_filecontent[__CONFIG_INI_MAX_SIZE__] = {0};
    
    ini_ptr = fopen(ini_filename, "r");
    Exception_CheckNULLPtr((FILE*)ini_ptr, "Fatal Error: Config Ini could not be opened! (Board_Settings_General)\n");

    // check the ini file format
    // make an algorithm to fallback on default values if the file is not correct
    if(feof(ini_ptr) == 0 && fgets(ini_filecontent, __CONFIG_INI_MAX_SIZE__, ini_ptr) != NULL){
        if(strcmp(ini_filecontent, "[general]\n") != 0){  // fgets also gets the newline terminator \n
            Exception_SendWarning("Warning: Incompatible Ini file content (Board_Settings_General)\n");
        }else{
            if(feof(ini_ptr) == 0 && fgets(ini_filecontent, __CONFIG_INI_MAX_SIZE__, ini_ptr) != NULL){
                sscanf(ini_filecontent, "table_size = %hux%hu", &boardCoord->x, &boardCoord->y);
            }else{
                Exception_SendWarning("Warning: Incompatible Ini file content (Board_Settings_General)\n");
            }
            if(feof(ini_ptr) == 0 && fgets(ini_filecontent, __CONFIG_INI_MAX_SIZE__, ini_ptr) != NULL){
                sscanf(ini_filecontent, "maxFps = %d", fpsMax_ptr);
            }else{
                Exception_SendWarning("Warning: Incompatible Ini file content (Board_Settings_General)\n");
            }
            if(feof(ini_ptr) == 0 && fgets(ini_filecontent, __CONFIG_INI_MAX_SIZE__, ini_ptr) != NULL){
                sscanf(ini_filecontent, "screenWidth = %d", &screenSize_ptr->x);
            }else{
                Exception_SendWarning("Warning: Incompatible Ini file content (Board_Settings_General)\n");
            }
            if(feof(ini_ptr) == 0 && fgets(ini_filecontent, __CONFIG_INI_MAX_SIZE__, ini_ptr) != NULL){
                sscanf(ini_filecontent, "screenHeight = %d", &screenSize_ptr->y);
            }else{
                Exception_SendWarning("Warning: Incompatible Ini file content (Board_Settings_General)\n");
            }
            if(feof(ini_ptr) == 0 && fgets(ini_filecontent, __CONFIG_INI_MAX_SIZE__, ini_ptr) != NULL){
                sscanf(ini_filecontent, "showCoordinates = %hhu", (u8_EXPLICIT*)showCoordinates_ptr);  // had to use the special type to get rid of the warning 
            }else{
                Exception_SendWarning("Warning: Incompatible Ini file content (Board_Settings_General)\n");
            }
        }
    }else{
        Exception_SendWarning("Warning: Incompatible Ini file format (Board_Settings_General)\n");
    }

    fclose(ini_ptr);
}
/*
NONNULL void Board_LoadPlacement(const u8* restrict ini_filename, playerPiece_t*** main_board_ptr, const u16Vect2 boardCoord) {
    FILE* ini_ptr = NULL;
    playerPiece_t piece_Placement_Buffer = {piece_NONE, color_NONE, false};
    u16Vect2 boardPlacementBuffer = {0};
    u8 ini_filecontent[__CONFIG_INI_MAX_SIZE__] = {0};
    u16 turnCounter = 0;
    
    ini_ptr = fopen(ini_filename, "r");
    Exception_CheckNULLPtr((FILE*)ini_ptr, "Fatal Error: Config Ini could not be opened! (Board_Placement)\n");
    
    if(feof(ini_ptr) == 0 && fgets(ini_filecontent, __CONFIG_INI_MAX_SIZE__, ini_ptr) != NULL){
        if(strcmp(ini_filecontent, "[layout]\n") != 0){  // fgets also gets the newline terminator \n
            Exception_SendWarning("Warning: Incompatible Ini file content (Board_Placement)\n");
        } else {  
            while(feof(ini_ptr) == 0 && fgets(ini_filecontent, __CONFIG_INI_MAX_SIZE__, ini_ptr) != NULL){
                sscanf(ini_filecontent, "piece = %c[%hu][%hu], color = %c", (u8*)&piece_Placement_Buffer.pieceType, &boardPlacementBuffer.y, &boardPlacementBuffer.x, (u8*)&piece_Placement_Buffer.pieceColor);
                if(Exception_CheckBounds(boardPlacementBuffer.x, boardPlacementBuffer.y, boardCoord.x, boardCoord.y, false) == IN_BOUNDS){
                    (*main_board_ptr)[boardPlacementBuffer.y][boardPlacementBuffer.x].pieceColor = piece_Placement_Buffer.pieceColor;
                    (*main_board_ptr)[boardPlacementBuffer.y][boardPlacementBuffer.x].pieceType = piece_Placement_Buffer.pieceType;
                }
            }
        }
    }

    fclose(ini_ptr);
}
*/

NONNULL void Board_LoadPlacement(const u8* restrict ini_filename, playerPiece_t*** main_board_ptr, const u16Vect2 boardCoord, u16* restrict turnCounter_ptr, __color_t* restrict colorCounter_ptr, u16* restrict moveCounter_ptr) {
    FILE* ini_ptr = NULL;
    playerPiece_t piece_Placement_Buffer = {piece_NONE, color_NONE};
    u16Vect2 boardPlacementBuffer = {0};
    u8 ini_filecontent[__CONFIG_INI_MAX_SIZE__] = {0};

    bool inLayoutSection = false;
    bool inTurnsSection = false;

    ini_ptr = fopen(ini_filename, "r");
    Exception_CheckNULLPtr((FILE*)ini_ptr, "Fatal Error: Config Ini could not be opened! (Board_Placement)\n");

    while (fgets((char*)ini_filecontent, __CONFIG_INI_MAX_SIZE__, ini_ptr) != NULL) {
        if (strcmp((char*)ini_filecontent, "[layout]\n") == 0) {
            inLayoutSection = true;
            inTurnsSection = false;
            continue;
        } else if (strcmp((char*)ini_filecontent, "[turns]\n") == 0) {
            inLayoutSection = false;
            inTurnsSection = true;
            continue;
        }

        if (inLayoutSection) {
            if (sscanf((char*)ini_filecontent, "piece = %c[%hu][%hu], color = %c",
                       (u8*)&piece_Placement_Buffer.pieceType,
                       &boardPlacementBuffer.y,
                       &boardPlacementBuffer.x,
                       (u8*)&piece_Placement_Buffer.pieceColor) == 4)
            {
                if (Exception_CheckBounds(boardPlacementBuffer.x, boardPlacementBuffer.y, boardCoord.x, boardCoord.y, false) == IN_BOUNDS) {
                    (*main_board_ptr)[boardPlacementBuffer.y][boardPlacementBuffer.x].pieceColor = piece_Placement_Buffer.pieceColor;
                    (*main_board_ptr)[boardPlacementBuffer.y][boardPlacementBuffer.x].pieceType  = piece_Placement_Buffer.pieceType;
                }
            }
        }

        if (inTurnsSection) {
            u16 tmpTurn = 0;
            u16 tmpMove = 0;
            char tmpColor = 0;
            if (sscanf((char*)ini_filecontent, "turn = %hu", &tmpTurn) == 1) {
                *turnCounter_ptr = tmpTurn;
            }
            else if (sscanf((char*)ini_filecontent, "player = %c", &tmpColor) == 1) {
                if (tmpColor == 'w') {
                    *colorCounter_ptr = white;
                } else if (tmpColor == 'b') {
                    *colorCounter_ptr = black;
                } else {
                    *colorCounter_ptr = color_NONE;
                    Exception_SendWarning("Warning: Unknown color character in save file.\n");
                }
            }
            else if (sscanf((char*)ini_filecontent, "move = %hu", &tmpMove) == 1) {
                *moveCounter_ptr = tmpMove;
            }
        }
    }

    fclose(ini_ptr);
}

NONNULL void Board_Free(playerPiece_t*** main_board_ptr, const u16 YBoardCoord){
    for(u16 freeIndex = 0; freeIndex < YBoardCoord; ++freeIndex){
        free((*main_board_ptr)[freeIndex]);
    }
    free((*main_board_ptr));
}

NONNULL void Board_Turn_NextColor(__color_t* restrict color_ptr){
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

NONNULL void Board_Save_SaveGame(playerPiece_t** restrict mainBoard, const u16Vect2 boardSize, u16 turnCounter, __color_t currentCol, u16 moveCounter, u8* restrict filename) {
    FILE* saveFile_ptr = NULL;

    saveFile_ptr = fopen(filename, "w");
    Exception_CheckNULLPtr((FILE*)saveFile_ptr, "Fatal Error: Game save failed (Board_Save_SaveGame)\n");

    fprintf(saveFile_ptr, "[layout]\n");

    for(u16 boardIndexY = 0; boardIndexY < boardSize.y; ++boardIndexY){
        for(u16 boardIndexX = 0; boardIndexX < boardSize.x; ++boardIndexX){
            if(mainBoard[boardIndexY][boardIndexX].pieceColor != color_NONE){
                fprintf(saveFile_ptr, "piece = %c[%hu][%hu], color = %c\n", mainBoard[boardIndexY][boardIndexX].pieceType, boardIndexY, boardIndexX, mainBoard[boardIndexY][boardIndexX].pieceColor);
            }
        }
    }

    fprintf(saveFile_ptr, "\n[turns]\n");
    fprintf(saveFile_ptr, "turn = %hu\n", turnCounter);
    fprintf(saveFile_ptr, "player = %c\n", currentCol);
    fprintf(saveFile_ptr, "move = %hu", moveCounter);
    
    fclose(saveFile_ptr);
}

NONNULL void Read_PGN_Header(PGN_Data* restrict PGNData, u8* restrict filename) {
    FILE* readFile_ptr = NULL;

    readFile_ptr = fopen(filename, "r");
    
    Exception_CheckNULLPtr((FILE*)readFile_ptr, "Fatal Error: Read PGN metadata from file failed (Read_PGN_Header)\n");

    // Populate the header
    char line[MAX_STR];
    char value[MAX_STR];
    char key[32];

    while (fgets(line, sizeof(line), readFile_ptr)) {
        if (sscanf(line, "[%31[^ ] \"%199[^\"]", key, value) == 2) {
            if (strcmp(key, "Event") == 0) {
                strncpy(PGNData->event, value, sizeof(PGNData->event) - 1);
                PGNData->event[sizeof(PGNData->event) - 1] = '\0';
            } 
            else if (strcmp(key, "Site") == 0) {
                strncpy(PGNData->site, value, sizeof(PGNData->site) - 1);
                PGNData->site[sizeof(PGNData->site)- 1] = '\0';
            } 
            else if (strcmp(key, "Date") == 0) {
                strncpy(PGNData->date, value, sizeof(PGNData->date) - 1);
                PGNData->date[sizeof(PGNData->date) - 1] = '\0';
            } 
            else if (strcmp(key, "Round") == 0) {
                strncpy(PGNData->round, value, sizeof(PGNData->round) - 1);
                PGNData->round[sizeof(PGNData->round) - 1] = '\0';

            } 
            else if (strcmp(key, "White") == 0) {
                strncpy(PGNData->white, value, sizeof(PGNData->white) - 1);
                PGNData->white[sizeof(PGNData->white) - 1] = '\0';

            } 
            else if (strcmp(key, "Black") == 0) {
                strncpy(PGNData->black, value, sizeof(PGNData->black) - 1);
                PGNData->black[sizeof(PGNData->black) - 1] = '\0';

            } 
            else if (strcmp(key, "Result") == 0) {
                strncpy(PGNData->result, value, sizeof(PGNData->result) - 1);
                PGNData->result[sizeof(PGNData->result) - 1] = '\0';
            }
        }
    }
    
    fclose(readFile_ptr);
}

NONNULL void Board_Save_PGN_Header(PGN_Data* restrict PGNData, u8* restrict filename) {
    FILE* saveFile_ptr = NULL;

    saveFile_ptr = fopen(filename, "w");

    Exception_CheckNULLPtr(saveFile_ptr, "Fatal Error: Game save failed (Board_Save_PGN_Header)\n");

    fprintf(saveFile_ptr, "[Event \"%s\"]\n", PGNData->event);
    fprintf(saveFile_ptr, "[Site \"%s\"]\n", PGNData->site);
    fprintf(saveFile_ptr, "[Date \"%s\"]\n", PGNData->date);
    fprintf(saveFile_ptr, "[Round \"%s\"]\n", PGNData->round);
    fprintf(saveFile_ptr, "[White \"%s\"]\n", PGNData->white);
    fprintf(saveFile_ptr, "[Black \"%s\"]\n", PGNData->black);
    fprintf(saveFile_ptr, "[Result \"%s\"]\n\n", PGNData->result);

    fclose(saveFile_ptr);
}

NONNULL void Board_Save_Moves_To_PGN(PGN_Data* restrict PGNData, u16 moveCounter, u8* restrict filename) {
    FILE* saveFile_ptr = NULL;

    saveFile_ptr = fopen(filename, "a");
    
    Exception_CheckNULLPtr((FILE*)saveFile_ptr, "Fatal Error: Game save failed (Board_Save_SaveGame)\n");
    
    // Append the moves 
    if(moveCounter % 2 == 0){
        fprintf(saveFile_ptr, "\n%d. ", moveCounter / 2 + 1); // Standard PGN format: white gets the move number
    }
    fprintf(saveFile_ptr, "%s ", PGNData->moves[moveCounter]);
    
    
    fclose(saveFile_ptr);
}

NONNULL void FormatMoveString(u8* outStr, u16Vect2 from, u16Vect2 to, playerPiece_t** mainBoard) {
    __chesspiece_t piece = mainBoard[from.y][from.x].pieceType;
    __color_t color = mainBoard[from.y][from.x].pieceColor;

    char pieceChar = '\0';
    
    if (piece != pawn && piece != piece_NONE) {
        pieceChar = (color == white) ? toupper((char)piece) : (char)piece;
    }

    char fromFile = 'a' + from.x;
    char fromRank = '1' + from.y;
    char toFile   = 'a' + to.x;
    char toRank   = '1' + to.y;

    if (pieceChar != '\0') {
        // For non-pawn pieces: Nf3g5 format
        snprintf(outStr, 8, "%c%c%c%c%c", pieceChar, fromFile, fromRank, toFile, toRank);
    } else {
        // For pawns: e2e4 format
        snprintf(outStr, 8, "%c%c%c%c", fromFile, fromRank, toFile, toRank);
    }
}

// quick and dirty implementation that probably needs to be fixed later
NONNULL bool Board_Save_LoadGame(u8* restrict loadPrompt){
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

bool Board_IsInsideBoard(const f32Vect2 mousePos, const u16Vect2 boardSize) {
    if((mousePos.x < 0 || mousePos.x >= boardSize.x) || (mousePos.y < 0 || mousePos.y >= boardSize.y)) {
        return false;
    } else {
        return true;
    }
}
  
NONNULL bool Board_GetFrom(playerPiece_t** restrict mainBoard, const u16Vect2 boardSize, u16Vect2* restrict from_ptr, const __color_t currentPlayerCol, bool* restrict highlightSelection, const f32Vect2 offset) {
    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT) != false) {
        f32Vect2 mousePos = Graphics_MapMouseToBoard(GetMousePosition(), boardSize.y, offset);
        if(Board_IsInsideBoard(mousePos, boardSize) != false) {
            from_ptr->x = (u16)mousePos.x;
            from_ptr->y = (u16)mousePos.y;
            if(mainBoard[from_ptr->y][from_ptr->x].pieceColor == currentPlayerCol) {
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

NONNULL bool Board_GetTo(playerPiece_t** restrict mainBoard, const u16Vect2 boardSize, const u16Vect2 from, u16Vect2* restrict to_ptr, const f32Vect2 offset, bool* restrict doCastling) {
    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT) != false) {
        f32Vect2 mousePos = Graphics_MapMouseToBoard(GetMousePosition(), boardSize.y, offset);
        bool validPieceMove = false;
        if(Board_IsInsideBoard(mousePos, boardSize) != false) {
            to_ptr->x = (u16)mousePos.x;
            to_ptr->y = (u16)mousePos.y;

            if(Piece_CheckCastling(mainBoard, from, *to_ptr) == false) {
                *doCastling = false;
                if(Piece_CheckOwnerDestination(mainBoard, from, *to_ptr) != false) {
                    switch(mainBoard[from.y][from.x].pieceType) {
                        case pawn:
                            validPieceMove = __CheckValidMove_Pawn__(mainBoard, from, *to_ptr);
                            break;
                        case knight:
                            validPieceMove = __CheckValidMove_Knight__(from, *to_ptr, boardSize);
                            break;
                        case bishop:
                            validPieceMove = __CheckValidMove_Bishop__(mainBoard, from, *to_ptr, boardSize);
                            break;
                        case rook:
                            validPieceMove = __CheckValidMove_Rook__(mainBoard, from, *to_ptr, boardSize);
                            break;
                        case queen:
                            validPieceMove = __CheckValidMove_Queen__(mainBoard, from, *to_ptr, boardSize);
                            break;
                        case king:
                            validPieceMove = __CheckValidMove_King__(from, *to_ptr);
                            break;
                        default:    
                            printf("Unimplemented (Board_Turn_Read)\n");
                            validPieceMove = false;
                    }
                    return validPieceMove;
                }
            } else {
                *doCastling = true;
                return true;
            }

            
        }
    }

    return false;
}

bool Board_UndoSelection(const u16Vect2 boardSize, const f32Vect2 offset) {
    if(IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) != false) {
        f32Vect2 mousePos = Graphics_MapMouseToBoard(GetMousePosition(), boardSize.y, offset);
        if(Board_IsInsideBoard(mousePos, boardSize) != false) {
            return true;
        } else {
            return false;
        }
    } else {
        return false;
    }
}