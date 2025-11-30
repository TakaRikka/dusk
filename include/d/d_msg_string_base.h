#ifndef D_MSG_D_MSG_STRING_BASE_H
#define D_MSG_D_MSG_STRING_BASE_H

#include "JSystem/JMessage/resource.h"

class COutFont_c;
class J2DTextBox;
class JUTFont;
struct jmessage_string_tControl;
struct jmessage_string_tReference;
struct jmessage_string_tSequenceProcessor;
struct jmessage_string_tRenderingProcessor;

class dMsgStringBase_c {
public:
    dMsgStringBase_c();
    ~dMsgStringBase_c();
    void getResource();
    f32 getStringLocal(u32, J2DTextBox*, J2DTextBox*, JUTFont*, COutFont_c*, u8);
    f32 getStringPageLocal(u32, u8, u8, J2DTextBox*, J2DTextBox*, JUTFont*,
                                           COutFont_c*, u8);
    u8 getPageMax(int);
    f32 getMessageLocal(u32, char*);

    virtual f32 getString(u32, J2DTextBox*, J2DTextBox*, JUTFont*, COutFont_c*, u8);
    virtual f32 getStringPage(u32, u8, u8, J2DTextBox*, J2DTextBox*, JUTFont*, COutFont_c*,
                                      u8);
    virtual f32 getMessage(u32, char*);
    virtual void resetStringLocal(J2DTextBox*);
    virtual void drawOutFontLocal(J2DTextBox*, f32);
    virtual void drawFontLocal(J2DTextBox*, u8, f32, f32, f32, f32, u32, u8);

private:
    /* 0x04 */ JMessage::TResourceContainer* mpResCont;
    /* 0x08 */ jmessage_string_tControl* mpCtrl;
    /* 0x0C */ jmessage_string_tReference* mpRefer;
    /* 0x10 */ jmessage_string_tSequenceProcessor* mpSeqProc;
    /* 0x14 */ jmessage_string_tRenderingProcessor* mpRenProc;
    /* 0x18 */ JMessage::TParse* mpParse;
    /* 0x1C */ void* field_0x1c;
    /* 0x20 */ u16 field_0x20;
};

#endif /* D_MSG_D_MSG_STRING_BASE_H */
