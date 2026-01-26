#ifndef JSTUDIOTOOLLIBRARY_CONTROLSET_TRANSFORM_H
#define JSTUDIOTOOLLIBRARY_CONTROLSET_TRANSFORM_H

#include "JSystem/JStudio/JStudioToolLibrary/controlset.h"
#include "JSystem/JStudio/JStudio/jstudio-control.h"

namespace JStudioToolLibrary_JStudio {
    struct TControlSet_transform : public JStudioToolLibrary::TControlSet {
        TControlSet_transform(JStudioToolLibrary::TConsole*, u32, JStudio::TControl);

        void resetOrigin();
        void setOrigin(const Vec&, f32);
        void setEnabled(bool);

        JStudio::TControl* jstudio_getControl() const;
        void jstudio_setControl(JStudio::TControl*);

        void control_updateEnabled();
        void value_setEnabled(bool);
        void control_updateOrigin();
        void value_setOrigin(const Vec&, f32);
        bool setOrigin(const char*);
        bool setOrigin_TxyzRy_(TControlSet_transform*, const char*);
        void control_generate_float_(JORMContext*, TControlSet_transform*, const JStudioToolLibrary::TControlSet::TControl&, f32);
        void control_reply_float_(const JORPropertyEvent*, TControlSet_transform*, f32*);
        void control_update_float_(TControlSet_transform*, const JStudioToolLibrary::TControlSet::TControl&, f32);
        void control_generate_enable(JORMContext*, TControlSet_transform*, const JStudioToolLibrary::TControlSet::TControl&);
        bool isEnabled() const;
        void control_reply_enable(const JORPropertyEvent*, TControlSet_transform*, const JStudioToolLibrary::TControlSet::TControl&);
        void control_update_enable(TControlSet_transform*, const JStudioToolLibrary::TControlSet::TControl&);
        void control_generate_origin_translationX(JORMContext*, TControlSet_transform*, const JStudioToolLibrary::TControlSet::TControl&);
        void control_reply_origin_translationX(const JORPropertyEvent*, TControlSet_transform*, const JStudioToolLibrary::TControlSet::TControl&);
        void control_update_origin_translationX(TControlSet_transform*, const JStudioToolLibrary::TControlSet::TControl&);
        void control_generate_origin_translationY(JORMContext*, TControlSet_transform*, const JStudioToolLibrary::TControlSet::TControl&);
        void control_reply_origin_translationY(const JORPropertyEvent*, TControlSet_transform*, const JStudioToolLibrary::TControlSet::TControl&);
        void control_update_origin_translationY(TControlSet_transform*, const JStudioToolLibrary::TControlSet::TControl&);
        void control_generate_origin_translationZ(JORMContext*, TControlSet_transform*, const JStudioToolLibrary::TControlSet::TControl&);
        void control_reply_origin_translationZ(const JORPropertyEvent*, TControlSet_transform*, const JStudioToolLibrary::TControlSet::TControl&);
        void control_update_origin_translationZ(TControlSet_transform*, const JStudioToolLibrary::TControlSet::TControl&);
        void control_generate_origin_rotationY(JORMContext*, TControlSet_transform*, const JStudioToolLibrary::TControlSet::TControl&);
        void control_reply_origin_rotationY(const JORPropertyEvent*, TControlSet_transform*, const JStudioToolLibrary::TControlSet::TControl&);
        void control_update_origin_rotationY(TControlSet_transform*, const JStudioToolLibrary::TControlSet::TControl&);
        void control_generate_origin_reset(JORMContext*, TControlSet_transform*, const JStudioToolLibrary::TControlSet::TControl&);
        void control_reply_origin_reset(const JORPropertyEvent*, TControlSet_transform*, const JStudioToolLibrary::TControlSet::TControl&);
        void control_update_origin_reset(TControlSet_transform*, const JStudioToolLibrary::TControlSet::TControl&);

        /* 0x14 */ bool bEnabled_;
        /* 0x18 */ TControl* pControl_;
        /* 0x1C */ Vec field_0x1c;
        /* 0x28 */ f32 field_0x28;
    };
}

#endif
