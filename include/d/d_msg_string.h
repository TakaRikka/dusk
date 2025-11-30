#ifndef D_MSG_D_MSG_STRING_H
#define D_MSG_D_MSG_STRING_H

#include "d/d_msg_string_base.h"

class COutFont_c;
class JUTFont;

class dMsgString_c : public dMsgStringBase_c {
public:
    dMsgString_c();
    dMsgString_c(u8);
    ~dMsgString_c();

    virtual f32 getString(u32 param_0, J2DTextBox* param_1, J2DTextBox* param_2,
                                         JUTFont* param_3, COutFont_c* param_4, u8 param_5) {
        if (param_4 == NULL && mpOutFont != NULL) {
            return getStringLocal(param_0, param_1, param_2, param_3, mpOutFont, param_5);
        } else {
            return getStringLocal(param_0, param_1, param_2, param_3, param_4, param_5);
        }
    }
    virtual f32 getStringPage(u32 param_1, u8 param_2, u8 param_3,
                                             J2DTextBox* param_4, J2DTextBox* param_5,
                                             JUTFont* param_6, COutFont_c* param_7, u8 param_8) {
        if (param_7 == NULL && mpOutFont != NULL) {
            return getStringPageLocal(param_1, param_2, param_3, param_4, param_5, param_6,
                                      mpOutFont, param_8);
        } else {
            return getStringPageLocal(param_1, param_2, param_3, param_4, param_5, param_6, param_7,
                                      param_8);
        }
    }
    virtual void resetStringLocal(J2DTextBox*);
    virtual void drawOutFontLocal(J2DTextBox*, f32);
    virtual void drawFontLocal(J2DTextBox*, u8, f32, f32, f32, f32, u32, u8);

    void drawOutFont(J2DTextBox* param_0, f32 param_1) {
        drawOutFontLocal(param_0, param_1);
    }

private:
    /* 0x24 */ COutFont_c* mpOutFont;
    /* 0x28 */ u8 field_0x28;
};

#endif /* D_MSG_D_MSG_STRING_H */
