#include "helpers/batch.hpp"
#include "dusk/logging.h"

#include <aurora/dl.hpp>
#include <dolphin/gx/GXEnum.h>

namespace batch {

void decode_leaf_template(const u8* dl, u32 size, LeafTemplate& out) {
    out.vtxCount = 0;
    out.posRefCount = 0;
    bool posSeen[256] = {};

    static constexpr GXVtxDescList kLeafDesc[] = {
        {GX_VA_POS, GX_INDEX8},
        {GX_VA_NRM, GX_INDEX8},
        {GX_VA_CLR0, GX_INDEX8},
        {GX_VA_TEX0, GX_INDEX8},
        {GX_VA_NULL, GX_NONE},
    };

    aurora::gx::dl::Reader reader{dl, size, kLeafDesc};
    while (const auto cmd = reader.next()) {
        if (cmd->kind == aurora::gx::dl::Command::Kind::Passthrough) {
            if (cmd->data[0] != GX_NOP) {
                DuskLog.fatal("decode_leaf_template: unexpected opcode {:#x}", cmd->data[0]);
            }
            continue;
        }
        if (cmd->kind != aurora::gx::dl::Command::Kind::Draw) {
            DuskLog.fatal("decode_leaf_template: unexpected pre-optimized draw");
        }

        const auto& draw = cmd->draw;
        bool overflow = false;
        const bool expanded =
            aurora::gx::dl::expand_triangles(draw.prim, draw.vtxCount, [&](u16 i0, u16 i1, u16 i2) {
                if (overflow || out.vtxCount + 3 > LeafTemplate::kMaxVtx) {
                    overflow = true;
                    return;
                }
                for (const u16 elem : {i0, i1, i2}) {
                    LeafTemplate::Vtx& v = out.vtx[out.vtxCount++];
                    v.pos = draw.attr_idx(elem, GX_VA_POS);
                    v.nrm = draw.attr_idx(elem, GX_VA_NRM);
                    v.clr = draw.attr_idx(elem, GX_VA_CLR0);
                    v.tex = draw.attr_idx(elem, GX_VA_TEX0);
                    if (!posSeen[v.pos]) {
                        posSeen[v.pos] = true;
                        if (out.posRefCount >= LeafTemplate::kMaxPosRefs) {
                            overflow = true;
                            return;
                        }
                        out.posRefs[out.posRefCount++] = v.pos;
                    }
                }
            });
        if (!expanded) {
            DuskLog.fatal("decode_leaf_template: untriangulable draw (prim {:#x}, {} verts)",
                static_cast<u32>(draw.prim), draw.vtxCount);
        }
        if (overflow) {
            DuskLog.fatal("decode_leaf_template: template overflow ({} verts, {} positions)",
                out.vtxCount, out.posRefCount);
        }
    }
    if (reader.failed()) {
        DuskLog.fatal("decode_leaf_template: failed to walk display list");
    }
}

}  // namespace batch
