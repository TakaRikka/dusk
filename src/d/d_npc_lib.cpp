#include "d/dolzel.h" // IWYU pragma: keep

#include "d/d_npc_lib.h"
#include "SSystem/SComponent/c_math.h"

dNpcLib_lookat_c::dNpcLib_lookat_c() {}

void dNpcLib_lookat_c::init(J3DModel* i_mdl_p, int* param_1, csXyz* param_2, csXyz* param_3) {
    int i;
    for (i = 0; i < 4; i++) {
        field_0xbc[i] = -1;
    }

    mCount = 0;
    for (i = 0; i < 3; i++) {
        int a_jntNum = param_1[i];
        if (a_jntNum < 0) {
            break;
        }

        JUT_ASSERT(649, a_jntNum < i_mdl_p->getModelData()->getJointNum());

        mDoMtx_stack_c::copy(i_mdl_p->getAnmMtx(a_jntNum));
        mDoMtx_stack_c::multVecZero(&field_0x04[mCount]);
        field_0xbc[i] = a_jntNum;
        field_0x94[i] = param_2[i];
        field_0x7c[i] = param_3[i];
        mCount++;
    }
}

// NONMATCHING - regswap, equivalent
void dNpcLib_lookat_c::action(cXyz param_0, cXyz param_1, fopAc_ac_c* param_2, Mtx param_3, int param_4) {
    cXyz sp90;

    Mtx spA0;
    cMtx_copy(param_3, spA0);
    spA0[0][3] = spA0[1][3] = spA0[2][3] = 0.0f;

    mDoMtx_stack_c::copy(spA0);
    mDoMtx_stack_c::inverse();
    cMtx_copy(mDoMtx_stack_c::get(), spA0);

    for (int i = 0; i < mCount; i++) {
        sp90 = field_0x04[i] - param_2->current.pos;
        mDoMtx_stack_c::transS(param_2->current.pos);
        mDoMtx_stack_c::concat(spA0);
        mDoMtx_stack_c::multVec(&sp90, &field_0x04[i]);
    }

    sp90 = param_0 - param_2->current.pos;
    mDoMtx_stack_c::transS(param_2->current.pos);
    mDoMtx_stack_c::concat(spA0);
    mDoMtx_stack_c::multVec(&sp90, &param_0);

    sp90 = param_1 - param_2->current.pos;
    mDoMtx_stack_c::transS(param_2->current.pos);
    mDoMtx_stack_c::concat(spA0);
    mDoMtx_stack_c::multVec(&sp90, &field_0x04[mCount]);

    setPrm();
    update();

    cXyz sp84;
    cXyz sp78;

    f32 var_f30 = 0.0f;
    f32 var_f31 = 1.0f;
    for (int i = mCount - 1; i >= 0 && param_4 == 1; i--) {
        sp84 = param_0 - field_0x04[i];

        if (!sp84.isZero()) {
            sp84.normalize();

            sp78 = field_0x04[mCount] - field_0x04[i];
            if (!sp78.isZero()) {
                sp78.normalize();

                s16 sp1A = -cM_atan2s(sp84.y, sp84.absXZ());
                s16 sp18 = cM_atan2s(sp84.x, sp84.z);

                s16 sp16 = -cM_atan2s(sp78.y, sp78.absXZ());
                s16 sp14 = cM_atan2s(sp78.x, sp78.z);

                field_0x4c[i].x += (s16)(var_f31 * (f32)(sp1A - sp16));
                field_0x4c[i].y += (s16)(var_f31 * (f32)(sp18 - sp14));

                limitter(&field_0x4c[i].x, field_0x34[i].x, field_0x94[i].x, field_0x7c[i].x);
                limitter(&field_0x4c[i].y, field_0x34[i].y, field_0x94[i].y, field_0x7c[i].y);
            }
        }

        var_f31 *= 0.5f;
        update();
    }

    csXyz sp114(csXyz::Zero);
    for (int i = 0; i < 4; i++) {
        sp114 += field_0x4c[i];
        field_0x64[i].x = sp114.x;
        field_0x64[i].y = sp114.y;
        field_0x64[i].z = 0;
    }
}

void dNpcLib_lookat_c::dbView() {}

void dNpcLib_lookat_c::setPrm() {
    cXyz sp14;
    int i;

    for (i = 0; i < mCount; i++) {
        field_0x34[i] = csXyz::Zero;
        field_0xac[i] = 0.0f;
    }

    for (i = 0; i < mCount; i++) {
        field_0x4c[i].x = 0.4f * field_0x4c[i].x;
        field_0x4c[i].y = 0.4f * field_0x4c[i].y;

        sp14 = field_0x04[i + 1] - field_0x04[i];
        field_0x34[i].x = -cM_atan2s(sp14.y, sp14.absXZ());
        field_0x34[i].y = cM_atan2s(sp14.x, sp14.z);

        field_0xac[i] = sp14.abs();
    }
}

void dNpcLib_lookat_c::update() {
    cXyz sp50;
    csXyz sp58(csXyz::Zero);
    Mtx m;
    int i;

    for (i = 0; i < mCount; i++) {
        mDoMtx_stack_c::XYZrotS(field_0x34[i]);
        cMtx_copy(mDoMtx_stack_c::get(), m);

        mDoMtx_stack_c::transS(field_0x04[i]);
        sp58.x += field_0x4c[i].x;
        sp58.y += field_0x4c[i].y;

        mDoMtx_stack_c::ZXYrotM(sp58);
        mDoMtx_stack_c::concat(m);

        sp50.set(0.0f, 0.0f, field_0xac[i]);
        mDoMtx_stack_c::multVec(&sp50, &field_0x04[i + 1]);
    }
}


int dNpcLib_lookat_c::limitter(s16* o_value, s16 param_1, s16 param_2, s16 param_3) {
    int limit = param_1 + *o_value;
    BOOL rt = FALSE;

    if (param_2 <= limit) {
        if (param_2 <= param_1) {
            *o_value = 0;
        } else {
            *o_value -= (s16)(limit - param_2);
            rt = TRUE;
        }
    }

    if (limit <= param_3) {
        if (param_1 <= param_3) {
            *o_value = 0;
        } else {
            *o_value -= (s16)(limit - param_3);
            rt = TRUE;
        }
    }

    return 1;
}
