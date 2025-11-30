#include "d/dolzel.h" // IWYU pragma: keep

#include "d/d_msg_scrn_base.h"
#include "JSystem/J2DGraph/J2DGrafContext.h"
#include "JSystem/J2DGraph/J2DScreen.h"
#include "d/d_msg_object.h"
#include "d/d_msg_out_font.h"
#include "d/d_pane_class.h"
#include "cstring.h"

dMsgScrnBase_c::dMsgScrnBase_c() {
    init();
}

dMsgScrnBase_c::~dMsgScrnBase_c() {}

void dMsgScrnBase_c::init() {
    mpScreen = NULL;
    mpPmP_c = NULL;
    field_0x54 = 0;

    for (int i = 0; i < 7; i++) {
        mpTm_c[i] = NULL;
        mpTmr_c[i] = NULL;
    }

    mpOutFont = NULL;
    field_0x48 = NULL;
    field_0x4c = 0;
    mCharInfoPtr = NULL;

    mFontSize.mSizeX = mFontSize.mSizeY = 0.0f;
    mRubySize = 0.0f;
    mTextBoxPosX = 0.0f;
    mTextBoxPosY = 0.0f;
    mTextBoxScaleX = 1.0f;
    mTextBoxScaleY = 1.0f;
    mTBoxWidth = 0.0f;
    mTBoxHeight = 0.0f;
    mLineSpace = 0.0f;
    mCharSpace = 0.0f;
    mRubyCharSpace = 0.0f;
    mSelFontSize = 0.0f;
    mSelRubySize = 0.0f;

    for (int i = 0; i < 3; i++) {
        mSelTextBoxPosX[i] = 0.0f;
        mSelTextBoxPosY[i] = 0.0f;
    }

    mSelTBoxWidth = 0.0f;
    mSelCharSpace = 0.0f;
    mSelRubyCharSpace = 0.0f;
    mTextBoxPosOffsetY = 0.0f;
    field_0xb8 = 0.0f;
    mCharAlphaRate = 1.0f;
}

void dMsgScrnBase_c::multiDraw() {
    if (field_0x48 != NULL) {
        dComIfGd_set2DOpa(field_0x48);
    }
}

void dMsgScrnBase_c::draw() {
    J2DGrafContext* ctx = dComIfGp_getCurrentGrafPort();

    ctx->setup2D();
    mpScreen->draw(0.0f, 0.0f, ctx);
    drawSelf();
    ctx->setup2D();
}

void dMsgScrnBase_c::drawSelf() {
    drawOutFont(0.0f, 0.0f, 1.0f);
}

void dMsgScrnBase_c::drawOutFont(f32 param_0, f32 param_1, f32 param_2) {
    mpOutFont->draw(NULL, param_0, param_1, param_2);
}

void dMsgScrnBase_c::setString(char* mpText, char* i_stringB) {
    for (int i = 0; i < 7; i++) {
        if (mpTm_c[i] != NULL) {
            JUT_ASSERT(262, ((J2DTextBox*)(mpTm_c[i]->getPanePtr()))->getStringAllocByte() > strlen(mpText));
            if (i == 0) {
                strcpy(((J2DTextBox*)mpTm_c[i]->getPanePtr())->getStringPtr(), mpText);
            } else {
                strcpy(((J2DTextBox*)mpTm_c[i]->getPanePtr())->getStringPtr(), i_stringB);
            }
        }
    }
}

void dMsgScrnBase_c::setRubyString(char* mpText) {
    for (int i = 0; i < 3; i++) {
        if (mpTmr_c[i] != NULL) {
            JUT_ASSERT(288, ((J2DTextBox*)(mpTmr_c[i]->getPanePtr()))->getStringAllocByte() > strlen(mpText));
            strcpy(((J2DTextBox*)mpTmr_c[i]->getPanePtr())->getStringPtr(), mpText);
        }
    }
}

void dMsgScrnBase_c::fukiScale(f32 i_scale) {
    mpPmP_c->paneScale(i_scale, i_scale);
}

void dMsgScrnBase_c::fukiTrans(f32 i_posX, f32 i_posY) {
    mpPmP_c->paneTrans(i_posX, i_posY);
}

void dMsgScrnBase_c::fukiAlpha(f32 i_alpha) {
    fontAlpha(i_alpha);
    mpPmP_c->setAlphaRate(i_alpha);
}

void dMsgScrnBase_c::fontAlpha(f32 i_alpha) {
    for (int i = 0; i < 7; i++) {
        if (mpTm_c[i] != NULL) {
            mpTm_c[i]->setAlphaRate(i_alpha);
        }

        if (mpTmr_c[i] != NULL) {
            mpTmr_c[i]->setAlphaRate(i_alpha);
        }
    }
}

bool dMsgScrnBase_c::isTalkNow() {
    switch (dMsgObject_getMsgObjectClass()->getStatus()) {
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 15:
    case 16:
    case 20:
    case 24:
        return true;
    default:
        return false;
    }
}
