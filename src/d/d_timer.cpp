/**
 * d_timer.cpp
 * Minigame HUD Timer / Counter
 */

#include "d/dolzel.h" // IWYU pragma: keep

#include "d/d_timer.h"
#include "JSystem/J2DGraph/J2DAnmLoader.h"
#include "JSystem/J2DGraph/J2DGrafContext.h"
#include "JSystem/J2DGraph/J2DScreen.h"
#include "JSystem/J2DGraph/J2DTextBox.h"
#include "JSystem/JKernel/JKRExpHeap.h"
#include "JSystem/JUtility/JUTFont.h"
#include "SSystem/SComponent/c_math.h"
#include "d/d_lib.h"
#include "d/d_meter2.h"
#include "d/d_meter2_draw.h"
#include "d/d_meter2_info.h"
#include "d/d_meter_HIO.h"
#include "d/d_pane_class.h"
#include "f_op/f_op_msg_mng.h"
#include "m_Do/m_Do_lib.h"

static int dTimer_createStart2D(s32 param_0, u16 param_1);

int dTimer_c::_create() {
    int phase_state = dComIfG_resLoad(&m_phase, "Timer");

    fopMsg_prm_timer* appen;
    if (phase_state == cPhs_COMPLEATE_e) {
        appen = (fopMsg_prm_timer*)fopMsgM_GetAppend(this);
        if (appen == NULL) {
            return cPhs_ERROR_e;
        }

        dRes_info_c* resInfo = dComIfG_getObjectResInfo("Timer");
        JUT_ASSERT(0, resInfo != NULL);
        dComIfGp_setAllMapArchive(resInfo->getArchive());

        mp_heap = fopMsgM_createExpHeap(0x11000, NULL);
        JKRHeap* prev_heap = mDoExt_setCurrentHeap(mp_heap);
        if (mp_heap != NULL) {
            mp_heap->getTotalFreeSize();

            mp_tm_scrn = new dDlst_TimerScrnDraw_c();
            JUT_ASSERT(0, mp_tm_scrn != NULL);

            if (appen->timer_mode == 10) {
                mp_tm_scrn->setScreen(dComIfG_getTimerMode(), resInfo->getArchive());
            } else {
                mp_tm_scrn->setScreen(appen->timer_mode, resInfo->getArchive());
            }

            mDoExt_setCurrentHeap(prev_heap);
        } else {
            return cPhs_ERROR_e;
        }
    } else {
        return phase_state;
    }

    setShowType(appen->type);

    if (appen->timer_mode == 10) {
        m_timer_mode = dComIfG_getTimerMode();
        mLimitTime = OSMillisecondsToTicks((OSTime)dComIfG_getTimerLimitTimeMs());

        m_time = m_start_time = dLib_time_c::getTime();
        m_start_time -= OSMillisecondsToTicks((OSTime)dComIfG_getTimerNowTimeMs());

        m_mode = 0;
        m_timer_mode = dComIfG_getTimerMode();

        dComIfG_setTimerNowTimeMs(getTimeMs());
        dComIfG_setTimerLimitTimeMs(getLimitTimeMs());
        dComIfG_setTimerMode(m_timer_mode);
        dComIfG_setTimerPtr(this);

        stock_start(10);

        switch (m_timer_mode) {
        case 3:
            mp_tm_scrn->setTimerTrans(0.0f, 145.0f);
            break;
        case 4:
            mp_tm_scrn->setTimerTrans(0.0f, 145.0f);
            break;
        case 6:
            mp_tm_scrn->setTimerTrans(0.0f, 145.0f);
            break;
        case 5:
            mp_tm_scrn->setTimerTrans(0.0f, 145.0f);
            break;
        }
    } else {
        m_timer_mode = appen->timer_mode;
        OS_REPORT("limit Ms = %d\n", appen->limit_ms);

        mLimitTime = OSMillisecondsToTicks((OSTime)appen->limit_ms);
        m_start_time = 0;
        m_time = 0;
        m_mode = 0;

        dComIfG_setTimerNowTimeMs(getTimeMs());
        dComIfG_setTimerLimitTimeMs(getLimitTimeMs());
        dComIfG_setTimerMode(m_timer_mode);
        dComIfG_setTimerPtr(this);
    }

    switch (m_timer_mode) {
    case 8:
        mp_tm_scrn->hideDenominator();
        field_0x16D = 2;
        break;
    case 7:
    case 2:
        field_0x16D = 0;
        break;
    default:
        field_0x16D = 1;
        mp_tm_scrn->hideDenominator();
        break;
    }

    mp_tm_scrn->setHIOType(field_0x16D);

    if (mp_tm_scrn->getHIOType() != 0 && field_0x16D == 0) {
        f32 var_f0 = 0.0f;
        setCounterPos(g_drawHIO.mMiniGame.mTimerPosX_4x3 + var_f0,
                      g_drawHIO.mMiniGame.mTimerPosY_4x3);
    } else {
        f32 var_f0 = 0.0f;
        setCounterPos(g_drawHIO.mMiniGame.mTimerPosX[field_0x16D] + var_f0,
                      g_drawHIO.mMiniGame.mTimerPosY[field_0x16D]);
    }

    f32 var_f0 = 0.0f;
    setImagePos(g_drawHIO.mMiniGame.mIconPosX[field_0x16D] + var_f0,
                g_drawHIO.mMiniGame.mIconPosY[field_0x16D]);
    setTimerPos(g_drawHIO.mMiniGame.mCounterPosX[field_0x16D],
                g_drawHIO.mMiniGame.mCounterPosY[field_0x16D]);

    field_0x120 = 0;
    field_0x128 = 0;
    field_0x130 = 0;
    field_0x138 = 0;
    field_0x16A = 0;
    field_0x16B = 0;
    field_0x158 = 120;
    field_0x15c = 0;
    m_is_ready = 0;
    m_se_idx = 0;
    field_0x160 = 0;

    OS_REPORT("\n\n\nm_se_idx = %d\n\n\n", m_se_idx);
    return cPhs_COMPLEATE_e;
}

int dTimer_c::_execute() {
    if (m_timer_mode == 6 && dComIfGs_getLife() == 0) {
        fopMsgM_Delete(this);
    }

    if (field_0x16A != 1) {
        if (dComIfGp_isPauseFlag() || dMsgObject_isTalkNowCheck()) {
            stop(2);
        }

        field_0x150 = getRestTimeMs();

        switch (m_mode) {
        case 0:
        case 1:
        case 5:
            m_start_time = 0;
            m_time = 0;

            if (m_mode == 1 || m_mode == 5) {
                if (--field_0x168 <= 0) {
                    if (m_mode == 5) {
                        stock_start();
                    } else {
                        start(m_timer_mode);
                    }
                }
            }
            break;
        case 2:
        case 3:
            if (--field_0x168 <= 0) {
                m_mode = 4;
            }

            if (m_mode == 2 && mp_tm_scrn->checkStartAnimeEnd()) {
                m_mode = 3;
            }
            break;
        case 4:
            m_time = dLib_time_c::getTime();
            dComIfG_setTimerNowTimeMs(getTimeMs());
            dComIfG_setTimerLimitTimeMs(getLimitTimeMs());

            if (getRestTimeMs() <= 0) {
                m_time = mLimitTime + m_start_time;
                field_0x138 = 0;
                dComIfG_setTimerNowTimeMs(getTimeMs());
            }

            if (m_timer_mode == 3 || m_timer_mode == 4 || m_timer_mode == 6 || m_timer_mode == 5) {
                f32 pos_y = mp_tm_scrn->getTimerTransY();
                if (pos_y < 145.0f) {
                    pos_y += 25.0f;
                    if (pos_y > 145.0f) {
                        pos_y = 145.0f;
                    }

                    mp_tm_scrn->setTimerTrans(0.0f, pos_y);
                }
            }
            break;
        default:
            m_time = field_0x120;
        }
    } else {
        if (field_0x16B == 2 && !dComIfGp_isPauseFlag() && !dMsgObject_isTalkNowCheck()) {
            restart(2);
        }

        if (field_0x16B == 3) {
            field_0x168--;

            if (field_0x168 <= 0) {
                field_0x168 = 0;
                restart(3);
            } else if (field_0x168 == g_drawHIO.mMiniGame.field_0x15a) {
                mp_tm_scrn->resetCowID();

                if (m_timer_mode != 3) {
                    createStart(0x52D);  // "GO!"
                }
            }
        }
    }

    mp_tm_scrn->setHIO();

    if (m_mode == 8) {
        if (mp_tm_scrn->closeAnime()) {
            fopMsgM_Delete(this);
            return 1;
        }
    } else if (m_mode == 6 && field_0x158 > 0) {
        field_0x158--;
    }

    if (m_mode != 0 && m_mode != 2) {
        if (field_0x15c > 5) {
            mp_tm_scrn->anime();
        } else {
            field_0x15c++;
        }
    }

    if (m_timer_mode == 2 || m_timer_mode == 7 || m_timer_mode == 3 || m_timer_mode == 4 ||
        m_timer_mode == 5 || m_timer_mode == 0)
    {
        mp_tm_scrn->setTimer(getTimeMs());
    } else {
        mp_tm_scrn->setTimer(getRestTimeMs());
    }

    mp_tm_scrn->setCounter(dMeter2Info_getNowCount(), dMeter2Info_getMaxCount());
    return 1;
}

// Helper functions to set float literal order
static f32 dummyLiteralOrder() {
    return 1.0f;
}

static f32 dummyLiteralOrder2() {
    return -1.0f;
}

int dTimer_c::_draw() {
    if (dComIfGp_isPauseFlag() || dMsgObject_isTalkNowCheck()) {
        return 1;
    }

    dComIfGd_set2DOpa(mp_tm_scrn);
    return 1;
}

int dTimer_c::_delete() {
    if (mp_heap != NULL) {
        JKRHeap* prev_heap = mDoExt_setCurrentHeap(mp_heap);
        mp_heap->getTotalFreeSize();

        mp_tm_scrn->deleteScreen();
        delete mp_tm_scrn;
        mp_tm_scrn = NULL;

        fopMsgM_destroyExpHeap(mp_heap);
        mp_heap = NULL;
        mDoExt_setCurrentHeap(prev_heap);
    }

    if ((m_timer_mode == 3 || m_timer_mode == 4) && m_mode != 8 && m_mode != 7) {
        dComIfG_setTimerNowTimeMs(getTimeMs());
        dComIfG_setTimerLimitTimeMs(getLimitTimeMs());
        dComIfG_setTimerMode(m_timer_mode);
    } else {
        dComIfG_setTimerNowTimeMs(0);
        dComIfG_setTimerLimitTimeMs(0);
        dComIfG_setTimerMode(-1);
    }

    dComIfG_setTimerPtr(NULL);
    dComIfG_resDelete(&m_phase, "Timer");
    dComIfGp_setAllMapArchive(NULL);
    return 1;
}

int dTimer_c::deleteCheck() {
    return m_mode == 7;
}

int dTimer_c::start(int i_timer_mode) {
    if (m_mode == 0 || m_mode == 1) {
        m_mode = 4;
        m_time = m_start_time = dLib_time_c::getTime();

        if (i_timer_mode == 3 || i_timer_mode == 4 || i_timer_mode == 6 || i_timer_mode == 5) {
            stop(3);

            if (i_timer_mode != 3) {
                createStart(0x515);  // "READY?"
                field_0x168 = g_drawHIO.mMiniGame.mReadyGo + g_drawHIO.mMiniGame.field_0x15a;
            } else {
                field_0x168 = g_drawHIO.mMiniGame.field_0x15a;
            }

            m_is_ready = 1;
        }
        return 1;
    }

    return 0;
}

int dTimer_c::start(int i_timer_mode, s16 param_1) {
    if (param_1 == -1) {
        if (m_mode == 0) {
            if (i_timer_mode == 2 || i_timer_mode == 7) {
                dTimer_createStart2D(i_timer_mode, 0x3E3);
            }

            field_0x168 = g_drawHIO.mMiniGame.mStartPikariDisplayFrames;
            m_mode = 4;
            m_time = m_start_time = dLib_time_c::getTime();

            stop(4);
            m_mode = 2;
        }
        return 1;
    } else if (m_mode == 0) {
        field_0x168 = param_1;
        m_mode = 1;

        if (i_timer_mode == 3 || i_timer_mode == 4 || i_timer_mode == 6 || i_timer_mode == 5) {
            stop(3);
        }
        return 1;
    }

    return 0;
}

bool dTimer_c::stock_start() {
    if (m_mode == 5) {
        m_mode = 4;

        OSTime current_time = dLib_time_c::getTime();
        m_start_time = current_time;
        m_time = current_time;

        m_start_time -= OSMillisecondsToTicks((OSTime)dComIfG_getTimerNowTimeMs());
        return true;
    }

    return false;
}

int dTimer_c::stock_start(s16 param_0) {
    if (m_mode == 0) {
        field_0x168 = param_0;
        m_mode = 5;
        return 1;
    }

    return 0;
}

int dTimer_c::stop(u8 param_0) {
    if (field_0x16A == 1 || field_0x16B != 0) {
        return 0;
    } else if (m_mode != 4) {
        return 0;
    } else {
        field_0x130 = dLib_time_c::getTime();
        field_0x16A = 1;
        field_0x16B = param_0;
        return 1;
    }
}

int dTimer_c::restart(u8 param_0) {
    if (field_0x16A != 1 || field_0x16B != param_0) {
        return 0;
    } else if (m_mode != 4 && m_mode != 2) {
        return 0;
    } else {
        m_time = dLib_time_c::getTime();
        field_0x138 += m_time - field_0x130;
        field_0x16A = 0;
        field_0x16B = 0;
        return 1;
    }
}

int dTimer_c::end(int param_0) {
    if (m_mode != 4) {
        return 0;
    }

    field_0x120 = dLib_time_c::getTime();
    m_mode = 6;
    dComIfG_setTimerNowTimeMs(OSTicksToMilliseconds(field_0x120 - m_start_time - field_0x138));

    if (param_0 != -1) {
        field_0x158 = param_0;
    }

    return 1;
}

int dTimer_c::deleteRequest() {
    m_mode = 8;
    return 1;
}

int dTimer_c::getTimeMs() {
    return OSTicksToMilliseconds(m_time - m_start_time - field_0x138);
}
int dTimer_c::getLimitTimeMs() {
    return OSTicksToMilliseconds(mLimitTime);
}

int dTimer_c::getRestTimeMs() {
    OSTime step0 = m_time - m_start_time - field_0x138;
    step0 = mLimitTime - step0;
    return OSTicksToMilliseconds(step0);
}

bool dTimer_c::isStart() {
    if (field_0x16A != 1 && m_mode == 4) {
        return true;
    }

    return false;
}

dDlst_TimerScrnDraw_c::dDlst_TimerScrnDraw_c() {
    field_0x3e2 = 0;
    mHIOType = 0;
    field_0x3D8 = 0;
    field_0x3CC = -1;
    field_0x3D0 = -1;
    field_0x3D4 = 0;

    mParentAlpha = g_drawHIO.mMiniGame.field_0x2c[mHIOType];
    mTimerAlpha = g_drawHIO.mMiniGame.mTimerAlpha[mHIOType];
    mCounterAlpha = g_drawHIO.mMiniGame.mCounterAlpha[mHIOType];
    mImageAlpha = g_drawHIO.mMiniGame.mIconAlpha[mHIOType];

    field_0x3DE = 0;
    field_0x3DF = 0;
    field_0x3E0 = 0;
    show();
}

void dDlst_TimerScrnDraw_c::setHIO() {
    bool var_r31 = false;

    if (mpParent != NULL) {
        setParentPos(g_drawHIO.mMiniGame.field_0x14[mHIOType],
                     g_drawHIO.mMiniGame.field_0x20[mHIOType]);

        mpParent->scale(g_drawHIO.mMiniGame.field_0x8[mHIOType],
                        g_drawHIO.mMiniGame.field_0x8[mHIOType]);

        if (field_0x3DF != 0 && field_0x3E0 != 0 &&
            mParentAlpha != g_drawHIO.mMiniGame.field_0x2c[mHIOType])
        {
            if (mpParent != NULL) {
                mpParent->setAlphaRate(g_drawHIO.mMiniGame.field_0x2c[mHIOType]);
            }

            mParentAlpha = g_drawHIO.mMiniGame.field_0x2c[mHIOType];
            var_r31 = true;
        }
    }

    if (mpTimeParent != NULL) {
        if (field_0x3e2 != 0 && mHIOType == 0) {
            setTimerPos(g_drawHIO.mMiniGame.mTimerPosX_4x3, g_drawHIO.mMiniGame.mTimerPosY_4x3);
        } else {
            setTimerPos(g_drawHIO.mMiniGame.mTimerPosX[mHIOType],
                        g_drawHIO.mMiniGame.mTimerPosY[mHIOType]);
        }

        if (field_0x3e2 != 0 && mHIOType == 0) {
            mpTimeParent->scale(g_drawHIO.mMiniGame.mTimerSizeX_4x3,
                                g_drawHIO.mMiniGame.mTimerSizeY_4x3);
        } else {
            mpTimeParent->scale(g_drawHIO.mMiniGame.mTimerSizeX[mHIOType],
                                g_drawHIO.mMiniGame.mTimerSizeY[mHIOType]);
        }

        if (field_0x3DF != 0) {
            if (mTimerAlpha != g_drawHIO.mMiniGame.mTimerAlpha[mHIOType] || var_r31) {
                if (mpTimeParent != NULL) {
                    mpTimeParent->setAlphaRate(g_drawHIO.mMiniGame.mTimerAlpha[mHIOType] *
                                               mParentAlpha);
                }

                mTimerAlpha = g_drawHIO.mMiniGame.mTimerAlpha[mHIOType];
            }
        }
    }

    if (mpCowParent != NULL) {
        setCounterPos(g_drawHIO.mMiniGame.mCounterPosX[mHIOType],
                      g_drawHIO.mMiniGame.mCounterPosY[mHIOType]);
        mpCowParent->scale(g_drawHIO.mMiniGame.mCounterSizeX[mHIOType],
                           g_drawHIO.mMiniGame.mCounterSizeY[mHIOType]);

        if (field_0x3E0 != 0) {
            if (mCounterAlpha != g_drawHIO.mMiniGame.mCounterAlpha[mHIOType] || var_r31) {
                if (mpCowParent != NULL) {
                    mpCowParent->setAlphaRate(g_drawHIO.mMiniGame.mCounterAlpha[mHIOType] *
                                              mParentAlpha);
                }

                mCounterAlpha = g_drawHIO.mMiniGame.mCounterAlpha[mHIOType];
            }
        }
    }

    if (mpImageParent != NULL) {
        setImagePos(g_drawHIO.mMiniGame.mIconPosX[mHIOType],
                    g_drawHIO.mMiniGame.mIconPosY[mHIOType]);
        mpImageParent->scale(g_drawHIO.mMiniGame.mIconSizeX[mHIOType],
                             g_drawHIO.mMiniGame.mIconSizeY[mHIOType]);

        if (field_0x3E0 != 0) {
            if (mImageAlpha != g_drawHIO.mMiniGame.mIconAlpha[mHIOType] || var_r31) {
                if (mpImageParent != NULL) {
                    mpImageParent->setAlphaRate(g_drawHIO.mMiniGame.mIconAlpha[mHIOType] *
                                                mParentAlpha);
                }

                mImageAlpha = g_drawHIO.mMiniGame.mIconAlpha[mHIOType];
            }
        }
    }
}

void dDlst_TimerScrnDraw_c::setScreen(s32 param_0, JKRArchive* i_archive) {
    field_0x3C8 = param_0;
    mpArchive = i_archive;

    if (field_0x3C8 == 3 || field_0x3C8 == 4) {
        setScreenBoatRace();
    } else if (field_0x3C8 == 8) {
        setScreenRider();
    } else {
        setScreenBase();
    }

    setShowType(3);

    mpGetInScreen = new J2DScreen();
    JUT_ASSERT(0, mpGetInScreen != NULL);

    bool fg = mpGetInScreen->setPriority("zelda_game_image_cow_get_in.blo", 0x20000, mpArchive);
    JUT_ASSERT(0, fg != false);
    dPaneClass_showNullPane(mpGetInScreen);

    OSInitFastCast();
    mpGetInBck = (J2DAnmTransform*)J2DAnmLoaderDataBase::load(
        JKRGetNameResource("zelda_game_image_cow_get_in.bck", mpArchive));

    mpGetInParent = new CPaneMgr(mpGetInScreen, 'get_in_n', 2, NULL);
    JUT_ASSERT(0, mpGetInParent != NULL);

    mpGetInRoot = new CPaneMgr(mpGetInScreen, 'n_all', 0, NULL);
    JUT_ASSERT(0, mpGetInRoot != NULL);

    mpGetInText = new CPaneMgr(mpGetInScreen, 'get_in', 0, NULL);
    JUT_ASSERT(0, mpGetInText != NULL);

    static_cast<J2DTextBox*>(mpGetInScreen->search('get_in_s'))->setFont(mDoExt_getMesgFont());
    static_cast<J2DTextBox*>(mpGetInScreen->search('get_in'))->setFont(mDoExt_getMesgFont());

    for (int i = 0; i < 51; i++) {
        m_getin_info[i].bck_frame = 0.0f;
        m_getin_info[i].pikari_frame = 0.0f;
        m_getin_info[i].pos_x = 0;
        m_getin_info[i].pos_y = 0;
        m_getin_info[i].field_0xc = 0;
        m_getin_info[i].field_0xd = 0xFF;
    }

    mCowID = 0;
    playBckAnimation(0.0f);
    mTimerTransY = 0.0f;
    mTimerTransX = 0.0f;
}

void dDlst_TimerScrnDraw_c::setScreenBase() {
    mpScreen = new J2DScreen();
    JUT_ASSERT(0, mpScreen != NULL);

    bool fg = mpScreen->setPriority("zelda_game_image_cow_game.blo", 0x20000, mpArchive);
    JUT_ASSERT(0, fg != false);
    dPaneClass_showNullPane(mpScreen);

    mpParent = NULL;

    mpCowParent = new CPaneMgr(mpScreen, 'cow_n', 2, NULL);
    JUT_ASSERT(0, mpCowParent != NULL);
    mpCowParent->setAlphaRate(0.0f);

    mpTimeParent = new CPaneMgr(mpScreen, 'time_n', 2, NULL);
    JUT_ASSERT(0, mpTimeParent != NULL);
    mpTimeParent->setAlphaRate(0.0f);

    mpImageParent = new CPaneMgr(mpScreen, 'cow_i_n', 2, NULL);
    JUT_ASSERT(0, mpImageParent != NULL);
    mpImageParent->setAlphaRate(0.0f);

    field_0x5c[0][0] = mpScreen->search('c_n_2');
    field_0x5c[0][1] = mpScreen->search('c_n_2_s');
    field_0x5c[1][0] = mpScreen->search('c_n_1');
    field_0x5c[1][1] = mpScreen->search('c_n_1_s');

    field_0x6c[0] = static_cast<J2DPicture*>(mpScreen->search('c_n_4'));
    field_0x6c[1] = static_cast<J2DPicture*>(mpScreen->search('c_n_4_s'));
    field_0x74[0] = static_cast<J2DPicture*>(mpScreen->search('c_n_3'));
    field_0x74[1] = static_cast<J2DPicture*>(mpScreen->search('c_n_3_s'));

    mTimerText[0][0] = mpScreen->search('t_n_6');
    mTimerText[0][1] = mpScreen->search('t_n_6_s');
    mTimerText[1][0] = mpScreen->search('t_n_5');
    mTimerText[1][1] = mpScreen->search('t_n_5_s');
    mTimerText[2][0] = mpScreen->search('t_n_4');
    mTimerText[2][1] = mpScreen->search('t_n_4_s');
    mTimerText[3][0] = mpScreen->search('t_n_3');
    mTimerText[3][1] = mpScreen->search('t_n_3_s');
    mTimerText[4][0] = mpScreen->search('t_n_2');
    mTimerText[4][1] = mpScreen->search('t_n_2_s');
    mTimerText[5][0] = mpScreen->search('t_n_1');
    mTimerText[5][1] = mpScreen->search('t_n_1_s');
}

void dDlst_TimerScrnDraw_c::setScreenBoatRace() {
    mpScreen = new J2DScreen();
    JUT_ASSERT(0, mpScreen != NULL);

    bool fg = mpScreen->setPriority("zelda_game_image_zora_kawakudari.blo", 0x20000, mpArchive);
    JUT_ASSERT(0, fg != false);
    dPaneClass_showNullPane(mpScreen);

    mpParent = new CPaneMgr(mpScreen, 'ta_co_n', 2, NULL);
    JUT_ASSERT(0, mpParent != NULL);

    mpCowParent = new CPaneMgr(mpScreen, 'num_n', 2, NULL);
    JUT_ASSERT(0, mpCowParent != NULL);
    mpCowParent->setAlphaRate(0.0f);

    mpTimeParent = NULL;

    mpImageParent = new CPaneMgr(mpScreen, 'target_n', 2, NULL);
    JUT_ASSERT(0, mpImageParent != NULL);
    mpImageParent->setAlphaRate(0.0f);

    mpScreen->search('w_target')->hide();
    mpScreen->search('target')->show();

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            field_0x5c[i][j] = NULL;
        }
    }

    field_0x6c[0] = static_cast<J2DPicture*>(mpScreen->search('num_1'));
    field_0x6c[1] = NULL;
    field_0x74[0] = static_cast<J2DPicture*>(mpScreen->search('num_0'));
    field_0x74[1] = NULL;

    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 2; j++) {
            mTimerText[i][j] = NULL;
        }
    }
}

void dDlst_TimerScrnDraw_c::setScreenRider() {
    mpScreen = new J2DScreen();
    JUT_ASSERT(0, mpScreen != NULL);

    bool fg = mpScreen->setPriority("zelda_game_image_rider.blo", 0x20000, mpArchive);
    JUT_ASSERT(0, fg != false);
    dPaneClass_showNullPane(mpScreen);

    mpParent = NULL;

    mpCowParent = new CPaneMgr(mpScreen, 'num_n', 2, NULL);
    JUT_ASSERT(0, mpCowParent != NULL);
    mpCowParent->setAlphaRate(0.0f);

    mpTimeParent = NULL;

    mpImageParent = new CPaneMgr(mpScreen, 'rid_i_n', 2, NULL);
    JUT_ASSERT(0, mpImageParent != NULL);
    mpImageParent->setAlphaRate(0.0f);

    field_0x5c[0][0] = mpScreen->search('n_n_2');
    field_0x5c[0][1] = mpScreen->search('n_n_2_s');
    field_0x5c[1][0] = mpScreen->search('n_n_1');
    field_0x5c[1][1] = mpScreen->search('n_n_1_s');

    field_0x6c[0] = static_cast<J2DPicture*>(mpScreen->search('n_n_4'));
    field_0x6c[1] = static_cast<J2DPicture*>(mpScreen->search('n_n_4_s'));
    field_0x74[0] = static_cast<J2DPicture*>(mpScreen->search('n_n_3'));
    field_0x74[1] = static_cast<J2DPicture*>(mpScreen->search('n_n_3_s'));

    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 2; j++) {
            mTimerText[i][j] = NULL;
        }
    }
}

void dDlst_TimerScrnDraw_c::hideDenominator() {
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            if (field_0x5c[i][j] != NULL) {
                field_0x5c[i][j]->hide();
            }
        }
    }

    if (field_0x3C8 == 8) {
        mpScreen->search('n_sl_s')->hide();
        mpScreen->search('n_sl')->hide();
    } else {
        J2DPane* sl_s = mpScreen->search('c_sl_s');
        J2DPane* sl = mpScreen->search('c_sl');

        if (sl_s != NULL) {
            sl_s->hide();
        }

        if (sl != NULL) {
            sl->hide();
        }
    }
}

void dDlst_TimerScrnDraw_c::deleteScreen() {
    delete mpScreen;
    mpScreen = NULL;

    if (mpParent != NULL) {
        delete mpParent;
        mpParent = NULL;
    }

    if (mpTimeParent != NULL) {
        delete mpTimeParent;
        mpTimeParent = NULL;
    }

    if (mpCowParent != NULL) {
        delete mpCowParent;
        mpCowParent = NULL;
    }

    if (mpImageParent != NULL) {
        delete mpImageParent;
        mpImageParent = NULL;
    }

    delete mpGetInScreen;
    mpGetInScreen = NULL;

    delete mpGetInBck;
    mpGetInBck = NULL;

    delete mpGetInParent;
    mpGetInParent = NULL;

    delete mpGetInRoot;
    mpGetInRoot = NULL;

    delete mpGetInText;
    mpGetInText = NULL;
}

void dDlst_TimerScrnDraw_c::changeNumberTexture(J2DPane* i_pane, int i_num) {
    if (i_num < 0 || i_num >= 10) {
        i_num = 0;
    }

    dComIfGp_getMain2DArchive()->getResource('TIMG', getNumber(i_num));
    static_cast<J2DPicture*>(i_pane)->changeTexture(getNumber(i_num), 0);
}

const char* dDlst_TimerScrnDraw_c::getNumber(int i_num) {
    return dMeter2Info_getNumberTextureName(i_num);
}

void dDlst_TimerScrnDraw_c::setTimer(int i_time) {
    int min;
    int sec;
    int ms;
    dMeter2Info_setTimeMs(i_time);

    sec = (i_time / 1000);
    ms = i_time - sec * 1000;
    min = sec / 60;
    sec = sec - min * 60;

    if (min > 99) {
        min = 99;
        sec = 59;
        ms = 999;
    }

    for (int i = 0; i < 2; i++) {
        if (mTimerText[0][i] != NULL) {
            changeNumberTexture(mTimerText[0][i], min / 10);
        }

        if (mTimerText[1][i] != NULL) {
            changeNumberTexture(mTimerText[1][i], min % 10);
        }

        if (mTimerText[2][i] != NULL) {
            changeNumberTexture(mTimerText[2][i], sec / 10);
        }

        if (mTimerText[3][i] != NULL) {
            changeNumberTexture(mTimerText[3][i], sec % 10);
        }

        if (mTimerText[4][i] != NULL) {
            changeNumberTexture(mTimerText[4][i], ms / 100);
        }

        if (mTimerText[5][i] != NULL) {
            changeNumberTexture(mTimerText[5][i], (ms % 100) / 10);
        }
    }

    if (mpTimeParent != NULL) {
        if ((dComIfGp_event_getMode() == 1 && field_0x3DF != 0) || !isVisible()) {
            s16 alpha_time = mpTimeParent->getAlphaTimer();
            if (alpha_time < 5) {
                f32 temp_f0 = acc(5, 5 - (s16)(alpha_time + 1), 0);
                mpTimeParent->setAlphaRate(mParentAlpha *
                                           (temp_f0 * g_drawHIO.mMiniGame.mTimerAlpha[mHIOType]));
                mpTimeParent->alphaAnimeStart(alpha_time + 1);
            }
        } else {
            s16 alpha_time = mpTimeParent->getAlphaTimer();
            if (alpha_time > 0) {
                f32 temp_f0 = acc(5, 5 - (s16)(alpha_time - 1), 0);
                mpTimeParent->setAlphaRate(mParentAlpha *
                                           (temp_f0 * g_drawHIO.mMiniGame.mTimerAlpha[mHIOType]));
                mpTimeParent->alphaAnimeStart(alpha_time - 1);
            }
        }
    }
}

void dDlst_TimerScrnDraw_c::setCounter(u8 i_count, u8 i_max) {
    bool var_r30 = false;

    if (field_0x3D4 > 0) {
        field_0x3D4--;
        var_r30 = true;
    }

    if (i_count != field_0x3CC || var_r30) {
        if (i_count < field_0x3CC) {
            field_0x3CC--;

            if (field_0x3C8 == 3 || field_0x3C8 == 4) {
                field_0x3D4 = 60;
            }
        } else if (i_count > field_0x3CC) {
            field_0x3CC++;

            if (field_0x3D4 > 0) {
                field_0x3D4 = 0;
            }
        }

        if (field_0x3CC < 0) {
            field_0x3CC = 0;
        }

        if (field_0x3CC > 99) {
            field_0x3CC = 99;
        }

        int temp_r29 = field_0x3CC % 10;
        int temp_r28 = (field_0x3CC % 100) / 10;

        for (int i = 0; i < 2; i++) {
            if (field_0x74[i] != NULL) {
                changeNumberTexture(field_0x74[i], temp_r29);

                if (var_r30) {
                    if (field_0x3D4 > 0) {
                        field_0x74[i]->setCornerColor(JUtility::TColor(255, 0, 0, 255),
                                                      JUtility::TColor(255, 0, 0, 255),
                                                      JUtility::TColor(255, 255, 255, 255),
                                                      JUtility::TColor(255, 255, 255, 255));
                    } else {
                        field_0x74[i]->setCornerColor(JUtility::TColor(255, 255, 0, 255),
                                                      JUtility::TColor(255, 255, 0, 255),
                                                      JUtility::TColor(255, 255, 255, 255),
                                                      JUtility::TColor(255, 255, 255, 255));
                    }
                }
            }

            if (field_0x6c[i] != NULL) {
                changeNumberTexture(field_0x6c[i], temp_r28);

                if (var_r30) {
                    if (field_0x3D4 > 0) {
                        field_0x6c[i]->setCornerColor(JUtility::TColor(255, 0, 0, 255),
                                                      JUtility::TColor(255, 0, 0, 255),
                                                      JUtility::TColor(255, 255, 255, 255),
                                                      JUtility::TColor(255, 255, 255, 255));
                    } else {
                        field_0x6c[i]->setCornerColor(JUtility::TColor(255, 255, 0, 255),
                                                      JUtility::TColor(255, 255, 0, 255),
                                                      JUtility::TColor(255, 255, 255, 255),
                                                      JUtility::TColor(255, 255, 255, 255));
                    }
                }
            }
        }
    }

    if (i_max != field_0x3D0) {
        if (i_max < field_0x3D0) {
            field_0x3D0--;
        } else {
            field_0x3D0++;
        }

        if (field_0x3D0 < 0) {
            field_0x3D0 = 0;
        }

        if (field_0x3D0 > 99) {
            field_0x3D0 = 99;
        }

        int temp_r27 = field_0x3D0 % 10;
        int temp_r28_2 = (field_0x3D0 % 100) / 10;

        for (int i = 0; i < 2; i++) {
            if (field_0x5c[1][i] != NULL) {
                changeNumberTexture(field_0x5c[1][i], temp_r27);
            }

            if (field_0x5c[0][i] != NULL) {
                changeNumberTexture(field_0x5c[0][i], temp_r28_2);
            }
        }
    }

    if ((dComIfGp_event_getMode() == 1 && field_0x3E0 != 0) || !isVisible()) {
        if (mpCowParent != NULL) {
            s16 alpha_time = mpCowParent->getAlphaTimer();

            if (alpha_time < 5) {
                alpha_time++;
                f32 var_f0 = 1.0f - acc(5, alpha_time, 0);
                mpCowParent->setAlphaRate(mParentAlpha *
                                          (var_f0 * g_drawHIO.mMiniGame.mCounterAlpha[mHIOType]));
                mpCowParent->alphaAnimeStart(alpha_time);
            }
        }

        if (mpImageParent != NULL) {
            s16 alpha_time = mpImageParent->getAlphaTimer();

            if (alpha_time < 5) {
                alpha_time++;
                f32 var_f0 = 1.0f - acc(5, alpha_time, 0);
                mpImageParent->setAlphaRate(mParentAlpha *
                                            (var_f0 * g_drawHIO.mMiniGame.mIconAlpha[mHIOType]));
                mpImageParent->alphaAnimeStart(alpha_time);
            }
        }
    } else {
        if (mpCowParent != NULL) {
            s16 alpha_time = mpCowParent->getAlphaTimer();

            if (alpha_time > 0) {
                alpha_time--;
                f32 var_f0 = 1.0f - acc(5, alpha_time, 0);
                mpCowParent->setAlphaRate(mParentAlpha *
                                          (var_f0 * g_drawHIO.mMiniGame.mCounterAlpha[mHIOType]));
                mpCowParent->alphaAnimeStart(alpha_time);
            }
        }

        if (mpImageParent != NULL) {
            s16 alpha_time = mpImageParent->getAlphaTimer();

            if (alpha_time > 0) {
                alpha_time--;
                f32 var_f0 = 1.0f - acc(5, alpha_time, 0);
                mpImageParent->setAlphaRate(mParentAlpha *
                                            (var_f0 * g_drawHIO.mMiniGame.mIconAlpha[mHIOType]));
                mpImageParent->alphaAnimeStart(alpha_time);
            }
        }
    }
}

void dDlst_TimerScrnDraw_c::setParentPos(f32 i_posX, f32 i_posY) {
    if (mpParent != NULL) {
        mpParent->paneTrans(i_posX, i_posY);
    }
}

void dDlst_TimerScrnDraw_c::setTimerPos(f32 i_posX, f32 i_posY) {
    if (mpTimeParent != NULL) {
        mpTimeParent->paneTrans(i_posX + mTimerTransX, i_posY + mTimerTransY);
    }
}

void dDlst_TimerScrnDraw_c::setCounterPos(f32 i_posX, f32 i_posY) {
    if (mpCowParent != NULL) {
        mpCowParent->paneTrans(i_posX, i_posY);
    }
}

void dDlst_TimerScrnDraw_c::setImagePos(f32 i_posX, f32 i_posY) {
    if (mpImageParent != NULL) {
        mpImageParent->paneTrans(i_posX, i_posY);
    }
}

void dDlst_TimerScrnDraw_c::setShowType(u8 i_type) {
    dComIfG_setTimerType(i_type);

    if (mpTimeParent != NULL) {
        if (i_type & 1) {
            mpTimeParent->show();
        } else {
            mpTimeParent->hide();
        }
    }

    if (i_type & 2) {
        if (mpCowParent != NULL) {
            mpCowParent->show();
        }

        if (mpImageParent != NULL) {
            mpImageParent->show();
        }
    } else {
        if (mpCowParent != NULL) {
            mpCowParent->hide();
        }

        if (mpImageParent != NULL) {
            mpImageParent->hide();
        }
    }
}

void dDlst_TimerScrnDraw_c::anime() {
    static const s16 animeFrame[] = {7, 15, 22};

    if (field_0x3DE == 0) {
        if (field_0x3D8 <= animeFrame[2]) {
            field_0x3D8++;
        } else {
            field_0x3DE = 1;
        }

        if (field_0x3D8 <= animeFrame[1]) {
            f32 temp_f31 = acc(animeFrame[1], field_0x3D8, 0);
            f32 temp_f1 = (1.0f - temp_f31) * -50.0f;

            if (mpTimeParent != NULL) {
                if (field_0x3e2 != 0 && mHIOType == 0) {
                    setTimerPos(g_drawHIO.mMiniGame.mTimerPosX_4x3 + temp_f1,
                                g_drawHIO.mMiniGame.mTimerPosY_4x3);
                } else {
                    setTimerPos(g_drawHIO.mMiniGame.mTimerPosX[mHIOType] + temp_f1,
                                g_drawHIO.mMiniGame.mTimerPosY[mHIOType]);
                }

                mpTimeParent->setAlphaRate(mParentAlpha *
                                           (temp_f31 * g_drawHIO.mMiniGame.mTimerAlpha[mHIOType]));

                if (field_0x3e2 != 0 && mHIOType == 0) {
                    mpTimeParent->scale(g_drawHIO.mMiniGame.mTimerSizeX_4x3,
                                        g_drawHIO.mMiniGame.mTimerSizeY_4x3);
                } else {
                    mpTimeParent->scale(g_drawHIO.mMiniGame.mTimerSizeX[mHIOType],
                                        g_drawHIO.mMiniGame.mTimerSizeY[mHIOType]);
                }
            }

            if (field_0x3D8 == animeFrame[1]) {
                field_0x3DF = 1;
            }
        }

        if (field_0x3D8 > animeFrame[0] && field_0x3D8 <= animeFrame[2]) {
            f32 temp_f31 = acc(animeFrame[1], field_0x3D8 - animeFrame[0], 0);
            f32 temp_f1 = (1.0f - temp_f31) * -50.0f;

            if (mpCowParent != NULL) {
                setCounterPos(g_drawHIO.mMiniGame.mCounterPosX[mHIOType] + temp_f1,
                              g_drawHIO.mMiniGame.mCounterPosY[mHIOType]);
                mpCowParent->setAlphaRate(mParentAlpha *
                                          (temp_f31 * g_drawHIO.mMiniGame.mCounterAlpha[mHIOType]));
                mpCowParent->scale(g_drawHIO.mMiniGame.mCounterSizeX[mHIOType],
                                   g_drawHIO.mMiniGame.mCounterSizeY[mHIOType]);
            }

            if (mpImageParent != NULL) {
                setImagePos(g_drawHIO.mMiniGame.mIconPosX[mHIOType] + temp_f1,
                            g_drawHIO.mMiniGame.mIconPosY[mHIOType]);
                mpImageParent->setAlphaRate(mParentAlpha *
                                            (temp_f31 * g_drawHIO.mMiniGame.mIconAlpha[mHIOType]));
                mpImageParent->scale(g_drawHIO.mMiniGame.mIconSizeX[mHIOType],
                                     g_drawHIO.mMiniGame.mIconSizeY[mHIOType]);
            }

            if (field_0x3D8 == animeFrame[2]) {
                field_0x3E0 = 1;
            }
        }
    }
}

BOOL dDlst_TimerScrnDraw_c::closeAnime() {
    BOOL var_r31 = false;
    field_0x3D8++;

    if (field_0x3D8 <= 7) {
        f32 temp_f31 = acc(7, field_0x3D8, 0);
        f32 temp_f30 = acc(7, 7 - field_0x3D8, 0);
        f32 temp_f1 = temp_f31 * -50.0f;

        if (mpTimeParent != NULL) {
            if (field_0x3e2 != 0 && mHIOType == 0) {
                setTimerPos(g_drawHIO.mMiniGame.mTimerPosX_4x3 + temp_f1,
                            g_drawHIO.mMiniGame.mTimerPosY_4x3);
            } else {
                setTimerPos(g_drawHIO.mMiniGame.mTimerPosX[mHIOType] + temp_f1,
                            g_drawHIO.mMiniGame.mTimerPosY[mHIOType]);
            }

            mpTimeParent->setAlphaRate(mParentAlpha *
                                       (temp_f30 * g_drawHIO.mMiniGame.mTimerAlpha[mHIOType]));
        }

        if (mpCowParent != NULL) {
            setCounterPos(g_drawHIO.mMiniGame.mCounterPosX[mHIOType] + temp_f1,
                          g_drawHIO.mMiniGame.mCounterPosY[mHIOType]);
            mpCowParent->setAlphaRate(mParentAlpha *
                                      (temp_f30 * g_drawHIO.mMiniGame.mCounterAlpha[mHIOType]));
        }

        if (mpImageParent != NULL) {
            setImagePos(g_drawHIO.mMiniGame.mIconPosX[mHIOType] + temp_f1,
                        g_drawHIO.mMiniGame.mIconPosY[mHIOType]);
            mpImageParent->setAlphaRate(mParentAlpha *
                                        (temp_f30 * g_drawHIO.mMiniGame.mIconAlpha[mHIOType]));
        }
    }

    if (field_0x3D8 >= 7) {
        var_r31 = true;
    }

    return var_r31;
}

int dDlst_TimerScrnDraw_c::createGetIn(cXyz i_pos) {
    char string[104];
    dMeter2Info_getString(0x3E4, string, NULL);  // "GOAT IN!"
    strcpy(static_cast<J2DTextBox*>(mpGetInScreen->search('get_in_s'))->getStringPtr(), string);
    strcpy(static_cast<J2DTextBox*>(mpGetInScreen->search('get_in'))->getStringPtr(), string);

    if (mCowID < 50) {
        m_getin_info[mCowID].bck_frame = 40.0f;
        m_getin_info[mCowID].pikari_frame = -1.0f;

        if (g_drawHIO.mMiniGame.mGetInTextLocation == 1) {
            m_getin_info[mCowID].pos_x = i_pos.x;
            m_getin_info[mCowID].pos_y = i_pos.y;
        } else {
            u8 spC[50];
            for (int i = 0; i < 50; i++) {
                spC[i] = 0;
            }

            f32 temp = (f32)g_drawHIO.mMiniGame.mGetInTextAlphaFrames +
                       ((f32)g_drawHIO.mMiniGame.mGetInTextWaitFrames + 60.0f);

            for (int i = 0; i < mCowID; i++) {
                if (m_getin_info[i].bck_frame > 40.0f && m_getin_info[i].bck_frame < temp) {
                    if (m_getin_info[i].field_0xd != 0xFF) {
                        spC[m_getin_info[i].field_0xd] = 1;
                    }
                }
            }

            for (int i = 0; i < 50; i++) {
                if (spC[i] == 0) {
                    m_getin_info[mCowID].field_0xd = i;
                    break;
                }
            }

            f32 pos_x = 0.0f;
            f32 pos_y = 0.0f;
            switch (m_getin_info[mCowID].field_0xd) {
            case 0:
                break;
            case 1:
                pos_x = -100.0f;
                break;
            case 2:
                pos_x = 100.0f;
                break;
            case 3:
                pos_y = 75.0f;
                break;
            case 4:
                pos_x = -100.0f;
                pos_y = 75.0f;
                break;
            case 5:
                pos_x = 100.0f;
                pos_y = 75.0f;
                break;
            case 6:
                pos_y = -50.0f;
                break;
            case 7:
                pos_x = -100.0f;
                pos_y = -75.0f;
                break;
            case 8:
                pos_x = 100.0f;
                pos_y = -75.0f;
                break;
            default:
                pos_x = cM_rndFX(200.0f);
                pos_y = cM_rndFX(150.0f);
                break;
            }

            m_getin_info[mCowID].pos_x = pos_x + cM_rndFX(20.0f);
            m_getin_info[mCowID].pos_y = pos_y + cM_rndFX(20.0f);
        }

        m_getin_info[mCowID].field_0xc = 0;
        Z2GetAudioMgr()->seStart(Z2SE_SY_COW_GET_IN, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
        mCowID++;
        return 1;
    }

    return 0;
}

s32 dDlst_TimerScrnDraw_c::createStart(u16 i_messageID) {
    char string[112];
    dMeter2Info_getString(i_messageID, string, NULL);
    strcpy(static_cast<J2DTextBox*>(mpGetInScreen->search('get_in_s'))->getStringPtr(), string);
    strcpy(static_cast<J2DTextBox*>(mpGetInScreen->search('get_in'))->getStringPtr(), string);

    if (mCowID == 0) {
        m_getin_info[mCowID].bck_frame = 40.0f;
        m_getin_info[mCowID].pikari_frame = -1.0f;
        m_getin_info[mCowID].pos_x = 0;
        m_getin_info[mCowID].pos_y = 0;
        m_getin_info[mCowID].field_0xc = 1;
        mCowID++;
        return 1;
    }

    return 0;
}

void dDlst_TimerScrnDraw_c::draw() {
    J2DGrafContext* graf_ctx = dComIfGp_getCurrentGrafPort();
    graf_ctx->setup2D();

    mpScreen->draw(0.0f, 0.0f, graf_ctx);

    f32 temp = (f32)g_drawHIO.mMiniGame.mGetInTextAlphaFrames +
               ((f32)g_drawHIO.mMiniGame.mGetInTextWaitFrames + 60.0f);

    for (int i = 0; i < 51; i++) {
        if (m_getin_info[i].bck_frame > 0.0f && m_getin_info[i].bck_frame < temp) {
            f32 var_f29 = 1.0f;

            if (m_getin_info[i].bck_frame < 60.0f) {
                m_getin_info[i].bck_frame += g_drawHIO.mMiniGame.mGetInTextAnimSpeed;
                if (m_getin_info[i].bck_frame > 60.0f) {
                    m_getin_info[i].bck_frame = 60.0f;
                }

                playBckAnimation(m_getin_info[i].bck_frame);
            } else if (m_getin_info[i].bck_frame < g_drawHIO.mMiniGame.mGetInTextWaitFrames + 60.0f)
            {
                m_getin_info[i].bck_frame++;
                playBckAnimation(60.0f);
            } else if (m_getin_info[i].bck_frame < temp) {
                m_getin_info[i].bck_frame++;
                playBckAnimation(60.0f);

                var_f29 = acc(g_drawHIO.mMiniGame.mGetInTextAlphaFrames,
                              temp - m_getin_info[i].bck_frame, 0);
            }

            mpGetInParent->setAlphaRate(var_f29);

            if (g_drawHIO.mMiniGame.mGetInTextLocation == 1) {
                mpGetInRoot->translate(m_getin_info[i].pos_x + g_drawHIO.mMiniGame.mGetInTextPosX,
                                       m_getin_info[i].pos_y + g_drawHIO.mMiniGame.mGetInTextPosY);
            } else {
                f32 temp_f2 = m_getin_info[i].bck_frame - 40.0f;
                f32 var_f3 = ((temp_f2 * 0.5f) * temp_f2) * 0.15f;
                if (i == 0) {
                    var_f3 = 0.0f;
                }

                mpGetInRoot->paneTrans(
                    m_getin_info[i].pos_x + g_drawHIO.mMiniGame.mGetInTextPosX,
                    (m_getin_info[i].pos_y + g_drawHIO.mMiniGame.mGetInTextPosY) - var_f3);
            }

            mpGetInRoot->scale(g_drawHIO.mMiniGame.mGetInTextSizeX,
                               g_drawHIO.mMiniGame.mGetInTextSizeY);
            mpGetInScreen->draw(0.0f, 0.0f, graf_ctx);

            if (m_getin_info[i].pikari_frame > 0.0f) {
                drawPikari(i);
            } else if (m_getin_info[i].pikari_frame == -1.0f) {
                if (m_getin_info[i].field_0xc == 0) {
                    if (m_getin_info[i].bck_frame > g_drawHIO.mMiniGame.mGetInPikariAppearFrames) {
                        m_getin_info[i].pikari_frame =
                            18.0f - g_drawHIO.mMiniGame.mGetInPikariAnimSpeed;
                    }
                } else if (m_getin_info[i].bck_frame > g_drawHIO.mMiniGame.mStartPikariAppearFrames)
                {
                    m_getin_info[i].pikari_frame =
                        18.0f - g_drawHIO.mMiniGame.mStartPikariAnimSpeed;
                }
            }
        }
    }
}

bool dDlst_TimerScrnDraw_c::checkStartAnimeEnd() {
    if (m_getin_info[0].field_0xc == 1 && m_getin_info[0].pikari_frame == 0.0f) {
        return true;
    }

    return false;
}

void dDlst_TimerScrnDraw_c::playBckAnimation(f32 i_frame) {
    mpGetInParent->getPanePtr()->setAnimation(mpGetInBck);
    mpGetInBck->setFrame(i_frame);

    mpGetInParent->getPanePtr()->animationTransform();
    mpGetInParent->getPanePtr()->setAnimation((J2DAnmTransform*)NULL);
}

#if VERSION == VERSION_GCN_JPN
bool dDlst_TimerScrnDraw_c::isLeadByte(int i_char) {
    return (i_char >= 0x81 && i_char <= 0x9f) || (i_char >= 0xe0 && i_char <= 0xfc);
}
#endif

void dDlst_TimerScrnDraw_c::drawPikari(int i_no) {
    f32 temp_f28;
    f32 anim_frame;
    f32 var_f26;
    f32 var_f29;

    anim_frame = m_getin_info[i_no].pikari_frame;
    Vec center = mpGetInText->getGlobalVtxCenter(false, 0);
    char* string = static_cast<J2DTextBox*>(mpGetInText->getPanePtr())->getStringPtr();
    s16 str_idx = 0;

    var_f29 = 0.0f;
    var_f26 = 0.0f;
    J2DTextBox::TFontSize font_size;
    static_cast<J2DTextBox*>(mpGetInText->getPanePtr())->getFontSize(font_size);
    JUTFont* font_p = mDoExt_getMesgFont();

    f32 var_f25 = 1.0f;
    for (J2DPane* pane = mpGetInText->getPanePtr(); pane != NULL; pane = pane->getParentPane()) {
        var_f25 *= pane->getScaleX();
    }

    for (; string[str_idx] != 0; str_idx++) {
        int c = string[str_idx] & 0xFF;

        if (str_idx > 0) {
            var_f29 +=
                var_f25 * static_cast<J2DTextBox*>(mpGetInText->getPanePtr())->getCharSpace();
        }

#if VERSION == VERSION_GCN_JPN
        if (isLeadByte(c)) {
            c = ((string[str_idx] & 0xFF) << 8) | (string[str_idx + 1] & 0xFF);
            str_idx++;
        } else {
            c = string[str_idx] & 0xFF;
        }
#else
        c = string[str_idx] & 0xFF;
#endif

        var_f29 +=
            var_f25 * (font_size.mSizeX * ((f32)font_p->getWidth(c) / (f32)font_p->getCellWidth()));
    }

    temp_f28 = center.x - (var_f29 / 2);
    f32 frame;

    for (str_idx = 0; string[str_idx] != 0; str_idx++) {
        int c = string[str_idx] & 0xFF;

        if (str_idx > 0) {
            var_f26 +=
                var_f25 * static_cast<J2DTextBox*>(mpGetInText->getPanePtr())->getCharSpace();
        }

#if VERSION == VERSION_GCN_JPN
        if (isLeadByte(c)) {
            c = ((string[str_idx] & 0xFF) << 8) | (string[str_idx + 1] & 0xFF);
            str_idx++;
        } else {
            c = string[str_idx] & 0xFF;
        }
#else
        c = string[str_idx] & 0xFF;
#endif
        f32 temp_f24 =
            var_f25 * (font_size.mSizeX * ((f32)font_p->getWidth(c) / (f32)font_p->getCellWidth()));
        if (c != 0x20 && c != 0x8140) {
            frame = anim_frame;
            if (m_getin_info[i_no].field_0xc == 0) {
                dMeter2Info_getMeterClass()->getMeterDrawPtr()->drawPikari(
                    (temp_f24 / 2) + (temp_f28 + var_f26), center.y, &frame,
                    g_drawHIO.mMiniGame.mGetInPikariScale,
                    g_drawHIO.mMiniGame.mGetInPikariFrontOuter,
                    g_drawHIO.mMiniGame.mGetInPikariFrontInner,
                    g_drawHIO.mMiniGame.mGetInPikariBackOuter,
                    g_drawHIO.mMiniGame.mGetInPikariBackInner,
                    g_drawHIO.mMiniGame.mGetInPikariAnimSpeed, 0);
            } else {
                dMeter2Info_getMeterClass()->getMeterDrawPtr()->drawPikari(
                    (temp_f24 / 2) + (temp_f28 + var_f26), center.y, &frame,
                    g_drawHIO.mMiniGame.mStartPikariScale,
                    g_drawHIO.mMiniGame.mStartPikariFrontOuter,
                    g_drawHIO.mMiniGame.mStartPikariFrontInner,
                    g_drawHIO.mMiniGame.mStartPikariBackOuter,
                    g_drawHIO.mMiniGame.mStartPikariBackInner,
                    g_drawHIO.mMiniGame.mStartPikariAnimSpeed, 0);
            }
        }

        var_f26 += temp_f24;
    }

    m_getin_info[i_no].pikari_frame = frame;
}

static int dTimer_Draw(dTimer_c* i_this) {
    return i_this->_draw();
}

static int dTimer_Execute(dTimer_c* i_this) {
    return i_this->_execute();
}

static int dTimer_IsDelete(dTimer_c* i_this) {
    return 1;
}

static int dTimer_Delete(dTimer_c* i_this) {
    return i_this->_delete();
}

static int dTimer_Create(msg_class* i_this) {
    return static_cast<dTimer_c*>(i_this)->_create();
}

fpc_ProcID dTimer_createTimer(s32 i_mode, u32 i_limitMs, u8 i_type, u8 param_3, f32 param_4, f32 param_5,
                       f32 param_6, f32 param_7) {
    if (dComIfG_getTimerMode() == -1) {
        return fopMsgM_Timer_create(PROC_TIMER, i_mode, i_limitMs, i_type, param_3, param_4,
                                    param_5, param_6, param_7, NULL);
    }

    return -1;
}

s32 dTimer_createStockTimer() {
    if (dComIfG_getTimerMode() != -1) {
        if ((dComIfG_getTimerMode() == 3 || dComIfG_getTimerMode() == 4) &&
            strcmp(dComIfGp_getStartStageName(), "F_SP115"))
        {
            dComIfG_setTimerMode(-1);
            return -1;
        } else {
            u8 timer_type = dComIfG_getTimerType();
            return fopMsgM_Timer_create(PROC_TIMER, 10, 0, timer_type, 0, 221.0f, 439.0f, 32.0f,
                                        419.0f, NULL);
        }
    } else {
        return -1;
    }
}

u32 dTimer_createGetIn2D(s32 param_0, cXyz i_pos) {
    if (dComIfG_getTimerPtr() != NULL) {
        cXyz proj_pos;
        mDoLib_project(&i_pos, &proj_pos);
        param_0 = dComIfG_getTimerPtr()->createGetIn(proj_pos);
    } else {
        param_0 = 0;
    }

    return param_0;
}

static int dTimer_createStart2D(s32 param_0, u16 param_1) {
    if (dComIfG_getTimerPtr() != NULL) {
        param_0 = dComIfG_getTimerPtr()->createStart(param_1);
    } else {
        param_0 = 0;
    }

    return param_0;
}

bool dTimer_isStart() {
    if (dComIfG_getTimerPtr() != NULL) {
        return dComIfG_getTimerPtr()->isStart();
    }

    return false;
}

int dTimer_getRestTimeMs() {
    if (dComIfG_getTimerPtr() != NULL) {
        return dComIfG_getTimerPtr()->getRestTimeMs();
    }

    return 0;
}

void dTimer_show() {
    if (dComIfG_getTimerPtr() != NULL) {
        dComIfG_getTimerPtr()->show();
    }
}

void dTimer_hide() {
    if (dComIfG_getTimerPtr() != NULL) {
        dComIfG_getTimerPtr()->hide();
    }
}

u8 dTimer_isReadyFlag() {
    if (dComIfG_getTimerPtr() != NULL) {
        return dComIfG_getTimerPtr()->isReadyFlag();
    }

    return 0;
}

dDlst_TimerScrnDraw_c::~dDlst_TimerScrnDraw_c() {}

int dTimer_c::createGetIn(cXyz i_pos) {
    return mp_tm_scrn->createGetIn(i_pos);
}

static leafdraw_method_class l_dTimer_Method = {
    (process_method_func)dTimer_Create,  (process_method_func)dTimer_Delete,
    (process_method_func)dTimer_Execute, (process_method_func)dTimer_IsDelete,
    (process_method_func)dTimer_Draw,
};

msg_process_profile_definition g_profile_TIMER = {
    fpcLy_CURRENT_e,
    12,
    fpcPi_CURRENT_e,
    PROC_TIMER,
    &g_fpcLf_Method.base,
    sizeof(dTimer_c),
    0,
    0,
    &g_fopMsg_Method,
    768,
    &l_dTimer_Method,
};
