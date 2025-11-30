#include "d/dolzel.h" // IWYU pragma: keep

#include "d/d_msg_scrn_light.h"
#include "JSystem/J2DGraph/J2DAnmLoader.h"
#include "JSystem/J2DGraph/J2DGrafContext.h"
#include "JSystem/J2DGraph/J2DScreen.h"
#include "d/d_com_inf_game.h"
#include "d/d_pane_class.h"

class dMsgScrnLight_HIO_c {
public:
    dMsgScrnLight_HIO_c();
    void updateColor(u8);
    virtual ~dMsgScrnLight_HIO_c() {}

    enum {
        COLOR_DEFAULT_e,
        COLOR_RED_e,
        COLOR_GREEN_e,
        COLOR_BLUE_e,
        COLOR_YELLOW_e,
        COLOR_LIGHT_BLUE_e,
        COLOR_PURPLE_e,
        COLOR_GREY_e,
        COLOR_ORANGE_e,
        COLOR_MAX_e,
    };

    /* 0x04 */ s8 field_0x04;
    /* 0x05 */ bool mDebugON;
    /* 0x06 */ u8 mBlackR[COLOR_MAX_e];
    /* 0x0F */ u8 mWhiteR[COLOR_MAX_e];
    /* 0x18 */ u8 mBlackG[COLOR_MAX_e];
    /* 0x21 */ u8 mWhiteG[COLOR_MAX_e];
    /* 0x2A */ u8 mBlackB[COLOR_MAX_e];
    /* 0x33 */ u8 mWhiteB[COLOR_MAX_e];
    /* 0x3C */ u8 mBlackA[COLOR_MAX_e];
    /* 0x45 */ u8 mWhiteA[COLOR_MAX_e];
};

dMsgScrnLight_HIO_c::dMsgScrnLight_HIO_c() {
    mDebugON = false;

    for (int i = 0; i < COLOR_MAX_e; i++) {
        mBlackR[i] = 160;
        mBlackG[i] = 135;
        mBlackB[i] = 20;
        mBlackA[i] = 0;

        mWhiteR[i] = 225;
        mWhiteG[i] = 210;
        mWhiteB[i] = 110;
        mWhiteA[i] = 210;
    }
}

void dMsgScrnLight_HIO_c::updateColor(u8 i_colorType) {
    switch (i_colorType) {
    case 0:
        for (int i = 0; i < COLOR_MAX_e; i++) {
            mBlackR[i] = 160;
            mBlackG[i] = 135;
            mBlackB[i] = 20;
            mBlackA[i] = 0;

            mWhiteR[i] = 225;
            mWhiteG[i] = 210;
            mWhiteB[i] = 110;
            mWhiteA[i] = 160;
        }
        break;
    case 2:
        for (int i = 0; i < COLOR_MAX_e; i++) {
            mBlackR[i] = 255;
            mBlackG[i] = 250;
            mBlackB[i] = 150;
            mBlackA[i] = 0;

            mWhiteR[i] = 255;
            mWhiteG[i] = 255;
            mWhiteB[i] = 110;
            mWhiteA[i] = 210;
        }
        break;
    case 1:
        for (int i = 0; i < COLOR_MAX_e; i++) {
            mBlackR[i] = 40;
            mBlackG[i] = 110;
            mBlackB[i] = 180;
            mBlackA[i] = 0;

            mWhiteR[i] = 40;
            mWhiteG[i] = 110;
            mWhiteB[i] = 180;
            mWhiteA[i] = 120;
        }
        break;
    case 4:
        for (int i = 0; i < COLOR_MAX_e; i++) {
            mBlackR[i] = 70;
            mBlackG[i] = 150;
            mBlackB[i] = 0;
            mBlackA[i] = 0;

            mWhiteR[i] = 70;
            mWhiteG[i] = 150;
            mWhiteB[i] = 0;
            mWhiteA[i] = 150;
        }
        break;
    }
}

static dMsgScrnLight_HIO_c g_MsgScrnLight_HIO_c;

static u8 l_lightCount;

dMsgScrnLight_c::dMsgScrnLight_c(u8 i_colorType, u8 param_1) {
    mColorType = i_colorType;
    field_0x21 = param_1;

    if (l_lightCount == 0) {
        g_MsgScrnLight_HIO_c.field_0x04 = -1;
    }
    l_lightCount++;

    g_MsgScrnLight_HIO_c.updateColor(i_colorType);

    mpScreen = new J2DScreen();
    JUT_ASSERT(0, mpScreen != NULL);
    bool fg = mpScreen->setPriority("zelda_message_window_text_light.blo", 0x20000,
                                    dComIfGp_getMain2DArchive());
    JUT_ASSERT(0, fg != false);
    dPaneClass_showNullPane(mpScreen);

    OSInitFastCast();
    mpBck = (J2DAnmTransform*)J2DAnmLoaderDataBase::load(
        JKRGetNameResource("zelda_message_window_text_light.bck", dComIfGp_getMain2DArchive()));
    mBckFrame = 0.0f;

    mpBpk = (J2DAnmColorKey*)J2DAnmLoaderDataBase::load(
        JKRGetNameResource("zelda_message_window_text_light.bpk", dComIfGp_getMain2DArchive()));
    mpBpk->searchUpdateMaterialID(mpScreen);
    mBpkFrame = 0.0f;

    mpParent_c = new CPaneMgr(mpScreen, 'moya00', 0, NULL);
    JUT_ASSERT(0, mpParent_c != NULL);

    mpParent_c->getPanePtr()->setAnimation(mpBck);
    mpParent_c->getPanePtr()->setAnimation(mpBpk);
    mPlayAnim = true;
    mAlpha = 0.0f;
}

dMsgScrnLight_c::~dMsgScrnLight_c() {
    l_lightCount--;

    delete mpScreen;
    mpScreen = NULL;

    delete mpBck;
    mpBck = NULL;

    delete mpBpk;
    mpBpk = NULL;

    delete mpParent_c;
    mpParent_c = NULL;
}

void dMsgScrnLight_c::draw(f32* i_anmFrame, f32 i_posX, f32 i_posY, f32 i_scaleX, f32 i_scaleY,
                           f32 i_alpha, u8 i_colorType) {
    if (g_MsgScrnLight_HIO_c.mDebugON) {
        if (i_colorType < dMsgScrnLight_HIO_c::COLOR_MAX_e) {
            mpParent_c->setBlackWhite(JUtility::TColor(g_MsgScrnLight_HIO_c.mBlackR[i_colorType],
                                                       g_MsgScrnLight_HIO_c.mBlackG[i_colorType],
                                                       g_MsgScrnLight_HIO_c.mBlackB[i_colorType],
                                                       g_MsgScrnLight_HIO_c.mBlackA[i_colorType]),
                                      JUtility::TColor(g_MsgScrnLight_HIO_c.mWhiteR[i_colorType],
                                                       g_MsgScrnLight_HIO_c.mWhiteG[i_colorType],
                                                       g_MsgScrnLight_HIO_c.mWhiteB[i_colorType],
                                                       g_MsgScrnLight_HIO_c.mWhiteA[i_colorType]));
        } else {
            mpParent_c->setBlackWhite(mpParent_c->getInitBlack(), mpParent_c->getInitWhite());
        }
    } else {
        if (mColorType == 0) {
            mpParent_c->setBlackWhite(JUtility::TColor(160, 135, 20, 0),
                                      JUtility::TColor(225, 210, 110, 160));
        } else if (mColorType == 2) {
            mpParent_c->setBlackWhite(JUtility::TColor(255, 255, 150, 0),
                                      JUtility::TColor(255, 255, 110, 210));
        } else if (mColorType == 1) {
            mpParent_c->setBlackWhite(JUtility::TColor(40, 110, 180, 0),
                                      JUtility::TColor(40, 110, 180, 120));
        } else if (mColorType == 4) {
            mpParent_c->setBlackWhite(JUtility::TColor(70, 150, 0, 0),
                                      JUtility::TColor(70, 150, 0, 150));
        } else {
            mpParent_c->setBlackWhite(mpParent_c->getInitBlack(), mpParent_c->getInitWhite());
        }
    }

    if (mPlayAnim) {
        *i_anmFrame += 1.0f;
        if (*i_anmFrame >= mpBck->getFrameMax()) {
            *i_anmFrame = 0.0f;
        }

        mBckFrame = *i_anmFrame;
        mBpkFrame = *i_anmFrame;
    }

    drawCommon(i_posX, i_posY, i_scaleX, i_scaleY, i_alpha);
}

void dMsgScrnLight_c::draw(f32* i_anmFrame, f32 i_posX, f32 i_posY, f32 i_scaleX, f32 i_scaleY,
                           f32 i_alpha, f32 i_anmRate, JUtility::TColor i_black,
                           JUtility::TColor i_white) {
    mpParent_c->setBlackWhite(i_black, i_white);

    if (mPlayAnim) {
        *i_anmFrame += i_anmRate;

        if (*i_anmFrame >= mpBck->getFrameMax()) {
            *i_anmFrame = 0.0f;
        }

        mBckFrame = *i_anmFrame;
        mBpkFrame = *i_anmFrame;
    }

    drawCommon(i_posX, i_posY, i_scaleX, i_scaleY, i_alpha);
}

void dMsgScrnLight_c::drawCommon(f32 i_posX, f32 i_posY, f32 i_scaleX, f32 i_scaleY, f32 i_alpha) {
    J2DGrafContext* graf_ctx = dComIfGp_getCurrentGrafPort();
    graf_ctx->setup2D();

    f32 alpha_rate = mpParent_c->getAlphaRate();

    if (((field_0x21 == 3 || field_0x21 == 5) && i_alpha < mAlpha) ||
        (field_0x21 != 3 && field_0x21 != 5 && i_alpha != 1.0f))
    {
        if (mPlayAnim) {
            mpParent_c->getPanePtr()->setAnimation((J2DAnmTransform*)NULL);
            mpParent_c->getPanePtr()->setAnimation((J2DAnmColorKey*)NULL);
            mPlayAnim = false;
        }

        mpParent_c->setAlphaRate(alpha_rate * i_alpha);
    } else if (!mPlayAnim) {
        mpParent_c->getPanePtr()->setAnimation(mpBck);
        mpParent_c->getPanePtr()->setAnimation(mpBpk);
        mPlayAnim = true;
    }

    if (mPlayAnim) {
        mpBck->setFrame(mBckFrame);
        mpBpk->setFrame(mBpkFrame);
        mpParent_c->getPanePtr()->animationTransform();
        mpScreen->animation();
    }

    mpParent_c->translate(i_posX, i_posY);

    f32 init_scale_x = mpParent_c->getInitScaleX();
    f32 init_scale_y = mpParent_c->getInitScaleY();
    mpParent_c->scale(init_scale_x * i_scaleX, init_scale_y * i_scaleY);
    mpScreen->draw(0.0f, 0.0f, graf_ctx);

    if (i_alpha != 1.0f) {
        mpParent_c->setAlphaRate(alpha_rate);
    }

    mAlpha = i_alpha;
}
