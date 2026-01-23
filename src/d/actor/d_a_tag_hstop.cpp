/**
 * d_a_tag_hstop.cpp
 *
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_horse.h"
#include "d/actor/d_a_tag_hstop.h"
#include "d/d_com_inf_game.h"
#include "d/d_debug_viewer.h"
#include "d/d_meter2_info.h"
#include "d/d_s_play.h"
#include "f_op/f_op_actor_mng.h"

int daTagHstop_c::create() {
    fopAcM_ct(this, daTagHstop_c);

    scale.x *= 100.0f;
    scale.y *= 100.0f;
    scale.z *= 100.0f;

    if (m_top != NULL) {
        daTagHstop_c* last = m_top;

        while (last->mNext != NULL) {
            last = last->mNext;
        }

        last->mNext = this;
        mPrev = last;
    } else {
        m_top = this;
    }

    mPrm0 = fopAcM_GetParam(this);
    mPrm1 = (fopAcM_GetParam(this) >> 8) & 0xF;

    if (mPrm1 == 15) {
        mPrm1 = 0;
    }

    setActive();

    if (mPrm1 == 2) {
        if (daPy_py_c::checkRoomRestartStart()) {
            if (!fopAcM_isSwitch(this, 0x8A)) {
                fopAcM_onSwitch(this, 0x8A);
            } else if (!fopAcM_isSwitch(this, 0x8B)) {
                fopAcM_onSwitch(this, 0x8B);
            } else if (!fopAcM_isSwitch(this, 0x8C)) {
                fopAcM_onSwitch(this, 0x8C);
            }
        }

        if (fopAcM_isSwitch(this, 0x8C)) {
            field_0x574 = 0;
        } else {
            field_0x574 = 1200;
        }
    } else {
        field_0x574 = 0;
    }

    return cPhs_COMPLEATE_e;
}

static int daTagHstop_Create(fopAc_ac_c* i_this) {
    daTagHstop_c* hStop = static_cast<daTagHstop_c*>(i_this);
    int id = fopAcM_GetID(i_this);
    return hStop->create();
}

daTagHstop_c::~daTagHstop_c() {
    if (mPrev != NULL) {
        mPrev->mNext = mNext;
    }

    if (mNext != NULL) {
        mNext->mPrev = mPrev;
    }

    if (m_top == this) {
        m_top = mNext;
    }
}

static int daTagHstop_Delete(daTagHstop_c* i_this) {
    int id = fopAcM_GetID(i_this);
    static_cast<daTagHstop_c*>(i_this)->~daTagHstop_c();
    return 1;
}

void daTagHstop_c::setActive() {
    if (mPrm0 == 0xFF || mPrm1 == 2 ||
        (mPrm1 == 0 && fopAcM_isSwitch(this, mPrm0)) ||
        (mPrm1 == 1 && !fopAcM_isSwitch(this, mPrm0)))
    {
        mActive = true;
    } else {
        mActive = false;
    }
}

dMsgFlow_c daTagHstop_c::m_msgFlow;

int daTagHstop_c::execute() {
    if (eventInfo.checkCommandTalk()) {
        if (field_0x573 == 2) {
            m_msgFlow.init(this, (u16)shape_angle.z, 0, NULL);
            field_0x573 = 3;
        } else if (m_msgFlow.doFlow(this, NULL, 0)) {
            dComIfGp_event_reset();
            field_0x573 = 0;

            dComIfGp_setItemArrowNumCount(dComIfGp_getItemMaxArrowNumCount());
            fopAcM_onSwitch(this, 0x8D);
        }
    } else {
        if (field_0x574 != 0) {
            if (fopAcM_isSwitch(this, 0x8F)) {
                field_0x574 = 0;
            } else if (daPy_getLinkPlayerActorClass()->checkSingleBoarBattleSecondBowReady()) {
                fopAcM_onSwitch(this, 0x8F);
                field_0x574 = 0;
            } else {
                field_0x574--;
            }
        }

        setActive();

        if (field_0x573) {
            daHorse_c* horse_p = (daHorse_c*)dComIfGp_getHorseActor();

            if (mPrm1 != 2 || dComIfGs_getArrowNum() != 0 || horse_p == NULL) {
                field_0x573 = 0;
            } else if (field_0x573 == 1) {
                if (horse_p->checkTurnStand() &&
                    !horse_p->checkTurnStandCamera())
                {
                    field_0x573 = 2;
                }
            } else if (field_0x573 == 2 && !horse_p->checkTurnStand()) {
                fopAcM_orderSpeakEvent(this, 0, 0);
                eventInfo.onCondition(dEvtCnd_CANTALK_e);
            }
        } else if (mPrm1 == 2 && !dComIfGp_event_runCheck()) {
            if (dComIfGs_getArrowNum() == 0 && !fopAcM_isSwitch(this, 0x8D))
            {
                fopAcM_onSwitch(this, 0x8D);
                dMeter2Info_setFloatingFlow(43, 90, true);

                if (!fopAcM_isSwitch(this, 0x8F)) {
                    field_0x574 = 1200;
                    fopAcM_offSwitch(this, 0x8A);
                    fopAcM_offSwitch(this, 0x8B);
                    fopAcM_offSwitch(this, 0x8C);
                }
            } else if (field_0x574 == 0 && !fopAcM_isSwitch(this, 0x8F)) {
                fopAcM_onSwitch(this, 0x8F);
                fopAcM_onSwitch(this, 0x8E);
            }
        }
    }

    return 1;
}

static int daTagHstop_Execute(daTagHstop_c* i_this) {
    return i_this->execute();
}

static int daTagHstop_Draw(daTagHstop_c* i_this) {
#if DEBUG
    if (UREG_S(9) != 0) {
        cXyz sp08[8];
        sp08[0].set(-i_this->scale.x, i_this->scale.y, -i_this->scale.z);
        sp08[1].set(i_this->scale.x, sp08[0].y, sp08[0].z);
        sp08[2].set(sp08[0].x, sp08[0].y, i_this->scale.z);
        sp08[3].set(sp08[1].x, sp08[0].y, sp08[2].z);
        sp08[4].set(sp08[0].x, 0.0f, sp08[0].z);
        sp08[5].set(sp08[1].x, 0.0f, sp08[1].z);
        sp08[6].set(sp08[2].x, 0.0f, sp08[2].z);
        sp08[7].set(sp08[3].x, 0.0f, sp08[3].z);

        mDoMtx_stack_c::transS(i_this->current.pos);
        mDoMtx_stack_c::YrotM(i_this->shape_angle.y);

        for (int i = 0; i < 8; i++) {
            mDoMtx_stack_c::multVec(&sp08[i], &sp08[i]);
        }

        static GXColor color = {0xff, 0, 0xff, 0x80};
        dDbVw_drawCube8pXlu(sp08, color);
    }
#endif

    return 1;
}

static actor_method_class l_daTagHstop_Method = {
    (process_method_func)daTagHstop_Create,  (process_method_func)daTagHstop_Delete,
    (process_method_func)daTagHstop_Execute, (process_method_func)NULL,
    (process_method_func)daTagHstop_Draw,
};

actor_process_profile_definition g_profile_Tag_Hstop = {
    fpcLy_CURRENT_e,
    3,
    fpcPi_CURRENT_e,
    PROC_Tag_Hstop,
    &g_fpcLf_Method.base,
    sizeof(daTagHstop_c),
    0,
    0,
    &g_fopAc_Method.base,
    435,
    &l_daTagHstop_Method,
    0x40000,
    fopAc_ENV_e,
    fopAc_CULLBOX_CUSTOM_e,
};
