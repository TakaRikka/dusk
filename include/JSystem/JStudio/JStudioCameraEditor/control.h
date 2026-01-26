#ifndef JSTUDIOCAMERAEDITOR_CONTROL_H
#define JSTUDIOCAMERAEDITOR_CONTROL_H

#include "JSystem/JStudio/JStudio/jstudio-control.h"
#include "JSystem/JStudio/JStudio/jstudio-object.h"
#include "JSystem/JStudio/JStudioCameraEditor/csb.h"
#include "JSystem/JStudio/JStudioCameraEditor/sequence.h"
#include "JSystem/JStudio/JStudioCameraEditor/controlset-csb-valueset.h"
#include "JSystem/JStudio/JStudioToolLibrary/controlset-preview.h"
#include "JSystem/JStudio/JStudioToolLibrary/controlset-transform.h"
#include "JSystem/JStudio/JStudioToolLibrary/console.h"
#include "JSystem/JStudio/JStudioToolLibrary/interface.h"
#include "JSystem/JStudio/JStudioToolLibrary/visual.h"
#include "JSystem/JStudio/JStudioToolLibrary/scroll.h"
#include "JSystem/JGadget/std-vector.h"
#include "JSystem/JHostIO/JORServer.h"

namespace JStudioCameraEditor {
    struct TControl : public JORReflexible, public JOREventCallbackListNode {
        struct TSWValueWidth_ {
            TSWValueWidth_(uint, uint);

            /* 0x0 */ uint field_0x0;
            /* 0x4 */ uint field_0x4;
        };

        struct TOptionSet_ : public csb::TValueSet {
            TOptionSet_();

            int getPadding_vertical_() const;
            int getPadding_horizontal_() const;
            int getMargin_horizontal_() const;

            /* 0x02C */ int field_0x2c;
            /* 0x030 */ int field_0x30;
            /* 0x034 */ u8 field_0x34;
            /* 0x035 */ u8 field_0x35;
            /* 0x038 */ int field_0x38;
            /* 0x03C */ int field_0x3c;
            /* 0x040 */ int field_0x40;
            /* 0x044 */ u8 field_0x44;
            /* 0x045 */ u8 field_0x45;
            /* 0x048 */ f32 field_0x48;
            /* 0x04C */ f32 field_0x4c;
            /* 0x050 */ f32 field_0x50;
            /* 0x054 */ f32 field_0x54;
            /* 0x058 */ f32 field_0x58;
            /* 0x05C */ f32 field_0x5c;
            /* 0x060 */ f32 field_0x60;
            /* 0x064 */ f32 field_0x64;
            /* 0x068 */ f32 field_0x68;
            /* 0x06C */ f32 field_0x6c;
            /* 0x070 */ int field_0x70;
            /* 0x074 */ int field_0x74;
            /* 0x078 */ int field_0x78;
            /* 0x07C */ u8 field_0x7c;
            /* 0x07D */ u8 field_0x7d;
            /* 0x07E */ u8 field_0x7e;
            /* 0x07F */ u8 field_0x7f;
            /* 0x080 */ u8 field_0x80;
            /* 0x081 */ u8 field_0x81;
            /* 0x082 */ u8 field_0x82;
            /* 0x083 */ u8 field_0x83;
            /* 0x084 */ u8 field_0x84;
            /* 0x088 */ int field_0x88;
            /* 0x08C */ int field_0x8c;
            /* 0x090 */ int field_0x90;
            /* 0x094 */ int field_0x94;
            /* 0x098 */ f32 field_0x98;
            /* 0x09C */ f32 field_0x9c;
            /* 0x0A0 */ JUtility::TColor field_0xa0;
            /* 0x0A4 */ int field_0xa4;
            /* 0x0A8 */ int field_0xa8;
            /* 0x0AC */ int field_0xac;
            /* 0x0B0 */ f32 field_0xb0;
            /* 0x0B4 */ int field_0xb4;
            /* 0x0B8 */ int field_0xb8;
            /* 0x0BC */ int field_0xbc;
            /* 0x0C0 */ int field_0xc0;
            /* 0x0C4 */ int field_0xc4;
            /* 0x0C8 */ int field_0xc8;
            /* 0x0CC */ int field_0xcc;
            /* 0x0D0 */ int field_0xd0;
            /* 0x0D4 */ int field_0xd4;
            /* 0x0D8 */ int field_0xd8;
            /* 0x0DC */ int field_0xdc;
            /* 0x0E0 */ int field_0xe0;
            /* 0x0E4 */ int field_0xe4;
            /* 0x0E8 */ int field_0xe8;
            /* 0x0EC */ TSWValueWidth_ field_0xec;
            /* 0x0F4 */ TSWValueWidth_ field_0xf4;
            /* 0x0FC */ TSWValueWidth_ field_0xfc;
            /* 0x104 */ TSWValueWidth_ field_0x104;
            /* 0x10C */ JUtility::TColor field_0x10c;
            /* 0x110 */ JUtility::TColor field_0x110;
            /* 0x114 */ JUtility::TColor field_0x114;
            /* 0x118 */ JUtility::TColor field_0x118;
            /* 0x11C */ JUtility::TColor field_0x11c;
            /* 0x120 */ JUtility::TColor field_0x120;
            /* 0x124 */ JUtility::TColor field_0x124;
            /* 0x128 */ JUtility::TColor field_0x128;
            /* 0x12C */ JUtility::TColor field_0x12c;
            /* 0x130 */ JUtility::TColor field_0x130;
            /* 0x134 */ JUtility::TColor field_0x134;
            /* 0x138 */ JUtility::TColor field_0x138;
            /* 0x13C */ JUtility::TColor field_0x13c;
            /* 0x140 */ JUtility::TColor field_0x140;
            /* 0x144 */ JUtility::TColor field_0x144;
            /* 0x148 */ JStudioToolLibrary::TColor_variable::TVelocity field_0x148;
            /* 0x158 */ JStudioToolLibrary::TColor_variable::TVelocity field_0x158;
            /* 0x168 */ JStudioToolLibrary::TColor_variable::TVelocity field_0x168;
            /* 0x178 */ JStudioToolLibrary::TColor_variable::TVelocity field_0x178;
            /* 0x188 */ JStudioToolLibrary::TColor_variable::TVelocity field_0x188;
            /* 0x198 */ JStudioToolLibrary::TColor_variable::TVelocity field_0x198;
            /* 0x1A8 */ int field_0x1a8;
            /* 0x1AC */ int field_0x1ac;
        };

        struct TAdaptor_preview_ : public JStudioToolLibrary::TControlSet_preview::TAdaptor {
            TAdaptor_preview_(TControl*);

            virtual bool do_isReady();
            virtual void do_begin();
            virtual void do_end();
            virtual void do_current(u32, s32);
            virtual ~TAdaptor_preview_();

            /* 0x4 */ TControl* pControl_;
        };

        enum TECommand {};

        TControl();

        void jstudio_setSecondPerFrame(double param_0) {
            oJSTControl_.setSecondPerFrame(param_0);
        }
    
        void show_setRender(J2DOrthoGraph* pOrthoGraph, JUTFont* pFont) {
            field_0x3cc.setRender(pOrthoGraph);
            oPrint_.setFont(pFont);
        }

        void interface_setPad(const JUTGamePad* pPad) {
            oPad_.setPad(pPad);
        }
    
        void jstudio_setAdaptor(JStudio::TAdaptor_camera* pAdaptor) {
            field_0x608.setAdaptor(pAdaptor);
        }

        bool isEnabled() const { return bEnabled_; }

        virtual void listenPropertyEvent(const JORPropertyEvent*);
        virtual void genMessage(JORMContext*);

        virtual int JORAct(u32, const char*);
        virtual ~TControl();
        virtual void update();
        virtual void show2D();
        virtual void show3D(const Mtx);
        virtual int command(TECommand, const char*);

        /* 0x018 */ JStudioToolLibrary::TConsole field_0x18;
        /* 0x020 */ TOptionSet_ oOptionSet_;
        /* 0x1D0 */ TAdaptor_preview_ field_0x1d0;
        /* 0x1D8 */ bool bEnabled_;
        /* 0x1DC */ int field_0x1dc;
        /* 0x1E0 */ JStudioToolLibrary::TControlSet_preview oPreviewControl_;
        /* 0x218 */ TControlSet_csb_valueSet field_0x218;
        /* 0x260 */ TControlSet_csb_valueSet field_0x260;
        /* 0x2A8 */ JStudioToolLibrary_JStudio::TControlSet_transform oTransformControl_;
        /* 0x2D4 */ TSequenceContainer oSequenceContainer_;
        /* 0x2E8 */ csb::TValueSet field_0x2e8;
        /* 0x314 */ JGadget::TVector<csb::TValueSet, JGadget::TAllocator<csb::TValueSet> > field_0x314;
        /* 0x328 */ TSequenceContainer field_0x328;
        /* 0x33C */ u8 field_0x33C[0x36C - 0x33C];
        /* 0x36C */ int field_0x36c;
        /* 0x370 */ u8 field_0x370;
        /* 0x371 */ u8 field_0x371;
        /* 0x372 */ u8 field_0x372;
        /* 0x373 */ u8 field_0x373;
        /* 0x374 */ u8 field_0x374;
        /* 0x378 */ int field_0x378;
        /* 0x37C */ u8 field_0x37c;
        /* 0x380 */ int field_0x380;
        /* 0x384 */ int field_0x384[2];
        /* 0x38C */ JGadget::TLinkList<csb::TSequence, 0>::iterator itIFSequence_;
        /* 0x390 */ u32 uitIFSequence_;
        /* 0x394 */ csb::TValueSet* itIFValueSet_;
        /* 0x398 */ csb::TValueSet* uitIFValueSet_;
        /* 0x39C */ JStudioToolLibrary::TPad oPad_;
        /* 0x3A0 */ JStudioToolLibrary::TPad::TData oPadData_;
        /* 0x3C8 */ bool bIsPad_applied_;
        /* 0x3CC */ JStudioToolLibrary::TDrawPrimitive2D field_0x3cc;
        /* 0x3D0 */ JStudioToolLibrary::TDrawPrimitive3D field_0x3d0;
        /* 0x3D8 */ JStudioToolLibrary::TPrint oPrint_;
        /* 0x3EC */ bool bVisible2D_;
        /* 0x3ED */ bool bVisible3D_;
        /* 0x3F0 */ JStudioToolLibrary::TScroll_drawBar_horizontal field_0x3f0;
        /* 0x428 */ JStudioToolLibrary::TScroll_drawBar_horizontal field_0x428;
        /* 0x460 */ int field_0x460;
        /* 0x464 */ int field_0x464;
        /* 0x468 */ JStudioToolLibrary::TColor_variable field_0x468;
        /* 0x47C */ JStudioToolLibrary::TColor_variable field_0x47c;
        /* 0x490 */ JStudioToolLibrary::TColor_variable field_0x490;
        /* 0x4A4 */ JStudioToolLibrary::TColor_variable field_0x4a4;
        /* 0x4B8 */ JStudioToolLibrary::TColor_variable field_0x4b8;
        /* 0x4CC */ JStudioToolLibrary::TColor_variable field_0x4cc;
        /* 0x4E0 */ u8 field_0x4E0[0x4F8 - 0x4E0];
        /* 0x4F8 */ JStudio::TControl oJSTControl_;
        /* 0x608 */ JStudio::TObject_camera field_0x608;
        /* 0x640 */ u8 field_0x640;
    };
}

#endif
