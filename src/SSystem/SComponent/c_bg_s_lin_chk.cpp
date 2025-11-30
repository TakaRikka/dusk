/**
 * c_bg_s_lin_chk.cpp
 *
 */

#include "SSystem/SComponent/c_bg_s_lin_chk.h"
#include "f_pc/f_pc_manager.h"

cBgS_LinChk::cBgS_LinChk() {
    ct();
}

cBgS_LinChk::~cBgS_LinChk() {}

void cBgS_LinChk::ct() {
    cXyz start_end(cXyz::Zero);
    mLin.SetStartEnd(start_end, start_end);
    field_0x40 = start_end;

    SetActorPid(fpcM_ERROR_PROCESS_ID_e);
    mStts = 0;
    mFrontFlag = 1;
    mBackFlag = 0;
}

void cBgS_LinChk::Set2(cXyz const* startPos, cXyz const* endPos, unsigned int actorPid) {
    mLin.SetStartEnd(*startPos, *endPos);
    field_0x40 = *endPos;
    SetActorPid(actorPid);
    mStts &= ~STTS_HIT;
    ClearPi();
}

void cBgS_LinChk::PreCalc() {
    mPreWallChk = !(mStts & STTS_WALL_OFF);
    mPreGroundChk = !(mStts & STTS_GROUND_OFF);
    mPreRoofChk = !(mStts & STTS_ROOF_OFF);
}
