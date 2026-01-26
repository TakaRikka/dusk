#ifndef JSTUDIOTOOLLIBRARY_INTERFACE_H
#define JSTUDIOTOOLLIBRARY_INTERFACE_H

#include "JSystem/JUtility/JUTGamePad.h"

namespace JStudioToolLibrary {
    struct TPad {
        struct TData {
            TData();
            void reset();

            /* 0x00 */ int iButton_;
            /* 0x04 */ int iButtonTrigger_;
            /* 0x08 */ int iButtonRelease_;
            /* 0x0C */ int iButtonRepeat_;
            /* 0x10 */ f32 fAnalogMainStickX_;
            /* 0x14 */ f32 fAnalogMainStickY_;
            /* 0x18 */ f32 fAnalogSubStickX_;
            /* 0x1C */ f32 fAnalogSubStickY_;
            /* 0x20 */ f32 fAnalogTriggerL_;
            /* 0x24 */ f32 fAnalogTriggerR_;
        };

        TPad();

        void setPad(const JUTGamePad* pPad) {
            pPad_ = pPad;
        }

        bool isEnabled() const;
        void getData(TData*) const;
        f32 getAnalog_triggerR() const;
        f32 getAnalog_triggerL() const;
        f32 getAnalog_subStickY() const;
        f32 getAnalog_subStickX() const;
        f32 getAnalog_mainStickY() const;
        f32 getAnalog_mainStickX() const;
        int getButton_repeat() const;
        int getButton_release() const;
        int getButton_trigger() const;
        int getButton() const;

        /* 0x0 */ const JUTGamePad* pPad_;
    };
}

#endif
