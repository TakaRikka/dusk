#ifndef JSYSTEM_JSTUDIO_JSTUDIO_JPREVIEWER_CONTROL_H
#define JSYSTEM_JSTUDIO_JSTUDIO_JPREVIEWER_CONTROL_H

#include "JSystem/JHostIO/JORReflexible.h"
#include "JSystem/JHostIO/JORServer.h"
#include "JSystem/JStudio/JStudioToolLibrary/interface.h"
#include "JSystem/JStudio/JStudioToolLibrary/visual.h"

class J2DOrthoGraph;
class JUTFont;
class JUTGamePad;

namespace JStudio {
    class TControl;
    class TParse;
};

namespace JStudioPreviewer {
    class TControl : public JORReflexible, public JOREventCallbackListNode {
    public:
        TControl();
        virtual ~TControl() {}
        void genMessage(JORMContext*);

        void interface_setPad(const JUTGamePad* pPad) {
            mPad.setPad(pPad);
        }

        void show_setRender(J2DOrthoGraph* pGraph, JUTFont* pFont) {
            field_0x128.setRender(pGraph);
            field_0x12c.setFont(pFont);
        }

        void jstudio_setControl(JStudio::TControl*);
        void jstudio_setParse(JStudio::TParse*);

        virtual void update();
        virtual void show2D();
        virtual void show3D(Mtx mtx);
        virtual int JORAct(u32, const char*);

    private:
        /* 0x018 */ u8 field_0x18[0xf8 - 0x18];
        /* 0x0F8 */ JStudioToolLibrary::TPad mPad;
        /* 0x018 */ u8 field_0xfc[0x128 - 0xfc];
        /* 0x128 */ JStudioToolLibrary::TDrawPrimitive2D field_0x128;
        /* 0x12C */ JStudioToolLibrary::TPrint field_0x12c;
    };
};

#endif
