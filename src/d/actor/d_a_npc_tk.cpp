/**
* @file d_a_npc_tk.cpp
 *
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_npc_tk.h"
#include "Z2AudioLib/Z2Instances.h"
#include "d/actor/d_a_ni.h"
#include "d/actor/d_a_npc_hanjo.h"
#include "d/actor/d_a_npc_moir.h"
#include "d/actor/d_a_obj_kago.h"
#include "d/actor/d_a_obj_pumpkin.h"
#include "d/d_camera.h"
#include "d/d_cc_d.h"
#include "d/d_s_play.h"
#include "f_op/f_op_camera_mng.h"

class daNPC_TK_HIO_c : public JORReflexible {
public:
    daNPC_TK_HIO_c();
    virtual ~daNPC_TK_HIO_c() {}

    void genMessage(JORMContext*) {}

    s8 field_0x4;
    f32 field_0x8;
    f32 field_0xc;
    f32 field_0x10;
    f32 field_0x14;
    f32 field_0x18;
    f32 field_0x1c;
    f32 field_0x20;
    f32 field_0x24;
    f32 field_0x28;
    f32 mFlySpeed;
    f32 field_0x30;
    f32 field_0x34;
    f32 field_0x38;
    u8 field_0x3c;
};

daNPC_TK_HIO_c::daNPC_TK_HIO_c() {
    field_0x4 = -1;
    field_0x8 = 1.0f;
    field_0xc = 19.0f;
    field_0x10 = 38.0f;
    field_0x14 = 91.0f;
    field_0x18 = 90.0f;
    field_0x1c = 90.0f;
    field_0x20 = 30.0f;
    field_0x24 = 20.0f;
    field_0x28 = 50.0f;
    mFlySpeed = 15.0f;
    field_0x30 = 256.0f;
    field_0x34 = 700.0f;
    field_0x38 = 4096.0f;
    field_0x3c = 1;
}

void daNPC_TK_c::setBck(int param_0, u8 param_1, f32 param_2, f32 param_3) {
    mpMorf->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("Npc_tk", param_0), param_1, param_2,
                   param_3, 0.0f, -1.0f);
}

bool daNPC_TK_c::checkBck(int param_0) {
    if (mpMorf->getAnm() == dComIfG_getObjectRes("Npc_tk", param_0)) {
        return true;
    } else {
        return false;
    }
}

int daNPC_TK_c::draw() {
    J3DModel* pJVar1 = mpMorf->getModel();
    g_env_light.settingTevStruct(0, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(pJVar1, &tevStr);
    if (field_0x6c0 == 0) {
        mpMorf->entryDL();
        cXyz shadowPos;
        shadowPos.set(current.pos.x, current.pos.y + 100.0f, current.pos.z);
        field_0x688 = dComIfGd_setShadow(field_0x688, 1, pJVar1, &shadowPos, 400.0f, 0.0f,
                                         current.pos.y, mAcch.GetGroundH(), mAcch.m_gnd, &tevStr, 0,
                                         1.0f, dDlst_shadowControl_c::getSimpleTex());
    }
    return 1;
}

static int daNPC_TK_Draw(daNPC_TK_c* i_this) {
    return i_this->draw();
}

int daNPC_TK_c::checkBeforeBg() {
    s32 rv = 0;

    dBgS_LinChk linChk;
    cXyz linChkEnd;
    f32 multiplier = 400.0f;
    linChkEnd = current.pos;
    f32 cosX = cM_scos(shape_angle.x) * multiplier;
    f32 sinX = cM_ssin(shape_angle.x) * -multiplier;
    linChkEnd.x += cosX * cM_ssin(shape_angle.y);
    linChkEnd.z += cosX * cM_scos(shape_angle.y);  // seems like a bug?
    linChk.Set(&current.pos, &linChkEnd, NULL);

    if (dComIfG_Bgsp().LineCross(&linChk) != 0) {
        rv = 1;
    }

    dBgS_GndChk gndChk;
    cXyz gndChkPos = current.pos;
    gndChkPos.y += 100.0f;
    gndChk.SetPos(&gndChkPos);
    linChkEnd.y = dComIfG_Bgsp().GroundCross(&gndChk);
    if (linChkEnd.y != -G_CM3D_F_INF) {
        if (current.pos.y < linChkEnd.y) {
            if (speed.y < 0.0f) {
                speed.y = 0.0f;
            }
            current.pos.y = linChkEnd.y;
        }
        if (speed.y < 0.0f && current.pos.y - 300.0f < linChkEnd.y) {
            rv = 2;
        }
    }

    return rv;
}

void daNPC_TK_c::setActionMode(int param_0) {
    if (mActionType != param_0) {
        mAcch.SetGroundUpY(0.0f);
        field_0x690 = mActionType;
        mActionType = param_0;
        field_0x694 = 0;
        field_0x6c5 = 0;
        current.angle.y = shape_angle.y;
        fopAcM_OnStatus(this, fopAcM_STATUS_UNK_0x4000);
    }
}

f32 daNPC_TK_c::setAddCalcSpeedXZ(cXyz& param_0, cXyz const& param_1, f32 param_2, f32 param_3,
                                  f32 param_4) {
    cXyz unkXyz1 = param_1 - param_0;
    f32 unkFloat1 = unkXyz1.abs();
    cXyz result(0.0f, 0.0f, 0.0f);
    if (param_0 != param_1 && unkFloat1 >= param_4) {
        unkFloat1 = unkFloat1 * param_2;
        unkXyz1 *= param_2;
        if (!cLib_IsZero(unkFloat1)) {
            if (unkFloat1 > param_3) {
                result = unkXyz1 * param_3 / unkFloat1;
            } else {
                if (unkFloat1 < param_4) {
                    result = unkXyz1 * param_4 / unkFloat1;
                }
            }
        }
    }
    speedF = result.absXZ();

    return result.y;
}

cXyz daNPC_TK_c::chaseTargetPos(cXyz param_0, f32 param_1, f32 param_2, s16 param_3) {
    s16 unkInt1 = 0x100;
    if (param_3 < unkInt1) {
        unkInt1 = param_3;
    }

    cLib_addCalcAngleS(&shape_angle.y, cLib_targetAngleY(&current.pos, &param_0), 8, param_3,
                       unkInt1);
    current.angle.y = shape_angle.y;
    f32 dVar3 = setAddCalcSpeedXZ(current.pos, param_0, 8.0f, param_1, 1.0f);
    if (dVar3 > param_2) {
        dVar3 = param_2;
    }
    if (dVar3 < -param_2) {
        dVar3 = -param_2;
    }
    cLib_chaseF(&speed.y, dVar3, 3.0f);
    return param_0 - current.pos;
}

fopAc_ac_c* daNPC_TK_c::getMasterPointer() {
    if (mpMaster != NULL) {
        return mpMaster;
    } else {
        return daPy_getPlayerActorClass();
    }
}

void daNPC_TK_c::setMasterShoulder(cXyz* o_pos) {
    if (mpMaster != NULL) {
        mIsHanjoHand = 1;
        *o_pos = getHanjoHandPos();
    } else {
        cXyz pos;
        MtxP itemMtx = daPy_getPlayerActorClass()->getLeftItemMatrix();
        pos.set(itemMtx[0][3], itemMtx[1][3], itemMtx[2][3]);
        *o_pos = pos;
    }
}

void daNPC_TK_c::setAwayAction(int param_0) {
    if (mActionType == 3 || mActionType == 5) {
        setBck(7, 2, 3.0f, 2.0f);
        setActionMode(3);
        field_0x694 = 2;
        field_0x6b0 = 30;

        speedF = -20.0f;
        speed.y = 10.0f;

        field_0x698 = param_0;

        if (shape_angle.x > 0) {
            shape_angle.x = 0;
        }
    }
}

void daNPC_TK_c::setFlySE() {
    if (checkBck(6) != 0) {
        if (mpMorf->checkFrame(10.0f) != 0) {
            mSound.startCreatureSound(Z2SE_HAWK_WING, 0, -1);
        }
    } else {
        if (checkBck(8) != 0) {
            mSound.startCreatureSoundLevel(Z2SE_HAWK_GLIDE, 0, -1);
        } else {
            if (checkBck(7) != 0) {
                if (mpMorf->checkFrame(5.0f) != 0) {
                    mSound.startCreatureSound(Z2SE_HAWK_HOVER, 0, -1);
                }
            } else {
                if (checkBck(9) != 0 && mpMorf->checkFrame(7.0f) != 0) {
                    mSound.startCreatureSound(Z2SE_HAWK_LANDING, 0, -1);
                }
            }
        }
    }
}

static s16 m_near_angle;

static fopAc_ac_c* m_near_actor;

static u8 g_isHioChildInitted;

static daNPC_TK_HIO_c l_HIO;

static s32 checkRangeOfTake(fopAc_ac_c* param_0, fopAc_ac_c* param_1) {
    csXyz local_48 = param_0->shape_angle;
    cXyz pos0 = param_0->current.pos;
    cXyz pos1 = param_1->current.pos;
    pos1.y += ((daNPC_TK_c*)param_0)->getTakePosY() + 30.0f;
    s16 distAngleY = cLib_distanceAngleS(local_48.y, cLib_targetAngleY(&pos0, &pos1));
    if (distAngleY > (s16)l_HIO.field_0x38) {
        return 0;
    }

    s16 distAngleX = cLib_distanceAngleS(local_48.x, -cLib_targetAngleX(&pos0, &pos1));
    if (distAngleX > 0x1000) {
        return 0;
    }

    if (m_near_angle > distAngleY + distAngleX) {
        m_near_angle = distAngleY + distAngleX;
        m_near_actor = param_1;
    }

    return 1;
}

static void* s_obj_sub(void* param_0, void* param_1) {
    if (fopAcM_IsActor(param_0) != 0 &&
        (fopAcM_GetName(param_0) == PROC_NI ||
         fopAcM_CheckCarryType((fopAc_ac_c*)param_0, fopAcM_CARRY_CHICKEN) != 0))
    {
        checkRangeOfTake((fopAc_ac_c*)param_1, (fopAc_ac_c*)param_0);
    }
    return NULL;
}

static void* s_hanjo(void* param_0, void* param_1) {
    if (fopAcM_IsActor(param_0) != 0 && fopAcM_GetName(param_0) == PROC_NPC_HANJO) {
        return param_0;
    } else {
        return 0;
    }
}

void daNPC_TK_c::executeFly() {
    field_0x6bd = 1;

    switch (field_0x694) {
    case 0:
        field_0x694 = 1;
        field_0x6b0 = (u8)(cM_rndF(100.0f) + 100.0f);
        field_0x6b4 = (u8)(cM_rndF(200.0f) + 500.0f);
        setBck(6, 2, 3.0f, 1.0f);
        field_0x69c = l_HIO.field_0x30;
        field_0x698 = 0;
    case 1:
        cLib_chaseF(&speed.y, 0.0f, 3.0f);
        if (checkBck(6) != 0) {
            current.pos.y = current.pos.y - 1.0f;
            if (field_0x6b0 == 0 || home.pos.y - current.pos.y > 50.0f) {
                setBck(8, 2, 10.0f, 1.0f);
                field_0x6b0 = (u8)(cM_rndF(100.0f) + 100.0f);
            }
        } else {
            if (mpMorf->getFrame() >= 7.0f && mpMorf->getFrame() <= 27.0f) {
                current.pos.y += 1.0f;
            }
            if (field_0x6b0 == 0) {
                setBck(6, 2, 3.0f, 1.0f);
                field_0x6b0 = (u8)(cM_rndF(100.0f) + 100.0f);
            }
        }
        if (field_0x6b4 == 0) {
            field_0x6b4 = (u8)(cM_rndF(200.0f) + 500.0f);
            field_0x698 = (field_0x698 + 1) & 1;
        }

        if ((home.pos - current.pos).absXZ() > 2000.0f) {
            s16 angleDiff = cLib_targetAngleY(&current.pos, &home.pos) - shape_angle.y;
            if (angleDiff < 0) {
                field_0x698 = 1;
            } else {
                field_0x698 = 0;
            }
        }

        if (field_0x698 == 0) {
            cLib_chaseS(&field_0x69c, l_HIO.field_0x30, 0x10);
        } else {
            cLib_chaseS(&field_0x69c, -l_HIO.field_0x30, 0x10);
        }
        shape_angle.y = current.angle.y = current.angle.y + field_0x69c;
        speedF = l_HIO.mFlySpeed;

        break;
    }

    if (field_0x6c4 == 0) {
        mpMaster = (daNpc_Hanjo_c*)fpcM_Search(s_hanjo, this);
        if (mpMaster != NULL) {
            field_0x6c4 = 1;
            if (mpMaster->getType() != 1) {
                mpMaster = NULL;
            }
        }

        if (mpMaster != NULL
               /* dSv_event_flag_c::F_0207 - Ordon Village - Viewed Hanch, the Hawker cutscene */
            && dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[0xCF])) {
            setActionMode(6);
            mSphere.OffCoSetBit();
        }
    }
}

void daNPC_TK_c::initPerchDemo(int param_0) {
    s16 masterAngleY2;
    s16 masterAngleY = getMasterPointer()->shape_angle.y;
    masterAngleY2 = masterAngleY;
    if (masterAngleY > -0x2000 && masterAngleY < 0x7000) {
        if (masterAngleY < 0x2800) {
            masterAngleY2 = -0x2000;
        } else {
            masterAngleY2 = 0x7000;
        }
    }

    cXyz commonXyz1;
    s16 targetAngleY;
    switch (param_0) {
    case 0:
        speedF = 0.0f;
        speed.y = 0.0f;
        if (mpPath1 == NULL) {
            current.pos = getMasterPointer()->current.pos;
            current.pos.y += 2000.0f;
            current.pos.z += 2000.0f;
        } else {
            mPathStep2 = 0;
            current.pos = dPath_GetPnt(mpPath1, mPathStep2)->m_position;
        }

        break;
    case 1:
        field_0x698 = 0;
        field_0x67c = 0.0f;
        field_0x6c2 = 0;
        shape_angle.z = 0;
        shape_angle.x = 0;
        speedF = 0.0f;
        speed.y = 0.0f;
        field_0x694 = 0;
        commonXyz1.set(0.0f, 3000.0f, -5000.0f);

        cLib_offsetPos(&field_0x604, &getMasterPointer()->current.pos, masterAngleY2, &commonXyz1);

        if (mpPath1 == NULL) {
            commonXyz1.set(0.0f, 3000.0f, -5000.0f);
            cLib_offsetPos(&field_0x604, &getMasterPointer()->current.pos, masterAngleY2,
                           &commonXyz1);

            commonXyz1.set(-350.0f, 0.0f, -600.0f);
            cLib_offsetPos(&current.pos, &field_0x604, masterAngleY2, &commonXyz1);

            current.angle.y = (s32)masterAngleY2 + 0x4000;
            shape_angle.y = masterAngleY2 + 0x4000;
            field_0x69e = 0;
            field_0x6b4 = 40;
            field_0x678 = 5.0f;
            field_0x67c = 30.0f;
        } else {
            if (l_HIO.field_0x3c) {
                mPathStep2 = cM_rndFX(5.0f);

                if (mPathStep2 < 0) {
                    mPathStep2 += mpPath1->m_num;
                }

                if (mPathStep2 >= mpPath1->m_num || mPathStep2 < 0) {
                    mPathStep2 = 0;
                }
            } else {
                mPathStep2 = 0;
            }

            current.pos = dPath_GetPnt(mpPath1, mPathStep2)->m_position;
            mPathStep2++;

            if (mPathStep2 >= mpPath1->m_num) {
                mPathStep2 = 0;
            }

            commonXyz1 = dPath_GetPnt(mpPath1, mPathStep2)->m_position;
            targetAngleY = cLib_targetAngleY(&current.pos, &commonXyz1);
            current.angle.y = targetAngleY;
            shape_angle.y = targetAngleY;

            field_0x6b4 = 180;
            field_0x6b0 = field_0x6b4 + l_HIO.field_0x18;
            speedF = l_HIO.field_0x24;
        }

        break;
    case 2:
        field_0x698 = 0;
        field_0x694 = 0;

        setBck(8, 2, 3.0f, 1.0f);

        speed.y = -32.0f;
        speedF = 20.0f;

        commonXyz1.set(100.0f, 150.0f, 0.0f);
        cLib_offsetPos(&field_0x604, &getMasterPointer()->current.pos, masterAngleY, &commonXyz1);

        commonXyz1.set(400.0f, 500.0f, -600.0f);
        cLib_offsetPos(&current.pos, &field_0x604, masterAngleY, &commonXyz1);

        shape_angle.set(0, 0, 0);

        targetAngleY = cLib_targetAngleY(&current.pos, &field_0x604);
        current.angle.y = targetAngleY;
        shape_angle.y = targetAngleY;

        break;
    case 3:
        field_0x694 = 0;

        setBck(7, 2, 5.0f, 1.0f);

        field_0x67c = 0.0f;

        break;
    }
}

bool daNPC_TK_c::executePerchDemo(int param_0) {
    cXyz targetPos;
    cXyz pathPos;
    cXyz masterPos;

    fopAc_ac_c* unusedMaster = getMasterPointer();

    field_0x698 = field_0x698 + 1;

    switch (param_0) {
    case 0:
        return true;
    case 1:
        field_0x6c0 = 1;

        if (field_0x6b4 == 150) {
            fopAcM_monsSeStart(this, Z2SE_TOBIKUSA_ECHO, 0);
        }

        if (mpPath1 == NULL) {
            targetPos = chaseTargetPos(field_0x604, field_0x678, 20.0f, 0x80);

            if ((field_0x698 & 0x10) != 0) {
                cLib_chaseF(&field_0x67c, 5.0f, 1.0f);
            } else {
                cLib_chaseF(&field_0x67c, -5.0f, 1.0f);
            }

            current.pos.y = current.pos.y + field_0x67c;

            if (abs(cLib_targetAngleY(&current.pos, &field_0x604) - shape_angle.y) <= 0x800) {
                if (field_0x6c2 == 0) {
                    mSound.startCreatureVoice(Z2SE_HAWK_V_FIND_LINK, -1);
                    field_0x6c2 = 1;
                }

                masterPos = getMasterPointer()->current.pos;
                cLib_addCalcAngleS(&shape_angle.x, cLib_targetAngleX(&current.pos, &masterPos), 4,
                                   0x300, 0x80);
            }

            if (targetPos.abs() < 20.0f) {
                return true;
            }
        } else if (field_0x694 == 0) {
            if ((field_0x698 & 0x20) != 0) {
                cLib_chaseF(&field_0x67c, 3.0f, 0.3f);
            } else {
                cLib_chaseF(&field_0x67c, -3.0f, 0.3f);
            }

            current.pos.y = current.pos.y + field_0x67c;

            pathPos = dPath_GetPnt(mpPath1, mPathStep2)->m_position;
            cLib_addCalcAngleS(&current.angle.y, cLib_targetAngleY(&current.pos, &pathPos), 0x20,
                               0x100, 0x40);

            if (current.pos.abs(pathPos) < 200.0f) {
                mPathStep2++;
                if (mPathStep2 >= mpPath1->m_num) {
                    mPathStep2 = 0;
                }
            }

            if (field_0x6b4 != 0) {
                shape_angle.y = current.angle.y;
            } else {
                if (field_0x6c2 == 0) {
                    mSound.startCreatureVoice(Z2SE_HAWK_V_FIND_LINK, -1);
                    field_0x6c2 = 1;
                }

                masterPos = getMasterPointer()->current.pos;
                cLib_addCalcAngleS(&shape_angle.y, cLib_targetAngleY(&current.pos, &masterPos), 8,
                                   0x100, 0x80);
                cLib_addCalcAngleS(&shape_angle.x, cLib_targetAngleX(&current.pos, &masterPos), 4,
                                   0x200, 0x80);
            }
            if (field_0x6b0 == 0) {
                field_0x694 = 1;
                field_0x6b0 = l_HIO.field_0x1c;
                field_0x67c = field_0x678 = 20.0f;
            }
        } else {
            masterPos = getMasterPointer()->current.pos;
            cLib_addCalcAngleS(&shape_angle.x, cLib_targetAngleX(&current.pos, &masterPos), 4,
                               0x200, 0x40);
            targetPos = chaseTargetPos(masterPos, field_0x678, field_0x678, 0x800);
            cLib_chaseF(&field_0x678, 60.0f, 1.0f);
            if (targetPos.abs() < 700.0f) {
                return true;
            }
        }

        break;
    case 2:
        cLib_chaseF(&speed.y, 0.0f, 1.0f);

        if (current.pos.absXZ(field_0x604) < 150.0f) {
            shape_angle.y = current.angle.y = cLib_targetAngleY(&current.pos, &field_0x604);
            return true;
        }

        break;
    case 3:
        setMasterShoulder(&field_0x604);
        cLib_addCalcAngleS(&shape_angle.y, getMasterPointer()->shape_angle.y - 0x2800, 8, 0x800,
                           0x100);
        switch (field_0x694) {
        case 0:
            cLib_chaseF(&speedF, 0.0f, 1.3f);
            if (cLib_chaseF(&speed.y, 6.0f, 1.0f) != 0) {
                field_0x694 = 1;
            }
            break;
        case 1:
            cLib_chaseF(&speedF, 0.0f, 1.3f);
            if (cLib_chaseF(&speed.y, 0.0f, 1.0f) != 0) {
                field_0x694 = 2;
                setBck(9, 0, 5.0f, 1.0f);
                field_0x678 = 0.0f;
            }
            break;
        case 2:
            setMasterShoulder(&field_0x604);
            cLib_addCalcAngleS(&shape_angle.y, getMasterPointer()->shape_angle.y - 0x2800, 8, 0x800,
                               0x100);
            cLib_chaseF(&field_0x678, 4.0f, 1.0f);

            if (cLib_chasePos(&current.pos, field_0x604, field_0x678) != 0) {
                field_0x694 = 3;
                speedF = speed.y = 0.0f;
                current.pos = field_0x604;

                mSound.startCreatureVoice(Z2SE_HAWK_V_LANDING, -1);

                if (mpMaster == NULL) {
                    dComIfGp_getVibration().StartShock(3, 1, cXyz(0.0f, 1.0f, 0.0f));
                }
            }

            break;
        case 3:
            setMasterShoulder(&current.pos);

            field_0x698 = 0;
            field_0x6c1 = 1;
            field_0x6a4 = field_0x6a0 = field_0x6a2 = 0;

            cLib_chaseF(&field_0x680, 5.0f, 1.0f);

            if (mpMaster != NULL) {
                setActionMode(6);
            }

            return true;
        }
    }

    return false;
}

void daNPC_TK_c::executePerch() {
    static char* action_table[4] = {
        "Wait",
        "Circle",
        "Nearly",
        "Land",
    };

    static char* action_table_w[2] = {
        "Wait",
        "Circle",
    };

    int staffId = dComIfGp_evmng_getMyStaffId("NPC_TK", 0, 0);
    if (staffId == -1) {
        return;
    }

    s32 unkInt1;
    s32 unkInt2;
    if (!daPy_getPlayerActorClass()->checkNowWolf()) {
        unkInt1 = 4;
        unkInt2 = dComIfGp_evmng_getMyActIdx(staffId, action_table, unkInt1, 0, 0);
    } else {
        unkInt1 = 2;
        unkInt2 = dComIfGp_evmng_getMyActIdx(staffId, action_table_w, unkInt1, 0, 0);
    }

    if (dComIfGp_evmng_getIsAddvance(staffId) != 0) {
        initPerchDemo(unkInt2);
    }

    if ((u8)executePerchDemo(unkInt2)) {
        dComIfGp_evmng_cutEnd(staffId);
    }
}

void daNPC_TK_c::executeHandOn() {
    if (field_0x698 == 0) {
        field_0x694 = 0;
        field_0x69e = shape_angle.y;
        field_0x6a2 = 0;
        field_0x698 = 1;
    }

    shape_angle.y = current.angle.y = daPy_getPlayerActorClass()->shape_angle.y - 0x2800;
    field_0x6a4 = daPy_getPlayerActorClass()->getBodyAngleX();

    switch (field_0x694) {
    case 0:
        setBck(12, 2, 3.0f, 1.0f);
        field_0x6b0 = (u8)(cM_rndF(200.0f) + 100.0f);
        field_0x694 = 1;
    case 1:
        cLib_chaseAngleS(&field_0x6a0, field_0x6a4, 0x400);
        cLib_chaseAngleS(&field_0x6a2, (shape_angle.y - field_0x69e) * 7, 0x200);
        field_0x69e = shape_angle.y;

        if (field_0x6b0 == 0 && mpMorf->checkFrame(0.0f) != 0) {
            if (cM_rndF(1.0f) < 0.5f) {
                setBck(10, 0, 0.0f, 1.0f);
            } else {
                setBck(11, 0, 3.0f, 1.0f);
                if (field_0x6a0 < 0) {
                    field_0x6a0 = 0;
                }
            }
            field_0x694 = 2;
        }

        break;
    case 2:
        cLib_chaseAngleS(&field_0x6a0, 0, 0x1000);
        cLib_chaseAngleS(&field_0x6a2, 0, 0x1000);

        if (mpMorf->isStop()) {
            field_0x694 = 0;
        }

        break;
    }

    cLib_chaseF(&field_0x680, cM_ssin(field_0x6a4) * 10.0f + 5.0f, 1.0f);
    field_0x6c1 = 1;

    setMasterShoulder(&current.pos);
}

bool daNPC_TK_c::checkWaterSurface(f32 param_0) {
    dBgS_ObjGndChk_Spl gndChk;
    cXyz pos = current.pos;
    pos.y += 500.0f;
    gndChk.SetPos((Vec*)&pos);
    field_0x684 = dComIfG_Bgsp().GroundCross(&gndChk);
    if (field_0x684 > current.pos.y + param_0) {
        return true;
    } else {
        return false;
    }
}

void daNPC_TK_c::executeAttack() {
    static f32 taka_attack_dist[4] = {
        5200.0f,
        2500.0f,
        1800.0f,
        3000.0f,
    };

    static u16 w_eff_id[4] = {
        0x01B8,
        0x01B9,
        0x01BA,
        0x01BB,
    };

    field_0x6bd = 1;

    switch (field_0x694) {
    case 0: {
        if (eventInfo.checkCommandDemoAccrpt() == 0) {
            fopAcM_orderPotentialEvent(this, 2, -1, 0);
            eventInfo.onCondition(dEvtCnd_CANDEMO_e);
        } else {
            mAcch.ClrWallHit();
            mAcch.ClrGroundHit();
            mSphere.ClrAtHit();

            setBck(6, 2, 3.0f, 2.0f);

            field_0x694 = 1;
            field_0x6b0 = 30;

            daPy_py_c* player = daPy_getPlayerActorClass();
            s16 oppBodyAngleX = -player->getBodyAngleX();
            s16 totalAngleY = player->shape_angle.y + player->getBodyAngleY();
            shape_angle.y = current.angle.y = totalAngleY;
            speedF = l_HIO.field_0x28 * cM_scos(oppBodyAngleX);
            speed.y = l_HIO.field_0x28 * cM_ssin(oppBodyAngleX);

            field_0x61c.set(cM_scos(shape_angle.y) * -50.0f, -50.0f,
                            cM_ssin(shape_angle.y) * 50.0f);
            mAcch.SetGroundUpY(-40.0f);

            dCamera_c* camera = dCam_getBody();
            camera->Stop();
            field_0x710 = 1;
            setHawkCamera(this);
            field_0x6fc = camera->Center();
            field_0x6f0 = camera->Eye();
            field_0x604 = current.pos;
            camera->SetTrimSize(3);

            mSound.startCreatureVoice(Z2SE_HAWK_V_TAKE_OFF, -1);

            field_0x719 = 0;

            if (strcmp("F_SP103", dComIfGp_getStartStageName()) == 0) {
                cXyz unkXyz1(2919.0f, 657.0f, -2095.0f);
                cXyz unkXyz2(1553.0f, 659.0f, -466.0f);
                cXyz unkXyz3(700.0f, 1750.0f, 5283.0f);

                if (current.pos.abs(unkXyz1) < 300.0f) {
                    if (totalAngleY > 0x5000 && totalAngleY < 0x6800) {
                        field_0x719 = 0;
                    } else {
                        if (totalAngleY > 0x2000 && totalAngleY < 0x5000) {
                            field_0x719 = 1;
                        } else {
                            if (totalAngleY > -0x5800 && totalAngleY < -0x3800 ||
                                totalAngleY < -0x6000 || totalAngleY > 0x6800)
                            {
                                field_0x719 = 2;
                            } else {
                                field_0x719 = 3;
                            }
                        }
                    }
                } else {
                    if (current.pos.abs(unkXyz2) < 300.0f) {
                        if (totalAngleY > 0x2800) {
                            field_0x719 = 3;
                        } else {
                            if (totalAngleY < -0x5000) {
                                field_0x719 = 2;
                            }
                        }
                    } else {
                        if (current.pos.abs(unkXyz3) < 300.0f && oppBodyAngleX > 0x400) {
                            if (totalAngleY < -4000) {
                                field_0x719 = 3;
                            } else if (totalAngleY < 0x4000) {
                                field_0x719 = 2;
                            }
                        }
                    }
                }
            }
        }
        break;
    }
    case 1: {
        mSphere.OnAtSetBit();
        mSphere.SetAtSpl(dCcG_At_Spl_UNK_0);
        mSphere.SetAtSPrm(31);

        if (cLib_chaseF(&field_0x61c.x, 0.0f, 2.0f) == 0) {
            if (field_0x61c.x < 0.0f) {
                current.pos.x -= 2.0f;
            } else {
                current.pos.x += 2.0f;
            }
        }

        if (cLib_chaseF(&field_0x61c.y, 0.0f, 2.0f) == 0) {
            if (field_0x61c.y < 0.0f) {
                current.pos.y -= 2.0f;
            } else {
                current.pos.y += 2.0f;
            }
        }

        if (cLib_chaseF(&field_0x61c.z, 0.0f, 2.0f) == 0) {
            if (field_0x61c.z < 0.0f) {
                current.pos.z -= 2.0f;
            } else {
                current.pos.z += 2.0f;
            }
        }

        field_0x6ae = 1;
        if (field_0x6b0 == 0 && checkBck(8) == 0) {
            if (mpMorf->checkFrame(mpMorf->getEndFrame()) != 0) {
                setBck(8, 2, 3.0f, 1.0f);
            }
        }

        if ((field_0x604 - current.pos).abs() >= taka_attack_dist[field_0x719] + nREG_F(18) &&
            mAcch.ChkWallHit() == 0 && mAcch.ChkGroundHit() == 0)
        {
            this->field_0x698 = 2;

            setActionMode(4);
        } else {
            if (mAcch.ChkWallHit() != 0) {
                setAwayAction(0);

                shape_angle.y = mCircle.GetWallAngleY() + 0x8000;
                current.angle.y = shape_angle.y;

                field_0x6c3 = 0;
            } else {
                if (mAcch.ChkGroundHit() != 0) {
                    setAwayAction(0);

                    field_0x6c3 = 1;
                } else {
                    if (checkWaterSurface(50.0f) != 0) {
                        setAwayAction(0);

                        field_0x6c3 = 2;
                    }
                }
            }
            if (mSphere.ChkAtHit() != 0) {
                fopAc_ac_c* local_118 = dCc_GetAc(mSphere.GetAtHitObj()->GetAc());
                if (fopAcM_GetName(local_118) != PROC_ALINK &&
                    fopAcM_GetName(local_118) != PROC_E_ARROW)
                {
                    setAwayAction(0);

                    field_0x6c3 = 0;
                }
            }

            cXyz unkXyz4(field_0x628);
            cXyz unkXyz5(current.pos);
            unkXyz4.y += 40.0f;
            unkXyz5.y += 40.0f;

            // this extra scope affects destructor placement for linChk, probably a fakematch
            {
                dBgS_LinChk linChk;
                linChk.Set(&unkXyz4, &unkXyz5, NULL);
                if (dComIfG_Bgsp().LineCross(&linChk) != 0) {
                    setAwayAction(0);

                    field_0x6c3 = 0;
                }
            }

            m_near_angle = 0x2000;
            m_near_actor = NULL;

            fpcM_Search(s_obj_sub, this);

            field_0x634 = m_near_actor;
            if (field_0x634 != 0 && current.pos.abs(field_0x634->current.pos) < l_HIO.field_0x34) {
                if (fopAcM_GetName(field_0x634) == PROC_NI) {
                    mCarryType = 0;
                } else {
                    if (fopAcM_GetName(field_0x634) == PROC_OBJ_KAGO) {
                        mCarryType = 2;
                    } else {
                        if (fopAcM_GetName(field_0x634) == PROC_OBJ_PUMPKIN) {
                            mCarryType = 1;
                        }
                    }
                }

                setActionMode(5);
            }
        }
        break;
    }
    case 2: {
        mAcch.SetGroundUpY(fabsf(cM_ssin(shape_angle.x) * 30.0f));

        MtxP anmMtx = mpMorf->getModel()->getAnmMtx(0);
        cXyz part1Trans(anmMtx[0][3], anmMtx[1][3], anmMtx[2][3]);

        field_0xb3c = dComIfGp_particle_set(field_0xb3c, 0x832b, &part1Trans, &tevStr, &shape_angle,
                                            0, -1, 0, -1, 0, 0, 0);

        if (field_0x6c3 == 1) {
            fopAcM_effSmokeSet2(&field_0xb20, &field_0xb24, &current.pos, 0, 1.5f, &tevStr);
        } else if (field_0x6c3 == 2) {
            cXyz part2Trans(current.pos.x, field_0x684, current.pos.z);
            if (field_0x6b4 == 0) {
                static cXyz sc(1.0f, 1.0f, 1.0f);

                for (s32 i = 0; i < 4; i++) {
                    field_0xb28[i] =
                        dComIfGp_particle_set(field_0xb28[i], w_eff_id[i], &part2Trans, &tevStr,
                                              &shape_angle, &sc, -1, 0, -1, 0, 0, 0);
                }

                field_0x6b4 = 10;
            }
            fopAcM_effHamonSet(&field_0xb38, &part2Trans, 1.0f, 0.3f);
        }

        if (shape_angle.x < -0x2000) {
            shape_angle.x = -0x2000;
        }

        cLib_chaseF(&speedF, 0.0f, 1.0f);
        cLib_chaseF(&speed.y, 0.0f, 1.0f);

        if (field_0x6b0 == 0) {
            setActionMode(4);

            mAcch.SetGroundUpY(0.0f);
        }
        break;
    }
    }
}

void daNPC_TK_c::executeAway() {
    field_0x6bd = 1;

    switch (field_0x694) {
    case 0: {
        if (field_0x698 == 0 || field_0x698 == 2) {
            if (checkBck(6) != 0) {
                mpMorf->setPlaySpeed(1.5f);
            } else {
                setBck(6, 2, 3.0f, 1.5f);
            }

            field_0x694 = 1;

            if (field_0x698 == 0) {
                field_0x67c = 0.0f;
                field_0x678 = 0.0f;
            }
        } else {
            setBck(6, 2, 3.0f, 2.0f);

            field_0x694 = 2;

            field_0x69e = cLib_targetAngleY(&current.pos, &home.pos);
        }

        field_0x6b0 = 75;

        break;
    }
    case 1: {
        if (field_0x6b0 == 0) {
            mSphere.OnCoSetBit();

            cLib_chaseF(&field_0x678, 20.0f, 1.0f);
            cLib_chaseF(&field_0x67c, 20.0f, 1.0f);

            if (this->field_0x698 == 0) {
                chaseTargetPos(home.pos, field_0x678, field_0x67c, 0x800);
            } else {
                chaseTargetPos(home.pos, field_0x678, field_0x67c, 0x100);
            }

            if (current.pos.y > home.pos.y) {
                setActionMode(0);
            }
        } else {
            u32 bgCheckRv = checkBeforeBg();
            if ((bgCheckRv & 1) != 0) {
                cLib_chaseF(&speedF, 0.0f, 3.0f);
            } else {
                cLib_chaseF(&speedF, 10.0f, 0.5f);
            }
            if ((bgCheckRv & 2) != 0) {
                cLib_chaseF(&speed.y, 20.0f, 3.0f);
            } else {
                cLib_chaseF(&speed.y, 20.0f, 0.5f);
            }

            cLib_addCalcAngleS(&shape_angle.y, cLib_targetAngleY(&current.pos, &home.pos), 8, 0x100,
                               0x80);
            current.angle.y = shape_angle.y;
        }

        break;
    }
    case 2: {
        u32 bgCheckRv = checkBeforeBg();
        if ((bgCheckRv & 1) != 0) {
            cLib_chaseF(&speedF, 0.0f, 3.0f);
        } else {
            cLib_chaseF(&speedF, 10.0f, 0.5f);
        }
        if ((bgCheckRv & 2) != 0) {
            cLib_chaseF(&speed.y, 20.0f, 3.0f);
        } else {
            cLib_chaseF(&speed.y, 20.0f, 0.5f);
        }

        cLib_addCalcAngleS(&shape_angle.y, field_0x69e, 8, 0x100, 0x80);
        current.angle.y = shape_angle.y;

        if (current.pos.y > home.pos.y) {
            mSphere.OnCoSetBit();
            setActionMode(0);
        }
    }
    }

    if (field_0x6b0 == 0 && field_0x6ec == this) {
        endHawkCamera();
    }
}

void daNPC_TK_c::setCarryActorMtx() {
    field_0x6a8 += 0x6bc;
    field_0x6a6 = cM_ssin(field_0x6a8) * 2048.0f + 4096.0f;
    if (field_0x634 != NULL) {
        cXyz unkXyz1;
        switch (mCarryType) {
        case 0:
            mDoMtx_stack_c::copy(mpMorf->getModel()->getAnmMtx(0));
            mDoMtx_stack_c::multVecZero(&unkXyz1);
            mDoMtx_stack_c::transS(unkXyz1);
            mDoMtx_stack_c::ZXYrotM(-shape_angle.x, shape_angle.y - 0x8000, shape_angle.z);
            mDoMtx_stack_c::transM(nREG_F(19), -60.0f + nREG_F(18), 30.0f + nREG_F(17));
            ((ni_class*)field_0x634)->setMtx(mDoMtx_stack_c::get());
            break;
        case 2:
            mDoMtx_stack_c::copy(mpMorf->getModel()->getAnmMtx(0));
            mDoMtx_stack_c::multVecZero(&unkXyz1);
            mDoMtx_stack_c::transS(unkXyz1);
            mDoMtx_stack_c::ZXYrotM(shape_angle.z, shape_angle.y - 0x4000, -shape_angle.x / 2);
            mDoMtx_stack_c::transM(-40.0f, -85.0f, 0.0f);
            mDoMtx_stack_c::transM(20.0f, 60.0f, 0.0f);
            mDoMtx_stack_c::ZXYrotM(0, 0, field_0x6a6);
            mDoMtx_stack_c::transM(-20.0f, -60.0f, 0.0f);
            ((daObj_Kago_c*)field_0x634)->setMtx(mDoMtx_stack_c::get());
            break;
        case 1:
            mDoMtx_stack_c::copy(mpMorf->getModel()->getAnmMtx(0));
            mDoMtx_stack_c::transM(-35.0f, -45.0f, 0.0f);
            ((daObj_Pumpkin_c*)field_0x634)->setMtx(mDoMtx_stack_c::get());
            break;
        }
    }
}

f32 daNPC_TK_c::getTakePosY() {
    cXyz unusedXyz;  // debug match

    if (mCarryType == 0) {
        return 30.0f + nREG_F(15);
    } else if (mCarryType == 2) {
        return 55.0f + nREG_F(15);
    } else if (mCarryType == 1) {
        return 45.0f + nREG_F(15);
    } else {
        return -1.0f;
    }
}

f32 daNPC_TK_c::getTakeOffPosY() {
    cXyz unusedXyz;  // debug match

    if (mCarryType == 0) {
        return 210.0f + nREG_F(15);
    } else if (mCarryType == 2) {
        return 250.0f + nREG_F(15);
    } else if (mCarryType == 1) {
        return 200.0f + nREG_F(15);
    } else {
        return -1.0f;
    }
}

void daNPC_TK_c::executeBack() {
    if (field_0x634 == NULL) {
        setActionMode(4);
        field_0x698 = 2;
    } else {
        cXyz unkXyz1 = *fopAcM_GetPosition_p(field_0x634);
        unkXyz1.y += getTakePosY();

        cXyz playerPos = daPy_getPlayerActorClass()->current.pos;
        playerPos.y += getTakeOffPosY();

        field_0x70c = 60 + nREG_S(1);

        switch (field_0x694) {
        case 0: {
            speed.y = 0.0f;
            speedF = 0.0f;

            field_0x694 = 1;

            field_0x678 = l_HIO.field_0x28;

            field_0x6e0 = 0;

            field_0x63c[0] = current.pos;
            field_0x63c[1] = unkXyz1;

            cXyz posDiff = playerPos - unkXyz1;
            posDiff.normalize();

            field_0x63c[2] = unkXyz1 + posDiff * 200.0f;
            field_0x63c[3] = unkXyz1;
            field_0x63c[4] = playerPos;

            mSphere.OffCoSetBit();

            field_0x698 = 0x14;
        }
        case 1: {
            field_0x698--;
            s16 unkInt1 = field_0x698 << 10;
            if (unkInt1 >= 0x4000) {
                unkInt1 = 0x4000;
            }
            if (unkInt1 <= 0x2800) {
                unkInt1 = 0x2800;
            }
            if (abs((s16)(shape_angle.y - cLib_targetAngleY(&current.pos, &unkXyz1))) > unkInt1 ||
                abs((s16)(shape_angle.x + cLib_targetAngleX(&current.pos, &unkXyz1))) > 0x4000)
            {
                setActionMode(4);

                field_0x698 = 2;
            } else {
                s16 unkInt2 = field_0x698;
                if (unkInt2 > 8) {
                    unkInt2 = 8;
                }
                if (unkInt2 < 2) {
                    unkInt2 = 2;
                }

                cLib_addCalcAngleS(&shape_angle.y, cLib_targetAngleY(&current.pos, &unkXyz1),
                                   unkInt2, 0x400, 0x10);
                current.angle.y = shape_angle.y;

                cLib_addCalcAngleS(&shape_angle.x, -cLib_targetAngleX(&current.pos, &unkXyz1),
                                   unkInt2, 0x400, 0x10);

                cLib_chaseF(&field_0x678, 30.0f, 3.0f);
                speedF = field_0x678 * cM_scos(-shape_angle.x);
                speed.y = field_0x678 * cM_ssin(-shape_angle.x);

                if (mAcch.ChkWallHit() != 0) {
                    setAwayAction(0);

                    shape_angle.y = mCircle.GetWallAngleY() + 0x8000;
                    current.angle.y = shape_angle.y;

                    field_0x6c3 = 0;

                    break;
                }
                if (current.pos.absXZ(unkXyz1) < 50.0f) {
                    field_0x6b0 = 10;
                    field_0x694 = 2;

                    current.angle.y = shape_angle.y;

                    if ((s16)(cLib_targetAngleY(&playerPos, &unkXyz1) - shape_angle.y) < 0) {
                        field_0x69e = 0x300;
                    } else {
                        field_0x69e = -0x300;
                    }

                    fopAcM_setHawkCarryNow(field_0x634);

                    if (fopAcM_GetName(field_0x634) == PROC_NI) {
                        fopAcM_setCarryNow(field_0x634, 0);
                    }

                    if (fopAcM_GetName(field_0x634) == PROC_OBJ_KAGO &&
                         /* dSv_event_flag_c::F_0577 - Ordon Village - 2nd Day - Retrieved basket from monkey (hit hawk) */
                        !dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[0x241]))
                    {
                        Z2GetAudioMgr()->changeBgmStatus(1);
                        Z2GetAudioMgr()->seStart(Z2SE_SY_HAWK_GET_KAGO, NULL, 0, 0, 1.0f, 1.0f,
                                                 -1.0f, -1.0f, 0);
                    }
                    mSound.startCreatureVoice(Z2SE_HAWK_V_TAKE_OFF, -1);
                    field_0x6a8 = 0;
                }
            }
            break;
        }
        case 2: {
            field_0x6c6 = 1;

            shape_angle.y = shape_angle.y + field_0x69e;
            current.angle.y = shape_angle.y;

            shape_angle.x = shape_angle.x - (0x300 + nREG_S(0));

            if (shape_angle.x < -0x3000) {
                shape_angle.x = -0x3000;
                field_0x694 = 3;
                field_0x69c = 0;
                s16 targetAngleY =
                    cLib_targetAngleY(&current.pos, &field_0x63c[2]) - current.angle.y;
                field_0x69e = abs(targetAngleY);
            }

            cLib_chaseF(&field_0x678, 18.0f, 1.0f);

            speedF = field_0x678 * cM_scos(-shape_angle.x);
            speed.y = field_0x678 * cM_ssin(-shape_angle.x);

            break;
        }
        case 3: {
            field_0x6c6 = 1;

            cLib_chaseAngleS(&shape_angle.y, cLib_targetAngleY(&current.pos, &field_0x63c[2]),
                             0x300);

            current.angle.y = shape_angle.y;

            if (fabsf(current.pos.y - unkXyz1.y) > 500.0f) {
                if (current.pos.y > unkXyz1.y) {
                    cLib_chaseAngleS(&shape_angle.x, 0x2000, 0x400);
                } else {
                    cLib_chaseAngleS(&shape_angle.x, 0xffffe000, 0x400);
                }
            } else {
                cLib_chaseAngleS(&field_0x69c, 0x200, 0x10);
                cLib_chaseAngleS(&shape_angle.x, -cLib_targetAngleX(&current.pos, &playerPos),
                                 field_0x69c);
            }

            cLib_chaseF(&field_0x678, 18.0f, 1.0f);

            speedF = field_0x678 * cM_scos(-shape_angle.x);
            speed.y = field_0x678 * cM_ssin(-shape_angle.x);

            s16 targetAngleY = cLib_targetAngleY(&current.pos, &field_0x63c[2]) - current.angle.y;
            s16 targetAngleYAbs = abs(targetAngleY);
            if (targetAngleYAbs >= field_0x69e) {
                field_0x694 = 10;
            } else {
                field_0x69e = targetAngleYAbs;
                if (targetAngleYAbs < 0x2800) {
                    field_0x694 = 10;
                }
            }

            break;
        }
        case 10:
        case 11: {
            field_0x6c6 = 1;
            unkXyz1 = playerPos;

            s16 targetAngleY = cLib_targetAngleY(&current.pos, &unkXyz1);
            cLib_addCalcAngleS(&shape_angle.y, targetAngleY, 8, 0x800, 0x40);
            current.angle.y = shape_angle.y;

            if (fabsf(current.pos.y - unkXyz1.y) > 500.0f) {
                if (current.pos.y > unkXyz1.y) {
                    cLib_chaseAngleS(&shape_angle.x, 0x2000, 0x400);
                } else {
                    cLib_chaseAngleS(&shape_angle.x, 0xffffe000, 0x400);
                }
            } else {
                cLib_chaseAngleS(&field_0x69c, 0x200, 0x10);
                cLib_chaseAngleS(&shape_angle.x, -cLib_targetAngleX(&current.pos, &unkXyz1),
                                 field_0x69c);
            }

            cLib_chaseF(&field_0x678, 50.0f, 1.0f);

            speedF = field_0x678 * cM_scos(-shape_angle.x);
            speed.y = field_0x678 * cM_ssin(-shape_angle.x);

            if (field_0x694 == 10) {
                if (current.pos.absXZ(playerPos) < 200.0f) {
                    field_0x6b4 = 0;
                    if (fopAcM_GetName(field_0x634) == PROC_OBJ_KAGO &&
                         /* dSv_event_flag_c::F_0577 - Ordon Village - 2nd Day - Retrieved basket from monkey (hit hawk) */
                        !dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[0x241]))
                    {
                        Z2GetAudioMgr()->changeBgmStatus(0);

                        /* dSv_event_flag_c::F_0577 - Ordon Village - 2nd Day - Retrieved basket from monkey (hit hawk) */
                        dComIfGs_onEventBit(dSv_event_flag_c::saveBitLabels[0x241]);

                        field_0x6b8 = 0x46;
                    }

                    field_0x694 = field_0x694 + 1;
                    field_0x6b0 = 4;

                    if (daPy_getPlayerActorClass()->setForceGrab(field_0x634, 1, 0)) {
                        daPy_getPlayerActorClass()->onForceGrabRebound();
                    }
                }
            } else if (field_0x6b0 == 0) {
                setActionMode(4);

                field_0x698 = 2;

                fopAcM_cancelHawkCarryNow(field_0x634);

                field_0x634 = NULL;

                break;
            }

            if (mAcch.ChkWallHit() != 0) {
                s16 angleDiffOpp = mCircle.GetWallAngleY() + 0x8000 - current.angle.y;
                if (abs(angleDiffOpp) < 0x3000) {
                    setAwayAction(0);

                    shape_angle.y = mCircle.GetWallAngleY() + 0x8000;
                    current.angle.y = shape_angle.y;

                    field_0x6c3 = 0;

                    fopAcM_cancelHawkCarryNow(field_0x634);
                    if (fopAcM_GetName(field_0x634) == PROC_NI) {
                        fopAcM_cancelCarryNow(field_0x634);
                    }
                    field_0x634 = NULL;
                }
            }
        }
        }
    }
}

cXyz daNPC_TK_c::getHanjoHandPos() {
    cXyz handPos(0.0f, 0.0f, 0.0f);
    if (mpMaster != NULL) {
        cXyz offset(23.0f, 3.0f, 0.0f);
        mDoMtx_stack_c::copy(mpMaster->getArmLMtx());
        mDoMtx_stack_c::multVec(&offset, &handPos);
    }
    return handPos;
}

void daNPC_TK_c::executeStayHanjo() {
    if (checkAttackDemo()) {
        return;
    }

    switch (field_0x694) {
    case 0: {
        field_0x694 = 1;
    }
    case 1: {
        mpMaster = (daNpc_Hanjo_c*)fpcM_Search(s_hanjo, this);
        if (mpMaster != NULL && mpMaster->getType() == 1) {
            field_0x694 = 2;
            mSphere.OffAtSetBit();
            speed.y = 0.0f;
            speedF = 0.0f;
            return;
        }

        setActionMode(0);

        return;
    }
    case 2: {
        setBck(0xc, 2, 10.0f, 1.0f);

        field_0x6b0 = (u8)(cM_rndF(200.0f) + 100.0f);
        field_0x694 = 3;
    }
    case 3: {
        if (field_0x6b0 == 0 && mpMorf->checkFrame(0.0f) != 0) {
            if (cM_rndF(1.0f) < 0.5f) {
                setBck(10, 0, 0.0f, 1.0f);
            } else {
                setBck(11, 0, 3.0f, 1.0f);
            }

            field_0x694 = 4;
        }

        mIsHanjoHand = 1;
        current.pos = getHanjoHandPos();

        shape_angle.y = mpMaster->shape_angle.y - 0x3000;

        break;
    }
    case 4: {
        if (mpMorf->isStop() != 0) {
            field_0x694 = 2;
        }

        mIsHanjoHand = 1;
        current.pos = getHanjoHandPos();

        shape_angle.y = mpMaster->shape_angle.y - 0x3000;

        break;
    }
    }
}

void daNPC_TK_c::executeAttackLink() {
    cXyz playerPos = dComIfGp_getPlayer(0)->current.pos;

    field_0x6bd = 1;
    field_0x71a = 1;
    if (checkAttackDemo()) {
        return;
    }

    switch (field_0x694) {
    case 0: {
        mSphere.ClrAtHit();
        current.angle.y = shape_angle.y;
        field_0x694 = 1;
        setBck(8, 2, 10.0f, 1.0f);
        mSound.startCreatureVoice(Z2SE_HAWK_V_TAKE_OFF, -1);
        fopAcM_OffStatus(this, fopAcM_STATUS_UNK_0x4000);
        field_0x6b4 = 0x1e;
        break;
    }
    case 1: {
        if (dComIfGp_checkPlayerStatus0(0, 0x100000) != 0 ||
            dComIfGp_checkPlayerStatus0(0, 0x100) != 0 ||
            daPy_getPlayerActorClass()->checkWolfTagLockJumpReady())
        {
            field_0x694 = 3;
            field_0x6b0 = 0x96;
            setBck(6, 2, 3.0f, 1.0f);
        } else {
            mSphere.OnAtSetBit();
            mSphere.SetAtSpl(dCcG_At_Spl_UNK_1);
            mSphere.SetAtSPrm(5);
            field_0x6ae = 1;
            if (playerPos.absXZ(current.pos) < 500.0f) {
                cLib_addCalcAngleS(&shape_angle.y, fopAcM_searchPlayerAngleY(this), 4, 0x1000,
                                   0x100);
            } else {
                cLib_addCalcAngleS(&shape_angle.y, fopAcM_searchPlayerAngleY(this), 8, 0x800,
                                   0x100);
            }
            current.angle.y = shape_angle.y;
            cXyz vecToPlayer = playerPos - current.pos;
            s32 pitch = cM_atan2s(vecToPlayer.absXZ(), vecToPlayer.y);
            cLib_chaseF(&speed.y, cM_scos(pitch) * (40.0f + nREG_F(5)), 3.0f);
            cLib_chaseF(&speedF, cM_ssin(pitch) * (40.0f + nREG_F(5)), 3.0f);
            if (mSphere.ChkAtHit()) {
                if (fopAcM_GetName(dCc_GetAc(mSphere.GetAtHitObj()->GetAc())) != PROC_ALINK) {
                    if (daPy_getPlayerActorClass()->getDamageWaitTimer() == 0) {
                        break;
                    }
                }
                if (fopAcM_GetName(dCc_GetAc(mSphere.GetAtHitObj()->GetAc())) == PROC_ALINK) {
                    field_0x6c7++;
                    mSphere.ClrAtHit();
                }
                field_0x694 = 2;
                field_0x6b0 = 0xf;
            }
            break;
        }
        break;
    }
    case 2: {
        cLib_chaseAngleS(&shape_angle.x, ~0x1fff, 0x400);
        cLib_chaseF(&speed.y, cM_scos(shape_angle.x) * (35.0f + nREG_F(6)), 3.0f);
        cLib_chaseF(&speedF, cM_ssin(shape_angle.x) * -(35.0f + nREG_F(6)), 3.0f);
        if (field_0x6b0 == 0) {
            field_0x694 = 4;
            field_0x6b0 = 0x96;
            setBck(6, 2, 3.0f, 1.0f);
            if (field_0x6c7 >= 3) {
                field_0x6c7 = 3;
                if (!dComIfGs_isSaveSwitch(0xf)) {
                    dComIfGs_onSaveSwitch(0xf);
                }
            }
        }
        break;
    }
    case 3: {
        mSphere.OnAtSetBit();
        field_0x6ae = 1;
        if (mSphere.ChkAtHit() != 0) {
            if (fopAcM_GetName(dCc_GetAc(mSphere.GetAtHitObj()->GetAc())) == PROC_ALINK) {
                field_0x6c7 = field_0x6c7 + 1;
                mSphere.ClrAtHit();
                field_0x694 = 4;
            } else {
                if (daPy_getPlayerActorClass()->getDamageWaitTimer() != 0) {
                    field_0x694 = 4;
                }
            }
        }
        if (field_0x6b0 < 90) {
            field_0x694 = 4;
        }
    }
    case 4: {
        if (daPy_getPlayerActorClass()->checkWolfTagLockJumpReady()) {
            playerPos = daPy_getPlayerActorClass()->current.pos;
            playerPos.y += 500.0f;
            cLib_addCalcAngleS(&shape_angle.y, cLib_targetAngleY((Vec*)&current.pos, &playerPos), 8,
                               0x400, 0x100);
        } else {
            playerPos.set(1800.0f, 500.0f, 1500.0f);
            cLib_addCalcAngleS(&shape_angle.y, cLib_targetAngleY((Vec*)&current.pos, &playerPos),
                               0x10, 0x300, 0x100);
        }
        current.angle.y = shape_angle.y;
        f32 yDiff = playerPos.y - current.pos.y;
        f32 targetSpeedY = 0.0f;
        if (fabsf(yDiff) >= 10.0f) {
            if (yDiff < 0.0f) {
                targetSpeedY = -10.0f;
            } else {
                targetSpeedY = 10.0f;
            }
        }
        cLib_chaseF(&speed.y, targetSpeedY, 1.0f);
        cLib_chaseF(&speedF, 30.0f, 3.0f);
        if (field_0x6b0 == 0) {
            if (dComIfGp_checkPlayerStatus0(0, fopAcM_STATUS_HOOK_CARRY_NOW) ||
                dComIfGp_checkPlayerStatus0(0, fopAcM_STATUS_UNK_0x100) ||
                daPy_getPlayerActorClass()->checkWolfTagLockJumpReady())
            {
                field_0x6b0 = 10;
                break;
            }

            field_0x694 = 5;
            field_0x6b0 = 10;
        }
        break;
    }
    case 5: {
        cLib_addCalcAngleS(&shape_angle.y, fopAcM_searchPlayerAngleY(this) + 0x8000, 0x10, 0x300,
                           0x100);
        current.angle.y = shape_angle.y;
        cLib_chaseF(&speed.y, 0.0f, 1.0f);
        cLib_chaseF(&speedF, 30.0f, 5.0f);
        if (playerPos.absXZ(current.pos) >= 700.0f) {
            setBck(8, 2, 10.0f, 1.0f);
            field_0x694 = 1;
            mSound.startCreatureVoice(Z2SE_HAWK_V_TAKE_OFF, -1);
        }
        break;
    }
    }
}

void daNPC_TK_c::executeBackHanjo() {
    mpMaster = (daNpc_Hanjo_c*)fpcM_Search(s_hanjo, this);

    if (mpMaster != NULL) {
        cXyz cStack_20;
        cXyz auStack_2c = getHanjoHandPos();
        if (!checkAttackDemo()) {
            if (field_0x698 == 0) {
                switch (field_0x694) {
                case 0: {
                    mSphere.OffAtSetBit();
                    cStack_20.set(800.0f, 300.0f, -800.0f);
                    cLib_offsetPos(&field_0x604, &auStack_2c, mpMaster->shape_angle.y, &cStack_20);
                    field_0x678 = 30.0f;
                    break;
                }
                case 1: {
                    cStack_20.set(200.0f, 0.0f, -150.0f);
                    cLib_offsetPos(&field_0x604, &auStack_2c, mpMaster->shape_angle.y, &cStack_20);
                    break;
                }
                case 2: {
                    cStack_20.set(100.0f, 50.0f, -70.0f);
                    cLib_offsetPos(&field_0x604, &auStack_2c, mpMaster->shape_angle.y, &cStack_20);
                    setBck(5, 2, 5.0f, 1.0f);
                    break;
                }
                case 3: {
                    field_0x604 = getHanjoHandPos();
                    setBck(9, 2, 5.0f, 1.0f);
                    mSound.startCreatureVoice(Z2SE_HAWK_V_LANDING, -1);
                    break;
                }
                }
                field_0x698 = field_0x698 + 1;
            }

            cXyz cStack_38 = field_0x604 - current.pos;
            s16 sVar4 = (s16)cM_atan2s(cStack_38.absXZ(), cStack_38.y);
            s16 sVar5 = cLib_targetAngleY(&current.pos, &field_0x604);
            switch (field_0x694) {
            case 1:
                cLib_chaseF(&field_0x678, 20.0f, 1.0f);
            case 0:
                cLib_addCalcAngleS(&shape_angle.y, sVar5, 8, 0x1000, 0x100);
                current.angle.y = shape_angle.y;

                cLib_chaseF(&speed.y, field_0x678 * cM_scos(sVar4), 3.0f);
                cLib_chaseF(&speedF, field_0x678 * cM_ssin(sVar4), 3.0f);

                if (cStack_38.abs() < 100.0f) {
                    field_0x694 = field_0x694 + 1;
                    field_0x698 = 0;
                }

                break;
            case 2:
            case 3:
                cLib_chaseF(&field_0x678, 5.0f, 1.0f);

                cLib_addCalcAngleS(&shape_angle.y, sVar5, 8, 0x1000, 0x100);
                current.angle.y = shape_angle.y;

                cLib_chaseF(&speed.y, field_0x678 * cM_scos(sVar4), 3.0f);
                cLib_chaseF(&speedF, field_0x678 * cM_ssin(sVar4), 3.0f);

                if (field_0x694 == 2) {
                    if (cStack_38.abs() < 100.0f + nREG_F(15)) {
                        if (checkBck(7) == 0) {
                            setBck(7, 2, 5.0f, 1.0f);
                        }

                        if (cStack_38.abs() < 30.0f) {
                            field_0x694 = 3;
                            field_0x698 = 0;
                        }
                    }
                    break;
                }

                if (cStack_38.abs() <= 5.0f) {
                    setActionMode(6);
                }
                break;
            }
        }
    }
}

bool daNPC_TK_c::checkAttackDemo() {
    cXyz cStack_14(2500.0f, 500.0f, 1050.0f);
    if (daPy_getPlayerActorClass()->checkWolfTagLockJump()) {
        if (cStack_14.absXZ(daPy_getPlayerActorClass()->current.pos) < 1200.0f) {
            setActionMode(9);
            return true;
        }
    }
    return false;
}

void daNPC_TK_c::executeAttackDemo() {
    daPy_py_c* player = daPy_getPlayerActorClass();

    cXyz posWithOffset = player->current.pos;
    posWithOffset.y += 50.0f;
    cXyz vecToPlayer = posWithOffset - current.pos;

    s32 pitch;
    switch (field_0x694) {
    case 0:
        field_0x694 = 1;
        setBck(8, 2, 10.0f, 1.0f);
        mSphere.ClrAtHit();
    case 1:
        mSphere.OnAtSetBit();
        mSphere.SetAtSpl(dCcG_At_Spl_UNK_1);
        mSphere.SetAtSPrm(5);

        field_0x6ae = 1;

        if (fopAcM_CheckCondition(this, fopAcCnd_NODRAW_e)) {
            current.angle.y = shape_angle.y = fopAcM_searchPlayerAngleY(this);
            cLib_chasePos(&current.pos, posWithOffset, 200.0f);
        } else {
            cLib_addCalcAngleS(&shape_angle.y, fopAcM_searchPlayerAngleY(this), 4, 0x2000, 0x200);
            current.angle.y = shape_angle.y;
        }

        pitch = cM_atan2s(vecToPlayer.absXZ(), vecToPlayer.y);
        cLib_chaseF(&speed.y, (50.0f + nREG_F(4)) * cM_scos(pitch), 10.0f);
        cLib_chaseF(&speedF, (50.0f + nREG_F(4)) * cM_ssin(pitch), 10.0f);

        if (mSphere.ChkAtHit()) {
            if (fopAcM_GetName(dCc_GetAc(mSphere.GetAtHitObj()->GetAc())) != PROC_ALINK) {
                if (daPy_getPlayerActorClass()->getDamageWaitTimer() == 0) {
                    return;
                }
            }

            setActionMode(7);

            field_0x694 = 2;
            field_0x6b0 = 0xf;

            if (fopAcM_GetName(dCc_GetAc(mSphere.GetAtHitObj()->GetAc())) == PROC_ALINK) {
                field_0x6c7++;
                mSphere.ClrAtHit();
            }
        }
        break;
    }
}

void daNPC_TK_c::executeBackHanjoDemo() {
    s16 angleY;
    s32 pitch;
    cXyz unkXyz1;

    cXyz offset;
    cXyz handPos = getHanjoHandPos();

    if (checkAttackDemo()) {
        return;
    }

    if (field_0x698 == 0) {
        switch (field_0x694) {
        case 0: {
            offset.set(0.0f, 3400.0f + nREG_F(19), -2000.0f);
            angleY = getMasterPointer()->shape_angle.y - 0x2000;
            cLib_offsetPos(&current.pos, &getMasterPointer()->current.pos, angleY, &offset);

            shape_angle.y = current.angle.y = angleY;

            speed.y = 0.0f;
            speedF = 0.0f;

            mSphere.OffAtSetBit();

            offset.set(800.0f, 900.0f, -800.0f);
            cLib_offsetPos(&field_0x604, &handPos, mpMaster->shape_angle.y, &offset);

            field_0x678 = 30.0f;
            field_0x6b0 = 60;
            field_0x6b4 = 105;

            break;
        }
        case 1: {
            offset.set(200.0f + nREG_F(14), 200.0f + nREG_F(15), -150.0f + nREG_F(16));
            cLib_offsetPos(&field_0x604, &handPos, mpMaster->shape_angle.y, &offset);

            break;
        }
        case 2: {
            offset.set(100.0f + nREG_F(17), 50.0f + nREG_F(18), -70.0f + nREG_F(19));
            cLib_offsetPos(&field_0x604, &handPos, mpMaster->shape_angle.y, &offset);
            setBck(5, 2, 5.0f, 1.0f);

            break;
        }
        case 3: {
            field_0x604 = getHanjoHandPos();
            mSound.startCreatureVoice(Z2SE_HAWK_V_LANDING, -1);
            setBck(9, 2, 5.0f, 1.0f);

            break;
        }
        }

        field_0x698++;
    }

    unkXyz1 = field_0x604 - current.pos;
    s16 sVar4 = cLib_targetAngleY(&current.pos, &field_0x604);
    cM_atan2s(unkXyz1.absXZ(), unkXyz1.y);

    switch (field_0x694) {
    case 1: {
        cLib_chaseF(&field_0x678, 20.0f, 1.0f);
    }
    case 0: {
        if (field_0x6b4 == 1) {
            mSound.startCreatureVoice(0x50025, -1);
        }

        cLib_addCalcAngleS(&shape_angle.y, sVar4, 8, 0x1000, 0x100);
        current.angle.y = shape_angle.y;

        if (field_0x6b0 == 0) {
            pitch = cM_atan2s(unkXyz1.absXZ(), unkXyz1.y);
            cLib_chaseF(&speed.y, field_0x678 * cM_scos(pitch), 1.0f);
            cLib_chaseF(&speedF, field_0x678 * cM_ssin(pitch), 1.0f);

            if (unkXyz1.abs() < 100.0f) {
                field_0x694++;
                field_0x698 = 0;
            }
        }
        break;
    }
    case 2:
    case 3: {
        cLib_chaseF(&field_0x678, 5.0f, 1.0f);

        cLib_addCalcAngleS(&shape_angle.y, sVar4, 8, 0x1000, 0x100);
        current.angle.y = shape_angle.y;

        pitch = cM_atan2s(unkXyz1.absXZ(), unkXyz1.y);
        cLib_chaseF(&speed.y, field_0x678 * cM_scos(pitch), 1.0f);
        cLib_chaseF(&speedF, field_0x678 * cM_ssin(pitch), 1.0f);

        if (field_0x694 == 2) {
            if (unkXyz1.abs() < 100.0f + nREG_F(15)) {
                if (checkBck(7) == 0) {
                    setBck(7, 2, 5.0f, 1.0f);
                }

                if (unkXyz1.abs() < 30.0f) {
                    field_0x694 = 3;
                    field_0x698 = 0;
                }
            }
            break;
        }

        if (unkXyz1.abs() <= 5.0f) {
            setActionMode(6);
        }

        break;
    }
    }
}

void daNPC_TK_c::executeWolfEvent() {
    mIsExecutingAction = false;
    if (eventInfo.checkCommandDemoAccrpt() != 0) {
        dComIfGp_getEvent().setSkipProc(this, dEv_defaultSkipProc, 0);
        setActionMode(12);
        executePerch();
    } else {
        fopAcM_orderOtherEvent(this, daPy_getPlayerActorClass(), "TAKAYOSE2", -1, 0, 3);
    }
    return;
}

void daNPC_TK_c::calcWolfDemoCam() {
    cXyz cStack_1c = daPy_getPlayerActorClass()->current.pos;
    s16 angleY = daPy_getPlayerActorClass()->shape_angle.y;
    cXyz cStack_28;
    cXyz cStack_34;

    cStack_28.set(nREG_F(0), 150.0f + nREG_F(1), 200.0f + nREG_F(2));

    cLib_offsetPos(&cStack_34, &cStack_1c, angleY, &cStack_28);
    cLib_addCalcPos2(&field_0x6fc, cStack_34, 0.1f, 3.0f);
}

void daNPC_TK_c::calcWolfDemoCam2() {
    cXyz targetPos;
    cXyz curPos = dPath_GetPnt(mWolfPathData, mPathStep2)->m_position;
    cXyz prevPos = dPath_GetPnt(mWolfPathData, mPathStep2 - 1)->m_position;
    cLib_addCalcPos2(&field_0x6fc, curPos, 0.2f, field_0x714);

    cXyz offset(0.0f, 0.0f, 400.0f);
    cLib_offsetPos(&targetPos, &curPos, cLib_targetAngleY(&curPos, &prevPos), &offset);

    cLib_addCalcPos2(&field_0x6f0, targetPos, 0.2f, field_0x714);
    cLib_chaseF(&field_0x714, 10.0f + nREG_F(16), 1.5f + nREG_F(17));
}

void daNPC_TK_c::executeWolfPerch() {
    dCamera_c* camera = dCam_getBody();
    cXyz playerPos = daPy_getPlayerActorClass()->current.pos;
    daPy_getPlayerActorClass();  // debug match
    cXyz posOffset;
    cXyz pathPnt1;
    cXyz pathPnt2;

    switch (field_0x6c5) {
    case 0: {
        if (eventInfo.checkCommandDemoAccrpt() != 0) {
            executePerch();
            break;
        }

        field_0x6c5 = 1;
    }
    case 1: {
        if (eventInfo.checkCommandDemoAccrpt() == 0) {
            fopAcM_orderPotentialEvent(this, 2, -1, 3);
            eventInfo.onCondition(dEvtCnd_CANDEMO_e);
            return;
        }

        camera->Stop();
        camera->SetTrimSize(3);

        field_0x6b0 = 0x3c;
        field_0x6c5 = 2;

        mWolfPathData = dPath_GetRoomPath(mpPath1->m_nextID, fopAcM_GetRoomNo(this));
        JUT_ASSERT(2498, mWolfPathData != NULL);

        field_0x6ea = mWolfPathData->field_0x6;
        field_0x6e8 = mWolfPathData->field_0x4;
        field_0x6e9 = mWolfPathData->field_0x7;
        field_0x6d0.Init(mWolfPathData);

        if (field_0x6e8 != 1) {
            field_0x6e8 = 0;
        }

        mPathStep2 = 0;
        pathPnt1 = dPath_GetPnt(mWolfPathData, mPathStep2)->m_position;
        daPy_getPlayerActorClass()->setPlayerPosAndAngle(
            &playerPos, cLib_targetAngleY(&pathPnt1, &playerPos), 0);

        s16 angleY = cLib_targetAngleY(&pathPnt1, &playerPos);

        setBck(8, 2, 3.0f, 1.0f);

        speed.y = -32.0f;
        speedF = 20.0f;

        posOffset.set(-150.0f, 100.0f, 100.0f);
        cLib_offsetPos(&field_0x604, &playerPos, angleY, &posOffset);

        posOffset.set(-500.0f, 610.0f, 850.0f);
        cLib_offsetPos(&current.pos, &field_0x604, angleY, &posOffset);

        old.pos = current.pos;

        shape_angle.set(0, 0, 0);

        shape_angle.y = current.angle.y = cLib_targetAngleY(&current.pos, &field_0x604);
        shape_angle.x = cM_atan2s(speedF, speed.y) - 0x4000;

        posOffset.set(nREG_F(0) - 200.0f, 150.0f + nREG_F(1), 200.0f + nREG_F(2));
        cLib_offsetPos(&field_0x6fc, &playerPos, angleY, &posOffset);

        posOffset.set(nREG_F(3) - 200.0f, 50.0f + nREG_F(4), nREG_F(5) - 150.0f);
        cLib_offsetPos(&field_0x6f0, &playerPos, angleY, &posOffset);

        field_0x708 = 60.0f + nREG_F(6);

        field_0x6b0 = 5;
    }
    case 2: {
        calcWolfDemoCam();

        shape_angle.x = cM_atan2s(speedF, speed.y) - 0x4000;

        if (field_0x6b0 == 0 && cLib_chaseF(&speed.y, 0.0f, 1.0f) != 0) {
            setBck(7, 2, 5.0f, 1.0f);

            field_0x6c5 = 3;
            field_0x604.set(-100.0f, 150.0f, 0.0f);
        }

        break;
    }
    case 3: {
        calcWolfDemoCam();

        shape_angle.x = cM_atan2s(speedF, speed.y) - 0x4000;
        cLib_addCalcAngleS(&shape_angle.y, fopAcM_searchPlayerAngleY(this), 8, 0x800, 0x100);

        cLib_chaseF(&speedF, 0.0f, 1.0f + nREG_F(6));
        if (cLib_chaseF(&speed.y, 6.0f, 0.5f) != 0) {
            field_0x6c5 = 4;
        }

        break;
    }
    case 4: {
        calcWolfDemoCam();

        cLib_addCalcAngleS(&shape_angle.x, 0, 8, 0x100, 0x10);
        cLib_addCalcAngleS(&shape_angle.y, fopAcM_searchPlayerAngleY(this), 8, 0x800, 0x100);

        cLib_chaseF(&speedF, 0.0f, 0.5f + nREG_F(7));
        if (cLib_chaseF(&speed.y, -3.0f, 0.5f + nREG_F(8)) != 0) {
            field_0x6c5 = 5;
            field_0x6b0 = 15;
        }

        break;
    }
    case 5: {
        calcWolfDemoCam();

        cLib_addCalcAngleS(&shape_angle.x, 0, 8, 0x100, 0x10);

        s16 angleY = fopAcM_searchPlayerAngleY(this);
        cLib_addCalcAngleS(&shape_angle.y, angleY, 8, 0x800, 0x100);

        cLib_chaseF(&speedF, 0.0f, 0.5f + nREG_F(7));
        if (cLib_chaseF(&speed.y, 0.0f, 0.5f + nREG_F(9)) != 0) {
            field_0x6c5 = 6;
            field_0x6b0 = 15;
        }

        break;
    }
    case 6: {
        calcWolfDemoCam();

        cLib_addCalcAngleS(&shape_angle.x, 0, 8, 0x100, 0x10);

        cLib_chaseF(&speedF, 0.0f, nREG_F(7) + 0.5f);

        if (field_0x6b0 == 0) {
            field_0x6eb = 0;
            if (field_0x6e9 != 0xff) {
                if (field_0x6e8 == 0) {
                    if (dComIfGs_isTbox(field_0x6e9) != 0) {
                        field_0x6eb = 1;
                    }
                } else {
                    if (dComIfGs_isSaveItem(field_0x6e9) != 0) {
                        field_0x6eb = 1;
                    }
                }
            }

            if (strcmp(dComIfGp_getStartStageName(), "F_SP103") == 0 &&
                dComIfGp_roomControl_getStayNo() == 0 && dComIfG_play_c::getLayerNo(0) == 1)
            {
                field_0x6eb = 1;
                mMsgFlow.init(this, 0xbcc, 0, NULL);
            } else if (field_0x6eb == 0) {
                mMsgFlow.init(this, 0xbce, 0, NULL);
            } else {
                mMsgFlow.init(this, 0xbcd, 0, NULL);
            }

            field_0x6c5 = 8;
            field_0x6b0 = 10;
            field_0x678 = 2.0f;
        }

        break;
    }
    case 8: {
        calcWolfDemoCam();

        cLib_addCalcAngleS(&shape_angle.x, 0, 8, 0x100, 0x10);

        cLib_chaseF(&speed.y, field_0x678, nREG_F(7) + 0.2f);

        if (field_0x678 > 0.0f) {
            if (current.pos.y - playerPos.y > 120.0f) {
                field_0x6b0 = 0;
            }
        } else {
            if (current.pos.y - playerPos.y < 80.0f) {
                field_0x6b0 = 0;
            }
        }

        if (field_0x6b0 == 0) {
            field_0x6b0 = cM_rndFX(3.0f) + 7.0f;
            field_0x678 = -field_0x678;
        }

        if (mMsgFlow.doFlow(this, NULL, 0) != 0) {
            field_0x6c5 = 9;
            field_0x6b0 = 30;
        }

        break;
    }
    case 9: {
        cLib_chaseF(&speed.y, 0.0f, nREG_F(7) + 0.5f);
        if (field_0x6b0 == 0) {
            mPathStep2 = 0;
            field_0x678 = 0.0f;

            setBck(6, 2, 10.0f, 1.0f);

            field_0x69c = 0;

            if (field_0x6eb == 0) {
                field_0x6c5 = 10;
                field_0x6b0 = 55;
                field_0x6e0 = 0.15f + nREG_F(0x11);

                f32 fVar11 = 0.0f;
                cXyz cStack_68;
                cXyz cStack_74;
                for (s32 i = 1; i < mWolfPathData->m_num - 1; i++) {
                    cStack_68 = dPath_GetPnt(mWolfPathData, i - 1)->m_position;
                    cStack_74 = dPath_GetPnt(mWolfPathData, i)->m_position;
                    fVar11 += cStack_68.abs(cStack_74);
                }
                field_0x6e4 = 30.0f / fVar11;
            } else {
                field_0x6c5 = 20;
                field_0x6b0 = 120;
            }
        }

        break;
    }
    case 10: {
        if (field_0x6b0 == 0) {
            field_0x6c5 = 11;
        }
    }
    case 11:
    case 12: {
        field_0x714 = field_0x678 + 1.0f;

        if (field_0x6c5 == 10) {
            posOffset.set(0.0f, 0.0f, 200.0f);
            cLib_offsetPos(&pathPnt1, &current.pos, shape_angle.y, &posOffset);
            cLib_addCalcPos2(&field_0x6fc, pathPnt1, 0.2f, field_0x714);
        } else {
            posOffset.set(0.0f, 0.0f, 200.0f);
            cLib_offsetPos(&pathPnt1, &current.pos, shape_angle.y, &posOffset);
            cLib_addCalcPos2(&field_0x6fc, pathPnt1, 0.2f, field_0x714);

            posOffset.set(0.0f, 30.0f, -200.0f);
            cLib_offsetPos(&pathPnt1, &current.pos, shape_angle.y, &posOffset);
            cLib_addCalcPos2(&field_0x6f0, pathPnt1, 0.1f, field_0x714);

            field_0x708 = 60.0f + nREG_F(6);
        }

        if (field_0x6c5 != 12) {
            pathPnt2 = field_0x6d0.bSpline2(field_0x6e0);
        } else {
            pathPnt2 = dPath_GetPnt(mWolfPathData, mPathStep2)->m_position;
        }

        cLib_chaseF(&field_0x678, 30.0f, 0.5f);

        if (field_0x6c5 == 11) {
            cLib_addCalcAngleS(&shape_angle.y, cLib_targetAngleY(&current.pos, &pathPnt2), 8, 0x400,
                               0x10);
            current.angle.y = shape_angle.y;
            cLib_addCalcAngleS(&shape_angle.x, -cLib_targetAngleX(&current.pos, &pathPnt2), 8,
                               0x400, 0x10);
        } else {
            s16 unkUnused1 = current.pos.abs(pathPnt2) / 100.0f - 2.0f;
            cLib_addCalcAngleS(&shape_angle.y, cLib_targetAngleY(&current.pos, &pathPnt2), 8, 0x400,
                               0x80);
            current.angle.y = shape_angle.y;
            cLib_addCalcAngleS(&shape_angle.x, -cLib_targetAngleX(&current.pos, &pathPnt2), 8,
                               0x400, 0x80);
        }

        speedF = field_0x678 * cM_scos(-shape_angle.x);
        speed.y = field_0x678 * cM_ssin(-shape_angle.x);

        if (field_0x6c5 != 12) {
            if (current.pos.abs(pathPnt2) < 200.0f) {
                field_0x6e0 = field_0x6e0 + field_0x6e4;
            }

            cXyz pathPnt3 = dPath_GetPnt(mWolfPathData, mWolfPathData->m_num - 1)->m_position;
            if (pathPnt3.abs(current.pos) < 500.0f) {
                cXyz cStack_8c;
                cXyz cStack_98 = dPath_GetPnt(mWolfPathData, mWolfPathData->m_num - 2)->m_position;
                cXyz cStack_a4 = dPath_GetPnt(mWolfPathData, mWolfPathData->m_num - 1)->m_position;
                mDoMtx_stack_c::YrotS(-cLib_targetAngleY(&cStack_98, &cStack_a4));
                mDoMtx_stack_c::transM(-current.pos.x, -current.pos.y, -current.pos.z);
                mDoMtx_stack_c::multVec(&cStack_98, &cStack_8c);

                if (cStack_8c.z < 0.0f) {
                    mPathStep2 = mWolfPathData->m_num - 1;
                    field_0x6c5 = 12;
                }
            }
        } else {
            if (current.pos.abs(pathPnt2) < 500.0f) {
                mPathStep2++;
                if (mPathStep2 >= mWolfPathData->m_num) {
                    mPathStep2 = mWolfPathData->m_num - 1;
                    field_0x6c5 = 13;
                    setBck(5, 2, 5.0f, 1.0f);
                    field_0x714 = field_0x678 + 3.0f;
                }
            }
        }

        break;
    }
    case 13: {
        pathPnt2 = dPath_GetPnt(mWolfPathData, mPathStep2)->m_position;

        calcWolfDemoCam2();

        cLib_addCalcAngleS(&shape_angle.y, cLib_targetAngleY(&current.pos, &pathPnt2), 0x10, 0x800,
                           0x10);

        current.angle.y = shape_angle.y;
        shape_angle.x = cM_atan2s(speedF, speed.y) - 0x4000;

        cLib_chaseF(&speedF, 0.0f, nREG_F(6) + 1.0f);
        if (cLib_chaseF(&speed.y, nREG_F(0x11) + 12.0f, nREG_F(7) + 1.0f) != 0) {
            field_0x6c5 = 14;
            setBck(7, 2, 5.0f, 1.0f);
        }

        break;
    }
    case 14: {
        pathPnt2 = dPath_GetPnt(mWolfPathData, mPathStep2)->m_position;

        calcWolfDemoCam2();

        cLib_addCalcAngleS(&shape_angle.x, 0, 8, 0x100, 0x10);
        cLib_addCalcAngleS(&shape_angle.y, cLib_targetAngleY(&current.pos, &pathPnt2), 8, 0x800,
                           0x100);
        current.angle.y = shape_angle.y;

        cLib_chasePos(&current.pos, pathPnt2, 1.0f + nREG_F(5));
        cLib_chaseF(&speedF, 0.0f, 1.0f + nREG_F(8));

        if (cLib_chaseF(&speed.y, -5.0f, 0.7f + nREG_F(9)) != 0) {
            field_0x6c5 = 15;
            field_0x6b0 = 15;
        }

        break;
    }
    case 15: {
        pathPnt2 = dPath_GetPnt(mWolfPathData, mPathStep2)->m_position;

        calcWolfDemoCam2();

        cLib_addCalcAngleS(&shape_angle.x, 0, 8, 0x100, 0x10);
        cLib_addCalcAngleS(&shape_angle.y, cLib_targetAngleY(&current.pos, &pathPnt2), 8, 0x800,
                           0x100);

        cLib_chasePos(&current.pos, pathPnt2, (nREG_F(5) + 1.0f));
        cLib_chaseF(&speedF, 0.0f, 0.5f + nREG_F(7));
        if (cLib_chaseF(&speed.y, 0.0f, 0.5f + nREG_F(10)) != 0) {
            field_0x6c5 = 6;
            field_0x6b4 = 120;
            speed.y = 0.0f;
            speedF = 0.0f;
            field_0x6c5 = 16;
            field_0x6b0 = 10;
            field_0x678 = 2.0f;
        }

        break;
    }
    case 16: {
        calcWolfDemoCam2();

        pathPnt2 = dPath_GetPnt(mWolfPathData, mPathStep2)->m_position;
        cLib_addCalcPos2(&field_0x6fc, pathPnt2, 0.1f, 3.0f);

        s16 angleY = cLib_targetAngleY(&current.pos,
                                       &dPath_GetPnt(mWolfPathData, mPathStep2 - 1)->m_position);
        cLib_addCalcAngleS(&shape_angle.y, angleY + 0x2000, 8, 0x100, 0x10);

        cLib_chaseF(&speed.y, field_0x678, 0.5f);

        if (field_0x678 > 0.0f) {
            if (current.pos.y - pathPnt2.y > 30.0f) {
                field_0x6b0 = 0;
            }
        } else {
            if (current.pos.y - pathPnt2.y < -30.0f) {
                field_0x6b0 = 0;
            }
        }

        if (field_0x6b0 == 0) {
            field_0x6b0 = cM_rndFX(3.0f) + 7.0f;
            field_0x678 = -field_0x678;
        }

        if (field_0x6b4 == 0) {
            setBck(6, 2, 10.0f, 1.0f);
            field_0x6b0 = 60;
            current.angle.y = shape_angle.y;
            field_0x6c5 = 17;
            field_0x678 = 0.0f;
        }

        break;
    }
    case 17: {
        cLib_chaseF(&field_0x678, 30.0f, 1.0f);

        cLib_addCalcAngleS(&shape_angle.x, ~0x5fff, 8, 0x200, 0x10);

        speedF = field_0x678 * cM_scos(-shape_angle.x);
        speed.y = field_0x678 * cM_ssin(-shape_angle.x);

        if (field_0x6b0 == 0) {
            dComIfGs_onSwitch(field_0x6ea, fopAcM_GetRoomNo(this));
            camera->Reset(field_0x6fc, field_0x6f0);
            camera->Start();
            camera->SetTrimSize(0);
            dComIfGp_event_reset();
            field_0x698 = 2;
            setActionMode(4);
        }
        break;
    }
    case 20: {
        field_0x714 = field_0x678 + 3.0f;

        posOffset.set(0.0f, 0.0f, 200.0f);
        cLib_offsetPos(&pathPnt1, &current.pos, shape_angle.y, &posOffset);

        cLib_addCalcPos2(&field_0x6fc, pathPnt1, 0.2f, field_0x714);

        cLib_chaseF(&field_0x678, 30.0f, 1.0f);

        cLib_addCalcAngleS(&shape_angle.x, ~0x5fff, 8, 0x200, 0x10);
        speedF = field_0x678 * cM_scos(-shape_angle.x);
        speed.y = field_0x678 * cM_ssin(-shape_angle.x);

        if (field_0x6b0 == 0) {
            dComIfGs_onSwitch(field_0x6ea, fopAcM_GetRoomNo(this));

            camera->Reset(field_0x6fc, field_0x6f0);
            camera->Start();
            camera->SetTrimSize(0);

            dComIfGp_event_reset();

            field_0x698 = 2;

            setActionMode(4);

            break;
        }
    }
    }

    camera->Set(field_0x6fc, field_0x6f0, field_0x708, 0);
}

void daNPC_TK_c::executeResistanceDemo() {
    daNpcMoiR_c* npcMoiR;
    if (fopAcM_SearchByName(PROC_NPC_MOIR, (fopAc_ac_c**)&npcMoiR) == NULL || npcMoiR == NULL) {
        return;
    }

    cXyz offset;
    cXyz posWithOffset;
    s16 sVar1 = shape_angle.y;
    cXyz basePos(-5191.0f, 2000.0f, 5246.0f);

    s16 unkInt1 = -0x1faf;
    s16 targetAngle;
    switch (field_0x694) {
    case 0:
        field_0x6a2 = field_0x6a0 = 0;

        offset.set(-700.0f + nREG_F(0), 400.0f + nREG_F(1), 700.0f + nREG_F(2));
        cLib_offsetPos(&current.pos, &basePos, unkInt1, &offset);

        shape_angle.y = unkInt1 - 0x3000;
        current.angle.y = shape_angle.y;
        current.angle.x = 0x800;
        shape_angle.x = -current.angle.x;
        shape_angle.z = 0;

        field_0x678 = 40.0f;
        field_0x69c = 0;
        field_0x694 = 1;
        field_0x6b0 = 0x1e;

        setBck(6, 2, 3.0f, 1.0f);
    case 1:
        if (field_0x6b0 == 0) {
            cLib_chaseAngleS(&field_0x69c, 0x400, 0x10);
            shape_angle.y -= field_0x69c;
            current.angle.y = shape_angle.y;
        }

        speedF = field_0x678 * fabsf(cM_scos(current.angle.x));
        speed.y = field_0x678 * cM_ssin(current.angle.x);

        targetAngle = cLib_targetAngleY(&current.pos, &basePos);
        if (abs((s16)(shape_angle.y - targetAngle)) < 0x4000) {
            field_0x694 = 2;
            setBck(8, 2, 10.0f, 1.0f);
        }

        break;
    case 2:
        offset.set(700.0f + nREG_F(3), 200.0f + nREG_F(4), 200.0f + nREG_F(5));
        cLib_offsetPos(&posWithOffset, &basePos, unkInt1, &offset);

        cLib_addCalcAngleS(&current.angle.x, cLib_targetAngleX(&current.pos, &posWithOffset), 4,
                           0x200, 0x10);
        shape_angle.x = -current.angle.x;

        cLib_addCalcAngleS(&current.angle.y, cLib_targetAngleY((Vec*)&current, &posWithOffset), 8,
                           0x400, 0x10);
        shape_angle.y = current.angle.y;

        cLib_chaseF(&field_0x678, 50.0f, 0.1f);

        speedF = field_0x678 * fabsf(cM_scos(current.angle.x));
        speed.y = field_0x678 * cM_ssin(current.angle.x);

        if (posWithOffset.abs(current.pos) < 500.0f) {
            field_0x694 = 3;
            field_0x69c = 0;
            field_0x610.set(-4058.0f, 549.0f, 7530.0f);
            field_0x604.set(-4107.0f, 163.0f, 8046.0f);
        }

        break;
    case 3:
        posWithOffset = field_0x610;

        cLib_chaseAngleS(&field_0x69c, 0x400, 0x20);

        cLib_addCalcAngleS(&current.angle.x, cLib_targetAngleX(&current.pos, &posWithOffset), 8,
                           field_0x69c, 0x10);
        shape_angle.x = -current.angle.x;

        cLib_addCalcAngleS(&current.angle.y, cLib_targetAngleY(&current.pos, &posWithOffset), 8,
                           field_0x69c, 0x10);
        shape_angle.y = current.angle.y;

        cLib_chaseF(&field_0x678, 50.0f, 1.0f);

        speedF = field_0x678 * fabsf(cM_scos(current.angle.x));
        speed.y = field_0x678 * cM_ssin(current.angle.x);

        if (posWithOffset.abs(current.pos) < 150.0f) {
            field_0x694 = 4;
            setBck(7, 2, 5.0f, 1.0f);
            field_0x678 = 0.0f;
            field_0x69c = 0;
        }

        break;
    case 4:
        posWithOffset = field_0x604;

        cLib_chaseAngleS(&field_0x69c, 0x400, 0x40);

        cLib_addCalcAngleS(&current.angle.y, cLib_targetAngleY(&current.pos, &posWithOffset), 8,
                           field_0x69c, 0x10);
        shape_angle.y = current.angle.y;

        cLib_addCalcAngleS(&current.angle.x, cM_atan2s(speedF, speed.y) - 0x4000, 8, 0x200, 0x10);
        shape_angle.x = -current.angle.x;

        cLib_chaseF(&speedF, 0.0f, 1.0f);
        if (cLib_chaseF(&speed.y, 6.0f, 1.4f) != 0) {
            field_0x694 = 5;
        }

        break;
    case 5:
        posWithOffset = field_0x604;

        cLib_addCalcAngleS(&current.angle.y, cLib_targetAngleY(&current.pos, &posWithOffset), 8,
                           field_0x69c, 0x10);
        cLib_addCalcAngleS(&shape_angle.y, npcMoiR->shape_angle.y + 0x2800 + XREG_S(6), 8, 0x400,
                           0x10);

        cLib_addCalcAngleS(&current.angle.x, 0, 8, 0x100, 0x10);
        shape_angle.x = current.angle.x;

        cLib_chaseF(&speedF, 3.0f, 1.0f);
        if (cLib_chaseF(&speed.y, 0.0f, 1.5f) != 0) {
            speed.y = 0.0f;
            field_0x694 = 6;
            setBck(9, 0, 10.0f, 1.0f);
            field_0x678 = 0.0f;
        }

        break;
    case 6:
        posWithOffset = field_0x604;
        cLib_chaseF(&speedF, 0.0f, 0.5f);

        cLib_addCalcAngleS(&current.angle.y, npcMoiR->shape_angle.y + 0x3000, 8, field_0x69c, 0x10);
        cLib_addCalcAngleS(&shape_angle.y, npcMoiR->shape_angle.y + 0x2800 + XREG_S(6), 8, 0x400,
                           0x10);

        cLib_addCalcAngleS(&current.angle.x, 0, 8, 0x100, 0x10);
        shape_angle.x = current.angle.x;

        cLib_addCalcAngleS(&shape_angle.z, -0x400 + XREG_S(8), 8, 0x200, 0x10);

        cLib_chaseF(&field_0x678, 4.0f, 1.0f);
        if (cLib_chasePos(&current.pos, posWithOffset, field_0x678) != 0) {
            field_0x694 = 7;
            speedF = speed.y = 0.0f;
            mSound.startCreatureVoice(Z2SE_HAWK_V_LANDING, -1);
        }

        return;
    case 7:
        if (mpMorf->isStop() != 0) {
            setBck(0xc, 2, 3.0f, 1.0f);
        }
    case 8:
        cLib_addCalcAngleS2(&field_0x6a2, 0xffffdc00, 8, 0x200);
        cLib_addCalcAngleS2(&field_0x6a0, 0x1000, 8, 0x200);
        cLib_addCalcAngleS2(&field_0x6aa, 0x2000, 8, 0x200);

        speed.y = speedF = 0.0f;

        mDoMtx_stack_c::copy(npcMoiR->getHandRMtx());
        mDoMtx_stack_c::transM(-15.0f + XREG_F(3), -1.0f + XREG_F(4), -5.0f + XREG_F(5));
        mDoMtx_stack_c::multVecZero(&current.pos);

        shape_angle.y = current.angle.y = npcMoiR->shape_angle.y + 0x2800 + XREG_S(6);
        shape_angle.x = XREG_S(7);
        shape_angle.z = -0x400 + XREG_S(8);

        return;
    }

    s16 local_48 = (sVar1 - shape_angle.y) * 10;
    if (local_48 < -0x2000) {
        local_48 = -0x2000;
    }
    if (local_48 > 0x2000) {
        local_48 = 0x2000;
    }

    cLib_addCalcAngleS(&shape_angle.z, local_48, 0x10, 0x200, 0x10);
}

void daNPC_TK_c::setHawkSideCamera(cXyz param_0) {
    field_0x710 = 4;

    cXyz unkXyz(600.0f, 300.0f, 600.0f);
    field_0x6f0 = param_0 + unkXyz;

    field_0x6fc = param_0;
    field_0x6fc.y += 80.0f;

    field_0x708 = 50.0f;
    field_0x714 = 5.0f;
    field_0x70c = 120;
}

void daNPC_TK_c::setHawkCamera(fopAc_ac_c* param_0) {
    field_0x6ec = param_0;
    field_0x70c = 300;
    cXyz acStack_28(0.0f, -100.0f, 1200.0f);
    cLib_offsetPos(&field_0x604, &param_0->current.pos, 0x2000 - shape_angle.y, &acStack_28);
}

void daNPC_TK_c::endHawkCamera() {
    if (field_0x710 == 2 || field_0x710 == 3) {
        field_0x710 = 5;
        field_0x6ec = NULL;
        field_0x6fc = daPy_getPlayerActorClass()->current.pos;
        field_0x6fc.y += 100.0f;
        cXyz acStack_28(0.0f, 100.0f, -300.0f);
        cLib_offsetPos(&field_0x6f0, &field_0x6fc, daPy_getPlayerActorClass()->shape_angle.y,
                       &acStack_28);
    }
}

void daNPC_TK_c::calcDemoCamera() {
    dCamera_c* camera;

    camera = dCam_getBody();

    cXyz offset(0.0f, 0.0f, 0.0f);
    cXyz pos;
    cXyz playerPos = daPy_getPlayerActorClass()->current.pos;
    playerPos.y += 150.0f;

    f32 unkFloat;
    switch (field_0x710) {
    case 1: {
        field_0x6fc = camera->Center();
        field_0x6f0 = camera->Eye();
        field_0x710 = 2;
        field_0x718 = 0;

        break;
    }
    case 2: {
        if (field_0x6ec == NULL || (field_0x70c == 0)) {
            field_0x710 = 5;
        } else {
            if (field_0x6ec == this) {
                pos = field_0x6ec->current.pos + offset;
                unkFloat = field_0x6f0.abs(pos);

                if (mActionType == 3) {
                    if (field_0x694 == 1) {
                        if (unkFloat >= 500.0f) {
                            cLib_chaseF(&field_0x714, l_HIO.field_0x28, 5.0f);
                        } else {
                            cLib_chaseF(&field_0x714, 0.0f, 5.0f);
                        }
                        cLib_chasePos(&field_0x6f0, pos, field_0x714);
                    } else if (field_0x694 == 2) {
                        if (unkFloat >= 700.0f) {
                            cLib_chaseF(&field_0x714, l_HIO.field_0x28, 5.0f);
                        } else {
                            if (unkFloat < 400.0f) {
                                cLib_chaseF(&field_0x714, -l_HIO.field_0x28, 5.0f);
                            } else {
                                field_0x714 = 0.0f;
                            }
                        }
                        cLib_chasePos(&field_0x6f0, pos, field_0x714);
                    }
                } else if (mActionType == 4) {
                    cLib_chaseF(&field_0x714, 0.0f, 3.0f);
                    if (field_0x714) {
                        cLib_chasePos(&field_0x6f0, pos, field_0x714);
                    }
                } else if (mActionType == 5) {
                    if (field_0x694 <= 1) {
                        cLib_chaseF(&field_0x714, 30.0f, 5.0f);
                        cLib_chasePos(&field_0x6f0, pos, field_0x714);
                    } else if (field_0x694 == 2) {
                        unkFloat = field_0x678 - 5.0f;
                        if (unkFloat < 0.0f) {
                            unkFloat = 0.0f;
                        }
                        cLib_chaseF(&field_0x714, unkFloat, 1.0f);
                        pos.set(field_0x63c[1].x, current.pos.y, field_0x63c[1].z);
                        cLib_chasePos(&field_0x6f0, pos, field_0x714);
                    } else if (field_0x694 == 3) {
                        cLib_chaseF(&field_0x714, 3.0f, 1.0f);
                        pos.set(field_0x63c[1].x, current.pos.y, field_0x63c[1].z);
                        field_0x718 = 0;
                        cLib_chasePos(&field_0x6f0, pos, field_0x714);
                    } else if (field_0x694 >= 10) {
                        if (field_0x718 == 0) {
                            cLib_chaseF(&field_0x714, 5.0f, 1.0f);
                            pos.set(field_0x6f0.x, current.pos.y, field_0x6f0.z);
                            unkFloat = field_0x6f0.abs(current.pos);
                            if (unkFloat > 600.0f + nREG_F(11)) {
                                field_0x718 = 1;
                            }
                            cLib_chasePos(&field_0x6f0, pos, field_0x714);
                        } else {
                            cLib_chaseF(&field_0x714, 55.0f + nREG_F(12), 3.0f);
                            offset.set(0.0f, 10.0f, -30.0f);
                            cLib_offsetPos(&pos, &field_0x6ec->current.pos, shape_angle.y, &offset);
                            if (field_0x6f0.abs(playerPos) < 1000.0f) {
                                field_0x710 = 3;
                                break;
                            }
                            cLib_addCalcPos2(&field_0x6f0, pos, 0.1f, field_0x714);
                        }
                    }
                } else {
                    field_0x714 = 0;
                }

                dBgS_GndChk gndChk;
                gndChk.SetPos(&field_0x6f0);
                unkFloat = dComIfG_Bgsp().GroundCross(&gndChk);
                if (field_0x6f0.y < unkFloat + 50.0f) {
                    cLib_chaseF(&(field_0x6f0).y, unkFloat + 50.0f, 10.0f);
                }
            } else {
                offset.set(0.0f, 50.0f, 0.0f);
                cLib_offsetPos(&field_0x6f0, &field_0x604,
                               daPy_getPlayerActorClass()->shape_angle.y, &offset);
            }
            offset.set(0.0f, nREG_F(10) + 80.0f, 0.0f);
            cLib_addCalcPos(&field_0x6fc, field_0x6ec->current.pos + offset, 0.8f, 50.0f, 1.0f);

            camera->Set(field_0x6fc, field_0x6f0);
        }

        break;
    }
    case 3: {
        if ((field_0x6ec == NULL) || (field_0x70c == 0)) {
            field_0x710 = 5;
        } else {
            if ((daNPC_TK_c*)field_0x6ec == this) {
                offset.y = 150.0f;
                pos = field_0x6ec->current.pos + offset;
                cLib_chaseF(&field_0x714, 1.0f, 3.0f);
                cLib_chasePos(&field_0x6f0, pos, field_0x714);
            }

            offset.set(0.0f, 80.0f, 0.0f);
            cLib_addCalcPos(&field_0x6fc, field_0x6ec->current.pos + offset, 0.8f, 50.0f, 1.0f);

            camera->Set(field_0x6fc, field_0x6f0);
        }

        break;
    }
    case 4: {
        cLib_addCalc2(&field_0x708, 30.0f, 0.1f, 0.5f);
        cLib_chasePos(&field_0x6fc, field_0x6f0, field_0x714);

        camera->Set(field_0x6fc, field_0x6f0, field_0x708, 0);

        if (field_0x70c == 0) {
            field_0x710 = 5;
        }

        break;
    }
    case 5: {
        camera->Reset(field_0x6fc, field_0x6f0);
        camera->Start();
        camera->SetTrimSize(0);

        dComIfGp_event_reset();

        field_0x710 = 0;

        break;
    }
    case 0:
        break;
    }
}

void daNPC_TK_c::checkActionSet() {
    cXyz acStack_24;

    if (mFlags == 0) {
        return;
    }

    if (mFlags & 0x1) {
        field_0x69e = -fopCamM_GetAngleX(dComIfGp_getCamera(0));
        setActionMode(3);
        mFlags ^= 0x1;
    } else if (mFlags & 0x2) {
        field_0x698 = 0;
        mSound.startCreatureVoice(Z2SE_HAWK_V_TAKE_OFF, -1);
        setActionMode(4);
        mFlags ^= 0x2;
    } else if (mFlags & 0x4) {
        setAwayAction(1);
        mFlags ^= 0x4;
    } else if (mFlags & 0x8) {
        setActionMode(1);
        mFlags ^= 0x8;
        mpMaster = NULL;
    } else if (mFlags & 0x10) {
        if (mActionType == 6 || mActionType == 8) {
            setActionMode(7);
        }

        mFlags ^= 0x10;
    } else if (mFlags & 0x20) {
        if (mActionType == 7 && field_0x694 != 0 && field_0x6b4 == 0) {
            field_0x698 = 0;
            setActionMode(8);
        }

        mFlags ^= 0x20;
    } else if (mFlags & 0x40) {
        field_0x698 = 0;
        setActionMode(10);

        mFlags ^= 0x40;
    } else if (mFlags & 0x80) {
        mFlags ^= 0x80;

        setActionMode(11);
        mpMaster = NULL;
    } else if (mFlags & 0x100) {
        mFlags ^= 0x100;

        setActionMode(13);
    } else if (mFlags & 0x200) {
        if (mActionType == 0 || mActionType == 10) {
            current.pos = getHanjoHandPos();
            old.pos = current.pos;

            mSphere.OffAtSetBit();

            speed.y = 0.0;
            speedF = 0.0;

            setActionMode(7);
        }
        mFlags ^= 0x200;
    }
}

void daNPC_TK_c::action() {
    s16 origAngleY = shape_angle.y;
    field_0x680 = 0.0f;
    field_0x6c0 = 0;

    checkActionSet();
    calcDemoCamera();

    field_0x6c1 = 0;
    field_0x71a = 0;
    mIsHanjoHand = 0;

    switch (mActionType) {
    case 0:
        mIsExecutingAction = false;
        executeFly();
        break;
    case 1:
        mIsExecutingAction = false;
        if (eventInfo.checkCommandDemoAccrpt()) {
            dComIfGp_getEvent().setSkipZev(this, "TAKAYOSE@");
            setActionMode(2);
            executePerch();
        } else {
            fopAcM_orderOtherEvent(this, daPy_getPlayerActorClass(), "TAKAYOSE", -1, 0, 3);
        }

        break;
    case 2:
        mIsExecutingAction = false;
        if (eventInfo.checkCommandDemoAccrpt()) {
            executePerch();
        } else {
            executeHandOn();
        }

        break;
    case 3:
        mIsExecutingAction = true;
        executeAttack();

        break;
    case 4:
        mIsExecutingAction = true;
        executeAway();

        break;
    case 5:
        mIsExecutingAction = true;
        executeBack();
        break;
    case 6:
        mIsExecutingAction = true;
        executeStayHanjo();

        break;
    case 7:
        mIsExecutingAction = true;
        executeAttackLink();

        break;
    case 8:
        mIsExecutingAction = true;
        executeBackHanjo();

        break;
    case 9:
        mIsExecutingAction = true;
        executeAttackDemo();

        break;
    case 10:
        mIsExecutingAction = true;
        executeBackHanjoDemo();

        break;
    case 0xb:
        executeWolfEvent();

        break;
    case 0xc:
        executeWolfPerch();

        break;
    case 0xd:
        executeResistanceDemo();

        break;
    }

    setFlySE();

    if (mActionType != 13) {
        s16 targetX = 0;
        s16 targetZ = 0;
        if (field_0x6bd != 0) {
            field_0x6bd = 0;

            targetX = cM_atan2s(speedF, speed.y) - 0x4000;
            if (speedF > 0.0f && targetX < -0x2000) {
                targetX = -0x2000;
            }
            f32 speed = speedF;
            if (speed >= 20.0f) {
                speed = 20.0f;
            }

            targetZ = (origAngleY - shape_angle.y) * 7;
            targetZ = targetZ * speed / 20.0f;
            if (targetZ < -0x2000) {
                targetZ = -0x2000;
            }
            if (targetZ > 0x2000) {
                targetZ = 0x2000;
            }
        }

        cLib_chaseAngleS(&shape_angle.x, targetX, 0x180);
        cLib_chaseAngleS(&shape_angle.z, targetZ, 0x200);
    }

    fopAcM_posMoveF(this, 0);

    if (mIsExecutingAction) {
        mAcch.CrrPos(dComIfG_Bgsp());
    }

    mpMorf->play(0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
}

void daNPC_TK_c::mtx_set() {
    if (field_0x6c1 != 0) {
        mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
        mDoMtx_stack_c::ZXYrotM(shape_angle);
        mDoMtx_stack_c::ZXYrotM(field_0x6a4 / 2, 0, -field_0x6a4 / 2);
        mDoMtx_stack_c::transM(-5.0f, 33.0f, -27.0f);
    } else {
        mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
        mDoMtx_stack_c::ZXYrotM(shape_angle);
        mDoMtx_stack_c::transM(-5.0, 33.0, -27.0);
    }

    mDoMtx_stack_c::scaleM(l_HIO.field_0x8, l_HIO.field_0x8, l_HIO.field_0x8);
    mpMorf->getModel()->setBaseTRMtx(mDoMtx_stack_c::get());
    mpMorf->modelCalc();
}

void daNPC_TK_c::cc_set() {
    MTXCopy(mpMorf->getModel()->getAnmMtx(0), mDoMtx_stack_c::get());
    mDoMtx_stack_c::multVecZero(&eyePos);
    attention_info.position = eyePos;
    attention_info.position.y += 30.0f;

    if (field_0x6ae != 0) {
        mSphere.SetC(eyePos);
        mSphere.SetR(80.0f);
        dComIfG_Ccsp()->Set(&mSphere);
    } else {
        mSphere.ResetAtHit();
        mSphere.ResetTgHit();
        mSphere.ResetCoHit();
    }

    field_0x6ae = 0;
}

int daNPC_TK_c::execute() {
    if (field_0x6b0 != 0) {
        field_0x6b0--;
    }

    if (field_0x6b4 != 0) {
        field_0x6b4--;
    }

    if (field_0x70c != 0) {
        field_0x70c--;
    }

    if (field_0x6b8 != 0) {
        field_0x6b8--;
        if (field_0x6b8 == 0) {
            mDoAud_seStart(0xc, 0, 0, 0);
        }
    }

    mSphere.OffAtSetBit();

    action();
    mtx_set();
    cc_set();

    if (field_0x6c6 != 0) {
        field_0x6c6 = 0;
        setCarryActorMtx();
    }

    field_0x628 = old.pos;

    return 1;
}

static int daNPC_TK_Execute(daNPC_TK_c* i_this) {
    return i_this->execute();
}

static int daNPC_TK_IsDelete(daNPC_TK_c* i_this) {
    return 1;
}

int daNPC_TK_c::_delete() {
    dComIfG_resDelete(&mPhase, "Npc_tk");
    if (field_0xb40) {
        g_isHioChildInitted = 0;
    }

    if (heap != NULL) {
        mSound.deleteObject();
    }

    return 1;
}

static int daNPC_TK_Delete(daNPC_TK_c* i_this) {
    return i_this->_delete();
}

int daNPC_TK_c::ctrlJoint(J3DJoint* param_0, J3DModel* param_1) {
    s32 jntNo = param_0->getJntNo();
    mDoMtx_stack_c::copy(param_1->getAnmMtx(jntNo));

    MtxP pMVar2 = mDoMtx_stack_c::get();

    if (mActionType == 13) {
        switch (jntNo) {
        case 15:
            mDoMtx_stack_c::ZXYrotM(0, field_0x6a2, field_0x6a0);
            break;
        case 16:
            mDoMtx_stack_c::ZXYrotM(0, field_0x6a2, field_0x6a0);
            break;
        case 6:
            mDoMtx_stack_c::XrotM(field_0x6aa);
            break;
        }
    } else if (field_0x6c1 == 0) {
        switch (jntNo) {
        case 4:
        case 5:
        case 6:
            pMVar2[1][3] = pMVar2[1][3] + field_0x680 / 3.0f;
            mDoMtx_stack_c::copy(pMVar2);
            break;
        }
    } else {
        switch (jntNo) {
        case 4:
        case 5:
        case 6:
            pMVar2[1][3] = pMVar2[1][3] + field_0x680 / 3.0f;
            mDoMtx_stack_c::copy(pMVar2);
            break;
        case 15:
            mDoMtx_stack_c::ZXYrotM(0, field_0x6a2, -field_0x6a0 / 2);
            break;
        case 16:
            mDoMtx_stack_c::ZXYrotM(0, field_0x6a2, -field_0x6a0 / 2);
            break;
        }
    }

    param_1->setAnmMtx(jntNo, mDoMtx_stack_c::get());
    cMtx_copy(mDoMtx_stack_c::get(), J3DSys::mCurrentMtx);

    return 1;
}

int daNPC_TK_c::JointCallBack(J3DJoint* param_0, int param_1) {
    J3DJoint* joint = param_0;
    if (param_1 == 0) {
        J3DModel* model = j3dSys.getModel();
        daNPC_TK_c* npcTk = (daNPC_TK_c*)model->getUserArea();
        if (npcTk != NULL) {
            npcTk->ctrlJoint(joint, model);
        }
    }

    return 1;
}

int daNPC_TK_c::CreateHeap() {
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes("Npc_tk", "tk.bmd");
    JUT_ASSERT_MSG(0xf4f, modelData != NULL, "  鷹匠");  // falconer

    mpMorf = new mDoExt_McaMorfSO(modelData, NULL, NULL,
                                  (J3DAnmTransform*)dComIfG_getObjectRes("Npc_tk", 6), 0, 1.0f, 0,
                                  -1, &mSound, 0x80000, 0x11000084);
    if (mpMorf == NULL || mpMorf->getModel() == NULL) {
        return 0;
    }

    J3DModel* model = mpMorf->getModel();
    model->setUserArea((uintptr_t)this);

    for (u16 i = 0; i < model->getModelData()->getJointNum(); i++) {
        if (i != 0) {
            model->getModelData()->getJointNodePointer(i)->setCallBack(JointCallBack);
        }
    }

    return 1;
}

static int useHeapInit(fopAc_ac_c* i_this) {
    return ((daNPC_TK_c*)i_this)->CreateHeap();
}

int daNPC_TK_c::create() {
    /* 80B0C4D4-80B0C514 0000E0 0040+00 1/1 0/0 0/0 .data            cc_sph_src$8096 */
    static dCcD_SrcSph cc_sph_src = {
        {
            {0x0, {{AT_TYPE_THROW_OBJ, 0x1, 0x1f}, {0x0, 0x0}, 0x69}},  // mObj
            {dCcD_SE_METAL, 0x0, 0x0, 0x0, 0x0},                        // mGObjAt
            {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0},                         // mGObjTg
            {0x0},                                                      // mGObjCo
        },                                                              // mObjInf
        {
            {{0.0f, 0.0f, 0.0f}, 80.0f}  // mSph
        }  // mSphAttr
    };

    fopAcM_ct(this, daNPC_TK_c);

    int loadRes = dComIfG_resLoad(&mPhase, "Npc_tk");
    eventInfo.setArchiveName("Npc_tk");
    if (loadRes == cPhs_COMPLEATE_e) {
        OS_REPORT("NPC_TK PARAM %x\n", fopAcM_GetParam(this));

        if (!fopAcM_entrySolidHeap(this, useHeapInit, 0x2540)) {
            return cPhs_ERROR_e;
        }

        if (g_isHioChildInitted == 0) {
            field_0xb40 = 1;
            g_isHioChildInitted = 1;
            l_HIO.field_0x4 = mDoHIO_CREATE_CHILD("鷹", &l_HIO);  // hawk
        }

        attention_info.flags = 0;

        fopAcM_SetMtx(this, mpMorf->getModel()->getBaseTRMtx());
        fopAcM_SetMin(this, -200.0f, -200.0f, -200.0f);
        fopAcM_SetMax(this, 200.0f, 200.0f, 200.0f);

        mAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1, &mCircle,
                  fopAcM_GetSpeed_p(this), NULL, NULL);
        mAcch.OnLineCheck();

        mCircle.SetWall(40.0f, 50.0f);

        field_0x560 = health = 100;

        mStts.Init(100, 0, this);

        mSphere.Set(cc_sph_src);
        mSphere.SetStts(&mStts);

        mSound.init(&current.pos, &eyePos, 3, 1);
        mAtInfo.mpSound = &mSound;

        maxFallSpeed = -60.0f;

        daNPC_TK_Execute(this);
    }

    return loadRes;
}

static int daNPC_TK_Create(daNPC_TK_c* i_this) {
    return i_this->create();
}

static actor_method_class l_daNPC_TK_Method = {
    (process_method_func)daNPC_TK_Create,  (process_method_func)daNPC_TK_Delete,
    (process_method_func)daNPC_TK_Execute, (process_method_func)daNPC_TK_IsDelete,
    (process_method_func)daNPC_TK_Draw,
};

extern actor_process_profile_definition g_profile_NPC_TK = {
    fpcLy_CURRENT_e,         // mLayerID
    6,                       // mListID
    fpcPi_CURRENT_e,         // mListPrio
    PROC_NPC_TK,             // mProcName
    &g_fpcLf_Method.base,    // sub_method
    sizeof(daNPC_TK_c),      // mSize
    0,                       // mSizeOther
    0,                       // mParameters
    &g_fopAc_Method.base,    // sub_method
    703,                     // mPriority
    &l_daNPC_TK_Method,      // sub_method
    0x08044000,              // mStatus
    fopAc_NPC_e,             // mActorType
    fopAc_CULLBOX_CUSTOM_e,  // cullType
};

AUDIO_INSTANCES
