#ifndef C_XYZ_H
#define C_XYZ_H

#include "dolphin/mtx.h"
#include "math.h"

struct cXyz : Vec {
    static const cXyz Zero;
    static const cXyz BaseX;
    static const cXyz BaseY;
    static const cXyz BaseZ;
    static const cXyz BaseXY;
    static const cXyz BaseXZ;
    static const cXyz BaseYZ;
    static const cXyz BaseXYZ;
#ifdef __MWERKS__
    cXyz() {}
    ~cXyz() {}
    cXyz(const cXyz& vec) {
        x = vec.x;
        y = vec.y;
        z = vec.z;
    }
#else
    cXyz() = default;
    ~cXyz() = default;
    cXyz(const cXyz& vec) = default;
#endif
    cXyz(f32 x, f32 y, f32 z) {
        this->x = x;
        this->y = y;
        this->z = z;
    }
    cXyz(const Vec& vec) {
        this->x = vec.x;
        this->y = vec.y;
        this->z = vec.z;
    }
    void operator=(const Vec& vec) {
        this->x = vec.x;
        this->y = vec.y;
        this->z = vec.z;
    }
    cXyz operator+(Vec const&) const;
    cXyz operator-(Vec const&) const;
    cXyz operator*(f32) const;
    cXyz operator*(Vec const&) const;
    cXyz operator/(f32) const;

    void operator+=(f32 f) {
        x += f;
        y += f;
        z += f;
    }
    void operator-=(f32 f) {
        x -= f;
        y -= f;
        z -= f;
    }

    void operator*=(const Vec& other) {
        x *= other.x;
        y *= other.y;
        z *= other.z;
    }

    cXyz* operator-=(const Vec& other) {
        PSVECSubtract(this, &other, this);
        return this;
    }
    cXyz* operator+=(const Vec& other) {
        PSVECAdd(this, &other, this);
        return this;
    }
    cXyz* operator*=(f32 scale) {
        PSVECScale(this, this, scale);
        return this;
    }
    cXyz* operator/=(f32 scale) {
        PSVECScale(this, this, 1.0f / scale);
        return this;
    }
    cXyz getCrossProduct(Vec const&) const;
    cXyz outprod(Vec const&) const;
    cXyz norm() const;
    cXyz normZP() const;
    cXyz normZC() const;
    cXyz normalize();
    cXyz normalizeZP();
    bool normalizeRS();
    bool operator==(Vec const&) const;
    bool operator!=(Vec const&) const;
    bool isZero() const;
    s16 atan2sX_Z() const;
    s16 atan2sY_XZ() const;

    void set(f32 pX, f32 pY, f32 pZ) {
        x = pX;
        y = pY;
        z = pZ;
    }

    void set(const Vec& other) {
        x = other.x;
        y = other.y;
        z = other.z;
    }

    f32 getXDiff(const Vec* other) const { return x - other->x; }
    f32 getYDiff(const Vec* other) const { return y - other->y; }
    f32 getZDiff(const Vec* other) const { return z - other->z; }

    void setall(f32 f) { set(f, f, f); }

    void zero() { set(0.0f, 0.0f, 0.0f); }

    f32 getSquareMag() const { return PSVECSquareMag(this); }
    f32 getSquareDistance(const Vec& other) const { return PSVECSquareDistance(this, &other); }

    static f32 getNearZeroValue() { return 8e-11f; }

    bool isNearZeroSquare() const { return (this->getSquareMag() < getNearZeroValue()); }
    bool isNearZeroSquare(const cXyz& other) const { return (PSVECSquareMag(&other) < getNearZeroValue()); }

    f32 abs2() const { return this->getSquareMag(); }
    f32 abs2(const Vec& other) const { return this->getSquareDistance(other); }
    f32 abs2XZ() const {
        cXyz tmp(this->x, 0, this->z);
        return tmp.abs2();
    }
    f32 abs2XZ(const Vec& other) const {
        cXyz tmp(this->x, 0, this->z);
        cXyz tmp2(other.x, 0, other.z);
        return tmp.abs2(tmp2);
    }

    f32 abs() const { return sqrtf(this->abs2()); }
    f32 abs(const Vec& other) const { return sqrtf(this->abs2(other)); }
    f32 absXZ() const { return sqrtf(this->abs2XZ()); }
    f32 absXZ(const Vec& other) const { return sqrtf(this->abs2XZ(other)); }

    f32 getMagXZ() const { return cXyz(this->x, 0, this->z).getSquareMag(); }

    f32 getDotProduct(const Vec& other) const { return PSVECDotProduct(this, &other); }

    f32 inprod(const Vec& other) const { return getDotProduct(other); }
    f32 inprodXZ(const Vec& other) const { return x * other.x + z * other.z; }
};

#endif /* C_XYZ_H */
