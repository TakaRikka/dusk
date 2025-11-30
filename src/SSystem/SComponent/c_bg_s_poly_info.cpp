/**
 * c_bg_s_poly_info.cpp
 *
 */

#include "SSystem/SComponent/c_bg_s_poly_info.h"
#include "JSystem/JUtility/JUTAssert.h"

cBgS_PolyInfo::cBgS_PolyInfo() {
    ClearPi();
}

cBgS_PolyInfo::~cBgS_PolyInfo() {}

bool cBgS_PolyInfo::ChkSetInfo() const {
    if (mPolyIndex == 0xFFFF || mBgIndex == 0x100) {
        return false;
    } else {
        return true;
    }
}

void cBgS_PolyInfo::ClearPi() {
    mPolyIndex = -1;
    mBgIndex = 0x100;
    unk_0x04 = 0;
    mActorId = -1;
}

void cBgS_PolyInfo::SetPolyInfo(const cBgS_PolyInfo& poly) {
    *this = poly;
}

void cBgS_PolyInfo::SetActorInfo(int bg_index, void* p_data, fpc_ProcID actor_id) {
    JUT_ASSERT(74, 0 <= bg_index);
    mBgIndex = bg_index;
    unk_0x04 = p_data;
    mActorId = actor_id;
}

bool cBgS_PolyInfo::ChkSafe(const void* param_1, fpc_ProcID actor_id) const {
    if (unk_0x04 == param_1 && mActorId == actor_id) {
        return true;
    } else {
        return false;
    }
}

void cBgS_PolyInfo::SetPolyIndex(int poly_index) {
    JUT_ASSERT(103, 0 <= poly_index);
    mPolyIndex = poly_index;
}

bool cBgS_PolyInfo::ChkBgIndex() const {
    if (mBgIndex == 0x100) {
        return false;
    }

    return true;
}
