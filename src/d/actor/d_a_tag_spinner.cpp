/**
 * d_a_tag_spinner.cpp
 * Tag - Spinner
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_tag_spinner.h"

#include "d/d_debug_viewer.h"
#include "d/d_procname.h"

class daTagSpinner_HIO_c : public mDoHIO_entry_c {
public:
    daTagSpinner_HIO_c();

    void genMessage(JORMContext*);

    u8 enable_debug_display;
};

#if DEBUG
daTagSpinner_HIO_c l_HIO;

daTagSpinner_HIO_c::daTagSpinner_HIO_c() {
    enable_debug_display = FALSE;
}

void daTagSpinner_HIO_c::genMessage(JORMContext* ctx) {
    // "Spinner suction tag"
    ctx->genLabel("スピンナー吸い込みタグ", 0);
    // "Debug display"
    ctx->genCheckBox("デバッグ表示", &enable_debug_display, 0x1);
}
#endif

static int CheckCreateHeap(fopAc_ac_c* param_0) {
    daTagSpinner_c* tag = (daTagSpinner_c*)param_0;
    return tag->CreateHeap();
}

static void* search_spinner_sub(void* tag_0, void* tag_1) {
    fopAc_ac_c* actor = (fopAc_ac_c*)tag_0;
    daTagSpinner_c* actor2 = (daTagSpinner_c*)tag_1;

    if (actor != NULL &&
        fopAcM_IsActor(actor) != NULL &&
        fopAcM_GetProfName(actor) == PROC_SPINNER) {
        f32 latDist = actor->current.pos.absXZ(actor2->current.pos);
        if (latDist < actor2->GetR()) {
            return actor;
        }
    }
    return NULL;
}

void daTagSpinner_c::initBaseMtx() {
    setBaseMtx();
}

void daTagSpinner_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::ZXYrotM(shape_angle.x, shape_angle.y, shape_angle.z);
}

int daTagSpinner_c::Create() {
    initBaseMtx();
    mRadius = scale.x * 100.0f;
    field_0x56c = scale.y * 100.0f;
    return 1;
}

int daTagSpinner_c::CreateHeap() {
    return 1;
}

int daTagSpinner_c::create() {
    fopAcM_ct(this, daTagSpinner_c);

    int var_r29 = cPhs_COMPLEATE_e;
    if (var_r29 == cPhs_COMPLEATE_e) {
        if (!fopAcM_entrySolidHeap(this, CheckCreateHeap, 0)) {
            return cPhs_ERROR_e;
        }

        if (!Create()) {
            return cPhs_ERROR_e;
        }

        OS_REPORT("TAG_SPINNER PARAM:%x\n", fopAcM_GetParam(this));

#if DEBUG
        // "Spinner suction tag"
        l_HIO.entryHIO("スピンナー吸い込みタグ");
#endif
    }

    return var_r29;
}

int daTagSpinner_c::execute() {
    daSpinner_c* mpSpinner =
        (daSpinner_c*)fopAcM_Search((fopAcIt_JudgeFunc)search_spinner_sub, this);

    if (mpSpinner) {
        mpSpinner->setSpinnerTag(current.pos);
        fopAcM_onSwitch(this, daTagSpinner_prm::getSwNo(this));
    } else {
        fopAcM_offSwitch(this, daTagSpinner_prm::getSwNo(this));
    }
    return 1;
}

int daTagSpinner_c::draw() {
#if DEBUG
    GXColor color = (GXColor){0, 0, 0x80, 0x80};

    if (l_HIO.enable_debug_display) {
        dDbVw_drawCylinderXlu(current.pos, mRadius, field_0x56c, color, 1);
    }
#endif

    return 1;
}

int daTagSpinner_c::_delete() {
#if DEBUG
    l_HIO.removeHIO();
#endif

    return 1;
}

static int daTagSpinner_Execute(daTagSpinner_c* i_this) {
    return i_this->execute();
}

static int daTagSpinner_Draw(daTagSpinner_c* i_this) {
    return i_this->draw();
}

static int daTagSpinner_Delete(daTagSpinner_c* i_this) {
    int id = fopAcM_GetID(i_this);
    return i_this->_delete();
}

static int daTagSpinner_Create(fopAc_ac_c* i_this) {
    daTagSpinner_c* spinner = static_cast<daTagSpinner_c*>(i_this);
    int id = fopAcM_GetID(i_this);
    return spinner->create();
}

static actor_method_class l_daTagSpinner_Method = {
    (process_method_func)daTagSpinner_Create,
    (process_method_func)daTagSpinner_Delete,
    (process_method_func)daTagSpinner_Execute,
    (process_method_func)NULL,
#if DEBUG
    (process_method_func)daTagSpinner_Draw,
#else
    (process_method_func)NULL,
#endif

};

actor_process_profile_definition g_profile_Tag_Spinner = {
    fpcLy_CURRENT_e,         // mLayerID
    3,                       // mListID
    fpcPi_CURRENT_e,         // mListPrio
    PROC_Tag_Spinner,        // mProcName
    &g_fpcLf_Method.base,   // sub_method
    sizeof(daTagSpinner_c),  // mSize
    0,                       // mSizeOther
    0,                       // mParameters
    &g_fopAc_Method.base,    // sub_method
    605,                     // mPriority
    &l_daTagSpinner_Method,  // sub_method
    0x40100,                 // mStatus
    fopAc_ACTOR_e,           // mActorType
    fopAc_CULLBOX_CUSTOM_e,  // cullType
};
