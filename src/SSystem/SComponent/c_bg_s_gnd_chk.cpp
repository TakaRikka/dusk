/**
 * c_bg_s_gnd_chk.cpp
 *
 */

#include "SSystem/SComponent/c_bg_s_gnd_chk.h"
#include "f_pc/f_pc_manager.h"

cBgS_GndChk::cBgS_GndChk() {
    m_pos = cXyz::Zero;
    SetActorPid(fpcM_ERROR_PROCESS_ID_e);
    mFlags = 2;
}

cBgS_GndChk::~cBgS_GndChk() {}

void cBgS_GndChk::SetPos(Vec const* pos) {
    m_pos = *pos;
}

void cBgS_GndChk::SetPos(cXyz const* pos) {
    m_pos = *pos;
}

void cBgS_GndChk::PreCheck() {
    mWallPrecheck = mFlags & 2;
}
