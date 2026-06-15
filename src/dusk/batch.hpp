#pragma once

#include <dolphin/types.h>

namespace dusk::batch {

struct LeafTemplate {
    static constexpr u32 kMaxVtx = 192;
    static constexpr u32 kMaxPosRefs = 64;

    struct Vtx {
        u8 pos;
        u8 nrm;
        u8 clr;
        u8 tex;
    };
    Vtx vtx[kMaxVtx];
    u16 vtxCount = 0;
    u8 posRefs[kMaxPosRefs];
    u8 posRefCount = 0;
};

void decode_leaf_template(const u8* dl, u32 size, LeafTemplate& out);

}  // namespace dusk
