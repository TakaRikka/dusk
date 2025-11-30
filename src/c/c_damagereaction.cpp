#include "c/c_damagereaction.h"
#include "d/actor/d_a_player.h"
#include "d/d_com_inf_game.h"
#include "f_op/f_op_actor_mng.h"

u8 cDmr_SkipInfo;
u8 data_80450C99;
u8 data_80450C9A;
u8 data_80450C9B;

u8 data_80450C9C;
u8 data_80450C9D;
u8 data_80450C9E;
u8 cDmr_FishingWether;

void JPTraceParticleCallBack4::execute(JPABaseEmitter* param_0, JPABaseParticle* param_1) {
    u32 age = param_1->getAge();
    if (age != 0) {
        Vec* user_work = (Vec*)param_0->getUserWork();
        JGeometry::TVec3<f32> vec1, vec2, vec3;
        f32 fVar1 = (25.0f - age) / 25.0f;
        if (fVar1 < 0.0f) {
            fVar1 = 0.0f;
        }
        vec1 = user_work[age];
        vec1.scale(0.3f * fVar1);
        vec2 = user_work[0];
        vec2.scale(0.5f * fVar1);

        param_1->getOffsetPosition(vec3);
        vec3.add(vec1 + vec2);
        param_1->setOffsetPosition(vec3);
    }
}


void JPTraceParticleCallBack4::draw(JPABaseEmitter* param_0, JPABaseParticle* param_1) {
    /* empty function */
}

BOOL cDmrNowMidnaTalk() {
    if (dComIfGp_event_runCheck()) {
        fopAc_ac_c* actor = (fopAc_ac_c*)fopAcM_getTalkEventPartner(daPy_getLinkPlayerActorClass());
        if (actor) {
            if (fopAcM_GetName(actor) == PROC_MIDNA || fopAcM_GetName(actor) == PROC_Tag_Mhint ||
                     fopAcM_GetName(actor) == PROC_Tag_Mstop || fopAcM_GetName(actor) == PROC_Tag_Hstop ||
                     fopAcM_GetName(actor) == PROC_Tag_Mwait || fopAcM_GetName(actor) == PROC_Tag_Wljump) {
                return 1;
            }
        }
    }

    return 0;
}

u8 data_80450CA0;

JPTraceParticleCallBack4 JPTracePCB4;
