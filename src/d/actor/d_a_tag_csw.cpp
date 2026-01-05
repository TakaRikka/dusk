//
// d_a_tag_csw
//

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_tag_csw.h"
#include "d/actor/d_a_cstatue.h"
#include "d/d_cc_d.h"
#include "d/d_com_inf_game.h"
#include "d/d_bg_w.h"

static const char* l_arcName = "Lv6Warp";

#pragma push
#pragma force_active on
static dCcD_SrcCyl l_cyl_src = {
    {
        {0x0, {{0x0, 0x0, 0x0}, {0xffffffff, 0x0}, 0x19}}, // mObj
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0}, // mGObjAt
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0}, // mGObjTg
        {0x0}, // mGObjCo
    }, // mObjInf
    {
        {
            {0.0f, 0.0f, 0.0f}, // mCenter
            300.0f, // mRadius
            600.0f // mHeight
        } // mCyl
    } // mCylAttr
};
#pragma pop

#pragma push
#pragma force_active on
static dCcD_SrcCyl l_tg_src = {
    {
        {0x0, {{0x0, 0x0, 0x0}, {0x400000, 0x11}, 0x0}}, // mObj
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0}, // mGObjAt
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x6}, // mGObjTg
        {0x0}, // mGObjCo
    }, // mObjInf
    {
        {
            {0.0f, 0.0f, 0.0f}, // mCenter
            300.0f, // mRadius
            600.0f // mHeight
        } // mCyl
    } // mCylAttr
};
#pragma pop

daTagCsw_c::~daTagCsw_c() {}

static u8 const l_bmd_idx[8] = {
    0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x06,
};

static u8 const l_brk_idx[8] = {
    0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00, 0x0A,
};

static u8 const l_dbz_idx[8] = {
    0x00, 0x00, 0x00, 0x0D, 0x00, 0x00, 0x00, 0x0E,
};

void daTagCsw_c::setMtx() {
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::YrotM(current.angle.y);
    MTXCopy(mDoMtx_stack_c::get(), field_0x58c);
    field_0x574->setBaseTRMtx(mDoMtx_stack_c::get());
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y + field_0x624, current.pos.z);
    mDoMtx_stack_c::YrotM(current.angle.y);
    MTXCopy(mDoMtx_stack_c::get(), field_0x5f4);
    if (field_0x578 != NULL) {
        mDoMtx_stack_c::transS(current.pos.x + field_0x8e8.x,
                               current.pos.y + field_0x624,
                               current.pos.z + field_0x8e8.z);
        mDoMtx_stack_c::YrotM(current.angle.y);
        field_0x578->setBaseTRMtx(mDoMtx_stack_c::get());
    }
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::YrotM(current.angle.y);
    mDoMtx_stack_c::scaleM(field_0x5ec, 1.0f, field_0x5ec);
    MTXCopy(mDoMtx_stack_c::get(), field_0x5bc);
}

int daTagCsw_c::createHeap() {
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes(l_arcName, 5);
    JUT_ASSERT(303, modelData != NULL);
    field_0x574 = mDoExt_J3DModel__create(modelData, 0, 0x11000084);
    if (field_0x574 == NULL) {
        return 0;
    }
    modelData = (J3DModelData*)dComIfG_getObjectRes(l_arcName, 6);
    JUT_ASSERT(314, modelData != NULL);
    if (field_0x570 == 0 && getSw() == 6) {
        field_0x578 = NULL;
    } else {
        field_0x578 = mDoExt_J3DModel__create(modelData, 0, 0x11000084);
        if (field_0x578 == NULL) {
            return 0;
        }
    }
    field_0x8e8.x = field_0x8e8.y = field_0x8e8.z;
    setMtx();
    field_0x57c = new mDoExt_brkAnm();
    if (field_0x57c == NULL) {
        return 0;
    }

    if (!field_0x57c->init(field_0x574->getModelData(),
                           (J3DAnmTevRegKey*)dComIfG_getObjectRes(l_arcName, 9), 1, 0, 1.0f, 0, -1))
    {
        return 0;
    }
    if (field_0x578 == NULL) {
        field_0x580 = NULL;
    } else {
        field_0x580 = new mDoExt_brkAnm();
        if (field_0x580 == NULL) {
            return 0;
        }
        if (!field_0x580->init(field_0x578->getModelData(),
                               (J3DAnmTevRegKey*)dComIfG_getObjectRes(l_arcName, 10), 1, 0, 1.0f, 0,
                               -1))
        {
            return 0;
        }
    }
    field_0x584 = new dBgW();
    if (field_0x584 == NULL) {
        return 0;
    }
    if (field_0x584->Set((cBgD_t*)dComIfG_getObjectRes(l_arcName, 13), 1, &field_0x58c)) {
        return 0;
    }
    if (field_0x578 == NULL) {
        field_0x5f0 = NULL;
    } else {
        field_0x5f0 = new dBgW();
        if (field_0x5f0 == NULL) {
            return 0;
        }
        if (field_0x5f0->Set((cBgD_t*)dComIfG_getObjectRes(l_arcName, 14), 1, &field_0x5f4)) {
            return 0;
        }
    }
    field_0x588 = new dBgW();
    if (field_0x588 == NULL) {
        return 0;
    }

    if (field_0x588->Set((cBgD_t*)dComIfG_getObjectRes(l_arcName, 15), 1, &field_0x5bc)) {
        return 0;
    }
   
    return 1;
}

static int daTagCsw_c_createHeap(fopAc_ac_c* i_this) {
    return static_cast<daTagCsw_c*>(i_this)->createHeap();
}

int daTagCsw_c::chkInsideStatueStart() {
    bool rv = false;
    if (fopAcM_isSwitch(this, getSw())) {
        bool bVar1 = true;
        if (getSw() != 10) {
            if (fopAcM_isSwitch(this, getSw() + 1)) {
                bVar1 = false;
            }
        }
        if (bVar1) {
            rv = true;
        }
    }
    return rv;
}

int daTagCsw_c::create() {
    fopAcM_ct(this, daTagCsw_c);
    field_0x570 = getType();
    int rv = dComIfG_resLoad(this, l_arcName);
    if (rv == cPhs_COMPLEATE_e) {
        mItemNo = getArg0();
        current.angle.x = 0;
        field_0x624 = 0.0f;
        field_0x628 = 0.0f;
        if (field_0x570 == 0) {
            field_0x624 = 500.0f;
        } else if (chkInsideStatueStart()) {
            field_0x624 = 0.0f;
            field_0x628 = 5.0f;
            if (getSw2() != 0xff) {
                fopAcM_onSwitch(this, getSw2());
            }
        } else if (!fopAcM_isSwitch(this, getSw())) {
            field_0x624 = 0.0f;
        } else {
            field_0x624 = 500.0f;
        }
        if (!fopAcM_entrySolidHeap(this, daTagCsw_c_createHeap, 0x4e00)) {
            return 5;
        } 
        mLightOn = false;
        offLight();
        if (field_0x570 == 1 && chkInsideStatueStart()) {
            onLight();
        }
        fopAcM_SetMtx(this, field_0x58c);
        dComIfG_Bgsp().Regist(field_0x584, this);
        if (field_0x5f0 != NULL) {
            dComIfG_Bgsp().Regist(field_0x5f0, this);
        }
        field_0x62c.Init(0xff, 0, this);
        field_0x668.Set(l_cyl_src);
        field_0x668.SetStts(&field_0x62c);
        field_0x7a4.Set(l_tg_src);
        field_0x7a4.SetStts(&field_0x62c);
        field_0x8f4 = 0;
        field_0x8e2 = 0;
        field_0x5ec = 0.0f;
    }
    return rv;
}

static void* searchTagCswOut(void* param_1, void* param_2) {
    if (param_1 != NULL && fopAcM_IsActor(param_1) && fopAcM_GetProfName(param_1) == PROC_TAG_CSW &&
        static_cast<daTagCsw_c*>(param_1)->getType() == 1)
    {
        if (static_cast<daTagCsw_c*>(param_1)->getSw() ==
            static_cast<daTagCsw_c*>(param_2)->getSw())
        {
            return param_1;
        }
    }
    return NULL;
}

static void* searchCStatue(void* param_1, void* param_2) {
    if (param_1 != NULL && fopAcM_IsActor(param_1) && fopAcM_GetProfName(param_1) == PROC_CSTATUE &&
        static_cast<daCstatue_c*>(param_1)->checkNormalType())
    {
        return param_1;
    }
    return NULL;
}

int daTagCsw_c::Delete() {
    if (field_0x584 != NULL && field_0x584->ChkUsed()) {
        dComIfG_Bgsp().Release(field_0x584);
    }
    if (field_0x5f0 != NULL && field_0x5f0->ChkUsed()) {
        dComIfG_Bgsp().Release(field_0x5f0);
    }
    if (field_0x588 != NULL && field_0x588->ChkUsed()) {
        dComIfG_Bgsp().Release(field_0x588);
    }
    return dComIfG_resDelete(this, l_arcName);
}

void daTagCsw_c::onLight() {
    field_0x57c->setPlayMode(0);
    field_0x57c->setPlaySpeed(1.0f);
    if (field_0x580 != NULL) {
        field_0x580->setPlayMode(0);
        field_0x580->setPlaySpeed(1.0f);
    }
    mLightOn = true;
}

void daTagCsw_c::offLight() {
    field_0x57c->setPlayMode(1);
    field_0x57c->setPlaySpeed(-1.0f);
    if (field_0x580 != NULL) {
        field_0x580->setPlayMode(1);
        field_0x580->setPlaySpeed(-1.0f);
    }
    mLightOn = false;
}

int daTagCsw_c::execute() {
    bool bVar3 = false;
    daCstatue_c* statue = (daCstatue_c*)fopAcM_Search(searchCStatue, this);
    if (statue != NULL && statue->current.pos.absXZ(current.pos) < 75.0f &&
        statue->current.pos.y > current.pos.y - 30.0f &&
        statue->current.pos.y < current.pos.y + 200.0f)
    {
        bVar3 = true;
    }
    if (field_0x570 == 0) {
        if (bVar3) {
            if (!fopAcM_isSwitch(this, getSw())) {
                if (statue != NULL) {
                    statue->current.pos.x = current.pos.x;
                    statue->current.pos.z = current.pos.z;
                }
                if (getSw() != 6) {
                    fopAc_ac_c* local_74 = fopAcM_Search(searchTagCswOut, this);
                    statue->onWarpGround(current.pos);
                    if (cM3d_IsZero(field_0x628) && getSw2() != 0xff) {
                        fopAcM_onSwitch(this, getSw2());
                    }
                    field_0x628 = -5.0f;
                    if (!field_0x588->ChkUsed()) {
                        dComIfG_Bgsp().Regist(field_0x588, this);
                        field_0x5ec = 0;
                    } else {
                        field_0x5ec = ((500.0f - field_0x624) * 2.0f) / 500.0f;
                        if (field_0x5ec > 1.1f) {
                            field_0x5ec = 1.1f;
                        }
                    }
                    field_0x668.SetC(current.pos);
                    field_0x668.SetR(field_0x5ec * 280.0f);
                    dComIfG_Ccsp()->Set(&field_0x668);
                    if (field_0x624 == 0.0f) {
                        if (field_0x588->ChkUsed()) {
                            dComIfG_Bgsp().Release(field_0x588);
                            field_0x5ec = 1.1f;
                        }
                        if (local_74 != NULL) {
                            statue->warpStart(local_74->current.pos);
                        } else {
                            statue->warpDelete();
                        }
                        fopAcM_onSwitch(this, getSw());
                    }
                } else {
                    fopAcM_onSwitch(this, getSw());
                    field_0x8e2 = 0x3c;
                    statue->onWarpGround(current.pos);
                }
            }
        }
        if (fopAcM_isSwitch(this, getSw())) {
            offLight();
        } else {
            if ((getSw() == 0 && fopAcM_isSwitch(this, 11)) || (getSw() != 0 && fopAcM_isSwitch(this, getSw() - 1))) {
                onLight();
            }
        }
        if (field_0x570 == 0 && getSw() == 6 && field_0x8e2 != 0) {
            s16 angleDiff = (current.angle.y - statue->current.angle.y);
            s16 sVar4 = angleDiff / field_0x8e2;
            if (sVar4 > 0x800) {
                statue->current.angle.y += 0x800;
            } else if (sVar4 < -0x800) {
                statue->current.angle.y -= 0x800;
            } else {
                statue->current.angle.y += sVar4;
            }
            statue->shape_angle.y = statue->current.angle.y;
            onLight();
            field_0x8e2--;
        }
        if (!bVar3 && field_0x624 == 0.0f) {
            if (fopAcM_isSwitch(this, getSw())) {
                field_0x628 = 5.0f;
            }
        }
    } else {
        if (field_0x628 == 0.0f && field_0x624 == 0.0f && fopAcM_isSwitch(this, getSw())) {
            if (getSw2() != 0xff) {
                fopAcM_onSwitch(this, getSw2());
            }
            field_0x628 = 5.0f;
            onLight();
        }
        else if (field_0x624 == 500.0f) {
            offLight();
        }
    }
    if (field_0x628 != 0.0f) {
        if (field_0x628 > 0.0f) {
            if (field_0x624 < 500.0f) {
                field_0x624 += field_0x628;
                field_0x8e1 = true;
            } else {
                field_0x624 = 500.0f;
                field_0x628 = 0;
                if (field_0x8e1 != 0) {
                    field_0x8e1 = 0;
                    cXyz acStack_40;
                    acStack_40 = current.pos;
                    acStack_40.y += field_0x624;
                    Z2GetAudioMgr()->seStart(Z2SE_OBJ_L6_WARP_UP_ST, &acStack_40, 0, 0, 1.0f, 1.0f,
                                             -1.0f, -1.0f, 0);
                }
            }
        } else {
            if (field_0x624 > 0.0f) {
                field_0x624 += field_0x628;
                field_0x8e1 = 1;
            } else {
                field_0x624 = 0;
                field_0x628 = 0;
                if (field_0x8e1 != 0) {
                    field_0x8e1 = 0;
                    cXyz cStack_4c;
                    cStack_4c = current.pos;
                    cStack_4c.y += field_0x624;
                    Z2GetAudioMgr()->seStart(Z2SE_OBJ_L6_WARP_DW_ST, &cStack_4c, 0, 0, 1.0f, 1.0f,
                                             -1.0f, -1.0f, 0);
                }
            }
        }
        cXyz cStack_58;
        cStack_58 = current.pos;
        cStack_58.y += field_0x624;
        Z2GetAudioMgr()->seStartLevel(Z2SE_OBJ_L6_WARP_LV, &cStack_58, 0, 0, 1.0f, 1.0f,
                                      -1.0f, -1.0f, 0);
    }
    if (!cM3d_IsZero(field_0x624)) {
        cXyz cStack_64(current.pos);
        cStack_64.y += field_0x624;
        if (field_0x7a4.ChkTgHit()) {
            if (field_0x8f4 == 0 && mItemNo != 0xff && getItemBit() != 0xff) {
                if (!fopAcM_isItem(this, getItemBit())) {
                    fopAcM_createItem(&cStack_64, mItemNo, getItemBit(), fopAcM_GetRoomNo(this), 0,
                                      0, 0);
                }
            }
            Z2GetAudioMgr()->seStart(Z2SE_OBJ_L6_WARP_COL, &cStack_64, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
            field_0x8f4 = 1;
            field_0x8e8.x = current.pos.x - field_0x7a4.GetTgHitPosP()->x;
            field_0x8e8.z = current.pos.z - field_0x7a4.GetTgHitPosP()->z;
            ;
            if (!cM3d_IsZero(field_0x8e8.getSquareMag())) {
                field_0x8e8.normalize();
                field_0x8e8 *= 30.0f;
            } else {
                field_0x8e8.x = 0.0f;
                field_0x8e8.z = 0.0f;
            }
        }
        field_0x7a4.SetC(cStack_64);
        dComIfG_Ccsp()->Set(&field_0x7a4);
    }
    ;
    if (!cM3d_IsZero(field_0x8e8.getSquareMag())) {
        field_0x8e8.x *= -0.9f;
        field_0x8e8.z *= -0.9f;
    } else {
        field_0x8e8.x = 0.0f;
                field_0x8e8.z = 0.0f;
    }
    field_0x57c->play();
    if (field_0x580 != NULL) {
        field_0x580->play();
    }
    setMtx();
    field_0x584->Move();
    field_0x588->Move();
    if (field_0x5f0 != NULL) {
        field_0x5f0->Move();
    }
    field_0x62c.Move();
    return 1;
}

int daTagCsw_c::draw() {
    field_0x57c->entry(field_0x574->getModelData());
    if (field_0x578 != NULL) {
        field_0x580->entry(field_0x578->getModelData());
    }
    g_env_light.settingTevStruct(0x10, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(field_0x574, &tevStr);
    if (field_0x578 != NULL) {
        g_env_light.setLightTevColorType_MAJI(field_0x578, &tevStr);
    }
    dComIfGd_setListBG();
    mDoExt_modelUpdateDL(field_0x574);
    if (field_0x578 != NULL) {
        mDoExt_modelUpdateDL(field_0x578);
    }
    dComIfGd_setList();
    return 1;
}

static int daTagCsw_Draw(daTagCsw_c* i_this) {
    return i_this->draw();
}

static int daTagCsw_Execute(daTagCsw_c* i_this) {
    return i_this->execute();
}

static int daTagCsw_IsDelete(daTagCsw_c* i_this) {
    return 1;
}

static int daTagCsw_Delete(daTagCsw_c* i_this) {
    i_this->Delete();
    i_this->~daTagCsw_c();
    return 1;
}

static int daTagCsw_Create(fopAc_ac_c* i_this) {
    return static_cast<daTagCsw_c*>(i_this)->create();
}

static actor_method_class l_daTagCsw_Method = {
    (process_method_func)daTagCsw_Create,
    (process_method_func)daTagCsw_Delete,
    (process_method_func)daTagCsw_Execute,
    (process_method_func)daTagCsw_IsDelete,
    (process_method_func)daTagCsw_Draw,
};

actor_process_profile_definition g_profile_TAG_CSW = {
    fpcLy_CURRENT_e,
    7,
    fpcPi_CURRENT_e,
    PROC_TAG_CSW,
    &g_fpcLf_Method.base,
    sizeof(daTagCsw_c),
    0,
    0,
    &g_fopAc_Method.base,
    0x111,
    &l_daTagCsw_Method,
    0x40000,
    fopAc_ACTOR_e,
    fopAc_CULLBOX_0_e,
};
