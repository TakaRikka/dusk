/**
 * @file d_a_obj_lv4CandleDemoTag.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_lv4CandleDemoTag.h"
#include "d/d_com_inf_game.h"
#include "d/d_path.h"
#include "m_Do/m_Do_hostIO.h"

class dalv4CandleDemoTag_HIO_c : public mDoHIO_entry_c {
public:
    dalv4CandleDemoTag_HIO_c();
    virtual ~dalv4CandleDemoTag_HIO_c() {}

    void genMessage(JORMContext*);

    /* 0x4 */ s16 mEnemyWaitTime;
};

dalv4CandleDemoTag_HIO_c::dalv4CandleDemoTag_HIO_c() {
    mEnemyWaitTime = 60;
}

void dalv4CandleDemoTag_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
}

int dalv4CandleDemoTag_c::create() {
    fopAcM_ct(this, dalv4CandleDemoTag_c);

    mPathID = getPathID();

    if (dPath_GetRoomPath(mPathID, fopAcM_GetRoomNo(this)) == NULL) {
        return 0;
    }

    mSwNG = getSwNG();
    mSwDest = getSwDest();
    fopAcM_offSwitch(this, mSwDest);

    setBaseMtx();

    if (fopAcM_isSwitch(this, getSwOK())) {
        init_modeEnd();
    } else {
        init_modeWatch();
    }

    return cPhs_COMPLEATE_e;
}

int dalv4CandleDemoTag_c::Execute() {
    eventUpdate();
    procMain();
    setBaseMtx();
    return 1;
}

static dalv4CandleDemoTag_HIO_c l_HIO;

void dalv4CandleDemoTag_c::procMain() {
    static dalv4CandleDemoTag_c::procFunc mode_proc[4] = {
        &dalv4CandleDemoTag_c::modeWatch,
        &dalv4CandleDemoTag_c::modeEnemyCreate,
        &dalv4CandleDemoTag_c::modeNG,
        &dalv4CandleDemoTag_c::modeEnd,
    };

    (this->*mode_proc[mMode])();
}

void dalv4CandleDemoTag_c::init_modeWatch() {
    mEnemyWaitTime = l_HIO.mEnemyWaitTime;
    mMode = 0;
}

void dalv4CandleDemoTag_c::modeWatch() {
    mpShutterActor = (daLv4EdShutter_c*)fopAcM_Search(searchShutterAct, this);

    u8 var3;
    if (mpShutterActor == NULL) {
        var3 = 1;
    } else {
        var3 = mpShutterActor->getStatus();
    }

    if (fopAcM_isSwitch(this, mSwNG) && var3 != 0) {
        if (mEnemyWaitTime == 0) {
            if (getEventID() != 0xFF) {
                orderEvent(getEventID(), 0xFF, 1);
            } else {
                eventStart();
            }
        } else {
            mEnemyWaitTime--;
        }
    }

    if (fopAcM_isSwitch(this, getSwOK())) {
        init_modeEnd();
    }
}

void dalv4CandleDemoTag_c::init_modeEnemyCreate() {
    dPath* path_p = dPath_GetRoomPath(mPathID, fopAcM_GetRoomNo(this));

    for (int i = 0; i < path_p->m_num; i++) {
        dPnt* pnt = &path_p->m_points[i];

        cXyz pos;
        pos.x = pnt->m_position.x;
        pos.y = pnt->m_position.y;
        pos.z = pnt->m_position.z;

        fopAcM_create(PROC_E_BS, 0xFFFFFEFF, &pos, fopAcM_GetRoomNo(this), NULL, NULL, -1);
    }

    mMode = 1;
}

void dalv4CandleDemoTag_c::modeEnemyCreate() {
    if (fopAcM_myRoomSearchEnemy(fopAcM_GetRoomNo(this))) {
        init_modeNG();
    }
}

void dalv4CandleDemoTag_c::init_modeNG() {
    mMode = 2;
}

void dalv4CandleDemoTag_c::modeNG() {
    if (!fopAcM_myRoomSearchEnemy(fopAcM_GetRoomNo(this))) {
        fopAcM_onSwitch(this, mSwDest);
        fopAcM_offSwitch(this, mSwNG);
        init_modeWatch();
    }
}

void dalv4CandleDemoTag_c::init_modeEnd() {
    mMode = 3;
}

void dalv4CandleDemoTag_c::modeEnd() {
    fopAcM_offSwitch(this, mSwNG);
}

bool dalv4CandleDemoTag_c::eventStart() {
    init_modeEnemyCreate();
    return true;
}

void* dalv4CandleDemoTag_c::searchShutterAct(void* param_0, void* param_1) {
    if (param_0 != NULL && fopAcM_IsActor(param_0) &&
        fopAcM_GetProfName(param_0) == PROC_Obj_Lv4EdShutter)
    {
        return param_0;
    }

    return NULL;
}

int dalv4CandleDemoTag_c::Draw() {
    return 1;
}

int dalv4CandleDemoTag_c::Delete() {
    return 1;
}

static int dalv4CandleDemoTag_Draw(dalv4CandleDemoTag_c* i_this) {
    return i_this->Draw();
}

static int dalv4CandleDemoTag_Execute(dalv4CandleDemoTag_c* i_this) {
    return i_this->Execute();
}

static int dalv4CandleDemoTag_Delete(dalv4CandleDemoTag_c* i_this) {
    return i_this->Delete();
}

static int dalv4CandleDemoTag_Create(fopAc_ac_c* i_this) {
    return static_cast<dalv4CandleDemoTag_c*>(i_this)->create();
}

static actor_method_class l_dalv4CandleDemoTag_Method = {
    (process_method_func)dalv4CandleDemoTag_Create,
    (process_method_func)dalv4CandleDemoTag_Delete,
    (process_method_func)dalv4CandleDemoTag_Execute,
    NULL,
    (process_method_func)dalv4CandleDemoTag_Draw,
};

extern actor_process_profile_definition g_profile_Tag_Lv4CandleDm = {
  fpcLy_CURRENT_e,              // mLayerID
  7,                            // mListID
  fpcPi_CURRENT_e,              // mListPrio
  PROC_Tag_Lv4CandleDm,         // mProcName
  &g_fpcLf_Method.base,        // sub_method
  sizeof(dalv4CandleDemoTag_c), // mSize
  0,                            // mSizeOther
  0,                            // mParameters
  &g_fopAc_Method.base,         // sub_method
  541,                          // mPriority
  &l_dalv4CandleDemoTag_Method, // sub_method
  0x00044000,                   // mStatus
  fopAc_ACTOR_e,                // mActorType
  fopAc_CULLBOX_CUSTOM_e,       // cullType
};
