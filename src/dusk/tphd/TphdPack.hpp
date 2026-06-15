#ifndef DUSK_TPHD_TPHD_PACK_HPP
#define DUSK_TPHD_TPHD_PACK_HPP

#include <filesystem>
#include <optional>
#include <span>
#include <string_view>
#include <vector>

#include <dolphin/types.h>

#include "dusk/endian.h"

namespace dusk::tphd {

// On-disk TMPK layout. 
struct TmpkRawHeader {
    /* 0x00 */ char magic[4];     // "TMPK"
    /* 0x04 */ BE(u32) count;
    /* 0x08 */ u8 pad[8];
};
static_assert(sizeof(TmpkRawHeader) == 0x10);

struct TmpkRawEntry {
    /* 0x00 */ BE(u32) nameOff;
    /* 0x04 */ BE(u32) dataOff;
    /* 0x08 */ BE(u32) dataSize;
    /* 0x0C */ BE(u32) flags;
};
static_assert(sizeof(TmpkRawEntry) == 0x10);

// Parsed TMPK entry: a view into the inflated pack buffer.
struct TmpkEntry {
    std::string_view name;
    std::span<const u8> data;
    u32 flags;
};

class TphdPack {
public:
    static std::optional<TphdPack> loadFromFile(const std::filesystem::path& path);
    static std::optional<TphdPack> loadFromMemory(std::span<const u8> gzipBytes);

    const std::vector<TmpkEntry>& entries() const { return m_entries; }
    const TmpkEntry* find(std::string_view name) const;

private:
    TphdPack() = default;

    std::vector<u8> m_buffer;
    std::vector<TmpkEntry> m_entries;
};

std::optional<std::vector<u8>> decompressGzip(std::span<const u8> in);
std::vector<TmpkEntry> parseTmpk(std::span<const u8> in);

}

#endif
