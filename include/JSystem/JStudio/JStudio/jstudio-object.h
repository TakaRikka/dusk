#ifndef JSTUDIO_OBJECT_H
#define JSTUDIO_OBJECT_H

#include "JSystem/JStudio/JStudio/jstudio-control.h"
#include "global.h"
#include "limits.h"
#include "math.h"

typedef struct _GXColor GXColor;

namespace JStudio {
namespace data {
    enum TEOperationData {
        UNK_0x1 = 0x1,
        UNK_0x2 = 0x2,
        UNK_0x3 = 0x3,
        UNK_0x10 = 0x10,
        UNK_0x11 = 0x11,
        UNK_0x12 = 0x12,
        UNK_0x18 = 0x18,
        UNK_0x19 = 0x19,
    };
};

struct TAdaptor;
struct TVariableValue {
    struct TOutput {
        virtual void operator()(f32, JStudio::TAdaptor*) const = 0;
        virtual ~TOutput() = 0;
    };

    struct TOutput_none_ : TOutput {
        ~TOutput_none_();
        void operator()(f32, JStudio::TAdaptor*) const;
    };

    void update(f64, JStudio::TAdaptor*);
    static void update_immediate_(JStudio::TVariableValue*, f64);
    static void update_time_(JStudio::TVariableValue*, f64);
    static void update_functionValue_(JStudio::TVariableValue*, f64);
    TVariableValue() : field_0x4(0), field_0x8(NULL), pOutput_(&soOutput_none_) {}

    void setValue_immediate(f32 value) {
        field_0x8 = &update_immediate_;
        field_0x4 = 0;
        field_0xc.val = value;
    }

     void setValue_none() {
#if DEBUG
        field_0xc.fv = NULL;
#endif
        field_0x8 = NULL;
    }

    void setValue_time(f32 value) {
        field_0x8 = &update_time_;
        field_0x4 = 0;
        field_0xc.val = value;
    }
    
    void setValue_functionValue(TFunctionValue* value) {
        field_0x8 = &update_functionValue_;
        field_0x4 = 0;
        field_0xc.fv = value;
    }

    f32 getValue() const { return mValue; }

    template<typename T>
    T getValue_clamp() const {
        u8 min = std::numeric_limits<T>::min();
        u8 max = std::numeric_limits<T>::max();
        if (mValue <= min) {
            return (T)min;
        } else if (mValue >= max) {
            return (T)max;
        }
        return mValue;
    }
    u8 getValue_uint8() const { return getValue_clamp<u8>(); }

    void forward(u32 param_0) {
        u32 max = std::numeric_limits<u32>::max();
        if (max - field_0x4 <= param_0) {
            field_0x4 = max;
        } else {
            field_0x4 += param_0;
        }
    }

    void setOutput(const TOutput* param_1) {
        pOutput_ = (param_1 != NULL) ? param_1 : &soOutput_none_;
    }

    static TOutput_none_ soOutput_none_;

    /* 0x00 */ f32 mValue;
    /* 0x04 */ u32 field_0x4;
    /* 0x08 */ void (*field_0x8)(TVariableValue*, double);
    /* 0x0C */ union {
        TFunctionValue* fv;
        f32 val;
    } field_0xc;
    /* 0x10 */ const TOutput* pOutput_;
};  // Size: 0x14

typedef void (TObject::*paragraphFunc)(u32, void const*, u32); 

class TObject : public stb::TObject {
public:
    TObject(JStudio::stb::data::TParse_TBlock_object const&, JStudio::TAdaptor*);
    void forward_value(u32);

    virtual ~TObject() = 0;
    virtual void do_begin();
    virtual void do_end();
    virtual void do_paragraph(u32, void const*, u32) = 0;
    virtual void do_wait(u32);
    virtual void do_data(void const*, u32, void const*, u32);

    TAdaptor* getAdaptor() const { return mpAdaptor; }
    TControl* getControl() { return (TControl*)stb::TObject::getControl(); }
    const TControl* getControl() const { return (const TControl*)stb::TObject::getControl(); }

    inline void prepareAdaptor();

    template<class T>
    T* createFromAdaptor(const stb::data::TParse_TBlock_object& param_0, T* param_1) {
        T* n = new T(param_0, param_1);

        if (n == NULL) {
            return NULL;
        }

        n->prepareAdaptor();

        return n;
    }

    /* 0x34 */ TAdaptor* mpAdaptor;
};

struct TAdaptor {
    struct TSetVariableValue_immediate {
        TSetVariableValue_immediate() : field_0x0(-1), field_0x4(NAN) {}
        TSetVariableValue_immediate(u32 param_1, f32 param_2) :
            field_0x0(param_1), field_0x4(param_2) {}
        u32 field_0x0;
        f32 field_0x4;
    };
    typedef void (*setVarFunc)(JStudio::TAdaptor*, JStudio::TControl*, u32, void const*, u32);
    TAdaptor(TVariableValue *param_1, u32 param_2) {
        pObject_ = NULL;
        pValue_ = param_1;
        uvv_ = param_2;
    }
    virtual ~TAdaptor() = 0;
    virtual void adaptor_do_prepare();
    virtual void adaptor_do_begin();
    virtual void adaptor_do_end();
    virtual void adaptor_do_update(u32);
    virtual void adaptor_do_data(void const*, u32, void const*, u32);

    void adaptor_setVariableValue(JStudio::TControl*, u32,
                                                 JStudio::data::TEOperationData, void const*, u32);
    void adaptor_setVariableValue_n(JStudio::TControl*, u32 const*, u32,
                                                   JStudio::data::TEOperationData, void const*,
                                                   u32);
    void
    adaptor_setVariableValue_immediate(JStudio::TAdaptor::TSetVariableValue_immediate const*);
    void adaptor_setVariableValue_Vec(u32 const*, Vec const&);
    void adaptor_getVariableValue_Vec(Vec*, u32 const*) const;
    void adaptor_setVariableValue_GXColor(u32 const*, GXColor const&);
    void adaptor_getVariableValue_GXColor(GXColor*, u32 const*) const;
    void adaptor_updateVariableValue(JStudio::TControl*, u32);
    static void adaptor_setVariableValue_VOID_(JStudio::TAdaptor*, JStudio::TControl*, u32,
                                                       void const*, u32);
    static void adaptor_setVariableValue_IMMEDIATE_(JStudio::TAdaptor*, JStudio::TControl*,
                                                            u32, void const*, u32);
    static void adaptor_setVariableValue_TIME_(JStudio::TAdaptor*, JStudio::TControl*, u32,
                                                       void const*, u32);
    static void adaptor_setVariableValue_FVR_NAME_(JStudio::TAdaptor*, JStudio::TControl*,
                                                           u32, void const*, u32);
    static void adaptor_setVariableValue_FVR_INDEX_(JStudio::TAdaptor*, JStudio::TControl*,
                                                            u32, void const*, u32);

    const char* adaptor_getID_string() const;

    void adaptor_setObject_(const TObject* pObject) {
        pObject_ = pObject;
    }

    TVariableValue* adaptor_referVariableValue(u32 param_0) {
        return &pValue_[param_0];
    }

    void adaptor_setVariableValue_immediate(u32 param_0, f32 param_1) {
        adaptor_referVariableValue(param_0)->setValue_immediate(param_1);
    }

     const TVariableValue* adaptor_getVariableValue(u32 u) const {
        JUT_ASSERT(293, u<uvv_);
        return &pValue_[u];
    }

    const TObject* adaptor_getObject() const { return pObject_; }

    /* 0x4 */ const TObject* pObject_;
    /* 0x8 */ TVariableValue* pValue_;
    /* 0xC */ u32 uvv_;
};

inline void TObject::prepareAdaptor() {
    if (mpAdaptor != NULL) {
        mpAdaptor->adaptor_setObject_(this);
        mpAdaptor->adaptor_do_prepare();
    }
}

struct TAdaptor_actor : public TAdaptor {
    enum TEVariableValue {
        TE_VALUE_NONE = -1,
        TEACTOR_1 = 1,
    };

    TAdaptor_actor()
        : TAdaptor(mValue, ARRAY_SIZEU(mValue))
        , mValue()
    {
    }
    virtual ~TAdaptor_actor() = 0;
    virtual void adaptor_do_PARENT(JStudio::data::TEOperationData, const void*, u32)                 = 0;
    virtual void adaptor_do_PARENT_NODE(JStudio::data::TEOperationData, const void*, u32)            = 0;
    virtual void adaptor_do_PARENT_ENABLE(JStudio::data::TEOperationData, const void*, u32)          = 0;
    virtual void adaptor_do_PARENT_FUNCTION(JStudio::data::TEOperationData, const void*, u32)        = 0;
    virtual void adaptor_do_RELATION(JStudio::data::TEOperationData, const void*, u32)               = 0;
    virtual void adaptor_do_RELATION_NODE(JStudio::data::TEOperationData, const void*, u32)          = 0;
    virtual void adaptor_do_RELATION_ENABLE(JStudio::data::TEOperationData, const void*, u32)        = 0;
    virtual void adaptor_do_SHAPE(JStudio::data::TEOperationData, const void*, u32)                  = 0;
    virtual void adaptor_do_ANIMATION(JStudio::data::TEOperationData, const void*, u32)              = 0;
    virtual void adaptor_do_ANIMATION_MODE(JStudio::data::TEOperationData, const void*, u32)         = 0;
    virtual void adaptor_do_TEXTURE_ANIMATION(JStudio::data::TEOperationData, const void*, u32)      = 0;
    virtual void adaptor_do_TEXTURE_ANIMATION_MODE(JStudio::data::TEOperationData, const void*, u32) = 0;

    /* 0x10 */ TVariableValue mValue[14];

    static u32 const sauVariableValue_3_TRANSLATION_XYZ[3];
    static u32 const sauVariableValue_3_ROTATION_XYZ[3];
    static u32 const sauVariableValue_3_SCALING_XYZ[3];
};  // Size: 0x128

struct TObject_actor : public TObject {
    TObject_actor(JStudio::stb::data::TParse_TBlock_object const&,
                                 JStudio::TAdaptor_actor*);
    
    virtual void do_paragraph(u32, void const*, u32);
};

struct TAdaptor_ambientLight : public TAdaptor {
    TAdaptor_ambientLight() : TAdaptor(mValue, 4) {}
    virtual ~TAdaptor_ambientLight()  = 0;

    /* 0x10 */ TVariableValue mValue[4];

    static u32 const sauVariableValue_3_COLOR_RGB[3];
    static u32 const sauVariableValue_4_COLOR_RGBA[4];
};

struct TObject_ambientLight : public TObject {
    TObject_ambientLight(JStudio::stb::data::TParse_TBlock_object const&,
                                        JStudio::TAdaptor_ambientLight*);
    
    virtual void do_paragraph(u32, void const*, u32);
};

struct TAdaptor_camera : public TAdaptor {
    enum TEVariableValue {
        TECAMERA_6 = 6,
        TECAMERA_7 = 7,
        TECAMERA_8 = 8,
        TECAMERA_9 = 9,
    };

    TAdaptor_camera() : TAdaptor(mValue, 12) {}
    virtual ~TAdaptor_camera() = 0;

    virtual void adaptor_do_PARENT(JStudio::data::TEOperationData, const void*, u32)               = 0;
    virtual void adaptor_do_PARENT_NODE(JStudio::data::TEOperationData, const void*, u32)          = 0;
    virtual void adaptor_do_PARENT_ENABLE(JStudio::data::TEOperationData, const void*, u32)        = 0;
    virtual void adaptor_do_PARENT_FUNCTION(JStudio::data::TEOperationData, const void*, u32)      = 0;
    virtual void adaptor_do_TARGET_PARENT(JStudio::data::TEOperationData, const void*, u32)        = 0;
    virtual void adaptor_do_TARGET_PARENT_NODE(JStudio::data::TEOperationData, const void*, u32)   = 0;
    virtual void adaptor_do_TARGET_PARENT_ENABLE(JStudio::data::TEOperationData, const void*, u32) = 0;

    /* 0x10 */ TVariableValue mValue[12];

    static u32 const sauVariableValue_3_POSITION_XYZ[3];
    static u32 const sauVariableValue_3_TARGET_POSITION_XYZ[3];
    static u32 const sauVariableValue_2_DISTANCE_NEAR_FAR[2];
};

struct TObject_camera : public TObject {
    TObject_camera(JStudio::stb::data::TParse_TBlock_object const&,
                                  JStudio::TAdaptor_camera*);
    
    virtual void do_paragraph(u32, void const*, u32);
};

struct TAdaptor_fog : public TAdaptor {
    enum TEVariableValue {
        TEFOG_4 = 4,
        TEFOG_5 = 5,
    };

    TAdaptor_fog() : TAdaptor(mValue, 6) {}
    virtual ~TAdaptor_fog() = 0;

    /* 0x10 */ TVariableValue mValue[6];

    static u32 const sauVariableValue_3_COLOR_RGB[3];
    static u32 const sauVariableValue_4_COLOR_RGBA[4];
    static u32 const sauVariableValue_2_RANGE_BEGIN_END[2];
};

struct TObject_fog : public TObject {
    TObject_fog(JStudio::stb::data::TParse_TBlock_object const&,
                               JStudio::TAdaptor_fog*);
    
    virtual void do_paragraph(u32, void const*, u32);
};

struct TAdaptor_light : public TAdaptor {
    enum TEVariableValue {
        TE_VALUE_NONE = -1,
        TE_VALUE_7 = 7,
        TE_VALUE_8 = 8,
        TE_VALUE_9 = 9,
        TE_VALUE_10 = 10,
        TE_VALUE_11 = 11,
    };

    TAdaptor_light() : TAdaptor(mValue, 13) {}
    virtual ~TAdaptor_light() = 0;
    virtual void adaptor_do_ENABLE(JStudio::data::TEOperationData, const void*, u32) = 0;
    virtual void adaptor_do_FACULTY(JStudio::data::TEOperationData, const void*, u32) = 0;

    /* 0x10 */ TVariableValue mValue[13];

    static u32 const sauVariableValue_3_COLOR_RGB[3];
    static u32 const sauVariableValue_4_COLOR_RGBA[4];
    static u32 const sauVariableValue_3_POSITION_XYZ[3];
    static u32 const sauVariableValue_3_TARGET_POSITION_XYZ[3];
    static u32 const sauVariableValue_2_DIRECTION_THETA_PHI[2];
};

struct TObject_light : public TObject {
    TObject_light(JStudio::stb::data::TParse_TBlock_object const&,
                                 JStudio::TAdaptor_light*);
    
    virtual void do_paragraph(u32, void const*, u32);
};

struct TAdaptor_message : public TAdaptor {
    TAdaptor_message() : TAdaptor(NULL, 0) {}
    virtual ~TAdaptor_message() = 0;
    virtual void adaptor_do_MESSAGE(JStudio::data::TEOperationData, const void*, u32) = 0;
};

struct TObject_message : public TObject {
    TObject_message(JStudio::stb::data::TParse_TBlock_object const&,
                                   JStudio::TAdaptor_message*);
    
    virtual void do_paragraph(u32, void const*, u32);
};

struct TAdaptor_particle : public TAdaptor {
    TAdaptor_particle() : TAdaptor(mValue, 20) {}
    virtual ~TAdaptor_particle() = 0;

    virtual void adaptor_do_PARTICLE(JStudio::data::TEOperationData, const void*, u32) = 0;
    virtual void adaptor_do_BEGIN(JStudio::data::TEOperationData, const void*, u32) = 0;
    virtual void adaptor_do_BEGIN_FADE_IN(JStudio::data::TEOperationData, const void*, u32) = 0;
    virtual void adaptor_do_END(JStudio::data::TEOperationData, const void*, u32) = 0;
    virtual void adaptor_do_END_FADE_OUT(JStudio::data::TEOperationData, const void*, u32) = 0;
    virtual void adaptor_do_PARENT(JStudio::data::TEOperationData, const void*, u32) = 0;
    virtual void adaptor_do_PARENT_NODE(JStudio::data::TEOperationData, const void*, u32) = 0;
    virtual void adaptor_do_PARENT_ENABLE(JStudio::data::TEOperationData, const void*, u32) = 0;
    virtual void adaptor_do_PARENT_FUNCTION(JStudio::data::TEOperationData, const void*, u32) = 0;
    virtual void adaptor_do_REPEAT(JStudio::data::TEOperationData, const void*, u32) = 0;
    virtual void adaptor_do_ON_EXIT_NOT_END(JStudio::data::TEOperationData, const void*, u32) = 0;

    /* 0x10 */ TVariableValue mValue[20];

    static u32 const sauVariableValue_3_TRANSLATION_XYZ[3];
    static u32 const sauVariableValue_3_ROTATION_XYZ[3];
    static u32 const sauVariableValue_3_SCALING_XYZ[3];
    static u32 const sauVariableValue_3_COLOR_RGB[3];
    static u32 const sauVariableValue_4_COLOR_RGBA[4];
    static u32 const sauVariableValue_3_COLOR1_RGB[3];
    static u32 const sauVariableValue_4_COLOR1_RGBA[4];
};

struct TObject_particle : public TObject {
    TObject_particle(JStudio::stb::data::TParse_TBlock_object const&,
                                    JStudio::TAdaptor_particle*);
    
    virtual void do_paragraph(u32, void const*, u32);
};

struct TAdaptor_sound : public TAdaptor {
    enum TEVariableValue {
        UNK_7 = 7,
        UNK_8 = 8,
        UNK_9 = 9,
        UNK_10 = 10,
        UNK_11 = 11,
        UNK_NONE = -1,
    };

    TAdaptor_sound() : TAdaptor(mValue, 13) {}
    virtual ~TAdaptor_sound() = 0;

    virtual void adaptor_do_SOUND(JStudio::data::TEOperationData, const void*, u32) = 0;
    virtual void adaptor_do_BEGIN(JStudio::data::TEOperationData, const void*, u32) = 0;
    virtual void adaptor_do_BEGIN_FADE_IN(JStudio::data::TEOperationData, const void*, u32) = 0;
    virtual void adaptor_do_END(JStudio::data::TEOperationData, const void*, u32) = 0;
    virtual void adaptor_do_END_FADE_OUT(JStudio::data::TEOperationData, const void*, u32) = 0;
    virtual void adaptor_do_PARENT(JStudio::data::TEOperationData, const void*, u32) = 0;
    virtual void adaptor_do_PARENT_NODE(JStudio::data::TEOperationData, const void*, u32) = 0;
    virtual void adaptor_do_PARENT_ENABLE(JStudio::data::TEOperationData, const void*, u32) = 0;
    virtual void adaptor_do_REPEAT(JStudio::data::TEOperationData, const void*, u32) = 0;
    virtual void adaptor_do_CONTINUOUS(JStudio::data::TEOperationData, const void*, u32) = 0;
    virtual void adaptor_do_LOCATED(JStudio::data::TEOperationData, const void*, u32) = 0;
    virtual void adaptor_do_ON_EXIT_NOT_END(JStudio::data::TEOperationData, const void*, u32) = 0;

    /* 0x10 */ TVariableValue mValue[13];

    static u32 const sauVariableValue_3_POSITION_XYZ[3];
};  // Size: 0x114

struct TObject_sound : public TObject {
    TObject_sound(JStudio::stb::data::TParse_TBlock_object const&,
                                 JStudio::TAdaptor_sound*);
    
    virtual void do_paragraph(u32, void const*, u32);
};

};  // namespace JStudio

#endif /* JSTUDIO_OBJECT_H */
