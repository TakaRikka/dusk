/**
 * c_angle.cpp
 *
 */

#include "SSystem/SComponent/c_angle.h"

extern f32 cM_atan2f(f32, f32);

const cSAngle cSAngle::_0(static_cast<s16>(0));
const cSAngle cSAngle::_1(static_cast<s16>(0xb6));
const cSAngle cSAngle::_90(static_cast<s16>(0x4000));
const cSAngle cSAngle::_180(static_cast<s16>(-0x8000));
const cSAngle cSAngle::_270(static_cast<s16>(-0x4000));

#ifdef __MWERKS__
cSAngle::cSAngle(const cSAngle& angle) {
    Val(angle);
}
#endif

cSAngle::cSAngle(s16 angle) {
    Val(angle);
}

cSAngle::cSAngle(f32 angle) {
    Val(angle);
}

void cSAngle::Val(const cSAngle& other) {
    mAngle = other.mAngle;
}

void cSAngle::Val(s16 val) {
    mAngle = val;
}

void cSAngle::Val(f32 degree) {
    mAngle = cAngle::Degree_to_SAngle(degree);
}

f32 cSAngle::Degree() const {
    return cAngle::SAngle_to_Degree(mAngle);
}

f32 cSAngle::Radian() const {
    return cAngle::SAngle_to_Radian(mAngle);
}

f32 cSAngle::Norm() const {
    return cAngle::SAngle_to_Normal(mAngle);
}

s16 cSAngle::Abs() const {
    return mAngle >= 0 ? mAngle : -mAngle;
}

s16 cSAngle::Inv() const {
    return mAngle - 0x8000;
}

f32 cSAngle::Sin() const {
    return sin(Radian());
}

f32 cSAngle::Cos() const {
    return cos(Radian());
}

cSAngle cSAngle::operator-() const {
    return cSAngle(static_cast<s16>(-mAngle));
}

cSAngle cSAngle::operator+(const cSAngle& other) const {
    return cSAngle(static_cast<s16>(mAngle + other.mAngle));
}

cSAngle cSAngle::operator-(const cSAngle& other) const {
    return cSAngle(static_cast<s16>(mAngle - other.mAngle));
}

void cSAngle::operator+=(const cSAngle& other) {
    mAngle += other.mAngle;
}

void cSAngle::operator-=(const cSAngle& other) {
    mAngle -= other.mAngle;
}

cSAngle cSAngle::operator+(s16 other) const {
    return cSAngle(static_cast<s16>(mAngle + other));
}

cSAngle cSAngle::operator-(s16 other) const {
    return cSAngle(static_cast<s16>(mAngle - other));
}

void cSAngle::operator+=(s16 other) {
    mAngle += other;
}

cSAngle cSAngle::operator*(f32 f1) const {
    return cSAngle(static_cast<s16>(mAngle * f1));
}

void cSAngle::operator*=(f32 f1) {
    mAngle *= f1;
}

cSAngle operator+(s16 other, const cSAngle& angle) {
    return cSAngle(static_cast<s16>(other + angle.Val()));
}

cSAngle operator-(s16 other, const cSAngle& angle) {
    return cSAngle(static_cast<s16>(other - angle.Val()));
}

cDegree::cDegree(f32 f) {
    Val(f);
}

cDegree& cDegree::Formal() {
    mDegree = cAngle::Adjust<f32>(mDegree, -180.0f, 180.0f);
    return *this;
}

void cDegree::Val(f32 f) {
    mDegree = f;
    Formal();
}

f32 cDegree::Radian() const {
    return cAngle::Degree_to_Radian(mDegree);
}

f32 cDegree::Sin() const {
    return sin(Radian());
}

f32 cDegree::Cos() const {
    return cos(Radian());
}

f32 cDegree::Tan() const {
    return tan(Radian());
}

cSPolar::cSPolar(const cXyz& xyz) {
    Val(xyz);
}

cSPolar& cSPolar::Formal() {
    if (mRadial < 0.0f) {
        mRadial = -mRadial;
        cSAngle tmp(static_cast<s16>(-0x8000));
        mAngle1.Val(tmp - mAngle1);
        mAngle2.Val(mAngle2.Inv());
    }

    if (mAngle1.Val() < 0 && mAngle1.Val() != -0x8000) {
        mAngle1.Val(-mAngle1);
        mAngle2.Val(mAngle2.Inv());
    }

    return *this;
}

void cSPolar::Val(f32 f, s16 s1, s16 s2) {
    mRadial = f;
    mAngle1 = cSAngle(s1);
    mAngle2 = cSAngle(s2);
    Formal();
}

inline double square(f32 f) {
    return f * f;
}

void cSPolar::Val(const cXyz& xyz) {
    f32 x, y, z, tmp4;
    x = xyz.x;
    y = xyz.y;
    z = xyz.z;
    double tmp = square(z) + square(x);
    double tmp2 = square(y) + tmp;
    tmp4 = (tmp > 0.0) ? (f32)sqrt(tmp) : 0.0f;
    mRadial = (tmp2 > 0.0) ? (f32)sqrt(tmp2) : 0.0f;
    mAngle1.Val(cAngle::Radian_to_SAngle(cM_atan2f(tmp4, y)));
    mAngle2.Val(cAngle::Radian_to_SAngle(cM_atan2f(x, z)));
    Formal();
}

cXyz cSPolar::Xyz() const {
    f32 ang1Cos;
    f32 ang2Cos;
    f32 ang1Sin = mRadial * (f32)sin(mAngle1.Radian());
    ang2Cos = (f32)cos(mAngle2.Radian());
    ang1Cos = mRadial * (f32)cos(mAngle1.Radian());
    f32 ang2Sin = sin(mAngle2.Radian());
    return cXyz(ang1Sin * ang2Sin, ang1Cos, ang1Sin * ang2Cos);
}

void cSPolar::Globe(cSGlobe* globe) const {
    globe->Val(mRadial, 0x4000 - mAngle1.Val(), mAngle2.Val());
}

#ifdef __MWERKS__
cSGlobe::cSGlobe(const cSGlobe& other) {
    Val(other);
}
#endif

cSGlobe::cSGlobe(f32 f, s16 s1, s16 s2) {
    Val(f, s1, s2);
}

cSGlobe::cSGlobe(f32 f, const cSAngle& a1, const cSAngle& a2) {
    Val(f, a1, a2);
}

cSGlobe::cSGlobe(const cXyz& xyz) {
    Val(xyz);
}

cSGlobe& cSGlobe::Formal() {
    if (mRadius < 0.0f) {
        mRadius = -mRadius;
        mInclination = -mInclination;
        mAzimuth.Val(mAzimuth.Inv());
    }

    if (mInclination.Val() < -0x4000 || 0x4000 < mInclination.Val()) {
        mInclination = cSAngle((s16)-0x8000) - mInclination;
        mAzimuth.Val(mAzimuth.Inv());
    }

    return *this;
}

void cSGlobe::Val(const cSGlobe& other) {
    mRadius = other.mRadius;
    mInclination = other.mInclination;
    mAzimuth = other.mAzimuth;
    Formal();
}

void cSGlobe::Val(f32 f, s16 s1, s16 s2) {
    mRadius = f;
    mInclination = cSAngle(s1);
    mAzimuth = cSAngle(s2);
    Formal();
}

void cSGlobe::Val(f32 f, const cSAngle& a1, const cSAngle& a2) {
    mRadius = f;
    mInclination = cSAngle(a1.Val());
    mAzimuth = cSAngle(a2.Val());
    Formal();
}

void cSGlobe::Val(const cXyz& xyz) {
    cSPolar csp(xyz);
    csp.Globe(this);
    Formal();
}

cXyz cSGlobe::Xyz() const {
    cSPolar csp;
    Polar(&csp);
    return csp.Xyz();
}

void cSGlobe::Polar(cSPolar* csp) const {
    csp->Val(mRadius, 0x4000 - mInclination.Val(), mAzimuth.Val());
}

cXyz cSGlobe::Norm() const {
    cSGlobe glob(1.0f, mInclination, mAzimuth);
    return glob.Xyz();
}

cSGlobe& cSGlobe::Invert() {
    mRadius = -mRadius;
    return Formal();
}
