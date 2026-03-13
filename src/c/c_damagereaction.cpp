#include "c/c_damagereaction.h"
#include "d/actor/d_a_player.h"
#include "d/d_com_inf_game.h"
#include "f_op/f_op_actor_mng.h"
#include "d/d_s_play.h"

u8 cDmr_SkipInfo;
u8 data_80450C99;
u8 data_80450C9A;
u8 data_80450C9B;

u8 data_80450C9C;
u8 data_80450C9D;
u8 data_80450C9E;
u8 cDmr_FishingWether;

#if DEBUG
void debug_actor_create() {
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
}
#endif

void JPTraceParticleCallBack4::execute(JPABaseEmitter* param_0, JPABaseParticle* param_1) {
    u32 age = param_1->getAge();
    if (age != 0) {
        Vec* user_work = (Vec*)param_0->getUserWork();
        JGeometry::TVec3<f32> vec1, vec2, vec3;
        f32 fVar1 = ((25.0f + nREG_F(18)) - age) / (25.0f + nREG_F(18));
        if (fVar1 < 0.0f) {
            fVar1 = 0.0f;
        }
        vec1 = user_work[age];
        vec1.scale((0.3f + nREG_F(16)) * fVar1);
        vec2 = user_work[0];
        vec2.scale((0.5f + nREG_F(17)) * fVar1);

        param_1->getOffsetPosition(&vec3);
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
            if (fopAcM_GetName(actor) == fpcNm_MIDNA_e || fopAcM_GetName(actor) == fpcNm_Tag_Mhint_e ||
                     fopAcM_GetName(actor) == fpcNm_Tag_Mstop_e || fopAcM_GetName(actor) == fpcNm_Tag_Hstop_e ||
                     fopAcM_GetName(actor) == fpcNm_Tag_Mwait_e || fopAcM_GetName(actor) == fpcNm_Tag_Wljump_e) {
                return 1;
            }
        }
    }

    return 0;
}

u8 data_80450CA0;

extern "C" {
    JPTraceParticleCallBack4 JPTracePCB4;
}
