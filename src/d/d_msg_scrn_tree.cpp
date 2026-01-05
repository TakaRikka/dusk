#include "d/dolzel.h" // IWYU pragma: keep

#include "d/d_msg_scrn_tree.h"
#include "JSystem/J2DGraph/J2DAnmLoader.h"
#include "JSystem/J2DGraph/J2DGrafContext.h"
#include "JSystem/J2DGraph/J2DScreen.h"
#include "JSystem/JKernel/JKRExpHeap.h"
#include "d/d_msg_object.h"
#include "d/d_msg_out_font.h"
#include "d/d_pane_class.h"

dMsgScrnTree_c::dMsgScrnTree_c(JUTFont* param_0, JKRExpHeap* param_1) {
    if (param_1 != NULL) {
        field_0xd8 = param_1;
    } else {
        field_0xd8 = dComIfGp_getSubHeap2D(7);
    }

    field_0xd8->getTotalFreeSize();
    init();

    if (param_0 == NULL) {
        field_0x54 = mDoExt_getMesgFont();
    } else {
        field_0x54 = param_0;
    }

    mpScreen = new J2DScreen();
    mpScreen->setPriority("zelda_kanban_wood_a.blo", 0x1020000, dComIfGp_getMsgArchive(2));
    dPaneClass_showNullPane(mpScreen);

    OSInitFastCast();

    field_0xcc = (J2DAnmTransform*)J2DAnmLoaderDataBase::load(
        JKRGetNameResource("zelda_kanban_wood_a.bck", dComIfGp_getMsgArchive(2)));

    field_0xd0 = (J2DAnmTextureSRTKey*)J2DAnmLoaderDataBase::load(
        JKRGetNameResource("zelda_kanban_wood_a.btk", dComIfGp_getMsgArchive(2)));
    field_0xd0->searchUpdateMaterialID(mpScreen);
    field_0xdc = 0.0f;

    field_0xd4 = (J2DAnmTextureSRTKey*)J2DAnmLoaderDataBase::load(
        JKRGetNameResource("zelda_kanban_wood_a_02.btk", dComIfGp_getMsgArchive(2)));
    field_0xd4->searchUpdateMaterialID(mpScreen);
    field_0xe0 = 0.0f;

    mpPmP_c = new CPaneMgr(mpScreen, 'n_size', 2, NULL);
    mpPmP_c->getPanePtr()->setAnimation(field_0xcc);
    field_0xcc->setFrame(1.0f);
    mpPmP_c->getPanePtr()->animationTransform();
    mpPmP_c->getPanePtr()->setAnimation((J2DAnmTransform*)NULL);
    mpPmP_c->scale(g_MsgObject_HIO_c.mBoxWoodScaleX, g_MsgObject_HIO_c.mBoxWoodScaleY);

    field_0xc4 = new CPaneMgr(mpScreen, 'back_b', 0, NULL);
    field_0xc8 = new CPaneMgr(mpScreen, 'spot00', 0, NULL);
    field_0xc8->getPanePtr()->setAnimation(field_0xd0);

    mpScreen->search('white_m')->setAnimation(field_0xd4);

#if VERSION == VERSION_GCN_JPN
    if (dComIfGs_getOptRuby() != 0) {
        for (int i = 0; i < 3; i++) {
            static u64 const t_tag[3] = {'mg_3line', 't3_w', 't3_s'};

            mpTm_c[i] = new CPaneMgr(mpScreen, t_tag[i], 0, NULL);
            ((J2DTextBox*)mpTm_c[i]->getPanePtr())->setFont(field_0x54);
            ((J2DTextBox*)mpTm_c[i]->getPanePtr())->setString(0x210, "");
        }

        mpScreen->search('n_3line')->show();
        mpScreen->search('n_3fline')->hide();
        mpScreen->search('n_e4line')->hide();
    } else {
        for (int i = 0; i < 3; i++) {
            static u64 const t_tag[3] = {'t3fline', 't3f_w', 't3f_s'};
            static u64 const tr_tag[3] = {'mg_3f', 'mg_3f_w', 'mg_3f_s'};

            mpTm_c[i] = new CPaneMgr(mpScreen, t_tag[i], 0, NULL);
            ((J2DTextBox*)mpTm_c[i]->getPanePtr())->setFont(field_0x54);
            ((J2DTextBox*)mpTm_c[i]->getPanePtr())->setString(0x210, "");

            mpTmr_c[i] = new CPaneMgr(mpScreen, tr_tag[i], 0, NULL);
            ((J2DTextBox*)mpTmr_c[i]->getPanePtr())->setFont(field_0x54);
            ((J2DTextBox*)mpTmr_c[i]->getPanePtr())->setString(0x210, "");
        }

        mpScreen->search('n_3line')->hide();
        mpScreen->search('n_3fline')->show();
        mpScreen->search('n_e4line')->hide();
    }
#else
    for (int i = 0; i < 3; i++) {
        static u64 const t_tag[3] = {'mg_e4lin', 'f4_w', 't4_s'};

        mpTm_c[i] = new CPaneMgr(mpScreen, t_tag[i], 0, NULL);
        ((J2DTextBox*)mpTm_c[i]->getPanePtr())->setFont(field_0x54);
        ((J2DTextBox*)mpTm_c[i]->getPanePtr())->setString(0x200, "");
    }

    mpScreen->search('n_3line')->hide();
    mpScreen->search('n_3fline')->hide();
    mpScreen->search('n_e4line')->show();
#endif

    ((J2DTextBox*)mpTm_c[0]->getPanePtr())->getFontSize(mFontSize);
    mTBoxWidth = mpTm_c[0]->getSizeX();
    mTBoxHeight = mpTm_c[0]->getSizeY();
    mLineSpace = ((J2DTextBox*)mpTm_c[0]->getPanePtr())->getLineSpace();
    mCharSpace = ((J2DTextBox*)mpTm_c[0]->getPanePtr())->getCharSpace();

    for (int i = 0; i < 3; i++) {
        ((J2DTextBox*)mpTm_c[i]->getPanePtr())->setLineSpace(mLineSpace);
        mpTm_c[i]->resize(1.2f * mpTm_c[i]->getSizeX(), mpTm_c[i]->getSizeY());
    }

    mTextBoxPosX = mpTm_c[0]->getGlobalPosX();
    mTextBoxPosY = mpTm_c[0]->getGlobalPosY();
    mTextBoxPosOffsetY = 0.0f;

    for (int i = 0; i < 3; i++) {
        if (mpTmr_c[i] != 0) {
            ((J2DTextBox*)mpTmr_c[i]->getPanePtr())->setLineSpace(mLineSpace);
            if (i == 0) {
                J2DTextBox::TFontSize aTStack_38;
                ((J2DTextBox*)mpTmr_c[0]->getPanePtr())->getFontSize(aTStack_38);
                mRubySize = aTStack_38.mSizeX;
                mRubyCharSpace = ((J2DTextBox*)mpTmr_c[0]->getPanePtr())->getCharSpace();
            }
            mpTmr_c[i]->resize(1.2f * mpTmr_c[i]->getSizeX(), mpTmr_c[i]->getSizeY());
        }
    }
}

dMsgScrnTree_c::~dMsgScrnTree_c() {
    delete mpScreen;
    mpScreen = NULL;

    delete field_0xcc;
    field_0xcc = NULL;

    delete field_0xd0;
    field_0xd0 = NULL;

    delete field_0xd4;
    field_0xd4 = NULL;

    delete mpPmP_c;
    mpPmP_c = NULL;

    delete field_0xc4;
    field_0xc4 = NULL;

    delete field_0xc8;
    field_0xc8 = NULL;

    for (int i = 0; i < 3; i++) {
        delete mpTm_c[i];
        mpTm_c[i] = NULL;

        if (mpTmr_c[i]) {
            delete mpTmr_c[i];
            mpTmr_c[i] = NULL;
        }
    }

    dComIfGp_getMsgArchive(2)->removeResourceAll();
}

void dMsgScrnTree_c::exec() {
    field_0xdc += 1.0f;
    if (field_0xdc >= field_0xd0->getFrameMax()) {
        field_0xdc -= field_0xd0->getFrameMax();
    }
    field_0xd0->setFrame(field_0xdc);

    field_0xe0 += 1.0f;
    if (field_0xe0 >= field_0xd4->getFrameMax()) {
        field_0xe0 -= field_0xd4->getFrameMax();
    }
    field_0xd4->setFrame(field_0xe0);
    mpScreen->animation();

    if (isTalkNow()) {
        fukiAlpha(1.0f);
    }
    mpPmP_c->scale(g_MsgObject_HIO_c.mBoxWoodScaleX, g_MsgObject_HIO_c.mBoxWoodScaleY);
}

void dMsgScrnTree_c::draw() {
    J2DGrafContext* grafContext = dComIfGp_getCurrentGrafPort();
    grafContext->setup2D();
    mpScreen->draw(0.0f, 0.0f, grafContext);
    mpOutFont->draw(NULL, 0.0f, 0.0f, 1.0f);
}

void dMsgScrnTree_c::fukiAlpha(f32 param_0) {
    field_0xc4->setAlphaRate(param_0);
    mpPmP_c->setAlphaRate(param_0 * g_MsgObject_HIO_c.mBoxWoodAlphaP);
    field_0xc8->setAlphaRate(param_0);

    for (int i = 0; i < 3; i++) {
        mpTm_c[i]->setAlphaRate(param_0 * mCharAlphaRate);
        if (mpTmr_c[i] != 0) {
            mpTmr_c[i]->setAlphaRate(param_0 * mCharAlphaRate);
        }
    }
}

void dMsgScrnTree_c::fukiScale(f32 param_0) {}

void dMsgScrnTree_c::fukiTrans(f32 param_0, f32 param_1) {}
