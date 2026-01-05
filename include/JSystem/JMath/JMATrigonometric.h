#ifndef JMATRIGONOMETRIC_H
#define JMATRIGONOMETRIC_H

#include "dolphin/types.h"
#include <utility>

namespace JMath {
template<typename T>
struct TAngleConstant_;

/**
 * @ingroup jsystem-jmath
 * 
 */
template<>
struct TAngleConstant_<f32> {
    static f32 RADIAN_DEG090() { return 1.5707964f; }
    static f32 RADIAN_DEG180() { return 3.1415927f; }
    static f32 RADIAN_DEG360() { return 6.2831855f; }
    static f32 RADIAN_TO_DEGREE_FACTOR() { return 180.0f / RADIAN_DEG180(); }
};

/**
 * @ingroup jsystem-jmath
 * 
 */
template<int N, typename T>
struct TSinCosTable {
    std::pair<T, T> table[1 << N];

    T sinShort(s16 v) const { return table[(u16)v >> (16U - N)].first; }
    T cosShort(s16 v) const { return table[(u16)v >> (16U - N)].second; }

    inline T sinLap(T v) const {
        if (v < (T)0.0) {
            return -table[(u16)(-(T)(1 << N) * v) & ((1 << N) - 1)].first;
        }
        return table[(u16)((T)(1 << N) * v) & ((1 << N) - 1)].first;
    }

    inline T sinDegree(T degree) const {
        if (degree < (T)0.0) {
            return -table[(u16)(-((T)(1 << N) / (T)360.0) * degree) & ((1 << N) - 1)].first;
        } 
        return table[(u16)(((T)(1 << N) / (T)360.0) * degree) & ((1 << N) - 1)].first;
    }

    inline T cosDegree(T degree) const {
        if (degree < (T)0.0) {
            degree = -degree;
        } 
        return table[(u16)(((T)(1 << N) / (T)360.0) * degree) & ((1 << N) - 1)].second;
    }

    inline T sinRadian(T radian) const {
        if (radian < (T)0.0) {
            return -table[(u16)(-(T)(1 << N) / TAngleConstant_<T>::RADIAN_DEG360() * radian) & ((1 << N) - 1)].first;
        }
        return table[(u16)((T)(1 << N) / TAngleConstant_<T>::RADIAN_DEG360() * radian) & ((1 << N) - 1)].first;
    }
};

/**
 * @ingroup jsystem-jmath
 * 
 */
struct TAtanTable {
    f32 table[1025];
    u8 pad[0x1C];
};

/**
 * @ingroup jsystem-jmath
 * 
 */
template<int N, typename T>
struct TAsinAcosTable {
    T table[1025];
    u8 pad[0x1C];

    T acos_(T x) const {
        if (x >= 1.0f) {
            return 0.0f;
        } else if (x <= -1.0f) {
            return TAngleConstant_<T>::RADIAN_DEG180();
        } else if (x < 0.0f) {
            return table[(u32)(-x * 1023.5f)] + TAngleConstant_<T>::RADIAN_DEG090();
        } else {
            return TAngleConstant_<T>::RADIAN_DEG090() - table[(u32)(x * 1023.5f)];
        }
    }

    T acosDegree(T x) const {
        return acos_(x) * TAngleConstant_<T>::RADIAN_TO_DEGREE_FACTOR();
    }
};

extern TSinCosTable<13, f32> sincosTable_;
extern TAtanTable atanTable_;
extern TAsinAcosTable<1024, f32> asinAcosTable_;

inline f32 acosDegree(f32 x) {
    return asinAcosTable_.acosDegree(x);
}
};  // namespace JMath

inline f32 JMACosShort(s16 v) {
    return JMath::sincosTable_.cosShort(v);
}
inline f32 JMASinShort(s16 v) {
    return JMath::sincosTable_.sinShort(v);
}

inline f32 JMASCos(s16 v) {
    return JMACosShort(v);
}
inline f32 JMASSin(s16 v) {
    return JMASinShort(v);
}

inline f32 JMASinLap(f32 v) {
    return JMath::sincosTable_.sinLap(v);
}

inline f32 JMASinDegree(f32 degree) {
    return JMath::sincosTable_.sinDegree(degree);
}

inline f32 JMACosDegree(f32 degree) {
    return JMath::sincosTable_.cosDegree(degree);
}

inline f32 JMASinRadian(f32 radian) {
    return JMath::sincosTable_.sinRadian(radian);
}

#endif /* JMATRIGONOMETRIC_H */
