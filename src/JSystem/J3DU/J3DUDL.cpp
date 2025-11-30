#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/J3DU/J3DUDL.h"
#include "JSystem/J3DGraphBase/J3DShapeMtx.h"

// Fake function to make the weak function J3DShapeMtx::getUseMtxNum appear in this TU
u32 J3DUGetUseMtxNum(J3DShapeMtx* i_shapeMtx) {
    return i_shapeMtx->getUseMtxNum();
}
