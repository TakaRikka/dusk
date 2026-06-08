#include "JSystem/JSystem.h"  // IWYU pragma: keep

#include <cstring>
#include <gx.h>
#include <stdint.h>
#include "JSystem/J3DGraphBase/J3DShapeDraw.h"
#include "JSystem/JKernel/JKRHeap.h"

#if TARGET_PC
#include <algorithm>
#include <tracy/Tracy.hpp>
#include <vector>
#include "dusk/logging.h"

namespace {

u16 read_be16(const u8* data) {
    return (u16(data[0]) << 8) | data[1];
}

void append_be16(std::vector<u8>& out, u16 value) {
    out.push_back(value >> 8);
    out.push_back(value & 0xFF);
}

void append_bytes(std::vector<u8>& out, const u8* data, u32 size) {
    out.insert(out.end(), data, data + size);
}

bool is_matrix_idx_attr(GXAttr attr) {
    return attr >= GX_VA_PNMTXIDX && attr <= GX_VA_TEX7MTXIDX;
}

bool is_draw_opcode(u8 opcode) {
    return opcode == GX_QUADS || opcode == GX_TRIANGLES || opcode == GX_TRIANGLESTRIP ||
           opcode == GX_TRIANGLEFAN || opcode == GX_LINES || opcode == GX_LINESTRIP ||
           opcode == GX_POINTS;
}

bool is_mergeable_draw_opcode(u8 opcode) {
    return opcode == GX_QUADS || opcode == GX_TRIANGLES || opcode == GX_TRIANGLESTRIP ||
           opcode == GX_TRIANGLEFAN;
}

bool calc_vtx_stride(const GXVtxDescList* vtxDesc, u32& stride) {
    stride = 0;
    for (; vtxDesc->attr != GX_VA_NULL; vtxDesc++) {
        switch (vtxDesc->type) {
        case GX_NONE:
            break;
        case GX_DIRECT:
            if (!is_matrix_idx_attr(vtxDesc->attr)) {
                return false;
            }
            stride += 1;
            break;
        case GX_INDEX8:
            stride += 1;
            break;
        case GX_INDEX16:
            stride += 2;
            break;
        default:
            return false;
        }
    }
    return stride != 0;
}

bool get_command_size(const u8* dlStart, u32 dlSize, u32 offset, u32 stride, u32& cmdSize) {
    if (offset >= dlSize) {
        return false;
    }

    const u8 cmd = dlStart[offset];
    const u8 opcode = cmd & GX_OPCODE_MASK;
    switch (opcode) {
    case GX_NOP:
    case GX_CMD_INVL_VC:
        cmdSize = 1;
        return true;
    case (GX_LOAD_BP_REG & GX_OPCODE_MASK):
        cmdSize = 5;
        return offset + cmdSize <= dlSize;
    case GX_LOAD_CP_REG:
        cmdSize = 6;
        return offset + cmdSize <= dlSize;
    case GX_LOAD_XF_REG: {
        if (offset + 5 > dlSize) {
            return false;
        }
        const u16 count = read_be16(dlStart + offset + 1) + 1;
        cmdSize = 5 + count * 4;
        return offset + cmdSize <= dlSize;
    }
    case GX_LOAD_INDX_A:
    case GX_LOAD_INDX_B:
    case GX_LOAD_INDX_C:
    case GX_LOAD_INDX_D:
        cmdSize = 5;
        return offset + cmdSize <= dlSize;
    case GX_CMD_CALL_DL:
        cmdSize = 9;
        return offset + cmdSize <= dlSize;
    default:
        if (is_draw_opcode(opcode)) {
            if (offset + 3 > dlSize) {
                return false;
            }
            const u16 vtxCount = read_be16(dlStart + offset + 1);
            cmdSize = 3 + vtxCount * stride;
            return offset + cmdSize <= dlSize;
        }
        return false;
    }
}

struct MergeRun {
    u8 cmd = 0;
    u16 vtxCount = 0;
    std::vector<u8> vertices;
};

void flush_merge_run(std::vector<u8>& out, MergeRun& run) {
    if (run.vtxCount == 0) {
        return;
    }

    out.push_back(run.cmd);
    append_be16(out, run.vtxCount);
    append_bytes(out, run.vertices.data(), run.vertices.size());
    run.vertices.clear();
    run.vtxCount = 0;
}

void append_vertex(std::vector<u8>& out, const u8* vertices, u32 stride, u16 idx) {
    append_bytes(out, vertices + idx * stride, stride);
}

bool triangulate_draw(
    std::vector<u8>& out, u8 opcode, const u8* vertices, u32 stride, u16 vtxCount) {
    switch (opcode) {
    case GX_TRIANGLES:
        append_bytes(out, vertices, vtxCount * stride);
        return true;
    case GX_TRIANGLEFAN:
        if (vtxCount < 3) {
            return false;
        }
        for (u16 v = 2; v < vtxCount; v++) {
            append_vertex(out, vertices, stride, 0);
            append_vertex(out, vertices, stride, v - 1);
            append_vertex(out, vertices, stride, v);
        }
        return true;
    case GX_TRIANGLESTRIP:
        if (vtxCount < 3) {
            return false;
        }
        for (u16 v = 2; v < vtxCount; v++) {
            if ((v & 1) == 0) {
                append_vertex(out, vertices, stride, v - 2);
                append_vertex(out, vertices, stride, v - 1);
            } else {
                append_vertex(out, vertices, stride, v - 1);
                append_vertex(out, vertices, stride, v - 2);
            }
            append_vertex(out, vertices, stride, v);
        }
        return true;
    case GX_QUADS:
        if ((vtxCount & 3) != 0) {
            return false;
        }
        for (u16 v = 0; v < vtxCount; v += 4) {
            append_vertex(out, vertices, stride, v);
            append_vertex(out, vertices, stride, v + 1);
            append_vertex(out, vertices, stride, v + 2);
            append_vertex(out, vertices, stride, v + 2);
            append_vertex(out, vertices, stride, v + 3);
            append_vertex(out, vertices, stride, v);
        }
        return true;
    default:
        return false;
    }
}

void append_triangles_to_run(
    std::vector<u8>& out, MergeRun& run, u8 cmd, const std::vector<u8>& vertices, u32 stride) {
    u32 offset = 0;
    u32 remaining = vertices.size() / stride;
    while (remaining != 0) {
        if (run.vtxCount != 0 && run.cmd != cmd) {
            flush_merge_run(out, run);
        }

        if (run.vtxCount == 0) {
            run.cmd = cmd;
        }

        u32 available = 0xFFFF - run.vtxCount;
        if (available == 0) {
            flush_merge_run(out, run);
            continue;
        }

        u32 toCopy = std::min(remaining, available);
        append_bytes(run.vertices, vertices.data() + offset * stride, toCopy * stride);
        run.vtxCount += toCopy;
        offset += toCopy;
        remaining -= toCopy;

        if (run.vtxCount == 0xFFFF) {
            flush_merge_run(out, run);
        }
    }
}

bool optimize_display_list(const u8* dlStart, u32 dlSize, u32 stride, std::vector<u8>& out) {
    MergeRun run;
    out.reserve(dlSize);

    for (u32 offset = 0; offset < dlSize;) {
        u32 cmdSize = 0;
        if (!get_command_size(dlStart, dlSize, offset, stride, cmdSize)) {
            return false;
        }

        const u8 cmd = dlStart[offset];
        const u8 opcode = cmd & GX_OPCODE_MASK;
        if (opcode == GX_NOP) {
            offset += cmdSize;
            continue;
        }

        if (!is_draw_opcode(opcode)) {
            flush_merge_run(out, run);
            append_bytes(out, dlStart + offset, cmdSize);
            offset += cmdSize;
            continue;
        }

        if (!is_mergeable_draw_opcode(opcode)) {
            flush_merge_run(out, run);
            append_bytes(out, dlStart + offset, cmdSize);
            offset += cmdSize;
            continue;
        }

        const u16 vtxCount = read_be16(dlStart + offset + 1);
        const u8* vertices = dlStart + offset + 3;
        std::vector<u8> triangles;
        if (!triangulate_draw(triangles, opcode, vertices, stride, vtxCount)) {
            flush_merge_run(out, run);
            append_bytes(out, dlStart + offset, cmdSize);
            offset += cmdSize;
            continue;
        }

        append_triangles_to_run(out, run, (GX_TRIANGLES | (cmd & GX_VAT_MASK)), triangles, stride);
        offset += cmdSize;
    }

    flush_merge_run(out, run);
    return true;
}

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
    for (u32 offset = 0; offset < getDisplayListSize();) {
        u8 cmd = dlStart[offset];
        u8 opcode = cmd & GX_OPCODE_MASK;
        u32 cmdSize = 0;
        if (!get_command_size(dlStart, getDisplayListSize(), offset, stride, cmdSize)) {
            break;
        }
        if (!is_draw_opcode(opcode)) {
            offset += cmdSize;
            continue;
        }
        int vtxNum = be16(*reinterpret_cast<u16*>(dlStart + offset + 1));
        count += vtxNum;
        offset += 3 + stride * vtxNum;
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

void J3DShapeDraw::addTexMtxIndexInDL(u32 stride, u32 attrOffs, u32 valueBase) {
    u32 byteNum = countVertex(stride);
    u32 oldSize = mDisplayListSize;
    u32 newSize = ALIGN_NEXT(oldSize + byteNum, 0x20);
    u8* newDLStart = JKR_NEW_ARRAY_ARGS(u8, newSize, 0x20);
    u8* oldDLStart = (u8*)mDisplayList;
    u8* oldDL = oldDLStart;
    u8* newDL = newDLStart;

    for (; (oldDL - oldDLStart) < mDisplayListSize;) {
#if TARGET_PC
        u32 oldOffset = oldDL - oldDLStart;
        u32 cmdSize = 0;
        if (!get_command_size(oldDLStart, mDisplayListSize, oldOffset, stride, cmdSize)) {
            memcpy(newDL, oldDL, mDisplayListSize - oldOffset);
            newDL += mDisplayListSize - oldOffset;
            break;
        }
#endif
        // Copy command
        u8 cmd = *(u8*)oldDL;
        oldDL++;
        *newDL++ = cmd;

#if TARGET_PC
        u8 opcode = cmd & GX_OPCODE_MASK;
        if (!is_draw_opcode(opcode)) {
            memcpy(newDL, oldDL, cmdSize - 1);
            oldDL += cmdSize - 1;
            newDL += cmdSize - 1;
            continue;
        }
#else
        if (cmd != GX_TRIANGLEFAN && cmd != GX_TRIANGLESTRIP)
            break;
#endif

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
    u32 stride = 0;
    std::vector<u8> optimized;
    if (calc_vtx_stride(vtxDesc, stride) &&
        optimize_display_list(displayList, displayListSize, stride, optimized))
    {
        set_display_list_copy(mDisplayList, mDisplayListSize, optimized.data(), optimized.size());
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
