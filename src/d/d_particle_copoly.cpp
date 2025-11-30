/**
 * d_particle_copoly.cpp
 *
 */

#include "d/dolzel.h" // IWYU pragma: keep

#include "d/d_particle_copoly.h"
#include "f_op/f_op_kankyo_mng.h"

void dPaPo_c::init(dBgS_Acch* param_0, f32 i_waterOffset, f32 i_rippleOffset) {
    field_0x28 = param_0;
    mWaterOffset = i_waterOffset;
    mRippleOffset = i_rippleOffset;

    field_0x28->ClrWaterNone();
    field_0x37 = 1;
}

int dPaPo_c::checkWaterIn(cXyz* param_0, cXyz const* param_1, dKy_tevstr_c const* param_2,
                          u32 param_3, s8 param_4, f32 param_5, f32 param_6) {
    if (field_0x28->ChkWaterHit() && dComIfG_Bgsp().ChkPolySafe(field_0x28->m_wtr) &&
        dComIfG_Bgsp().GetPolyAtt0(field_0x28->m_wtr) != 6)
    {
        f32 wtr_height = field_0x28->m_wtr.GetHeight();
        f32 var_f31 = wtr_height - param_0->y;

        if (var_f31 >= 0.0f) {
            param_0->y = wtr_height;
            if (!field_0x37) {
                field_0x37 = 1;

                if (!(param_3 & 0x40000) &&
                    field_0x28->m_wtr.GetHeight() - field_0x28->GetGroundH() >= mWaterOffset)
                {
                    fopKyM_createWpillar(param_0, param_5, 0);
                }
            }

            if (var_f31 < mRippleOffset && !(param_3 & 0x20000)) {
                dComIfGp_particle_setWaterRipple(field_0x0, field_0x28->m_wtr, param_0, param_6,
                                                 param_2, param_1, param_4);
            }

            if (var_f31 < mWaterOffset) {
                return 1;
            }

            param_0->y -= var_f31;
            return 2;
        }

        field_0x37 = 0;
    } else {
        field_0x37 = 0;
    }

    return 0;
}

void dPaPo_c::setEffect(u32* param_0, int param_1, dKy_tevstr_c const* param_2, cXyz const* param_3,
                        u32 param_4, u32 param_5, cXyz const* param_6, csXyz const* param_7,
                        s8 param_8, u8* param_9, u8* param_10) {
    if (param_6 != NULL) {
        cXyz sp18(*param_6);

        cBgS_PolyInfo* poly_p;
        if (param_1 == 1) {
            sp18.y = field_0x28->m_wtr.GetHeight();
            poly_p = &field_0x28->m_wtr;
        } else {
            if (param_1 == 2) {
                param_5 |= 0x80000000;
            }
            poly_p = &field_0x28->m_gnd;
        }

        u16 temp_r29 = dComIfGp_particle_setCommonPoly(param_0, poly_p, param_3, &sp18, param_2,
                                                       param_4, param_5, param_7, NULL, param_8);
        param_9[0] = temp_r29 & 0xFF;
        param_9[1] = (temp_r29 >> 8) & 0xFF;

        if (param_9[0] != 0xFF) {
            if (param_4 & (1 << param_9[0])) {
                *param_10 = 0;
            } else {
                *param_10 = 20;
            }
        } else if (param_9[1] != 0xFF) {
            if (param_4 & (1 << param_9[1])) {
                *param_10 = 0;
            } else {
                *param_10 = 20;
            }
        }
    } else if (*param_10 != 0) {
        *param_10 -= 1;

        u32* var_r31 = param_0;
        for (int i = 0; i < 8; i++) {
            if (*var_r31 != 0) {
                *var_r31 = dComIfGp_particle_setStopContinue(*var_r31);
            }

            var_r31++;
        }
    }
}

int dPaPo_c::setEffectCenter(dKy_tevstr_c const* param_0, cXyz const* param_1, u32 param_2,
                             u32 param_3, cXyz const* param_4, csXyz const* param_5,
                             cXyz const* param_6, s8 param_7, f32 param_8, f32 param_9) {
    cXyz sp18(*param_1);

    int rt = checkWaterIn(&sp18, param_6, param_0, param_3, param_7, param_8, param_9);
    mCenterEffType[0] = 0xFF;
    mCenterEffType[1] = 0xFF;

    if (rt == 1 && (param_3 & 0x10000)) {
        setEffect(field_0x8[0], rt, param_0, &sp18, param_2, param_3, &sp18, param_5, param_7,
                  &mCenterEffType[0], &field_0x36);

        if (mCenterEffType[0] == 2) {
            mCenterEffType[0] = 16;
            field_0x36 = 20;
        }

        return 3;
    } else {
        setEffect(field_0x8[0], rt, param_0, &sp18, param_2, param_3, param_4, param_5, param_7,
                  &mCenterEffType[0], &field_0x36);
    }

    return rt;
}

void dPaPo_c::clearID(u32* param_0, u8* param_1) {
    *param_1 = 0;

    for (int i = 0; i < 8; i++) {
        *param_0 = 0;
        param_0++;
    }
}

int dPaPoT_c::setEffectTwo(dKy_tevstr_c const* param_0, cXyz const* param_1, u32 param_2,
                           u32 param_3, cXyz const* param_4, cXyz const* param_5,
                           cXyz const* param_6, csXyz const* param_7, cXyz const* param_8,
                           s8 param_9, f32 param_10, f32 param_11) {
    mLeftEffType[0] = 0xFF;
    mLeftEffType[1] = 0xFF;
    mRightEffType[0] = 0xFF;
    mRightEffType[1] = 0xFF;

    int rt = setEffectCenter(param_0, param_1, param_2, param_3, param_4, param_7, param_8, param_9,
                             param_10, param_11);
    if (rt != 3) {
        cXyz sp18(*param_1);
        if (rt == 1) {
            sp18.y = field_0x28->m_wtr.GetHeight();
        }

        setEffect(mLeftEmitter[0], rt, param_0, &sp18, param_2, param_3, param_5, param_7, param_9,
                  mLeftEffType, &field_0x7c);
        setEffect(mRightEmitter[0], rt, param_0, &sp18, param_2, param_3, param_6, param_7, param_9,
                  mRightEffType, &field_0x7d);
    } else {
        field_0x7c = 0;
        field_0x7d = 0;
    }

    return rt;
}

void dPaPoT_c::clearTwoAllID() {
    clearLeftID();
    clearRightID();
    clearCenterID();
}

/* 800512E8-80051424 04BC28 013C+00 0/0 1/1 1/1 .text
 * setEffectFour__8dPaPoF_cFPC12dKy_tevstr_cPC4cXyzUlUlPC4cXyzPC4cXyzPC4cXyzPC4cXyzPC4cXyzPC5csXyzPC4cXyzScff
 */
int dPaPoF_c::setEffectFour(dKy_tevstr_c const* param_0, cXyz const* param_1, u32 param_2,
                            u32 param_3, cXyz const* param_4, cXyz const* param_5,
                            cXyz const* param_6, cXyz const* param_7, cXyz const* param_8,
                            csXyz const* param_9, cXyz const* param_10, s8 param_11, f32 param_12,
                            f32 param_13) {
    mBackLeftEffType[0] = 0xFF;
    mBackLeftEffType[1] = 0xFF;
    mBackRightEffType[0] = 0xFF;
    mBackRightEffType[1] = 0xFF;

    int rt = setEffectTwo(param_0, param_1, param_2, param_3, param_4, param_5, param_6, param_9,
                          param_10, param_11, param_12, param_13);
    if (rt != 3) {
        cXyz sp18(*param_1);
        if (rt == 1) {
            sp18.y = field_0x28->m_wtr.GetHeight();
        }

        setEffect(mBackLeftEmitter[0], rt, param_0, &sp18, param_2, param_3, param_7, param_9,
                  param_11, mBackLeftEffType, &field_0xc4);
        setEffect(mBackRightEmitter[0], rt, param_0, &sp18, param_2, param_3, param_8, param_9,
                  param_11, mBackRightEffType, &field_0xc5);
    } else {
        field_0xc4 = 0;
        field_0xc5 = 0;
    }

    return rt;
}

void dPaPoF_c::clearFourAllID() {
    clearBLeftID();
    clearBRightID();
    clearTwoAllID();
}

dPaPoT_c::effTypeFunc dPaPoT_c::m_typeTwoData[] = {
    &dPaPoT_c::getCenterEffType,
    &dPaPoT_c::getLeftEffType,
    &dPaPoT_c::getRightEffType,
};

dPaPoT_c::emitterFunc dPaPoT_c::m_emitterTwoData[] = {
    &dPaPoT_c::getCenterEmitter,
    &dPaPoT_c::getLeftEmitter,
    &dPaPoT_c::getRightEmitter,
};

dPaPoF_c::effTypeFunc dPaPoF_c::m_typeFourData[] = {
    &dPaPoF_c::getCenterEffType,   &dPaPoF_c::getLeftEffType,      &dPaPoF_c::getRightEffType,
    &dPaPoF_c::getBackLeftEffType, &dPaPoF_c::getBackRightEffType,
};

dPaPoF_c::emitterFunc dPaPoF_c::m_emitterFourData[] = {
    &dPaPoF_c::getCenterEmitter,   &dPaPoF_c::getLeftEmitter,      &dPaPoF_c::getRightEmitter,
    &dPaPoF_c::getBackLeftEmitter, &dPaPoF_c::getBackRightEmitter,
};
