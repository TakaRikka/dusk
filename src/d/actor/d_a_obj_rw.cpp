/**
 * @file d_a_obj_rw.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_rw.h"
#include "d/actor/d_a_player.h"
#include "d/d_a_obj.h"
#include "d/d_s_play.h"

class daOBJ_RW_HIO_c : public JORReflexible {
public:
    daOBJ_RW_HIO_c();
    virtual ~daOBJ_RW_HIO_c() {}

    void genMessage(JORMContext*);

    /* 0x4 */ s8 id;
    /* 0x8 */ f32 base_size;
    /* 0xC */ u8 unk_0xC[0x14 - 0xC];
};

daOBJ_RW_HIO_c::daOBJ_RW_HIO_c() {
    id = -1;
    base_size = 1.0f;
}

static int nodeCallBack(J3DJoint* i_joint, int param_1) {
    if (param_1 == 0) {
        int jnt_no = i_joint->getJntNo();
        J3DModel* model = j3dSys.getModel();
        obj_rw_class* i_this = (obj_rw_class*)model->getUserArea();

        if (i_this != NULL) {
            if (jnt_no == 1) {
                MTXCopy(model->getAnmMtx(jnt_no), *calc_mtx);
                cMtx_YrotM(*calc_mtx, i_this->field_0x670);
                model->setAnmMtx(jnt_no, *calc_mtx);
                MTXCopy(*calc_mtx, J3DSys::mCurrentMtx);
            }
        }
    }

    return 1;
}

static int daOBJ_RW_Draw(obj_rw_class* i_this) {
    fopAc_ac_c* actor = &i_this->enemy;

    g_env_light.settingTevStruct(0, &actor->current.pos, &actor->tevStr);
    g_env_light.setLightTevColorType_MAJI(i_this->model, &actor->tevStr);

    J3DShape* shape = i_this->model->getModelData()->getMaterialNodePointer(0)->getShape();
    if (shape != NULL) {
        if (i_this->field_0xe38 != 0) {
            shape->hide();
        } else {
            shape->show();
        }
    }

    mDoExt_modelUpdateDL(i_this->model);

    cXyz sp8;
    sp8.set(actor->current.pos.x, 100.0f + actor->current.pos.y + BREG_F(18), actor->current.pos.z);
    if (fopAcM_gc_c::gndCheck(&sp8)) {
        i_this->shadowId = dComIfGd_setShadow(i_this->shadowId, 1, i_this->model, &sp8, 1500.0f, 0.0f, actor->current.pos.y, actor->current.pos.y, *fopAcM_gc_c::getGroundCheck(), &actor->tevStr, 0, 1.0f, dDlst_shadowControl_c::getSimpleTex());
    }

    return 1;
}

static void damage_check(obj_rw_class* i_this) {
    fopAc_ac_c* actor = &i_this->enemy;
    daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);
    
    i_this->ccStts.Move();

    if (i_this->invulnerability_timer == 0) {
        for (int i = 0; i < 2; i++) {
            if (i_this->ccSph[i].ChkTgHit()) {
                i_this->atInfo.mpCollider = i_this->ccSph[i].GetTgHitObj();
                cc_at_check(actor, &i_this->atInfo);

                if (player->getCutType() == daPy_py_c::CUT_TYPE_JUMP && player->checkCutJumpCancelTurn()) {
                    i_this->invulnerability_timer = 3;
                } else {
                    i_this->invulnerability_timer = 10;
                }

                if (i_this->field_0x66c < 50.0f) {
                    s16 sp8 = (actor->shape_angle.y + 0x4000) - fopAcM_searchPlayerAngleY(actor);
                    if (sp8 < 0) {
                        i_this->field_0x672 = 0;
                    } else {
                        i_this->field_0x672 = -0x8000;
                    }
                }

                i_this->field_0x66c = 150.0f + TREG_F(17);

                cXyz sp2C;
                cXyz pos;
                if (cM_rndF(1.0f) < 0.5f) {
                    cMtx_YrotS(*calc_mtx, actor->shape_angle.y);
                    sp2C.x = -200.0f + JREG_F(8);
                    sp2C.y = JREG_F(9);
                    sp2C.z = 0.0f;
                    MtxPosition(&sp2C, &pos);
                    pos += actor->current.pos;
                    fopAcM_createItem(&pos, fpcNm_ITEM_HEART, -1, -1, NULL, NULL, 0);
                }

                i_this->field_0x674 = 15;

                if (actor->health <= 0) {
                    i_this->field_0xe38 = 1;
                    pos = actor->current.pos;
                    pos.y += 150.0f + JREG_F(7);
                    fopAcM_createDisappear(actor, &pos, 20, 0, actor->field_0x564);

                    cXyz scale(1.0f, 1.0f, 1.0f);
                    fopAcM_createItemForBoss(&pos, fpcNm_ITEM_KAKERA_HEART, fopAcM_GetRoomNo(actor), &actor->shape_angle, &scale, 0.0f, 0.0f, actor->field_0x564);

                    int swbit = (fopAcM_GetParam(actor) & 0xFF000000) >> 0x18;
                    if (swbit != 0xFF) {
                        dComIfGs_onSwitch(swbit, fopAcM_GetRoomNo(actor));
                    }
                }
                break;
            }
        }

        for (int i = 0; i < 4; i++) {
            if (i_this->ccCyl[i].ChkTgHit()) {
                i_this->invulnerability_timer = 10;
                daObj::HitSeStart(&actor->current.pos, fopAcM_GetRoomNo(actor), &i_this->ccCyl[i], 11);
                i_this->field_0x66c = 70.0f + TREG_F(18);
                i_this->field_0x674 = 15;
                return;
            }
        }
    }
}

static void normal(obj_rw_class* i_this) {
    switch (i_this->mode) {
    case 0:
        i_this->mode = 1;
        break;
    }
}

static void action(obj_rw_class* i_this) {
    cXyz sp14;
    cXyz sp8;

    switch (i_this->action) {
    case 0:
        normal(i_this);
        break;
    }

    damage_check(i_this);

    cLib_addCalc0(&i_this->field_0x66c, 0.05f, 5.0f);
    i_this->field_0x672 += 0xD00;
    i_this->field_0x670 = i_this->field_0x66c * (20.0f + TREG_F(16)) * cM_ssin(i_this->field_0x672);
}

static u8 hio_set;

static daOBJ_RW_HIO_c l_HIO;

static int daOBJ_RW_Execute(obj_rw_class* i_this) {
    fopAc_ac_c* actor = &i_this->enemy;
    cXyz sp1C;
    cXyz sp10;

    i_this->field_0x660++;

    for (int i = 0; i < 2; i++) {
        if (i_this->field_0x666[i] != 0) {
            i_this->field_0x666[i]--;
        }
    }

    if (i_this->invulnerability_timer != 0) {
        i_this->invulnerability_timer--;
    }

    if (i_this->field_0x674 != 0) {
        i_this->field_0x674--;
    }

    action(i_this);

    s16 sp8 = (30.0f + TREG_F(14)) * (i_this->field_0x674 * cM_ssin(i_this->field_0x674 * 0x1700));

    mDoMtx_stack_c::transS(actor->current.pos.x, actor->current.pos.y, actor->current.pos.z);
    mDoMtx_stack_c::YrotM(actor->shape_angle.y);
    mDoMtx_stack_c::XrotM(sp8);
    mDoMtx_stack_c::scaleM(l_HIO.base_size, l_HIO.base_size, l_HIO.base_size);
    i_this->model->setBaseTRMtx(mDoMtx_stack_c::get());

    cMtx_YrotS(*calc_mtx, actor->shape_angle.y);

    sp1C.x = -130.0f + KREG_F(0);
    sp1C.y = 200.0f + KREG_F(1);
    sp1C.z = 0.0f;
    MtxPosition(&sp1C, &sp10);
    sp10 += actor->current.pos;
    i_this->ccSph[0].SetC(sp10);
    i_this->ccSph[0].SetR((100.0f + KREG_F(2)) * l_HIO.base_size);

    sp1C.x = 80.0f + KREG_F(3);
    sp1C.y = 220.0f + KREG_F(4);
    sp1C.z = 0.0f;
    MtxPosition(&sp1C, &sp10);
    sp10 += actor->current.pos;
    i_this->ccSph[1].SetR((100.0f + KREG_F(5)) * l_HIO.base_size);
    i_this->ccSph[1].SetC(sp10);

    if (i_this->field_0xe38 != 0) {
        sp10.z -= 52000.0f;
        i_this->ccSph[0].SetC(sp10);
        i_this->ccSph[1].SetC(sp10);
        fopAcM_OffStatus(actor, 0);
        actor->attention_info.flags = 0;
    }

    dComIfG_Ccsp()->Set(&i_this->ccSph[0]);
    dComIfG_Ccsp()->Set(&i_this->ccSph[1]);

    for (int i = 0; i < 4; i++) {
        static f32 pole_x[4] = {-340.0f, -240.0f, 330.0f, 290.0f};
        static f32 pole_z[4] = {90.0f, -90.0f, 110.0f, -100.0f};

        sp1C.x = pole_x[i];
        sp1C.y = 0.0f;
        sp1C.z = pole_z[i];
        MtxPosition(&sp1C, &sp10);
        sp10 += actor->current.pos;
        i_this->ccCyl[i].SetC(sp10);
        i_this->ccCyl[i].SetR((30.0f + KREG_F(12)) * l_HIO.base_size);
        i_this->ccCyl[i].SetH((200.0f + KREG_F(18)) * l_HIO.base_size);
        dComIfG_Ccsp()->Set(&i_this->ccCyl[i]);
    }

    actor->eyePos = actor->current.pos;
    actor->eyePos.y += 200.0f + TREG_F(4);
    actor->attention_info.position = actor->eyePos;
    actor->attention_info.position.y += 150.0f + TREG_F(5);
    i_this->sound.framework(0, dComIfGp_getReverb(fopAcM_GetRoomNo(actor)));
    return 1;
}

static int daOBJ_RW_IsDelete(obj_rw_class* i_this) {
    return 1;
}

static int daOBJ_RW_Delete(obj_rw_class* i_this) {
    fopAc_ac_c* actor = &i_this->enemy;
    fopAcM_GetID(actor);
    dComIfG_resDelete(&i_this->phase, "Obj_rw");

    if (i_this->HIOInit) {
        hio_set = FALSE;
        mDoHIO_DELETE_CHILD(l_HIO.id);
    }

    if (actor->heap != NULL) {
        i_this->sound.deleteObject();
    }

    return 1;
}

static int useHeapInit(fopAc_ac_c* actor) {
    obj_rw_class* i_this = (obj_rw_class*)actor;

    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes("Obj_rw", 3);
    JUT_ASSERT(603, modelData != NULL);

    i_this->model = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
    if (i_this->model == NULL) {
        return 0;
    }

    i_this->model->setUserArea((uintptr_t)i_this);
    i_this->model->getModelData()->getJointNodePointer(1)->setCallBack(nodeCallBack);
    return 1;
}

static int daOBJ_RW_Create(fopAc_ac_c* actor) {
    obj_rw_class* i_this = (obj_rw_class*)actor;
    fopAcM_ct(actor, obj_rw_class);

    int phase_state = dComIfG_resLoad(&i_this->phase, "Obj_rw");
    if (phase_state == cPhs_COMPLEATE_e) {
        OS_REPORT("Obj_rw", fopAcM_GetParam(actor));
        OS_REPORT("OBJ_RW//////////////OBJ_RW SET 1 !!\n");

        if (!fopAcM_entrySolidHeap(actor, useHeapInit, 0xFD0)) {
            OS_REPORT("//////////////OBJ_RW SET NON !!\n");
            return cPhs_ERROR_e;
        }

        OS_REPORT("//////////////OBJ_RW SET 2 !!\n");

        if (!hio_set) {
            i_this->HIOInit = TRUE;
            hio_set = TRUE;
            l_HIO.id = mDoHIO_CREATE_CHILD("いのししまる焼き", &l_HIO);
        }

        fopAcM_SetMtx(actor, i_this->model->getBaseTRMtx());
        fopAcM_SetMin(actor, -500.0f, -200.0f, -500.0f);
        fopAcM_SetMax(actor, 500.0f, 500.0f, 500.0f);

        actor->health = 200;
        actor->field_0x560 = 200;

        static dCcD_SrcSph cc_sph_src = {
            {
                {0x0, {{0x0, 0x0, 0x0}, {0xd8fbfdff, 0x3}, 0x75}}, // mObj
                {dCcD_SE_HARD_BODY, 0x0, 0x0, 0x0, 0x0}, // mGObjAt
                {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x2}, // mGObjTg
                {0x0}, // mGObjCo
            }, // mObjInf
            {
                {{0.0f, 0.0f, 0.0f}, 40.0f} // mSph
            } // mSphAttr
        };

        static dCcD_SrcCyl cc_cyl_src = {
            {
                {0x0, {{0x0, 0x0, 0x0}, {0xd8fbfdff, 0x3}, 0x75}}, // mObj
                {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0}, // mGObjAt
                {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x3}, // mGObjTg
                {0x0}, // mGObjCo
            }, // mObjInf
            {
                {
                    {0.0f, 0.0f, 0.0f}, // mCenter
                    200.0f, // mRadius
                    250.0f // mHeight
                } // mCyl
            }
        };

        i_this->ccStts.Init(0xFF, 0, actor);

        i_this->ccSph[0].Set(cc_sph_src);
        i_this->ccSph[0].SetStts(&i_this->ccStts);

        i_this->ccSph[1].Set(cc_sph_src);
        i_this->ccSph[1].SetStts(&i_this->ccStts);

        for (int i = 0; i < 4; i++) {
            i_this->ccCyl[i].Set(cc_cyl_src);
            i_this->ccCyl[i].SetStts( &i_this->ccStts);
        }

        i_this->sound.init(&actor->current.pos, &actor->eyePos, 3, 1);
        i_this->atInfo.mpSound = &i_this->sound;

        actor->field_0x564 = (fopAcM_GetParam(actor) & 0xFF0000) >> 0x10;

        int swbit = (fopAcM_GetParam(actor) & 0xFF000000) >> 0x18;
        if (swbit != 0xFF && dComIfGs_isSwitch(swbit, fopAcM_GetRoomNo(actor))) {
            i_this->field_0xe38 = 1;
            cXyz scale(1.0f, 1.0f, 1.0f);
            fopAcM_createItemForBoss(&actor->current.pos, fpcNm_ITEM_KAKERA_HEART, fopAcM_GetRoomNo(actor), &actor->shape_angle, &scale, 0.0f, 0.0f, actor->field_0x564);
        } else {
            fopAcM_createChild(PROC_OBJ_MAKI, fopAcM_GetID(actor), 0xFFFFFF01, &actor->home.pos, fopAcM_GetRoomNo(actor), &actor->home.angle, NULL, -1, NULL);
            fopAcM_OnStatus(actor, 0);
            actor->attention_info.flags = fopAc_AttnFlag_BATTLE_e;
        }

        daOBJ_RW_Execute(i_this);
    }

    return phase_state;
}

static actor_method_class l_daOBJ_RW_Method = {
    (process_method_func)daOBJ_RW_Create,
    (process_method_func)daOBJ_RW_Delete,
    (process_method_func)daOBJ_RW_Execute,
    (process_method_func)daOBJ_RW_IsDelete,
    (process_method_func)daOBJ_RW_Draw,
};

actor_process_profile_definition g_profile_OBJ_RW = {
  fpcLy_CURRENT_e,        // mLayerID
  7,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_OBJ_RW,            // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(obj_rw_class),   // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  711,                    // mPriority
  &l_daOBJ_RW_Method,     // sub_method
  0x00040100,             // mStatus
  fopAc_ENEMY_e,          // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};
