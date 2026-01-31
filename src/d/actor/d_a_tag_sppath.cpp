/**
 * d_a_tag_sppath.cpp
 * Tag - Spinner Path
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_tag_sppath.h"
#include "d/actor/d_a_player.h"
#include <cmath>

static dCcD_SrcCyl l_cylSrc = {
    {
        {0x0, {{0x0, 0x0, 0x0}, {0x80000, 0x11}, 0x0}},  // mObj
        {dCcD_SE_STONE, 0x0, 0x0, 0x0, 0x0},             // mGObjAt
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x4},              // mGObjTg
        {0x0},                                           // mGObjCo
    },                                                   // mObjInf
    {
        {
            {0.0f, 0.0f, 0.0f},  // mCenter
            30.0f,               // mRadius
            100.0f               // mHeight
        }                        // mCyl
    }
};

int daTagSppath_c::create() {
    fopAcM_ct(this, daTagSppath_c);

    mSwNo1 = (fopAcM_GetParam(this) >> 8) & 0xFF;
    mSwNo2 = (fopAcM_GetParam(this) >> 16) & 0xFF;
    s32 pathIndex = fopAcM_GetParam(this) & 0xff;

    if (pathIndex == 0xff ||
        (mpInitPath = dPath_GetRoomPath(pathIndex, fopAcM_GetRoomNo(this))) == NULL)
    {
        return cPhs_ERROR_e;
    }

    mStts.Init(0, 0xff, this);
    mCyl.Set(l_cylSrc);
    mCyl.SetStts(&mStts);

    if (strcmp(dComIfGp_getStartStageName(), "D_MN10A") == 0) {
        mHeight = 600.0;
        mCyl.SetH(mHeight);
    } else {
        mHeight = 100.0;
    }

    return cPhs_COMPLEATE_e;
}

static int daTagSppath_Create(fopAc_ac_c* i_this) {
    daTagSppath_c* spPath = static_cast<daTagSppath_c*>(i_this);
    int id = fopAcM_GetID(i_this);
    return spPath->create();
}

daTagSppath_c::~daTagSppath_c() {}

static int daTagSppath_Delete(daTagSppath_c* i_this) {
    int id = fopAcM_GetID(i_this);
    i_this->~daTagSppath_c();
    return 1;
}

int daTagSppath_c::getNearPathPos(cXyz* i_result, dPath* i_path) {
    cXyz* pyCurPos = &daPy_getPlayerActorClass()->current.pos;
    dPnt* curPnt = i_path->m_points;
    f32 bestDist = FLT_MAX;

    f32 distToSeg;
    BOOL isValid;
    cM3dGLin line;
    cXyz nearestPointOnSeg;
    cXyz pntDiff;
    int bestNo;
    f32 flatness;
    f32 segLen;
    dPnt* adjPnt;

    for (int i = 0; i < i_path->m_num; i++, curPnt++) {
        if (i != i_path->m_num - 1) {
            line.set(curPnt[0].m_position, curPnt[1].m_position);
            if (!dPath_ChkClose(i_path)) {
                adjPnt = curPnt + 1;
                if (i == 0) {
                    pntDiff.set(adjPnt->m_position.x - curPnt->m_position.x,
                                adjPnt->m_position.y - curPnt->m_position.y,
                                adjPnt->m_position.z - curPnt->m_position.z);
                    segLen = pntDiff.abs();
                    pntDiff.normalizeZP();
                    flatness = pntDiff.absXZ();
                    if (flatness > 0.001f) {
                        flatness = 30.0f / flatness;
                        if (segLen > flatness) {
                            line.GetStartP() = line.GetStart() + pntDiff * flatness;
                        }
                    }
                }
                if (i == i_path->m_num - 2) {
                    pntDiff.set(curPnt->m_position.x - adjPnt->m_position.x,
                                curPnt->m_position.y - adjPnt->m_position.y,
                                curPnt->m_position.z - adjPnt->m_position.z);
                    segLen = pntDiff.abs();
                    pntDiff.normalizeZP();
                    flatness = pntDiff.absXZ();
                    if (flatness > 0.001f) {
                        flatness = 30.0f / flatness;
                        if (segLen > flatness) {
                            line.GetEndP() = line.GetEnd() + pntDiff * flatness;
                        }
                    }
                }
            }
            isValid = cM3d_Len3dSqPntAndSegLine(&line, pyCurPos, &nearestPointOnSeg, &distToSeg);
        } else {
            if (dPath_ChkClose(i_path)) {
                line.set(curPnt->m_position, i_path->m_points[0].m_position);
                isValid =
                    cM3d_Len3dSqPntAndSegLine(&line, pyCurPos, &nearestPointOnSeg, &distToSeg);
            } else {
                isValid = 0;
            }
        }

        if (isValid == 0) {
            nearestPointOnSeg = curPnt->m_position;
            if (!dPath_ChkClose(i_path) && ((i == 0 || (i == i_path->m_num - 1)))) {
                if (i == 0) {
                    adjPnt = curPnt + 1;
                } else {
                    adjPnt = curPnt - 1;
                }

                pntDiff.set(adjPnt->m_position.x - curPnt->m_position.x,
                            adjPnt->m_position.y - curPnt->m_position.y,
                            adjPnt->m_position.z - curPnt->m_position.z);
                segLen = pntDiff.abs();
                pntDiff.normalizeZP();
                flatness = pntDiff.absXZ();
                if (flatness > 0.001f) {
                    flatness = 30.0f / flatness;
                    if (segLen > flatness) {
                        nearestPointOnSeg += pntDiff * flatness;
                    }
                }
            }
        }

        distToSeg = pyCurPos->abs2(nearestPointOnSeg);
        if (bestDist > distToSeg) {
            bestDist = distToSeg;
            bestNo = i;
            *i_result = nearestPointOnSeg;
        }
    }

    return bestNo;
}

int daTagSppath_c::execute() {
    daPy_py_c* py = daPy_getPlayerActorClass();

    if (py->checkSpinnerRide() == 0 || (mSwNo1 != 0xff && !fopAcM_isSwitch(this, mSwNo1)) ||
        (mSwNo2 != 0xff && fopAcM_isSwitch(this, mSwNo2)))
    {
        return 1;
    }

    dPath* path = mpInitPath;
    cXyz nearestPointOnPath;
    f32 bestDist = FLT_MAX;
    f32 dist;
    int no;

    for (; path != NULL; path = dPath_GetNextRoomPath(path, fopAcM_GetRoomNo(this))) {
        no = getNearPathPos(&nearestPointOnPath, path);
        dist = nearestPointOnPath.abs2(py->current.pos);
        if (dist < bestDist) {
            bestDist = dist;
            current.pos = nearestPointOnPath;
            mpBestPath = path;
            mBestNo = no;
        }
    }

    if (mpBestPath->field_0x4 == 0) {
        if (mpBestPath->field_0x6 == 0xff || fopAcM_isSwitch(this, mpBestPath->field_0x6) == 0) {
            field_0x6e8 = 1;
        } else {
            field_0x6e8 = 2;
        }
    } else {
        field_0x6e8 = 0;
    }

    cXyz sp10(current.pos.x, current.pos.y - mHeight * 0.5f, current.pos.z);
    mCyl.SetC(sp10);
    dComIfG_Ccsp()->Set(&mCyl);

    return 1;
}

static int daTagSppath_Execute(daTagSppath_c* tag) {
    return tag->execute();
}

static int daTagSppath_Draw(daTagSppath_c* i_this) {
    return 1;
}

static actor_method_class l_daTagSppath_Method = {
    (process_method_func)daTagSppath_Create,  (process_method_func)daTagSppath_Delete,
    (process_method_func)daTagSppath_Execute, (process_method_func)NULL,
    (process_method_func)daTagSppath_Draw,
};

actor_process_profile_definition g_profile_Tag_Sppath = {
    fpcLy_CURRENT_e,         // mLayerID
    7,                       // mListID
    fpcPi_CURRENT_e,         // mListPrio
    PROC_Tag_Sppath,         // mProcName,
    &g_fpcLf_Method.base,   // sub_method
    sizeof(daTagSppath_c),   // mSize
    0,                       // mSizeOther
    0,                       // mParameters
    &g_fopAc_Method.base,    // sub_method
    258,                     // mPriority
    &l_daTagSppath_Method,   // sub_method
    0x44000,                 // mStatus
    fopAc_ENV_e,             // mActorType
    fopAc_CULLBOX_CUSTOM_e,  // cullType
};
