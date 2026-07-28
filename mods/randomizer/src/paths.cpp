#include "paths.hpp"

#include "session.hpp"

namespace randomizer::paths {

std::filesystem::path GetRandomizerPath() {
    // TODO: need a more permanent directory than this
    return session::svc_mng.host->mod_dir(session::svc_mng.mod_ctx);
}

std::filesystem::path GetRandomizerSettingsPath() {
    return GetRandomizerPath() / "settings.yaml";
}

std::filesystem::path GetRandomizerPreferencesPath() {
    return GetRandomizerPath() / "preferences.yaml";
}

std::filesystem::path GetRandomizerSeedsPath() {
    return GetRandomizerPath() / "seeds";
}

}  // namespace randomizer::paths
