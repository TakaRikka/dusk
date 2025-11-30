/**
 * c_bg_w.cpp
 *
 */

#include "SSystem/SComponent/c_bg_w.h"

void cBgW_BgId::Regist(int id) {
    m_id = id;
}

void cBgW_BgId::Release() {
    m_id = 0x100;
}

bool cBgW_BgId::ChkUsed() const {
    if (m_id < 0x100) {
        return true;
    }

    return false;
}

bool cBgW_CheckBGround(float y) {
    if (y >= 0.5f) {
        return true;
    } else {
        return false;
    }
}

bool cBgW_CheckBRoof(float y) {
    if (y < (-4.0f / 5.0f)) {
        return true;
    } else {
        return false;
    }
}

bool cBgW_CheckBWall(float y) {
    if (!cBgW_CheckBGround(y) && !cBgW_CheckBRoof(y))
        return true;

    return false;
}
