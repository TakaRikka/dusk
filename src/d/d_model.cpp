#include "d/dolzel.h" // IWYU pragma: keep

#include "d/d_model.h"
#include "JSystem/J3DGraphBase/J3DDrawBuffer.h"
#include "JSystem/J3DGraphBase/J3DMaterial.h"
#include "d/d_com_inf_game.h"

void dMdl_c::draw() {
    j3dSys.setVtxPos(mpModelData->getVtxPosArray());
    j3dSys.setVtxNrm(mpModelData->getVtxNrmArray());
    j3dSys.setVtxCol(mpModelData->getVtxColorArray(0));
    J3DShape::resetVcdVatCache();

    J3DShape* shape = mpModelData->getMaterialNodePointer(mMaterialId)->getShape();
    mpModelData->getMaterialNodePointer(mMaterialId)->loadSharedDL();
    shape->loadPreDrawSetting();

    GXColor amb_color = {mpTevstr->AmbCol.r, mpTevstr->AmbCol.g, mpTevstr->AmbCol.b,
                         mpTevstr->AmbCol.a};
    GXSetChanAmbColor(GX_COLOR0A0, amb_color);
    GXSetChanMatColor(GX_COLOR0A0, g_whiteColor);
    dKy_setLight_nowroom_actor(mpTevstr);
    dKy_setLight_again();
    dKy_GxFog_tevstr_set(mpTevstr);

    Mtx m;
    for (dMdl_obj_c* obj = mpModelObj; obj != NULL; obj = obj->mpObj) {
        MTXConcat(j3dSys.getViewMtx(), obj->getMtx(), m);
        GXLoadPosMtxImm(m, GX_PNMTX0);
        GXLoadNrmMtxImm(m, GX_PNMTX0);
        shape->simpleDrawCache();
    }

    field_0x1a = false;
}

void dMdl_c::create(J3DModelData* i_modelData, u16 i_materialId, dKy_tevstr_c* i_tevstr) {
    mpModelData = i_modelData;
    mMaterialId = i_materialId;
    mpTevstr = i_tevstr;
    field_0x1a = false;
}

void dMdl_c::entryObj(dMdl_obj_c* i_obj) {
    if (!field_0x1a) {
        dComIfGd_getListPacket()->entryImm(this, 0);
        field_0x1a = true;
        mpModelObj = NULL;
    }

    i_obj->mpObj = mpModelObj;
    mpModelObj = i_obj;
}

dMdl_c* dMdl_mng_c::search(J3DModelData* i_modelData, u16 i_materialId, dKy_tevstr_c* i_tevstr) {
    dMdl_c* model = field_0x0;
    for (int i = field_0x80; i > 0; i--) {
        if (model->getModelData() == i_modelData && model->getMaterialId() == i_materialId &&
            model->getTevstr() == i_tevstr) {
            return model;
        }
        model++;
    }
    return NULL;
}

dMdl_c* dMdl_mng_c::entry(J3DModelData* i_modelData, u16 i_materialId, dKy_tevstr_c* i_tevstr) {
    dMdl_c* model = search(i_modelData, i_materialId, i_tevstr);

    if (model == NULL) {
        if (field_0x80 >= 4) {
            return NULL;
        } else {
            model = &field_0x0[field_0x80];
            model->create(i_modelData, i_materialId, i_tevstr);
            field_0x80++;
        }
    }

    return model;
}

dMdl_mng_c* dMdl_mng_c::m_myObj;

void dMdl_mng_c::create() {
    m_myObj = new dMdl_mng_c();
}

dMdl_c::~dMdl_c() {}

void dMdl_mng_c::remove() {
    if (m_myObj != NULL) {
        delete m_myObj;
        m_myObj = NULL;
    }
}

void dMdl_mng_c::reset() {
    if (m_myObj == NULL) {
        return;
    }

    m_myObj->field_0x80 = 0;
}

dMdl_c* dMdl_mng_c::entry(J3DModelData* i_modelData, u16 i_materialId, int i_roomNo) {
    if (m_myObj == NULL) {
        return NULL;
    }
    dKy_tevstr_c* tevstr = dComIfGp_roomControl_getTevStr(i_roomNo);
    return m_myObj->entry(i_modelData, i_materialId, tevstr);
}
