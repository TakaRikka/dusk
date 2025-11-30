#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/J3DGraphLoader/J3DModelLoaderCalcSize.h"
#include "JSystem/J3DGraphLoader/J3DModelLoader.h"
#include "JSystem/J3DGraphLoader/J3DShapeFactory.h"
#include "JSystem/J3DGraphLoader/J3DMaterialFactory.h"
#include "JSystem/J3DGraphBase/J3DMaterial.h"
#include "JSystem/J3DGraphAnimator/J3DModelData.h"
#include "JSystem/JSupport/JSupport.h"
#include "dolphin/os.h"
#include "global.h"


u16 J3DModelLoader::countMaterialNum(const void* stream) {
    const J3DModelFileData* header = (const J3DModelFileData*)stream;
    const J3DModelBlock* block = header->mBlocks;
    for (int i = 0; i < header->mBlockNum; block = (const J3DModelBlock*)((u8*)block + block->mBlockSize), i++)
    {
        if (block->mBlockType == 'MAT3') {
            return ((const J3DMaterialBlock*)block)->mMaterialNum;
        }
    }
    return 0;
}

u32 J3DModelLoader::calcLoadSize(void const* stream, u32 flags_) {
    int flags = flags_;
    const J3DModelFileData* header = static_cast<const J3DModelFileData*>(stream);
    size_t size = 0;
    const J3DModelBlock* nextBlock = header->mBlocks;
    u32 i = 0;
    // TODO: What sizeof will get us a size of 0xE4?
    size += 0xE4;
    for (; i < header->mBlockNum; i++) {
        switch (nextBlock->mBlockType) {
        case 'INF1':
            size += calcSizeInformation((const J3DModelInfoBlock*)nextBlock, flags);
            break;
        case 'JNT1':
            size += calcSizeJoint((const J3DJointBlock*)nextBlock);
            break;
        case 'MAT2':
            break;
        case 'MAT3':
            size += calcSizeMaterial((const J3DMaterialBlock*)nextBlock, flags);
            break;
        case 'SHP1':
            size += calcSizeShape((const J3DShapeBlock*)nextBlock, flags);
            break;
        case 'TEX1':
            size += calcSizeTexture((const J3DTextureBlock*)nextBlock);
            break;
        case 'EVP1':
            size += calcSizeEnvelope((const J3DEnvelopeBlock*)nextBlock);
            break;
        case 'DRW1':
            size += calcSizeDraw((const J3DDrawBlock*)nextBlock);
            break;
        case 'VTX1':
            break;
        default:
            OSReport("Unknown data block\n");
            break;
        }
        nextBlock = (const J3DModelBlock*)((u8*)nextBlock + nextBlock->mBlockSize);
    }
    return size;
}

u32 J3DModelLoader::calcLoadMaterialTableSize(const void* stream) {
    size_t size;
    const J3DModelBlock* nextBlock;
    bool hasTextureTable;
    u32 i;
    const J3DModelFileData* header = reinterpret_cast<const J3DModelFileData*>(stream);
    nextBlock = header->mBlocks;
    hasTextureTable = false;
    i = 0;
    // TODO: What sizeof will get us a size of 0x20? Is this just the file header???
    size = 0x20;
    for (; i < header->mBlockNum; i++) {
        switch (nextBlock->mBlockType) {
        case 'MAT2':
            break;
        case 'MAT3':
            size +=
                calcSizeMaterialTable((const J3DMaterialBlock*)nextBlock,
                                      J3DMLF_21 | J3DMLF_Material_UseIndirect |
                                          J3DMLF_Material_PE_Full | J3DMLF_Material_Color_LightOn);
            break;
        case 'TEX1':
            size += calcSizeTextureTable((const J3DTextureBlock*)nextBlock);
            hasTextureTable = true;
            break;
        default:
            OSReport("Unknown data block\n");
            break;
        }
        nextBlock = (const J3DModelBlock*)((u8*)nextBlock + nextBlock->mBlockSize);
    }

    if (!hasTextureTable) {
        size += 12;
    }
    return size;
}

inline u32 getBdlFlag_MaterialType(u32 flags) {
    return flags & (J3DMLF_13 | J3DMLF_DoBdlMaterialCalc);
}

// NONMATCHING flags issue
u32 J3DModelLoader::calcLoadBinaryDisplayListSize(const void* stream, u32 flags) {
    u32 size = 0;
    size += sizeof(J3DModelData);
    const J3DModelFileData* header = (const J3DModelFileData*)stream;
    const J3DModelBlock* nextBlock = header->mBlocks;
    for (u32 i = 0; i < header->mBlockNum; i++) {
        switch (nextBlock->mBlockType) {
        case 'INF1':
            size += calcSizeInformation((const J3DModelInfoBlock*)nextBlock, (u32)flags);
            break;
        case 'JNT1':
            size += calcSizeJoint((const J3DJointBlock*)nextBlock);
            break;
        case 'SHP1':
			size += calcSizeShape((const J3DShapeBlock*)nextBlock, (u32)flags);
			break;
		case 'TEX1':
			size += calcSizeTexture((const J3DTextureBlock*)nextBlock);
			break;
		case 'MDL3':
			size += calcSizeMaterialDL((const J3DMaterialDLBlock*)nextBlock, flags);
			break;
		case 'MAT2':
			break;
		case 'MAT3': {
			u32 flags2 = (J3DMLF_21 | J3DMLF_Material_PE_Full | J3DMLF_Material_Color_LightOn);
            flags2 |= (u32)flags & (J3DMLF_Material_UseIndirect | J3DMLF_26);
			mpMaterialBlock = (const J3DMaterialBlock*)nextBlock;
            u32 flag_mtl_type = getBdlFlag_MaterialType(flags);
			if (flag_mtl_type == 0) {
				field_0x18 = 1;
				size += calcSizeMaterial((const J3DMaterialBlock*)nextBlock, flags2);
			} else if (flag_mtl_type == J3DMLF_DoBdlMaterialCalc) {
				field_0x18 = 1;
				size += calcSizePatchedMaterial((const J3DMaterialBlock*)nextBlock, flags2);
			}
			break;
        }
		case 'EVP1':
			size += calcSizeEnvelope((const J3DEnvelopeBlock*)nextBlock);
			break;
		case 'DRW1':
			size += calcSizeDraw((const J3DDrawBlock*)nextBlock);
			break;
		case 'VTX1':
			break;
		default:
            OSReport("Unknown data block\n");
			break;
        }
        nextBlock = (const J3DModelBlock*)((u8*)nextBlock + nextBlock->mBlockSize);
    }
    return size;
}

u32 J3DModelLoader::calcSizeInformation(const J3DModelInfoBlock* block, u32 flags) {
	int size = 0;
	switch ((flags | block->mFlags) & J3DMLF_MtxTypeMask) {
	case J3DMLF_None: // Basic
		size = 4;
		break;
	case J3DMLF_MtxSoftImageCalc:
		size = 4;
		break;
	case J3DMLF_MtxMayaCalc:
		size = 4;
		break;
	default:
		break;
	}
	mpModelHierarchy = JSUConvertOffsetToPtr<J3DModelHierarchy>(block, block->mpHierarchy);
	return size;
}

u32 J3DModelLoader::calcSizeJoint(const J3DJointBlock* block) {
	int size = 0;
	if (block->mpNameTable) {
		size = 0x10;
	}
	size += (block->mJointNum * sizeof(J3DJoint*));
	size += (block->mJointNum * sizeof(J3DJoint));
	return size;
}

u32 J3DModelLoader::calcSizeEnvelope(const J3DEnvelopeBlock* block) {
	mEnvelopeSize = block->mWEvlpMtxNum;
	return 0;
}

u32 J3DModelLoader::calcSizeDraw(const J3DDrawBlock* block) {
    return (block->mMtxNum - mEnvelopeSize) * 2; 
}

u32 J3DModelLoader_v26::calcSizeMaterial(const J3DMaterialBlock* block, u32 flags) {
	int padding = 0;
	J3DMaterialFactory factory(*block);
	u32 count       = block->mMaterialNum;
	int uniqueCount = factory.countUniqueMaterials();
	if (block->mpNameTable != NULL) {
		padding = 0x10;
	}
	size_t size = padding + (count * sizeof(J3DMaterial*));
	
    if ((flags & J3DMLF_UseUniqueMaterials) != 0) {
		// calc for allocated materials as well
		size += ALIGN_NEXT((u16)uniqueCount * sizeof(J3DMaterial), 0x20);
	}
    if ((flags & J3DMLF_UseUniqueMaterials) != 0) {
        for (u32 i = 0; i < uniqueCount; i++) {
            size += factory.calcSize(NULL, J3DMaterialFactory::MATERIAL_TYPE_NORMAL, i, flags);
        }
    }
	for (u32 i = 0; i < count; i++) {
		size += factory.calcSize(NULL, J3DMaterialFactory::MATERIAL_TYPE_NORMAL, i, flags);
	}
	return size;
}

u32 J3DModelLoader::calcSizeShape(const J3DShapeBlock* block, u32 flags) {
	int size = 0;
	J3DShapeFactory factory(*block);
	int count = block->mShapeNum;
	if (block->mpNameTable) {
		size = 0x10;
	}
	size += count * sizeof(J3DShape*);
	size += factory.calcSizeVcdVatCmdBuffer(count);
	for (J3DModelHierarchy* hierarchy = mpModelHierarchy; hierarchy->mType != 0; hierarchy++) {
		if (hierarchy->mType == 0x12) {
			size += factory.calcSize(hierarchy->mValue, flags);
		}
	}
	return size;
}

u32 J3DModelLoader::calcSizeTexture(const J3DTextureBlock* block) {
	// TODO: use sizeofs here.
	int padding = 0;
	if (block->mpNameTable) {
		padding = 0x10;
	}
	return padding + 0xC;
}

u32 J3DModelLoader_v26::calcSizeMaterialTable(const J3DMaterialBlock* block, u32 flags) {
    int size = 0;
    u32 count = block->mMaterialNum;
    J3DMaterialFactory factory(*block);
    if (block->mpNameTable) {
        size = 0x10;
    }
    size += (count * sizeof(J3DMaterial*));
    for (u16 i = 0; i < count; i++) {
        size += factory.calcSize(NULL, J3DMaterialFactory::MATERIAL_TYPE_NORMAL, i, flags);
    }
    return size;
}

u32 J3DModelLoader::calcSizeTextureTable(const J3DTextureBlock* block) {
	return calcSizeTexture(block);
}

u32 J3DModelLoader::calcSizePatchedMaterial(const J3DMaterialBlock* block, u32 flags) {
    int padding = 0;
    u32 count = block->mMaterialNum;
    J3DMaterialFactory factory(*block);
    if (block->mpNameTable) {
        padding = 0x10;
    }
    u32 size = padding + (count * sizeof(J3DMaterial*));
    for (u16 i = 0; i < count; i++) {
        size += factory.calcSize(NULL, J3DMaterialFactory::MATERIAL_TYPE_PATCHED, i, flags);
    }
    return size;
}

u32 J3DModelLoader::calcSizeMaterialDL(const J3DMaterialDLBlock* block, u32 flags_) {
    int flags = flags_;
	size_t size = 0;
	J3DMaterialFactory factory(*block);
	;
	if (!field_0x18) {
		u32 count = block->mMaterialNum;
		if (block->mpNameTable) {
			size = 0x10;
		}
		size += count * sizeof(J3DLockedMaterial*);
		for (u16 i = 0; i < count; i++) {
			size += factory.calcSize(NULL, J3DMaterialFactory::MATERIAL_TYPE_LOCKED, i, flags);
		}
	} else {
		u32 count = block->mMaterialNum;
		for (u16 i = 0; i < count; i++) {
			size += factory.calcSize((J3DMaterial*)this, J3DMaterialFactory::MATERIAL_TYPE_LOCKED, i, flags);
		}
	}
	return size;
}
