#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JAHostIO/JAHioMessage.h"
#include "JSystem/JAHostIO/JAHioNode.h"
#include "JSystem/JAHostIO/JAHioUtil.h"
#include "JSystem/JHostIO/JORMContext.h"

u16 JAHControl::smButtonWidth[] = {20, 50, 100, 150, 300, 600};
u16 JAHControl::smCommentWidth[] = {20, 50, 100, 200, 400, 800};
u16 JAHControl::smComboWidth[] = { 50, 100, 150, 200, 300, 600};
u16 JAHControl::smYTop = 5;
u16 JAHControl::smXLeft = 5;
u16 JAHControl::smIndentSize = 30;
u16 JAHControl::smLineHeight = 23;
u16 JAHControl::smContWidth = 450;
u16 JAHControl::smIntX = 2;
u16 JAHControl::smIntY = 5;
u16 JAHControl::smNameWidth = 150;

void JAHControl::returnY(u16 param_1) {
    mY += u16(smIntY + smLineHeight * param_1);
    mX = smXLeft + field_0x4 * smIndentSize;
}

void JAHControl::indent(s8 param_1) {
    field_0x4 += param_1;
    mX = smXLeft + field_0x4 * smIndentSize;
}

void JAHControl::makeComment(const char* label, u32 id, u8 param_3, u32 style) {
    mContext->genLabel(label, id, style, NULL, mX, mY, smCommentWidth[param_3], smLineHeight);
    returnY(1);
}

JAHControl::JAHControl(JORMContext* mctx, JAHioNode* node) {
    mContext = mctx;
    mNode = node;
    mX = smXLeft;
    mY = smYTop;
    field_0x4 = 0;
    char* name = node->getNodeName();
    if (name) {
        makeComment(JAHioUtil::getString("■■■ %s ■■■", name), 0xCCCC0000, 5, 0);
    } else {
        makeComment("■■■ NO NAMED NODE ■■■", 0, 5, 0);
    }
    indent(1);
    returnY(1);
}
