#include "dusk/crash_reporting.h"

#include "dusk/app_info.hpp"
#include "dusk/dusk.h"
#include "dusk/logging.h"
#include "dusk/main.h"
#include "version.h"

#include <cstdlib>
#include <filesystem>
#include <string>
#include <string_view>
#include <system_error>

#if DUSK_ENABLE_SENTRY_NATIVE
#include <sentry.h>
#endif

namespace dusk::crash_reporting {

namespace {

#if DUSK_ENABLE_SENTRY_NATIVE
bool g_sentryInitialized = false;

bool truthy(std::string_view value) {
    return value == "1" || value == "true" || value == "TRUE" || value == "yes" || value == "YES" ||
           value == "on" || value == "ON";
}

std::string env_or_empty(const char* name) {
    if (const char* value = std::getenv(name)) {
        return value;
    }
    return {};
}

bool disabled_by_env() {
    const std::string env = env_or_empty("DUSK_SENTRY_ENABLED");
    return !env.empty() && !truthy(env);
}

std::string effective_dsn() {
    const std::string env = env_or_empty("DUSK_SENTRY_DSN");
    if (!env.empty()) {
        return env;
    }
    return DUSK_SENTRY_DSN;
}

bool effective_debug() {
    const std::string env = env_or_empty("DUSK_SENTRY_DEBUG");
    if (!env.empty()) {
        return truthy(env);
    }
    return false;
}

std::string release_name() {
    return std::string(AppName) + "@" DUSK_WC_DESCRIBE;
}

std::filesystem::path sentry_database_path() {
    return dusk::CachePath / "sentry";
}

std::filesystem::path log_attachment_path() {
    if (const char* logPath = GetLogFilePath()) {
        return logPath;
    }
    return {};
}

void configure_path_options(sentry_options_t* options) {
    const auto databasePath = sentry_database_path();
    std::error_code ec;
    std::filesystem::create_directories(databasePath, ec);
    if (ec) {
        DuskLog.warn(
            "Unable to create Sentry database path '{}': {}", databasePath.string(), ec.message());
    }

#if _WIN32
    const std::wstring databasePathWide = databasePath.wstring();
    sentry_options_set_database_pathw(options, databasePathWide.c_str());
#else
    const std::string databasePathUtf8 = databasePath.string();
    sentry_options_set_database_path(options, databasePathUtf8.c_str());
#endif

    const auto logPath = log_attachment_path();
    if (!logPath.empty()) {
#if _WIN32
        sentry_options_add_attachmentw(options, logPath.wstring().c_str());
#else
        sentry_options_add_attachment(options, logPath.string().c_str());
#endif
    }
}
#endif

}  // namespace

void initialize() {
#if DUSK_ENABLE_SENTRY_NATIVE
    if (g_sentryInitialized || disabled_by_env()) {
        return;
    }

    const std::string dsn = effective_dsn();
    if (dsn.empty()) {
        DuskLog.warn("Crash reporting is enabled but no Sentry DSN is configured");
        return;
    }

    const std::string release = release_name();

    sentry_options_t* options = sentry_options_new();
    sentry_options_set_dsn(options, dsn.c_str());
    sentry_options_set_release(options, release.c_str());
    sentry_options_set_environment(options, DUSK_SENTRY_ENVIRONMENT);
    sentry_options_set_debug(options, effective_debug() ? 1 : 0);
    sentry_options_set_require_user_consent(options, 1);
    sentry_options_set_cache_keep(options, 1);
    sentry_options_set_max_breadcrumbs(options, 100);
    configure_path_options(options);

    if (sentry_init(options) != 0) {
        DuskLog.warn("Failed to initialize Sentry crash reporting");
        return;
    }

    sentry_set_tag("git_branch", DUSK_WC_BRANCH);
    sentry_set_tag("build_type", DUSK_BUILD_TYPE);
    g_sentryInitialized = true;

    DuskLog.info("Initialized Sentry crash reporting");
#endif
}

void shutdown() {
#if DUSK_ENABLE_SENTRY_NATIVE
    if (!g_sentryInitialized) {
        return;
    }

    sentry_close();
    g_sentryInitialized = false;
#endif
}

Consent get_consent() {
#if DUSK_ENABLE_SENTRY_NATIVE
    if (!g_sentryInitialized) {
        return Consent::Unavailable;
    }

    switch (sentry_user_consent_get()) {
    case SENTRY_USER_CONSENT_GIVEN:
        return Consent::Given;
    case SENTRY_USER_CONSENT_REVOKED:
        return Consent::Revoked;
    case SENTRY_USER_CONSENT_UNKNOWN:
    default:
        return Consent::Unknown;
    }
#else
    return Consent::Unavailable;
#endif
}

void set_consent(bool enabled) {
#if DUSK_ENABLE_SENTRY_NATIVE
    if (!g_sentryInitialized) {
        return;
    }

    if (enabled) {
        sentry_user_consent_give();
    } else {
        sentry_user_consent_revoke();
    }
#else
    (void)enabled;
#endif
}

}  // namespace dusk::crash_reporting
