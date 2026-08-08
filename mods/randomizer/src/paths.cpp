#include "paths.hpp"

#include "session.hpp"

namespace randomizer::paths {

std::filesystem::path GetRandomizerPath() {
    const char* dataDir = nullptr;
    if (session::svc_mng.host->data_dir(session::svc_mng.mod_ctx, &dataDir) != MOD_OK) {
        session::svc_mng.host->fail(session::svc_mng.mod_ctx, MOD_ERROR, "Failed to get data directory");
    }

    return dataDir;
}

std::filesystem::path GetRandomizerSettingsPath() {
    return GetRandomizerPath() / "settings.yaml";
}

std::filesystem::path GetRandomizerPreferencesPath() {
    return GetRandomizerPath() / "preferences.yaml";
}

std::filesystem::path GetRandomizerPresetsPath() {
    return GetRandomizerPath() / "presets";
}

std::filesystem::path GetRandomizerSeedsPath() {
    return GetRandomizerPath() / "seeds";
}

}  // namespace randomizer::paths
