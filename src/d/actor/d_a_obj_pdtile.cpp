/**
 * @file d_a_obj_pdtile.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_pdtile.h"
#include "SSystem/SComponent/c_math.h"
#include "d/actor/d_a_player.h"
#include "d/d_bg_w.h"
#include "d/d_cc_d.h"
#include "d/d_com_inf_game.h"

static void rideCallBack(dBgW* param_1, fopAc_ac_c* param_2, fopAc_ac_c* param_3) {
    static_cast<daObjPDtile_c*>(param_2)->rideActor(param_3);
}

static char* l_arcName4 = "P_Dtile";

static u32 l_dzbidx4[2] = {
    9, 8,
};

static u32 l_bmdidx4[2] = {
    5, 4,
};

static cull_box l_cull_box4[2] = {
    {{-50.0f, -100.0f, -50.0f}, {50.0f, 20.49f, 50.0f}},
    {{-160.0f, -460.0f, -160.0f}, {160.0f, 20.0f, 160.0f}},
};

static char* l_arcName7 = "P_Dtile00";

static u32 l_dzbidx7 = 7;

static u32 l_bmdidx7 = 4;

static cull_box l_cull_box7 = {
    {-200.0f, -700.0f, -200.0f},
    {200.0f, 50.0f, 200.0f},
};

static char* l_arcName9 = "Lv9_Dtile";

static u32 l_dzbidx9 = 7;

static u32 l_bmdidx9 = 4;

static cull_box l_cull_box9 = {
    {-200.0f, -650.0f, -200.0f},
    {200.0f, 50.0f, 200.0f},
};

int daObjPDtile_c::create1st() {
    mType = (Type)getType();
    if (mType == TYPE_4) {
        mArcName = l_arcName9;
        mDzbId = l_dzbidx9;
        mBmdId = l_bmdidx9;
        mCullBox = &l_cull_box9;
        field_0x7cc = 1500.0f;
    } else if (mType == TYPE_2) {
        mArcName = l_arcName7;
        mDzbId = l_dzbidx7;
        mBmdId = l_bmdidx7;
        mCullBox = &l_cull_box7;
        field_0x7cc = 600.0f;
    } else if (mType == TYPE_0) {
        mArcName = l_arcName4;
        mDzbId = l_dzbidx4[0];
        mBmdId = l_bmdidx4[0];
        mCullBox = &l_cull_box4[0];
        field_0x7cc = 450.0f;
    } else {
        mArcName = l_arcName4;
        mDzbId = l_dzbidx4[1];
        mBmdId = l_bmdidx4[1];
        mCullBox = &l_cull_box4[1];
        field_0x7cc = 450.0f;
    }
    int rv = dComIfG_resLoad(this, mArcName);
    if (rv == cPhs_COMPLEATE_e) {
        setMtx();
        MoveBGActor_SetFunc setFunc;
        if (mType != TYPE_4) {
            setFunc = dBgS_MoveBGProc_TypicalRotY;
        } else {
            setFunc = NULL;
        }
        rv = MoveBGCreate(mArcName, mDzbId, setFunc, 0x12a0, &mBgMtx);
        if (rv == cPhs_ERROR_e) {
            return rv;
        }
        field_0x620.z = 0.0f;
        field_0x620.y = 0.0f;
        field_0x620.x = 0.0f;
    }
    return rv;
}

void daObjPDtile_c::setMtx() {
    mDoMtx_stack_c::transS(current.pos.x + field_0x620.x,
                           current.pos.y,
                           current.pos.z + field_0x620.z);
    mDoMtx_stack_c::XYZrotM(current.angle.x, current.angle.y, current.angle.z);
    MTXCopy(mDoMtx_stack_c::get(), field_0x5d8);
    if (mType != TYPE_2 && mType != TYPE_4) {
        MTXCopy(mDoMtx_stack_c::get(), mBgMtx);
    } else {
        mDoMtx_stack_c::transS(current.pos.x, current.pos.y,
                               current.pos.z);
        mDoMtx_stack_c::XYZrotM(0, current.angle.y, 0);
        if (mType == TYPE_2) {
            cXyz local_1c;
            local_1c.x = 1.01f;
            local_1c.y = 1.0f;
            local_1c.z = 1.01f;
            mDoMtx_stack_c::scaleM(local_1c);
        }
        MTXCopy(mDoMtx_stack_c::get(), mBgMtx);
    }
}

void daObjPDtile_c::rideActor(fopAc_ac_c* param_1) {
    if (param_1 == daPy_getLinkPlayerActorClass() ||
        fopAcM_GetProfName(param_1) == PROC_B_GG)
    {
        if (mType == TYPE_0) {
            field_0x7b0 = 200;
        } else {
            field_0x7b0 = 100;
        }
        field_0x7b5 = 1;
        field_0x61c = 2.5f;
        if (field_0x610 == 0) {
            if (mType == TYPE_2) {
                field_0x610 = 5;
                mSoundObj.startSound(Z2SE_OBJ_FLR_FALL_START, 0, -1);
            } else if ((mType == TYPE_1) || (mType == TYPE_3)) {
                field_0x610 = 7;
                field_0x7d8 = 0;
                Z2GetAudioMgr()->seStart(Z2SE_OBJ_SINK_FLOOR_DW_TRG, &current.pos, 0, 0, 1.0f, 1.0f, -1.0f,
                                 -1.0f, 0);
            } else {
                field_0x610 = 1;
                if (mType == TYPE_4) {
                    mSoundObj.startSound(Z2SE_OBJ_FLR_FALL_START, 0, -1);
                }
            }
        }
        if (param_1 == daPy_getLinkPlayerActorClass()) {
            if (daPy_getLinkPlayerActorClass()->checkBootsOrArmorHeavy()) {
                field_0x630 = 1;
            }
        }
    }
    if (!cM3d_IsZero(param_1->speed.x) || !cM3d_IsZero(param_1->speed.z)) {
        field_0x61c = 2.5f;
        if (mType == TYPE_0) {
            field_0x7b0 = 200;
        } else {
            field_0x7b0 = 100;
        }
    }
    if (field_0x614 == 0) {
        field_0x7ac = 0;
        if (mType == TYPE_0) {
            field_0x7b0 = 200;
        } else {
            field_0x7b0 = 100;
        }
        field_0x7b2 = cM_rndF(65535.0f);
        field_0x61c = 2.5f;
    }
    field_0x614 = 1;
    field_0x7b4 = 1;
}

int daObjPDtile_c::CreateHeap() {
    J3DModelData* model_data = (J3DModelData*)dComIfG_getObjectRes(mArcName, mBmdId);
    JUT_ASSERT(557, model_data != NULL);
    mModel = mDoExt_J3DModel__create(model_data, 0x80000, 0x11000084);
    return mModel != 0 ? TRUE : FALSE;
}

static dCcD_SrcCyl cc_cyl_src = {
    {
        {0x0, {{0x0, 0x0, 0x0}, {0x8020, 0x11}, 0x0}}, // mObj
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0}, // mGObjAt
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x2}, // mGObjTg
        {0x0}, // mGObjCo
    }, // mObjInf
    {
        {
            {0.0f, 0.0f, 0.0f}, // mCenter
            70.0f, // mRadius
            100.0f // mHeight
        } // mCyl
    }
};

int daObjPDtile_c::Create() {
    mModel->setBaseTRMtx(field_0x5d8);
    fopAcM_SetMtx(this, field_0x5d8);
    fopAcM_setCullSizeBox(this, 
        mCullBox->min.x, mCullBox->min.y, mCullBox->min.z,
        mCullBox->max.x, mCullBox->max.y, mCullBox->max.z);
    mpBgW->SetRideCallback(rideCallBack);
    field_0x610 = 0;
    field_0x614 = 0;
    field_0x618 = 0;
    field_0x61c = 0.0f;
    field_0x62c = 0.0f;
    field_0x630 = 0;
    mStts.Init(0xff, 0, this);
    mCyl.Set(cc_cyl_src);
    mCyl.SetStts(&mStts);
    field_0x7ac = 0;
    field_0x7b0 = 0;
    field_0x7b2 = cM_rndF(65535.0f);
    field_0x7b4 = 0;
    field_0x7d0 = 0;
    field_0x7d8 = 0;
    mSoundObj.init(&current.pos, 1);
    return 1;
}

int daObjPDtile_c::Execute(Mtx** param_1) {
    if (mCyl.ChkTgHit()) {
        field_0x614 = 1;
        if (mType == TYPE_0) {
            field_0x7b0 = 200;
        } else {
            field_0x7b0 = 100;
        }
        field_0x61c = 2.5f;
        if (mType == TYPE_2 || mType == TYPE_4) {
            field_0x610 = 2;
            field_0x62c = 0.3f;
        }
    }
    if (field_0x610 == 1 || field_0x614 != 0 ||
        field_0x7b0 != 0)
    {
        field_0x620.x = cM_rndFX(field_0x61c);
        field_0x620.z = cM_rndFX(field_0x61c);
        field_0x7ac++;
        current.angle.x = field_0x7b0 * cM_ssin(field_0x7ac * 10000);
        current.angle.z = field_0x7b0 *
                      cM_ssin(field_0x7b2 + field_0x7ac * 10000);
        field_0x7b0 *= 0.95f;
        field_0x61c *= 0.95f;
        if (field_0x7b0 < 32) {
            field_0x7b0 = 0;
            field_0x7ac = 0;
        }
        if (field_0x610 == 1) {
            field_0x618++;
            if (field_0x618 >= 4.5f)
            {
                field_0x610 = 2;
                field_0x62c = 0.3f;
            }
        }
        if ((mType == TYPE_2 || mType == TYPE_4) && field_0x7b5 == 0 && field_0x610 != 5 &&
            field_0x610 != 6 && field_0x610 != 2 && field_0x610 != 4 && field_0x7b0 != 0)
        {
            mSoundObj.startLevelSound(Z2SE_OBJ_FLR_GURAGURA, 0, -1);
        }
    } else {
        field_0x620 *= -0.8f;
        field_0x7ac = 0;
    }
    bool bVar5 = false;
    cXyz local_f0(daPy_getLinkPlayerActorClass()->current.pos);
    f32 diffx = local_f0.x - current.pos.x;
    f32 diffz = local_f0.z - current.pos.z;
    if (diffx * diffx +
                diffz * diffz <
            14400.0f &&
        local_f0.y > current.pos.y && local_f0.y < current.pos.y + 450.0f)
    {
        bVar5 = true;
    }
    if (field_0x610 == 2 || field_0x610 == 5) {
        if (mType == TYPE_3 && field_0x7b5 == 0 && !bVar5) {
            field_0x7b8++;
            if (75 <= field_0x7b8) {
                field_0x610 = 3;
            }
        } else {
            current.pos.y -= field_0x62c;
            if ((field_0x610 == 5) &&
                (current.pos.y <= home.pos.y - 30.0f))
            {
                current.pos.y = home.pos.y - 30.0f;
                field_0x610 = 6;
                field_0x7d4 = 0;
                field_0x62c *= -0.8f;
            }
            if (current.pos.y <= home.pos.y - field_0x7cc) {
                current.pos.y = home.pos.y - field_0x7cc;
                if (mType == TYPE_3 && field_0x7b5 == 0) {
                    field_0x7b8++;
                    if (75 <= field_0x7b8) {
                        field_0x7b8 = 75;
                        if (!bVar5) {
                            field_0x610 = 3;
                        }
                    }
                } else {
                    field_0x7b8 = 0;
                }
                if (mType == TYPE_2 || mType == TYPE_4) {
                    field_0x610 = 4;
                    fopAcM_OnStatus(this, 0x4000);
                }
            } else {
                field_0x7b8 = 0;
                if (mType != TYPE_2 && mType != TYPE_4) {
                    Z2GetAudioMgr()->seStartLevel(Z2SE_OBJ_SINK_FLOOR_DW, &current.pos, 0, 0, 1.0f, 1.0f,
                                          -1.0f, -1.0f, 0);
                }
                if (field_0x610 == 5) {
                    if (field_0x630 != 0) {
                        field_0x62c += 5.0f;
                    } else {
                        field_0x62c += 3.0f;
                    }
                } else if (mType == 2 && field_0x630 != 0) {
                    field_0x62c += 1.0f;
                    if (field_0x62c >= 15.0f) {
                        field_0x62c = 15.0f;
                    }
                } else if (mType == 4) {
                    field_0x62c += 2.0f;
                    if (field_0x630 != 0) {
                        field_0x62c += 2.0f;
                    }
                    if (field_0x62c >= 30.0f) {
                        field_0x62c = 30.0f;
                    }
                } else {
                    field_0x62c += 0.03f;
                    if (field_0x630 != 0) {
                        field_0x62c += 0.03f;
                    }
                    if (field_0x62c >= 10.0f) {
                        field_0x62c = 10.0f;
                    }
                }
            }
        }
    }
    if (field_0x610 == 6) {
        current.pos.y -= field_0x62c;
        field_0x7d4++;
        if (field_0x62c > 0.0f &&
            current.pos.y <= home.pos.y - 30.0f)
        {
            if (field_0x630 == 0 && 30 < field_0x7d4) {
                field_0x610 = 2;
                field_0x62c = 0;
            } else if (field_0x630 != 0 && field_0x7d4 > 5) {
                field_0x610 = 2;
                field_0x62c = 0;
            } else {
                field_0x62c *= -0.2f;
            }
        } else {
            if (field_0x630 != 0) {
                field_0x62c += 5.0f;
            } else {
                field_0x62c += 3.0f;
            }
        }
    }
    if (field_0x610 == 3 && mType == TYPE_3) {
        if (field_0x7b5 == 1) {
            field_0x610 = 2;
            if (mType == TYPE_2 && field_0x630 != 0) {
                field_0x62c = 0.3f;
            } else if (mType == 4) {
                field_0x62c = 2.0f;
            } else {
                field_0x62c = 0.3f;
            }
        }
        current.pos.y += 1.5f;
        Z2GetAudioMgr()->seStartLevel(Z2SE_OBJ_SINK_FLOOR_UP, &current.pos, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f,
                              0);
        if (current.pos.y >= home.pos.y) {
            current.pos.y = home.pos.y;
            field_0x610 = 0;
        }
    }
    if (field_0x610 == 4) {
        if (mpBgW->ChkUsed()) {
            dComIfG_Bgsp().Release(mpBgW);
        }
        if (field_0x7d0 != 0) {
            field_0x62c *= 2.0f;
        }
        field_0x7d0 = 0;
        if (mType == TYPE_2 && field_0x630 != 0) {
            field_0x62c += 7.0f;
        } else if (mType == 4) {
            field_0x62c += 10.0f;
        } else {
            field_0x62c += 5.0f;
        }
        fopAc_ac_c* player = dComIfGp_getPlayer(0);
        if (field_0x62c >= -player->maxFallSpeed + 10.0f) {
            field_0x62c = -player->maxFallSpeed + 10.0f;
        }
        current.pos.y -= field_0x62c;
        if (mType != TYPE_2 && mType != TYPE_4) {
            Z2GetAudioMgr()->seStartLevel(Z2SE_OBJ_SINK_FLOOR_DW, &current.pos, 0, 0, 1.0f, 1.0f, -1.0f,
                                  -1.0f, 0);
        }
        dBgS_ObjGndChk adStack_e4;
        adStack_e4.SetActorPid(base.base.id);
        adStack_e4.SetPos(&current.pos);
        dComIfG_Bgsp().GroundCross(&adStack_e4);
        if (home.pos.y - current.pos.y < -3000.0f) {
            fopAcM_delete(this);
        }
        field_0x7b0 = 0;
        field_0x614 = 0;
    }
    if (field_0x610 == 7) {
        if (field_0x7d8 < 5.0f) {
            current.pos.y = home.pos.y - (30.0f * field_0x7d8) / 5.0f;
        } else {
            current.pos.y = home.pos.y - 30.0f * (10.0f - field_0x7d8) / 5.0f;
        }
        field_0x7d8++;
        if (field_0x7d8 >= 10) {
            current.pos.y = home.pos.y;
            field_0x610 = 2;
            field_0x62c = 0.3f;
        }
    }
    if ((mType == TYPE_2 || mType == TYPE_4) &&
        (field_0x610 == 6 || field_0x610 == 5 || field_0x610 == 2))
    {
        // Fake match to load field_0x62c first.
        f32 xx = *(vf32*)&field_0x62c;
        u32 ff;
        ff = xx = (127 * xx) / 15;
        mSoundObj.startLevelSound(Z2SE_OBJ_FLR_FALLING, ff, -1);
    }
    setMtx();
    mModel->setBaseTRMtx(field_0x5d8);
    *param_1 = &mBgMtx;
    mCyl.SetC(current.pos);
    dComIfG_Ccsp()->Set(&mCyl);
    if (field_0x7b4 == 0) {
        field_0x614 = 0;
    }
    field_0x7b4 = 0;
    field_0x630 = 0;
    field_0x7b5 = 0;
    mSoundObj.framework(0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
    return 1;
}

int daObjPDtile_c::Draw() {
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    if (field_0x610 == 4 && player != NULL) {
        cXyz local_48(player->current.pos);
        f32 diffZ = local_48.z - current.pos.z;
        f32 diffX = fabsf(local_48.x - current.pos.x);
        if (diffX < 200.0f && fabsf(diffZ) < 200.0f && local_48.y < current.pos.y)
        {
            current.pos.y = local_48.y - field_0x62c;
            field_0x7d0 = 1;
            setMtx();
        } else {
            field_0x7d0 = 0;
        }
    }
    g_env_light.settingTevStruct(0x10, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mModel, &tevStr);
    dComIfGd_setListBG();
    mDoExt_modelUpdateDL(mModel);
    dComIfGd_setList();
    return 1;
}

int daObjPDtile_c::Delete() {
    dComIfG_resDelete(this, mArcName);
    mSoundObj.deleteObject();
    return 1;
}

static int daObjPDtile_create1st(daObjPDtile_c* i_this) {
    fopAcM_ct(i_this,daObjPDtile_c);
    return i_this->create1st();
}

static int daObjPDtile_MoveBGDelete(daObjPDtile_c* i_this) {
    return i_this->MoveBGDelete();
}

static int daObjPDtile_MoveBGExecute(daObjPDtile_c* i_this) {
    return i_this->MoveBGExecute();
}

static int daObjPDtile_MoveBGDraw(daObjPDtile_c* i_this) {
    return i_this->MoveBGDraw();
}


static actor_method_class daObjPDtile_METHODS = {
    (process_method_func)daObjPDtile_create1st,
    (process_method_func)daObjPDtile_MoveBGDelete,
    (process_method_func)daObjPDtile_MoveBGExecute,
    NULL,
    (process_method_func)daObjPDtile_MoveBGDraw,
};

actor_process_profile_definition g_profile_Obj_PDtile = {
  fpcLy_CURRENT_e,        // mLayerID
  3,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_Obj_PDtile,        // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(daObjPDtile_c),  // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  670,                    // mPriority
  &daObjPDtile_METHODS,   // sub_method
  0x00040100,             // mStatus
  fopAc_ACTOR_e,          // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};
