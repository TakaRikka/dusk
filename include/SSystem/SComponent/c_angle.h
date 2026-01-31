#ifndef C_ANGLE_H
#define C_ANGLE_H

#include "SSystem/SComponent/c_xyz.h"

#define ADD_VAR(x, y)  ((x) += (y))
#define SUB_VAR(x, y)  ((x) -= (y))
#define MULT_VAR(x, y) ((x) *= (y))

#define ADD_VAR_CAST(x, y, t)  ((x) += (t)(y))
#define SUB_VAR_CAST(x, y, t)  ((x) -= (t)(y))
#define MULT_VAR_CAST(x, y, t) ((x) *= (t)(y))

#define ADD_ANGLE(x, y)  ADD_VAR_CAST(x, y, s16)
#define SUB_ANGLE(x, y)  SUB_VAR_CAST(x, y, s16)
#define MULT_ANGLE(x, y) MULT_VAR_CAST(x, y, s16)

// There are some angles that weren't sign-extended until the shield version
#if !PLATFORM_SHIELD
    #define ADD_ANGLE_2    ADD_VAR
    #define SUB_ANGLE_2    SUB_VAR
    #define MULT_ANGLE_2   MULT_VAR

    #define ADD_S8_2       ADD_VAR
#else
    #define ADD_ANGLE_2    ADD_ANGLE
    #define SUB_ANGLE_2    SUB_ANGLE
    #define MULT_ANGLE_2   MULT_ANGLE
    
    #define ADD_S8_2(x, y) ADD_VAR_CAST(x, y, s8)
#endif

#define DEG2S_CONSTANT (0x8000 / 180.0f)
#define S2DEG_CONSTANT (180.0f / 0x8000)
#define S2RAD_CONSTANT (M_PI / 0x8000)
#define RAD2S_CONSTANT (0x8000 / M_PI)

#define DEG2S(x) ((s16)((x) * DEG2S_CONSTANT))
#define S2DEG(x) ((x) * S2DEG_CONSTANT)
#define S2RAD(x) ((x) * S2RAD_CONSTANT)
#define RAD2S(x) ((x) * RAD2S_CONSTANT)

class cSAngle {
private:
    s16 mAngle;

public:
    const static cSAngle _0;
    const static cSAngle _1;
    const static cSAngle _90;
    const static cSAngle _180;
    const static cSAngle _270;
#ifdef __MWERKS__
    cSAngle() {}
    ~cSAngle() {}
    cSAngle(const cSAngle&);
#else
    cSAngle() = default;
    ~cSAngle() = default;
    cSAngle(const cSAngle&) = default;
#endif
    cSAngle(s16);
    cSAngle(float);
    s16 Val() const { return this->mAngle; }
    // void Set(s16 angle) { this->mAngle = angle; }
    void Val(const cSAngle&);
    void Val(s16);
    void Val(float);
    float Degree(void) const;
    float Radian(void) const;
    float Norm(void) const;
    s16 Abs(void) const;
    s16 Inv(void) const;
    float Sin(void) const;
    float Cos(void) const;
    cSAngle operator-(void) const;
    cSAngle operator+(const cSAngle&) const;
    cSAngle operator-(const cSAngle&) const;
    void operator+=(const cSAngle&);
    void operator-=(const cSAngle&);
    cSAngle operator+(short) const;
    cSAngle operator-(short) const;
    void operator+=(short);
    cSAngle operator*(float) const;
    void operator*=(float);
    bool operator<(const cSAngle& other) const { return mAngle < other.mAngle; }
    bool operator>(const cSAngle& other) const { return mAngle > other.mAngle; }
    bool operator<=(const cSAngle& other) const { return mAngle <= other.mAngle; }
    bool operator>=(const cSAngle& other) const { return mAngle >= other.mAngle; }
    bool operator==(const cSAngle& other) const { return mAngle == other.mAngle; }
    operator s16(void) const { return mAngle; }
    static inline cSAngle getMaxNegative(void) { return cSAngle((s16)-0x8000); }
    inline void mirrorAtMaxNeg(void) { *this = cSAngle((s16)-0x8000) - *this; }
};

cSAngle operator+(short, const cSAngle&);
cSAngle operator-(short, const cSAngle&);

struct cAngle {
    static f32 Radian_to_Degree(f32 rad) { return rad * (180.0f / M_PI); }
    static f32 Degree_to_Radian(f32 deg) { return deg * (M_PI / 180.0f); }
    static s16 Degree_to_SAngle(f32 deg) { return DEG2S(deg); }
    static f32 SAngle_to_Degree(s16 angle) { return S2DEG(angle); }
    static f32 SAngle_to_Radian(s16 angle) { return angle * (M_PI / 0x8000); }
    static f32 SAngle_to_Normal(s16 angle) { return angle * (1.0f / 0x8000); }
    static s16 Radian_to_SAngle(f32 rad) { return rad * (0x8000 / M_PI); }

    /* Converts Radian value into Degree value */
    static f32 r2d(f32 r) { return Radian_to_Degree(r); }

    /* Converts Degree value into Radian value */
    static f32 d2r(f32 d) { return Degree_to_Radian(d); }

    /* Converts Degree value to s16 angle */
    static s16 d2s(f32 d) { return Degree_to_SAngle(d); }

    /* Converts s16 angle to Degree value */
    static f32 s2d(s16 a) { return SAngle_to_Degree(a); }

    template <typename T>
    static T Adjust(T f1, T f2, T f3);
};

template <typename T>
T cAngle::Adjust(T f1, T f2, T f3) {
    while (f1 >= f3) {
        f1 -= f3 - f2;
    }
    while (f1 < f2) {
        f1 += f3 - f2;
    }
    return f1;
}

class cDegree {
private:
    float mDegree;

public:
    cDegree(float);
    ~cDegree() {}

    cDegree& Formal(void);
    void Val(float);
    float Radian(void) const;
    float Sin(void) const;
    float Cos(void) const;
    float Tan(void) const;

    operator f32() const { return mDegree; }

    cDegree operator*(float v) const {
        return cDegree(mDegree * v);
    }
};

class cSPolar {
private:
    float mRadial;
    cSAngle mAngle1;
    cSAngle mAngle2;

public:
    cSPolar() {}
    cSPolar(const cXyz&);
    cSPolar& Formal(void);
    void Val(float, short, short);
    void Val(const cXyz&);
    cXyz Xyz(void) const;
    void Globe(class cSGlobe*) const;

    void R(f32 i_radial) { mRadial = i_radial; }
    void U(cSAngle const& i_angle) { mAngle2 = i_angle.Val(); }
    void V(cSAngle const& i_angle) { mAngle1 = i_angle.Val(); }
};

class cSGlobe {
public:
    float mRadius;
    cSAngle mInclination;  // original: V
    cSAngle mAzimuth;      // original: U

#ifdef __MWERKS__
    cSGlobe() {}
    ~cSGlobe() {}
    cSGlobe(const cSGlobe&);
#else
    cSGlobe() = default;
    ~cSGlobe() = default;
    cSGlobe(const cSGlobe&) = default;
#endif

    void R(f32 i_radius) { mRadius = i_radius; }
    void U(cSAngle const& i_azimuth) { mAzimuth = i_azimuth.Val(); }
    void V(cSAngle const& i_inclination) { mInclination = i_inclination.Val(); }

    cSGlobe(float, short, short);
    cSGlobe(float, const cSAngle&, const cSAngle&);
    cSGlobe(const cXyz&);
    cSGlobe& Formal(void);
    void Val(const cSGlobe&);
    void Val(float, short, short);
    void Val(float, const cSAngle&, const cSAngle&);
    void Val(const cXyz&);
    float R(void) const { return mRadius; }
    const cSAngle& V(void) const { return mInclination; }
    const cSAngle& U(void) const { return mAzimuth; }
    cXyz Xyz(void) const;
    void Polar(cSPolar*) const;
    cXyz Norm(void) const;
    cSGlobe& Invert(void);
};

#endif /* C_ANGLE_H */
