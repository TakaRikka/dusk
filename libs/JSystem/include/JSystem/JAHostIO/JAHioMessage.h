#ifndef JAHIOMESSAGE_H
#define JAHIOMESSAGE_H

#include <JSystem/JHostIO/JORMContext.h>
#include <types.h>

class JAHioNode;
class JORMContext;
class JAHControl;

class JAHControl {
public:
    JAHControl(JORMContext*, JAHioNode*);
    void returnY(u16);
    void indent(s8);

    void makeComment(const char*, u32, u8, u32);
    void makeButtonL(const char*, u32, u16, u32);
    void makeCommentL(const char*, u32, u16, u32);
    void endComboBox();
    void startComboBoxL(const char*, s32*, u16, u16, u32);
    void makeSlider(const char*, s32*, s32, s32, u32);
    void makeSlider(const char*, s16*, s16, s16, u32);
    void makeSlider(const char*, f32*, f32, f32, u32);
    void makeButton(const char*, u32, u8, u32);
    void endRadioButton();
    void makeRadioButtonItem(const char*, u8);
    void startRadioButton(const char*, u8*, u8, u32);
    void makeCheckBox(const char*, u8*, u8, bool, u32);
    void makeSlider(const char*, u8*, u8, u8, u32);
    void makeComboBoxItem(const char*, u8);
    void makeComboBoxItem(const char*, s32);
    void startComboBox(const char*, u8*, u8, u32);

    static u16 getComboWidth(u8 itemNo) { return smComboWidth[itemNo]; }
    static u16 getLineHeight() { return smLineHeight; }
    static u32 getContWidth() { return smContWidth; }
    static u32 getIntervalX() { return smIntX; }
    static u32 getNameWidth() { return smNameWidth; }

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

    void tabX(u16 param_1) { mX += smIntX + param_1; }
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

// There's a funky dependency loop between JAHControl and JAHContTemplate, which must be resolved
// THIS way, as makeSlider etc functions must be defined NOT out-of-line for their string literal
// symbols to get the correct mangling.

class JAHContTemplate {
public:
    template <class T>
    static void startComboBox(JAHControl* param_1, const char* param_2, T* param_3, u16 param_4,
                              u16 param_5, u32 param_6) {
        if (param_2) {
            param_1->getContext()->genLabel(param_2, 0, param_6, NULL, param_1->getX(),
                                            param_1->getY(), param_4, param_1->getLineHeight());
            param_1->tabX(param_4);
        }

        param_1->getContext()->startComboBox(param_2, param_3, param_6, NULL, param_1->getX(),
                                             param_1->getY() - 7, param_5,
                                             param_1->getLineHeight());
        param_1->tabX(param_5);
    }

    template <class T>
    static void makeSlider(JAHControl* param_1, const char* param_2, T* param_3, T param_4,
                           T param_5, u32 param_6) {
        if (param_2) {
            param_1->getContext()->genLabel(param_2, 0, param_6, NULL, param_1->getX(),
                                            param_1->getY(), param_1->getNameWidth(),
                                            param_1->getLineHeight());
        }

        param_1->getContext()->genSlider(
            "---", param_3, param_4, param_5, param_6, NULL,
            (u32)param_1->getX() + param_1->getNameWidth() + param_1->getIntervalX(),
            param_1->getY() - 3, param_1->getContWidth(), param_1->getLineHeight());
        param_1->returnY(1);
    }

    template <class T>
    static void startRadioButton(JAHControl* param_1, const char* param_2, T* param_3, T param_4,
                                 u32 param_5) {
        if (param_2) {
            param_1->getContext()->genLabel(param_2, 0, param_5, NULL, param_1->getX(),
                                            param_1->getY(), param_1->getNameWidth() + 48,
                                            param_1->getLineHeight());
        }
        param_1->getContext()->startRadioButton(
            param_2, param_3, param_5, NULL,
            (u32)param_1->getX() + param_1->getNameWidth() + param_1->getIntervalX() + 48,
            param_1->getY(), param_1->getContWidth() + 250, param_4 * 18);

        param_1->returnYDirectSize(param_4 * 18);
    }

    template <class T>
    static void makeCheckBox(JAHControl* param_1, const char* param_2, T* param_3, T param_4,
                             bool param_5, u32 param_6) {
        if (!param_5) {
            param_1->getContext()->genCheckBox(param_2, param_3, param_4, param_6, NULL,
                                               param_1->getX(), param_1->getY(), 1000,
                                               param_1->getLineHeight());
        } else {
            param_1->getContext()->genCheckBox(param_2, param_3, param_4, param_6, NULL,
                                               param_1->getX() + param_1->getNameWidth() +
                                                   param_1->getIntervalX() + 52,
                                               param_1->getY(), 1000, param_1->getLineHeight());
        }
        param_1->returnY(1);
    }

    template <class T>
    static void startComboBox(JAHControl* param_1, const char* param_2, T* param_3, T param_4,
                              u32 param_5) {
        if (param_2) {
            param_1->getContext()->genLabel(param_2, 0, param_5, NULL, param_1->getX(),
                                            param_1->getY(), param_1->getNameWidth() + 48,
                                            param_1->getLineHeight());
        }

        param_1->getContext()->startComboBox(
            param_2, param_3, param_5, NULL,
            param_1->getX() + param_1->getNameWidth() + param_1->getIntervalX() + 48,
            param_1->getY() - 7, param_1->getComboWidth(param_4), param_1->getLineHeight());
    }
};

inline void JAHControl::makeButtonL(const char* param_1, u32 param_2, u16 param_3, u32 param_4) {
    mContext->genButton(param_1, param_2, param_4, NULL, mX, mY - 5, param_3, smLineHeight);
    tabX(param_3);
}

inline void JAHControl::makeCommentL(const char* param_1, u32 param_2, u16 param_3, u32 param_4) {
    mContext->genLabel(param_1, param_2, param_4, NULL, mX, mY, param_3, smLineHeight);
    tabX(param_3);
}

inline void JAHControl::endComboBox() {
    mContext->endComboBox();
}

inline void JAHControl::startComboBoxL(const char* param_1, s32* param_2, u16 param_3, u16 param_4,
                                       u32 param_5) {
    JAHContTemplate::startComboBox(this, param_1, param_2, param_3, param_4, param_5);
}

inline void JAHControl::makeSlider(const char* param_1, s32* param_2, s32 param_3, s32 param_4,
                                   u32 param_5) {
    JAHContTemplate::makeSlider(this, param_1, param_2, param_3, param_4, param_5);
}

inline void JAHControl::makeSlider(const char* param_1, s16* param_2, s16 param_3, s16 param_4,
                                   u32 param_5) {
    JAHContTemplate::makeSlider(this, param_1, param_2, param_3, param_4, param_5);
}

inline void JAHControl::makeSlider(const char* param_1, f32* param_2, f32 param_3, f32 param_4,
                                   u32 param_5) {
    JAHContTemplate::makeSlider(this, param_1, param_2, param_3, param_4, param_5);
}

inline void JAHControl::makeSlider(const char* param_1, u8* param_2, u8 param_3, u8 param_4,
                                   u32 param_5) {
    JAHContTemplate::makeSlider(this, param_1, param_2, param_3, param_4, param_5);
}

inline void JAHControl::makeButton(const char* param_1, u32 param_2, u8 param_3, u32 param_4) {
    mContext->genButton(param_1, param_2, param_4, NULL, mX, mY - 5, smButtonWidth[param_3],
                        smLineHeight);
    returnY(1);
}

inline void JAHControl::endRadioButton() {
    mContext->endRadioButton();
}

inline void JAHControl::makeRadioButtonItem(const char* param_1, u8 param_2) {
    mContext->genRadioButtonItem(param_1, param_2, 0, -1, -1, 128, 16);
}

inline void JAHControl::startRadioButton(const char* param_1, u8* param_2, u8 param_3,
                                         u32 param_4) {
    JAHContTemplate::startRadioButton(this, param_1, param_2, param_3, param_4);
}

inline void JAHControl::makeCheckBox(const char* param_1, u8* param_2, u8 param_3, bool param_4,
                                     u32 param_5) {
    JAHContTemplate::makeCheckBox(this, param_1, param_2, param_3, param_4, param_5);
}

inline void JAHControl::makeComboBoxItem(const char* label, u8 itemNo) {
    makeComboBoxItem(label, (s32)itemNo);
}

inline void JAHControl::makeComboBoxItem(const char* label, s32 itemNo) {
    mContext->genComboBoxItem(label, itemNo);
}

inline void JAHControl::startComboBox(const char* label, u8* param_2, u8 param_3, u32 param_4) {
    JAHContTemplate::startComboBox(this, label, param_2, param_3, param_4);
}

#endif /* JAHIOMESSAGE_H */
