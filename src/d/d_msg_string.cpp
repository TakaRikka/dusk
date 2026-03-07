#include "d/dolzel.h" // IWYU pragma: keep

#include "d/d_msg_string.h"
#include "JSystem/J2DGraph/J2DTextBox.h"
#include "d/d_msg_out_font.h"

dMsgString_c::dMsgString_c() {
    field_0x28 = 0;
    mpOutFont = new COutFont_c(field_0x28);
    mpOutFont->createPane();
    getResource();
}

dMsgString_c::dMsgString_c(u8 param_0) {
    field_0x28 = param_0;
    mpOutFont = new COutFont_c(field_0x28);
    mpOutFont->createPane();
    getResource();
}

dMsgString_c::~dMsgString_c() {
    delete mpOutFont;
    mpOutFont = NULL;
}

void dMsgString_c::resetStringLocal(J2DTextBox* p_textBox) {
    mpOutFont->reset(p_textBox);
}

void dMsgString_c::drawOutFontLocal(J2DTextBox* p_textBox, f32 i_alpha) {
    Vec pos = p_textBox->getGlbVtx(0);
    f32 y = pos.y;

    if (i_alpha == -1.0f) {
        i_alpha = p_textBox->getAlpha() / 255.0f;
    }

    mpOutFont->setAlphaRatio(i_alpha);
    mpOutFont->draw(p_textBox, pos.x, y, 1.0f);
}

void dMsgString_c::drawFontLocal(J2DTextBox* p_textBox, u8 type, f32 posX, f32 posY,
                              f32 sizeX, f32 sizeY, u32 color, u8 alpha) {
    mpOutFont->drawFont(p_textBox, type, posX, posY, sizeX, sizeY, color, alpha);
}
