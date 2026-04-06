#include "dusk/dvd_asset.hpp"
#include "dusk/logging.h"
#include "dusk/endian.h"
#include "aurora/dvd.h"
#include "DynamicLink.h"
#include "JSystem/JKernel/JKRArchive.h"
#include "JSystem/JKernel/JKRDvdRipper.h"

#include <cstring>

namespace dusk {

static const u8* s_dolData = nullptr; // pointer to dol data
static size_t    s_dolSize = 0;

struct DolHeader {
    BE(u32) textOffset[7];
    BE(u32) dataOffset[11];
    BE(u32) textAddr[7];
    BE(u32) dataAddr[11];
    BE(u32) textSize[7];
    BE(u32) dataSize[11];
};

struct DolSection {
    u32 fileOffset;
    u32 vaddr;
    u32 size;
};

static DolSection s_dolSections[18]; // 7 text + 11 data
static int        s_dolSectionCount = 0;

static bool EnsureDolParsed() {
    if (s_dolData) return true;

    s32 sz = 0;
    const u8* p = aurora_dvd_get_dol(sz);
    if (!p || sz < 256) {
        DuskLog.fatal("dvd_asset: aurora_dvd_get_dol failed (size={})", sz);
        return false;
    }
    
    s_dolData = p;
    s_dolSize = sz;

    const auto* hdr = (const DolHeader*)s_dolData;
    s_dolSectionCount = 0;

    for (int i = 0; i < 7;  i++) {
        u32 off = hdr->textOffset[i];
        u32 addr = hdr->textAddr[i];
        u32 sz = hdr->textSize[i];
        if (sz > 0 && off > 0) {
            s_dolSections[s_dolSectionCount++] = {off, addr, sz};
        }
    }

    for (int i = 0; i < 11; i++) {
        u32 off = hdr->dataOffset[i];
        u32 addr = hdr->dataAddr[i];
        u32 sz = hdr->dataSize[i];
        if (sz > 0 && off > 0) {
            s_dolSections[s_dolSectionCount++] = {off, addr, sz};
        }
    }

    return true;
}

static s32 DolVaToFileOffset(u32 va) {
    if (!EnsureDolParsed()) return -1;
    for (int i = 0; i < s_dolSectionCount; i++) {
        const auto& sec = s_dolSections[i];
        if (va >= sec.vaddr && va < sec.vaddr + sec.size) {
            return static_cast<s32>(sec.fileOffset + (va - sec.vaddr));
        }
    }
    DuskLog.fatal("dvd_asset: VA 0x{:08X} not found in any DOL section", va);
    return -1;
}

bool LoadDolAsset(void* dst, u32 virtualAddress, s32 size) {
    s32 fileOffset = DolVaToFileOffset(virtualAddress);
    if (fileOffset < 0) {
        return false;
    }

    if (size <= 0 || (size_t)(fileOffset + size) > s_dolSize) {
        DuskLog.fatal("dvd_asset: DOL read out of range (offset={:#x} size={:#x} dolSize={})", fileOffset, size, s_dolSize);
        return false;
    }

    std::memcpy(dst, s_dolData + fileOffset, size);
    return true;
}

bool LoadRelAsset(void* dst, const char* dvdPath, s32 offset, s32 size) {
    void* p = JKRDvdRipper::loadToMainRAM(dvdPath, (u8*)dst, EXPAND_SWITCH_UNKNOWN1, (u32)size, nullptr, JKRDvdRipper::ALLOC_DIRECTION_FORWARD, (u32)offset, nullptr, nullptr);
    if (!p) DuskLog.fatal("dvd_asset: failed to load {} (offset={:#x} size={:#x})", dvdPath, offset, size);
    return p != nullptr;
}

bool LoadArchivedRelAsset(void* dst, u32 memType, const char* relFileName, s32 offset, s32 size) {
    // On TARGET_PC, cDyl_InitCallback skips DynamicModuleControl::initialize() due to static linking
    // Mount RELS.arc on first use so sArchive is available
    static bool s_mountAttempted = false;
    if (!DynamicModuleControl::sArchive && !s_mountAttempted) {
        s_mountAttempted = true; DynamicModuleControl::initialize();
    }

    if (!DynamicModuleControl::sArchive) {
        DuskLog.fatal("dvd_asset: RELS archive not mounted"); return false;
    }

    const u8* rel = static_cast<const u8*>(DynamicModuleControl::sArchive->getResource(memType, relFileName));
    if (!rel) {
        DuskLog.fatal("dvd_asset: {} not found in RELS archive", relFileName); return false;
    }

    std::memcpy(dst, rel + offset, size);
    return true;
}

}  // namespace dusk
