#include "d/d_jcam_editor.h"
#include "m_Do/m_Do_hostIO.h"
#include "m_Do/m_Do_ext.h"

dJcame_c* dJcame_c::m_myObj;

dJcame_c::dJcame_c(const JStage::TSystem* i_system, f32 param_1, JUTGamePad& i_pad) {
    mOrthoGraph = new J2DOrthoGraph(0.0f, 0.0f, 608.0f, 448.0f, -1.0f, 1.0f);
    mFont = new JUTResFont((ResFONT*)JUTResFONT_Ascfont_fix12, NULL);
    
    mHeap = JKRExpHeap::create(0x100000, JKRHeap::getRootHeap2(), false);
    JUT_ASSERT(54, mHeap != NULL);

    mControl = new JStudioCameraEditor::TControl();
    mHioId = mDoHIO_createChild("JStudioCameraEditor", mControl);
    JUT_ASSERT(57, mControl != NULL);

    i_pad.setButtonRepeat((PAD_BUTTON_A | PAD_BUTTON_UP | PAD_BUTTON_DOWN | PAD_BUTTON_LEFT | PAD_BUTTON_RIGHT), 30, 5);
    mControl->interface_setPad(&i_pad);
    mControl->show_setRender(mOrthoGraph, mFont);
    mControl->jstudio_setSecondPerFrame(param_1);

    mAdaptor = NULL;
    mSystem = i_system;
    mIsAdaptorSet = false;

    m_myObj = this;
}

dJcame_c::~dJcame_c() {
    if (mAdaptor != NULL) {
        delete mAdaptor;
    }

    mControl->jstudio_setAdaptor(NULL);
    mDoHIO_deleteChild(mHioId);

    delete mControl;
    mHeap->destroy();
    delete mFont;
    delete mOrthoGraph;

    m_myObj = NULL;
}

void dJcame_c::create(const JStage::TSystem* i_system, f32 param_1, JUTGamePad& i_pad) {
    JUT_ASSERT(109, m_myObj == NULL);
    new dJcame_c(i_system, param_1, i_pad);
    JUT_ASSERT(111, m_myObj != NULL);
}

void dJcame_c::remove() {
    JUT_ASSERT(126, m_myObj != NULL);
    delete m_myObj;
}

void dJcame_c::update() {
    if (!mIsAdaptorSet) {
        if (mAdaptor == NULL) {
            JStage::TObject* object;
            int rt = mSystem->JSGFindObject(&object, "EditCam", JStage::OBJECT_CAMERA);
            if (object == NULL) {
                return;
            }

            mAdaptor = new JStudio_JStage::TAdaptor_camera(mSystem, (JStage::TCamera*)object);
            JUT_ASSERT(155, mAdaptor != NULL);

            mControl->jstudio_setAdaptor(mAdaptor);
            mIsAdaptorSet = true;
        }
    } else if (isEnabled()) {
        JKRHeap* heap = mDoExt_setCurrentHeap(mHeap);
        mControl->update();
        mDoExt_setCurrentHeap(heap);
    }
}

void dJcame_c::show3D(Mtx i_mtx) {
    if (mIsAdaptorSet) {
        mControl->show3D(i_mtx);
    }
}

void dJcame_c::show2D() {
    if (mIsAdaptorSet) {
        mControl->show2D();
    }
}
