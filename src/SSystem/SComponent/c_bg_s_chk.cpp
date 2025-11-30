/**
 * c_bg_s_chk.cpp
 *
 */

#include "SSystem/SComponent/c_bg_s_chk.h"
#include "f_pc/f_pc_manager.h"

cBgS_Chk::cBgS_Chk() {
    mPolyPassChk = NULL;
    mGrpPassChk = NULL;
    mSameActorChk = true;
}

cBgS_Chk::~cBgS_Chk() {}

void cBgS_Chk::SetExtChk(cBgS_Chk& other) {
    mPolyPassChk = other.mPolyPassChk;
    mGrpPassChk = other.mGrpPassChk;
    mActorPid = other.mActorPid;
    mSameActorChk = other.mSameActorChk;
}

bool cBgS_Chk::ChkSameActorPid(fpc_ProcID pid) const {
    if (mActorPid == fpcM_ERROR_PROCESS_ID_e || pid == fpcM_ERROR_PROCESS_ID_e || !mSameActorChk) {
        return FALSE;
    } else if (mActorPid == pid) {
        return TRUE;
    } else {
        return FALSE;
    }
}
