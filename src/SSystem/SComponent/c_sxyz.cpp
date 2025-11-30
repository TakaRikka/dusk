/**
 * c_sxyz.cpp
 *
 */

#include "SSystem/SComponent/c_sxyz.h"

const csXyz csXyz::Zero = csXyz(0, 0, 0);

csXyz::csXyz(s16 x, s16 y, s16 z) {
    this->x = x;
    this->y = y;
    this->z = z;
}

csXyz csXyz::operator+(csXyz& other) {
    return csXyz(x + other.x, y + other.y, z + other.z);
}

void csXyz::operator+=(csXyz& other) {
    x += other.x;
    y += other.y;
    z += other.z;
}

csXyz csXyz::operator-(csXyz& other) {
    return csXyz(x - other.x, y - other.y, z - other.z);
}

csXyz csXyz::operator*(f32 mul) {
    return csXyz(x * mul, y * mul, z * mul);
}
