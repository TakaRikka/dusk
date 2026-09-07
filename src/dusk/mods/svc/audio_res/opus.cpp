#if DUSK_OPUS

#include "wsys.hpp"
#include "aurora/lib/logging.hpp"
#include "dusk/mod_loader.hpp"
#include "helpers/cast.hpp"
#include "opusfile.h"

namespace {

aurora::Module Log("dusk::mods::svc::audio_res::wsys::opus");

struct OpusHandle {
    OggOpusFile* file;

    ~OpusHandle() {
        op_free(file);
    }

    operator OggOpusFile*() const {
        return file;
    }
};

}

namespace dusk::mods::svc::audio_res::wsys {
ModResult load_opus(LoadedMod const& mod, RuntimeWaveReplacementSlot& slot, std::span<u8 const> fileData) {
    OpusHead head;
    const int result = op_test(&head, fileData.data(), fileData.size());
    if (result != 0) {
        return MOD_UNSUPPORTED;
    }

    if (head.channel_count != 1) {
        Log.error("[{}] replace_wave: file '{}' must be mono but actually has {} channels", mod.metadata.id, slot.bundle_path, head.channel_count);
        return MOD_INVALID_ARGUMENT;
    }

    if (head.stream_count != 1) {
        Log.error("[{}] replace_wave: file '{}' has multiple Opus streams!", mod.metadata.id, slot.bundle_path);
        return MOD_INVALID_ARGUMENT;
    }

    int error;
    auto const handle_raw = op_open_memory(fileData.data(), fileData.size(), &error);
    if (error != 0) {
        Log.error("[{}] replace_wave: file '{}': unable to open Opus file: {}", mod.metadata.id, slot.bundle_path, error);
        return MOD_ERROR;
    }

    const OpusHandle handle(handle_raw);
    auto const length = op_pcm_total(handle, -1);
    if (length < 0) {
        Log.error("[{}] replace_wave: file '{}': failed to check stream length?", mod.metadata.id, slot.bundle_path);
        return MOD_ERROR;
    }

    SampleDataPcm16 pcm_buffer;
    pcm_buffer.data.resize(length);

    std::span span_pcm(pcm_buffer.data);

    while (!span_pcm.empty()) {
        auto read_result = op_read(
            handle,
            span_pcm.data(), helpers::cast::bounded_cast(span_pcm.size()),
            nullptr);

        if (read_result <= 0) {
            Log.error("[{}] replace_wave: file '{}': failed to read Opus: {}", mod.metadata.id, slot.bundle_path, read_result);
            return MOD_ERROR;
        }

        if (read_result > span_pcm.size()) {
            Log.fatal("What teh fuck?");
        }

        span_pcm = span_pcm.subspan(read_result);
    }

    pcm_buffer.be_swap();

    slot.data = std::make_shared<SampleDataPcm16>(std::move(pcm_buffer));
    slot.sample_rate = 48000; // Opus always decodes at 48 kHz.
    slot.sample_count = length;
    slot.format = AUDIO_WAVE_FORMAT_PCM16;

    return MOD_OK;
}
}

#endif
