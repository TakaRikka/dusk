/**
 * @file d_a_coach_2D.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_coach_2D.h"
#include "d/d_com_inf_game.h"
#include "d/d_meter2_info.h"
#include "d/d_msg_object.h"
#include "JSystem/J2DGraph/J2DGrafContext.h"
#include "JSystem/J2DGraph/J2DScreen.h"
#include "JSystem/J2DGraph/J2DAnmLoader.h"

class daCoach2D_HIO_c : public mDoHIO_entry_c {
public:
    struct Param {
        /* 0x00 */ f32 pos_x;
        /* 0x04 */ f32 pos_y;
        /* 0x08 */ f32 size;
        /* 0x0C */ f32 alpha;
        /* 0x10 */ f32 icon_space_x;
        /* 0x14 */ f32 icon_pos_x;
        /* 0x18 */ f32 icon_pos_y;
        /* 0x1C */ f32 icon_size;
        /* 0x20 */ f32 icon_alpha;
        /* 0x24 */ f32 coach_pos_x;
        /* 0x28 */ f32 coach_pos_y;
        /* 0x2C */ f32 coach_size;
        /* 0x30 */ f32 coach_alpha;
        /* 0x34 */ f32 blink_speed_min;
        /* 0x38 */ f32 blink_speed_max;
        /* 0x3C */ f32 pikari_scale;
        /* 0x40 */ f32 pikari_anim_speed;
        /* 0x44 */ u8 pikari_moyaR0_R;
        /* 0x45 */ u8 pikari_moyaR0_G;
        /* 0x46 */ u8 pikari_moyaR0_B;
        /* 0x47 */ u8 pikari_moyaR0_A;
        /* 0x48 */ u8 pikari_moyaR1_R;
        /* 0x49 */ u8 pikari_moyaR1_G;
        /* 0x4A */ u8 pikari_moyaR1_B;
        /* 0x4B */ u8 pikari_moyaR1_A;
        /* 0x4C */ u8 unk_0x4C;
        /* 0x4D */ u8 unk_0x4D;
        /* 0x4E */ u8 unk_0x4E;
        /* 0x4F */ u8 unk_0x4F;
        /* 0x50 */ u8 unk_0x50;
        /* 0x51 */ u8 unk_0x51;
        /* 0x52 */ u8 unk_0x52;
        /* 0x53 */ u8 unk_0x53;
        /* 0x54 */ u8 debug_ON;
        /* 0x55 */ u8 max_damage_num;
        /* 0x56 */ u8 now_damage_num;
        /* 0x57 */ u8 reverse_flag;
    };

    daCoach2D_HIO_c() {
        static Param aParam = {
            45.0f,
            -15.0f,
            1.0f,
            1.0f,
            7.5f,
            -6.5f,
            0.0f,
            0.5f,
            1.0f,
            0.0f,
            0.0f,
            1.0f,
            1.0f,
            1.0f,
            5.0f,
            3.0f,
            8.0f,
            0xFF, 0xFF, 0xFF, 0xB9,
            0x00, 0x9B, 0x00, 0x00,
            0xFF, 0xFF, 0xFF, 0x57,
            0x00, 0x73, 0x00, 0x00,
            false,
            20,
            0,
            1,
        };

        memcpy(&param, &aParam, sizeof(Param));
    }

    virtual ~daCoach2D_HIO_c() {}
    void genMessage(JORMContext*) {}

    /* 0x4 */ Param param;
};

void daCoach2D_c::c_list::draw() {
    dComIfGp_getCurrentGrafPort()->setup2D();
    mpCoach2D->drawMeter();
}

static int daCoach2D_createHeap(fopAc_ac_c* i_this) {
    return ((daCoach2D_c*)i_this)->createHeap();
}

static char* l_arcName = "Coach2D";

static daCoach2D_HIO_c l_HOSTIO;

int daCoach2D_c::createHeap() {
    mpScrn = new J2DScreen();
    if (mpScrn == NULL) {
        return 0;
    }

    dRes_info_c* resInfo = dComIfG_getObjectResInfo(l_arcName);
    JUT_ASSERT(285, resInfo != NULL);

    JKRArchive* archive = resInfo->getArchive();
    mpScrn->setPriority("zelda_game_image_fire_icon.blo", 0x20000, archive);
    dPaneClass_showNullPane(mpScrn);

    void* res = dComIfG_getObjectRes(l_arcName, "zelda_game_image_fire_icon.brk");
    mpFireIconBrk = (J2DAnmTevRegKey*)J2DAnmLoaderDataBase::load(res);
    mpFireIconBrk->searchUpdateMaterialID(mpScrn);
    setBrkAnime(true);

    mpPaneAll = new CPaneMgr(mpScrn, 'n_all', 2, NULL);
    mpPaneBasha = new CPaneMgr(mpScrn, 'basha_n', 2, NULL);
    mpPaneFire = new CPaneMgr(mpScrn, 'fire_n', 2, NULL);

    mpScrn->search('fire_b_n')->move(mpPaneFire->getPosX(), mpPaneFire->getPosY());

    mpPaneFireB = new CPaneMgr(mpScrn, 'fire_b_n', 2, NULL);

    mMsgLight = new dMsgScrnLight_c(6, 0xFF);

    if (mpPaneAll == NULL || mpPaneBasha == NULL || mpPaneFire == NULL || mpPaneFireB == NULL || mMsgLight == NULL) {
        return 0;
    }

    mList.set(this);

    if (fopAcM_GetParam(this) & 1) {
        show();
    } else {
        hide();
    }

    return 1;
}

int daCoach2D_c::create() {
    int phase_state = dComIfG_resLoad(this, l_arcName);
    if (phase_state == cPhs_COMPLEATE_e) {
        if (!fopAcM_entrySolidHeap(this, daCoach2D_createHeap, 0x5050)) {
            return cPhs_ERROR_e;
        }

#if DEBUG
        l_HOSTIO.entryHIO("馬車アイコン");
#endif
    }

    return phase_state;
}

int daCoach2D_c::destroy() {
    dComIfG_resDelete(this, l_arcName);
#if DEBUG
        l_HOSTIO.removeHIO();
#endif

    dMeter2Info_offSub2DStatus(0);
    return 1;
}

int daCoach2D_c::draw() {
    if (isVisible() && !dComIfGp_isPauseFlag() && !dMsgObject_isTalkNowCheck()) {
        dComIfGd_set2DOpa(&mList);
        dMeter2Info_onSub2DStatus(0);
    } else {
        dMeter2Info_offSub2DStatus(0);
    }

    return 1;
}

int daCoach2D_c::execute() {
#if DEBUG
    s32 freeSize = mDoExt_getCurrentHeap()->getTotalFreeSize();
#endif

    setBrkAnime(false);
    mpScrn->animation();

    JUT_ASSERT(426, freeSize == mDoExt_getCurrentHeap()->getTotalFreeSize());
    return 1;
}

void daCoach2D_c::drawMeter() {
    update();
    if (l_HOSTIO.param.reverse_flag) {
        mpPaneAll->getPanePtr()->rotateY(180.0f);
    } else {
        mpPaneAll->getPanePtr()->rotateY(0.0f);
    }

    for (int i = 0; i < mMaxHitCount; i++) {
        mpPaneFire->paneTrans(mIconInfo[i].pos_x, mIconInfo[i].pos_y);
        mpPaneFireB->paneTrans(mIconInfo[i].pos_x, mIconInfo[i].pos_y);

        if (i < mHitCount) {
            mpPaneBasha->hide();
            mpPaneFire->show();
            mpPaneFireB->hide();

            if (i == mHitCount - 1) {
                mpPaneFire->scale(1.0f, 1.0f);
            } else {
                mpPaneFire->scale(l_HOSTIO.param.icon_size, l_HOSTIO.param.icon_size);
            }

            mpScrn->draw(0.0f, 0.0f, dComIfGp_getCurrentGrafPort());

            if (mIconInfo[i].field_0x15 == 0xFF) {
                mIconInfo[i].pikari_anm_frame = 0.0f;
                mIconInfo[i].field_0x15 = 1;
            }

            if (mIconInfo[i].field_0x15 == 1) {
                Vec sp2C = mpPaneFire->getGlobalVtxCenter(false, 0);
                mIconInfo[i].pikari_pos_x = sp2C.x;
                mIconInfo[i].pikari_pos_y = sp2C.y;
            }
        } else {
            mpPaneBasha->hide();
            mpPaneFire->hide();
            mpPaneFireB->show();

            mpPaneFireB->scale(l_HOSTIO.param.icon_size, l_HOSTIO.param.icon_size);
            mpScrn->draw(0.0f, 0.0f, dComIfGp_getCurrentGrafPort());

            if (mIconInfo[i].field_0x15 == 0) {
                mIconInfo[i].field_0x15 = 0xFF;
            }
        }
    }

    mpPaneBasha->show();
    mpPaneFire->hide();
    mpPaneFireB->hide();

    mpPaneBasha->paneTrans(l_HOSTIO.param.coach_pos_x, l_HOSTIO.param.coach_pos_y);
    mpScrn->draw(0.0f, 0.0f, dComIfGp_getCurrentGrafPort());

    for (int i = 0; i < mMaxHitCount; i++) {
        if (mIconInfo[i].field_0x15 == 1) {
            mMsgLight->draw(&mIconInfo[i].pikari_anm_frame,
                              mIconInfo[i].pikari_pos_x, mIconInfo[i].pikari_pos_y,
                              l_HOSTIO.param.pikari_scale, l_HOSTIO.param.pikari_scale, 1.0f, l_HOSTIO.param.pikari_anim_speed,
                              JUtility::TColor(l_HOSTIO.param.pikari_moyaR1_R, l_HOSTIO.param.pikari_moyaR1_G, l_HOSTIO.param.pikari_moyaR1_B, l_HOSTIO.param.pikari_moyaR1_A),
                              JUtility::TColor(l_HOSTIO.param.pikari_moyaR0_R, l_HOSTIO.param.pikari_moyaR0_G, l_HOSTIO.param.pikari_moyaR0_B, l_HOSTIO.param.pikari_moyaR0_A));

            if (0.0f == mIconInfo[i].pikari_anm_frame) {
                mIconInfo[i].field_0x15 = 0;
            }
        }
    }
}

void daCoach2D_c::initiate() {
    for (int i = 0; i < 20; i++) {
        mIconInfo[i].field_0x15 = 0xFF;
    }

    mMaxHitCount = l_HOSTIO.param.max_damage_num;
}

void daCoach2D_c::update() {
    f32 var_f29 = l_HOSTIO.param.icon_space_x;
    f32 icon_pos_x = l_HOSTIO.param.icon_pos_x + ((mMaxHitCount - 1) * var_f29);
    f32 var_f28 = l_HOSTIO.param.icon_pos_y;

    for (int i = 0; i < 20; i++) {
        mIconInfo[i].pos_x = icon_pos_x;
        mIconInfo[i].pos_y = var_f28;
        icon_pos_x -= var_f29;
    }

    f32 var_f30 = 4.0f * ((J2DPicture*)mpScrn->search('basha_p0'))->getWhite().r;
    if (var_f30 > 255.0f) {
        var_f30 = 255.0f;
    }

    f32 temp_f27 = (70.0f + (185.0f * (var_f30 / 255.0f))) / 255.0f;
    mpPaneAll->paneTrans(l_HOSTIO.param.pos_x, l_HOSTIO.param.pos_y);
    mpPaneAll->scale(l_HOSTIO.param.size, l_HOSTIO.param.size);
    mpPaneAll->setAlphaRate(l_HOSTIO.param.alpha);
    mpPaneFire->setAlphaRate(temp_f27 * (l_HOSTIO.param.icon_alpha * l_HOSTIO.param.alpha));
    mpPaneFireB->setAlphaRate(l_HOSTIO.param.icon_alpha * l_HOSTIO.param.alpha);
    mpPaneBasha->scale(l_HOSTIO.param.coach_size, l_HOSTIO.param.coach_size);
    mpPaneBasha->setAlphaRate(l_HOSTIO.param.coach_alpha * l_HOSTIO.param.alpha);

    if (l_HOSTIO.param.debug_ON) {
        mMaxHitCount = l_HOSTIO.param.max_damage_num;
        mHitCount = l_HOSTIO.param.now_damage_num;
    }
}

void daCoach2D_c::setBrkAnime(bool param_0) {
    if (mHitCount != 0) {
        mBrkSpeed = l_HOSTIO.param.blink_speed_min + ((l_HOSTIO.param.blink_speed_max - l_HOSTIO.param.blink_speed_min) * ((f32)mHitCount / (f32)mMaxHitCount));
    } else {
        mBrkSpeed = 0.0f;
    }

    if (mBrkSpeed > 0.0f && !param_0) {
        mBrkFrame += mBrkSpeed;
        if (mBrkFrame >= mpFireIconBrk->getFrameMax()) {
            mBrkFrame -= mpFireIconBrk->getFrameMax();
        }
        mpScrn->search('basha_f0')->show();
    } else {
        mpScrn->search('basha_f0')->hide();
        mBrkFrame = 0.0f;
    }

    mpFireIconBrk->setFrame(mBrkFrame);
    mpScrn->search('basha_p0')->setAnimation(mpFireIconBrk);
}

static int daCoach2D_create(daCoach2D_c* i_this) {
    fopAcM_ct(i_this, daCoach2D_c);
    return i_this->create();
}

static int daCoach2D_destroy(daCoach2D_c* i_this) {
    return i_this->destroy();
}

static int daCoach2D_execute(daCoach2D_c* i_this) {
    return i_this->execute();
}

static int daCoach2D_draw(daCoach2D_c* i_this) {
    return i_this->draw();
}

static actor_method_class daCoach2D_METHODS = {
    (process_method_func)daCoach2D_create,
    (process_method_func)daCoach2D_destroy,
    (process_method_func)daCoach2D_execute,
    NULL,
    (process_method_func)daCoach2D_draw,
};

extern actor_process_profile_definition g_profile_COACH2D = {
  fpcLy_CURRENT_e,       // mLayerID
  3,                     // mListID
  fpcPi_CURRENT_e,       // mListPrio
  PROC_COACH2D,          // mProcName
  &g_fpcLf_Method.base, // sub_method
  sizeof(daCoach2D_c),   // mSize
  0,                     // mSizeOther
  0,                     // mParameters
  &g_fopAc_Method.base,  // sub_method
  764,                   // mPriority
  &daCoach2D_METHODS,    // sub_method
  0x00040000,            // mStatus
  fopAc_ACTOR_e,         // mActorType
  fopAc_CULLBOX_0_e,     // cullType
};
