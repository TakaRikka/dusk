#ifndef _denormals_
#define _denormals_

#if defined(__x86_64__) || defined(_M_X64) || defined(__i386__) || defined(_M_IX86)

#include <immintrin.h>
using denormal_state = unsigned int;
inline denormal_state denormals_enable()
{
    denormal_state saved = _mm_getcsr();
    _mm_setcsr(saved | 0x8040); // FTZ (0x8000) | DAZ (0x0040)
    return saved;
}
inline void denormals_restore(denormal_state saved) { _mm_setcsr(saved); }

#elif defined(__aarch64__) || defined(_M_ARM64)

#include <cstdint>
using denormal_state = uint64_t;
inline denormal_state denormals_enable()
{
    denormal_state saved;
    asm volatile("mrs %0, fpcr" : "=r"(saved));
    asm volatile("msr fpcr, %0" :: "r"(saved | (1ULL << 24))); // FZ
    return saved;
}
inline void denormals_restore(denormal_state saved)
{
    asm volatile("msr fpcr, %0" :: "r"(saved));
}

#elif defined(__arm__) || defined(_M_ARM)

#include <cstdint>
using denormal_state = uint32_t;
inline denormal_state denormals_enable()
{
    denormal_state saved;
    asm volatile("vmrs %0, fpscr" : "=r"(saved));
    asm volatile("vmsr fpscr, %0" :: "r"(saved | (1U << 24))); // FZ
    return saved;
}
inline void denormals_restore(denormal_state saved)
{
    asm volatile("vmsr fpscr, %0" :: "r"(saved));
}

#else

// unknown platform so denormals will be preserved
#warning "This platform is not supported for denormals, reverb will be very slow!"
using denormal_state = int;
inline denormal_state denormals_enable() { return 0; }
inline void denormals_restore(denormal_state) {}

#endif

#endif
