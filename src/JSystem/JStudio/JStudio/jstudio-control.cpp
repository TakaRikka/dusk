//
// jstudio-control
//

#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JStudio/JStudio/jstudio-control.h"
#include "JSystem/JStudio/JStudio/jstudio-math.h"
#include "JSystem/JStudio/JStudio/jstudio-data.h"
#include "string.h"

JStudio::TControl::TControl() {
    mTransformOnSet = false;
    mTransformOnGet = false;
    Vec aVStack_20 = {0.0f,0.0f,0.0f};
    transform_setOrigin_TxyzRy(aVStack_20, 0.0f);
}

JStudio::TControl::~TControl() {}

void JStudio::TControl::setFactory(JStudio::TFactory* factory) {
    fvb::TFactory* fvbFactory = NULL;
    ctb::TFactory* ctbFactory = NULL;
    if (factory != NULL) {
        fvbFactory = &factory->fvb_Factory;
        ctbFactory = &factory->ctb_Factory;
    }
    stb::TControl::setFactory(factory);
    fvb_Control.setFactory(fvbFactory);
    ctb_Control.setFactory(ctbFactory);
}

int JStudio::TControl::transformOnSet_setOrigin_TxyzRy(Vec const& param_0, f32 param_1) {
    field_0x8c = param_0;
    mTransformOnSet_RotationY = param_1;
    JStudio::math::getTransformation_RyT(mTransformOnSet_Matrix, param_0, param_1);
    return 1;
}

int JStudio::TControl::transformOnGet_setOrigin_TxyzRy(Vec const& param_0, f32 param_1) {
    field_0x98 =  param_0;
    mTransformOnGet_RotationY = param_1;
    Mtx afStack_48;
    MTXTrans(afStack_48, -param_0.x, -param_0.y, -param_0.z);
    math::rotate_y(mTransformOnGet_Matrix, afStack_48, -param_1);
    return 1;
}

int JStudio::TControl::transform_setOrigin_ctb(JStudio::ctb::TObject const& param_0) {
    switch (param_0.getScheme()) {
    case 1:
        const f32* pfVar4 = (const f32*)param_0.getData();
        Vec local_144 = {0.0f, 0.0f, 0.0f};
        local_144.x = pfVar4[0];
        local_144.y = pfVar4[1];
        local_144.z = pfVar4[2];
        transform_setOrigin_TxyzRy(local_144, pfVar4[3]);
        break;
    default:
        return 0;
    }
    return 1;
}

bool JStudio::TControl::transform_setOrigin_ctb_index(u32 index) {
    ctb::TObject* object = ctb_getObject_index(index);
    if (object == NULL) {
        return false;
    }
    transform_setOrigin_ctb(*object);
    return true;
}

JStudio::TCreateObject::~TCreateObject() {}

JStudio::TFactory::~TFactory() {}

void JStudio::TFactory::appendCreateObject(JStudio::TCreateObject* param_0) {
    mList.Push_back(param_0);
}

JStudio::TObject* JStudio::TFactory::create(JStudio::stb::data::TParse_TBlock_object const& param_0) {
    JGadget::TContainerEnumerator<TCreateObject, -4> aTStack_368(&mList);
    while(aTStack_368) {
        TCreateObject& piVar1 = *aTStack_368;
        JStudio::TObject* obj;
        if (piVar1.create(&obj, param_0)) {
            return obj;
        }
    }
    return NULL;
}

JStudio::TParse::TParse(JStudio::TControl* param_0) : stb::TParse(param_0) {
}

JStudio::TParse::~TParse() {}

bool JStudio::TParse::parseHeader(JStudio::stb::data::TParse_THeader const& param_0,
                                      u32 param_1) {
    const JStudio::stb::data::THeader::Target& target = param_0.get_target();
    if (memcmp(target.name, JStudio::data::ga8cSignature, sizeof(JStudio::data::ga8cSignature)) != 0) {
        return false;
    }
    if (target.target_version < 2) {
        return false;
    } 
    if (target.target_version > 6) {
        return false;
    }
    JStudio::TControl* pControl = getControl();
    JUT_ASSERT(322, pControl!=NULL);
    if ((param_1 & 0x100) == 0) {
        Vec local_468 = {0.0f, 0.0f, 0.0f};
        pControl->transform_setOrigin_TxyzRy(local_468, 0.0f);
        pControl->transform_enable(false);
    }
    return true;
}

bool JStudio::TParse::parseBlock_block(JStudio::stb::data::TParse_TBlock const& param_0,
                                           u32 param_1) {
    switch(param_0.get_type()) {
    case 'JFVB':
        return parseBlock_block_fvb_(param_0, param_1);
    case 'JCTB':
        return parseBlock_block_ctb_(param_0, param_1);
    default:
        return stb::TParse::parseBlock_block(param_0, param_1);
    }
}

bool JStudio::TParse::parseBlock_block_fvb_(JStudio::stb::data::TParse_TBlock const& param_0,
                                                u32 param_1) {
    TControl* pControl = getControl();
    JUT_ASSERT(361, pControl!=NULL);
    const void* pContent = param_0.getContent();
    JUT_ASSERT(363, pContent!=NULL);
    u32 uVar4 = param_1 & 0xf;
    if ((param_1 & 0x10) != 0) {
        uVar4 |= 0x10;
    }
    if ((param_1 & 0x20) != 0) {
        uVar4 |= 0x20;
    }
    if ((param_1 & 0x40) != 0) {
        uVar4 |= 0x40;
    }
    fvb::TParse aTStack_30(&pControl->fvb_Control);
    if (!aTStack_30.parse(pContent, uVar4)) {
        return false;
    } 
    return true;
}

bool JStudio::TParse::parseBlock_block_ctb_(JStudio::stb::data::TParse_TBlock const& param_0,
                                                u32 param_1) {
    TControl* pControl = getControl();
    JUT_ASSERT(386, pControl!=NULL);
    const void* pContent = param_0.getContent();
    JUT_ASSERT(388, pContent!=NULL);
    u32 uVar4 = param_1 & 0xf;
    if ((param_1 & 0x10) != 0) {
        uVar4 |= 0x10;
    }
    if ((param_1 & 0x20) != 0) {
        uVar4 |= 0x20;
    }
    if ((param_1 & 0x40) != 0) {
        uVar4 |= 0x40;
    }
    ctb::TParse aTStack_30(&pControl->ctb_Control);
    if (!aTStack_30.parse(pContent, uVar4)) {
        return false;
    }
    if ((param_1 & 0x200) == 0) {
        if (pControl->transform_setOrigin_ctb_index(0) == 0) {
            return 0;
        }
        pControl->transform_enable(true);
    }
    return true;
}
