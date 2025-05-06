#ifndef TYPEDEFS_H
#define TYPRDEFS_H

typedef signed char i8;
typedef char u8;
typedef short i16;
typedef unsigned short u16;
typedef int i32;
typedef unsigned int u32;
typedef long long i64;
typedef unsigned long long u64;
typedef float f32;
typedef double f64;

#ifndef true
    #define true 1
#endif

#ifndef false
    #define false 0
#endif

typedef _Bool bool;

#ifndef NULL
    #define NULL ((void*)0)
#endif

// GCC specific attributes (currently unimplemented)
#ifdef __GNUC__
    // #define __attribute_const__ CONST_FUNC
    // #define __attribute_pure__ PURE_FUNC
#endif

#endif // end of TYPEDEFS_H