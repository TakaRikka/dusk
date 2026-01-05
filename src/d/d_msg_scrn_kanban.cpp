//
// d_msg_scrn_kanban
//

#include "d/dolzel.h" // IWYU pragma: keep

#include "d/d_msg_scrn_kanban.h"
#include "JSystem/J2DGraph/J2DAnmLoader.h"
#include "JSystem/J2DGraph/J2DGrafContext.h"
#include "JSystem/J2DGraph/J2DScreen.h"
#include "JSystem/JKernel/JKRExpHeap.h"
#include "d/d_msg_object.h"
#include "d/d_msg_out_font.h"
#include "d/d_pane_class.h"

dMsgScrnKanban_c::dMsgScrnKanban_c(JKRExpHeap* param_0) {
    if (param_0 != NULL) {
        field_0xd4 = param_0;
    } else {
        field_0xd4 = dComIfGp_getSubHeap2D(7);
    }

    s32 uVar14 = field_0xd4->getTotalFreeSize();
    init();

    mpScreen = new J2DScreen();
    JUT_ASSERT(43, mpScreen != NULL);
    bool fg =
        mpScreen->setPriority("zelda_kanban_stone_a.blo", 0x1020000, dComIfGp_getMsgArchive(2));
    JUT_ASSERT(45, fg != false);
    dPaneClass_showNullPane(mpScreen);

    OSInitFastCast();

    field_0xcc = (J2DAnmTransform*)J2DAnmLoaderDataBase::load(
        JKRGetNameResource("zelda_kanban_stone_a.bck", dComIfGp_getMsgArchive(2)));
    field_0xd0 = (J2DAnmTextureSRTKey*)J2DAnmLoaderDataBase::load(
        JKRGetNameResource("zelda_kanban_stone_a.btk", dComIfGp_getMsgArchive(2)));
    field_0xd0->searchUpdateMaterialID(mpScreen);
    field_0xd8 = 0.0f;

    mpPmP_c = new CPaneMgr(mpScreen, 'n_size', 2, NULL);
    JUT_ASSERT(60, mpPmP_c != NULL);
    mpPmP_c->getPanePtr()->setAnimation(field_0xcc);

    field_0xcc->setFrame(1.0f);
    mpPmP_c->getPanePtr()->animationTransform();
    mpPmP_c->getPanePtr()->setAnimation((J2DAnmTransform*)NULL);
    mpPmP_c->scale(g_MsgObject_HIO_c.mBoxStoneScaleX, g_MsgObject_HIO_c.mBoxStoneScaleY);

    mpBack_c = new CPaneMgr(mpScreen, 'back_b', 0, NULL);
    JUT_ASSERT(68, mpBack_c != NULL);

    mpSpot_c = new CPaneMgr(mpScreen, 'spot00', 0, NULL);
    JUT_ASSERT(72, mpSpot_c != NULL);

    mpSpot_c->getPanePtr()->setAnimation(field_0xd0);


#if VERSION == VERSION_GCN_JPN
    if (dComIfGs_getOptRuby() != 0) {
        static u64 const t_tag[3] = {'mg_3line', 't3_w', 't3_s'};
        for (int i = 0; i < 3; i++) {
            mpTm_c[i] = new CPaneMgr(mpScreen, t_tag[i], 0, NULL);
            ((J2DTextBox*)mpTm_c[i]->getPanePtr())->setFont(mDoExt_getMesgFont());
        }

        mpScreen->search('n_3line')->show();
        mpScreen->search('n_3fline')->hide();
        mpScreen->search('n_e4line')->hide();
    } else {
        static u64 const t_tag_2[3] = {'t3fline', 't3f_w', 't3f_s'};
        static u64 const tr_tag[3] = {'mg_3f', 'mg_3f_w', 'mg_3f_s'};

        for (int i = 0; i < 3; i++) {
            mpTm_c[i] = new CPaneMgr(mpScreen, t_tag_2[i], 0, NULL);
            ((J2DTextBox*)mpTm_c[i]->getPanePtr())->setFont(mDoExt_getMesgFont());

            mpTmr_c[i] = new CPaneMgr(mpScreen, tr_tag[i], 0, NULL);
            ((J2DTextBox*)mpTmr_c[i]->getPanePtr())->setFont(mDoExt_getMesgFont());
        }

        mpScreen->search('n_3line')->hide();
        mpScreen->search('n_3fline')->show();
        mpScreen->search('n_e4line')->hide();
    }
#else
    static u64 const t_tag[3] = {'mg_e4lin', 'f4_w', 't4_s'};

    for (int i = 0; i < 3; i++) {
        mpTm_c[i] = new CPaneMgr(mpScreen, t_tag[i], 0, NULL);
        JUT_ASSERT(81, mpTm_c[i] != NULL);
        ((J2DTextBox*)mpTm_c[i]->getPanePtr())->setFont(mDoExt_getMesgFont());
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
    for (int i = 0; i < 3; i = i + 1) {
        ((J2DTextBox*)mpTm_c[i]->getPanePtr())->setLineSpace(mLineSpace);
        mpTm_c[i]->resize(1.2f * mpTm_c[i]->getSizeX(), mpTm_c[i]->getSizeY());
    }

    mTextBoxPosX = mpTm_c[0]->getGlobalPosX();
    mTextBoxPosY = mpTm_c[0]->CPaneMgr::getGlobalPosY();
    mTextBoxPosOffsetY = 0.0f;

    for (int i = 0; i < 3; i++) {
        if (mpTmr_c[i] != NULL) {
            ((J2DTextBox*)mpTmr_c[i]->getPanePtr())->setLineSpace(mLineSpace);
            if (i == 0) {
                J2DTextBox::TFontSize fontSize;
                ((J2DTextBox*)mpTmr_c[0]->getPanePtr())->getFontSize(fontSize);
                mRubySize = fontSize.mSizeX;
                mRubyCharSpace = ((J2DTextBox*)mpTmr_c[0]->getPanePtr())->getCharSpace();
            }
            mpTmr_c[i]->resize(1.2f * mpTmr_c[i]->getSizeX(), mpTmr_c[i]->getSizeY());
        }
    }

    OS_REPORT("remain ===> %d\n", uVar14 - field_0xd4->getTotalFreeSize());
}

dMsgScrnKanban_c::~dMsgScrnKanban_c() {
    delete mpScreen;
    mpScreen = NULL;

    delete field_0xcc;
    field_0xcc = NULL;

    delete field_0xd0;
    field_0xd0 = NULL;

    delete mpPmP_c;
    mpPmP_c = NULL;

    delete mpBack_c;
    mpBack_c = NULL;

    delete mpSpot_c;
    mpSpot_c = NULL;

    for (int i = 0; i < 3; i++) {
        delete mpTm_c[i];
        mpTm_c[i] = NULL;
        if (mpTmr_c[i] != NULL) {
            delete mpTmr_c[i];
            mpTmr_c[i] = NULL;
        }
    }

    dComIfGp_getMsgArchive(2)->removeResourceAll();
}

void dMsgScrnKanban_c::exec() {
    field_0xd8 += 1.0f;
    if (field_0xd8 >= field_0xd0->getFrameMax()) {
        field_0xd8 -= field_0xd0->getFrameMax();
    }

    field_0xd0->setFrame(field_0xd8);
    mpScreen->animation();

    if (isTalkNow()) {
        fukiAlpha(1.0f);
    }

    mpPmP_c->scale(g_MsgObject_HIO_c.mBoxStoneScaleX, g_MsgObject_HIO_c.mBoxStoneScaleY);
}

void dMsgScrnKanban_c::draw() {
    J2DGrafContext* grafContext = dComIfGp_getCurrentGrafPort();
    grafContext->setup2D();
    mpScreen->draw(0.0f, 0.0f, grafContext);
    mpOutFont->draw(NULL, 0.0f, 0.0f, 1.0f);
}

void dMsgScrnKanban_c::fukiAlpha(f32 param_0) {
    mpBack_c->setAlphaRate(param_0);
    mpPmP_c->setAlphaRate(param_0 * g_MsgObject_HIO_c.mBoxStoneAlphaP);
    mpSpot_c->setAlphaRate(param_0);

    for (int i = 0; i < 3; i++) {
        mpTm_c[i]->setAlphaRate(param_0 * mCharAlphaRate);
        if (mpTmr_c[i] != NULL) {
            mpTmr_c[i]->setAlphaRate(param_0 * mCharAlphaRate);
        }
    }
}
void dMsgScrnKanban_c::fukiScale(f32 param_0) {}

void dMsgScrnKanban_c::fukiTrans(f32 param_0, f32 param_1) {}
