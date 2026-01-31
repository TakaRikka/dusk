#ifndef JPAMATH_H
#define JPAMATH_H

#include <dolphin/mtx.h>
#include "JSystem/JGeometry.h"

void JPAGetDirMtx(JGeometry::TVec3<f32> const& param_0, f32 (*param_1)[4]);
void JPAGetYZRotateMtx(s16 angleY, s16 angleZ, f32 (*param_2)[4]);
void JPAGetXYZRotateMtx(s16 x, s16 y, s16 z, Mtx dst);
void JPASetRMtxTVecfromMtx(f32 const (*param_0)[4], f32 (*param_1)[4],
                               JGeometry::TVec3<f32>* param_2);
f32 JPACalcKeyAnmValue(f32 param_0, u16 param_1, f32 const* param_2);
void JPASetRMtxSTVecfromMtx(f32 const (*param_0)[4], f32 (*param_1)[4],
                                JGeometry::TVec3<f32>* param_2, JGeometry::TVec3<f32>* param_3);

#endif /* JPAMATH_H */
