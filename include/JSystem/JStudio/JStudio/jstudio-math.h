#ifndef JSTUDIO_MATH_H
#define JSTUDIO_MATH_H

#include "dolphin/mtx.h"
#include "math.h"

namespace JStudio {
namespace math {
    void getRotation_xyz(MtxP, f32, f32, f32);
    void getTransformation_SRxyzT(MtxP, Vec const&, Vec const&, Vec const&);
    void getFromTransformation_SRxyzT(Vec*, Vec*, Vec*, CMtxP);

    inline void getRotation_y(Mtx param_0, f32 param_1) {
        MTXRotRad(param_0, 0x79, DEG_TO_RAD(param_1));
    }

    inline void getTransformation_RyT(Mtx param_0, const Vec& param_1, f32 param_2) {
        Mtx amStack_40;
        getRotation_y(amStack_40, param_2);
        MTXTransApply(amStack_40, param_0, param_1.x, param_1.y, param_1.z);
    }

    inline void rotate_y(Mtx param_0, Mtx param_1, f32 param_2) {
        Mtx afStack_38;
        getRotation_y(afStack_38, param_2);
        MTXConcat(afStack_38, param_1, param_0);
    }

    inline void rotate_xyz(Mtx param_0, Mtx param_1, f32 param_2, f32 param_3, f32 param_4) {
        Mtx amStack_30;
        getRotation_xyz(amStack_30, param_2, param_3, param_4);
        MTXConcat(amStack_30, param_0, param_1);
    }

    inline void rotate_xyz(Mtx param_0, Mtx param_1, const Vec& param_2) {
        rotate_xyz(param_0, param_1, param_2.x, param_2.y, param_2.z);
    }

    inline f32 getFromTransformation_Sn(CMtxP param_1, u32 param_2) {
        Vec local_18;
        local_18.x = param_1[0][param_2];
        local_18.y = param_1[1][param_2];
        local_18.z = param_1[2][param_2];
        return VECMag(&local_18);
    }

    inline void getFromTransformation_S(Vec* param_1, CMtxP param_2) {
        param_1->x = getFromTransformation_Sn(param_2, 0);
        param_1->y = getFromTransformation_Sn(param_2, 1);
        param_1->z = getFromTransformation_Sn(param_2, 2);
    }

    inline void getFromTransformation_T(Vec* param_1, CMtxP param_2) {
        param_1->x = param_2[0][3];
        param_1->y = param_2[1][3];
        param_1->z = param_2[2][3];
    }

    inline void getTransformation_T(Mtx mtx, const Vec* param_2) {
        MTXTrans(mtx, param_2->x, param_2->y, param_2->z);
    }
};
};  // namespace JStudio

#endif /* JSTUDIO_MATH_H */
