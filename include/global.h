#ifndef _global_h_
#define _global_h_

// Version ordering defined in configure.py
#define VERSION_GCN_USA          0
#define VERSION_GCN_PAL          1
#define VERSION_GCN_JPN          2
#define VERSION_WII_USA_R0       3
#define VERSION_WII_USA_R2       4
#define VERSION_WII_PAL          5
#define VERSION_WII_JPN          6
#define VERSION_WII_KOR          7
#define VERSION_WII_USA_KIOSK    8
#define VERSION_WII_PAL_KIOSK    9
#define VERSION_SHIELD           10
#define VERSION_SHIELD_PROD      11
#define VERSION_SHIELD_DEBUG     12

#define PLATFORM_GCN    (VERSION >= VERSION_GCN_USA && VERSION <= VERSION_GCN_JPN)
#define PLATFORM_WII    (VERSION >= VERSION_WII_USA_R0 && VERSION <= VERSION_WII_PAL_KIOSK)
#define PLATFORM_SHIELD (VERSION >= VERSION_SHIELD && VERSION <= VERSION_SHIELD_DEBUG)

#define REGION_USA (VERSION == VERSION_GCN_USA || VERSION == VERSION_WII_USA_R0 || VERSION == VERSION_WII_USA_R2 || VERSION == VERSION_WII_USA_KIOSK)
#define REGION_PAL (VERSION == VERSION_GCN_PAL || VERSION == VERSION_WII_PAL || VERSION == VERSION_WII_PAL_KIOSK)
#define REGION_JPN (VERSION == VERSION_GCN_JPN || VERSION == VERSION_WII_JPN)
#define REGION_KOR (VERSION == VERSION_WII_KOR)
#define REGION_CHN (VERSION == VERSION_SHIELD || VERSION == VERSION_SHIELD_PROD || VERSION == VERSION_SHIELD_DEBUG)

// define DEBUG if it isn't already so it can be used in conditions
#ifndef DEBUG
#define DEBUG 0
#endif

#define MSL_INLINE inline

#define ARRAY_SIZE(o) (s32)(sizeof(o) / sizeof(o[0]))
#define ARRAY_SIZEU(o) (sizeof(o) / sizeof(o[0]))

// Align X to the previous N bytes (N must be power of two)
#define ALIGN_PREV(X, N) ((X) & ~((N)-1))
// Align X to the next N bytes (N must be power of two)
#define ALIGN_NEXT(X, N) ALIGN_PREV(((X) + (N)-1), N)
#define IS_ALIGNED(X, N) (((X) & ((N)-1)) == 0)
#define IS_NOT_ALIGNED(X, N) (((X) & ((N)-1)) != 0)

#define ROUND(n, a) (((u32)(n) + (a)-1) & ~((a)-1))
#define TRUNC(n, a) (((u32)(n)) & ~((a)-1))

// Silence unused parameter warnings.
// Necessary for debug matches.
#define UNUSED(x) ((void)(x))

#ifdef __MWERKS__
#ifndef decltype
#define decltype __decltype__
#endif
#endif

#define _SDA_BASE_(dummy) 0
#define _SDA2_BASE_(dummy) 0

#ifdef __MWERKS__
#define GLUE(a, b) a##b
#define GLUE2(a, b) GLUE(a, b)

#if VERSION == VERSION_GCN_USA
#define STATIC_ASSERT(cond) typedef char GLUE2(static_assertion_failed, __LINE__)[(cond) ? 1 : -1]
#else
#define STATIC_ASSERT(...)
#endif
#else
#define STATIC_ASSERT(...)
#endif

#ifndef __MWERKS__
// Silence clangd errors about MWCC PPC intrinsics by declaring them here.
extern int __cntlzw(unsigned int);
extern int __rlwimi(int, int, int, int, int);
extern void __dcbf(void*, int);
extern void __dcbz(void*, int);
extern void __sync();
extern int __abs(int);
void* __memcpy(void*, const void*, int);
#endif

#define FAST_DIV(x, n) (x >> (n / 2))

#define SQUARE(x) ((x) * (x))

#define POINTER_ADD_TYPE(type_, ptr_, offset_) ((type_)((unsigned long)(ptr_) + (unsigned long)(offset_)))
#define POINTER_ADD(ptr_, offset_) POINTER_ADD_TYPE(__typeof__(ptr_), ptr_, offset_)

// floating-point constants
static const float INF = 2000000000.0f;

// hack to make strings with no references compile properly
#define DEAD_STRING(s) OSReport(s)

#define READU32_BE(ptr, offset) \
    (((u32)ptr[offset] << 24) | ((u32)ptr[offset + 1] << 16) | ((u32)ptr[offset + 2] << 8) | (u32)ptr[offset + 3]);

#ifndef NO_INLINE
#ifdef __MWERKS__
#define NO_INLINE __attribute__((never_inline))
#else
#define NO_INLINE
#endif
#endif
    
// Hack to trick the compiler into not inlining functions that use this macro.
#define FORCE_DONT_INLINE \
    (void*)0; (void*)0; (void*)0; (void*)0; (void*)0; (void*)0; (void*)0; (void*)0; (void*)0; (void*)0; \
    (void*)0; (void*)0; (void*)0; (void*)0; (void*)0; (void*)0; (void*)0; (void*)0; (void*)0; (void*)0; \
    (void*)0; (void*)0; (void*)0; (void*)0; (void*)0; (void*)0; (void*)0; (void*)0; (void*)0; (void*)0; \
    (void*)0; (void*)0; (void*)0; (void*)0; (void*)0; (void*)0; (void*)0; (void*)0; (void*)0; (void*)0; \
    (void*)0; (void*)0; (void*)0; (void*)0; (void*)0; (void*)0; (void*)0; (void*)0; (void*)0; (void*)0; \
    (void*)0; (void*)0; (void*)0; (void*)0; (void*)0; (void*)0; (void*)0; (void*)0; (void*)0; (void*)0; \
    (void*)0; (void*)0; (void*)0; (void*)0; (void*)0; (void*)0; (void*)0; (void*)0; (void*)0; (void*)0; \
    (void*)0; (void*)0; (void*)0; (void*)0; (void*)0; (void*)0; (void*)0; (void*)0; (void*)0; (void*)0; \
    (void*)0; (void*)0; (void*)0; (void*)0; (void*)0; (void*)0; (void*)0; (void*)0; (void*)0; (void*)0; \
    (void*)0; (void*)0; (void*)0; (void*)0; (void*)0; (void*)0; (void*)0; (void*)0; (void*)0; (void*)0; \
    (void*)0; (void*)0; (void*)0; (void*)0; (void*)0; (void*)0; (void*)0; (void*)0; (void*)0; (void*)0; \
    (void*)0; (void*)0; (void*)0; (void*)0; (void*)0; (void*)0; (void*)0; (void*)0; (void*)0; (void*)0; \
    (void*)0; (void*)0; (void*)0; (void*)0; (void*)0; (void*)0; (void*)0; (void*)0; (void*)0; (void*)0; \
    (void*)0; (void*)0; (void*)0; (void*)0; (void*)0; (void*)0; (void*)0; (void*)0; (void*)0; (void*)0; \
    (void*)0; (void*)0; (void*)0; (void*)0; (void*)0; (void*)0; (void*)0; (void*)0; (void*)0; (void*)0; \
    (void*)0; (void*)0; (void*)0; (void*)0; (void*)0; (void*)0; (void*)0; (void*)0; (void*)0; (void*)0;

#ifdef __MWERKS__
#define SJIS(character, value) character
#else
#define SJIS(character, value) ((u32)value)
#endif

#ifdef __MWERKS__
#define ASM asm
#else
#define ASM
#endif

// potential fakematch?
#if PLATFORM_SHIELD
    #define UNSET_FLAG(var, flag, type) (var) &= (type)~(flag)
#else
    #define UNSET_FLAG(var, flag, type) (var) &= ~(flag)
#endif

#endif
