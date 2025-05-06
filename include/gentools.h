// General utility functions go here

#ifndef __GENTOOLS_H__
#define __GENTOOLS_H__

#include "../include/typedefs.h"
#include "../include/gameTexts.h"

#define EXIT_ON_FATAL_ERROR 1
#define EXIT_ON_NONFATAL_ERROR 1
// #define EXIT_ON_WARNING 1

// used for the return of CheckBounds
#define OUT_OF_BOUNDS -1
#define IN_BOUNDS 0

// for platform specific code
#if defined _WIN32 || defined _WIN64 || defined __CYGWIN__
    #define OS_WINDOWS
#elif defined unix || defined __unix || defined __unix__ || defined __linux__
    #define OS_UNIX
#else
    #error "Unsupported platform\n"
    #pragma "The current supported platforms are Windows and UNIX/Linux\n"
#endif

// used for clearing the screen; platform independent
void Sys_Clear(void);

// used to send a warning; the warning will be sent to stderr
void SendWarning(const u8* restrict warningMsg);

// used to check if a critical operation (such as an allocation or a file operation)
// failed; the errorText will be sent to stderr
// the pointer needs to be casted to check if it is NULL
void CheckCriticalOperation(void* restrict general_ptr, const u8* restrict errorMsg); 

// used to put the system on wait for the desired amount of seconds
void Sys_Sleep(const u32 seconds);

// used to check if there current X and Y are outside the board bounds; returns OUT_OF_BOUNDS if the coordinates are outside the bounds, IN_BOUNDS otherwise
i8 CheckBounds(const u16 currentX, const u16 currentY, const u16 maxX, const u16 maxY, const bool supressWarnings);

#endif  // end of __GENTOOLS_H__