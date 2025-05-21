#ifndef __TYPEDEFS_H__
#define __TYPEDEFS_H__

#include "raylib.h"

// basic types

#ifndef i8
    typedef signed char i8;
#endif

#ifndef u8
    typedef char u8;
#endif

#ifndef i16
    typedef short i16;
#endif

#ifndef u16
    typedef unsigned short u16;
#endif

#ifndef i32
    typedef int i32;
#endif

#ifndef u32
    typedef unsigned int u32;
#endif

#ifndef i64
    typedef long long i64;
#endif

#ifndef u64
    typedef unsigned long long u64;
#endif

#ifndef f32
    typedef float f32;
#endif

#ifndef f64
    typedef double f64;
#endif

#ifndef true
    #define true 1
#endif

#ifndef false
    #define false 0
#endif

#ifndef ____cplusplus
    #ifndef bool
        typedef _Bool bool;
    #endif
#endif

#ifndef NULL
    #define NULL ((void*)0)
#endif

// GCC attributes

#ifdef __GNUC__
    #ifndef DEPRECATED
        #define DEPRECATED __attribute__((deprecated))
    #endif

    #ifndef NONNULL 
        #define NONNULL __attribute__((nonnull))
    #endif
#endif

// to fix specific errors/warnings

#ifndef u8_EXPLICIT
    typedef unsigned char u8_EXPLICIT;
#endif

// useful struct for reducing the code size

typedef struct u8Vect2 {
    u8 x;
    u8 y;
} u8Vect2;

typedef struct i8Vect2 {
    i8 x;
    i8 y;
} i8Vect2;

typedef struct u16Vect2 {
    u16 x;
    u16 y;
} u16Vect2;

typedef struct i16Vect2 {
    i16 x;
    i16 y;
} i16Vect2;

typedef struct u32Vect2 {
    u32 x;
    u32 y;
} u32Vect2;

typedef struct i32Vect2 {
    i32 x;
    i32 y;
} i32Vect2;

typedef struct u64Vect2 {
    u64 x;
    u64 y;
} u64Vect2;

typedef struct i64Vect2 {
    i64 x;
    i64 y;
} i64Vect2;

// raylib struct
typedef Vector2 f32Vect2;

typedef struct f64Vect2 {
    f64 x;
    f64 y;
} f64Vect2;

#endif // end of __TYPEDEFS_H__