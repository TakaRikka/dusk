#include "dusk/crash_reporting.h"

#include "dusk/app_info.hpp"
#include "dusk/dusk.h"
#include "dusk/logging.h"
#include "dusk/settings.h"
#include "version.h"

#include <cstdlib>
#include <filesystem>
#include <string>
#include <string_view>
#include <system_error>

#include "SDL3/SDL_filesystem.h"

#if DUSK_ENABLE_SENTRY_NATIVE
#include <sentry.h>
#endif

namespace dusk {

namespace {

#if DUSK_ENABLE_SENTRY_NATIVE
bool g_sentryInitialized = false;

bool IsTruthy(std::string_view value) {
    return value == "1" || value == "true" || value == "TRUE" || value == "yes"
        || value == "YES" || value == "on" || value == "ON";
}

std::string GetEnvOrEmpty(const char* name) {
    if (const char* value = std::getenv(name)) {
        return value;
    }
    return {};
}

bool GetEffectiveEnabled() {
    const std::string env = GetEnvOrEmpty("DUSK_SENTRY_ENABLED");
    if (!env.empty()) {
        return IsTruthy(env);
    }
    return getSettings().backend.enableCrashReporting;
}

std::string GetEffectiveDsn() {
    const std::string env = GetEnvOrEmpty("DUSK_SENTRY_DSN");
    if (!env.empty()) {
        return env;
    }
    return DUSK_SENTRY_DSN;
}

bool GetEffectiveDebug() {
    const std::string env = GetEnvOrEmpty("DUSK_SENTRY_DEBUG");
    if (!env.empty()) {
        return IsTruthy(env);
    }
    return false;
}

std::string GetReleaseName() {
    return std::string(AppName) + "@" DUSK_WC_DESCRIBE;
}

std::filesystem::path GetSentryDatabasePath() {
    return std::filesystem::path(configPath) / "sentry";
}

std::filesystem::path GetCrashpadHandlerPath() {
    const char* basePath = SDL_GetBasePath();
    if (!basePath) {
        return {};
    }

    const std::filesystem::path handlerDir(basePath);
#if _WIN32
    return handlerDir / "crashpad_handler.exe";
#else
    return handlerDir / "crashpad_handler";
#endif
}

void ConfigurePathOptions(sentry_options_t* options) {
    const auto databasePath = GetSentryDatabasePath();
    std::error_code ec;
    std::filesystem::create_directories(databasePath, ec);
    if (ec) {
        DuskLog.warn("Unable to create Sentry database path '{}': {}",
                     databasePath.string(), ec.message());
    }

#if _WIN32
    const std::wstring databasePathWide = databasePath.wstring();
    sentry_options_set_database_pathw(options, databasePathWide.c_str());

    const auto handlerPath = GetCrashpadHandlerPath();
    if (!handlerPath.empty()) {
        const std::wstring handlerPathWide = handlerPath.wstring();
        sentry_options_set_handler_pathw(options, handlerPathWide.c_str());
    }
#else
    const std::string databasePathUtf8 = databasePath.string();
    sentry_options_set_database_path(options, databasePathUtf8.c_str());

    const auto handlerPath = GetCrashpadHandlerPath();
    if (!handlerPath.empty()) {
        const std::string handlerPathUtf8 = handlerPath.string();
        sentry_options_set_handler_path(options, handlerPathUtf8.c_str());
    }
#endif
}
#endif

}  // namespace

void InitializeCrashReporting() {
#if DUSK_ENABLE_SENTRY_NATIVE
    if (g_sentryInitialized) {
        return;
    }

    if (!GetEffectiveEnabled()) {
        return;
    }

    const std::string dsn = GetEffectiveDsn();
    if (dsn.empty()) {
        DuskLog.warn("Crash reporting is enabled but no Sentry DSN is configured");
        return;
    }

    const std::string release = GetReleaseName();

    sentry_options_t* options = sentry_options_new();
    sentry_options_set_dsn(options, dsn.c_str());
    sentry_options_set_release(options, release.c_str());
    sentry_options_set_environment(options, DUSK_SENTRY_ENVIRONMENT);
    sentry_options_set_debug(options, GetEffectiveDebug() ? 1 : 0);
    sentry_options_set_cache_keep(options, 1);
    sentry_options_set_max_breadcrumbs(options, 100);
    ConfigurePathOptions(options);

    if (sentry_init(options) != 0) {
        DuskLog.warn("Failed to initialize Sentry crash reporting");
        return;
    }

    sentry_set_tag("git_branch", DUSK_WC_BRANCH);
    sentry_set_tag("build_type", DUSK_BUILD_TYPE);
    sentry_set_tag("tp_version", DUSK_TP_VERSION);
    g_sentryInitialized = true;

    DuskLog.info("Initialized Sentry crash reporting");
#endif
}

void ShutdownCrashReporting() {
#if DUSK_ENABLE_SENTRY_NATIVE
    if (!g_sentryInitialized) {
        return;
    }

    sentry_close();
    g_sentryInitialized = false;
#endif
}

}  // namespace dusk
