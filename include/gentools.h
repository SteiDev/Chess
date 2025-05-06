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
#define BOUNDS_PASS_VAR_CHECK 0
#define BOUNDS_PASS_MAX_CHECK __UINT16_MAX__ 

// for platform specific code
#if defined _WIN32 || defined _WIN64 || defined __CYGWIN__
    #define OS_WINDOWS
#elif defined unix || defined __unix || defined __unix__ || defined __linux__
    #define OS_UNIX
#else
    #error "Unsupported platform\n"
    #pragma "The current support    ed platforms are Windows and UNIX/Linux\n"
#endif

// [Exception handling tools]

// used to send a warning; the warning will be sent to stderr
void Exception_SendWarning(const u8* restrict warningMsg);

// used to check if a critical operation (such as an allocation or a file operation)
// failed; the errorText will be sent to stderr
// the pointer needs to be casted to check if it is NULL
void Exception_CheckCriticalOperation(void* restrict general_ptr, const u8* restrict errorMsg); 

// used to check if there current X and Y are outside the board bounds; returns OUT_OF_BOUNDS if the coordinates are outside the bounds, IN_BOUNDS otherwise; BOUNDS_PASS_VAR_CHECK and BOUNDS_PASS_MAX_CHECK may also be used to skip checks
i8 Exception_CheckBounds(const u16 currentX, const u16 currentY, const u16 maxX, const u16 maxY, const bool supressWarnings);


// [Platform independent system functions]

// used for clearing the screen; platform independent
void Sys_Clear(void);

// used to put the system on wait for the desired amount of seconds
void Sys_Sleep(const u32 seconds);


#endif  // end of __GENTOOLS_H__