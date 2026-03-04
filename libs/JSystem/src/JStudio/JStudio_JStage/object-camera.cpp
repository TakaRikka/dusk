//
// object-camera
//

#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JStudio/JStudio_JStage/object-camera.h"

JStudio_JStage::TAdaptor_camera::TAdaptor_camera(JStage::TSystem const* param_1,
                                                 JStage::TCamera* param_2)
    : TAdaptor_object_(this, param_1, param_2) {
    field_0x108 = 0;
    field_0x10c = 0;
    field_0x110 = -1;
    field_0x114 = 0;
    field_0x118 = 0;
    field_0x11c = -1;
    field_0x120 = 0;
}

JStudio_JStage::TAdaptor_camera::~TAdaptor_camera() {
    adaptor_do_end();
}

JStudio_JStage::TAdaptor_camera::TVVOutput JStudio_JStage::TAdaptor_camera::saoVVOutput_[5] = {
    TVVOutput(JStudio_JStage::TAdaptor_camera::TECAMERA_7, &JStage::TCamera::JSGSetViewRoll,
              &JStage::TCamera::JSGGetViewRoll),
    TVVOutput(JStudio_JStage::TAdaptor_camera::TECAMERA_6, &JStage::TCamera::JSGSetProjectionFovy,
              &JStage::TCamera::JSGGetProjectionFovy),
    TVVOutput(JStudio_JStage::TAdaptor_camera::TECAMERA_8, &JStage::TCamera::JSGSetProjectionNear,
              &JStage::TCamera::JSGGetProjectionNear),
    TVVOutput(JStudio_JStage::TAdaptor_camera::TECAMERA_9, &JStage::TCamera::JSGSetProjectionFar,
              &JStage::TCamera::JSGGetProjectionFar),
    TVVOutput(),
};

void JStudio_JStage::TAdaptor_camera::adaptor_do_prepare() {
    TVVOutput* pOutput = saoVVOutput_;
    while (!pOutput->isEnd_()) {
        pOutput->adaptor_setOutput_(this);
        pOutput++;
    }
}

void JStudio_JStage::TAdaptor_camera::adaptor_do_begin() {
    adaptor_object_begin_();
    JStage::TCamera* pCamera = get_pJSG_();
    const JStudio::TObject* pObject = adaptor_getObject();
    JUT_ASSERT(70, pObject != NULL);
    const JStudio::TControl* pControl = pObject->getControl();
    JUT_ASSERT(72, pControl != NULL);
    getJSG_position_(pControl);
    getJSG_targetPosition_(pControl);
    TVVOutput* pOutput = saoVVOutput_;
    while (!pOutput->isEnd_()) {
        pOutput->setVariableValue_(pCamera, this);
        pOutput++;
    }
}

void JStudio_JStage::TAdaptor_camera::adaptor_do_end() {
    adaptor_object_end_();
}

void JStudio_JStage::TAdaptor_camera::adaptor_do_update(u32 param_1) {
    const JStudio::TObject* pObject = adaptor_getObject();
    JUT_ASSERT(98, pObject != NULL);
    const JStudio::TControl* pControl = pObject->getControl();
    JUT_ASSERT(100, pControl != NULL);
    setJSG_position_(pControl);
    setJSG_targetPosition_(pControl);
    pJSGObject_->JSGUpdate();
}

void JStudio_JStage::TAdaptor_camera::adaptor_do_data(void const* param_1, u32 param_2,
                                                      void const* param_3, u32 param_4) {
    adaptor_object_data_(param_1, param_2, param_3, param_4);
}

void JStudio_JStage::TAdaptor_camera::adaptor_do_PARENT(JStudio::data::TEOperationData param_1,
                                                        void const* pContent, u32 uSize) {
    switch(param_1) {
    case JStudio::data::UNK_0x18:
        JUT_ASSERT(132, pContent!=NULL);
        JUT_ASSERT(133, uSize!=0);
        field_0x10c = adaptor_object_findJSGObject_((char*)pContent);
        break;
    }
}

void JStudio_JStage::TAdaptor_camera::adaptor_do_PARENT_NODE(JStudio::data::TEOperationData param_1,
                                                        void const* pContent, u32 uSize) {
    switch (param_1) {
    case JStudio::data::UNK_0x18:
        JUT_ASSERT(155, pContent!=NULL);
        JUT_ASSERT(156, uSize!=0);
        field_0x110 = adaptor_object_findJSGObjectNode_(field_0x10c, (char*)pContent);
        break;
    case JStudio::data::UNK_0x19:
        JUT_ASSERT(166, uSize==4);
        JUT_ASSERT(167, pContent!=NULL);
        field_0x110 = *(int*)pContent;
        break;
    }
}

void
JStudio_JStage::TAdaptor_camera::adaptor_do_PARENT_ENABLE(JStudio::data::TEOperationData param_1,
                                                          void const* pContent, u32 uSize) {
    switch (param_1) {
    case JStudio::data::UNK_0x2:
        JUT_ASSERT(185, uSize==4);
        JUT_ASSERT(186, pContent!=NULL);
        field_0x114 = *(u32*)pContent ? true : false;
        switch(field_0x108) {
        case 1: {
            JStage::TObject* parent = 0;
            int idx = -1;
            if (field_0x114 != 0) {
                parent = field_0x10c;
                idx = field_0x110;
            }
            pJSGObject_->JSGSetParent(parent, idx);
            break;
        }
        case 0:
            break;
        }
        break;
    }
}

void
JStudio_JStage::TAdaptor_camera::adaptor_do_PARENT_FUNCTION(JStudio::data::TEOperationData param_1,
                                                          void const* pContent, u32 uSize) {
    switch(param_1) {
    case JStudio::data::UNK_0x2:
        JUT_ASSERT(226, uSize==4);
        JUT_ASSERT(227, pContent!=NULL);
        field_0x108 = *(int*)pContent;
        break;
    }
}

void JStudio_JStage::TAdaptor_camera::adaptor_do_TARGET_PARENT(
    JStudio::data::TEOperationData param_1, void const* pContent, u32 uSize) {
    switch(param_1) {
    case JStudio::data::UNK_0x18:
        JUT_ASSERT(247, pContent!=NULL);
        JUT_ASSERT(248, uSize!=0);
        field_0x118 = adaptor_object_findJSGObject_((char*)pContent);
        break;
    }
}

void JStudio_JStage::TAdaptor_camera::adaptor_do_TARGET_PARENT_NODE(
    JStudio::data::TEOperationData param_1, void const* pContent, u32 uSize) {
    switch (param_1) {
    case JStudio::data::UNK_0x18:
        JUT_ASSERT(270, pContent!=NULL);
        JUT_ASSERT(271, uSize!=0);
        field_0x11c = adaptor_object_findJSGObjectNode_(field_0x118, (char*)pContent);
        break;
    case JStudio::data::UNK_0x19:
        JUT_ASSERT(282, uSize==4);
        JUT_ASSERT(283, pContent!=NULL);
        field_0x11c = *(int*)pContent;
        break;
    }
}

void JStudio_JStage::TAdaptor_camera::adaptor_do_TARGET_PARENT_ENABLE(
    JStudio::data::TEOperationData param_1, void const* pContent, u32 uSize) {
    switch(param_1) {
    case JStudio::data::UNK_0x2:
        JUT_ASSERT(301, uSize==4);
        JUT_ASSERT(302, pContent!=NULL);
        field_0x120 = *(int*)pContent ? 1 : 0;
        break;
    }
}

void JStudio_JStage::TAdaptor_camera::setJSG_position_(JStudio::TControl const* param_1) {
    JStudio::TControl::TTransform_position VStack_138;
    JStudio::TControl::TTransform_position VStack_144;
    adaptor_getVariableValue_Vec(&VStack_138, sauVariableValue_3_POSITION_XYZ);
    const Vec* pViewPosition;
    if (field_0x114 == 0) {
        pViewPosition = param_1->transformOnSet_transform_ifEnabled(VStack_138, &VStack_144);
    } else {
        pViewPosition = &VStack_138;
        switch (field_0x108) {
        case 0: {
            int iVar1 =
                transform_toGlobalFromLocal(&VStack_144, VStack_138, field_0x10c, field_0x110);
            if (iVar1 == 0) {
                return;
            }
            pViewPosition = &VStack_144;
            break;
        }
        case 1:
            break;
        }
    }
    get_pJSG_()->JSGSetViewPosition(*pViewPosition);
}

void JStudio_JStage::TAdaptor_camera::getJSG_position_(JStudio::TControl const* param_1) {
    Mtx mtx;
    JStudio::TControl::TTransform_position transform;
    JStudio::TControl::TTransform_position tempTransform;
    const JStudio::TControl::TTransform_position* outTransform;

    get_pJSG_()->JSGGetViewPosition(&transform);

    if (!field_0x114) {
        outTransform = param_1->transformOnGet_transform_ifEnabled(transform, &tempTransform);
    } else {
        outTransform = &transform;
        switch (field_0x108) {
        case 0:
            bool check;
            if (!transform_toLocalFromGlobal(mtx, *outTransform, field_0x10c, field_0x110)) {
                check = false;
            } else {
                tempTransform.x = mtx[0][3];
                tempTransform.y = mtx[1][3];
                tempTransform.z = mtx[2][3];
                check = true;
            }

            if (!check) {
                return;
            }
            outTransform = &tempTransform;
            break;
        case 1:
            break;
        }
    }

    adaptor_setVariableValue_Vec(sauVariableValue_3_POSITION_XYZ, *outTransform);
}

void JStudio_JStage::TAdaptor_camera::setJSG_targetPosition_(JStudio::TControl const* param_1) {
	JStudio::TControl::TTransform_position transform;
	JStudio::TControl::TTransform_position tempTransform;
	const JStudio::TControl::TTransform_position* outTransform;
	adaptor_getVariableValue_Vec(&transform, sauVariableValue_3_TARGET_POSITION_XYZ);
    if (field_0x120 == 0) {
        outTransform = param_1->transformOnSet_transform_ifEnabled(
            transform, &tempTransform);
    } else {
        if (transform_toGlobalFromLocal(&tempTransform, transform,
                                            field_0x118, field_0x11c) == 0) {
            return;
        }
        outTransform = &tempTransform;
    }
    get_pJSG_()->JSGSetViewTargetPosition(*outTransform);
}

void JStudio_JStage::TAdaptor_camera::getJSG_targetPosition_(JStudio::TControl const* param_1) {
    Mtx mtx;
    JStudio::TControl::TTransform_position transform;
    JStudio::TControl::TTransform_position tempTransform;
    const JStudio::TControl::TTransform_position* outTransform;

    get_pJSG_()->JSGGetViewTargetPosition(&transform);

    if (!field_0x120) {
        outTransform = param_1->transformOnGet_transform_ifEnabled(transform, &tempTransform);
    } else {
        outTransform = &transform;
        bool check;
        if (!transform_toLocalFromGlobal(mtx, *outTransform, field_0x118, field_0x11c)) {
            check = false;
        } else {
            tempTransform.x = mtx[0][3];
            tempTransform.y = mtx[1][3];
            tempTransform.z = mtx[2][3];
            check = true;
        }

        if (!check) {
            return;
        }
        outTransform = &tempTransform;
    }

    adaptor_setVariableValue_Vec(sauVariableValue_3_TARGET_POSITION_XYZ, *outTransform);
}
