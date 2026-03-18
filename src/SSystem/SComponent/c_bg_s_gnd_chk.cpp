/**
 * c_bg_s_gnd_chk.cpp
 *
 */

#include "SSystem/SComponent/c_bg_s_gnd_chk.h"
#include "f_pc/f_pc_manager.h"
#include "JSystem/JUtility/JUTAssert.h"

#define CHECK_FLOAT_CLASS(line, x) JUT_ASSERT(line, !isnan(x));
#define CHECK_FLOAT_RANGE(line, x) JUT_ASSERT(line, -INF < x && x < INF);
#define CHECK_FLOAT_MIN(line, x) JUT_ASSERT(line, -INF < x);
#define CHECK_FLOAT_MAX(line, x) JUT_ASSERT(line, x < INF);

cBgS_GndChk::cBgS_GndChk() {
    m_pos = cXyz::Zero;
    SetActorPid(fpcM_ERROR_PROCESS_ID_e);
    mFlags = 2;
}

cBgS_GndChk::~cBgS_GndChk() {}

void cBgS_GndChk::SetPos(Vec const* pos) {
    m_pos.x = pos->x;
    m_pos.y = pos->y;
    m_pos.z = pos->z;

    CHECK_FLOAT_CLASS(37, m_pos.x);
    CHECK_FLOAT_CLASS(38, m_pos.y);
    CHECK_FLOAT_CLASS(39, m_pos.z);

    CHECK_FLOAT_RANGE(43, m_pos.x);
    CHECK_FLOAT_MIN(44, m_pos.y);
    CHECK_FLOAT_MAX(45, m_pos.y);
    CHECK_FLOAT_RANGE(46, m_pos.z);
}

void cBgS_GndChk::SetPos(cXyz const* pos) {
    m_pos = *pos;

    CHECK_FLOAT_CLASS(59, m_pos.x);
    CHECK_FLOAT_CLASS(60, m_pos.y);
    CHECK_FLOAT_CLASS(61, m_pos.z);

    CHECK_FLOAT_RANGE(65, m_pos.x);
    CHECK_FLOAT_MIN(66, m_pos.y);
    CHECK_FLOAT_MAX(67, m_pos.y);
    CHECK_FLOAT_RANGE(68, m_pos.z);
}

void cBgS_GndChk::PreCheck() {
    mWallPrecheck = mFlags & 2;
}
