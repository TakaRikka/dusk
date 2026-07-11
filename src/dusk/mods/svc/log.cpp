#include "registry.hpp"

#include "dusk/mods/loader/loader.hpp"
#include "dusk/mods/log_buffer.hpp"

namespace dusk::mods::svc {
namespace {

void log_write(ModContext* context, const LogLevel level, const char* message) {
    const char* text = message != nullptr ? message : "";
    log::emit(log::Source::Mod, mod_id_from_context(context), level, text);
}

void log_trace(ModContext* context, const char* message) {
    log_write(context, LOG_LEVEL_TRACE, message);
}

void log_debug(ModContext* context, const char* message) {
    log_write(context, LOG_LEVEL_DEBUG, message);
}

void log_info(ModContext* context, const char* message) {
    log_write(context, LOG_LEVEL_INFO, message);
}

void log_warn(ModContext* context, const char* message) {
    log_write(context, LOG_LEVEL_WARN, message);
}

void log_error(ModContext* context, const char* message) {
    log_write(context, LOG_LEVEL_ERROR, message);
}

constexpr LogService s_logService{
    .header = SERVICE_HEADER(LogService, LOG_SERVICE_MAJOR, LOG_SERVICE_MINOR),
    .write = log_write,
    .trace = log_trace,
    .debug = log_debug,
    .info = log_info,
    .warn = log_warn,
    .error = log_error,
};

}  // namespace

constinit const ServiceModule g_logModule{
    .id = LOG_SERVICE_ID,
    .majorVersion = LOG_SERVICE_MAJOR,
    .minorVersion = LOG_SERVICE_MINOR,
    .service = &s_logService,
};

}  // namespace dusk::mods::svc
