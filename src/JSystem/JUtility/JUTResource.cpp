#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JUtility/JUTResource.h"
#include "JSystem/JKernel/JKRArchive.h"
#include "JSystem/JSupport/JSUInputStream.h"
#include <string>

void* JUTResReference::getResource(JSUInputStream* stream, u32 resType, JKRArchive* archive) {
    stream->read(mType);
    stream->read(mNameLength);
    stream->read(&mName, mNameLength);

    if (mType == RESTYPE_Unk2 || mType == RESTYPE_Unk3 || mType == RESTYPE_Unk4) {
        mName[mNameLength] = 0;
    }

    return getResource(resType, archive);
}


void* JUTResReference::getResource(const void* data, u32 resType, JKRArchive* archive) {
    const u8* pData = (const u8*)data;
    mType = pData[0];
    mNameLength = pData[1];

    if (mNameLength != 0) {
        memcpy(&mName, &pData[2], mNameLength);
    }

    if (mType == RESTYPE_Unk2 || mType == RESTYPE_Unk3 || mType == RESTYPE_Unk4) {
        mName[mNameLength] = 0;
    }

    return getResource(resType, archive);
}

void* JUTResReference::getResource(u32 resType, JKRArchive* archive) {
    void* res = NULL;
    switch (mType) {
    case RESTYPE_Unk1:
        break;
    case RESTYPE_Unk2:
        res = JKRGetTypeResource(resType, mName, archive);
        break;
    case RESTYPE_Unk3:
        res = JKRGetNameResource(mName, archive);
        break;
    case RESTYPE_Unk4:
        res = JKRGetResource(mName);
        break;
    }

    return res;
}
