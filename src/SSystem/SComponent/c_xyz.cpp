/**
 * c_xyz.cpp
 *
 */

#include "SSystem/SComponent/c_xyz.h"
#include "SSystem/SComponent/c_math.h"
#include "JSystem/JUtility/JUTAssert.h"

cXyz cXyz::operator+(const Vec& vec) const {
    Vec ret;
    PSVECAdd(this, &vec, &ret);
    return ret;
}

cXyz cXyz::operator-(const Vec& vec) const {
    Vec ret;
    PSVECSubtract(this, &vec, &ret);
    return ret;
}

cXyz cXyz::operator*(f32 scale) const {
    Vec ret;
    PSVECScale(this, &ret, scale);
    return ret;
}

cXyz cXyz::operator*(const Vec& vec) const {
    cXyz ret;
    ret.x = this->x * vec.x;
    ret.y = this->y * vec.y;
    ret.z = this->z * vec.z;
    return ret;
}

cXyz cXyz::operator/(f32 scale) const {
    Vec ret;
    PSVECScale(this, &ret, 1.0f / scale);
    return ret;
}

cXyz cXyz::getCrossProduct(const Vec& vec) const {
    Vec ret;
    PSVECCrossProduct(this, &vec, &ret);
    return ret;
}

cXyz cXyz::outprod(const Vec& vec) const {
    return this->getCrossProduct(vec);
}

cXyz cXyz::norm() const {
    Vec ret;
    JUT_CONFIRM(251, isNearZeroSquare() == FALSE);
    PSVECNormalize(this, &ret);
    return ret;
}

cXyz cXyz::normZP() const {
    Vec vec;
    if (this->isNearZeroSquare() == false) {
        PSVECNormalize(this, &vec);
    } else {
        vec = cXyz::Zero;
    }
    return vec;
}

cXyz cXyz::normZC() const {
    Vec outVec;
    if (isNearZeroSquare() == 0) {
        PSVECNormalize(this, &outVec);
    } else {
        outVec = (*this * 1.25f * 1000000.0f).normZP();

        if (isNearZeroSquare(outVec)) {
            outVec.x = 0.0f;
            outVec.y = 0.0f;
            outVec.z = 1.0f;
            outVec = (Vec){0,0,1};
        }
    }

    return outVec;
}

cXyz cXyz::normalize() {
    JUT_ASSERT(285, isNearZeroSquare() == FALSE);
    PSVECNormalize(this, this);
    return *this;
}

cXyz cXyz::normalizeZP() {
    if (this->isNearZeroSquare() == false) {
        PSVECNormalize(this, this);
    } else {
        *this = cXyz::Zero;
    }
    return *this;
}

bool cXyz::normalizeRS() {
    if (this->isNearZeroSquare()) {
        return false;
    } else {
        PSVECNormalize(this, this);
        return true;
    }
}

bool cXyz::operator==(const Vec& vec) const {
    return this->x == vec.x && this->y == vec.y && this->z == vec.z;
}

bool cXyz::operator!=(const Vec& vec) const {
    return this->x != vec.x || this->y != vec.y || this->z != vec.z;
}

bool cXyz::isZero() const {
    return fabsf(this->x) < 32.0f * FLT_EPSILON && fabsf(this->y) < 32.0f * FLT_EPSILON &&
           fabsf(this->z) < 32.0f * FLT_EPSILON;
}

s16 cXyz::atan2sX_Z() const {
    return cM_atan2s(this->x, this->z);
}

s16 cXyz::atan2sY_XZ() const {
    return cM_atan2s(-this->y, absXZ());
}

const cXyz cXyz::Zero(0, 0, 0);
const cXyz cXyz::BaseX(1, 0, 0);
const cXyz cXyz::BaseY(0, 1, 0);
const cXyz cXyz::BaseZ(0, 0, 1);
const cXyz cXyz::BaseXY(1, 1, 0);
const cXyz cXyz::BaseXZ(1, 0, 1);
const cXyz cXyz::BaseYZ(0, 1, 1);
const cXyz cXyz::BaseXYZ(1, 1, 1);
