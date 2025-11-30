#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/J3DGraphBase/J3DShape.h"
#include "JSystem/J3DGraphBase/J3DPacket.h"
#include "JSystem/J3DGraphBase/J3DVertex.h"
#include "JSystem/J3DGraphBase/J3DFifo.h"
#include <dolphin/gd.h>

void J3DGDSetVtxAttrFmtv(_GXVtxFmt, GXVtxAttrFmtList const*, bool);
void J3DFifoLoadPosMtxImm(Mtx, u32);
void J3DFifoLoadNrmMtxImm(Mtx, u32);

void J3DShape::initialize() {
    mMaterial = NULL;
    mIndex = -1;
    mMtxGroupNum = 0;
    mFlags = 0;
    mRadius = 0.0f;
    mMin.x = 0.0f;
    mMin.y = 0.0f;
    mMin.z = 0.0f;
    mMax.x = 0.0f;
    mMax.y = 0.0f;
    mMax.z = 0.0f;
    mVtxDesc = NULL;
    mShapeMtx = NULL;
    mShapeDraw = NULL;
    mVertexData = NULL;
    mDrawMtxData = NULL;
    mScaleFlagArray = NULL;
    mDrawMtx = NULL;
    mNrmMtx = NULL;
    mCurrentViewNo = &j3dDefaultViewNo;
    mHasNBT = false;
    mHasPNMTXIdx = false;
}

void J3DShape::addTexMtxIndexInDL(GXAttr attr, u32 valueBase) {
    u32 kSize[] = {0, 1, 1, 2};

    s32 pnmtxidxOffs = -1;
    s32 attrOffs = -1;
    s32 stride = 0;
    bool found = false;

    for (GXVtxDescList* vtxDesc = getVtxDesc(); vtxDesc->attr != GX_VA_NULL; vtxDesc++) {
        if (vtxDesc->attr == GX_VA_PNMTXIDX)
            pnmtxidxOffs = stride;

        if (attr < vtxDesc->attr && !found) {
            attrOffs = stride;
            found = true;
        }

        stride = stride + kSize[vtxDesc->type];
    }

    if (pnmtxidxOffs == -1)
        return;

    for (u16 i = 0; i < (u16)getMtxGroupNum(); i++)
        getShapeDraw(i)->addTexMtxIndexInDL(stride, attrOffs, (s32)valueBase);
}

void J3DShape::addTexMtxIndexInVcd(GXAttr attr) {
    u32 kSize[] = {0, 1, 1, 2};  // stripped data

    s32 attrIdx = -1;
    s32 attrOffs = -1;
    s32 stride = 0;

    GXVtxDescList* vtxDesc = getVtxDesc();
    s32 attrCount = 0;

    for (; vtxDesc->attr != GX_VA_NULL; attrCount++, vtxDesc++) {
        if (vtxDesc->attr == GX_VA_PNMTXIDX)
            attrIdx = stride;
    }

    if (attrIdx == -1)
        return;

    GXVtxDescList* newVtxDesc = new GXVtxDescList[attrCount + 2];
    bool inserted = false;

    vtxDesc = getVtxDesc();
    GXVtxDescList* dst = newVtxDesc;
    for (; vtxDesc->attr != GX_VA_NULL; dst++, vtxDesc++) {
        if ((attr < vtxDesc->attr) && !inserted) {
            dst->attr = attr;
            dst->type = GX_DIRECT;
            attrOffs = stride;
            dst++;

            inserted = true;
        }

        dst->attr = vtxDesc->attr;
        dst->type = vtxDesc->type;
        stride = stride + kSize[vtxDesc->type];
    }

    dst->attr = GX_VA_NULL;
    dst->type = GX_NONE;
    mVtxDesc = newVtxDesc;
    makeVcdVatCmd();
}

void J3DShape::calcNBTScale(const Vec& param_0, f32 (*param_1)[3][3], f32 (*param_2)[3][3]) {
    for (u16 i = 0; i < mMtxGroupNum; i++)
        mShapeMtx[i]->calcNBTScale(param_0, param_1, param_2);
}

u16 J3DShape::countBumpMtxNum() const {
    u16 num = 0;
    for (u16 i = 0; i < mMtxGroupNum; i++)
        num += mShapeMtx[i]->getUseMtxNum();

    return num;
}

void J3DLoadCPCmd(u8 addr, u32 val) {
    GXCmd1u8(GX_LOAD_CP_REG);
    GXCmd1u8(addr);
    GXCmd1u32(val);
}

static void J3DLoadArrayBasePtr(GXAttr attr, void* data) {
    u32 idx = (attr == GX_VA_NBT) ? 1 : (attr - GX_VA_POS);
    J3DLoadCPCmd(0xA0 + idx, ((uintptr_t)data & 0x7FFFFFFF));
}

void J3DShape::loadVtxArray() const {
    J3DLoadArrayBasePtr(GX_VA_POS, j3dSys.getVtxPos());

    if (!mHasNBT) {
        J3DLoadArrayBasePtr(GX_VA_NRM, j3dSys.getVtxNrm());
    }

    J3DLoadArrayBasePtr(GX_VA_CLR0, j3dSys.getVtxCol());
}

bool J3DShape::isSameVcdVatCmd(J3DShape* other) {
    u8* a = (u8*)other->getVcdVatCmd();
    u8* b = mVcdVatCmd;
    for (u32 i = 0; i < kVcdVatDLSize; i++)
        if (a[i] != b[i])
            return false;

    return true;
}

void J3DShape::makeVtxArrayCmd() {
    GXVtxAttrFmtList* vtxAttr = mVertexData->getVtxAttrFmtList();

    u8 stride[12];
    void* array[12];
    for (u32 i = 0; i < 12; i++) {
        stride[i] = 0;
        array[i] = 0;
    }

    for (; vtxAttr->attr != GX_VA_NULL; vtxAttr++) {
        switch (vtxAttr->attr) {
        case GX_VA_POS: {
            if (vtxAttr->type == GX_F32)
                stride[vtxAttr->attr - GX_VA_POS] = 12;
            else
                stride[vtxAttr->attr - GX_VA_POS] = 6;

            array[vtxAttr->attr - GX_VA_POS] = mVertexData->getVtxPosArray();
            mVertexData->setVtxPosFrac((u8)vtxAttr->frac);
            mVertexData->setVtxPosType((GXCompType)vtxAttr->type);
        } break;
        case GX_VA_NRM: {
            if (vtxAttr->type == GX_F32)
                stride[vtxAttr->attr - GX_VA_POS] = 12;
            else
                stride[vtxAttr->attr - GX_VA_POS] = 6;

            array[vtxAttr->attr - GX_VA_POS] = mVertexData->getVtxNrmArray();
            mVertexData->setVtxNrmFrac((u8)vtxAttr->frac);
            mVertexData->setVtxNrmType((GXCompType)vtxAttr->type);
        } break;
        case GX_VA_CLR0:
        case GX_VA_CLR1: {
            stride[vtxAttr->attr - GX_VA_POS] = 4;
            array[vtxAttr->attr - GX_VA_POS] = mVertexData->getVtxColorArray(vtxAttr->attr - GX_VA_CLR0);
        } break;
        case GX_VA_TEX0:
        case GX_VA_TEX1:
        case GX_VA_TEX2:
        case GX_VA_TEX3:
        case GX_VA_TEX4:
        case GX_VA_TEX5:
        case GX_VA_TEX6:
        case GX_VA_TEX7: {
            if (vtxAttr->type == GX_F32)
                stride[vtxAttr->attr - GX_VA_POS] = 8;
            else
                stride[vtxAttr->attr - GX_VA_POS] = 4;

            array[vtxAttr->attr - GX_VA_POS] = mVertexData->getVtxTexCoordArray(vtxAttr->attr - GX_VA_TEX0);
        } break;
        default:
            break;
        }
    }

    GXVtxDescList* vtxDesc = mVtxDesc;
    mHasPNMTXIdx = false;
    for (; vtxDesc->attr != GX_VA_NULL; vtxDesc++) {
        if (vtxDesc->attr == GX_VA_NBT && vtxDesc->type != GX_NONE) {
            mHasNBT = true;
            stride[GX_VA_NRM - GX_VA_POS] *= 3;
            array[GX_VA_NRM - GX_VA_POS] = mVertexData->getVtxNBTArray();
        } else if (vtxDesc->attr == GX_VA_PNMTXIDX && vtxDesc->type != GX_NONE) {
            mHasPNMTXIdx = true;
        }
    }

    for (u32 i = 0; i < 12; i++) {
        if (array[i] != 0)
            GDSetArray((GXAttr)(i + GX_VA_POS), array[i], stride[i]);
        else
            GDSetArrayRaw((GXAttr)(i + GX_VA_POS), NULL, stride[i]);
    }
}

void J3DShape::makeVcdVatCmd() {
    static BOOL sInterruptFlag = OSDisableInterrupts();
    OSDisableScheduler();

    GDLObj gdl_obj;
    GDInitGDLObj(&gdl_obj, mVcdVatCmd, kVcdVatDLSize);
    GDSetCurrent(&gdl_obj);
    GDSetVtxDescv(mVtxDesc);
    makeVtxArrayCmd();
    J3DGDSetVtxAttrFmtv(GX_VTXFMT0, mVertexData->getVtxAttrFmtList(), mHasNBT);
    GDPadCurr32();
    GDFlushCurrToMem();
    GDSetCurrent(NULL);
    OSEnableScheduler();
    OSRestoreInterrupts(sInterruptFlag);
}

void* J3DShape::sOldVcdVatCmd;

void J3DShape::loadCurrentMtx() const {
    mCurrentMtx.load();
}

void J3DShape::loadPreDrawSetting() const {
    if (sOldVcdVatCmd != mVcdVatCmd) {
        GXCallDisplayList(mVcdVatCmd, kVcdVatDLSize);
        sOldVcdVatCmd = mVcdVatCmd;
    }

    mCurrentMtx.load();
}

bool J3DShape::sEnvelopeFlag;

void J3DShape::setArrayAndBindPipeline() const {
    J3DShapeMtx::setCurrentPipeline((mFlags & 0x1C) >> 2);
    loadVtxArray();
    j3dSys.setModelDrawMtx(mDrawMtx[*mCurrentViewNo]);
    j3dSys.setModelNrmMtx(mNrmMtx[*mCurrentViewNo]);
    J3DShapeMtx::sCurrentScaleFlag = mScaleFlagArray;
    J3DShapeMtx::sNBTFlag = mHasNBT;
    sEnvelopeFlag = mHasPNMTXIdx;
    J3DShapeMtx::sTexMtxLoadType = getTexMtxLoadType();
}

void J3DShape::drawFast() const {
    if (sOldVcdVatCmd != mVcdVatCmd) {
        GXCallDisplayList(mVcdVatCmd, kVcdVatDLSize);
        sOldVcdVatCmd = mVcdVatCmd;
    }

    if (sEnvelopeFlag != 0 && !mHasPNMTXIdx)
        mCurrentMtx.load();

    setArrayAndBindPipeline();
    if (!checkFlag(J3DShpFlag_NoMtx)) {
        if (J3DShapeMtx::getLODFlag())
            J3DShapeMtx::resetMtxLoadCache();

        for (u16 n = mMtxGroupNum, i = 0; i < n; i++) {
            if (mShapeMtx[i] != NULL)
                mShapeMtx[i]->load();
            if (mShapeDraw[i] != NULL)
                mShapeDraw[i]->draw();
        }
    } else {
        J3DFifoLoadPosMtxImm(*j3dSys.getShapePacket()->getBaseMtxPtr(), GX_PNMTX0);
        J3DFifoLoadNrmMtxImm(*j3dSys.getShapePacket()->getBaseMtxPtr(), GX_PNMTX0);
        for (u16 n = mMtxGroupNum, i = 0; i < n; i++)
            if (mShapeDraw[i] != NULL)
                mShapeDraw[i]->draw();
    }
}

void J3DShape::draw() const {
    resetVcdVatCache();
    loadPreDrawSetting();
    drawFast();
}

void J3DShape::simpleDraw() const {
    resetVcdVatCache();
    loadPreDrawSetting();
    J3DShapeMtx::setCurrentPipeline((mFlags & 0x1C) >> 2);
    loadVtxArray();
    for (u16 n = mMtxGroupNum, i = 0; i < n; i++) {
        if (mShapeDraw[i] != NULL) {
            mShapeDraw[i]->draw();
        }
    }
}

void J3DShape::simpleDrawCache() const {
    if (sOldVcdVatCmd != mVcdVatCmd) {
        GXCallDisplayList(mVcdVatCmd, kVcdVatDLSize);
        sOldVcdVatCmd = mVcdVatCmd;
    }

    if (sEnvelopeFlag && !mHasPNMTXIdx)
        mCurrentMtx.load();

    loadVtxArray();
    for (u16 n = mMtxGroupNum, i = 0; i < n; i++)
        if (mShapeDraw[i] != NULL)
            mShapeDraw[i]->draw();
}
