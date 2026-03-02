#include <dolphin/dvd.h>
#include <dolphin/gx.h>
#include <dolphin/os.h>
#include <charPipeline/fileCache.h>
#include <charPipeline/texPalette.h>

static void LoadTexPalette(TEXPalettePtr* pal, char* name);
static void UnpackTexPalette(TEXPalettePtr pal);
static void TexFreeFunc(TEXPalettePtr* pal);

void TEXGetPalette(TEXPalettePtr* pal, char* name) {
    void* p = TexFreeFunc;

    if (DOCacheInitialized) {
        *pal = (TEXPalettePtr)DSGetCacheObj(&DODisplayCache, name);
    }

    if (!*pal) {
        LoadTexPalette(pal, name);
        if (DOCacheInitialized) {
            DSAddCacheNode(&DODisplayCache, name, (Ptr)*pal, p);
            DSGetCacheObj(&DODisplayCache, name);
        }
    }
}

static void LoadTexPalette(TEXPalettePtr* pal, char* name) {
    DVDFileInfo dfi;

    DVDOpen(name, &dfi);
    *pal = OSAlloc(OSRoundUp32B(dfi.length));
    DVDReadPrio(&dfi, *pal, OSRoundUp32B(dfi.length), 0, 2);
    DVDClose(&dfi);
    UnpackTexPalette(*pal);
}

#define PALETTE_VERSION 0x20AF30

static void UnpackTexPalette(TEXPalettePtr pal) {
    u16 i;

    if (pal->versionNumber != PALETTE_VERSION) {
        OSPanic(__FILE__, 86, "invalid version number for texture palette");
    }

    pal->descriptorArray = (TEXDescriptorPtr)((Ptr)pal->descriptorArray + (u32)pal);
    for (i = 0; i < pal->numDescriptors; i++) {
        if (pal->descriptorArray[i].textureHeader) {
            pal->descriptorArray[i].textureHeader = (TEXHeaderPtr)((Ptr)pal + (u32)pal->descriptorArray[i].textureHeader);
            if (!pal->descriptorArray[i].textureHeader->unpacked) {
                pal->descriptorArray[i].textureHeader->data = (Ptr)pal + (u32)pal->descriptorArray[i].textureHeader->data;
                pal->descriptorArray[i].textureHeader->unpacked = TRUE;
            }
        }

        if (pal->descriptorArray[i].CLUTHeader) {
            pal->descriptorArray[i].CLUTHeader = (CLUTHeaderPtr)((u8 *)pal + (u32)pal->descriptorArray[i].CLUTHeader);
            if (!pal->descriptorArray[i].CLUTHeader->unpacked) {
                pal->descriptorArray[i].CLUTHeader->data = (Ptr)pal + (u32)pal->descriptorArray[i].CLUTHeader->data;
                pal->descriptorArray[i].CLUTHeader->unpacked = TRUE;
            }
        }
    }
}

TEXDescriptorPtr TEXGet(TEXPalettePtr pal, u32 id) {
    ASSERTMSGLINE(147, id < pal->numDescriptors, "GetTexture():  Texture Not Found ");
    return &pal->descriptorArray[id];
}

static void TexFreeFunc(TEXPalettePtr* pal) {
    OSFree(*pal);
    *pal = NULL;
}

void TEXReleasePalette(TEXPalettePtr* pal) {
    if (DOCacheInitialized) {
        DSReleaseCacheObj(&DODisplayCache, (Ptr)*pal);
    } else {
        OSFree(*pal);
        *pal = NULL;
    }
}

void TEXGetGXTexObjFromPalette(TEXPalettePtr pal, GXTexObj* to, u32 id) {
    TEXDescriptorPtr tdp;
    GXBool mipMap;

    tdp = TEXGet(pal, id);
    if (tdp->textureHeader->minLOD == tdp->textureHeader->maxLOD) {
        mipMap = GX_FALSE;
    } else {
        mipMap = GX_TRUE;
    }
    GXInitTexObj(to, tdp->textureHeader->data, tdp->textureHeader->width, tdp->textureHeader->height, tdp->textureHeader->format, tdp->textureHeader->wrapS, tdp->textureHeader->wrapT, mipMap);
    GXInitTexObjLOD(to, tdp->textureHeader->minFilter, tdp->textureHeader->magFilter, tdp->textureHeader->minLOD, tdp->textureHeader->maxLOD, tdp->textureHeader->LODBias, GX_DISABLE, tdp->textureHeader->edgeLODEnable, GX_ANISO_1);
}

void TEXGetGXTexObjFromPaletteCI(TEXPalettePtr pal, GXTexObj* to, GXTlutObj* tlo, GXTlut tluts, u32 id) {
    GXBool mipMap;
    TEXDescriptorPtr tdp;

    tdp = TEXGet(pal, id);
    if (tdp->textureHeader->minLOD == tdp->textureHeader->maxLOD) {
        mipMap = GX_FALSE;
    } else {
        mipMap = GX_TRUE;
    }
    GXInitTlutObj(tlo, tdp->CLUTHeader->data, tdp->CLUTHeader->format, tdp->CLUTHeader->numEntries);
    GXInitTexObjCI(to, tdp->textureHeader->data, tdp->textureHeader->width, tdp->textureHeader->height, tdp->textureHeader->format, tdp->textureHeader->wrapS, tdp->textureHeader->wrapT, mipMap, tluts);
    GXInitTexObjLOD(to, tdp->textureHeader->minFilter, tdp->textureHeader->magFilter, tdp->textureHeader->minLOD, tdp->textureHeader->maxLOD, tdp->textureHeader->LODBias, GX_DISABLE, tdp->textureHeader->edgeLODEnable, GX_ANISO_1);
}
