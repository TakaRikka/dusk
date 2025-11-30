#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/J3DGraphBase/J3DShapeDraw.h"
#include "JSystem/JKernel/JKRHeap.h"
#include <string.h>
#include <stdint.h>
#include <dolphin/gx.h>

u32 J3DShapeDraw::countVertex(u32 stride) {
    u32 count = 0;
    uintptr_t dlStart = (uintptr_t)getDisplayList();

    for (u8* dl = (u8*)dlStart; ((uintptr_t)dl - dlStart) < getDisplayListSize();) {
        if (*dl != GX_TRIANGLEFAN && *dl != GX_TRIANGLESTRIP)
            break;
        u16 vtxNum = *((u16*)(dl + 1));
        count += vtxNum;
        dl += stride * vtxNum;
        dl += 3;
    }

    return count;
}

// NONMATCHING regalloc
void J3DShapeDraw::addTexMtxIndexInDL(u32 stride, u32 attrOffs, u32 valueBase) {
    u32 byteNum = countVertex(stride);
    u32 newSize = ALIGN_NEXT(mDisplayListSize + byteNum, 0x20);
    u8* newDLStart = new (0x20) u8[newSize];
    u8* oldDLStart = getDisplayList();
    u8* oldDL = oldDLStart;
    u8* newDL = newDLStart;

    for (; (oldDL - oldDLStart) < mDisplayListSize;) {
        // Copy command
        u8 h = *oldDL;
        *newDL++ = h;

        if (h != GX_TRIANGLEFAN && h != GX_TRIANGLESTRIP)
            break;

        // Copy count
        // regalloc (I suspect there's a way to shove this in a u16 temp without an mr)
        s32 vtxNum = *((u16*)(oldDL + 1));
        *((u16*)newDL) = vtxNum;
        newDL += 2;

        for (s32 i = 0; i < vtxNum; i++) {
            u8* oldDLVtx = &oldDL[stride * i + 3];
            u8 pnmtxidx = *oldDLVtx;
            memcpy(newDL, oldDLVtx, attrOffs);
            newDL += attrOffs;
            *newDL++ = valueBase + pnmtxidx;
            memcpy(newDL, oldDLVtx + attrOffs, stride - attrOffs);
            newDL += (stride - attrOffs);
        }

        oldDL += stride * vtxNum;
        oldDL += 3;
    }

    u32 realSize = ALIGN_NEXT((uintptr_t)newDL - (uintptr_t)newDLStart, 0x20);
    for (; (newDL - newDLStart) < newSize; newDL++)
        *newDL = 0;

    mDisplayListSize = realSize;
    mDisplayList = newDLStart;
    DCStoreRange(newDLStart, mDisplayListSize);
}

J3DShapeDraw::J3DShapeDraw(const u8* displayList, u32 displayListSize) {
    mDisplayList = (void*)displayList;
    mDisplayListSize = displayListSize;
}

void J3DShapeDraw::draw() const {
    GXCallDisplayList(mDisplayList, mDisplayListSize);
}

J3DShapeDraw::~J3DShapeDraw() {}
