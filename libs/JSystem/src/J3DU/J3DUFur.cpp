#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/J3DU/J3DUFur.h"

#include "JSystem/J3DGraphAnimator/J3DModel.h"
#include "JSystem/J3DGraphBase/J3DMaterial.h"
#include "JSystem/J3DGraphBase/J3DVertex.h"

static void dummy(J3DShape* shape,  J3DVertexBuffer* vtxBuf, J3DModel* model, J3DModelData* modelData) {
    J3DMaterial* material = shape->getMaterial();
    J3DJoint* joint = material->getJoint();

    vtxBuf->swapTransformedVtxPos();
    J3DVertexData* vtxData = vtxBuf->getVertexData();
    vtxBuf->getTransformedVtxPos(0);
    vtxBuf->getCurrentVtxNrm();
    vtxBuf->setCurrentVtxPos(NULL);

    model->getVertexBuffer();

    vtxData->getNrmNum();
    modelData->getNrmNum();
    vtxData->getVtxNum();
    modelData->getVtxNum();
}
