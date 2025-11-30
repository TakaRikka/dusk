/**
* @file d_a_obj_laundry_rope.cpp
 *
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_laundry_rope.h"
#include "SSystem/SComponent/c_lib.h"
#include "SSystem/SComponent/c_math.h"
#include "d/d_cc_d.h"
#include "d/d_com_inf_game.h"
#include "d/d_kankyo_wether.h"
#include "d/d_path.h"

daObjLndRope_Attr_c const daObjLndRope_c::mAttr = {-5.0f, 0.2f, 0.5f, 0.8f, 0.0f};

static const s8 l_laundryList[13] = {-1, -1, 3, 0, -1, 1, 1, -1, 2, 2, -1, 2, -1};

u8 daObjLndRope_c::getPathId() {
    return fopAcM_GetParamBit(this, 8, 8);
}

void daObjLndRope_c::create_init() {
    gravity = attr()->mGravity;
    dPath* roomPath = dPath_GetRoomPath(getPathId(), fopAcM_GetRoomNo(this));

    dPnt* pathPoints = roomPath->m_points;
    cXyz startPoint = pathPoints[0].m_position;
    cXyz endPoint = pathPoints[1].m_position;
    current.pos = (startPoint + endPoint) * 0.5f;
    mPos = endPoint - startPoint;

    if (fabsf(mPos.x) > fabsf(mPos.z)) {
        mScale = 1.0f / mPos.x;
        mFlag = 1;
    } else {
        mScale = 1.0f / mPos.z;
        mFlag = 0;
    }

    shape_angle.y = cM_atan2s(mPos.x, mPos.z);
    shape_angle.x = cM_atan2s(-mPos.y, mPos.absXZ());
    f32 cullSizeBoxZ = mPos.absXZ() * 0.5f + 3.0f;
    fopAcM_setCullSizeBox(this, -75.0f, -200.0f, -cullSizeBoxZ, 75.0f, 50.0f, cullSizeBoxZ);

    f32 totalLength = mPos.abs();
    const f32 segmentFraction = 1.0f / 14.0f;
    mSegmentLength = totalLength * segmentFraction;
    cXyz segmentOffset = mPos * (mSegmentLength / totalLength);

    cXyz* ropePosition = mRopeMat.getPos(0);
    cXyz* loopPos = &mPositions2[0];
    *ropePosition = startPoint;
    ropePosition++;

    for (int i = 1; i < 14; i++) {
        *ropePosition = *(ropePosition - 1) + segmentOffset;
        *loopPos = *ropePosition;
        ropePosition++;
        loopPos++;
    }

    *ropePosition = endPoint;
    setNormalRopePos();
    initBaseMtx();

    fpc_ProcID* procId = &mProcIds[0];
    ropePosition = mRopeMat.getPos(0) + 1;
    const s8* laundryEntry = &l_laundryList[0];

    for (int i = 1; i < 14; i++) {
        if (*laundryEntry == -1) {
            *procId = fpcM_ERROR_PROCESS_ID_e;
        } else {
            csXyz childAngle(0, cLib_targetAngleY(ropePosition, ropePosition + 1) + 0x4000,
                             -cLib_targetAngleX(ropePosition, ropePosition + 1));
            *procId =
                fopAcM_createChild(PROC_Obj_Laundry, fopAcM_GetID(this), *laundryEntry,
                                   ropePosition, fopAcM_GetRoomNo(this), &childAngle, NULL, -1, 0);
        }

        ropePosition++;
        procId++;
        laundryEntry++;
    }
}

void daObjLndRope_c::initBaseMtx() {
    setBaseMtx();
    fopAcM_SetMtx(this, mMtx);
}

void daObjLndRope_c::setBaseMtx() {
    mDoMtx_trans(mMtx, current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_ZXYrotM(mMtx, shape_angle.x, shape_angle.y, 0);
}

const static dCcD_SrcSph ccSphSrc = {
    {
        {0x0, {{0x0, 0x0, 0x0}, {0x1607a, 0x11}, 0x149}},  // mObj
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0},                // mGObjAt
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x4},                // mGObjTg
        {0x0},                                             // mGObjCo
    },                                                     // mObjInf
    {
        {{0.0f, 0.0f, 0.0f}, 0.0f}  // mSph
    }  // mSphAttr
};

cXyz* daObjLndRope_c::getRopeStartPos() {
    return mRopeMat.getPos(0);
}

f32 daObjLndRope_c::getStartRate(cXyz* i_ropeSegmentPos) {
    if (mFlag != NULL) {
        return mScale * (i_ropeSegmentPos->x - getRopeStartPos()->x);
    } else {
        return mScale * (i_ropeSegmentPos->z - getRopeStartPos()->z);
    }
}

void daObjLndRope_c::setNormalRopePos() {
    cXyz* ropePos = mRopeMat.getPos(0) + 1;
    cXyz* pos1 = mPositions1;
    cXyz* pos2 = mPositions2;

    cXyz posModifier;
    cXyz windPow = dKyw_get_AllWind_vecpow(&current.pos);

    if (cM_rnd() < 0.1f) {
        windPow *= mSegmentLength * attr()->field_0x4;
    } else {
        windPow.zero();
    }

    int i;
    for (i = 1; i < 14;) {
        ropePos->y = pos2->y;
        posModifier = *ropePos - *(ropePos - 1);
        posModifier.y += gravity;
        posModifier += *pos1;

        if (cM_rnd() < 0.6f) {
            posModifier += windPow;
        }

        posModifier.normalizeZP();
        *ropePos = *(ropePos - 1) + (posModifier * mSegmentLength);

        i++;
        ropePos++;
        pos2++;
        pos1++;
    }

    ropePos = mRopeMat.getPos(0) + 13;
    pos1 = &mPositions1[12];
    for (i = 13; i > 0;) {
        posModifier = *ropePos - *(ropePos + 1);
        posModifier.y += gravity;
        posModifier += *pos1;
        posModifier.normalizeZP();
        *ropePos = *(ropePos + 1) + (posModifier * mSegmentLength);

        i--;
        ropePos--;
        pos1--;
    }

    ropePos = mRopeMat.getPos(0) + 1;
    pos1 = mPositions1;
    pos2 = mPositions2;
    cXyz* current = mRopeMat.getPos(0);

    for (i = 1; i < 14;) {
        f32 adjustedRopeY = current->y + mPos.y * getStartRate(ropePos);

        *pos1 = (*ropePos - *pos2) * attr()->field_0x8;
        *pos2 = *ropePos;
        ropePos->y += attr()->field_0xc * (adjustedRopeY - ropePos->y);

        i++;
        ropePos++;
        pos2++;
        pos1++;
    }

    ropePos = mRopeMat.getPos(0) + 1;
    fpc_ProcID* points = mProcIds;
    for (i = 1; i < 14;) {
        if (*points != -1) {
            fopAc_ac_c* actor = (fopAc_ac_c*)fpcM_SearchByID(*points);
            if (actor != NULL) {
                cXyz* actorPos = fopAcM_GetPosition_p(actor);
                csXyz* actorAngle = fopAcM_GetShapeAngle_p(actor);
                cXyz temp = *(ropePos + 1) - *ropePos;
                *actorPos = *ropePos;
                actorAngle->y = cLib_targetAngleY(ropePos, ropePos + 1) + 0x4000;
                actorAngle->z = -cLib_targetAngleX(ropePos, ropePos + 1);
            }
        }

        i++;
        ropePos++;
        points++;
    }
}

static char* l_arcName = "Obj_SRope";

int daObjLndRope_c::createHeap() {
    ResTIMG* resTimg = (ResTIMG*)dComIfG_getObjectRes(l_arcName, "Obj_ItaRope.bti");
    int result = mRopeMat.init(1, 15, resTimg, 1);
    if (result == 0) {
        return 0;
    }

    f32* ropeSize = mRopeMat.getSize(0);
    for (int i = 0; i < 15; i++) {
        *ropeSize = 2.0f;
        ropeSize++;
    }

    return 1;
}

static int createSolidHeap(fopAc_ac_c* i_this) {
    return static_cast<daObjLndRope_c*>(i_this)->createHeap();
}

int daObjLndRope_c::draw() {
    static _GXColor l_color = {20, 15, 0, 255};
    g_env_light.settingTevStruct(16, &current.pos, &tevStr);
    mRopeMat.update(15, l_color, &tevStr);
    dComIfGd_set3DlineMat(&mRopeMat);
    return 1;
}

static int daObjLndRope_Draw(daObjLndRope_c* i_this) {
    return i_this->draw();
}

int daObjLndRope_c::execute() {
    gravity = attr()->mGravity;
    setNormalRopePos();
    return 1;
}

static int daObjLndRope_Execute(daObjLndRope_c* i_this) {
    return i_this->execute();
}

static int daObjLndRope_IsDelete(daObjLndRope_c* i_this) {
    return 1;
}

daObjLndRope_c::~daObjLndRope_c() {
    dComIfG_resDelete(&mPhase, l_arcName);
}

static int daObjLndRope_Delete(daObjLndRope_c* i_this) {
    i_this->~daObjLndRope_c();
    return 1;
}

int daObjLndRope_c::create() {
    fopAcM_ct(this, daObjLndRope_c);
    int phase = dComIfG_resLoad(&mPhase, l_arcName);
    if (phase == cPhs_COMPLEATE_e) {
        if (!fopAcM_entrySolidHeap(this, createSolidHeap, 0x6c0)) {
            phase = cPhs_ERROR_e;
        } else {
            create_init();
        }
    }

    return phase;
}

static int daObjLndRope_Create(fopAc_ac_c* i_this) {
    return ((daObjLndRope_c*)i_this)->create();
}

static actor_method_class l_daObjLndRope_Method = {
    (process_method_func)daObjLndRope_Create,  (process_method_func)daObjLndRope_Delete,
    (process_method_func)daObjLndRope_Execute, (process_method_func)daObjLndRope_IsDelete,
    (process_method_func)daObjLndRope_Draw,
};

extern actor_process_profile_definition g_profile_Obj_LndRope = {
    fpcLy_CURRENT_e,         // mLayerID
    7,                       // mListID
    fpcPi_CURRENT_e,         // mListPrio
    PROC_Obj_LndRope,        // mProcName
    &g_fpcLf_Method.base,    // sub_method
    sizeof(daObjLndRope_c),  // mSize
    0,                       // mSizeOther
    0,                       // mParameters
    &g_fopAc_Method.base,    // sub_method
    34,                      // mPriority
    &l_daObjLndRope_Method,  // sub_method
    0x00040180,              // mStatus
    fopAc_ACTOR_e,           // mActorType
    fopAc_CULLBOX_CUSTOM_e,  // cullType
};
