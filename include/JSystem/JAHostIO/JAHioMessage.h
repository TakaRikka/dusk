#ifndef JAHIOMESSAGE_H
#define JAHIOMESSAGE_H

#include "dolphin/types.h"

class JAHioNode;
class JORMContext;

class JAHControl {
public:
    JAHControl(JORMContext*, JAHioNode*);
    void returnY(u16);
    void indent(s8);
    void makeComment(const char*, u32, u8, u32);

    static u16 getLineHeight() { return smLineHeight; }
    static u16 getContWidth() { return smContWidth; }
    static u16 getIntervalX() { return smIntX; }
    static u16 getNameWidth() { return smNameWidth; }

    static u16 smButtonWidth[];
    static u16 smCommentWidth[];
    static u16 smComboWidth[];
    static u16 smYTop;
    static u16 smXLeft;
    static u16 smIndentSize;
    static u16 smLineHeight;
    static u16 smContWidth;
    static u16 smIntX;
    static u16 smIntY;
    static u16 smNameWidth;

    u16 getX() { return mX; }
    u16 getY() { return mY; }
    JORMContext* getContext() { return mContext; }

    void tabX(u16 param_1) {
        mX += param_1 + smIntX;
    }
    void returnYDirectSize(u16 param_1) {
        mY += param_1;
        mX = smXLeft + field_0x4 * smIndentSize;
    }

    /* 0x00 */ u16 mX;
    /* 0x02 */ u16 mY;
    /* 0x04 */ u16 field_0x4;
    /* 0x08 */ JORMContext* mContext;
    /* 0x0C */ JAHioNode* mNode;
};

#endif /* JAHIOMESSAGE_H */
