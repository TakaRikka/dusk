#ifndef D_JCAM_EDITOR_H
#define D_JCAM_EDITOR_H

#include "JSystem/JStudio/JStudioCameraEditor/control.h"
#include "JSystem/JStudio/JStudio_JStage/object-camera.h"
#include "JSystem/JUtility/JUTResFont.h"
#include "JSystem/JKernel/JKRExpHeap.h"

class JUTGamePad;

class dJcame_c {
public:
    dJcame_c(const JStage::TSystem*, f32, JUTGamePad&);
    ~dJcame_c();

    void update();
    void show3D(Mtx);
    void show2D();

    bool isEnabled() { return mControl->isEnabled(); }

    static void create(const JStage::TSystem*, f32, JUTGamePad&);
    static void remove();

    static dJcame_c* m_myObj;

    static dJcame_c* get() { return m_myObj; }

    /* 0x00 */ const JStage::TSystem* mSystem;
    /* 0x04 */ J2DOrthoGraph* mOrthoGraph;
    /* 0x08 */ JUTResFont* mFont;
    /* 0x0C */ JKRExpHeap* mHeap;
    /* 0x10 */ JStudioCameraEditor::TControl* mControl;
    /* 0x14 */ JStudio_JStage::TAdaptor_camera* mAdaptor;
    /* 0x18 */ bool mIsAdaptorSet;
    /* 0x19 */ s8 mHioId;
};

#endif /* D_JCAM_EDITOR_H */
