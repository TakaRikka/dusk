#include "GtxParser.hpp"

#include <cstring>

#include "dusk/endian.h"

namespace dusk::tphd {

namespace {

constexpr u32 kBlockTypeEOF      = 0x01;
constexpr u32 kBlockTypeSurface  = 0x0B;
constexpr u32 kBlockTypeImage    = 0x0C;
constexpr u32 kBlockTypeMipChain = 0x0D;

}

std::vector<GtxSurface> parseGtx(std::span<const u8> gtx) {
    std::vector<GtxSurface> out;

    if (gtx.size() < sizeof(Gfx2Header) ||
        std::memcmp(gtx.data(), "Gfx2", 4) != 0) {
        return out;
    }
    const auto* fileHdr = reinterpret_cast<const Gfx2Header*>(gtx.data());
    const u32 headerSize = fileHdr->headerSize;
    if (headerSize > gtx.size()) {
        return out;
    }

    GtxSurface* current = nullptr;
    size_t off = headerSize;

    while (off + sizeof(Gfx2BlockHeader) <= gtx.size()) {
        const auto* blk = reinterpret_cast<const Gfx2BlockHeader*>(gtx.data() + off);
        if (std::memcmp(blk->magic, "BLK{", 4) != 0) {
            break;
        }
        const u32 blockHdrSize = blk->headerSize;
        const u32 blockType    = blk->blockType;
        const u32 blockDataSz  = blk->blockDataSize;

        if (blockHdrSize < sizeof(Gfx2BlockHeader) ||
            off + blockHdrSize + blockDataSz > gtx.size()) {
            break;
        }
        const u8* body = gtx.data() + off + blockHdrSize;

        switch (blockType) {
        case kBlockTypeSurface: {
            if (blockDataSz < sizeof(Gx2SurfaceBody)) break;
            const auto* sb = reinterpret_cast<const Gx2SurfaceBody*>(body);
            GtxSurface s{};
            s.format   = sb->format;
            s.width    = sb->width;
            s.height   = sb->height;
            s.depth    = sb->depth;
            s.mipCount = sb->mipCount;
            s.aa       = sb->aa;
            s.use      = sb->use;
            s.imgSize  = sb->imgSize;
            s.mipSize  = sb->mipSize;
            s.tileMode = sb->tileMode;
            s.swizzle  = sb->swizzle;
            s.pitch    = sb->pitch;
            for (u32 i = 0; i < 13; ++i) {
                s.mipOffsets[i] = sb->mipOffsets[i];
            }
            out.push_back(s);
            current = &out.back();
            break;
        }
        case kBlockTypeImage:
            if (current) current->baseData = gtx.subspan(off + blockHdrSize, blockDataSz);
            break;
        case kBlockTypeMipChain:
            if (current) current->mipData = gtx.subspan(off + blockHdrSize, blockDataSz);
            break;
        case kBlockTypeEOF:
            return out;
        default:
            break;
        }
        off += blockHdrSize + blockDataSz;
    }
    return out;
}

}
