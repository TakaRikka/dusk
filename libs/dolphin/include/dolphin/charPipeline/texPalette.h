#ifndef _CHARPIPELINE_TEXPALETTE_H_
#define _CHARPIPELINE_TEXPALETTE_H_

#include <dolphin/gx/GXStruct.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    u16 numEntries;
    u8 unpacked;
    u8 pad8;
    GXTlutFmt format;
    Ptr data;
} CLUTHeader, *CLUTHeaderPtr;

typedef struct {
    u16 height;
    u16 width;
    u32 format;
    Ptr data;
    GXTexWrapMode wrapS; 
    GXTexWrapMode wrapT;
    GXTexFilter minFilter;
    GXTexFilter magFilter;
    f32 LODBias;
    u8 edgeLODEnable;
    u8 minLOD;
    u8 maxLOD;
    u8 unpacked;
} TEXHeader, *TEXHeaderPtr;

typedef struct {
    TEXHeaderPtr textureHeader;
    CLUTHeaderPtr CLUTHeader;
} TEXDescriptor, *TEXDescriptorPtr;

typedef struct {
    u32 versionNumber;
    u32 numDescriptors;
    TEXDescriptorPtr descriptorArray;
} TEXPalette, *TEXPalettePtr;

void TEXGetPalette(TEXPalettePtr* pal, char* name);
TEXDescriptorPtr TEXGet(TEXPalettePtr pal, u32 id);
void TEXReleasePalette(TEXPalettePtr* pal);
void TEXGetGXTexObjFromPalette(TEXPalettePtr pal, GXTexObj* to, u32 id);
void TEXGetGXTexObjFromPaletteCI(TEXPalettePtr pal, GXTexObj* to, GXTlutObj* tlo, GXTlut tluts, u32 id);

#ifdef __cplusplus
}
#endif

#endif
