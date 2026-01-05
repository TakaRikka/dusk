#ifndef NW4HBM_MATH_ARITHMETIC_H
#define NW4HBM_MATH_ARITHMETIC_H

#include <math>
#include <math>

#include <revolution/os.h>
#include <revolution/types.h>


namespace nw4hbm {
    namespace math {
        f32 FrSqrt(f32 x);

        f32 Hermite(f32 p1, f32 t1, f32 p2, f32 t2, f32 s);
        f32 Bezier(f32 p1, f32 p2, f32 p3, f32 p4, f32 s);
        f32 CatmullRom(f32 p0, f32 p1, f32 p2, f32 p3, f32 s);

        u32 CntBit1(u32 x);
        u32 CntBit1(u32 const* first, u32 const* last);
        u32 DistBit(u32 const* first1, u32 const* last1, u32 const* first2);
        u32 RevBit(u32 x);

        int IExp(int x, u32 n);
        u32 ILog10(u32 x);

        inline u32 F32AsU32(f32 x) {
            return *reinterpret_cast<u32*>(&x);
        }

        inline f32 U32AsF32(u32 x) {
            return *reinterpret_cast<f32*>(&x);
        }

        inline s32 FGetExpPart(f32 f) {
            s32 s = (F32AsU32(f) >> 23) & 0xff;

            return s - 127;
        }

        inline f32 FGetMantPart(f32 f) {
            u32 u = (F32AsU32(f) & 0x807fffff) | 0x3f800000;

            return U32AsF32(u);
        }

        inline f32 FSelect(register f32 cond, register f32 ifPos, register f32 ifNeg) {
            register f32 ret;

#if defined(__MWERKS__)
            asm { fsel ret, cond, ifPos, ifNeg }
            ;
#else
#pragma unused(cond, ifPos, ifNeg)
            ret = 0;
#endif

            return ret;
        }

        inline f32 FAbs(register f32 x) {
            register f32 ret;

#if defined(__MWERKS__)
            asm { fabs ret, x }
#else
#pragma unused(x)
            ret = 0;
#endif

            return ret;
        }

        inline f32 FNAbs(register f32 x) {
            register f32 ret;

#if defined(__MWERKS__)
            asm { fnabs ret, x }
#else
#pragma unused(x)
            ret = 0;
#endif

            return ret;
        }

        inline f32 AcosRad(f32 x) {
            return std::acos(x);
        }

        inline f32 FCopySign(f32 abs, f32 sign) {
            f32 pos = FAbs(abs);
            f32 neg = FNAbs(abs);

            return FSelect(sign, pos, neg);
        }

        inline s16 F32ToS16(f32 x) {
            s16 rval;

            OSf32tos16(&x, &rval);

            return rval;
        }

        inline u16 F32ToU16(f32 x) {
            u16 rval;

            OSf32tou16(&x, &rval);

            return rval;
        }

        inline f32 U16ToF32(u16 x) {
            f32 rval;

            OSu16tof32(&x, &rval);

            return rval;
        }

        inline f32 S16ToF32(s16 x) {
            f32 rval;

            OSs16tof32(&x, &rval);

            return rval;
        }

        inline f32 FSqrt(f32 x) {
            return x <= 0.0f ? 0.0f : x * FrSqrt(x);
        }

        inline f32 FCbrt(f32 x) {
            return std::pow(x, 1.0f / 3.0f);
        }

        inline u32 CntLz(register u32 x) {
            register u32 result;

#if defined(__MWERKS__)
            asm { cntlzw result, x }
#else
#pragma unused(x)
            result = 0;
#endif

            return result;
        }

        inline u32 DistBit(u32 x, u32 y) {
            return CntBit1(x ^ y);
        }

        namespace detail {
            f32 FExp(f32 x);
            f32 FLog(f32 x);

            u32 CntLz_(u32 x);
        }  // namespace detail
    }  // namespace math
}  // namespace nw4hbm

#endif  // NW4HBM_MATH_ARITHMETIC_H
