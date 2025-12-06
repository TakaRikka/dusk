#ifndef TPOSITION3_H
#define TPOSITION3_H

#include "dolphin/mtx.h"
#include "JSystem/JMath/JMath.h"
#include "JSystem/JGeometry.h"

namespace JGeometry {

template <typename T>
struct SMatrix34C {
    T data[3][4];
};

template <>
struct SMatrix34C<f32> {
    f32 data[3][4];

    typedef f32 ArrType[4];
    void set(const ArrType* src) {  
        JMath::gekko_ps_copy12(data, src);
    }

    operator ArrType*() { return data; }
    operator const ArrType*() const { return data; }
};

template <typename T>
struct SMatrix33C {
    T data[3][3];

    inline T& ref(int i, int j) {
        return data[i][j];
    }

    inline T at(int i, int j) const {
        return data[i][j];
    }

    inline void set(T param_1, T param_2, T param_3, T param_4, T param_5,
             T param_6, T param_7, T param_8, T param_9) {
        ref(0,0) = param_1;
        ref(0,1) = param_2;
        ref(0,2) = param_3;
        ref(1,0) = param_4;
        ref(1,1) = param_5;
        ref(1,2) = param_6;
        ref(2,0) = param_7;
        ref(2,1) = param_8;
        ref(2,2) = param_9;
    }
};

template <typename T>
struct TMatrix34 : public T {
    void identity() { MTXIdentity(this->data); }
};

template <typename T>
struct TRotation3 : public T {};

template<typename T>
struct TRotation3<SMatrix33C<T> > : public SMatrix33C<T> {
    inline void getEulerXYZ(TVec3<T>* param_1) const {
        if (this->at(2, 0) - TUtil<T>::one() >= -TUtil<T>::epsilon()) {
            param_1->set(TUtil<T>::atan2(-this->at(0, 1), this->at(1, 1)), -TUtil<T>::halfPI(), 0.0);
        } else {
            if (this->at(2, 0) + TUtil<T>::one() <= TUtil<T>::epsilon()) {
                param_1->set(TUtil<T>::atan2(this->at(0, 1), this->at(1, 1)), TUtil<T>::halfPI(), 0.0);
            } else {
                param_1->x = TUtil<T>::atan2(this->at(2, 1), this->at(2, 2));
                param_1->z = TUtil<T>::atan2(this->at(1, 0), this->at(0, 0));
                param_1->y = TUtil<T>::asin(-this->at(2, 0));
            }
        }
    }
};

template <typename T>
struct TPosition3 : public T {
    TPosition3() {}
};

typedef TPosition3<TMatrix34<SMatrix34C<f32> > > TPosition3f32;

}  // namespace JGeometry

#endif
