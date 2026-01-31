#ifndef JMATH_H
#define JMATH_H

#include <dolphin/mtx.h>
#include <cmath>

void JMAMTXApplyScale(const Mtx, Mtx, f32, f32, f32);
void JMAEulerToQuat(s16 param_0, s16 param_1, s16 param_2, Quaternion* param_3);
void JMAQuatLerp(const Quaternion*, const Quaternion*, f32, Quaternion*);
void JMAFastVECNormalize(__REGISTER const Vec* src, __REGISTER Vec* dst);
void JMAVECScaleAdd(__REGISTER const Vec* vec1, __REGISTER const Vec* vec2, __REGISTER Vec* dst,
                    __REGISTER f32 scale);

inline int JMAAbs(int value) {
#ifdef __MWERKS__
    return __abs(value);
#endif
}

inline f32 JMAAbs(f32 x) {
#ifdef __MWERKS__
    return __fabsf(x);
#endif
}

inline f32 JMAFastReciprocal(f32 value) {
#ifdef __MWERKS__
    return __fres(value);
#endif
}

inline float __frsqrtes(__REGISTER double f) {
#ifdef __MWERKS__
    __REGISTER float out;
    // clang-format off

    asm {
        frsqrte out, f
    }

    // clang-format on
    return out;
#endif
}

inline f32 JMAFastSqrt(__REGISTER const f32 input) {
#ifdef __MWERKS__
    if (input > 0.0f) {
        __REGISTER f32 out;
        asm {
            frsqrte out, input
        }
        return out * input;
    } else {
        return input;
    }
#endif
}

inline f32 JMAHermiteInterpolation(__REGISTER f32 p1, __REGISTER f32 p2, __REGISTER f32 p3,
                                   __REGISTER f32 p4, __REGISTER f32 p5, __REGISTER f32 p6,
                                   __REGISTER f32 p7) {
#ifdef __MWERKS__
    __REGISTER f32 ff25;
    __REGISTER f32 ff31;
    __REGISTER f32 ff30;
    __REGISTER f32 ff29;
    __REGISTER f32 ff28;
    __REGISTER f32 ff27;
    __REGISTER f32 ff26;
    // clang-format off
    asm {
        fsubs   ff31, p1, p2
        fsubs   ff30, p5, p2
        fdivs   ff29, ff31, ff30
        fmuls   ff28,ff29,ff29
        fadds   ff25,ff29,ff29
        fsubs   ff27,ff28,ff29
        fsubs   ff30, p3, p6
        fmsubs  ff26,ff25,ff27,ff28
        fmadds  ff25,p4,ff27,p4
        fmadds  ff26,ff26,ff30,p3
        fmadds  ff25,p7,ff27,ff25
        fmsubs  ff25,ff29,p4,ff25
        fnmsubs ff25,ff31,ff25,ff26

    }
    // clang-format on
    return ff25;
#endif
}

namespace JMath {

template <typename T>
inline T fastSqrt(T value) {
    return JMAFastSqrt(value);
}

inline f32 fastReciprocal(f32 value) {
    return JMAFastReciprocal(value);
}

inline void fastVECNormalize(const Vec* src, Vec* dst) {
    return JMAFastVECNormalize(src, dst);
}

inline void gekko_ps_copy3(__REGISTER void* dst, __REGISTER const void* src) {
#ifdef __MWERKS__
    __REGISTER f32 src0;
    __REGISTER f32 src1;
    asm {
        psq_l src0, 0(src), 0, 0
        lfs src1, 8(src)
        psq_st src0, 0(dst), 0, 0
        stfs src1, 8(dst)
    };
#endif
}

inline void gekko_ps_copy6(__REGISTER void* dst, __REGISTER const void* src) {
#ifdef __MWERKS__
    __REGISTER f32 src0;
    __REGISTER f32 src1;
    __REGISTER f32 src2;
    asm {
        psq_l src0, 0(src), 0, 0
        psq_l src1, 8(src), 0, 0
        psq_l src2, 16(src), 0, 0
        psq_st src0, 0(dst), 0, 0
        psq_st src1, 8(dst), 0, 0
        psq_st src2, 16(dst), 0, 0
    };
#endif
}

inline void gekko_ps_copy12(__REGISTER void* dst, __REGISTER const void* src) {
#ifdef __MWERKS__
    __REGISTER f32 src0;
    __REGISTER f32 src1;
    __REGISTER f32 src2;
    __REGISTER f32 src3;
    __REGISTER f32 src4;
    __REGISTER f32 src5;
    asm {
        psq_l src0, 0(src), 0, 0
        psq_l src1, 8(src), 0, 0
        psq_l src2, 16(src), 0, 0
        psq_l src3, 24(src), 0, 0
        psq_l src4, 32(src), 0, 0
        psq_l src5, 40(src), 0, 0
        psq_st src0, 0(dst), 0, 0
        psq_st src1, 8(dst), 0, 0
        psq_st src2, 16(dst), 0, 0
        psq_st src3, 24(dst), 0, 0
        psq_st src4, 32(dst), 0, 0
        psq_st src5, 40(dst), 0, 0
    };
#endif
}

inline void gekko_ps_copy16(__REGISTER void* dst, __REGISTER const void* src) {
#ifdef __MWERKS__
    __REGISTER f32 src0;
    __REGISTER f32 src1;
    __REGISTER f32 src2;
    __REGISTER f32 src3;
    __REGISTER f32 src4;
    __REGISTER f32 src5;
    __REGISTER f32 src6;
    __REGISTER f32 src7;
    asm {
        psq_l src0, 0(src), 0, 0
        psq_l src1, 8(src), 0, 0
        psq_l src2, 16(src), 0, 0
        psq_l src3, 24(src), 0, 0
        psq_l src4, 32(src), 0, 0
        psq_l src5, 40(src), 0, 0
        psq_l src6, 48(src), 0, 0
        psq_l src7, 56(src), 0, 0
        psq_st src0, 0(dst), 0, 0
        psq_st src1, 8(dst), 0, 0
        psq_st src2, 16(dst), 0, 0
        psq_st src3, 24(dst), 0, 0
        psq_st src4, 32(dst), 0, 0
        psq_st src5, 40(dst), 0, 0
        psq_st src6, 48(dst), 0, 0
        psq_st src7, 56(dst), 0, 0
    };
#endif
}

};  // namespace JMath

namespace JMathInlineVEC {
    inline void C_VECAdd(__REGISTER const Vec* a, __REGISTER const Vec* b, __REGISTER Vec* ab) {
    #ifdef __MWERKS__
        __REGISTER f32 axy;
        __REGISTER f32 bxy;
        __REGISTER f32 sumab;
        __REGISTER f32 az;
        __REGISTER f32 bz;
        __REGISTER f32 sumz;
        asm {
            psq_l axy, 0(a), 0, 0
            psq_l bxy, 0(b), 0, 0
            ps_add sumab, axy, bxy
            psq_st sumab, 0(ab), 0, 0
            psq_l az, 8(a), 1, 0
            psq_l bz, 8(b), 1, 0
            ps_add sumz, az, bz
            psq_st sumz, 8(ab), 1, 0
        }
    #endif
    }

    inline void C_VECSubtract(__REGISTER const Vec* a, __REGISTER const Vec* b, __REGISTER Vec* ab) {
    #ifdef __MWERKS__
        __REGISTER f32 axy;
        __REGISTER f32 bxy;
        __REGISTER f32 az;
        __REGISTER f32 subz;
        __REGISTER f32 bz;
        asm {
            psq_l axy, 0(a), 0, 0
            psq_l bxy, 0(b), 0, 0
            ps_sub bxy, axy, bxy
            psq_st bxy, 0(ab), 0, 0
            psq_l az, 8(a), 1, 0
            psq_l bz, 8(b), 1, 0
            ps_sub subz, az, bz
            psq_st subz, 8(ab), 1, 0
        }
    #endif
    }

    inline f32 C_VECSquareMag(__REGISTER const Vec* v) {
    #ifdef __MWERKS__
        __REGISTER f32 x_y;
        __REGISTER f32 z;
        __REGISTER f32 res;
    
        asm {
            psq_l   x_y, 0(v), 0, 0
            ps_mul  x_y, x_y, x_y
            lfs     z,   8(v)
            ps_madd res, z, z, x_y
            ps_sum0 res, res, x_y, x_y
        }
        return res;
    #endif
    }

    inline f32 C_VECDotProduct(__REGISTER const Vec *a, __REGISTER const Vec *b) {
    #ifdef __MWERKS__
        __REGISTER f32 res;
        __REGISTER f32 thisyz;
        __REGISTER f32 otheryz;
        __REGISTER f32 otherxy;
        __REGISTER f32 thisxy;
        asm {
            psq_l thisyz, 4(a), 0, 0
            psq_l otheryz, 4(b), 0, 0
            ps_mul thisyz, thisyz, otheryz
            psq_l thisxy, 0(a), 0, 0
            psq_l otherxy, 0(b), 0, 0
            ps_madd otheryz, thisxy, otherxy, thisyz
            ps_sum0 res, otheryz, thisyz, thisyz
        };
        return res;
    #endif
    }
};

template<typename T>
inline T JMAMax(T param_0, T param_1) {
    T ret;
    if (param_0 > param_1) {
        ret = param_0;
    } else {
        ret = param_1;
    }
    return ret;
}

#endif /* JMATH_H */
