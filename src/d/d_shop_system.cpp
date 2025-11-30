/**
 * d_shop_system.cpp
 * Main Shop System
 */

#include "d/dolzel.h" // IWYU pragma: keep

#include "d/d_shop_system.h"
#include "SSystem/SComponent/c_math.h"
#include "d/d_a_shop_item_static.h"
#include "d/d_item.h"
#include "d/d_lib.h"
#include "d/d_select_cursor.h"
#include "d/d_meter2_info.h"
#include "d/d_meter_HIO.h"
#include "d/d_msg_object.h"
#include "m_Do/m_Do_controller_pad.h"
#include "m_Do/m_Do_lib.h"
#include "d/actor/d_a_tag_shop_item.h"


static daTag_ShopItem_c* dShopSystem_itemActor[7] = {
    NULL, NULL, NULL, NULL, NULL, NULL, NULL,
};

static u8 dShopSystem_itemNo[7] = {
    fpcNm_ITEM_NONE, fpcNm_ITEM_NONE, fpcNm_ITEM_NONE, fpcNm_ITEM_NONE, fpcNm_ITEM_NONE, fpcNm_ITEM_NONE, fpcNm_ITEM_NONE,
};

static u8 dShopSystem_sellItemMax;

static int dShopSystem_item_count;

static u8 data_80451060;

static int dShopSystem_searchItemActor(void* i_actor, void* param_1) {
    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_TAG_SHOPITM) {
        if ((fopAcM_GetParam(param_1) & 0xF0000000) == (fopAcM_GetParam(i_actor) & 0xF0000000) &&
            dShopSystem_item_count < dShopSystem_sellItemMax)
        {
            u8 i_posID = (fopAcM_GetParam(i_actor) >> 0x18) & 0xF;
            JUT_ASSERT(0, i_posID <= dShopSystem_c::ITEM_MAX_e);

            if (dShopSystem_itemActor[0] != i_actor && dShopSystem_itemActor[1] != i_actor &&
                dShopSystem_itemActor[2] != i_actor && dShopSystem_itemActor[3] != i_actor &&
                dShopSystem_itemActor[4] != i_actor && dShopSystem_itemActor[5] != i_actor &&
                dShopSystem_itemActor[6] != i_actor)
            {
                u8 sw = static_cast<fopAc_ac_c*>(i_actor)->home.angle.z;
                u8 sw2 = ((u16) static_cast<fopAc_ac_c*>(i_actor)->home.angle.z) >> 8;
                u8 item_no = fopAcM_GetParam(i_actor) & 0xFF;

                if ((sw == 0xFF || !dComIfGs_isSaveSwitch(sw)) &&
                    (sw2 == 0xFF || dComIfGs_isSaveSwitch(sw2)))
                {
                    if (sw != 0xFF && item_no == fpcNm_ITEM_HYLIA_SHIELD && checkItemGet(item_no, true)) {
                        dComIfGs_onSaveSwitch(sw);
                    } else {
                        if (i_posID == 0) {
                            data_80451060 = 1;
                            dShopSystem_itemActor[dShopSystem_item_count] =
                                (daTag_ShopItem_c*)i_actor;
                            dShopSystem_itemNo[dShopSystem_item_count] = item_no;
                        } else if (dShopSystem_itemActor[i_posID - 1] == NULL) {
                            dShopSystem_itemActor[i_posID - 1] = (daTag_ShopItem_c*)i_actor;
                            dShopSystem_itemNo[i_posID - 1] = item_no;
                        }

                        dShopSystem_item_count++;
                    }
                }
            }
        }
    }

    return 0;
}

static fopAc_ac_c* dShopSystem_cameraActor[2] = {NULL, NULL};

static int dShopSystem_camera_count;

static int dShopSystem_searchCameraActor(void* i_actor, void* param_1) {
    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_TAG_SHOPCAM) {
        if ((fopAcM_GetParam(param_1) & 0xf0000000) == (fopAcM_GetParam(i_actor) & 0xf0000000) &&
            dShopSystem_camera_count < 2)
        {
            switch (fopAcM_GetParam(i_actor) & 0xf) {
            case 0:
                if (dShopSystem_cameraActor[0] == NULL) {
                    dShopSystem_cameraActor[0] = (fopAc_ac_c*)i_actor;
                    dShopSystem_camera_count++;
                }
                break;
            case 1:
                if (dShopSystem_cameraActor[1] == NULL) {
                    dShopSystem_cameraActor[1] = (fopAc_ac_c*)i_actor;
                    dShopSystem_camera_count++;
                }
                break;
            default:
                break;
            }
        }
    }
    return 0;
}

void dShopSystem_c::initShopSystem() {
    dShopSystem_sellItemMax = 3;
    data_80451060 = 0;

    for (int i = 0; i < 7; i++) {
        dShopSystem_itemActor[i] = NULL;
        dShopSystem_itemNo[i] = fpcNm_ITEM_NONE;
    }
    dShopSystem_item_count = 0;

    for (int i = 0; i < 2; i++) {
        dShopSystem_cameraActor[i] = NULL;
    }
    dShopSystem_camera_count = 0;

    if (mpStick == NULL) {
        mpStick = new STControl();
    }

    if (mpDrawCursor == NULL) {
        mpDrawCursor = new dSelect_cursor_c(1, 1.0f, NULL);
        mpDrawCursor->setParam(1.0f, 1.0f, 0.1f, 0.7f, 0.7f);
    }

    field_0xf64 = -1;
    mEventParam = 0;
    field_0xf58 = 0;
    mFlag = 0;
    mCursorPos = 0;
    mLastCursorPos = 0;
    field_0xf68 = 0;
    mSeq = 0;
    field_0xf71 = mSeq;
    field_0xf79 = 0;
    field_0xf7a = 0;
    mWaitTimer = 0;
    mSoldOutFlag = -1;
    mSoldOutItemFlags = 0;
    offSpMode();
    field_0xf6c = 0;
    field_0xf76 = 0;
    mMasterType = 9;
    field_0xf78 = -1;
    mFlow.onNonStopJunpFlowFlag();
}

struct shop_item_data {
    Vec mItemPos[3];
};

static shop_item_data item_seira_shop = {
    {
        {110.0f, 150.0f, -115.0f},
        {160.0f, 150.0f, -115.0f},
        {210.0f, 150.0f, -115.0f},
    },
};

static processFunc process[] = {
    &dShopSystem_c::seq_wait,        &dShopSystem_c::seq_start,
    &dShopSystem_c::seq_select_wait, &dShopSystem_c::seq_select_start,
    &dShopSystem_c::seq_select,      &dShopSystem_c::seq_moving,
    &dShopSystem_c::seq_decide,      &dShopSystem_c::seq_choose,
    &dShopSystem_c::seq_decide_yes,  &dShopSystem_c::seq_decide_no,
    &dShopSystem_c::seq_finish,      (processFunc)&dShopSystem_c::seq_event,
};

dShopSystem_c::~dShopSystem_c() {
    deleteObject();
    dShopSystem_sellItemMax = 0;
    data_80451060 = 0;

    for (int i = 0; i < 7; i++) {
        dShopSystem_itemActor[i] = NULL;
        dShopSystem_itemNo[i] = fpcNm_ITEM_NONE;
    }
    dShopSystem_item_count = 0;

    for (int i = 0; i < 2; i++) {
        dShopSystem_cameraActor[i] = NULL;
    }
    dShopSystem_camera_count = 0;
}

void dShopSystem_c::onFlag(int i_flag) {
    mFlag |= (u16)(1 << i_flag);
}

void dShopSystem_c::offFlag(int i_flag) {
    mFlag &= ~(u16)(1 << i_flag);
}

BOOL dShopSystem_c::isFlag(int i_flag) {
    return mFlag & (u16)(1 << i_flag) ? TRUE : FALSE;
}

void dShopSystem_c::onSoldOutItemFlag(int i_flag) {
    mSoldOutItemFlags |= (u8)(1 << i_flag);
}

void dShopSystem_c::offSoldOutItemFlag(int i_flag) {
    mSoldOutItemFlags &= ~(u8)(1 << i_flag);
}

BOOL dShopSystem_c::isSoldOutItemFlag(int i_flag) {
    return mSoldOutItemFlags & (u8)(1 << i_flag) ? TRUE : FALSE;
}

int dShopSystem_c::checkController(u8 i_seq, dMsgFlow_c* i_flow) {
    if (mWaitTimer != 0) {
        if (mDoCPd_c::getTrigA(PAD_1)) {
            dMsgObject_setShopWaitTimer(1);
        }
        return 0;
    }

    if (!dpdMove()) {
        mpStick->checkTrigger();
    }

    int control_status;
    if (dShopSystem_sellItemMax == 4) {
        control_status = chooseItem4(i_seq);
    } else if (dShopSystem_sellItemMax == 5) {
        control_status = chooseItem5(i_seq);
    } else {
        control_status = chooseItem3(i_seq);
    }

    if (control_status != 0) {
        return control_status;
    }

    dMsgObject_c* msg = dMsgObject_getMsgObjectClass();
    if (msg->getSelectPushFlag() != 0) {
        return msg->getSelectPushFlag() == 2 ? 2 : 1;
    }

    if (mDoCPd_c::getTrigA(PAD_1)) {
        if (msg->getStatus() != 6 && msg->getStatus() != 8 && msg->getStatus() != 9 &&
            msg->getStatus() != 20 && i_seq != SEQ_WAIT)
        {
            return 1;
        }
    }

    if (mDoCPd_c::getTrigB(PAD_1)) {
        if (i_seq != SEQ_SELECT_WAIT) {
            mLastCursorPos = mCursorPos;
            mCursorPos = 0;
        }

        if (!i_flow->isSelectMessage()) {
            return 2;
        }
    }

    return 0;
}

int dShopSystem_c::chooseItem3(u8 i_seq) {
    u8 old_cursor = mLastCursorPos;
    if (checkLeftTrigger(mpStick) && i_seq != SEQ_SELECT_WAIT) {
        u8 cursor_pos = mCursorPos;

        if (cursor_pos == 7) {
            mCursorPos = mLastCursorPos;
            mLastCursorPos = 7;

            if (mCursorPos != 0 && isFlag(mCursorPos - 1)) {
                mCursorPos = mLastCursorPos;
                mLastCursorPos = old_cursor;
            } else {
                if (i_seq != SEQ_START) {
                    if (mCursorPos != 0) {
                        mDoAud_seStart(Z2SE_SY_TALK_NEXT, NULL, 0, 0);
                    }

                    mShopCamAction.SetSelectIdx(mCursorPos);
                }

                return 3;
            }
        } else if (cursor_pos == 4) {
            mLastCursorPos = cursor_pos;
            mCursorPos = 0;

            if (i_seq != SEQ_START) {
                if (mCursorPos != 0) {
                    mDoAud_seStart(Z2SE_SY_TALK_NEXT, NULL, 0, 0);
                }

                mShopCamAction.SetSelectIdx(mCursorPos);
            }

            return 3;
        } else if (cursor_pos != 0) {
            mLastCursorPos = cursor_pos;
            mCursorPos--;

            if (mCursorPos != 0 && isFlag(mCursorPos - 1)) {
                mCursorPos = mLastCursorPos;
                mLastCursorPos = old_cursor;
            } else {
                if (i_seq != SEQ_START) {
                    if (mCursorPos != 0) {
                        mDoAud_seStart(Z2SE_SY_TALK_NEXT, NULL, 0, 0);
                    }

                    mShopCamAction.SetSelectIdx(mCursorPos);
                }
                return 3;
            }
        }
    }

    if (checkRightTrigger(mpStick) && i_seq != SEQ_SELECT_WAIT) {
        u8 cursor_pos = mCursorPos;

        if (cursor_pos != 7) {
            if (cursor_pos != 3 && cursor_pos != 6) {
                mLastCursorPos = cursor_pos;
                mCursorPos++;

                if (mCursorPos != 0 && isFlag(mCursorPos - 1)) {
                    mCursorPos = mLastCursorPos;
                    mLastCursorPos = old_cursor;
                } else {
                    if (i_seq != SEQ_START) {
                        mDoAud_seStart(Z2SE_SY_TALK_NEXT, NULL, 0, 0);
                        mShopCamAction.SetSelectIdx(mCursorPos);
                    }
                    return 4;
                }
            } else if (dShopSystem_sellItemMax == 7) {
                mLastCursorPos = cursor_pos;
                mCursorPos = 7;

                if (mCursorPos != 0 && isFlag(mCursorPos - 1)) {
                    mCursorPos = mLastCursorPos;
                    mLastCursorPos = old_cursor;
                } else {
                    if (i_seq != SEQ_START) {
                        mDoAud_seStart(Z2SE_SY_TALK_NEXT, NULL, 0, 0);
                        mShopCamAction.SetSelectIdx(mCursorPos);
                    }
                    return 4;
                }
            }
        }
    }

    if (dShopSystem_sellItemMax >= 6) {
        if (mpStick->checkUpTrigger() && i_seq != SEQ_SELECT_WAIT && mCursorPos <= 3) {
            mLastCursorPos = mCursorPos;
            mCursorPos += 3;

            if (mCursorPos != 0 && isFlag(mCursorPos - 1)) {
                mCursorPos = mLastCursorPos;
                mLastCursorPos = old_cursor;
            } else {
                if (i_seq != SEQ_START) {
                    if (mCursorPos != 0) {
                        mDoAud_seStart(Z2SE_SY_TALK_NEXT, NULL, 0, 0);
                    }

                    mShopCamAction.SetSelectIdx(mCursorPos);
                }

                return 5;
            }
        }

        if (mpStick->checkDownTrigger() && i_seq != SEQ_SELECT_WAIT) {
            u8 cursor_pos = mCursorPos;

            if (cursor_pos > 3 && cursor_pos <= 6) {
                mLastCursorPos = cursor_pos;
                mCursorPos -= 3;

                if (mCursorPos != 0 && isFlag(mCursorPos - 1)) {
                    mCursorPos = mLastCursorPos;
                    mLastCursorPos = old_cursor;
                } else {
                    if (i_seq != SEQ_START) {
                        mDoAud_seStart(Z2SE_SY_TALK_NEXT, NULL, 0, 0);
                        mShopCamAction.SetSelectIdx(mCursorPos);
                    }
                    return 6;
                }
            }
        }
    }

    return 0;
}

int dShopSystem_c::chooseItem5(u8 i_seq) {
    u8 old_cursor = mLastCursorPos;
    if (checkLeftTrigger(mpStick) && i_seq != SEQ_SELECT_WAIT) {
        u8 cursor_pos = mCursorPos;

        if (cursor_pos == 3) {
            mLastCursorPos = cursor_pos;
            mCursorPos = 0;
            if (i_seq != SEQ_START) {
                if (mCursorPos != 0) {
                    mDoAud_seStart(Z2SE_SY_TALK_NEXT, NULL, 0, 0);
                }

                mShopCamAction.SetSelectIdx(mCursorPos);
            }
            return 3;
        }

        if (cursor_pos != 0) {
            mLastCursorPos = cursor_pos;
            mCursorPos--;

            if (mCursorPos != 0 && isFlag(mCursorPos - 1)) {
                mCursorPos = mLastCursorPos;
                mLastCursorPos = old_cursor;
            } else {
                if (i_seq != SEQ_START) {
                    if (mCursorPos != 0) {
                        mDoAud_seStart(Z2SE_SY_TALK_NEXT, NULL, 0, 0);
                    }

                    mShopCamAction.SetSelectIdx(mCursorPos);
                }
                return 3;
            }
        }
    }

    if (checkRightTrigger(mpStick) && i_seq != SEQ_SELECT_WAIT) {
        u8 cursor_pos = mCursorPos;

        if (cursor_pos != 2 && cursor_pos != 5) {
            mLastCursorPos = cursor_pos;
            mCursorPos++;

            if (mCursorPos != 0 && isFlag(mCursorPos - 1)) {
                mCursorPos = mLastCursorPos;
                mLastCursorPos = old_cursor;
            } else {
                if (i_seq != SEQ_START) {
                    mDoAud_seStart(Z2SE_SY_TALK_NEXT, NULL, 0, 0);
                    mShopCamAction.SetSelectIdx(mCursorPos);
                }
                return 4;
            }
        }
    }

    if (mpStick->checkUpTrigger() && i_seq != SEQ_SELECT_WAIT) {
        u8 cursor_pos = mCursorPos;

        if (cursor_pos == 3 || cursor_pos == 5) {
            mLastCursorPos = cursor_pos;

            if (mCursorPos == 3) {
                mCursorPos = 1;
            } else if (mCursorPos == 5) {
                mCursorPos = 2;
            }

            if (mCursorPos != 0 && isFlag(mCursorPos - 1)) {
                mCursorPos = mLastCursorPos;
                mLastCursorPos = old_cursor;
            } else {
                if (i_seq != SEQ_START) {
                    if (mCursorPos != 0) {
                        mDoAud_seStart(Z2SE_SY_TALK_NEXT, NULL, 0, 0);
                    }

                    mShopCamAction.SetSelectIdx(mCursorPos);
                }
                return 5;
            }
        }
    }

    if (mpStick->checkDownTrigger() && i_seq != SEQ_SELECT_WAIT) {
        u8 cursor_pos = mCursorPos;

        if (cursor_pos == 1 || cursor_pos == 2) {
            mLastCursorPos = cursor_pos;

            if (mCursorPos == 1) {
                mCursorPos = 3;
            } else if (mCursorPos == 2) {
                mCursorPos = 5;
            }

            if (mCursorPos != 0 && isFlag(mCursorPos - 1)) {
                mCursorPos = mLastCursorPos;
                mLastCursorPos = old_cursor;
            } else {
                if (i_seq != SEQ_START) {
                    mDoAud_seStart(Z2SE_SY_TALK_NEXT, NULL, 0, 0);
                    mShopCamAction.SetSelectIdx(mCursorPos);
                }
                return 6;
            }
        }
    }

    return 0;
}

int dShopSystem_c::chooseItem4(u8 i_seq) {
    u8 old_cursor = mLastCursorPos;
    if (checkLeftTrigger(mpStick) && i_seq != SEQ_SELECT_WAIT && mCursorPos != 0) {
        mLastCursorPos = mCursorPos;
        mCursorPos--;

        if (mCursorPos != 0 && isFlag(mCursorPos - 1)) {
            mCursorPos = mLastCursorPos;
            mLastCursorPos = old_cursor;
        } else {
            if (i_seq != SEQ_START) {
                if (mCursorPos != 0) {
                    mDoAud_seStart(Z2SE_SY_TALK_NEXT, NULL, 0, 0);
                }

                mShopCamAction.SetSelectIdx(mCursorPos);
            }

            return 3;
        }
    }

    if (checkRightTrigger(mpStick) && i_seq != SEQ_SELECT_WAIT && mCursorPos < 4) {
        mLastCursorPos = mCursorPos;
        mCursorPos++;

        if (mCursorPos != 0 && isFlag(mCursorPos - 1)) {
            mCursorPos = mLastCursorPos;
            mLastCursorPos = old_cursor;
        } else {
            if (i_seq != SEQ_START) {
                mDoAud_seStart(Z2SE_SY_TALK_NEXT, NULL, 0, 0);
                mShopCamAction.SetSelectIdx(mCursorPos);
            }

            return 4;
        }
    }

    return 0;
}

int dShopSystem_c::moveCursor(int control_status, u8 param_1) {
    int cursor_status;
    if (field_0xf60 >= 0) {
        cursor_status = moveCursor0(control_status, param_1);
    } else {
        cursor_status = moveCursor1(control_status, param_1);
    }

    field_0xf6c = cursor_status;
    return cursor_status;
}

int dShopSystem_c::moveCursor0(int control_status, u8 param_1) {
    offFlag(7);

    switch (control_status) {
    case 3:
        if (mCursorPos == 0) {
            return getEventParamU8(mCursorPos) + 0x52;
        }

        return (getEventParamU8(mCursorPos) - 1) * 2 + 0x66;

    case 4:
        if (mCursorPos == 0) {
            return getEventParamU8(mCursorPos) + 0x52;
        }

        return (getEventParamU8(mCursorPos) - 1) * 2 + 0x66;

    case 7:
        if (mCursorPos == 0) {
            return getEventParamU8(mCursorPos) + 0x52;
        }

        return (getEventParamU8(mCursorPos) - 1) * 2 + 0x66;

    case 1:
        if (mCursorPos == 0) {
            return getEventParamU8(mCursorPos) + 0x52;
        }

        if (param_1 != 2) {
            return (getEventParamU8(mCursorPos) - 1) * 2 + 0x65;
        }

        return (getEventParamU8(mCursorPos) - 1) * 2 + 0x66;

    case 2:
        onFlag(7);
        mShopCamAction.SetSelectIdx(mCursorPos);

        if (mCursorPos == 0) {
            return getEventParamU8(mCursorPos) + 0x52;
        }

        if (param_1 != 2) {
            return (getEventParamU8(mCursorPos) - 1) * 2 + 0x65;
        }

        return (getEventParamU8(mCursorPos) - 1) * 2 + 0x66;
    }

    return 0;
}

int dShopSystem_c::moveCursor1(int control_status, u8 param_1) {
    offFlag(7);

    switch (control_status) {
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
        if (mCursorPos == 0) {
            return getFlowNodeNum() + 1;
        }

        return mItemCtrl.getMessageIndex(mCursorPos - 1) + 1;

    case 1:
        if (mCursorPos == 0) {
            return getFlowNodeNum() + 1;
        } else if (param_1 != 2) {
            return mItemCtrl.getMessageIndex(mCursorPos - 1);
        }

        return mItemCtrl.getMessageIndex(mCursorPos - 1) + 1;

    case 2:
        onFlag(7);
        mShopCamAction.SetSelectIdx(mCursorPos);

        if (mCursorPos == 0) {
            return getFlowNodeNum() + 1;
        } else if (param_1 != 2) {
            return mItemCtrl.getMessageIndex(mCursorPos - 1);
        }

        return mItemCtrl.getMessageIndex(mCursorPos - 1) + 1;

    default:
        return 0;
    }
}

int dShopSystem_c::drawCursor() {
    if ((mSeq == 4 || mSeq == 5 ||
         ((mSeq == 6 || mSeq == SEQ_SELECT_WAIT) && isFlag(mCursorPos - 1))) &&
        mCursorPos != 0 && mItemCtrl.isHomePos(mCursorPos - 1))
    {
        mpDrawCursor->addAlpha();
    } else {
        mpDrawCursor->decAlpha();
    }

    mpDrawCursor->setScale(g_cursorHIO.mShopCursorScale);
    if (mCursorPos != 0) {
        dComIfGd_set2DOpa(mpDrawCursor);
    }

    return 1;
}

int dShopSystem_c::itemRotate() {
    u8 item_index = 0;
    u8 cursor_pos = mCursorPos;

    if (cursor_pos != 0 && mSeq != SEQ_WAIT) {
        if (dShopSystem_itemNo[cursor_pos - 1] == fpcNm_ITEM_ARMOR) {
            int tmp_index;
            if (isFlag(8) == false) {
                tmp_index = 0;
            } else {
                tmp_index = mCursorPos;
            }
            item_index = tmp_index;
        } else {
            bool no_rotate = false;
            int tmp_index;
            if (isFlag(cursor_pos - 1) || isSoldOutItemFlag(mCursorPos - 1)) {
                no_rotate = true;
            }

            if (no_rotate) {
                tmp_index = 0;
            } else {
                tmp_index = mCursorPos;
            }
            item_index = tmp_index;
        }
    }

    mItemCtrl.setRotateAnime(item_index);
    return 1;
}

int dShopSystem_c::itemZoom(cXyz* param_0) {
    cXyz local_1c;

    if (field_0xf60 >= 0) {
        local_1c.set(*param_0);

        if (dShopSystem_itemNo[mCursorPos - 1] == fpcNm_ITEM_OIL_BOTTLE) {
            mItemCtrl.setZoomAnime(mCursorPos, &local_1c,
                                   g_cursorHIO.mSeraShopObjZoomAngleX + -5000,
                                   isFlag(8) ? true : false);
        } else {
            mItemCtrl.setZoomAnime(mCursorPos, &local_1c, g_cursorHIO.mSeraShopObjZoomAngleX,
                                   isFlag(8) ? true : false);
        }
    } else {
        cXyz local_28;
        cXyz local_34;

        if (dShopSystem_itemActor[1] != NULL && dShopSystem_cameraActor[1] != NULL) {
            cXyz local_40;
            local_40.set(dShopSystem_itemActor[1]->home.pos);

            f32 tmp_05 = 0.5f;
            if (dShopSystem_sellItemMax == 4) {
                if (dShopSystem_itemActor[2] != NULL) {
                    f32 tmp = (dShopSystem_itemActor[2]->home.pos.x +
                               dShopSystem_itemActor[1]->home.pos.x);
                    local_40.x = tmp * tmp_05;

                    f32 tmp2 = (dShopSystem_itemActor[2]->home.pos.y +
                                dShopSystem_itemActor[1]->home.pos.y);
                    local_40.y = tmp2 * tmp_05;

                    f32 tmp3 = (dShopSystem_itemActor[2]->home.pos.z +
                                dShopSystem_itemActor[1]->home.pos.z);
                    local_40.z = tmp3 * tmp_05;
                }
            } else if (dShopSystem_sellItemMax == 5) {
                f32 tmp =
                    (dShopSystem_itemActor[2]->home.pos.x + dShopSystem_itemActor[3]->home.pos.x);
                local_40.x = tmp * tmp_05;

                local_40.y = dShopSystem_itemActor[0]->home.pos.y;

                f32 tmp3 =
                    (dShopSystem_itemActor[2]->home.pos.z + dShopSystem_itemActor[3]->home.pos.z);
                local_40.z = tmp3 * tmp_05;
            }

            cXyz local_4c = local_40 - dShopSystem_cameraActor[1]->current.pos;
            s16 atan = cM_atan2s(local_4c.x, local_4c.z);

            if (!strcmp("R_SP01", dComIfGp_getStartStageName())) {
                local_34.set(g_cursorHIO.mObjZoom.x, g_cursorHIO.mObjZoom.y + -25.0f,
                             g_cursorHIO.mObjZoom.z + 150.0f);
            } else {
                if (mMasterType == 5) {
                    if (dShopSystem_itemNo[mCursorPos - 1] == fpcNm_ITEM_ARROW_10 ||
                        dShopSystem_itemNo[mCursorPos - 1] == fpcNm_ITEM_ARROW_20 ||
                        dShopSystem_itemNo[mCursorPos - 1] == fpcNm_ITEM_ARROW_30)
                    {
                        local_34.set(g_cursorHIO.mObjZoom.x, -50.0f + g_cursorHIO.mObjZoom.y + 5.0f,
                                     (g_cursorHIO.mObjZoom.z + 250.0f) - 60.0f);
                    } else {
                        local_34.set(g_cursorHIO.mObjZoom.x,
                                     -50.0f + g_cursorHIO.mObjZoom.y + 20.0f,
                                     (g_cursorHIO.mObjZoom.z + 250.0f) - 60.0f);
                    }
                } else if (mCursorPos == 7) {
                    local_34.set(g_cursorHIO.mMagicArmorObjZoom.x,
                                 g_cursorHIO.mMagicArmorObjZoom.y + -130.0f,
                                 g_cursorHIO.mMagicArmorObjZoom.z + 215.0f);
                } else {
                    local_34.set(g_cursorHIO.mObjZoom.x, g_cursorHIO.mObjZoom.y + -50.0f,
                                 g_cursorHIO.mObjZoom.z + 250.0f);
                }
            }

            cLib_offsetPos(&local_28, &dShopSystem_cameraActor[1]->current.pos, atan, &local_34);
            if (dShopSystem_sellItemMax == 6 || dShopSystem_sellItemMax == 5) {
                param_0->y += 15.0f;
            }
            local_1c.set(local_28.x + param_0->x, local_28.y + param_0->y, local_28.z + param_0->z);
        } else {
            local_28.set(*param_0);
            local_1c.set(*param_0);
        }

        u8 dvar1 = mMasterType;
        if (dvar1 == 1) {
            if (dShopSystem_itemNo[mCursorPos - 1] == fpcNm_ITEM_OIL_BOTTLE) {
                mItemCtrl.setZoomAnime(mCursorPos, &local_1c,
                                       g_cursorHIO.mShopObjZoomAngleX + -7000,
                                       isFlag(8) ? true : false);
            } else {
                mItemCtrl.setZoomAnime(mCursorPos, &local_1c, g_cursorHIO.mShopObjZoomAngleX,
                                       isFlag(8) ? true : false);
            }
        } else if (dvar1 == 2) {
            if (dShopSystem_itemNo[mCursorPos - 1] == fpcNm_ITEM_RED_BOTTLE) {
                mItemCtrl.setZoomAnime(mCursorPos, &local_1c, g_cursorHIO.mShopObjZoomAngleX - 3000,
                                       isFlag(8) ? true : false);
            } else {
                mItemCtrl.setZoomAnime(mCursorPos, &local_1c, g_cursorHIO.mShopObjZoomAngleX,
                                       isFlag(8) ? true : false);
            }
        } else if (dvar1 == 5) {
            if (dShopSystem_itemNo[mCursorPos - 1] == fpcNm_ITEM_ARROW_10 ||
                dShopSystem_itemNo[mCursorPos - 1] == fpcNm_ITEM_ARROW_20 ||
                dShopSystem_itemNo[mCursorPos - 1] == fpcNm_ITEM_ARROW_30)
            {
                mItemCtrl.setZoomAnime(mCursorPos, &local_1c, g_cursorHIO.mShopObjZoomAngleX - 4000,
                                       isFlag(8) ? true : false);
            } else {
                mItemCtrl.setZoomAnime(mCursorPos, &local_1c, g_cursorHIO.mShopObjZoomAngleX - 2000,
                                       isFlag(8) ? true : false);
            }
        } else if (mCursorPos == 7) {
            mItemCtrl.setZoomAnime(mCursorPos, &local_1c, g_cursorHIO.mMagicArmorObjZoomAngleX,
                                   isFlag(8) ? true : false);
        } else if (dShopSystem_sellItemMax == 7) {
            mItemCtrl.setZoomAnime(mCursorPos, &local_1c, g_cursorHIO.mShopObjZoomAngleX - 3000,
                                   isFlag(8) ? true : false);
        } else {
            mItemCtrl.setZoomAnime(mCursorPos, &local_1c, g_cursorHIO.mShopObjZoomAngleX,
                                   isFlag(8) ? true : false);
        }
    }

    return 1;
}

int dShopSystem_c::seq_wait(fopAc_ac_c* param_0, dMsgFlow_c* param_1) {
    return 0;
}

inline void pos3Dto2D(Vec* a, Vec* b) {
    mDoLib_project(a, b);
}

int dShopSystem_c::seq_start(fopAc_ac_c* actor, dMsgFlow_c* i_flow) {
    if (field_0xf79 != 0) {
        dComIfGp_setDoStatusForce(0, 0);
        dComIfGp_setAStatusForce(0, 0);
        field_0xf79 = 0;
    }

    cXyz pos3d;
    cXyz pos2d;
    pos3d.set(mItemCtrl.getCurrentPos(0));
    pos3Dto2D(&pos3d, &pos2d);
    mpDrawCursor->setPos(pos2d.x, pos2d.y + g_cursorHIO.mShopCursorOffsetY);

    if (chkSpMode() && !beforeStartSeqAction(i_flow, field_0xf5c)) {
        return 0;
    } else {
        if (dMsgObject_c::isMsgSendControl()) {
            dComIfGp_setDoStatusForce(0, 0);
            dComIfGp_setAStatusForce(0x2A, 0);

            int control_status = checkController(SEQ_WAIT, i_flow);
            if (control_status != 0) {
                dMsgObject_c::onMsgSend();
                int cursor_status = moveCursor(control_status, 0);

                if (chkSpMode()) {
                    if (mCursorPos != 0) {
                        field_0xf5c = cursor_status;
                        mShopCamAction.setCameraSpeed(0.0f, 0.0f, 0.0f, 0.0f);
                        setSeq(SEQ_SELECT);
                    } else if (i_flow->doFlow(actor, NULL, cursor_status)) {
                        mShopCamAction.setCameraSpeed(0.0f, 0.0f, 0.0f, 0.0f);
                        setSeq(SEQ_FINISH);
                    }
                } else if (i_flow->doFlow(actor, NULL, cursor_status)) {
                    if (mCursorPos == 0) {
                        setSeq(SEQ_FINISH);
                    } else {
                        setSeq(SEQ_SELECT);
                    }
                }
            }
        } else {
            if (i_flow->doFlow(actor, NULL, 0)) {
                int itemNo;
                if (mFlow.getEventId(&itemNo) == 1) {
                    if (mItemPartnerId == fpcM_ERROR_PROCESS_ID_e) {
                        mItemPartnerId = fopAcM_createItemForPresentDemo(&current.pos, itemNo, 0, -1,
                                                                      -1, NULL, NULL);
                    }

                    if (fpcEx_IsExist(mItemPartnerId)) {
                        mEvtNo = 1;
                        evtChange();
                        return 1;
                    } else {
                        return 0;
                    }
                } else {
                    return 1;
                }
            }
        }
    }

    return 0;
}

int dShopSystem_c::seq_select_wait(fopAc_ac_c*, dMsgFlow_c*) {
    if (mItemCtrl.isHomePos(mCursorPos - 1)) {
        setSoldOut();
        setSeq(SEQ_SELECT);
    }

    return 0;
}

int dShopSystem_c::seq_select_start(fopAc_ac_c*, dMsgFlow_c*) {
    if (field_0xf60 < 0 && field_0xf76 == 0 && dShopSystem_item_count != -1) {
        if (dShopSystem_item_count != dShopSystem_sellItemMax) {
            return 0;
        }
        searchItemActor();
    }

    if (isFlag(mCursorPos - 1)) {
        switch (mCursorPos) {
        case 1:
            mCursorPos = 2;
            if (isFlag(mCursorPos - 1)) {
                mCursorPos = 3;
                if (isFlag(mCursorPos - 1)) {
                    mCursorPos = 0;
                }
            }
            break;

        case 2:
            mCursorPos = 1;
            if (isFlag(mCursorPos - 1)) {
                mCursorPos = 3;
                if (isFlag(mCursorPos - 1)) {
                    mCursorPos = 0;
                }
            }
            break;

        case 3:
            mCursorPos = 2;
            if (isFlag(mCursorPos - 1)) {
                mCursorPos = 1;
                if (isFlag(mCursorPos - 1)) {
                    mCursorPos = 0;
                }
            }
            break;
        }
    }

    mShopCamAction.SetSelectIdx(mCursorPos);
    int cursor_status = moveCursor(4, 0);
    initTalk(cursor_status, NULL);
    setSeq(SEQ_SELECT);

    return 0;
}

int dShopSystem_c::seq_select(fopAc_ac_c* actor, dMsgFlow_c* i_flow) {
    if (chkSpMode() && !beforeSelectSeqAction(i_flow, field_0xf5c)) {
        return 0;
    }

    if (field_0xf76 != 0) {
        field_0xf76 = 0;
    }

    int control_status = checkController(SEQ_START, i_flow);
    dComIfGp_setDoStatusForce(0x22, 0);
    dComIfGp_setAStatusForce(0x2A, 0);

    if (control_status != 0) {
        int cursor_status = moveCursor(control_status, 1);

        int spMode = chkSpMode();
        if (spMode && control_status == 2) {
            field_0xf5c = cursor_status;
            mShopCamAction.setCameraSpeed(0.0f, 0.0f, 0.0f, 0.0f);
            setSeq(SEQ_FINISH);
        } else if (spMode && mCursorPos == 0) {
            field_0xf5c = cursor_status;
            mShopCamAction.SetSelectIdx(mCursorPos);
            mShopCamAction.setCameraSpeed(0.0f, 0.0f, 0.0f, 0.0f);
            setSeq(SEQ_START);
        } else if (i_flow->doFlow(actor, NULL, cursor_status)) {
            if (control_status == 3) {
                if (mCursorPos != 0) {
                    mDoAud_seStart(Z2SE_SY_TALK_NEXT, NULL, 0, 0);
                }

                mShopCamAction.SetSelectIdx(mCursorPos);
            } else if (control_status == 4 || control_status == 5 || control_status == 6) {
                mDoAud_seStart(Z2SE_SY_TALK_NEXT, NULL, 0, 0);

                mShopCamAction.SetSelectIdx(mCursorPos);
            }

            dComIfGp_setMesgCancelButton(1);

            if (control_status == 1) {
                if (!isFlag(mCursorPos - 1) && !isSoldOutItemFlag(mCursorPos - 1)) {
                    onFlag(8);
                }
                dMeter2Info_set2DVibrationM();
                setSeq(SEQ_DECIDE);
            } else if (control_status == 2) {
                setSeq(SEQ_FINISH);
            } else if (mCursorPos == 0) {
                setSeq(SEQ_START);
            } else {
                field_0xf68 = 0;
                setSeq(SEQ_MOVING);

                if (mLastCursorPos != 0) {
                    return 0;
                }
            }
        } else if (control_status == 3 || control_status == 4 || control_status == 5 ||
                   control_status == 6 || control_status == 7 || control_status == 2)
        {
            if (mLastCursorPos == 7) {
                mLastCursorPos = mCursorPos;
                mCursorPos = 7;
            } else {
                mCursorPos = mLastCursorPos;
            }

            field_0xf7a = 0;

            if (control_status == 2) {
                mShopCamAction.SetSelectIdx(mCursorPos);
            }
        }
    } else {
        i_flow->doFlow(actor, NULL, 0);
    }

    u8 old_cursor = 0;
    if (mCursorPos != 0) {
        old_cursor = mCursorPos;
    } else if (mLastCursorPos != 0) {
        old_cursor = mLastCursorPos;
    }

    if (old_cursor != 0) {
        cXyz pos3d;
        cXyz pos2d;
        pos3d.set(mItemCtrl.getCurrentPos(old_cursor - 1));
        pos3Dto2D(&pos3d, &pos2d);

        if (old_cursor == 7) {
            pos2d.x += g_cursorHIO.mMagicArmorCursorOffsetX;
            pos2d.y += g_cursorHIO.mMagicArmorCursorOffsetY;
        } else {
            pos2d.y += g_cursorHIO.mShopCursorOffsetY;
        }

        mpDrawCursor->setPos(pos2d.x, pos2d.y);
    }

    return 0;
}

int dShopSystem_c::seq_moving(fopAc_ac_c*, dMsgFlow_c*) {
    field_0xf68++;

    cXyz last_pos3d;
    cXyz pos3d;
    cXyz last_pos2d;
    cXyz pos2d;

    pos3d.set(mItemCtrl.getCurrentPos(mCursorPos - 1));
    pos3Dto2D(&pos3d, &pos2d);

    if (mCursorPos == 7) {
        pos2d.x += g_cursorHIO.mMagicArmorCursorOffsetX;
        pos2d.y += g_cursorHIO.mMagicArmorCursorOffsetY;
    } else {
        pos2d.y += g_cursorHIO.mShopCursorOffsetY;
    }

    if (mLastCursorPos != 0) {
        last_pos3d.set(mItemCtrl.getCurrentPos(mLastCursorPos - 1));
        pos3Dto2D(&last_pos3d, &last_pos2d);

        if (mLastCursorPos == 7) {
            last_pos2d.x += g_cursorHIO.mMagicArmorCursorOffsetX;
            last_pos2d.y += g_cursorHIO.mMagicArmorCursorOffsetY;
        } else {
            last_pos2d.y += g_cursorHIO.mShopCursorOffsetY;
        }

        f32 tmp = (f32)(field_0xf68 * field_0xf68) / 9.0f;
        mpDrawCursor->setPos(last_pos2d.x + tmp * (pos2d.x - last_pos2d.x),
                             last_pos2d.y + tmp * (pos2d.y - last_pos2d.y));

    } else {
        mpDrawCursor->setPos(pos2d.x, pos2d.y);
    }

    if (field_0xf68 >= 3) {
        setSeq(SEQ_SELECT);
    }

    return 0;
}

int dShopSystem_c::seq_decide(fopAc_ac_c* actor, dMsgFlow_c* i_flow) {
    int control_status = checkController(SEQ_SELECT_WAIT, i_flow);

    dComIfGp_setDoStatusForce(0x22, 0);
    if (!i_flow->isSelectMessage()) {
        dComIfGp_setAStatusForce(0x2A, 0);
    } else if (dMsgObject_getMsgObjectClass()->getSelectCancelPos() != 0) {
        dComIfGp_setAStatusForce(0x2A, 0);
    }

    if (control_status != 0) {
        int cursor_status = moveCursor(control_status, 2);

        if (i_flow->doFlow(actor, NULL, cursor_status)) {
            offFlag(8);
            setSeq(SEQ_SELECT_WAIT);
        } else if (i_flow->isSelectMessage()) {
            field_0xf58 = control_status;
            setSeq(SEQ_CHOOSE);
        }
    } else {
        i_flow->doFlow(actor, NULL, 0);
    }

    return 0;
}

int dShopSystem_c::seq_choose(fopAc_ac_c* actor, dMsgFlow_c* i_flow) {
    dComIfGp_setDoStatusForce(0x22, 0);
    if (!i_flow->isSelectMessage()) {
        dComIfGp_setAStatusForce(0x2A, 0);
    } else if (dMsgObject_getMsgObjectClass()->getSelectCancelPos() != 0) {
        dComIfGp_setAStatusForce(0x2A, 0);
    }

    i_flow->doFlow(actor, NULL, 0);

    u16 select_num = i_flow->getSelectNum();
    if (dMsgObject_c::getStatus() != 6 && dMsgObject_c::getStatus() != 8 &&
        dMsgObject_c::getStatus() != 9 && dMsgObject_c::getStatus() != 20)
    {
        if (select_num == 0) {
            setSeq(SEQ_DECIDE_YES);
        } else {
            setSeq(SEQ_DECIDE_NO);
        }
    }

    return 0;
}

int dShopSystem_c::seq_decide_yes(fopAc_ac_c* actor, dMsgFlow_c* i_flow) {
    if (dMsgObject_getMessageID() == 0x1B82) {
        mShopCamAction.SetSelectIdx(0);
        field_0xf76 = 1;
    }

    int itemNo;
    if (mFlow.getEventId(&itemNo) == 1) {
        if (i_flow->doFlow(actor, NULL, 0)) {
            if (mItemPartnerId == fpcM_ERROR_PROCESS_ID_e) {
                mItemPartnerId =
                    fopAcM_createItemForPresentDemo(&current.pos, itemNo, 0, -1, -1, NULL, NULL);
            }

            if (fpcEx_IsExist(mItemPartnerId)) {
                offFlag(8);
                setSoldOutItemHide();
                mEvtNo = 1;
                evtChange();
                return 2;
            }

            return 0;
        }
    } else {
        int flow;
        if (field_0xf60 >= 0) {
            flow = 100;
        } else {
            flow = mItemCtrl.getMessageIndex(mCursorPos - 1) + 1;
        }

        if (i_flow->doFlow(actor, NULL, flow)) {
            offFlag(8);
            setSeq(SEQ_SELECT_WAIT);
        }
    }

    return 0;
}

int dShopSystem_c::seq_decide_no(fopAc_ac_c* actor, dMsgFlow_c* i_flow) {
    if (i_flow->doFlow(actor, NULL, moveCursor(field_0xf58, 2))) {
        offFlag(8);
        setSeq(SEQ_SELECT_WAIT);
    }

    return 0;
}

int dShopSystem_c::seq_finish(fopAc_ac_c* actor, dMsgFlow_c* i_flow) {
    if (chkSpMode() && !beforeStartSeqAction(i_flow, field_0xf5c)) {
        return 0;
    } else if (i_flow->doFlow(actor, NULL, 0)) {
        setSeq(SEQ_WAIT);
        return 1;
    }

    return 0;
}

bool dShopSystem_c::seq_event(fopAc_ac_c*, dMsgFlow_c*) {
    return mEvtNo == 0;
}

int dShopSystem_c::shop_init(bool param_0) {
    if (param_0) {
        setSeq(SEQ_SELECT_START);
        dMeter2Info_onShopTalkFlag();
        setSoldOut();
        return 1;
    } else if (mSeq == SEQ_WAIT) {
        setSeq(SEQ_START);
        dMeter2Info_onShopTalkFlag();
        return 1;
    } else {
        return 0;
    }
}

int dShopSystem_c::shop_process(fopAc_ac_c* param_0, dMsgFlow_c* param_1) {
    int iVar1;
    iVar1 = (this->*process[mSeq])(param_0, param_1);

    if (mWaitTimer != 0) {
        mWaitTimer--;
    }
    if (field_0xf7a != 0) {
        dMeter2Info_set2DVibration();
        field_0xf7a = 0;
    }
    if (iVar1 != 0) {
        g_meter2_info.offShopTalkFlag();
        if (mSeq != 0) {
            setSeq(0);
        }
        if (iVar1 != 2) {
            mCursorPos = 0;
            mLastCursorPos = 0;
        }
    }
    if (mSeq != field_0xf71) {
        field_0xf71 = mSeq;
        field_0xf79 = 1;
    }
    return iVar1;
}

static shop_item_data* shop_item_pos_data_tbl = &item_seira_shop;

void dShopSystem_c::createShopItem(int itemType) {
    static const u8 item_no[6] = {
        fpcNm_ITEM_MILK_BOTTLE, fpcNm_ITEM_BEE_CHILD, fpcNm_ITEM_PACHINKO, fpcNm_ITEM_MILK_BOTTLE, fpcNm_ITEM_BEE_CHILD, fpcNm_ITEM_NONE,
    };

    if (itemType != -1) {
        field_0xf60 = itemType;
        mSoldOutFlag = -1;

        cXyz pos;
        for (int i = 0; i < 3; i++) {
            const u8* itemTbl = item_no + (itemType * 3);

            pos.x = shop_item_pos_data_tbl->mItemPos[i].x + current.pos.x;
            pos.y = shop_item_pos_data_tbl->mItemPos[i].y + current.pos.y;
            pos.z = shop_item_pos_data_tbl->mItemPos[i].z + current.pos.z;

            if (itemTbl[i] != fpcNm_ITEM_NONE) {
                u32 index = fopAcM_create(PROC_ShopItem, itemTbl[i], &pos, fopAcM_GetRoomNo(this),
                                          &current.angle, NULL, -1);
                mItemCtrl.setItemIndex(i, index);
                offFlag(i);
            } else {
                mItemCtrl.setItemIndex(i, -1);
                onFlag(i);
            }
        }
    }
}

void dShopSystem_c::setSeq(u8 i_seq) {
    mSeq = i_seq;
    mWaitTimer = 2;
}

void dShopSystem_c::setSoldOutFlag() {
    mSoldOutFlag = mCursorPos - 1;
}

void dShopSystem_c::setSoldOut() {
    if (mSoldOutFlag != 0xFF) {
        if (field_0xf60 < 0) {
            u8 sw = (u8)dShopSystem_itemActor[mSoldOutFlag]->home.angle.z & 0xFF;
            if (sw != 0xFF && !dComIfGs_isSaveSwitch(sw)) {
                dComIfGs_onSaveSwitch(sw);
            }
        }

        int index = mSoldOutFlag;
        mSoldOutFlag = -1;
        if (field_0xf60 >= 0) {
            fopAcM_delete(mItemCtrl.getItemIndex(index));
            mItemCtrl.setItemIndex(index, -1);
            onFlag(index);
            return;
        } else if (index == 0) {
            dShopSystem_itemActor[0] = NULL;
            dShopSystem_itemNo[0] = fpcNm_ITEM_NONE;
            dShopSystem_item_count = 2;
        } else if (index == 1) {
            dShopSystem_itemActor[1] = NULL;
            dShopSystem_itemNo[1] = fpcNm_ITEM_NONE;
            dShopSystem_item_count = 2;
        } else if (index == 2) {
            dShopSystem_itemActor[2] = NULL;
            dShopSystem_itemNo[2] = fpcNm_ITEM_NONE;
            dShopSystem_item_count = 2;
        } else if (index == 6) {
            dShopSystem_itemActor[6] = NULL;
            dShopSystem_itemNo[6] = fpcNm_ITEM_NONE;
            dShopSystem_item_count = 6;
        }

        if (field_0xf76 != 0) {
            onFlag(index);
        }
    }
}

void dShopSystem_c::setSoldOutItemHide() {
    if (field_0xf60 < 0) {
        fopAcM_delete(mItemCtrl.getItemIndex(mSoldOutFlag));
    } else {
        daShopItem_c* item = (daShopItem_c*)fopAcM_SearchByID(mItemCtrl.getItemIndex(mSoldOutFlag));

        if (item != NULL) {
            item->hide();
        }
    }
}

void dShopSystem_c::deleteObject() {
    if (mpStick != NULL) {
        delete mpStick;
        mpStick = NULL;
    }

    if (mpDrawCursor != NULL) {
        delete mpDrawCursor;
        mpDrawCursor = NULL;
    }
}

bool dShopSystem_c::searchItemActor() {
    u8 temp_data;
    if ((dShopSystem_item_count != -1 &&
         ((dShopSystem_item_count < (temp_data = dShopSystem_sellItemMax)) ||
          dShopSystem_camera_count < 2)))
    {
        if (dShopSystem_item_count < temp_data) {
            fpcEx_Search((fpcLyIt_JudgeFunc)dShopSystem_searchItemActor, this);
        }
        if (dShopSystem_camera_count < 2) {
            fpcEx_Search((fpcLyIt_JudgeFunc)dShopSystem_searchCameraActor, this);
        }
        return 0;
    }

    if (dShopSystem_item_count >= dShopSystem_sellItemMax && dShopSystem_camera_count >= 2) {
        mSoldOutFlag = -1;

        if (data_80451060) {
            f32 actorDistArr[7];
            u8 itemNoArr[7];
            f32 arr2[7];
            daTag_ShopItem_c* actorArr[7];
            u8 arr3[7];

            for (int i = 0; i < dShopSystem_sellItemMax; i++) {
                actorArr[i] = dShopSystem_itemActor[i];
                itemNoArr[i] = dShopSystem_itemNo[i];
                actorDistArr[i] = fopAcM_searchActorDistance(this, actorArr[i]);
                arr2[i] = 0.0f;
                arr3[i] = 0;
            }

            for (int i = 0; i < dShopSystem_sellItemMax; i++) {
                arr2[i] = actorDistArr[i];
                arr3[i] = i;

                if (i > 0) {
                    for (int j = i; j >= 0; j--) {
                        if (arr2[j] < arr2[j - 1]) {
                            f32 temp = arr2[j];
                            arr2[j] = arr2[j - 1];
                            arr2[j - 1] = temp;

                            u8 temp2 = arr3[j];
                            arr3[j] = arr3[j - 1];
                            arr3[j - 1] = temp2;
                        }
                    }
                }
            }

            for (int i = 0; i < dShopSystem_sellItemMax; i++) {
                dShopSystem_itemActor[i] = actorArr[arr3[i]];
                dShopSystem_itemNo[i] = itemNoArr[arr3[i]];
            }
        }

        for (int i = 0; i < dShopSystem_sellItemMax; i++) {
            u32 processId = dShopSystem_itemActor[i]->getProcessID();
            mItemCtrl.setItemIndex(i, processId);
            mItemCtrl.setMessageIndex(i, (u16)dShopSystem_itemActor[i]->home.angle.x != 0xFFFF ?
                                             (u16)dShopSystem_itemActor[i]->home.angle.x :
                                             -1);

            if (processId + 0x10000 == 0xFFFF) {
                onFlag(i);
                onSoldOutItemFlag(i);
            } else {
                offFlag(i);

                if ((u8)fopAcM_GetParam(dShopSystem_itemActor[i]) == 0xFF) {
                    onSoldOutItemFlag(i);
                } else {
                    offSoldOutItemFlag(i);
                }
            }
        }

        if (mMasterType == 8) {
            dShopSystem_cameraActor[0]->current.pos.set(-175.0f, 165.0f, 10.0f);
            dShopSystem_cameraActor[1]->current.pos.set(215.0f, 225.0f, 370.0f);
        }

        if (dShopSystem_sellItemMax > 3) {
            mShopCamAction.setCamDataIdx2(
                this, dShopSystem_itemActor[0], dShopSystem_itemActor[1], dShopSystem_itemActor[2],
                dShopSystem_itemActor[3], dShopSystem_itemActor[4], dShopSystem_itemActor[5],
                &dShopSystem_cameraActor[0]->current.pos, &dShopSystem_cameraActor[1]->current.pos);

            if (mMasterType == 8) {
                cXyz camCtrPos;
                camCtrPos.set(-180.0f, 245.0f, -380.0f);
                mShopCamAction.setMasterCamCtrPos(&camCtrPos);
            }

            for (int i = 0; i < 7; i++) {
                if (dShopSystem_itemActor[i] != NULL) {
                    u8 sw0 = dShopSystem_itemActor[i]->home.angle.z;
                    u8 sw1 = ((u16)dShopSystem_itemActor[i]->home.angle.z) >> 8;

                    if ((sw0 != 0xFF && !dComIfGs_isSaveSwitch(sw0)) &&
                        (sw1 != 0xFF && !dComIfGs_isSaveSwitch(sw1)))
                    {
                        onFlag(i);
                        onSoldOutItemFlag(i);
                    }
                }
            }

            dShopSystem_item_count = -1;
        } else {
            mShopCamAction.setCamDataIdx(
                this, dShopSystem_itemActor[0], dShopSystem_itemActor[1], dShopSystem_itemActor[2],
                &dShopSystem_cameraActor[0]->current.pos, &dShopSystem_cameraActor[1]->current.pos);

            for (int i = 0; i < 7; i++) {
                if (dShopSystem_itemActor[i] != NULL) {
                    u8 sw0 = dShopSystem_itemActor[i]->home.angle.z;
                    u8 sw1 = ((u16)dShopSystem_itemActor[i]->home.angle.z) >> 8;

                    if ((sw0 != 0xFF && !dComIfGs_isSaveSwitch(sw0)) &&
                        (sw1 != 0xFF && !dComIfGs_isSaveSwitch(sw1)))
                    {
                        onFlag(i);
                        onSoldOutItemFlag(i);
                    }
                }
            }

            dShopSystem_item_count = -1;
        }
    }

    return 1;
}

int dShopSystem_c::getFlowNodeNum() {
    u16 num = home.angle.x;
    if (num == 0xFFFF) {
        return -1;
    }

    return num;
}

static void dummy() {
    static const char* bck_str = ".bck";
    static const char* btp_str = ".btp";
    static const char* btk_str = ".btk";
}

void dShopSystem_c::setSellItemMax(u8 i_max) {
    dShopSystem_sellItemMax = i_max;
}

BOOL dShopSystem_c::checkShopOpen() {
    for (int i = 0; i < 2; i++) {
        if (dShopSystem_cameraActor[i] != NULL) {
            return true;
        }
    }

    if (dShopSystem_camera_count) {
        return true;
    } else {
        return false;
    }
}

bool dShopSystem_c::checkLeftTrigger(STControl* i_stick) {
    return i_stick->checkLeftTrigger();
}

bool dShopSystem_c::checkRightTrigger(STControl* i_stick) {
    return i_stick->checkRightTrigger();
}

bool dShopSystem_c::dpdMove() {
    return false;
}
