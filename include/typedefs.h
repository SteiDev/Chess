#ifndef __TYPEDEFS_H__
#define __TYPEDEFS_H__

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
#endif

#endif // end of __TYPEDEFS_H__