#ifndef _REVOLUTION_GX_H_
#define _REVOLUTION_GX_H_

#include <revolution/gx/GXStruct.h>
#include <revolution/gx/GXTransform.h>
#include <revolution/gx/GXTev.h>
#include <revolution/gx/GXPixel.h>
#include <revolution/gx/GXManage.h>
#include <revolution/gx/GXFifo.h>
#include <revolution/gx/GXCull.h>
#include <revolution/gx/GXGeometry.h>
#include <revolution/gx/GXVert.h>
#include <revolution/gx/GXTexture.h>
#include <revolution/gx/GXTev.h>
#include <revolution/gx/GXLighting.h>
#include <revolution/gx/GXDispList.h>
#include <revolution/gx/GXCommandList.h>
#include <revolution/gx/GXBump.h>
#include <revolution/gx/GXFrameBuffer.h>
#include <revolution/gx/GXGet.h>
#include <revolution/gx/GXDraw.h>
#include <revolution/gx/GXPerf.h>
#include <revolution/gx/GXCpu2Efb.h>
#include <revolution/gx/GXVerify.h>

// unsorted GX externs

#ifdef __cplusplus
extern "C" {
#endif

// GXMisc
void (*GXSetDrawSyncCallback(void (*cb)(u16)))(u16);
void GXSetDrawSync(u16 token);

#ifdef __cplusplus
}
#endif

#endif
