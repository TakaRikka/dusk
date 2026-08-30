#include "wsys.hpp"
#include "aurora/lib/logging.hpp"
#include "dusk/mod_loader.hpp"
#include "helpers/alignment.hpp"

namespace {

aurora::Module Log("dusk::mods::svc::audio_res::wsys::wav");

struct ChunkHeader {
    char magic[4];
    LE(u32) size;
};

struct RiffChunk {
    ChunkHeader header; // magic "RIFF"
    char id[4]; // "WAVE"
    // Data after chunk.
};

constexpr u16 WAVE_FORMAT_PCM = 0x0001;

struct FmtPcmChunkData {
    ChunkHeader header;

    u16 wFormatTag;
    u16 nChannels;
    u32 nSamplesPerSec;
    u32 nAvgBytesPerSec;
    u16 nBlockAlign;
    u16 wBitsPerSample;
};

bool check_four_cc(char const (&field)[4], char const (&expected)[5]) {
    return memcmp(field, expected, 4) == 0;
}

}

namespace dusk::mods::svc::audio_res::wsys {

ModResult load_wav(LoadedMod const& mod, RuntimeWaveReplacementSlot& slot, std::span<u8 const> fileData) {
    using namespace helpers::alignment;

    if (fileData.size() < sizeof(RiffChunk)) {
        return MOD_UNSUPPORTED;
    }

    auto const riffChunk = read_unaligned<RiffChunk>(&fileData[0]);
    if (!check_four_cc(riffChunk.header.magic, "RIFF")) {
        return MOD_UNSUPPORTED;
    }

    if (!check_four_cc(riffChunk.id, "WAVE")) {
        return MOD_UNSUPPORTED;
    }

    if (sizeof(ChunkHeader) + riffChunk.header.size > fileData.size()) {
        Log.error("[{}] wav file {}: invalid size!", mod.metadata.id, slot.bundle_path);
        return MOD_INVALID_ARGUMENT;
    }

    bool has_read_fmt = false;
    SampleDataPcm16 pcmData;

    auto waveData = fileData.subspan(sizeof(RiffChunk), riffChunk.header.size);
    while (waveData.size() > sizeof(ChunkHeader)) {
        auto const chunkHeader = read_unaligned<ChunkHeader>(&waveData[0]);

        if (check_four_cc(chunkHeader.magic, "fmt ")) {
            if (has_read_fmt) {
                Log.error("[{}] wav file {}: multiple fmt chunks!", mod.metadata.id, slot.bundle_path);
                return MOD_INVALID_ARGUMENT;
            }

            if (waveData.size() < sizeof(FmtPcmChunkData)) {
                Log.error("[{}] wav file {}: fmt chunk too small!", mod.metadata.id, slot.bundle_path);
                return MOD_INVALID_ARGUMENT;
            }

            auto const fmtChunk = read_unaligned<FmtPcmChunkData>(&waveData[0]);

            if (fmtChunk.nChannels != 1) {
                Log.error("[{}] wav file {}: only mono audio is supported", mod.metadata.id, slot.bundle_path);
                return MOD_INVALID_ARGUMENT;
            }

            if (fmtChunk.wFormatTag != WAVE_FORMAT_PCM) {
                Log.error("[{}] wav file {}: only 16-bit PCM is supported", mod.metadata.id, slot.bundle_path);
                return MOD_INVALID_ARGUMENT;
            }

            if (fmtChunk.wBitsPerSample != 16) {
                Log.error("[{}] wav file {}: only 16-bit PCM is supported", mod.metadata.id, slot.bundle_path);
                return MOD_INVALID_ARGUMENT;
            }

            has_read_fmt = true;
            slot.sample_rate = static_cast<f32>(fmtChunk.nSamplesPerSec);

        } else if (check_four_cc(chunkHeader.magic, "data")) {
            if (sizeof(chunkHeader) + chunkHeader.size > fileData.size()) {
                Log.error("[{}] wav file {}: unexpected EOF reading sample data", mod.metadata.id, slot.bundle_path);
                return MOD_INVALID_ARGUMENT;
            }

            auto const sampleData = waveData.subspan(sizeof(chunkHeader), chunkHeader.size);
            for (size_t i = 0; i + 1 < sampleData.size(); i += 2) {
                pcmData.data.push_back(read_unaligned<LE(s16)>(&sampleData[i]));
            }
        }

        auto skip = sizeof(chunkHeader) + chunkHeader.size;
        if (skip % 2 != 0) {
            skip += 1;
        }

        if (waveData.size() < skip) {
            Log.error("[{}] wav file {}: unexpected EOF", mod.metadata.id, slot.bundle_path);
            return MOD_INVALID_ARGUMENT;
        }

        waveData = waveData.subspan(skip);
    }

    if (!has_read_fmt) {
        Log.error("[{}] wav file {}: missing fmt chunk", mod.metadata.id, slot.bundle_path);
        return MOD_INVALID_ARGUMENT;
    }

    if (pcmData.data.empty()) {
        Log.error("[{}] wav file {}: missing data chunk", mod.metadata.id, slot.bundle_path);
        return MOD_INVALID_ARGUMENT;
    }

    pcmData.be_swap();

    slot.format = AUDIO_WAVE_FORMAT_PCM16;
    slot.sample_count = pcmData.size() / sizeof(u16);
    slot.data = std::make_unique<SampleDataPcm16>(std::move(pcmData));

    return MOD_OK;
}

}
