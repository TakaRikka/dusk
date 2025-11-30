#ifndef JSTUDIO_CONTROL_H
#define JSTUDIO_CONTROL_H

#include "JSystem/JStudio/JStudio/fvb.h"
#include "JSystem/JStudio/JStudio/stb.h"
#include "JSystem/JStudio/JStudio/ctb.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>

namespace JStudio {
struct TObject;
struct TCreateObject {
    TCreateObject() {}
    virtual ~TCreateObject() = 0;
    virtual bool create(TObject**, JStudio::stb::data::TParse_TBlock_object const&) = 0;

    template<class AdaptorT>
    static typename AdaptorT::ObjectType* createFromAdaptor(JStudio::stb::data::TParse_TBlock_object const& param_1, AdaptorT* param_2) {
        typename AdaptorT::ObjectType* rv = new typename AdaptorT::ObjectType(param_1, param_2);
        if (rv == NULL) {
            return NULL;
        }
        rv->prepareAdaptor();
        return rv;
    }

    /* 0x4 */ JGadget::TLinkListNode mNode;
};  // Size: 0xC

struct TFactory : public stb::TFactory {
    TFactory() {}

    virtual ~TFactory();
    virtual TObject* create(JStudio::stb::data::TParse_TBlock_object const&);

    void appendCreateObject(JStudio::TCreateObject*);

    /* 0x04 */ JGadget::TLinkList<TCreateObject, -4> mList;
    /* 0x10 */ fvb::TFactory fvb_Factory;
    /* 0x14 */ ctb::TFactory ctb_Factory;
};

class TControl : public stb::TControl {
public:
    struct TTransform_translation_rotation_scaling {
        Vec translation;
        Vec rotation;
        Vec scaling;
    };
    struct TTransform_position : public Vec {};
    struct TTransform_position_direction {
        Vec position;
        Vec direction;
    };

    TControl();
    virtual ~TControl();
    void setFactory(JStudio::TFactory*);
    int transformOnSet_setOrigin_TxyzRy(Vec const&, f32);
    int transformOnGet_setOrigin_TxyzRy(Vec const&, f32);
    int transform_setOrigin_ctb(JStudio::ctb::TObject const&);
    bool transform_setOrigin_ctb_index(u32);

    void stb_destroyObject_all() { stb::TControl::destroyObject_all(); }
    void fvb_destroyObject_all() { fvb_Control.destroyObject_all(); }
    void ctb_destroyObject_all() { ctb_Control.destroyObject_all(); }

    void destroyObject_all() {
        stb_destroyObject_all();
        fvb_destroyObject_all();
        ctb_destroyObject_all();
    }

    void transformOnSet_enable(bool param_0) { mTransformOnSet = param_0; }
    void transformOnGet_enable(bool param_0) { mTransformOnGet = param_0; }

    void transform_enable(bool param_0) {
        transformOnSet_enable(param_0);
        transformOnGet_enable(param_0);
    }

    void transform_setOrigin_TxyzRy(const Vec& xyz, f32 rotY) {
        transformOnSet_setOrigin_TxyzRy(xyz, rotY);
        transformOnGet_setOrigin_TxyzRy(xyz, rotY);
    }

    void transform_setOrigin(const Vec& xyz, f32 rotY) {
        transform_setOrigin_TxyzRy(xyz, rotY);
    }

    void setSecondPerFrame(f64 param_0) { mSecondPerFrame = param_0; }
    f64 getSecondPerFrame() const { return mSecondPerFrame; }

    ctb::TObject* ctb_getObject_index(u32 index) {
        return ctb_Control.getObject_index(index);
    }

    fvb::TObject* fvb_getObject(const void* param_1, u32 param_2) {
        return fvb_Control.getObject(param_1, param_2);
    }

    fvb::TObject* fvb_getObject_index(u32 index) {
        return fvb_Control.getObject_index(index);
    }

    TFunctionValue* getFunctionValue(const void* param_1, u32 param_2) {
        fvb::TObject* obj = fvb_getObject(param_1, param_2);
        if (obj == NULL) {
            return NULL;
        } 
        return obj->referFunctionValue();
    }

    TFunctionValue* getFunctionValue_index(u32 index) {
        fvb::TObject* obj = fvb_getObject_index(index);
        if (obj == NULL) {
            return NULL;
        } 
        return obj->referFunctionValue();
    }

    bool transformOnSet_isEnabled() const { return mTransformOnSet; }
    CMtxP transformOnSet_getMatrix() const { return mTransformOnSet_Matrix; }

    void transformOnSet_transformTranslation(const Vec& rSrc, Vec* pDst) const {
        JUT_ASSERT(226, pDst!=NULL);
        JUT_ASSERT(227, &rSrc!=pDst);
        MTXMultVec(transformOnSet_getMatrix(), &rSrc, pDst);
    }

    void transformOnSet_transformRotation(const Vec& rSrc, Vec* pDst) const {
        JUT_ASSERT(232, pDst!=NULL);
        JUT_ASSERT(233, &rSrc!=pDst);
        pDst->x = rSrc.x;
        pDst->y = rSrc.y + mTransformOnSet_RotationY;
        pDst->z = rSrc.z;
    }

    void transformOnSet_transformScaling(const Vec& rSrc, Vec* pDst) const {
        JUT_ASSERT(240, pDst!=NULL);
        JUT_ASSERT(241, &rSrc!=pDst);
        *pDst = rSrc;
    }

    void transformOnSet_transform(TTransform_position* param_1, TTransform_position* param_2) const {
        transformOnSet_transformTranslation(*param_1, param_2);
    }

    void transformOnSet_transform(TTransform_translation_rotation_scaling* param_1,
                                  TTransform_translation_rotation_scaling* param_2) const {
        transformOnSet_transformTranslation(param_1->translation, &param_2->translation);
        transformOnSet_transformRotation(param_1->rotation, &param_2->rotation);
        transformOnSet_transformScaling(param_1->scaling, &param_2->scaling);
    }

    TTransform_position* transformOnSet_transform_ifEnabled(TTransform_position* param_1,
                                                            TTransform_position* param_2) const {
        if (!transformOnSet_isEnabled()) {
            return param_1;
        }
        transformOnSet_transform(param_1, param_2);
        return param_2;
    }

    TTransform_translation_rotation_scaling*
    transformOnSet_transform_ifEnabled(TTransform_translation_rotation_scaling* param_1,
                                       TTransform_translation_rotation_scaling* param_2) const {
        if (!transformOnSet_isEnabled()) {
            return param_1;
        }
        transformOnSet_transform(param_1, param_2);
        return param_2;
    }

    bool transformOnGet_isEnabled() const { return mTransformOnGet; }
    CMtxP transformOnGet_getMatrix() const { return mTransformOnGet_Matrix; }

    void transformOnGet_transformTranslation(const Vec& rSrc, Vec* pDst) const {
        JUT_ASSERT(296, pDst!=NULL);
        JUT_ASSERT(297, &rSrc!=pDst);
        MTXMultVec(transformOnGet_getMatrix(), &rSrc, pDst);
    }

    void transformOnGet_transformDirection(const Vec& rSrc, Vec* pDst) const {
        JUT_ASSERT(316, pDst!=NULL);
        JUT_ASSERT(317, &rSrc!=pDst);
        MTXMultVecSR(transformOnGet_getMatrix(), &rSrc, pDst);
    }

    void transformOnGet_transform(TTransform_position_direction* param_1,
                                  TTransform_position_direction* pDst) const {
        JUT_ASSERT(289, pDst!=NULL);
        transformOnGet_transformTranslation(param_1->position, &pDst->position);
        transformOnGet_transformDirection(param_1->direction, &pDst->direction);
    }

    TTransform_position_direction*
    transformOnGet_transform_ifEnabled(TTransform_position_direction& param_1,
                                       TTransform_position_direction* param_2) const {
        if (!transformOnGet_isEnabled()) {
            return &param_1;
        }
        transformOnGet_transform(&param_1, param_2);
        return param_2;
    }

    void transformOnGet_transform(TTransform_position* param_1,
                                  TTransform_position* pDst) const {
        transformOnGet_transformTranslation(*param_1, pDst);
    }

    TTransform_position*
    transformOnGet_transform_ifEnabled(TTransform_position* param_1,
                                       TTransform_position* param_2) const {
        if (!transformOnGet_isEnabled()) {
            return param_1;
        }
        transformOnGet_transform(param_1, param_2);
        return param_2;
    }

    void transformOnGet_transformRotation(const Vec& rSrc, Vec* pDst) const {
        JUT_ASSERT(302, pDst!=NULL);
        JUT_ASSERT(303, &rSrc!=pDst);
        pDst->x = rSrc.x;
        pDst->y = rSrc.y + mTransformOnGet_RotationY;
        pDst->z = rSrc.z;
    }

    void transformOnGet_transformScaling(const Vec& rSrc, Vec* pDst) const {
        JUT_ASSERT(310, pDst!=NULL);
        JUT_ASSERT(311, &rSrc!=pDst);
        *pDst = rSrc;
    }

    void transformOnGet_transform(TTransform_translation_rotation_scaling* param_1,
                                  TTransform_translation_rotation_scaling* pDst) const {
        JUT_ASSERT(263, pDst!=NULL);
        transformOnGet_transformTranslation(param_1->translation, &pDst->translation);
        transformOnGet_transformRotation(param_1->rotation, &pDst->rotation);
        transformOnGet_transformScaling(param_1->scaling, &pDst->scaling);
    }

    TTransform_translation_rotation_scaling*
    transformOnGet_transform_ifEnabled(TTransform_translation_rotation_scaling* param_1,
                                       TTransform_translation_rotation_scaling* param_2) const {
        if (!transformOnGet_isEnabled()) {
            return param_1;
        }
        transformOnGet_transform(param_1, param_2);
        return param_2;
    }

    void transformOnSet_transformDirection(const Vec& rSrc, Vec* pDst) const {
        JUT_ASSERT(246, pDst!=NULL);
        JUT_ASSERT(247, &rSrc!=pDst);
        MTXMultVecSR(transformOnSet_getMatrix(), &rSrc, pDst);
    }

    void transformOnSet_transform(TTransform_position_direction* param_1,
                                  TTransform_position_direction* pDst) const {
        JUT_ASSERT(219, pDst!=NULL);
        transformOnSet_transformTranslation(param_1->position, &pDst->position);
        transformOnSet_transformDirection(param_1->direction, &pDst->direction);
    }

    TTransform_position_direction*
    transformOnSet_transform_ifEnabled(TTransform_position_direction* param_1,
                                       TTransform_position_direction* param_2) const {
        if (!transformOnSet_isEnabled()) {
            return param_1;
        }
        transformOnSet_transform(param_1, param_2);
        return param_2;
    }

    /* 0x58 */ f64 mSecondPerFrame;
    /* 0x60 */ fvb::TControl fvb_Control;
    /* 0x74 */ ctb::TControl ctb_Control;
    /* 0x88 */ bool mTransformOnSet;
    /* 0x89 */ bool mTransformOnGet;
    /* 0x8C */ Vec field_0x8c;
    /* 0x98 */ Vec field_0x98;
    /* 0xA4 */ f32 mTransformOnSet_RotationY;
    /* 0xA8 */ f32 mTransformOnGet_RotationY;
    /* 0xAC */ Mtx mTransformOnSet_Matrix;
    /* 0xDC */ Mtx mTransformOnGet_Matrix;
};

struct TParse : public stb::TParse {
    TParse(JStudio::TControl*);
    bool parseBlock_block_fvb_(JStudio::stb::data::TParse_TBlock const&, u32);
    bool parseBlock_block_ctb_(JStudio::stb::data::TParse_TBlock const&, u32);

    virtual ~TParse();
    virtual bool parseHeader(JStudio::stb::data::TParse_THeader const&, u32);
    virtual bool parseBlock_block(JStudio::stb::data::TParse_TBlock const&, u32);

    TControl* getControl() { return (TControl*)stb::TParse::getControl(); }
};

};  // namespace JStudio

#endif /* JSTUDIO_CONTROL_H */
