#include "JSystem/JSystem.h"  // IWYU pragma: keep

#include <cstring>
#include <gx.h>
#include <stdint.h>
#include "JSystem/J3DGraphBase/J3DShapeDraw.h"
#include "JSystem/JKernel/JKRHeap.h"

#if TARGET_PC
#include <aurora/dl.hpp>
#include <tracy/Tracy.hpp>

namespace {

void set_display_list_copy(void*& displayList, u32& displayListSize, const u8* data, u32 size) {
    const u32 alignedSize = ALIGN_NEXT(size, 0x20);
    u8* newDL = JKR_NEW_ARRAY_ARGS(u8, alignedSize, 0x20);
    if (size != 0) {
        std::memcpy(newDL, data, size);
    }
    for (u32 i = size; i < alignedSize; i++) {
        newDL[i] = 0;
    }

    displayList = newDL;
    displayListSize = alignedSize;
    DCStoreRange(newDL, displayListSize);
}

}  // namespace
#endif

u32 J3DShapeDraw::countVertex(u32 stride) {
    u32 count = 0;
    u8* dlStart = (u8*)getDisplayList();

#if TARGET_PC
    aurora::gx::dl::Reader reader{dlStart, getDisplayListSize(), static_cast<u8>(stride)};
    while (const auto cmd = reader.next()) {
        if (cmd->kind != aurora::gx::dl::Command::Kind::Passthrough) {
            count += cmd->draw.vtxCount;
        }
    }
#else
    for (u8* dl = dlStart; (dl - dlStart) < getDisplayListSize();) {
        u8 cmd = *(u8*)dl;
        dl++;
        if (cmd != GX_TRIANGLEFAN && cmd != GX_TRIANGLESTRIP)
            break;
        int vtxNum = be16(*((u16*)(dl)));
        dl += 2;
        count += vtxNum;
        dl = (u8*)dl + stride * vtxNum;
    }
#endif

    return count;
}

#if TARGET_PC
void J3DShapeDraw::addTexMtxIndexInDL(u32 stride, u32 attrOffs, u32 valueBase) {
    u32 byteNum = countVertex(stride);
    u32 oldSize = mDisplayListSize;
    u32 newSize = ALIGN_NEXT(oldSize + byteNum, 0x20);
    u8* newDLStart = JKR_NEW_ARRAY_ARGS(u8, newSize, 0x20);
    u8* oldDLStart = (u8*)mDisplayList;
    u8* newDL = newDLStart;

    aurora::gx::dl::Reader reader{oldDLStart, mDisplayListSize, static_cast<u8>(stride)};
    while (const auto cmd = reader.next()) {
        if (cmd->kind == aurora::gx::dl::Command::Kind::Passthrough) {
            std::memcpy(newDL, cmd->data, cmd->size);
            newDL += cmd->size;
            continue;
        }

        const auto& draw = cmd->draw;
        const u32 headerSize = draw.vertices - cmd->data;
        std::memcpy(newDL, cmd->data, headerSize);
        newDL += headerSize;

        for (u32 i = 0; i < draw.vtxCount; i++) {
            const u8* oldVtx = draw.vertices + stride * i;
            u8 pnmtxidx = oldVtx[0];
            std::memcpy(newDL, oldVtx, attrOffs);
            newDL += attrOffs;
            *newDL++ = valueBase + pnmtxidx;
            std::memcpy(newDL, oldVtx + attrOffs, stride - attrOffs);
            newDL += stride - attrOffs;
        }
    }
    if (reader.failed()) {
        // preserve the remainder untouched
        std::memcpy(newDL, oldDLStart + reader.pos(), mDisplayListSize - reader.pos());
        newDL += mDisplayListSize - reader.pos();
    }

    u32 realSize = ALIGN_NEXT((uintptr_t)newDL - (uintptr_t)newDLStart, 0x20);
    for (; (newDL - newDLStart) < newSize; newDL++)
        *newDL = 0;

    mDisplayListSize = realSize;
    mDisplayList = newDLStart;
    DCStoreRange(newDLStart, mDisplayListSize);
}
#else
void J3DShapeDraw::addTexMtxIndexInDL(u32 stride, u32 attrOffs, u32 valueBase) {
    u32 byteNum = countVertex(stride);
    u32 oldSize = mDisplayListSize;
    u32 newSize = ALIGN_NEXT(oldSize + byteNum, 0x20);
    u8* newDLStart = JKR_NEW_ARRAY_ARGS(u8, newSize, 0x20);
    u8* oldDLStart = (u8*)mDisplayList;
    u8* oldDL = oldDLStart;
    u8* newDL = newDLStart;

    for (; (oldDL - oldDLStart) < mDisplayListSize;) {
        // Copy command
        u8 cmd = *(u8*)oldDL;
        oldDL++;
        *newDL++ = cmd;

        if (cmd != GX_TRIANGLEFAN && cmd != GX_TRIANGLESTRIP)
            break;

        // Copy count
        int vtxNum = *(u16*)oldDL;
        oldDL += 2;
        *(u16*)newDL = vtxNum;
        newDL += 2;

        for (int i = 0; i < be16(vtxNum); i++) {
            u8* oldDLVtx = &oldDL[stride * i];
            u8 pnmtxidx = *oldDLVtx;
            memcpy(newDL, oldDLVtx, (int)attrOffs);
            newDL += attrOffs;
            *newDL++ = valueBase + pnmtxidx;
            memcpy(newDL, oldDLVtx + attrOffs, stride - attrOffs);
            newDL += (stride - attrOffs);
        }

        oldDL = (u8*)oldDL + stride * be16(vtxNum);
    }

    u32 realSize = ALIGN_NEXT((uintptr_t)newDL - (uintptr_t)newDLStart, 0x20);
    for (; (newDL - newDLStart) < newSize; newDL++)
        *newDL = 0;

    mDisplayListSize = realSize;
    mDisplayList = newDLStart;
    DCStoreRange(newDLStart, mDisplayListSize);
}
#endif

J3DShapeDraw::J3DShapeDraw(const u8* displayList, u32 displayListSize) {
#if TARGET_PC
    set_display_list_copy(mDisplayList, mDisplayListSize, displayList, displayListSize);
#else
    mDisplayList = (void*)displayList;
    mDisplayListSize = displayListSize;
#endif
}

#if TARGET_PC
J3DShapeDraw::J3DShapeDraw(
    const u8* displayList, u32 displayListSize, const GXVtxDescList* vtxDesc) {
    if (const auto optimized = aurora::gx::dl::optimize(displayList, displayListSize, vtxDesc)) {
        set_display_list_copy(mDisplayList, mDisplayListSize, optimized->data(), optimized->size());
    } else {
        set_display_list_copy(mDisplayList, mDisplayListSize, displayList, displayListSize);
    }
}
#endif

void J3DShapeDraw::draw() const {
    ZoneScoped;
    GXCallDisplayList(mDisplayList, mDisplayListSize);
}

J3DShapeDraw::~J3DShapeDraw() {}
