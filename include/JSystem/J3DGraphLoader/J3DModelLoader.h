#ifndef J3DMODELLOADER_H
#define J3DMODELLOADER_H

#include "dolphin/mtx.h"

class J3DModelData;
class J3DMaterialTable;
class J3DModelHierarchy;

/**
 * @ingroup jsystem-j3d
 * 
 */
struct J3DModelBlock {
    /* 0x00 */ u32 mBlockType;
    /* 0x04 */ u32 mBlockSize;
};

/**
 * @ingroup jsystem-j3d
 * 
 */
struct J3DModelFileData {
    /* 0x00 */ u32 mMagic1;
    /* 0x04 */ u32 mMagic2;
    /* 0x08 */ u8 field_0x08[4];
    /* 0x0C */ u32 mBlockNum;
    /* 0x10 */ u8 field_0x10[0x10];
    /* 0x20 */ J3DModelBlock mBlocks[1];
};

/**
 * @ingroup jsystem-j3d
 * 
 */
struct J3DModelInfoBlock : public J3DModelBlock {
    /* 0x08 */ u16 mFlags;
    /* 0x0C */ u32 mPacketNum;
    /* 0x10 */ u32 mVtxNum;
    /* 0x14 */ void* mpHierarchy;
}; // size 0x18

/**
 * @ingroup jsystem-j3d
 * 
 */
struct J3DVertexBlock : public J3DModelBlock {
    /* 0x08 */ void* mpVtxAttrFmtList;
    /* 0x0C */ void* mpVtxPosArray;
    /* 0x10 */ void* mpVtxNrmArray;
    /* 0x14 */ void* mpVtxNBTArray;
    /* 0x18 */ void* mpVtxColorArray[2];
    /* 0x20 */ void* mpVtxTexCoordArray[8];
}; // size 0x40

/**
 * @ingroup jsystem-j3d
 * 
 */
struct J3DEnvelopeBlock : public J3DModelBlock {
    /* 0x08 */ u16 mWEvlpMtxNum;
    /* 0x0C */ void* mpWEvlpMixMtxNum;
    /* 0x10 */ void* mpWEvlpMixIndex;
    /* 0x14 */ void* mpWEvlpMixWeight;
    /* 0x18 */ void* mpInvJointMtx;
}; // size 0x1C

/**
 * @ingroup jsystem-j3d
 * 
 */
struct J3DDrawBlock : public J3DModelBlock {
    /* 0x08 */ u16 mMtxNum;
    /* 0x0C */ void* mpDrawMtxFlag;
    /* 0x10 */ void* mpDrawMtxIndex;
}; // size 0x14

/**
 * @ingroup jsystem-j3d
 * 
 */
struct J3DJointBlock : public J3DModelBlock {
    /* 0x08 */ u16 mJointNum;
    /* 0x0C */ void* mpJointInitData;
    /* 0x10 */ void* mpIndexTable;
    /* 0x14 */ void* mpNameTable;
}; // size 0x18

/**
 * @ingroup jsystem-j3d
 * 
 */
struct J3DMaterialBlock : public J3DModelBlock {
    /* 0x08 */ u16 mMaterialNum;
    /* 0x0C */ void* mpMaterialInitData;
    /* 0x10 */ void* mpMaterialID;
    /* 0x14 */ void* mpNameTable;
    /* 0x18 */ void* mpIndInitData;
    /* 0x1C */ void* mpCullMode;
    /* 0x20 */ void* mpMatColor;
    /* 0x24 */ void* mpColorChanNum;
    /* 0x28 */ void* mpColorChanInfo;
    /* 0x2C */ void* mpAmbColor;
    /* 0x30 */ void* mpLightInfo;
    /* 0x34 */ void* mpTexGenNum;
    /* 0x38 */ void* mpTexCoordInfo;
    /* 0x3C */ void* mpTexCoord2Info;
    /* 0x40 */ void* mpTexMtxInfo;
    /* 0x44 */ void* field_0x44;
    /* 0x48 */ void* mpTexNo;
    /* 0x4C */ void* mpTevOrderInfo;
    /* 0x50 */ void* mpTevColor;
    /* 0x54 */ void* mpTevKColor;
    /* 0x58 */ void* mpTevStageNum;
    /* 0x5C */ void* mpTevStageInfo;
    /* 0x60 */ void* mpTevSwapModeInfo;
    /* 0x64 */ void* mpTevSwapModeTableInfo;
    /* 0x68 */ void* mpFogInfo;
    /* 0x6C */ void* mpAlphaCompInfo;
    /* 0x70 */ void* mpBlendInfo;
    /* 0x74 */ void* mpZModeInfo;
    /* 0x78 */ void* mpZCompLoc;
    /* 0x7C */ void* mpDither;
    /* 0x80 */ void* mpNBTScaleInfo;
};

/**
 * @ingroup jsystem-j3d
 * 
 */
struct J3DMaterialBlock_v21 : public J3DModelBlock {
    /* 0x08 */ u16 mMaterialNum;
    /* 0x0C */ void* mpMaterialInitData;
    /* 0x10 */ void* mpMaterialID;
    /* 0x14 */ void* mpNameTable;
    /* 0x18 */ void* mpCullMode;
    /* 0x1C */ void* mpMatColor;
    /* 0x20 */ void* mpColorChanNum;
    /* 0x24 */ void* mpColorChanInfo;
    /* 0x28 */ void* mpTexGenNum;
    /* 0x2C */ void* mpTexCoordInfo;
    /* 0x30 */ void* mpTexCoord2Info;
    /* 0x34 */ void* mpTexMtxInfo;
    /* 0x38 */ void* field_0x38;
    /* 0x3C */ void* mpTexNo;
    /* 0x40 */ void* mpTevOrderInfo;
    /* 0x44 */ void* mpTevColor;
    /* 0x48 */ void* mpTevKColor;
    /* 0x4C */ void* mpTevStageNum;
    /* 0x50 */ void* mpTevStageInfo;
    /* 0x54 */ void* mpTevSwapModeInfo;
    /* 0x58 */ void* mpTevSwapModeTableInfo;
    /* 0x5C */ void* mpFogInfo;
    /* 0x60 */ void* mpAlphaCompInfo;
    /* 0x64 */ void* mpBlendInfo;
    /* 0x68 */ void* mpZModeInfo;
    /* 0x6C */ void* mpZCompLoc;
    /* 0x70 */ void* mpDither;
    /* 0x74 */ void* mpNBTScaleInfo;
};

/**
 * @ingroup jsystem-j3d
 * 
 */
struct J3DMaterialDLBlock : public J3DModelBlock {
    /* 0x08 */ u16 mMaterialNum;
    /* 0x0C */ void* mpDisplayListInit;
    /* 0x10 */ void* mpPatchingInfo;
    /* 0x14 */ void* mpCurrentMtxInfo;
    /* 0x18 */ void* mpMaterialMode;
    /* 0x1C */ void* field_0x1c;
    /* 0x20 */ void* mpNameTable;
};

/**
 * @ingroup jsystem-j3d
 * 
 */
struct J3DShapeBlock : public J3DModelBlock {
    /* 0x08 */ u16 mShapeNum;
    /* 0x0C */ void* mpShapeInitData;
    /* 0x10 */ void* mpIndexTable;
    /* 0x14 */ void* mpNameTable;
    /* 0x18 */ void* mpVtxDescList;
    /* 0x1C */ void* mpMtxTable;
    /* 0x20 */ void* mpDisplayListData;
    /* 0x24 */ void* mpMtxInitData;
    /* 0x28 */ void* mpDrawInitData;
}; // size 0x2C

/**
 * @ingroup jsystem-j3d
 * 
 */
struct J3DTextureBlock : public J3DModelBlock {
    /* 0x08 */ u16 mTextureNum;
    /* 0x0C */ void* mpTextureRes;
    /* 0x10 */ void* mpNameTable;
};

enum J3DModelLoaderFlagTypes {
    J3DMLF_None = 0x00000000,

    J3DMLF_MtxSoftImageCalc = 0x00000001,
    J3DMLF_MtxMayaCalc = 0x00000002,
    J3DMLF_MtxBasicCalc = 0x00000004,
    J3DMLF_04 = 0x00000008,
    J3DMLF_MtxTypeMask = J3DMLF_MtxSoftImageCalc | J3DMLF_MtxMayaCalc | J3DMLF_MtxBasicCalc |
                         J3DMLF_04,  // 0 - 2 (0 = Basic, 1 = SoftImage, 2 = Maya)

    J3DMLF_UseImmediateMtx = 0x00000010,
    J3DMLF_UsePostTexMtx = 0x00000020,
    J3DMLF_07 = 0x00000040,
    J3DMLF_08 = 0x00000080,
    J3DMLF_NoMatrixTransform = 0x00000100,
    J3DMLF_10 = 0x00000200,
    J3DMLF_11 = 0x00000400,
    J3DMLF_12 = 0x00000800,
    J3DMLF_13 = 0x00001000,
    J3DMLF_DoBdlMaterialCalc = 0x00002000,
    J3DMLF_15 = 0x00004000,
    J3DMLF_16 = 0x00008000,
    J3DMLF_TevNumShift = 0x00010000,
    J3DMLF_18 = 0x00020000,
    J3DMLF_UseSingleSharedDL = 0x00040000,
    J3DMLF_20 = 0x00080000,
    J3DMLF_21 = 0x00100000,
    J3DMLF_UseUniqueMaterials = 0x00200000,
    J3DMLF_23 = 0x00400000,
    J3DMLF_24 = 0x00800000,
    J3DMLF_Material_UseIndirect = 0x01000000,
    J3DMLF_26 = 0x02000000,
    J3DMLF_27 = 0x04000000,
    J3DMLF_Material_TexGen_Block4 = 0x08000000,
    J3DMLF_Material_PE_Full = 0x10000000,
    J3DMLF_Material_PE_FogOff = 0x20000000,
    J3DMLF_Material_Color_LightOn = 0x40000000,
    J3DMLF_Material_Color_AmbientOn = 0x80000000
};

static inline u32 getMdlDataFlag_TevStageNum(u32 flags) { return (flags >> 0x10) & 0x1f; }
static inline u32 getMdlDataFlag_TexGenFlag(u32 flags) { return flags & 0x0c000000; }
static inline u32 getMdlDataFlag_ColorFlag(u32 flags) { return flags & 0xc0000000; }
static inline u32 getMdlDataFlag_PEFlag(u32 flags) { return flags & 0x30000000; }
static inline u32 getMdlDataFlag_MtxLoadType(u32 flags) { return flags & 0x10; }

/**
 * @ingroup jsystem-j3d
 * 
 */
class J3DModelLoader {
public:
    J3DModelLoader();
    void readInformation(J3DModelInfoBlock const*, u32);
    void readVertex(J3DVertexBlock const*);
    void readEnvelop(J3DEnvelopeBlock const*);
    void readDraw(J3DDrawBlock const*);
    void readJoint(J3DJointBlock const*);
    void readShape(J3DShapeBlock const*, u32);
    void readTexture(J3DTextureBlock const*);
    void readTextureTable(J3DTextureBlock const*);
    void readPatchedMaterial(J3DMaterialBlock const*, u32);
    void readMaterialDL(J3DMaterialDLBlock const*, u32);
    void modifyMaterial(u32);

    u32 calcSizeInformation(J3DModelInfoBlock const*, u32);
    u32 calcSizeJoint(J3DJointBlock const*);
    u32 calcSizeEnvelope(J3DEnvelopeBlock const*);
    u32 calcSizeDraw(J3DDrawBlock const*);
    u32 calcSizeShape(J3DShapeBlock const*, u32);
    u32 calcSizeTexture(J3DTextureBlock const*);
    u32 calcSizeTextureTable(J3DTextureBlock const*);
    u32 calcSizePatchedMaterial(J3DMaterialBlock const*, u32);
    u32 calcSizeMaterialDL(J3DMaterialDLBlock const*, u32);


    virtual J3DModelData* load(void const*, u32);
    virtual J3DMaterialTable* loadMaterialTable(void const*);
    virtual J3DModelData* loadBinaryDisplayList(void const*, u32);
    virtual u32 calcLoadSize(void const*, u32);
    virtual u32 calcLoadMaterialTableSize(void const*);
    virtual u32 calcLoadBinaryDisplayListSize(void const*, u32);
    virtual u16 countMaterialNum(void const*);
    virtual void setupBBoardInfo();
    virtual ~J3DModelLoader() {}
    virtual void readMaterial(J3DMaterialBlock const*, u32) {}
    virtual void readMaterial_v21(J3DMaterialBlock_v21 const*, u32) {}
    virtual void readMaterialTable(J3DMaterialBlock const*, u32) {}
    virtual void readMaterialTable_v21(J3DMaterialBlock_v21 const*, u32) {}
    virtual u32 calcSizeMaterial(J3DMaterialBlock const*, u32) { return false; }
    virtual u32 calcSizeMaterialTable(J3DMaterialBlock const*, u32) { return false; }

    /* 0x04 */ J3DModelData* mpModelData;
    /* 0x08 */ J3DMaterialTable* mpMaterialTable;
    /* 0x0C */ J3DShapeBlock const* mpShapeBlock;
    /* 0x10 */ J3DMaterialBlock const* mpMaterialBlock;
    /* 0x14 */ J3DModelHierarchy* mpModelHierarchy;
    /* 0x18 */ u8 field_0x18;
    /* 0x19 */ u8 field_0x19;
    /* 0x1A */ u16 mEnvelopeSize;
};

/**
 * @ingroup jsystem-j3d
 * 
 */
class J3DModelLoader_v21 : public J3DModelLoader {
public:
    ~J3DModelLoader_v21() {}
    void readMaterial_v21(J3DMaterialBlock_v21 const*, u32);
    void readMaterialTable_v21(J3DMaterialBlock_v21 const*, u32);
};

/**
 * @ingroup jsystem-j3d
 * 
 */
class J3DModelLoader_v26 : public J3DModelLoader {
public:
    ~J3DModelLoader_v26() {}
    void readMaterial(J3DMaterialBlock const*, u32);
    void readMaterialTable(J3DMaterialBlock const*, u32);
    u32 calcSizeMaterial(J3DMaterialBlock const*, u32);
    u32 calcSizeMaterialTable(J3DMaterialBlock const*, u32);
};

/**
 * @ingroup jsystem-j3d
 * 
 */
class J3DModelLoaderDataBase {
public:
    static J3DModelData* load(void const* i_data, u32 i_flags);
};

#endif /* J3DMODELLOADER_H */
