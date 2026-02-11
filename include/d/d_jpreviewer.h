#ifndef D_JPREVIEWER_H
#define D_JPREVIEWER_H

#include <dolphin/mtx.h>
#include "JSystem/JStudio/JStudio_JPreviewer/control.h"

namespace JStudio {
    class TControl;
}

class J2DOrthoGraph;
class JKRExpHeap;
class JUTGamePad;
class JUTResFont;
class tParse_;

class dJprevCtrl_c  : public JStudioPreviewer::TControl {
public:

};

class dJprev_c {
public:
    dJprev_c(JStudio::TControl*, const JUTGamePad&);
    ~dJprev_c();

    void show3D(Mtx);
    void show2D();
    void create(JStudio::TControl* pControl, const JUTGamePad& pad);
    void remove();
    void update();

    static dJprev_c* get() { return m_myObj; }

    static dJprev_c* m_myObj;

    /* 0x00 */ J2DOrthoGraph* mOrthoGraph;
    /* 0x04 */ JUTResFont* mFont;
    /* 0x08 */ JKRExpHeap* mHeap;
    /* 0x0C */ tParse_* m_parse;
    /* 0x10 */ dJprevCtrl_c* mControl;
    /* 0x14 */ s8 mHIOId;
};

#endif /* D_JPREVIEWER_H */
