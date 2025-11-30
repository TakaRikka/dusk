#ifndef JSTUDIO_JAUDIO2_CONTROL_H
#define JSTUDIO_JAUDIO2_CONTROL_H

#include "JSystem/JAudio2/JAISoundHandles.h"
#include "JSystem/JAudio2/JAISoundStarter.h"
#include "JSystem/JStage/JSGSystem.h"
#include "JSystem/JStudio/JStudio/jstudio-object.h"

namespace JStudio_JAudio2 {
struct TCreateObject : public JStudio::TCreateObject {
    typedef JStudio::TObject* (*CreateFunc)(JStudio::stb::data::TParse_TBlock_object const&,
                                            JStudio_JAudio2::TCreateObject*);

    TCreateObject(JAISoundStarter* p_soundStarter, const JStage::TSystem* p_system) {
        pJAISoundStarter_ = p_soundStarter;
        pJSGSystem_ = p_system;
        mPermit_onExit_notEnd = false;
        JUT_ASSERT(45, pJAISoundStarter_!=NULL);
    }

    virtual ~TCreateObject();
    virtual bool create(JStudio::TObject**,
                                       JStudio::stb::data::TParse_TBlock_object const&);
    static JStudio::TObject*
    createObject_JAI_SOUND_(JStudio::stb::data::TParse_TBlock_object const&,
                            JStudio_JAudio2::TCreateObject*);
    JAISoundStarter* get_pJAISoundStarter_() { return pJAISoundStarter_; }
    const JStage::TSystem* get_pJSGSystem_() { return pJSGSystem_; }
    bool isPermit_onExit_notEnd() { return mPermit_onExit_notEnd; }

    /* 0x0C */ JAISoundStarter* pJAISoundStarter_;
    /* 0x10 */ const JStage::TSystem* pJSGSystem_;
    /* 0x14 */ bool mPermit_onExit_notEnd;
};

struct TAdaptor_sound : public JStudio::TAdaptor_sound {
    typedef void (*TVVOSoundSetFunc)(JAISound*, f32);
    typedef JStudio::TObject_sound ObjectType;

    struct TVVOSetValue_ : public JStudio::TVariableValue::TOutput {
        TVVOSetValue_(TEVariableValue param_1, TVVOSoundSetFunc param_2)
            : field_0x04(param_1), field_0x08(param_2) {}
        virtual void operator()(f32, JStudio::TAdaptor*) const;
        virtual ~TVVOSetValue_() {}

        /* 0x04 */ TEVariableValue field_0x04;
        /* 0x08 */ TVVOSoundSetFunc field_0x08;
    };

    TAdaptor_sound(JStudio_JAudio2::TCreateObject*);
    void prepareSound_();
    void beginSound_fadeIn_();
    void endSound_fadeOut_(u32);

    virtual ~TAdaptor_sound();
    virtual void adaptor_do_prepare();
    virtual void adaptor_do_end();
    virtual void adaptor_do_update(u32);
    virtual void adaptor_do_SOUND(JStudio::data::TEOperationData, void const*, u32);
    virtual void adaptor_do_BEGIN(JStudio::data::TEOperationData, void const*, u32);
    virtual void adaptor_do_BEGIN_FADE_IN(JStudio::data::TEOperationData, void const*,
                                                    u32);
    virtual void adaptor_do_END(JStudio::data::TEOperationData, void const*, u32);
    virtual void adaptor_do_END_FADE_OUT(JStudio::data::TEOperationData, void const*,
                                                u32);
    virtual void adaptor_do_PARENT(JStudio::data::TEOperationData, void const*, u32);
    virtual void adaptor_do_PARENT_NODE(JStudio::data::TEOperationData, void const*,
                                                u32);
    virtual void adaptor_do_PARENT_ENABLE(JStudio::data::TEOperationData, void const*,
                                                    u32);
    virtual void adaptor_do_REPEAT(JStudio::data::TEOperationData, void const*, u32);
    virtual void adaptor_do_CONTINUOUS(JStudio::data::TEOperationData, void const*, u32);
    virtual void adaptor_do_LOCATED(JStudio::data::TEOperationData, void const*, u32);
    virtual void adaptor_do_ON_EXIT_NOT_END(JStudio::data::TEOperationData, void const*,
                                                    u32);

    void beginSound_fadeIn_(u32 param_1) {
        field_0x128 = param_1;
        beginSound_fadeIn_();
    }
    
    void beginSound_() {
        beginSound_fadeIn_(0);
    }

    void endSound_() {
        endSound_fadeOut_(0);
    }

    void opJAISoundHandle_stop_() {
        opJAISoundHandle_->stop();
    }

    void set_bPermit_onExit_notEnd_(bool param_1) { field_0x11c = param_1; }

    static TVVOSetValue_ saoVVOSetValue_[6];

    /* 0x114 */ TCreateObject* pCreateObject_;
    /* 0x118 */ JAISoundHandle opJAISoundHandle_;
    /* 0x11C */ bool field_0x11c;
    /* 0x11D */ u8 field_0x11d;
    /* 0x11E */ u8 field_0x11e;
    /* 0x11F */ u8 field_0x11f;
    /* 0x120 */ u8 field_0x120;
    /* 0x124 */ u32 field_0x124;
    /* 0x128 */ u32 field_0x128;
    /* 0x12C */ JGeometry::TVec3<f32>* field_0x12c;
    /* 0x130 */ JGeometry::TVec3<f32> field_0x130;
    /* 0x13C */ JStage::TObject* field_0x13c;
    /* 0x140 */ u32 field_0x140;
    /* 0x144 */ u8 field_0x144;
};
};  // namespace JStudio_JAudio2

#endif /* JSTUDIO_JAUDIO2_CONTROL_H */
