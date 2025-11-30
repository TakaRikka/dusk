/**
 * d_bg_w_base.cpp
 *
 */

#include "d/dolzel.h" // IWYU pragma: keep

#include "d/d_bg_w_base.h"

dBgW_Base::dBgW_Base() {
    ClrDBgWBase();
}

dBgW_Base::~dBgW_Base() {}

void dBgW_Base::ClrDBgWBase() {
    m_roomId = 0xFF;
    m_priority = 2;
    field_0xa = 0xFF;
    m_diff_ShapeAngleY = 0;
    m_old_ShapeAngleY = 0;
    m_pushPull_Callback = NULL;
    m_pushPull_Ok = true;
    field_0xb = 0;
}

bool dBgW_Base::ChkMemoryError() {
    return false;
}

void dBgW_Base::CallRideCallBack(fopAc_ac_c* param_0, fopAc_ac_c* param_1) {}

void dBgW_Base::CallArrowStickCallBack(fopAc_ac_c* param_0, fopAc_ac_c* param_1, cXyz& param_2) {}

void dBgW_Base::CalcDiffShapeAngleY(s16 param_0) {
    m_diff_ShapeAngleY = param_0 - m_old_ShapeAngleY;
    m_old_ShapeAngleY = param_0;
}
