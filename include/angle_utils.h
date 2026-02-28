#ifndef _ANGLE_UTILS_H_
#define _ANGLE_UTILS_H_

#include "global.h"

#define VAR_ADD(x, y)  ((x) += (y))
#define VAR_SUB(x, y)  ((x) -= (y))
#define VAR_MULT(x, y) ((x) *= (y))

#define VAR_ADD_CAST(x, y, t)  ((x) += (t)(y))
#define VAR_SUB_CAST(x, y, t)  ((x) -= (t)(y))
#define VAR_MULT_CAST(x, y, t) ((x) *= (t)(y))

#define S8_ADD(x, y)  VAR_ADD_CAST(x, y, s8)
#define U8_ADD(x, y)  VAR_ADD_CAST(x, y, u8)
#define S16_ADD(x, y) VAR_ADD_CAST(x, y, s16)
#define U16_ADD(x, y) VAR_ADD_CAST(x, y, u16)
#define S32_ADD(x, y) VAR_ADD_CAST(x, y, s32)
#define U32_ADD(x, y) VAR_ADD_CAST(x, y, u32)

#define S8_SUB(x, y)  VAR_SUB_CAST(x, y, s8)
#define U8_SUB(x, y)  VAR_SUB_CAST(x, y, u8)
#define S16_SUB(x, y) VAR_SUB_CAST(x, y, s16)
#define U16_SUB(x, y) VAR_SUB_CAST(x, y, u16)
#define S32_SUB(x, y) VAR_SUB_CAST(x, y, s32)
#define U32_SUB(x, y) VAR_SUB_CAST(x, y, u32)

#define S8_MULT(x, y)  VAR_MULT_CAST(x, y, s8)
#define U8_MULT(x, y)  VAR_MULT_CAST(x, y, u8)
#define S16_MULT(x, y) VAR_MULT_CAST(x, y, s16)
#define U16_MULT(x, y) VAR_MULT_CAST(x, y, u16)
#define S32_MULT(x, y) VAR_MULT_CAST(x, y, s32)
#define U32_MULT(x, y) VAR_MULT_CAST(x, y, u32)

#define ANGLE_ADD  S16_ADD
#define ANGLE_SUB  S16_SUB
#define ANGLE_MULT S16_MULT

// There are some angles that weren't sign-extended until the shield version
#if !PLATFORM_SHIELD
    #define S8_ADD_2   VAR_ADD
    #define U8_ADD_2   VAR_ADD
    #define S16_ADD_2  VAR_ADD
    #define U16_ADD_2  VAR_ADD
    #define S32_ADD_2  VAR_ADD
    #define U32_ADD_2  VAR_ADD
    
    #define S8_SUB_2   VAR_SUB
    #define U8_SUB_2   VAR_SUB
    #define S16_SUB_2  VAR_SUB
    #define U16_SUB_2  VAR_SUB
    #define S32_SUB_2  VAR_SUB
    #define U32_SUB_2  VAR_SUB

    #define S8_MULT_2  VAR_MULT
    #define U8_MULT_2  VAR_MULT
    #define S16_MULT_2 VAR_MULT
    #define U16_MULT_2 VAR_MULT
    #define S32_MULT_2 VAR_MULT
    #define U32_MULT_2 VAR_MULT
#else
    #define S8_ADD_2   S8_ADD
    #define U8_ADD_2   U8_ADD
    #define S16_ADD_2  S16_ADD
    #define U16_ADD_2  U16_ADD
    #define S32_ADD_2  S32_ADD
    #define U32_ADD_2  U32_ADD

    #define S8_SUB_2   S8_SUB
    #define U8_SUB_2   U8_SUB
    #define S16_SUB_2  S16_SUB
    #define U16_SUB_2  U16_SUB
    #define S32_SUB_2  S32_SUB
    #define U32_SUB_2  U32_SUB

    #define S8_MULT_2  S8_MULT
    #define U8_MULT_2  U8_MULT
    #define S16_MULT_2 S16_MULT
    #define U16_MULT_2 U16_MULT
    #define S32_MULT_2 S32_MULT
    #define U32_MULT_2 U32_MULT
#endif

#define ANGLE_ADD_2  S16_ADD_2
#define ANGLE_SUB_2  S16_SUB_2
#define ANGLE_MULT_2 S16_MULT_2

#endif // !_ANGLE_UTILS_H_
