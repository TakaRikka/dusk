/**
 * d_a_andsw.cpp
 * AND - Switch
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_andsw.h"
#include "d/d_procname.h"

#if DEBUG
daAndsw_HIO_c l_HIO;

daAndsw_HIO_c::daAndsw_HIO_c() {
    field_0x6 = 0;
}

void daAndsw_HIO_c::genMessage(JORMContext* ctx) {
    ctx->genLabel("ＳＷ監視", 0, 0, NULL, 0xFFFF, 0xFFFF, 512, 24);
    ctx->genCheckBox("ＳＷ状態出力", &field_0x6, 0x01, 0, NULL, 0xFFFF, 0xFFFF, 512, 24);
}
#endif

int daAndsw_c::Create() {
    mSwNo = getSwNo();
    mSwNo2 = getSwNo2();

    if (getTimer() != 0xFF) {
        mTimer = getTimer() * 15;
    } else {
        mTimer = 0;
    }

    OS_REPORT("ANDSW PARAM 0x%x\n", fopAcM_GetParam(this));

    return 1;
}

int daAndsw_c::create() {
    fopAcM_ct(this, daAndsw_c);
    if (!Create()) {
        return cPhs_ERROR_e;
    }

#if DEBUG
    l_HIO.entryHIO("ＳＷ監視");
#endif

    return cPhs_COMPLEATE_e;
}

int daAndsw_c::execute() {
    if (fopAcM_isSwitch(this,mSwNo2)) {
        if (0 < mTimer) {
            mTimer--;
        }

        if (mTimer == 0) {
            fopAcM_offSwitch(this,mSwNo);

#if DEBUG
            if (l_HIO.field_0x6 != 0) {
                OS_REPORT("-- ＳＷ監視状態出力 --\n");
                OS_REPORT("sw<%d>ＯＦＦしました\n", mSwNo);
            }
#endif
        
            if (getType() == 1) {
                fopAcM_delete(this);
                OS_REPORT("ＳＷ監視：ＳＷＯＦＦしたので処理終わりますSW<%d>\n", mSwNo);
            }
        }
    } else {
#if DEBUG
        if (l_HIO.field_0x6 != 0) {
            OS_REPORT("sw2<%d>を待っています\n", mSwNo2);
            l_HIO.field_0x6 = 0;
        }
#endif
    }

    return 1;
}

int daAndsw_c::_delete() {
#if DEBUG
    l_HIO.removeHIO();
#endif
    return 1;
}

static int daAndsw_Execute(daAndsw_c* i_this) {
    return i_this->execute();
}

static int daAndsw_Delete(daAndsw_c* i_this) {
    fopAcM_RegisterDeleteID(i_this, "Andsw");
    return i_this->_delete();
}

static int daAndsw_Create(fopAc_ac_c* i_this) {
    fopAcM_RegisterCreateID(daAndsw_c, i_this, "Andsw");
    return a_this->create();
}

static actor_method_class l_daAndsw_Method = {
    (process_method_func)daAndsw_Create,
    (process_method_func)daAndsw_Delete,
    (process_method_func)daAndsw_Execute,
    (process_method_func)NULL,
    (process_method_func)NULL,
};

extern actor_process_profile_definition g_profile_ANDSW = {
    fpcLy_CURRENT_e,        // mLayerID
    7,                      // mListID
    fpcPi_CURRENT_e,        // mListPrio
    PROC_ANDSW,             // mProcName
    &g_fpcLf_Method.base,  // sub_method
    sizeof(daAndsw_c),      // mSize
    0,                      // mSizeOther
    0,                      // mParameters
    &g_fopAc_Method.base,   // sub_method
    302,                    // mPriority
    &l_daAndsw_Method,      // sub_method
    0x40000,                // mStatus
    fopAc_ACTOR_e,          // mActorType
    fopAc_CULLBOX_CUSTOM_e, // cullType
};
