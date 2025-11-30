#ifndef JSTUDIO_JSTAGE_CONTROL_H
#define JSTUDIO_JSTAGE_CONTROL_H

#include "JSystem/JGadget/pointer.h"
#include "JSystem/JStage/JSGActor.h"
#include "JSystem/JStage/JSGAmbientLight.h"
#include "JSystem/JStage/JSGCamera.h"
#include "JSystem/JStage/JSGFog.h"
#include "JSystem/JStage/JSGLight.h"
#include "JSystem/JStage/JSGSystem.h"
#include "JSystem/JStudio/JStudio/jstudio-object.h"
#include "JSystem/JStudio/JStudio/jstudio-math.h"

namespace JStudio_JStage {
typedef JStudio::TObject* (*ObjCreateFuncT)(const JStudio::stb::data::TParse_TBlock_object&, JStage::TObject*, const JStage::TSystem*);

struct TCreateObject : public JStudio::TCreateObject {
    TCreateObject(const JStage::TSystem* pSystem) {
        pJSGSystem_ = pSystem;
        JUT_ASSERT(42, pJSGSystem_!=NULL);
    }

    virtual ~TCreateObject();
    virtual bool create(JStudio::TObject**,
                                       JStudio::stb::data::TParse_TBlock_object const&);

    /* 0x0C */ const JStage::TSystem* pJSGSystem_;
};

struct TAdaptor_object_ {
    TAdaptor_object_(JStage::TSystem const*param_2, JStage::TObject *param_3) :
        pJSGSystem_(param_2), pJSGObject_(param_3) {}
    void adaptor_object_data_(void const*, u32, void const*, u32);
    JStage::TObject* adaptor_object_findJSGObject_(char const*);
    s32 adaptor_object_findJSGObjectNode_(JStage::TObject const*, char const*);
    void adaptor_object_ENABLE_(JStudio::data::TEOperationData, void const*, u32);

    void adaptor_object_begin_() { pJSGObject_->JSGFEnableFlag(1); }
    void adaptor_object_end_() { pJSGObject_->JSGFDisableFlag(1); }

    /* 0x0 */ JStage::TSystem const* pJSGSystem_;
    /* 0x4 */ JStage::TObject* pJSGObject_;
};

template<class TAdaptor, class TStageObject>
struct TVariableValueOutput_object_;

struct TAdaptor_actor : public JStudio::TAdaptor_actor, public JStudio_JStage::TAdaptor_object_ {
    typedef JStudio::TObject_actor ObjectType;
    typedef TVariableValueOutput_object_<TAdaptor_actor, JStage::TActor> TVVOutputObject;
    typedef void (JStage::TActor::*Setter)(f32);
    typedef f32 (JStage::TActor::*Getter)() const;
    typedef f32 (JStage::TActor::*MaxGetter)() const;

    struct TVVOutput_ANIMATION_FRAME_ 
    : public JStudio::TVariableValue::TOutput
    {
        TVVOutput_ANIMATION_FRAME_() {
            mValueIndex = -1;
            field_0x8 = 0;
            mSetter = NULL;
            mGetter = NULL;
            mMaxGetter = NULL;
        }
        TVVOutput_ANIMATION_FRAME_(int param_1, u32 param_2, Setter param_3, Getter param_4,
                                   MaxGetter param_5) {
            mValueIndex = param_1;
            field_0x8 = param_2;
            mSetter = param_3;
            mGetter = param_4;
            mMaxGetter = param_5;
        }

        virtual void operator()(f32, JStudio::TAdaptor*) const;
        inline virtual ~TVVOutput_ANIMATION_FRAME_();
        
        void adaptor_setOutput_(TAdaptor* adaptor) const {
            adaptor->adaptor_referVariableValue(mValueIndex)->setOutput(this);
        }

        void setVariableValue_(const JStage::TActor *param_1, JStudio::TAdaptor *param_2) const {
            f32 val = (param_1->*mGetter)();
            param_2->adaptor_setVariableValue_immediate(mValueIndex, val);
        }

        bool isEnd_() const { return mValueIndex == -1; }

        /* 0x04 */ int mValueIndex;
        /* 0x08 */ u32 field_0x8;
        /* 0x0C */ Setter mSetter;
        /* 0x18 */ Getter mGetter;
        /* 0x24 */ MaxGetter mMaxGetter;
    };

    TAdaptor_actor(JStage::TSystem const*, JStage::TActor*);

    virtual ~TAdaptor_actor();
    virtual void adaptor_do_prepare();
    virtual void adaptor_do_begin();
    virtual void adaptor_do_end();
    virtual void adaptor_do_update(u32);
    virtual void adaptor_do_data(void const*, u32, void const*, u32);
    virtual void adaptor_do_PARENT(JStudio::data::TEOperationData, void const*, u32);
    virtual void adaptor_do_PARENT_NODE(JStudio::data::TEOperationData, void const*,
                                                u32);
    virtual void adaptor_do_PARENT_ENABLE(JStudio::data::TEOperationData, void const*,
                                                    u32);
    virtual void adaptor_do_PARENT_FUNCTION(JStudio::data::TEOperationData, void const*,
                                                    u32);
    virtual void adaptor_do_RELATION(JStudio::data::TEOperationData, void const*, u32);
    virtual void adaptor_do_RELATION_NODE(JStudio::data::TEOperationData, void const*,
                                                    u32);
    virtual void adaptor_do_RELATION_ENABLE(JStudio::data::TEOperationData, void const*,
                                                    u32);
    virtual void adaptor_do_SHAPE(JStudio::data::TEOperationData, void const*, u32);
    virtual void adaptor_do_ANIMATION(JStudio::data::TEOperationData, void const*, u32);
    virtual void adaptor_do_ANIMATION_MODE(JStudio::data::TEOperationData, void const*,
                                                    u32);
    virtual void adaptor_do_TEXTURE_ANIMATION(JStudio::data::TEOperationData,
                                                        void const*, u32);
    virtual void adaptor_do_TEXTURE_ANIMATION_MODE(JStudio::data::TEOperationData,
                                                            void const*, u32);
    
    void setJSG_ID_(void (JStage::TActor::*)(u32),
                                    JStudio::data::TEOperationData, void const*, u32);
    void setJSG_SRT_(JStudio::TControl const*);
    void getJSG_SRT_(JStudio::TControl const*);

    JStage::TActor* get_pJSG_() { return (JStage::TActor*) pJSGObject_; }

    static const TVVOutputObject saoVVOutput_[];
    static const TVVOutput_ANIMATION_FRAME_  saoVVOutput_ANIMATION_FRAME_[];

    /* 0x130 */ u32 field_0x130;
    /* 0x134 */ u32 field_0x134;
    /* 0x138 */ u32 field_0x138;
    /* 0x13C */ JStage::TObject* field_0x13c;
    /* 0x140 */ u32 field_0x140;
    /* 0x144 */ u8 field_0x144;
    /* 0x148 */ JStage::TObject* field_0x148;
    /* 0x14C */ u32 field_0x14c;
};

struct TAdaptor_ambientLight : public JStudio::TAdaptor_ambientLight, public TAdaptor_object_ {
    typedef JStudio::TObject_ambientLight ObjectType;
    TAdaptor_ambientLight(JStage::TSystem const*, JStage::TAmbientLight*);

    virtual ~TAdaptor_ambientLight();
    virtual void adaptor_do_prepare();
    virtual void adaptor_do_begin();
    virtual void adaptor_do_end();
    virtual void adaptor_do_update(u32);
    virtual void adaptor_do_data(void const*, u32, void const*, u32);

    JStage::TAmbientLight* get_pJSG_() { return (JStage::TAmbientLight*)pJSGObject_; }
};

struct TAdaptor_camera : public JStudio::TAdaptor_camera, public TAdaptor_object_ {
    typedef JStudio::TObject_camera ObjectType;
    typedef TVariableValueOutput_object_<TAdaptor_camera, JStage::TCamera> TVVOutput;

    TAdaptor_camera(JStage::TSystem const*, JStage::TCamera*);
    virtual ~TAdaptor_camera();
    virtual void adaptor_do_prepare();
    virtual void adaptor_do_begin();
    virtual void adaptor_do_end();
    virtual void adaptor_do_update(u32);
    virtual void adaptor_do_data(void const*, u32, void const*, u32);
    virtual void adaptor_do_PARENT(JStudio::data::TEOperationData, void const*, u32);
    virtual void adaptor_do_PARENT_NODE(JStudio::data::TEOperationData, void const*, u32);
    virtual void adaptor_do_PARENT_ENABLE(JStudio::data::TEOperationData, void const*, u32);
    virtual void adaptor_do_PARENT_FUNCTION(JStudio::data::TEOperationData, void const*,
                                                   u32);
    virtual void adaptor_do_TARGET_PARENT(JStudio::data::TEOperationData, void const*, u32);
    virtual void adaptor_do_TARGET_PARENT_NODE(JStudio::data::TEOperationData, void const*,
                                                      u32);
    virtual void adaptor_do_TARGET_PARENT_ENABLE(JStudio::data::TEOperationData, void const*,
                                                        u32);

    void setJSG_position_(JStudio::TControl const*);
    void getJSG_position_(JStudio::TControl const*);
    void setJSG_targetPosition_(JStudio::TControl const*);
    void getJSG_targetPosition_(JStudio::TControl const*);

    JStage::TCamera* get_pJSG_() { return (JStage::TCamera*)pJSGObject_; }

    static TVVOutput saoVVOutput_[];

    /* 0x108 */ int field_0x108;
    /* 0x10C */ JStage::TObject* field_0x10c;
    /* 0x110 */ int field_0x110;
    /* 0x114 */ u8 field_0x114;
    /* 0x118 */ JStage::TObject* field_0x118;
    /* 0x11C */ int field_0x11c;
    /* 0x120 */ u8 field_0x120;
};

struct TAdaptor_fog : public JStudio::TAdaptor_fog, public TAdaptor_object_ {
    typedef JStudio::TObject_fog ObjectType;

    TAdaptor_fog(JStage::TSystem const*, JStage::TFog*);
    virtual ~TAdaptor_fog();
    virtual void adaptor_do_prepare();
    virtual void adaptor_do_begin();
    virtual void adaptor_do_end();
    virtual void adaptor_do_update(u32);
    virtual void adaptor_do_data(void const*, u32, void const*, u32);

    JStage::TFog* get_pJSG_() { return (JStage::TFog*)pJSGObject_; }

    static TVariableValueOutput_object_<TAdaptor_fog, JStage::TFog> saoVVOutput_[];
};

template<class TAdaptor, class TStageObject>
struct TVariableValueOutput_object_ : public JStudio::TVariableValue::TOutput {
    typedef f32 (TStageObject::*GetFunc)() const;
    typedef void (TStageObject::*SetFunc)(f32);
    TVariableValueOutput_object_() : field_0x4(-1), field_0x8(NULL), field_0x14(NULL) {}
    TVariableValueOutput_object_(typename TAdaptor::TEVariableValue param_1, SetFunc param_2, GetFunc param_3) : field_0x4(param_1), field_0x8(param_2), field_0x14(param_3) {}

    virtual void operator()(f32 param_1, JStudio::TAdaptor* param_2) const {
        (((TAdaptor*)param_2)->get_pJSG_()->*field_0x8)(param_1);
    }
    virtual ~TVariableValueOutput_object_() {}

    bool isEnd_() const { return field_0x4 == -1; }
    void adaptor_setOutput_(TAdaptor* adaptor) const {
        adaptor->adaptor_referVariableValue(field_0x4)->setOutput(this);
    }
    void setVariableValue_(const TStageObject* pObj, TAdaptor* pAdaptor) const {
        f32 val = (pObj->*field_0x14)();
        pAdaptor->adaptor_setVariableValue_immediate(field_0x4, val);
    }

    int field_0x4;
    SetFunc field_0x8;
    GetFunc field_0x14;
};

struct TAdaptor_light : public JStudio::TAdaptor_light, public TAdaptor_object_ {
    typedef JStudio::TObject_light ObjectType;

    enum TEDirection_ {
        DIRECTION_0,
        DIRECTION_1,
        DIRECTION_2,
    };
    struct TVVOutput_direction_ : public JStudio::TVariableValue::TOutput {
        TVVOutput_direction_() {
            field_0x4 = TE_VALUE_NONE;
            field_0x8 = DIRECTION_0;
        }
        TVVOutput_direction_(TEVariableValue param_1, TEDirection_ param_2) {
            field_0x4 = param_1;
            field_0x8 = param_2;
        }

        virtual void operator()(f32, JStudio::TAdaptor*) const;
        virtual ~TVVOutput_direction_() {}

        void adaptor_setOutput_(TAdaptor* adaptor) {
            adaptor->adaptor_referVariableValue(field_0x4)->setOutput(this);
        }

        bool isEnd_() const { return field_0x4 == -1; }

        TEVariableValue field_0x4;
        TEDirection_ field_0x8;
    };

    TAdaptor_light(JStage::TSystem const*, JStage::TLight*);
    virtual ~TAdaptor_light();
    virtual void adaptor_do_prepare();
    virtual void adaptor_do_begin();
    virtual void adaptor_do_end();
    virtual void adaptor_do_update(u32);
    virtual void adaptor_do_data(void const*, u32, void const*, u32);
    virtual void adaptor_do_ENABLE(JStudio::data::TEOperationData, void const*, u32);
    virtual void adaptor_do_FACULTY(JStudio::data::TEOperationData, void const*, u32);

    JStage::TLight* get_pJSG_() { return (JStage::TLight*)pJSGObject_; }

    int field_0x11c;

    static TVVOutput_direction_ saoVVOutput_direction_[6];
};

bool
    transform_toGlobalFromLocal(f32 (*)[4],
                                JStudio::TControl::TTransform_translation_rotation_scaling const&,
                                JStage::TObject const*, u32);
bool transform_toGlobalFromLocal(f32 (*)[4],
                                                JStudio::TControl::TTransform_position const&,
                                                JStage::TObject const*, u32);

inline bool transform_toGlobalFromLocal(JStudio::TControl::TTransform_position* param_1,
                                        JStudio::TControl::TTransform_position const& param_2,
                                        JStage::TObject const* param_3, u32 param_4) {
    Mtx afStack_38;

    if (!transform_toGlobalFromLocal(afStack_38, param_2, param_3, param_4)) {
        return false;
    }
    JStudio::math::getFromTransformation_T(param_1, afStack_38);
    return true;
}
bool
    transform_toLocalFromGlobal(f32 (*)[4],
                                JStudio::TControl::TTransform_translation_rotation_scaling const&,
                                JStage::TObject const*, u32);
bool transform_toLocalFromGlobal(f32 (*)[4],
                                                JStudio::TControl::TTransform_position const&,
                                                JStage::TObject const*, u32);

namespace {

template <class AdaptorT, class ObjectT>
JStudio::TObject* createObject_JSG_(const JStudio::stb::data::TParse_TBlock_object& param_1,
                                    JStage::TObject* param_2, const JStage::TSystem* param_3) {
    ObjectT* objt = (ObjectT*)param_2;                                        
    AdaptorT* pAdaptor = new AdaptorT(param_3, objt);
    if (pAdaptor == NULL) {
        return NULL;
    }
    JGadget::TPointer_delete<AdaptorT> adaptorGuard(pAdaptor);
    JStudio::TObject* pObj = JStudio::TCreateObject::createFromAdaptor<AdaptorT>(param_1, pAdaptor);
    if (pObj == NULL) {
        return NULL;
    }
    adaptorGuard.set(NULL);
    return pObj;
}

}  // namespace

};  // namespace JStudio_JStage

#endif /* JSTUDIO_JSTAGE_CONTROL_H */
