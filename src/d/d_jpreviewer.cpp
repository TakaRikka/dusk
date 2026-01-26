#include "d/dolzel.h" // IWYU pragma: keep

#include "d/d_jpreviewer.h"
#include "JSystem/JStudio/JStudio/jstudio-control.h"
#include "JSystem/J2DGraph/J2DOrthoGraph.h"
#include "JSystem/JUtility/JUTResFont.h"
#include "JSystem/JKernel/JKRHeap.h"
#include "JSystem/JKernel/JKRExpHeap.h"
#include "JSystem/JHostIO/JORServer.h"

class tParse_  : public JStudio::TParse {
public:
    tParse_() : JStudio::TParse(NULL) {}

    bool parseBlock_object(const JStudio::stb::data::TParse_TBlock_object& param_1, u32 param_2) {
        u32 type = param_1.get_type();
        switch(type) {
        case 'JMSG':
        case -1: {
            char acStack_20[10];
            JStudio::stb::data::toString_block(acStack_20, type);

            const void* id = param_1.get_ID();
            // Ignoring demo object: (%s) %s\n
            OS_REPORT("デモオブジェクト無視 : (%s) %s\n", acStack_20, id);
            return 1;
        }
        }

        return JStudio::stb::TParse::parseBlock_object(param_1, param_2);
    }
};

dJprev_c::dJprev_c(JStudio::TControl* param_1, const JUTGamePad& param_2) {
    mOrthoGraph = new J2DOrthoGraph(0.0f, 0.0f, 640.0f, 480.0f, -1.0f, 1.0f);
    JUT_ASSERT(72, mOrthoGraph != NULL)
    mFont = new JUTResFont((ResFONT*)JUTResFONT_Ascfont_fix12, NULL);
    JUT_ASSERT(74, mFont != NULL)
    m_parse = new tParse_();
    JUT_ASSERT(76, m_parse != NULL)
    mHeap = JKRExpHeap::create(0x100000, JKRHeap::getRootHeap2(), NULL);
    JUT_ASSERT(82, mHeap != NULL)
    mControl = new dJprevCtrl_c();
    JUT_ASSERT(86, mControl != NULL)
    mHIOId = mDoHIO_createChild("JStudioPreviewer", mControl);
    mControl->interface_setPad(&param_2);
    mControl->show_setRender(mOrthoGraph, mFont);
    mControl->jstudio_setControl(param_1);
    mControl->jstudio_setParse(m_parse);
    JOR_ENABLEEVENTCALLBACKLIST(1);
    m_myObj = this;
}

dJprev_c::~dJprev_c() {
    mDoHIO_deleteChild(mHIOId);
    delete mControl;
    mHeap->destroy();
    delete m_parse;
    delete mFont;
    delete mOrthoGraph;
    m_myObj = NULL;
}

void dJprev_c::create(JStudio::TControl* pControl, const JUTGamePad& pad) {
    JUT_ASSERT(130, m_myObj == NULL);
    new dJprev_c(pControl, pad);
    JUT_ASSERT(132, m_myObj != NULL);
}

void dJprev_c::remove() {
    JUT_ASSERT(147, m_myObj != NULL);
    delete m_myObj;
}

void dJprev_c::update() {
    mControl->update();
}

void dJprev_c::show3D(Mtx mtx) {
    mControl->show3D(mtx);
}

void dJprev_c::show2D() {
    mControl->show2D();
}
