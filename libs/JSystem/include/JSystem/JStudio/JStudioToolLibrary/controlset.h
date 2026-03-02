#ifndef JSTUDIOTOOLLIBRARY_CONTROLSET_H
#define JSTUDIOTOOLLIBRARY_CONTROLSET_H

#include "JSystem/JStudio/JStudioToolLibrary/console.h"

namespace JStudioToolLibrary {
    struct TControlSet {
        struct TControl {
            typedef void* (*generateFn)(JORMContext*, TControlSet*, const TControl&);
            typedef void* (*replyFn)(const JORPropertyEvent*, TControlSet*, const TControl&);
            typedef void* (*updateFn)(TControlSet*, const TControl&);

            TControl();
            TControl(const char*, generateFn, replyFn, updateFn);

            void update(TControlSet*) const;
            void reply(const JORPropertyEvent*, TControlSet*) const;
            void generate(JORMContext*, TControlSet*) const;
            bool isEnd() const;
            const char* getLabel() const;
            int getID(const TControlSet&) const;
            void setStyle(TControlSet*, u32) const;

            /* 0x00 */ char* szLabel_;
            /* 0x04 */ void* pfnGenerate_;
            /* 0x08 */ void* pfnReply_;
            /* 0x0C */ void* pfnUpdate_;
        };

        TControlSet(TConsole*, const TControl*, u32, u32, const u32*);

        TConsole* getConsole();

        TControl* control_getControl_IDOffset(u32) const;
        u32 control_getNumber() const;
        void control_generate(JORMContext*);
        void control_generate_all(JORMContext*);
        void control_generate(JORMContext*, const TControl&);
        void control_generate_IDOffset(JORMContext*, const u32*);
        void control_generate_IDOffset(JORMContext*, u32);
        void control_generate_separator(JORMContext*, TControlSet*, const TControl&);
        void control_generate_label(JORMContext*, TControlSet*, const TControl&);
        void control_reply(const JORPropertyEvent*);
        void control_getControl(u32) const;
        u32 control_getIDBegin() const;
        void control_toID(const TControl*) const;
        u32 control_toID_IDOffset(u32) const;
        u32 control_toIDOffset(const TControl&) const;
        void control_update_IDOffset(u32);
        void control_update(const TControl&);
        void control_update_IDOffset(const u32*);
        void control_setStyle(const TControl&, u32);
        void control_setStyle_IDOffset(u32, u32);

        /* 0x00 */ TConsole* pConsole_;
        /* 0x04 */ TControl* paoControl_;
        /* 0x08 */ u32 uNumber_;
        /* 0x0C */ u32 field_0xc;
        /* 0x10 */ u32 field_0x10;
    };
}

#endif
