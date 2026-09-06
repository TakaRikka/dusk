#include "dusk/logging.h"

#include <tracy/Tracy.hpp>

#include <string_view>

bool StubLogEnabled = true;

using namespace std::literals::string_view_literals;

namespace {
constexpr std::string_view kStubFragments[] = {
    "is a stub"sv,
    "Unimplemented: BP register"sv,
    "Unhandled BP register"sv,
    "Unhandled XF register"sv,
    "Unhandled XF memory write"sv,
    "but selective updates are not implemented"sv,
};

bool is_for_stub_log(const std::string_view message) {
    for (const auto& fragment : kStubFragments) {
        if (message.find(fragment) != std::string_view::npos) {
            return true;
        }
    }
    return false;
}

bool divert_stub_messages(const borealis::log::Message& message) {
    ZoneScoped;
    if (!StubLogEnabled || !is_for_stub_log(message.text)) {
        return false;
    }
    dusk::SendToStubLog(message.level, message.module, message.text);
    return true;
}
}  // namespace

void dusk::InitializeLogging(
    const std::filesystem::path& cacheDir, const borealis::cli::StandardOptions& standard) {
    borealis::log::Options options{};
    options.level = borealis::LogLevel::Debug;
    options.fileDirectory = cacheDir.empty() ? std::filesystem::path{} : cacheDir / "logs";
    options.filePrefix = "dusklight";
    options.legacyFilePrefixes = {"dusk"};
    options.divert = &divert_stub_messages;
    standard.apply_to(options);
    borealis::log::init(options);
}
