#include "d/dolzel.h" // IWYU pragma: keep

#include "d/d_msg_scrn_arrow.h"
#include "JSystem/J2DGraph/J2DAnmLoader.h"
#include "JSystem/J2DGraph/J2DScreen.h"
#include "d/d_com_inf_game.h"
#include "d/d_pane_class.h"

dMsgScrnArrow_c::dMsgScrnArrow_c() {
    mpScreen = new J2DScreen();
    JUT_ASSERT(0, mpScreen != NULL);
    bool fg =
        mpScreen->setPriority("zelda_window_yajirushi.blo", 0x20000, dComIfGp_getMsgArchive(0));
    JUT_ASSERT(0, fg != false);
    dPaneClass_showNullPane(mpScreen);

    OSInitFastCast();

    mpBck = (J2DAnmTransform*)J2DAnmLoaderDataBase::load(
        JKRGetNameResource("zelda_window_yajirushi.bck", dComIfGp_getMsgArchive(0)));
    mBckFrame = 0.0f;

    mpBpk = (J2DAnmColor*)J2DAnmLoaderDataBase::load(
        JKRGetNameResource("zelda_window_yajirushi.bpk", dComIfGp_getMsgArchive(0)));
    mpBpk->searchUpdateMaterialID(mpScreen);
    mBpkFrame = 0.0f;

    mpParent_c = new CPaneMgr(mpScreen, 'set_ya_n', 0, NULL);
    JUT_ASSERT(0, mpParent_c != NULL);

    mpArw_c = new CPaneMgr(mpScreen, 'ya_next', 0, NULL);
    JUT_ASSERT(0, mpArw_c != NULL);

    mpArw_c->hide();
    mpArw_c->mPane->setAnimation(mpBck);
    mpScreen->search('yajnext')->setAnimation(mpBpk);
    mpScreen->search('yajinexl')->setAnimation(mpBpk);

    mpDot_c = new CPaneMgr(mpScreen, 'ya_end', 0, NULL);
    JUT_ASSERT(0, mpDot_c != NULL);

    mpDot_c->hide();
    mpScreen->search('yaj_end')->setAnimation(mpBpk);
    mpScreen->search('yajiendl')->setAnimation(mpBpk);
}

dMsgScrnArrow_c::~dMsgScrnArrow_c() {
    delete mpScreen;
    mpScreen = NULL;

    delete mpBck;
    mpBck = NULL;

    delete mpBpk;
    mpBpk = NULL;

    delete mpParent_c;
    mpParent_c = NULL;

    delete mpArw_c;
    mpArw_c = NULL;

    delete mpDot_c;
    mpDot_c = NULL;
}

void dMsgScrnArrow_c::draw() {
    J2DGrafContext* graf_ctx = dComIfGp_getCurrentGrafPort();
    mpScreen->draw(0.0f, 0.0f, graf_ctx);
}

void dMsgScrnArrow_c::setPos(f32 i_posX, f32 i_posY) {
    mpParent_c->translate(i_posX, i_posY);
}

void dMsgScrnArrow_c::arwAnimeInit() {
    mpArw_c->hide();

    mBckFrame = 0.0f;
    mpBck->setFrame(mBckFrame);

    mBpkFrame = 0.0f;
    mpBpk->setFrame(mBpkFrame);

    mpScreen->animation();
}

void dMsgScrnArrow_c::arwAnimeMove() {
    if (!mpArw_c->isVisible()) {
        mpArw_c->show();
    }

    if (mpDot_c->isVisible() == true) {
        mpDot_c->hide();
    }

    mBckFrame += 1.0f;
    if (mBckFrame >= (f32)mpBck->getFrameMax()) {
        mBckFrame -= (f32)mpBck->getFrameMax();
    }
    mpBck->setFrame(mBckFrame);

    mBpkFrame += 1.0f;
    if (mBpkFrame >= (f32)mpBpk->getFrameMax()) {
        mBpkFrame -= (f32)mpBpk->getFrameMax();
    }
    mpBpk->setFrame(mBpkFrame);

    mpScreen->animation();
}

void dMsgScrnArrow_c::dotAnimeInit() {
    mpDot_c->hide();

    mBpkFrame = 0.0f;
    mpBpk->setFrame(mBpkFrame);

    mpScreen->animation();
}

void dMsgScrnArrow_c::dotAnimeMove() {
    if (mpArw_c->isVisible() == true) {
        mpArw_c->hide();
    }

    if (!mpDot_c->isVisible()) {
        mpDot_c->show();
    }

    mBpkFrame += 1.0f;
    if (mBpkFrame >= (f32)mpBpk->getFrameMax()) {
        mBpkFrame -= (f32)mpBpk->getFrameMax();
    }

    mpBpk->setFrame(mBpkFrame);
    mpScreen->animation();
}
