// General utility functions go here

#ifndef __GENTOOLS_H__
#define __GENTOOLS_H__

#include "../include/typedefs.h"
#include "../include/gameTexts.h"
#include "../include/graphics.h"

/** * @brief #### Exit on fatal errors if the macro is defined */
#define EXIT_ON_FATAL_ERROR 1
/** * @brief #### Exit on warnings errors if the macro is defined */
// #define EXIT_ON_WARNING 1

// used for the return of CheckBounds
#define OUT_OF_BOUNDS -1
#define IN_BOUNDS 0
#define BOUNDS_PASS_VAR_CHECK 0
#define BOUNDS_PASS_MAX_CHECK __UINT16_MAX__ 

// for platform specific code
#if defined _WIN32 || defined _WIN64 || defined __CYGWIN__
    #define OS_WINDOWS
#elif defined unix || defined __unix || defined __unix__ || defined __linux__
    #define OS_UNIX
#else
    #error "Unsupported platform\n"
    #pragma "The current supported platforms are Windows and UNIX/Linux\n"
#endif

// [Exception handling tools]

/**
 * @brief #### Send a warning
 * @brief Sends a message to stderr and if EXIT_ON_WARNING is defined, the program will exit.
 * @param warningMsg The message send to stderr.
 */
void Exception_SendWarning(const u8* restrict warningMsg);

/**
 * @brief #### Check if a pointer is NULL
 * @brief Checks if the pointer is NULL and sends errorMsg to stderr if true and if EXIT_ON_FATAL_ERROR is defined, the program will exit.
 * @note This function can throw exceptions
 * @param general_ptr The pointer to be checked; must be type casted.
 * @param errorMsg The message sent to stderr if the pointer is NULL.
 */
void Exception_CheckNULLPtr(void* restrict general_ptr, const u8* restrict errorMsg); 

/**
 * @brief #### Check if a piece is within the limits of a board
 * @brief Checks if the coordinates of the pieces are not higher than the those of the board.
 * @note This function can throw exceptions
 * @param currentX The X coordinates (number of lines) of the piece.
 * @param currentY The Y coordinates (number of columns) of the piece.
 * @param maxX The X coordinates (number of lines) of the board.
 * @param maxY The Y coordinates (number of lines) of the board.
 * @param suppressWarnings If set to false, no exceptions will be thrown
 * @return OUT_OF_BOUNDS if the coordinates are outside the bounds or IN_BOUNDS if the coordinates are inside the bounds.
 */
i8 Exception_CheckBounds(const u16 currentX, const u16 currentY, const u16 maxX, const u16 maxY, const bool suppressWarnings);

/**
 * @brief #### Check if the texture is loaded
 * @brief Checks if the texure was correctly loaded from the file.
 * @note This function can throw exceptions
 * @param inptexture The texture to check.
 */
void Exception_CheckLoadedTexture(pieceTextures_t inptexture);

#endif  // end of __GENTOOLS_H__