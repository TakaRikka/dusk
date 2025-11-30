#ifndef D_D_JNT_COL_H
#define D_D_JNT_COL_H

#include "JSystem/J3DGraphAnimator/J3DModel.h"
#include "SSystem/SComponent/c_m3d_g_lin.h"
#include "SSystem/SComponent/c_sxyz.h"

class fopAc_ac_c;

class dJntColData_c {
public:
    /* 0x0 */ u8 field_0x0;
    /* 0x1 */ u8 mType;
    /* 0x2 */ s16 mJntNum;
    /* 0x4 */ f32 field_0x4;
    /* 0x8 */ Vec* field_0x8;
};

class dJntCol_c {
public:
    dJntCol_c();
    int init(fopAc_ac_c*, dJntColData_c const*, J3DModel*, int);
    static void setNowLine(cM3dGLin*, cXyz const*, csXyz const*, cXyz*);
    int searchNearPos(cM3dGLin const*, cXyz const*, cXyz*, int) const;
    int getArrowOffsetPosAndAngle(cXyz const*, csXyz const*, cXyz*, cXyz*) const;
    int getHitmarkPosAndAngle(cXyz const*, csXyz const*, cXyz*, csXyz*, int) const;
    void setArrowPosAndAngle(cXyz const*, cXyz const*, int, cXyz*, csXyz*);

    bool checkPassNum(int bit) { return field_0xc & (1 << bit); }
    void onPassNum(int num) { field_0xc |= (1 << num); }
    void offPassNum(int num) { field_0xc &= ~(1 << num); }
    bool checkShieldType(int i) { return getType(i) == 3; }

    s8 getType(int i) { return mData[i].mType; }
    int getJntNum(int i) { return mData[i].mJntNum; }

    /* 0x00 */ const dJntColData_c* mData;
    /* 0x04 */ J3DModel* mModel;
    /* 0x08 */ int field_0x8;
    /* 0x0C */ int field_0xc;
};

#endif /* D_D_JNT_COL_H */
