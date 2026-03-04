#ifndef _DUSK_ENDIAN_SSYSTEM_H_
#define _DUSK_ENDIAN_SSYSTEM_H_

#include "SSystem/SComponent/c_sxyz.h"
#include "endian.h"

template<>
struct BE<csXyz> {
    BE<s16> x;
    BE<s16> y;
    BE<s16> z;

    auto & operator =(const csXyz & arg) {
        this->x = arg.x;
        this->y = arg.y;
        this->z = arg.z;
        return *this;
    }

    void set(s16 oX, s16 oY, s16 oZ) {
        x = oX;
        y = oY;
        z = oZ;
    }

    operator csXyz() const {
        return { x, y, z };
    }

    static csXyz swap(csXyz val) {
        return {
            BE<s16>::swap(val.x),
            BE<s16>::swap(val.y),
            BE<s16>::swap(val.z),
        };
    }
};

template<>
struct BE<cXyz> {
    BE<f32> x;
    BE<f32> y;
    BE<f32> z;

    auto & operator =(const cXyz & arg) {
        this->x = arg.x;
        this->y = arg.y;
        this->z = arg.z;
        return *this;
    }

    operator cXyz() const {
        return { x, y, z };
    }

    static cXyz swap(cXyz val) {
        return {
            BE<f32>::swap(val.x),
            BE<f32>::swap(val.y),
            BE<f32>::swap(val.z),
        };
    }
};

#endif