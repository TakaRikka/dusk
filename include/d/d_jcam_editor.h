#ifndef D_JCAM_EDITOR_H
#define D_JCAM_EDITOR_H

#include "dolphin/mtx.h"

namespace JStage {
    class TSystem;
}
namespace JStudioCameraEditor {
    class TControl;
}

class JKRExpHeap;
class JUTGamePad;
class JUTResFont;

class dJcame_c {
public:
    dJcame_c(const JStage::TSystem*, f32, JUTGamePad&);

    void show3D(Mtx);
    void show2D();

    static void create(const JStage::TSystem*, f32, JUTGamePad&);

    static dJcame_c* m_myObj;

    static dJcame_c* get() { return m_myObj; }

    /* 0x00 */ JStage::TSystem* field_0x0;
    /* 0x04 */ JUTResFont* field_0x4;
    /* 0x08 */ JKRExpHeap* mHeap;
    /* 0x0C */ JStudioCameraEditor::TControl* mControl;
    /* 0x10 */ int field_0x10;
    /* 0x14 */ u8 field_0x14;
};

#endif /* D_JCAM_EDITOR_H */
