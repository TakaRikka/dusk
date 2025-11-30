#include "d/dolzel.h" // IWYU pragma: keep

#include "d/d_msg_out_font.h"
#include "JSystem/J2DGraph/J2DTextBox.h"
#include "JSystem/JUtility/JUTTexture.h"
#include "d/d_meter2_info.h"
#include "d/d_msg_object.h"
#include "f_op/f_op_msg_mng.h"

COutFontSet_c::COutFontSet_c() {
    initialize();
}

COutFontSet_c::~COutFontSet_c() {}

void COutFontSet_c::initialize() {
    mpTextBoxPtr = NULL;
    mPosX = 0.0f;
    mPosY = 0.0f;
    mSizeX = 1.0f;
    mSizeY = 1.0f;
    mAlpha = 255;
    mType = 0x47;
}

void COutFontSet_c::drawFont(J2DTextBox* i_textbox, u8 i_type, f32 i_posX, f32 i_posY, f32 i_sizeX,
                             f32 i_sizeY, u32 i_color, u8 i_alpha) {
    mpTextBoxPtr = i_textbox;
    mPosX = i_posX;
    mPosY = i_posY;
    mSizeX = i_sizeX;
    mSizeY = i_sizeY;
    mAlpha = i_alpha;
    mColor = i_color;
    mType = i_type;
}

COutFont_c::COutFont_c(u8 param_0) {
    field_0x242 = param_0;

    for (int i = 0; i < 35; i++) {
        mpOfs[i] = new COutFontSet_c();
    }

    for (int i = 0; i < 70; i++) {
        mpPane[i] = NULL;
        field_0x1b4[i] = 0;
    }

    mAlphaRatio = 0.0f;
    field_0x1b0 = 1.0f;
    field_0x1ac = 1.0f;
    field_0x240 = 0;
    mRupeeColor = 0;
}

COutFont_c::~COutFont_c() {
    for (int i = 0; i < 35; i++) {
        delete mpOfs[i];
        mpOfs[i] = NULL;
    }

    if (field_0x240) {
        field_0x240 = 0;

        for (int i = 0; i < 70; i++) {
            if (mpPane[i] != NULL) {
                delete mpPane[i];
                mpPane[i] = NULL;
            }
        }
    }

    if (dComIfGp_getExpHeap2D() == NULL) {
        dComIfGp_getItemIconArchive()->removeResourceAll();
    }
}

void COutFont_c::setPane(J2DPicture* i_pic) {
    for (int i = 0; i < 70; i++) {
        mpPane[i] = i_pic;
        i_pic++;
    }

    field_0x240 = false;
}

void COutFont_c::createPane() {
    ResTIMG* img;

    for (int i = 0; i < 70; i++) {
        if (i == 41) {
            img = (ResTIMG*)dComIfGp_getItemIconArchive()->getResource('TIMG', getBtiName(i));
        } else {
            img = (ResTIMG*)dComIfGp_getMain2DArchive()->getResource('TIMG', getBtiName(i));
        }

        mpPane[i] = new J2DPicture(img);
        switch (i) {
        case 0:
            mpPane[i]->setBlackWhite(JUtility::TColor(255, 255, 255, 0),
                                     JUtility::TColor(98, 163, 46, 255));
            break;
        case 1:
            mpPane[i]->setBlackWhite(JUtility::TColor(255, 255, 255, 0),
                                     JUtility::TColor(200, 39, 39, 255));
            break;
        case 2:
            mpPane[i]->setBlackWhite(JUtility::TColor(0, 0, 0, 0),
                                     JUtility::TColor(255, 200, 50, 255));
            break;
        case 3:
        case 4:
        case 5:
        case 6:
        case 8:
            if (field_0x242 == 0x1) {
                mpPane[i]->setBlackWhite(JUtility::TColor(255, 255, 255, 0),
                                         JUtility::TColor(120, 120, 120, 255));
            } else {
                mpPane[i]->setBlackWhite(JUtility::TColor(0, 0, 0, 0),
                                         JUtility::TColor(200, 200, 200, 255));
            }
            break;
        case 7:
            mpPane[i]->setBlackWhite(JUtility::TColor(255, 255, 255, 0),
                                     JUtility::TColor(80, 70, 165, 255));
            break;
        case 9:
        case 0xe:
        case 0xf:
        case 0x10:
        case 0x11:
        case 0x12:
        case 0x13:
            img = (ResTIMG*)dComIfGp_getMain2DArchive()->getResource('TIMG', "font_07_02.bti");
            mpPane[i]->append(img, 1.0f);
            mpPane[i]->setBlendRatio(1.0f, 0.0f);
            mpPane[i]->setBlackWhite(JUtility::TColor(0, 0, 0, 0),
                                     JUtility::TColor(200, 200, 200, 255));
            break;
        case 0x14:
            mpPane[i]->setBlackWhite(JUtility::TColor(0, 0, 0, 0),
                                     JUtility::TColor(220, 50, 50, 255));
            break;
        case 0x15:
            mpPane[i]->setBlackWhite(JUtility::TColor(0, 0, 0, 0),
                                     JUtility::TColor(255, 200, 50, 255));
            break;
        case 0x16:
            mpPane[i]->setBlackWhite(JUtility::TColor(0, 0, 0, 0),
                                     JUtility::TColor(255, 100, 0, 255));
            break;
        case 0x17:
            mpPane[i]->setBlackWhite(JUtility::TColor(0, 0, 0, 0),
                                     JUtility::TColor(0, 255, 180, 255));
            break;
        case 0x18:
            mpPane[i]->setBlackWhite(JUtility::TColor(0, 0, 0, 0),
                                     JUtility::TColor(255, 255, 255, 255));
            break;
        case 0x19:
            mpPane[i]->setBlackWhite(JUtility::TColor(0, 0, 0, 0),
                                     JUtility::TColor(170, 255, 255, 255));
            break;
        case 0x1a:
            mpPane[i]->setBlackWhite(JUtility::TColor(0, 0, 255, 0),
                                     JUtility::TColor(30, 50, 120, 255));
            break;
        case 0x1b:
            mpPane[i]->setBlackWhite(JUtility::TColor(0, 0, 0, 0),
                                     JUtility::TColor(255, 50, 50, 255));
            break;
        case 0x1c:
        case 0x2b:
            mpPane[i]->setBlackWhite(JUtility::TColor(0, 0, 0, 0),
                                     JUtility::TColor(255, 255, 255, 255));
            break;
        case 0x2a:
            mpPane[i]->setBlackWhite(JUtility::TColor(0, 0, 0, 0), JUtility::TColor(0, 0, 0, 255));
            break;
        case 0x1d:
            mpPane[i]->setBlackWhite(JUtility::TColor(0, 0, 0, 0),
                                     JUtility::TColor(255, 80, 80, 255));
            break;
        case 0x1e:
            mpPane[i]->setBlackWhite(JUtility::TColor(0, 0, 0, 0),
                                     JUtility::TColor(255, 255, 255, 255));
            field_0x1ac = (f32)img->width / (f32)img->height;
            field_0x1b0 = 1.0f;
            break;
        case 0x1f:
        case 0x20:
        case 0x21:
        case 0x22:
        case 0x23:
        case 0x24:
        case 0x25:
        case 0x26:
        case 0x27:
        case 0x28:
            mpPane[i]->setBlackWhite(JUtility::TColor(0, 0, 0, 0),
                                     JUtility::TColor(255, 255, 255, 255));
            break;
        case 0x29:
            mpPane[i]->setBlackWhite(JUtility::TColor(0, 0, 0, 0),
                                     JUtility::TColor(255, 255, 255, 255));
            break;
        case 0x2c:
        case 0x2d:
        case 0x2e:
        case 0x2f:
        case 0x37:
        case 0x38:
        case 0x39:
        case 0x3a:
        case 0x3d:
            mpPane[i]->setBlackWhite(JUtility::TColor(0, 0, 0, 0),
                                     JUtility::TColor(255, 255, 255, 255));
            break;
        case 0x30:
            img = (ResTIMG*)dComIfGp_getMain2DArchive()->getResource('TIMG', "font_20.bti");
            mpPane[i]->append(img, 1.0f);
            mpPane[i]->setBlendRatio(1.0f, 0.0f);
            mpPane[i]->setBlackWhite(JUtility::TColor(0, 0, 0, 0),
                                     JUtility::TColor(255, 255, 255, 255));
            break;
        case 0x31:
        case 0x32:
        case 0x41:
        case 0x42:
        case 0x43:
        case 0x44:
            img = (ResTIMG*)dComIfGp_getMain2DArchive()->getResource('TIMG', "font_21.bti");
            mpPane[i]->append(img, 1.0f);
            mpPane[i]->setBlendRatio(1.0f, 0.0f);
            mpPane[i]->setBlackWhite(JUtility::TColor(0, 0, 0, 0),
                                     JUtility::TColor(255, 255, 255, 255));
            break;
        case 0x33:
            mpPane[i]->setBlackWhite(JUtility::TColor(0, 0, 0, 0),
                                     JUtility::TColor(190, 190, 190, 255));
            break;
        case 0x34:
        case 0x35:
        case 0x36:
        case 0x3f:
        case 0x40:
            mpPane[i]->setBlackWhite(JUtility::TColor(0, 0, 0, 0),
                                     JUtility::TColor(255, 255, 255, 255));
            break;
        case 0x3b:
            img = (ResTIMG*)dComIfGp_getMain2DArchive()->getResource('TIMG', "font_30.bti");
            mpPane[i]->append(img, 1.0f);
            mpPane[i]->setBlendRatio(1.0f, 0.0f);
            mpPane[i]->setBlackWhite(JUtility::TColor(0, 0, 0, 0),
                                     JUtility::TColor(255, 255, 255, 255));
            break;
        case 0x3c:
            img = (ResTIMG*)dComIfGp_getMain2DArchive()->getResource('TIMG', "font_31.bti");
            mpPane[i]->append(img, 1.0f);
            mpPane[i]->setBlendRatio(1.0f, 0.0f);
            mpPane[i]->setBlackWhite(JUtility::TColor(0, 0, 0, 0),
                                     JUtility::TColor(255, 255, 255, 255));
            break;
        case 0x3e:
            img = (ResTIMG*)dComIfGp_getMain2DArchive()->getResource('TIMG', "font_34.bti");
            mpPane[i]->append(img, 1.0f);
            mpPane[i]->setBlendRatio(1.0f, 0.0f);
            mpPane[i]->setBlackWhite(JUtility::TColor(0, 0, 0, 0),
                                     JUtility::TColor(255, 255, 255, 255));
            break;
        case 0x45:
            mpPane[i]->setBlackWhite(JUtility::TColor(166, 12, 0, 0),
                                     JUtility::TColor(255, 255, 255, 255));
            break;
        }
    }

    field_0x240 = true;
}

void COutFont_c::initialize() {
    for (int i = 0; i < 35; i++) {
        mpOfs[i]->initialize();
    }
}

void COutFont_c::drawFont(J2DTextBox* i_textbox, u8 i_type, f32 i_posX, f32 i_posY, f32 i_sizeX,
                          f32 i_sizeY, u32 i_color, u8 i_alpha) {
#if VERSION != VERSION_GCN_JPN
    i_posY += 1.0f;
#endif
    for (int i = 0; i < 35; i++) {
        if (mpOfs[i]->getType() == 0x47) {
            mpOfs[i]->drawFont(i_textbox, i_type, i_posX, i_posY, i_sizeX, i_sizeY, i_color,
                               i_alpha);
            return;
        }
    }
}

void COutFont_c::setAlphaRatio(f32 i_ratio) {
    mAlphaRatio = i_ratio;
}

void COutFont_c::draw(J2DTextBox* i_textbox, f32 param_1, f32 param_2, f32 param_3) {
    s16 sp256[70];

    for (int i = 0; i < 70; i++) {
        sp256[i] = field_0x1b4[i];
    }

    for (int i = 0; i < 35; i++) {
        u8 type = mpOfs[i]->getType();
        J2DTextBox* tbox = mpOfs[i]->getTextBoxPtr();

        if (i_textbox == tbox && type != 0x47) {
            f32 scale_x = 1.0f;
            f32 scale_y = 1.0f;

            if (i_textbox != NULL) {
                for (J2DPane* pane = i_textbox; pane != NULL; pane = pane->getParentPane()) {
                    scale_x *= pane->getScaleX();
                    scale_y *= pane->getScaleY();
                }
            }

            f32 posX = param_1 + param_3 * (scale_x * mpOfs[i]->getPosX());
            f32 posY = param_2 + (scale_y * mpOfs[i]->getPosY());
            f32 sizeX = scale_x * mpOfs[i]->getSizeX();
            f32 sizeY = scale_y * mpOfs[i]->getSizeY();

            if (mpPane[type] != NULL) {
                mpPane[type]->setAlpha(mAlphaRatio * mpOfs[i]->getAlpha());

                switch (type) {
                case 10:
                    mpPane[type]->setBlackWhite(JUtility::TColor(0, 0, 0, 0),
                                                JUtility::TColor(0, 0, 0, 255));
                    mpPane[type]->draw(2.0f + posX, 2.0f + posY, sizeX, sizeY, false, false, true);

                    mpPane[type]->setBlackWhite(JUtility::TColor(0, 0, 0, 0), mpOfs[i]->getColor());
                    mpPane[type]->draw(posX, posY, sizeX, sizeY, false, false, true);
                    break;
                case 11:
                    mpPane[type]->setBlackWhite(JUtility::TColor(0, 0, 0, 0),
                                                JUtility::TColor(0, 0, 0, 255));
                    mpPane[type]->draw(2.0f + posX, 2.0f + posY, sizeX, sizeY, false, true, true);

                    mpPane[type]->setBlackWhite(JUtility::TColor(0, 0, 0, 0), mpOfs[i]->getColor());
                    mpPane[type]->draw(posX, posY, sizeX, sizeY, false, true, true);
                    break;
                case 29:
                    mpPane[type]->setBlackWhite(JUtility::TColor(0, 0, 0, 0),
                                                JUtility::TColor(0, 0, 0, 255));
                    mpPane[type]->draw(2.0f + posX, 2.0f + posY, sizeX, sizeY, false, true, true);

                    mpPane[type]->setBlackWhite(JUtility::TColor(0, 0, 0, 0),
                                                JUtility::TColor(255, 80, 80, 255));
                    mpPane[type]->draw(posX, posY, sizeX, sizeY, false, true, true);
                    break;
                case 12:
                    mpPane[type]->setBlackWhite(JUtility::TColor(0, 0, 0, 0),
                                                JUtility::TColor(0, 0, 0, 255));
                    mpPane[type]->draw(2.0f + posX, 2.0f + posY, sizeX, sizeY, false, true, false);

                    mpPane[type]->setBlackWhite(JUtility::TColor(0, 0, 0, 0), mpOfs[i]->getColor());
                    mpPane[type]->draw(posX, posY, sizeX, sizeY, false, true, false);
                    break;
                case 5:
                case 6: {
#if VERSION == VERSION_GCN_JPN
                    posY -= 2.0f;
                    sizeY -= 2.0f;
#else
                    posY += 1.0f;
                    sizeY -= 3.0f;
#endif
                    JUtility::TColor black = mpPane[type]->getBlack();
                    JUtility::TColor white = mpPane[type]->getWhite();

                    mpPane[type]->setBlackWhite(JUtility::TColor(0, 0, 0, 0),
                                                JUtility::TColor(0, 0, 0, 255));
                    mpPane[type]->draw(2.0f + posX, 2.0f + posY, sizeX, sizeY, false, false, false);

                    mpPane[type]->setBlackWhite(black, white);
                    mpPane[type]->draw(posX, posY, sizeX, sizeY, false, false, false);
                    break;
                }
                case 9:
                    if (sp256[type] == field_0x1b4[type]) {
                        field_0x1b4[type]++;

                        if (field_0x1b4[type] >= 80) {
                            field_0x1b4[type] = 0;
                        }
                        setBlendAnime(mpPane[type], field_0x1b4[type]);
                    }

                    if (field_0x1b4[type] < 20) {
                        mpPane[type]->draw(posX, posY, sizeX, sizeY, false, false, true);
                    } else if (field_0x1b4[type] < 40) {
                        mpPane[type]->draw(posX, posY, sizeX, sizeY, false, false, false);
                    } else if (field_0x1b4[type] < 60) {
                        mpPane[type]->draw(posX, posY, sizeX, sizeY, true, true, true);
                    } else {
                        mpPane[type]->draw(posX, posY, sizeX, sizeY, true, true, false);
                    }
                    break;
                case 14:
                    if (sp256[type] == field_0x1b4[type]) {
                        field_0x1b4[type]++;

                        if (field_0x1b4[type] >= 20) {
                            field_0x1b4[type] = 0;
                        }
                        setBlendAnime(mpPane[type], field_0x1b4[type]);
                    }
                    mpPane[type]->draw(posX, posY, sizeX, sizeY, false, false, true);
                    break;
                case 15:
                    if (sp256[type] == field_0x1b4[type]) {
                        field_0x1b4[type]++;

                        if (field_0x1b4[type] >= 20) {
                            field_0x1b4[type] = 0;
                        }
                        setBlendAnime(mpPane[type], field_0x1b4[type]);
                    }
                    mpPane[type]->draw(posX, posY, sizeX, sizeY, true, true, true);
                    break;
                case 16:
                    if (sp256[type] == field_0x1b4[type]) {
                        field_0x1b4[type]++;

                        if (field_0x1b4[type] >= 20) {
                            field_0x1b4[type] = 0;
                        }
                        setBlendAnime(mpPane[type], field_0x1b4[type]);
                    }
                    mpPane[type]->draw(posX, posY, sizeX, sizeY, false, false, false);
                    break;
                case 17:
                    if (sp256[type] == field_0x1b4[type]) {
                        field_0x1b4[type]++;

                        if (field_0x1b4[type] >= 20) {
                            field_0x1b4[type] = 0;
                        }
                        setBlendAnime(mpPane[type], field_0x1b4[type]);
                    }
                    mpPane[type]->draw(posX, posY, sizeX, sizeY, true, true, false);
                    break;
                case 18:
                    if (sp256[type] == field_0x1b4[type]) {
                        field_0x1b4[type]++;

                        if (field_0x1b4[type] >= 40) {
                            field_0x1b4[type] = 0;
                        }
                        setBlendAnime(mpPane[type], field_0x1b4[type]);
                    }

                    if (field_0x1b4[type] < 20) {
                        mpPane[type]->draw(posX, posY, sizeX, sizeY, false, false, true);
                    } else {
                        mpPane[type]->draw(posX, posY, sizeX, sizeY, true, true, true);
                    }
                    break;
                case 19:
                    if (sp256[type] == field_0x1b4[type]) {
                        field_0x1b4[type]++;

                        if (field_0x1b4[type] >= 40) {
                            field_0x1b4[type] = 0;
                        }
                        setBlendAnime(mpPane[type], field_0x1b4[type]);
                    }

                    if (field_0x1b4[type] < 20) {
                        mpPane[type]->draw(posX, posY, sizeX, sizeY, false, false, false);
                    } else {
                        mpPane[type]->draw(posX, posY, sizeX, sizeY, true, true, false);
                    }
                    break;
                case 13:
                    mpPane[type]->setBlackWhite(JUtility::TColor(0, 0, 0, 0),
                                                JUtility::TColor(0, 0, 0, 255));
                    mpPane[type]->draw(2.0f + posX, 2.0f + posY, sizeX, sizeY, false, false, false);

                    mpPane[type]->setBlackWhite(JUtility::TColor(0, 0, 0, 0), mpOfs[i]->getColor());
                    mpPane[type]->draw(posX, posY, sizeX, sizeY, false, false, false);
                    break;
                case 24:
                case 27:
                case 28:
                case 43:
                    mpPane[type]->setBlackWhite(JUtility::TColor(0, 0, 0, 0), mpOfs[i]->getColor());
                    mpPane[type]->draw(posX, posY, sizeX, sizeY, false, false, false);
                    break;
                case 42:
                    mpPane[type]->draw(posX, posY, sizeX, sizeY, false, false, false);
                    break;
                case 20:
                case 21:
                case 22:
                    field_0x1b4[type]++;
                    if (field_0x1b4[type] >= 28) {
                        field_0x1b4[type] = 0;
                    }

                    mpPane[type]->rotate(0.5f * sizeX, 0.5f * sizeY, ROTATE_Z,
                                         (360.0f * (f32)field_0x1b4[type]) / 28.0f);
                    mpPane[type]->draw(posX, posY, sizeX, sizeY, false, false, false);
                    break;
                case 25:  // some issues in here, 2020
                    if (sp256[type] == field_0x1b4[type]) {
                        field_0x1b4[type]++;

                        if (field_0x1b4[type] >= 18) {
                            field_0x1b4[type] -= 18;
                        }
                    }

                    f32 alpha;
                    if (field_0x1b4[type] < 9) {
                        alpha = 50.0f + 205.0f * (field_0x1b4[type] / 9.0f);
                    } else {
                        alpha = 50.0f + 205.0f * ((18.0f - field_0x1b4[type]) / 9.0f);
                    }

                    mpPane[26]->setAlpha(alpha * mAlphaRatio);
                    mpPane[26]->draw(
                        posX - 0.5f * ((g_MsgObject_HIO_c.mPortalIconScale - 1.0f) * sizeX),
                        posY - 0.5f * ((g_MsgObject_HIO_c.mPortalIconScale - 1.0f) * sizeY),
                        sizeX * g_MsgObject_HIO_c.mPortalIconScale,
                        sizeY * g_MsgObject_HIO_c.mPortalIconScale, false, false, false);
                    mpPane[type]->draw(posX, posY, sizeX, sizeY, false, false, false);
                    break;
                case 26:
                    mpPane[type]->draw(posX, posY, sizeX, sizeY, false, false, false);
                    break;
                case 30:
                    mpPane[type]->setBlackWhite(JUtility::TColor(0, 0, 0, 0),
                                                JUtility::TColor(255, 255, 255, 255));
                    mpPane[type]->draw(posX + (0.5f * (sizeX * (1.0f - field_0x1ac))),
                                       posY + (0.5f * (sizeY * (1.0f - field_0x1b0))),
                                       sizeX * field_0x1ac, sizeY * field_0x1b0, false, false,
                                       false);
                    break;
                case 31:
                case 32:
                case 33:
                case 34:
                case 35:
                case 36:
                case 37:
                case 38:
                case 39:
                case 40:
                    mpPane[type]->setBlackWhite(JUtility::TColor(0, 0, 0, 0), mpOfs[i]->getColor());
                    mpPane[type]->draw(posX, posY, sizeX, sizeY, false, false, false);
                    break;
                case 41:
                    mpPane[type]->draw(posX, posY, sizeX, sizeY, false, false, false);
                    break;
                case 44:
                    mpPane[type]->setBlackWhite(JUtility::TColor(0, 0, 0, 0),
                                                JUtility::TColor(0, 0, 0, 255));
                    mpPane[type]->draw(2.0f + posX, 2.0f + ((posY + -3.0f) - 4.0f), sizeX,
                                       sizeY * 1.25f, false, false, false);

                    mpPane[type]->setBlackWhite(JUtility::TColor(0, 0, 0, 0),
                                                JUtility::TColor(255, 255, 255, 255));
                    mpPane[type]->draw(posX, (posY + -3.0f) - 4.0f, sizeX, sizeY * 1.25f, false,
                                       false, false);
                    break;
                case 45:
                    mpPane[type]->draw(posX, (posY + -3.0f) - 4.0f, sizeX, sizeY * 1.25f, false,
                                       false, false);
                    break;
                case 46:
                case 47:
                case 55:
                case 56:
                case 57:
                case 58:
                case 61:
                case 69:
                    mpPane[type]->draw(posX, posY, sizeX, sizeY, false, false, false);
                    break;
                case 49:
                case 50:
                case 65:
                case 66:
                case 67:
                case 68:
                    if (sp256[type] == field_0x1b4[type]) {
                        field_0x1b4[type]++;

                        if (field_0x1b4[type] >= 40) {
                            field_0x1b4[type] = 0;
                        }
                        setBlendAnime(mpPane[type], field_0x1b4[type]);
                    }

                    mpPane[type]->setBlackWhite(JUtility::TColor(0, 0, 0, 0),
                                                JUtility::TColor(0, 0, 0, 255));
                    mpPane[type]->draw(2.0f + posX, 2.0f + posY, sizeX, sizeY, false, false, false);

                    mpPane[type]->setBlackWhite(JUtility::TColor(0, 0, 0, 0),
                                                JUtility::TColor(255, 255, 255, 255));
                    mpPane[type]->draw(posX, posY, sizeX, sizeY, false, false, false);
                    break;
                case 48:
                case 59:
                case 60:
                    if (sp256[type] == field_0x1b4[type]) {
                        field_0x1b4[type]++;

                        if (field_0x1b4[type] >= 40) {
                            field_0x1b4[type] = 0;
                        }
                        setBlendAnime(mpPane[type], field_0x1b4[type]);
                    }
                    mpPane[type]->draw(posX, posY, sizeX, sizeY, false, false, false);
                    break;
                case 62:
                    if (sp256[type] == field_0x1b4[type]) {
                        field_0x1b4[type]++;

                        if (field_0x1b4[type] >= 10) {
                            field_0x1b4[type] = 0;
                        }

                        if (field_0x1b4[type] < 5) {
                            mpPane[type]->setBlendRatio(1.0f, 0.0f);
                        } else {
                            mpPane[type]->setBlendRatio(0.0f, 1.0f);
                        }
                    }

                    mpPane[type]->draw(posX, -3.0f + posY, sizeX, 1.25f * sizeY, false, false,
                                       false);
                    break;
                default:
                    JUtility::TColor black = mpPane[type]->getBlack();
                    JUtility::TColor white = mpPane[type]->getWhite();

                    mpPane[type]->setBlackWhite(JUtility::TColor(0, 0, 0, 0),
                                                JUtility::TColor(0, 0, 0, 255));
                    mpPane[type]->draw(2.0f + posX, 2.0f + posY, sizeX, sizeY, false, false, false);

                    mpPane[type]->setBlackWhite(black, white);
                    mpPane[type]->draw(posX, posY, sizeX, sizeY, false, false, false);
                    break;
                }
            }
        }
    }
}

void COutFont_c::reset(J2DTextBox* i_textbox) {
    if (i_textbox != NULL) {
        for (int i = 0; i < 35; i++) {
            if (i_textbox == mpOfs[i]->getTextBoxPtr()) {
                mpOfs[i]->setTextBoxPtr(NULL);
                mpOfs[i]->resetType();
            }
        }
    }
}

void COutFont_c::setBlendAnime(J2DPicture* i_pic, s16 param_1) {
    int i = param_1 % 20;

    if (i < 10) {
        f32 dVar6 = fopMsgM_valueIncrease(10, i, 0);
        i_pic->setBlendRatio(1.0f - dVar6, dVar6);
    } else {
        f32 dVar6 = fopMsgM_valueIncrease(10, i - 10, 0);
        i_pic->setBlendRatio(dVar6, 1.0f - dVar6);
    }
}

const char* COutFont_c::getBtiName(int i_nameIdx) {
    static const char* mpIconName[] = {
        "font_00.bti",
        "font_01.bti",
        "font_09.bti",
        "font_04.bti",
        "font_05.bti",
        "font_02.bti",
        "font_03.bti",
        "font_06.bti",
        "font_08.bti",
        "font_07_01.bti",
        "font_10.bti",
        "font_10.bti",
        "font_10.bti",
        "font_10.bti",
        "font_07_01.bti",
        "font_07_01.bti",
        "font_07_01.bti",
        "font_07_01.bti",
        "font_07_01.bti",
        "font_07_01.bti",
        "font_15.bti",
        "font_15.bti",
        "font_15.bti",
        "font_12.bti",
        "font_15.bti",
        "im_map_icon_portal_4ia_40_05.bti",
        "font_16_backlight.bti",
        "font_13.bti",
        "font_14.bti",
        "font_10.bti",
        "im_zelda_item_icon_rupy_13.bti",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "st_bompoach_lv1.bti",
        "font_46.bti",
        "font_47.bti",
        "font_35.bti",
        "font_36.bti",
        "font_19.bti",
        "font_20.bti",
        "font_19.bti",
        "font_22.bti",
        "font_23.bti",
        "font_24.bti",
        "font_25.bti",
        "font_40.bti",
        "font_39.bti",
        "font_29.bti",
        "font_28.bti",
        "font_30.bti",
        "font_31.bti",
        "font_29.bti",
        "font_28.bti",
        "font_32.bti",
        "font_33.bti",
        "font_41.bti",
        "font_42.bti",
        "font_50.bti",
        "font_49.bti",
        "font_51.bti",
        "font_52.bti",
        "font_53.bti",
    };

    if (i_nameIdx >= 31 && i_nameIdx <= 40) {
        return dMeter2Info_getNumberTextureName(i_nameIdx - 31);
    }

    return mpIconName[i_nameIdx];
}
