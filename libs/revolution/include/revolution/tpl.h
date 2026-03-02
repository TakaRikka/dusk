#ifndef _REVOLUTION_TPL_H_
#define _REVOLUTION_TPL_H_

#include <revolution/gx.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    u16 numEntries;
    u8 unpacked;
    u8 _4;
    GXTlutFmt format;
    char* data;
} TPLClutHeader, *TPLClutHeaderPtr;

typedef struct {
    u16 height;
    u16 width;
    u32 format;
    char* data;
    GXTexWrapMode wrapS;
    GXTexWrapMode wrapT;
    GXTexFilter minFilter;
    GXTexFilter magFilter;
    f32 LODBias;
    u8 edgeLODEnable;
    u8 minLOD;
    u8 maxLOD;
    u8 unpacked;
} TPLHeader, *TPLHeaderPtr;

typedef struct {
    TPLHeaderPtr textureHeader;
    TPLClutHeaderPtr CLUTHeader;
} TPLDescriptor, *TPLDescriptorPtr;

typedef struct {
    u32 versionNumber;
    u32 numDescriptors;
    TPLDescriptorPtr descriptorArray;
} TPLPalette, *TPLPalettePtr;

void TPLBind(TPLPalettePtr);
TPLDescriptorPtr TPLGet(TPLPalettePtr, u32);

#ifdef __cplusplus
}
#endif

#endif  // _REVOLUTION_TPL_H_
