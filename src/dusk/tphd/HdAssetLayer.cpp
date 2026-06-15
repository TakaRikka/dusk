#include "HdAssetLayer.hpp"

#include <algorithm>
#include <cstdint>
#include <cstring>
#include <list>
#include <memory>
#include <mutex>
#include <optional>
#include <span>
#include <string>
#include <system_error>
#include <unordered_map>
#include <vector>

#include <aurora/dvd.h>
#include <aurora/texture.hpp>
#include <dolphin/dvd.h>
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_iostream.h>

#include "JSystem/J3DGraphLoader/J3DModelLoader.h"
#include "JSystem/JKernel/JKRArchive.h"
#include "JSystem/JKernel/JKRDecomp.h"
#include "JSystem/JUtility/JUTTexture.h"
#include "dusk/endian.h"
#include "dusk/io.hpp"
#include "dusk/logging.h"
#include "AddrLib.hpp"
#include "GtxParser.hpp"
#include "LosTable.hpp"
#include "TphdPack.hpp"
#include "tracy/Tracy.hpp"

static aurora::Module HdLog("dusk::tphd::hd");

namespace dusk::tphd {

namespace {

std::filesystem::path g_contentPath;
std::mutex g_cacheMutex;

// Heap-allocated, never freed — these must outlive g_dComIfG_gameInfo's
// static destructor which holds JKRArchives referencing these bytes.
std::list<std::vector<u8>>& g_mountBuffers() {
    static auto* p = new std::list<std::vector<u8>>{};
    return *p;
}

std::list<std::vector<u8>>& g_textureBuffers() {
    static auto* p = new std::list<std::vector<u8>>{};
    return *p;
}

std::unordered_map<std::string, std::shared_ptr<const TphdPack>>& g_packCache() {
    static auto* p = new std::unordered_map<std::string, std::shared_ptr<const TphdPack>>{};
    return *p;
}

aurora::texture::ReplacementGroup& g_textureReplacementGroup() {
    static auto* p = new aurora::texture::ReplacementGroup{};
    return *p;
}

struct HdArcRange {
    const void* begin = nullptr;
    size_t size = 0;
    std::string label;
};

std::vector<HdArcRange>& g_arcRanges() {
    static auto* p = new std::vector<HdArcRange>{};
    return *p;
}

struct HdOverlayEntry {
    std::string dvdPath;
    std::filesystem::path arcPath;
    std::filesystem::path packPath;
    size_t size = 0;
};

std::list<HdOverlayEntry>& g_overlayEntries() {
    static auto* p = new std::list<HdOverlayEntry>{};
    return *p;
}

std::unordered_map<s32, HdOverlayEntry*>& g_entryNumToOverlay() {
    static auto* p = new std::unordered_map<s32, HdOverlayEntry*>{};
    return *p;
}

bool g_overlayCallbacksRegistered = false;

void clear_hd_texture_registrations_locked() {
    aurora::texture::unregister_replacements(g_textureReplacementGroup());
    g_textureReplacementGroup().registrations.clear();
    g_textureBuffers().clear();
}

void register_hd_arc_range_locked(const void* begin, size_t size, std::string_view label) {
    if (begin == nullptr || size == 0) return;
    g_arcRanges().push_back({
        .begin = begin,
        .size = size,
        .label = std::string(label),
    });
}

bool endsWithSuffix(std::string_view s, std::string_view suffix) {
    return s.size() >= suffix.size() &&
           s.compare(s.size() - suffix.size(), suffix.size(), suffix) == 0;
}

bool path_exists(const std::filesystem::path& path) {
    std::error_code ec;
    return std::filesystem::is_regular_file(path, ec);
}

struct SDL_IODeleter {
    void operator()(SDL_IOStream* io) const {
        if (io != nullptr) {
            SDL_CloseIO(io);
        }
    }
};

using IOStream = std::unique_ptr<SDL_IOStream, SDL_IODeleter>;

IOStream open_stream(const std::filesystem::path& path) {
    const auto pathString = io::fs_path_to_string(path);
    return IOStream{SDL_IOFromFile(pathString.c_str(), "rb")};
}

std::optional<size_t> get_file_size(const std::filesystem::path& path) {
    auto stream = open_stream(path);
    if (stream == nullptr) {
        return std::nullopt;
    }

    const Sint64 size = SDL_GetIOSize(stream.get());
    if (size < 0) {
        return std::nullopt;
    }
    return size;
}

// On-disk Yaz0 file header.
struct Yaz0Header {
    /* 0x00 */ char magic[4];     // "Yaz0"
    /* 0x04 */ BE(u32) decompressedSize;
    /* 0x08 */ u8 pad[8];
};
static_assert(sizeof(Yaz0Header) == 0x10);

// If `bytes` is a Yaz0 stream, return the inflated payload; otherwise nullopt.
std::optional<std::vector<u8>> tryDecodeYaz0(std::span<const u8> bytes) {
    if (bytes.size() < sizeof(Yaz0Header) ||
        std::memcmp(bytes.data(), "Yaz0", 4) != 0) {
        return std::nullopt;
    }
    ZoneScoped;
    const auto* hdr = reinterpret_cast<const Yaz0Header*>(bytes.data());
    const u32 expandedSize = hdr->decompressedSize;
    std::vector<u8> decoded(expandedSize);
    JKRDecomp::decodeSZS(const_cast<u8*>(bytes.data()), decoded.data(),
                         expandedSize, 0);
    return decoded;
}

std::optional<std::vector<u8>> read_file(const std::filesystem::path& path) {
    auto stream = open_stream(path);
    if (stream == nullptr) {
        return std::nullopt;
    }
    const Sint64 len = SDL_GetIOSize(stream.get());
    if (len < 0) {
        return std::nullopt;
    }
    std::vector<u8> buf(len);
    size_t total = 0;
    while (total < buf.size()) {
        const size_t got = SDL_ReadIO(stream.get(), buf.data() + total, buf.size() - total);
        if (got == 0) {
            break;
        }
        total += got;
    }
    if (total != buf.size() || SDL_GetIOStatus(stream.get()) == SDL_IO_STATUS_ERROR) {
        return std::nullopt;
    }
    return buf;
}

std::optional<TphdPack> load_pack_from_file(const std::filesystem::path& path) {
    auto raw = read_file(path);
    if (!raw) {
        return std::nullopt;
    }
    return TphdPack::loadFromMemory(*raw);
}

std::shared_ptr<const TphdPack> load_pack_cached(const std::filesystem::path& path) {
    const auto key = io::fs_path_to_string(path);
    {
        std::lock_guard lk{g_cacheMutex};
        const auto it = g_packCache().find(key);
        if (it != g_packCache().end()) {
            return it->second;
        }
    }

    auto loaded = load_pack_from_file(path);
    if (!loaded) {
        return {};
    }

    auto pack = std::make_shared<TphdPack>(std::move(*loaded));
    {
        std::lock_guard lk{g_cacheMutex};
        auto [it, inserted] = g_packCache().emplace(key, pack);
        if (!inserted) {
            return it->second;
        }
    }
    return pack;
}

// Extract the path portion under "res/" from JSystem's absolute path.
// Example: "/arcName/res/Stage/D_SB10/R00_00.arc" -> "res/Stage/D_SB10/R00_00.arc"
std::string_view extractResPath(std::string_view gcPath) {
    auto p = gcPath.find("res/");
    if (p == std::string_view::npos) return {};
    return gcPath.substr(p);
}


// Case-insensitive ASCII suffix match — RARC archives lowercase filenames
// at build time, but our HD pack.gz preserves the original Wii-U authoring
// camelCase. Example: RARC has "coverbg.bti", pack has "coverBG.bti.gtx".
bool endsWithSuffixCI(std::string_view s, std::string_view suffix) {
    if (s.size() < suffix.size()) return false;
    auto toLower = [](unsigned char c) -> unsigned char {
        return (c >= 'A' && c <= 'Z') ? c + ('a' - 'A') : c;
    };
    const char* a = s.data() + (s.size() - suffix.size());
    for (size_t i = 0; i < suffix.size(); ++i) {
        if (toLower(a[i]) != toLower(suffix[i])) return false;
    }
    return true;
}

// Match an arc-relative path (e.g. "bmdr/model.bmd") against the Gfx2 entries
// in the HD pack, which look like "tex/.../<arc-rel>.gtx".
const TmpkEntry* findGtxBySuffix(const TphdPack& pack, std::string_view arcRelPath) {
    const std::string tail = "/" + std::string(arcRelPath) + ".gtx";
    for (const auto& e : pack.entries()) {
        if (e.data.size() < 4 || std::memcmp(e.data.data(), "Gfx2", 4) != 0) continue;
        if (endsWithSuffixCI(e.name, tail)) return &e;
    }
    return nullptr;
}

// Post-deswizzle CPU expansions to RGBA8. Used for formats whose HD layout
// can't be directly sampled with a GPU view swizzle (IA4 nibble unpack,
// RGB565 16-bit), and as a fallback if R8_PC/RG8_PC view swizzle isn't
// available. GC sampling semantics: I8 -> (I,I,I,I); IA4/IA8 -> (I,I,I,A).

std::vector<u8> expandR5G6B5toRgba8(std::span<const u8> in, u32 width, u32 height) {
    std::vector<u8> out(static_cast<size_t>(width) * height * 4);
    const size_t pixelCount = static_cast<size_t>(width) * height;
    for (size_t i = 0; i < pixelCount && (i * 2 + 1) < in.size(); ++i) {
        // GX2 stores RGB565 pixel data in GPU-native LE
        u16 px;
        std::memcpy(&px, &in[i * 2], sizeof(px));
        u8 b5 = static_cast<u8>((px >> 11) & 0x1F);
        u8 g6 = static_cast<u8>((px >> 5) & 0x3F);
        u8 r5 = static_cast<u8>(px & 0x1F);
        out[i * 4 + 0] = static_cast<u8>((r5 << 3) | (r5 >> 2));
        out[i * 4 + 1] = static_cast<u8>((g6 << 2) | (g6 >> 4));
        out[i * 4 + 2] = static_cast<u8>((b5 << 3) | (b5 >> 2));
        out[i * 4 + 3] = 0xFF;
    }
    return out;
}

// IA4: high nibble = A, low nibble = I (matches aurora's GC IA4 decoder).
std::vector<u8> expandIA4toRgba8(std::span<const u8> in, u32 width, u32 height) {
    std::vector<u8> out(static_cast<size_t>(width) * height * 4);
    const size_t pixelCount = static_cast<size_t>(width) * height;
    for (size_t i = 0; i < pixelCount && i < in.size(); ++i) {
        u8 b = in[i];
        u8 A = static_cast<u8>((b & 0xF0) | (b >> 4));
        u8 I = static_cast<u8>(((b & 0x0F) << 4) | (b & 0x0F));
        out[i * 4 + 0] = I; out[i * 4 + 1] = I; out[i * 4 + 2] = I; out[i * 4 + 3] = A;
    }
    return out;
}

enum class Expansion {
    None,
    R5G6B5_to_RGBA8,
    IA4_to_RGBA8,
};

struct Gx2FormatMapping {
    u32 gx2Format;       // GX2 surface format
    u8 newGxFormat;      // Aurora PC-target format
    u32 bpp;             // Deswizzle bits-per-pixel (per pixel, or per 4x4 block for BCn)
    bool isBcn;
    Expansion expansion; // Optional post-deswizzle CPU expansion
};

// I8/IA8 pass through as R8_PC/RG8_PC (aurora applies .rrrr/.rrrg view
// swizzle on the GPU side — half / quarter the VRAM of CPU-expanded RGBA8).
// IA4 + RGB565 need CPU expansion (nibble / 16-bit unpack). CMPR stays
// BC1_PC (compressed on the GPU).
constexpr Gx2FormatMapping kFormatMap[] = {
    // gx2 fmt          PC target            bpp  isBcn  expansion
    { 0x01 /* I8     */, 0x41 /* R8_PC    */,  8,  false, Expansion::None },
    { 0x02 /* IA4    */, 0x46 /* RGBA8_PC */,  8,  false, Expansion::IA4_to_RGBA8 },
    { 0x07 /* IA8    */, 0x43 /* RG8_PC   */, 16,  false, Expansion::None },
    { 0x08 /* RGB565 */, 0x46 /* RGBA8_PC */, 16,  false, Expansion::R5G6B5_to_RGBA8 },
    { 0x1A /* RGBA8  */, 0x46 /* RGBA8_PC */, 32,  false, Expansion::None },
    { 0x31 /* CMPR   */, 0x4E /* BC1_PC   */, 64,  true,  Expansion::None },
};

const Gx2FormatMapping* findFormatMapping(u32 gx2Format) {
    for (const auto& m : kFormatMap) {
        if (m.gx2Format == gx2Format) return &m;
    }
    return nullptr;
}

std::vector<u8> applyExpansion(Expansion exp, std::vector<u8> linear, u32 w, u32 h) {
    switch (exp) {
    case Expansion::R5G6B5_to_RGBA8: return expandR5G6B5toRgba8(linear, w, h);
    case Expansion::IA4_to_RGBA8:    return expandIA4toRgba8(linear, w, h);
    case Expansion::None: break;
    }
    return linear;
}

// Per-mip tile-mode + pitch. Demote rule mirrored from decaf-emu's
// R600AddrLib::ComputeSurfaceMipLevelTileMode (MIT, AMD-derived) — see
// AddrLib.cpp header for the full copyright notice.
//
// R700 macro-tile size: 32 × 16 elements (BCN element = 4×4 block).
// Mips below that are demoted to Tiled1DThin1 (microtile-only, 8-element
// align).
struct MipLevelDesc {
    u32 width;
    u32 height;
    u32 pitch;
    addrlib::TileMode tileMode;
};

MipLevelDesc mipLevelDesc(const GtxSurface& s, u32 level, bool isBcn, u32 bpp) {
    MipLevelDesc d{};
    d.width    = std::max(1u, s.width  >> level);
    d.height   = std::max(1u, s.height >> level);

    if (level == 0) {
        d.pitch    = s.pitch;
        d.tileMode = static_cast<addrlib::TileMode>(s.tileMode);
        return d;
    }

    const addrlib::SurfaceInfoIn si{
        .width    = s.width,
        .height   = s.height,
        .bpp      = bpp,
        .mipLevel = level,
        .tileMode = static_cast<addrlib::TileMode>(s.tileMode),
        .isBcn    = isBcn,
    };
    addrlib::SurfaceInfoOut so{};
    addrlib::computeSurfaceInfo(si, so);
    d.pitch    = so.pitch;     // block units for BCN, pixel units for plain.
    d.tileMode = so.tileMode;
    return d;
}

// Slice the bytes for a single mip level. Wii-U quirk: mipOffsets[0] is
// often image_size, not a mipData offset. Level 1
// always starts at 0 in mipData; level >= 2 uses mipOffsets[level - 1].
std::span<const u8> mipLevelData(const GtxSurface& s, u32 level) {
    if (level == 0) return s.baseData;
    if (level >= s.mipCount) return {};

    u32 start = 0;
    if (level >= 2 && level - 1 < s.mipOffsets.size()) {
        start = s.mipOffsets[level - 1];
    }
    if (start >= s.mipData.size()) return {};

    u32 end = static_cast<u32>(s.mipData.size());
    if (level + 1 < s.mipCount && level < s.mipOffsets.size()) {
        const u32 next = s.mipOffsets[level];
        if (next > start && next <= s.mipData.size()) end = next;
    }
    return s.mipData.subspan(start, end - start);
}

struct DeswizzleResult {
    std::vector<u8> bytes;
    u32 mipCount;
};

DeswizzleResult deswizzleAllMips(const Gx2FormatMapping& m, const GtxSurface& s) {
    ZoneScoped;
    DeswizzleResult out{};
    const u32 maxLevels = std::min(s.mipCount, 13u);
    for (u32 level = 0; level < maxLevels; ++level) {
        const std::span<const u8> slice = mipLevelData(s, level);
        if (slice.empty()) break;

        const MipLevelDesc lvl = mipLevelDesc(s, level, m.isBcn, m.bpp);
        const addrlib::SurfaceDesc desc{
            .width    = lvl.width,
            .height   = lvl.height,
            .pitch    = lvl.pitch,
            .bpp      = m.bpp,
            .tileMode = lvl.tileMode,
            .swizzle  = s.swizzle,
            .isBcn    = m.isBcn,
            .isDepth  = false,
        };

        auto linear = applyExpansion(m.expansion,
                                     addrlib::deswizzle(desc, slice),
                                     lvl.width, lvl.height);
        out.bytes.insert(out.bytes.end(), linear.begin(), linear.end());
        out.mipCount = level + 1;
    }
    return out;
}

void registerHdSurface(const Gx2FormatMapping& m, const GtxSurface& s,
                       const void* pixelPtr, std::string_view gtxName,
                       u32 surfaceIdx, bool replaceExistingPointer = false) {
    ZoneScoped;
    auto decoded = deswizzleAllMips(m, s);

    HdLog.info("HD reg: ptr={} fmt=0x{:02X} {}x{} mips={}/{} bytes={} gtx={}[{}]",
               pixelPtr, m.newGxFormat, s.width, s.height,
               decoded.mipCount, s.mipCount, decoded.bytes.size(),
               gtxName, surfaceIdx);

    if (decoded.bytes.empty() || pixelPtr == nullptr) {
        return;
    }

    std::lock_guard lk{g_cacheMutex};
    g_textureBuffers().emplace_back(std::move(decoded.bytes));
    const auto& bytes = g_textureBuffers().back();
    const aurora::texture::RawTextureReplacement replacement{
        .bytes = {bytes.data(), bytes.size()},
        .width = s.width,
        .height = s.height,
        .mipCount = std::max(decoded.mipCount, 1u),
        .gxFormat = m.newGxFormat,
        .label = gtxName,
    };

    aurora::texture::ReplacementKey replacementKey{
        aurora::texture::TexturePointerKey{.data = pixelPtr},
    };
    if (replaceExistingPointer) {
        aurora::texture::unregister_replacements(replacementKey);
    }
    auto registration = aurora::texture::register_replacement(std::move(replacementKey), replacement);
    if (registration.id != 0) {
        g_textureReplacementGroup().registrations.push_back(std::move(registration));
    }
}

bool register_hd_bti_replacement_for_buffer(const TphdPack& pack, std::string_view resourceName,
    void* buffer, size_t resourceSize, bool replaceExistingPointer) {
    if (buffer == nullptr || resourceSize < 0x20 || !endsWithSuffixCI(resourceName, ".bti")) {
        return false;
    }

    const TmpkEntry* gtx = findGtxBySuffix(pack, resourceName);
    if (!gtx) {
        return false;
    }

    auto surfaces = parseGtx(gtx->data);
    if (surfaces.empty()) {
        return false;
    }

    const auto& s = surfaces[0];
    if (s.baseData.empty()) {
        return false;
    }

    const Gx2FormatMapping* m = findFormatMapping(s.format);
    if (!m) {
        return false;
    }

    auto* timg = reinterpret_cast<ResTIMG*>(buffer);
    timg->imageOffset = 0x20;
    const u8 hdMips = static_cast<u8>(std::clamp<u32>(s.mipCount, 1u, 11u));
    timg->mipmapCount = hdMips;
    timg->maxLOD = static_cast<s8>((hdMips - 1) * 8);
    registerHdSurface(*m, s, static_cast<u8*>(buffer) + 0x20, gtx->name, 0, replaceExistingPointer);
    return true;
}

// Absolute offset of slot `slotIdx`'s BTI header within a BMD's TEX1 block.
// Returns 0 on failure (the TEX1 table never sits at offset 0, so 0 is a
// safe sentinel).
u32 bmdSlotBtiOffset(std::span<const u8> bmd, u32 slotIdx) {
    constexpr size_t kBlocksOffset = offsetof(J3DModelFileData, mBlocks);  // = 0x20
    if (bmd.size() < kBlocksOffset ||
        std::memcmp(bmd.data(), "J3D2", 4) != 0) return 0;

    const auto* fileData = reinterpret_cast<const J3DModelFileData*>(bmd.data());
    const u32 numSections = fileData->mBlockNum;
    size_t pos = kBlocksOffset;

    for (u32 i = 0; i < numSections && pos + sizeof(J3DModelBlock) <= bmd.size(); ++i) {
        const auto* blk = reinterpret_cast<const J3DModelBlock*>(bmd.data() + pos);
        const u32 blockSize = blk->mBlockSize;
        if (blk->mBlockType == 'TEX1') {
            const auto* tex1 = reinterpret_cast<const J3DTextureBlock*>(bmd.data() + pos);
            const u16 numTex = tex1->mTextureNum;
            if (slotIdx >= numTex) return 0;
            const size_t btiAbs = pos + static_cast<u32>(tex1->mpTextureRes) + slotIdx * 0x20;
            if (btiAbs + 0x20 > bmd.size()) return 0;
            return static_cast<u32>(btiAbs);
        }
        if (blockSize == 0) break;
        pos += blockSize;
    }
    return 0;
}

size_t register_hd_bmd_textures_for_buffer(const TphdPack& pack, std::string_view resourceName,
    void* buffer, size_t resourceSize, bool replaceExistingPointer) {
    if (buffer == nullptr || resourceSize < 0x20) return 0;
    if (!endsWithSuffixCI(resourceName, ".bmd") &&
        !endsWithSuffixCI(resourceName, ".bdl")) return 0;

    const TmpkEntry* gtx = findGtxBySuffix(pack, resourceName);
    if (gtx == nullptr) return 0;

    std::span<u8> bmdBytes(static_cast<u8*>(buffer), resourceSize);
    auto surfaces = parseGtx(gtx->data);
    size_t reg = 0;
    for (u32 i = 0; i < surfaces.size(); ++i) {
        const auto& s = surfaces[i];
        if (s.baseData.empty()) continue;
        const Gx2FormatMapping* m = findFormatMapping(s.format);
        if (!m) continue;

        // HD-stub BMDs collapse every BTI's imageOffset to the same
        // pixel address. Rewrite each to be slot-unique so our pointer
        // map doesn't overwrite.
        const u32 btiAbs = bmdSlotBtiOffset(bmdBytes, i);
        if (btiAbs == 0) continue;
        auto* timg = reinterpret_cast<ResTIMG*>(bmdBytes.data() + btiAbs);
        if (timg->imageOffset == 0) {
            HdLog.debug("Skip cross-arc placeholder slot {} in {}: "
                        "imageOffset==0",
                        i, gtx->name);
            continue;
        }

        const u32 newImgOff = 0x20 + i * 0x20;
        timg->imageOffset = static_cast<s32>(newImgOff);
        const u8 hdMips = static_cast<u8>(std::clamp<u32>(s.mipCount, 1u, 11u));
        timg->mipmapCount = hdMips;
        timg->maxLOD = static_cast<s8>((hdMips - 1) * 8);
        timg->maxAnisotropy = GX_ANISO_4;
        registerHdSurface(*m, s, bmdBytes.data() + btiAbs + newImgOff, gtx->name, i,
                          replaceExistingPointer);
        ++reg;
    }
    return reg;
}

// Lightweight RARC walker that returns per-file offsets without copying
// arc bytes — we need absolute pointers into the cached HD arc bytes
// (stable address) to match what the game later passes to GXInitTexObj.
struct ArcFileInfo {
    std::string path;  // e.g. "bmdr/model.bmd"
    u32 dataOffset;    // absolute offset from arc base
    u32 dataSize;
};

std::vector<ArcFileInfo> parseRarcFiles(std::span<const u8> arc) {
    std::vector<ArcFileInfo> out;
    if (arc.size() < 0x40 || std::memcmp(arc.data(), "RARC", 4) != 0) return out;

    constexpr size_t kMetaBase = sizeof(SArcHeader);  // = 0x20
    if (arc.size() < kMetaBase + sizeof(SArcDataInfo)) return out;

    const auto* hdr = reinterpret_cast<const SArcHeader*>(arc.data());
    const auto* dataInfo = reinterpret_cast<const SArcDataInfo*>(arc.data() + kMetaBase);

    const u32 nodeCount   = dataInfo->num_nodes;
    const size_t nodeTbl  = dataInfo->node_offset + kMetaBase;
    const size_t fileTbl  = dataInfo->file_entry_offset + kMetaBase;
    const size_t strTbl   = dataInfo->string_table_offset + kMetaBase;
    const size_t dataBase = kMetaBase + hdr->file_data_offset;

    auto readStringAt = [&](u32 offset) -> std::string {
        const u8* start = arc.data() + strTbl + offset;
        const u8* bufferEnd = arc.data() + arc.size();
        if (start >= bufferEnd) return {};

        const void* nul = std::memchr(start, 0,
                                      static_cast<size_t>(bufferEnd - start));
        const u8* terminator = nul ? static_cast<const u8*>(nul) : bufferEnd;
        return std::string(reinterpret_cast<const char*>(start),
                           static_cast<size_t>(terminator - start));
    };

    const auto* nodes = reinterpret_cast<const JKRArchive::SDIDirEntry*>(
        arc.data() + nodeTbl);
    const auto* files = reinterpret_cast<const JKRArchive::SDIFileEntry*>(
        arc.data() + fileTbl);

    for (u32 ni = 0; ni < nodeCount; ++ni) {
        const auto& node = nodes[ni];
        const std::string dirName = readStringAt(node.name_offset);
        const u16 fc       = node.num_entries;
        const u32 firstIdx = node.first_file_index;
        const bool isRoot = (ni == 0);

        for (u32 fi = 0; fi < fc; ++fi) {
            const auto& entry = files[firstIdx + fi];
            const u32 typeFlagsAndName = entry.type_flags_and_name_offset;
            const u8  typeFlags = static_cast<u8>(typeFlagsAndName >> 24);
            // Bit 0x01 = file, 0x02 = directory. We only want files.
            if ((typeFlags & 0x03) != 0x01) continue;

            std::string fname = readStringAt(typeFlagsAndName & 0xFFFFFF);
            if (fname == "." || fname == "..") continue;

            out.push_back({
                (!isRoot && !dirName.empty())
                    ? dirName + "/" + fname
                    : std::move(fname),
                static_cast<u32>(dataBase + entry.data_offset),
                entry.data_size,
            });
        }
    }
    return out;
}

// Walk the HD arc, pair BMDs with their pack.gz GTX entries, deswizzle each
// HD surface, and register the decoded bytes with aurora under the absolute
// pointer that GXInitTexObj will later receive.
//
// arcBytes must point at the mounted archive bytes the game will later use;
// aurora's pointer lookups depend on those addresses staying valid.
void register_hd_textures_for_arc(std::span<u8> arcBytes, const std::vector<ArcFileInfo>& files,
    const TphdPack& pack, std::string_view arcLabel) {
    ZoneScoped;
    ZoneText(arcLabel.data(), arcLabel.size());
    size_t bmdReg = 0;
    size_t btiReg = 0;

    // Phase A: per-slot textures inside BMD/BDL models.
    for (const auto& f : files) {
        bmdReg += register_hd_bmd_textures_for_buffer(pack, f.path, arcBytes.data() + f.dataOffset, f.dataSize, false);
    }

    // Phase B: standalone .bti files. Each BTI is its own arc entry; the
    // game loads it via JUTTexture (or similar) which calls GXInitTexObj
    // with `(u8*)resTIMG + imageOffset`. Register that exact pointer.
    for (const auto& f : files) {
        if (register_hd_bti_replacement_for_buffer(pack, f.path, arcBytes.data() + f.dataOffset, f.dataSize, false)) {
            ++btiReg;
        }
    }

    HdLog.info("registerHdTextures[{}]: {} BMD-slot, {} standalone-BTI replacements",
               arcLabel, bmdReg, btiReg);
}

// HD arcs whose Wii-U layouts don't match the GC UI pipeline.
constexpr std::string_view kHdSkipList[] = {
    "res/Object/fileSel.arc",
};

bool is_layout_arc_path(std::string_view resPath) {
    return resPath.starts_with("res/Layout/") ||
           resPath.starts_with("res/LayoutRevo/");
}

std::filesystem::path hd_pack_path_for_arc(std::string_view resPath) {
    std::filesystem::path packPath = g_contentPath / std::string(resPath);
    packPath.replace_extension(".pack.gz");

    if (!resPath.starts_with("res/Layout/")) {
        return packPath;
    }

    const std::filesystem::path arcPath{std::string(resPath)};
    std::string revoStem = arcPath.stem().string();
    if (!revoStem.empty() && revoStem.back() != 'R') {
        revoStem += 'R';
    }

    const auto revoPackPath = g_contentPath / "res" / "LayoutRevo" /
                              (revoStem + ".pack.gz");
    if (path_exists(revoPackPath)) {
        return revoPackPath;
    }

    return packPath;
}

bool should_skip_hd_arc_mount(std::string_view resPath) {
    // Layout HD archives do not match the GC UI pipeline, but their pack.gz
    // textures can still be registered against the vanilla archive.
    if (is_layout_arc_path(resPath)) {
        return true;
    }
    for (auto skip : kHdSkipList) {
        if (resPath == skip) return true;
    }
    return false;
}

bool should_register_hd_pack_for_vanilla_arc(std::string_view resPath) {
    return resPath.starts_with("res/Layout/");
}

void* overlay_open(void* userData) {
    auto* entry = static_cast<HdOverlayEntry*>(userData);
    if (entry == nullptr) return nullptr;

    SDL_IOStream* stream = open_stream(entry->arcPath).release();
    if (stream == nullptr) {
        HdLog.warn("HD overlay open failed: {} ({})",
                   entry->arcPath.string(), SDL_GetError());
        return nullptr;
    }

    return stream;
}

void overlay_close(void* handle) {
    auto* stream = static_cast<SDL_IOStream*>(handle);
    if (stream == nullptr) return;
    SDL_CloseIO(stream);
}

int64_t overlay_read(void* handle, uint8_t* buf, size_t len) {
    auto* stream = static_cast<SDL_IOStream*>(handle);
    if (stream == nullptr || buf == nullptr) {
        return -1;
    }
    if (len == 0) {
        return 0;
    }
    const size_t got = SDL_ReadIO(stream, buf, len);
    if (got == 0 && SDL_GetIOStatus(stream) == SDL_IO_STATUS_ERROR) {
        return -1;
    }
    return static_cast<int64_t>(got);
}

int64_t overlay_seek(void* handle, int64_t offset, int32_t whence) {
    auto* stream = static_cast<SDL_IOStream*>(handle);
    if (stream == nullptr) {
        return -1;
    }
    const Sint64 pos = SDL_SeekIO(stream, offset, static_cast<SDL_IOWhence>(whence));
    return pos < 0 ? -1 : static_cast<int64_t>(pos);
}

void ensure_overlay_callbacks_registered() {
    if (g_overlayCallbacksRegistered) {
        return;
    }
    static constexpr AuroraOverlayCallbacks callbacks{
        .open = overlay_open,
        .close = overlay_close,
        .read = overlay_read,
        .seek = overlay_seek,
    };
    aurora_dvd_overlay_callbacks(&callbacks);
    g_overlayCallbacksRegistered = true;
}

void rebuild_hd_overlay_locked() {
    if (g_contentPath.empty()) {
        if (g_overlayCallbacksRegistered) {
            aurora_dvd_overlay_files(nullptr, 0, nullptr);
        }
        return;
    }

    std::error_code ec;
    const auto resRoot = g_contentPath;
    if (!std::filesystem::is_directory(resRoot, ec)) {
        HdLog.warn("HD content path has no res directory: {}", g_contentPath.string());
        return;
    }

    const s32 baseEntryCount = aurora_dvd_base_entry_count();
    if (baseEntryCount <= 0) {
        HdLog.warn("DVD overlay skipped because no base DVD FST is loaded yet");
        return;
    }

    ensure_overlay_callbacks_registered();

    std::vector<AuroraOverlayFile> overlayFiles;
    std::vector<HdOverlayEntry*> overlayEntries;
    for (std::filesystem::recursive_directory_iterator it(resRoot, ec), end;
         !ec && it != end; it.increment(ec)) {
        const bool regularFile = it->is_regular_file(ec);
        if (ec) {
            ec.clear();
            continue;
        }
        if (!regularFile) continue;

        const auto& arcPath = it->path();
        
        const auto rel = arcPath.lexically_relative(g_contentPath);
        const std::string resPath = rel.generic_string();
        if (resPath.empty()) continue;

        if (should_register_hd_pack_for_vanilla_arc(resPath)) {
            auto packPath = hd_pack_path_for_arc(resPath);
            if (path_exists(packPath)) {
                auto& entry = g_overlayEntries().emplace_back();
                entry.dvdPath = "/" + resPath;
                entry.arcPath = arcPath;
                entry.packPath = std::move(packPath);

                const s32 entryNum = DVDConvertPathToEntrynum(entry.dvdPath.c_str());
                if (entryNum >= 0) {
                    g_entryNumToOverlay()[entryNum] = &entry;
                    HdLog.info("HD texture pack registered for vanilla arc: {} -> {}",
                               entry.dvdPath, entry.packPath.string());
                } else {
                    HdLog.warn("HD texture pack skipped because DVD path was not found: {}",
                               entry.dvdPath);
                }
            }
        }

        if (should_skip_hd_arc_mount(resPath)) continue;

        const auto fileSize = get_file_size(arcPath);
        if (!fileSize.has_value()) {
            HdLog.warn("HD overlay file size failed: {} ({})",
                       arcPath.string(), SDL_GetError());
            continue;
        }

        auto& entry = g_overlayEntries().emplace_back();
        entry.dvdPath = "/" + resPath;
        entry.arcPath = arcPath;
        entry.packPath = hd_pack_path_for_arc(resPath);
        entry.size = *fileSize;

        overlayFiles.push_back({
            .fileName = entry.dvdPath.c_str(),
            .userData = &entry,
            .size = entry.size,
        });
        overlayEntries.push_back(&entry);
    }

    std::vector overlayEntryNums(overlayFiles.size(), -1);
    aurora_dvd_overlay_files(overlayFiles.data(), overlayFiles.size(), overlayEntryNums.data());

    for (size_t i = 0; i < overlayEntryNums.size() && i < overlayEntries.size(); ++i) {
        auto* entry = overlayEntries[i];
        if (overlayEntryNums[i] < 0) {
            HdLog.warn("HD overlay entry was not accepted by DVD FST: {}", entry->dvdPath);
            continue;
        }
        g_entryNumToOverlay()[overlayEntryNums[i]] = entry;
    }

    HdLog.info("HD DVD overlay registered {} files (arcs, .jpc and Audiores) from {}",
               overlayFiles.size(), g_contentPath.string());
}

}

void set_hd_content_path(std::filesystem::path contentPath) {
    g_contentPath = std::move(contentPath);
    std::lock_guard lk{g_cacheMutex};
    clear_hd_texture_registrations_locked();
    g_mountBuffers().clear();
    g_packCache().clear();
    g_overlayEntries().clear();
    g_entryNumToOverlay().clear();
    g_arcRanges().clear();
    rebuild_hd_overlay_locked();
    load_los_table(g_contentPath);
    HdLog.info("HD content path set to: {}",
               g_contentPath.empty() ? "(disabled)" : g_contentPath.string());
}

std::optional<std::vector<u8>*> try_load_hd_archive(std::string_view gcPath) {
    if (g_contentPath.empty()) return std::nullopt;

    std::string_view resPath = extractResPath(gcPath);
    if (resPath.empty()) return std::nullopt;

    if (should_skip_hd_arc_mount(resPath)) return std::nullopt;

    std::filesystem::path hdArcPath = g_contentPath / std::string(resPath);
    ZoneScoped;
#ifdef TRACY_ENABLE
    {
        auto fn = hdArcPath.filename().string();
        ZoneText(fn.c_str(), fn.size());
    }
#endif

    auto hdBytesOpt = read_file(hdArcPath);
    if (!hdBytesOpt) {
        return std::nullopt;
    }

    if (auto inflated = tryDecodeYaz0(*hdBytesOpt)) {
        HdLog.info("HD arc Yaz0-decompressed: {} -> {} bytes",
                   hdArcPath.filename().string(), inflated->size());
        hdBytesOpt = std::move(inflated);
    }

    auto hdFiles = parseRarcFiles(std::span<const u8>(
        hdBytesOpt->data(), hdBytesOpt->size()));

    // Sidecar pack.gz holds the HD textures.
    auto hdPackPath = hd_pack_path_for_arc(resPath);
    auto hdPack = load_pack_cached(hdPackPath);

    // std::list keeps element addresses stable for aurora's pointer map.
    std::vector<u8>* mountBytes;
    std::string filename = hdArcPath.filename().string();
    {
        std::lock_guard lk{g_cacheMutex};
        g_mountBuffers().emplace_back(std::move(*hdBytesOpt));
        mountBytes = &g_mountBuffers().back();
        register_hd_arc_range_locked(mountBytes->data(), mountBytes->size(), filename);
    }

    HdLog.info("HD arc mount buffer allocated: {} at {} ({} bytes, pack.gz={})",
               filename, static_cast<const void*>(mountBytes->data()),
               mountBytes->size(), hdPack ? "yes" : "no");

    if (hdPack != nullptr) {
        register_hd_textures_for_arc(*mountBytes, hdFiles, *hdPack, filename);
    }

    return mountBytes;
}

void register_mounted_hd_archive(s32 entryNum, void* arcBytes, size_t arcSize) {
    if (entryNum < 0 || arcBytes == nullptr || arcSize == 0) return;

    std::filesystem::path packPath;
    std::string label;
    {
        std::lock_guard lk{g_cacheMutex};
        auto it = g_entryNumToOverlay().find(entryNum);
        if (it == g_entryNumToOverlay().end()) return;
        packPath = it->second->packPath;
        label = it->second->arcPath.filename().string();
    }

    auto arcSpan = std::span{static_cast<uint8_t*>(arcBytes), arcSize};
    {
        std::lock_guard lk{g_cacheMutex};
        register_hd_arc_range_locked(arcSpan.data(), arcSpan.size(), label);
    }

    auto hdPack = load_pack_cached(packPath);
    if (hdPack == nullptr) {
        return;
    }

    auto hdFiles = parseRarcFiles(std::span<const u8>(arcSpan.data(), arcSpan.size()));
    register_hd_textures_for_arc(arcSpan, hdFiles, *hdPack, label);
}

void register_copied_hd_resource(s32 entryNum, std::string_view resourceName, void* buffer,
                            size_t resourceSize) {
    if (entryNum < 0 || buffer == nullptr || resourceSize < 0x20) return;

    const bool isBti = endsWithSuffixCI(resourceName, ".bti");
    const bool isBmd = endsWithSuffixCI(resourceName, ".bmd") ||
                       endsWithSuffixCI(resourceName, ".bdl");
    if (!isBti && !isBmd) return;

    std::filesystem::path packPath;
    {
        std::lock_guard lk{g_cacheMutex};
        auto it = g_entryNumToOverlay().find(entryNum);
        if (it == g_entryNumToOverlay().end()) {
            return;
        }
        packPath = it->second->packPath;
    }

    auto hdPack = load_pack_cached(packPath);
    if (hdPack == nullptr) {
        return;
    }

    if (isBti) {
        register_hd_bti_replacement_for_buffer(*hdPack, resourceName, buffer, resourceSize, true);
    } else {
        register_hd_bmd_textures_for_buffer(*hdPack, resourceName, buffer, resourceSize, true);
    }
}

std::optional<size_t> find_registered_hd_archive_remaining(const void* ptr) {
    if (ptr == nullptr) return std::nullopt;

    const auto p = reinterpret_cast<uintptr_t>(ptr);
    std::lock_guard lk{g_cacheMutex};
    for (const auto& range : g_arcRanges()) {
        const auto begin = reinterpret_cast<uintptr_t>(range.begin);
        const auto end = begin + range.size;
        if (p >= begin && p < end) {
            return end - p;
        }
    }
    return std::nullopt;
}

}
