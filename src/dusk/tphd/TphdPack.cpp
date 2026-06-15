#include "TphdPack.hpp"

#include <zlib.h>

#include <cstdio>
#include <cstring>

#include "dusk/endian.h"
#include "dusk/logging.h"

static aurora::Module TphdLog("dusk::tphd");

namespace dusk::tphd {

namespace {

std::optional<std::vector<u8>> readFile(const std::filesystem::path& path) {
    std::FILE* f = std::fopen(path.string().c_str(), "rb");
    if (!f) return std::nullopt;
    std::fseek(f, 0, SEEK_END);
    long len = std::ftell(f);
    std::fseek(f, 0, SEEK_SET);
    if (len < 0) { std::fclose(f); return std::nullopt; }
    std::vector<u8> buf(static_cast<size_t>(len));
    size_t got = std::fread(buf.data(), 1, buf.size(), f);
    std::fclose(f);
    if (got != buf.size()) return std::nullopt;
    return buf;
}

}

std::optional<std::vector<u8>> decompressGzip(std::span<const u8> in) {
    if (in.size() < 18) return std::nullopt;
    if (in[0] != 0x1F || in[1] != 0x8B) return std::nullopt;

    u32 isize;
    std::memcpy(&isize, in.data() + in.size() - 4, sizeof(isize));
    std::vector<u8> out(isize);

    z_stream strm{};
    strm.next_in  = const_cast<Bytef*>(in.data());
    strm.avail_in = static_cast<uInt>(in.size());
    strm.next_out = out.data();
    strm.avail_out = static_cast<uInt>(out.size());

    if (inflateInit2(&strm, 15 + 16) != Z_OK) return std::nullopt;
    int rc = inflate(&strm, Z_FINISH);
    inflateEnd(&strm);
    if (rc != Z_STREAM_END) return std::nullopt;
    return out;
}

std::vector<TmpkEntry> parseTmpk(std::span<const u8> in) {
    std::vector<TmpkEntry> out;
    if (in.size() < sizeof(TmpkRawHeader)) return out;

    const auto* hdr = reinterpret_cast<const TmpkRawHeader*>(in.data());
    if (std::memcmp(hdr->magic, "TMPK", 4) != 0) return out;

    const u32 count = hdr->count;
    if (in.size() < sizeof(TmpkRawHeader) + count * sizeof(TmpkRawEntry)) return out;

    const auto* entries = reinterpret_cast<const TmpkRawEntry*>(
        in.data() + sizeof(TmpkRawHeader));

    out.reserve(count);
    for (u32 i = 0; i < count; ++i) {
        const u32 nameOff  = entries[i].nameOff;
        const u32 dataOff  = entries[i].dataOff;
        const u32 dataSize = entries[i].dataSize;
        const u32 flags    = entries[i].flags;

        if (nameOff >= in.size() || dataOff + dataSize > in.size()) continue;

        const char* nameStart = reinterpret_cast<const char*>(in.data() + nameOff);
        size_t maxLen = in.size() - nameOff;
        const void* nul = std::memchr(nameStart, 0, maxLen);
        size_t nameLen = nul ? static_cast<size_t>(static_cast<const char*>(nul) - nameStart)
                             : maxLen;

        out.push_back({
            std::string_view(nameStart, nameLen),
            in.subspan(dataOff, dataSize),
            flags,
        });
    }
    return out;
}

std::optional<TphdPack> TphdPack::loadFromMemory(std::span<const u8> gzipBytes) {
    auto inflated = decompressGzip(gzipBytes);
    if (!inflated) return std::nullopt;

    TphdPack p;
    p.m_buffer = std::move(*inflated);
    p.m_entries = parseTmpk(std::span<const u8>(p.m_buffer.data(), p.m_buffer.size()));
    if (p.m_entries.empty() && !p.m_buffer.empty()) {
        TphdLog.warn("TMPK parse yielded 0 entries (buffer size {})", p.m_buffer.size());
    }
    return p;
}

std::optional<TphdPack> TphdPack::loadFromFile(const std::filesystem::path& path) {
    auto raw = readFile(path);
    if (!raw) {
        TphdLog.error("Failed to read {}", path.string());
        return std::nullopt;
    }
    return loadFromMemory(*raw);
}

const TmpkEntry* TphdPack::find(std::string_view name) const {
    for (const auto& e : m_entries) {
        if (e.name == name) return &e;
    }
    return nullptr;
}

}
