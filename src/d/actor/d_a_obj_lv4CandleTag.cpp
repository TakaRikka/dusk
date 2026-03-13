#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_lv4CandleTag.h"

class dalv4CandleTag_HIO_c : public mDoHIO_entry_c {
public:
    dalv4CandleTag_HIO_c();
    virtual ~dalv4CandleTag_HIO_c() {}

    void genMessage(JORMContext*);

    /* 0x4 */ u8 mTimer;
};

dalv4CandleTag_HIO_c::dalv4CandleTag_HIO_c() {
    mTimer = 60;
}

void dalv4CandleTag_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
}

int dalv4CandleTag_c::create() {
    fopAcM_ct(this, dalv4CandleTag_c);

    setBaseMtx();

    mSwTop = getSwTop();
    mSwOK = getSwOK();
    mSwNG = getSwNG();
    mSwTotal = getSwTotal();

    if (fopAcM_isSwitch(this, mSwOK)) {
        init_modeEnd();
    } else {
        init_modeWatch();
    }

    return cPhs_COMPLEATE_e;
}

int dalv4CandleTag_c::Execute() {
    eventUpdate();
    procMain();
    setBaseMtx();
    return 1;
}

static dalv4CandleTag_HIO_c l_HIO;

void dalv4CandleTag_c::procMain() {
    static const modeFunc mode_proc[] = {
        &dalv4CandleTag_c::modeWatch,
        &dalv4CandleTag_c::modeNG,
        &dalv4CandleTag_c::modeNGWait,
        &dalv4CandleTag_c::modeEnd,
    };

    (this->*mode_proc[mMode])();
}

void dalv4CandleTag_c::init_modeWatch() {
    mMode = 0;
}

void dalv4CandleTag_c::modeWatch() {
    if (fopAcM_isSwitch(this, mSwOK)) {
        init_modeEnd();
    } else if (fopAcM_isSwitch(this, mSwNG)) {
        if (getEventID() != 0xFF) {
            orderEvent(getEventID(), 0xFF, 1);
        } else {
            eventStart();
        }
    }
}

void dalv4CandleTag_c::init_modeNG() {
    mTimer = l_HIO.mTimer;
    mDoAud_seStart(Z2SE_OBJ_LV4_CNDL_MISS, NULL, 0, 0);
    mMode = 1;
}

void dalv4CandleTag_c::modeNG() {
    if (mTimer != 0) {
        mTimer--;
    } else {
        u8 swbit = mSwTop;
        for (int i = 0; i < mSwTotal; i++) {
            fopAcM_offSwitch(this, swbit);
            swbit++;
        }

        init_modeNGWait();
    }
}

void dalv4CandleTag_c::init_modeNGWait() {
    mMode = 2;
}

void dalv4CandleTag_c::modeNGWait() {
    if (!fopAcM_isSwitch(this, mSwNG)) {
        init_modeWatch();
    }
}

void dalv4CandleTag_c::init_modeEnd() {
    mMode = 3;
}

void dalv4CandleTag_c::modeEnd() {}

bool dalv4CandleTag_c::eventStart() {
    init_modeNG();
    return true;
}

int dalv4CandleTag_c::Draw() {
    return 1;
}

int dalv4CandleTag_c::Delete() {
    return 1;
}

static int dalv4CandleTag_Draw(dalv4CandleTag_c* i_this) {
    return i_this->Draw();
}

static int dalv4CandleTag_Execute(dalv4CandleTag_c* i_this) {
    return i_this->Execute();
}

static int dalv4CandleTag_Delete(dalv4CandleTag_c* i_this) {
    return i_this->Delete();
}

static int dalv4CandleTag_Create(fopAc_ac_c* i_this) {
    return ((dalv4CandleTag_c*)i_this)->create();
}

static actor_method_class l_dalv4CandleTag_Method = {
    (process_method_func)dalv4CandleTag_Create,
    (process_method_func)dalv4CandleTag_Delete,
    (process_method_func)dalv4CandleTag_Execute,
    NULL,
    (process_method_func)dalv4CandleTag_Draw,
};

actor_process_profile_definition g_profile_Tag_Lv4Candle = {
    /* Layer ID     */ fpcLy_CURRENT_e,
    /* List ID      */ 7,
    /* List Prio    */ fpcPi_CURRENT_e,
    /* Proc Name    */ fpcNm_Tag_Lv4Candle_e,
    /* Proc SubMtd  */ &g_fpcLf_Method.base,
    /* Size         */ sizeof(dalv4CandleTag_c),
    /* Size Other   */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopAc_Method.base,
    /* Draw Prio    */ fpcDwPi_Tag_Lv4Candle_e,
    /* Actor SubMtd */ &l_dalv4CandleTag_Method,
    /* Status       */ fopAcStts_UNK_0x40000_e | fopAcStts_UNK_0x4000_e,
    /* Group        */ fopAc_ACTOR_e,
    /* Cull Type    */ fopAc_CULLBOX_CUSTOM_e,
};
