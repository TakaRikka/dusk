#include "registry.hpp"

#include "internal.hpp"
#include "net.hpp"

#include "dusk/main.h"
#include "dusk/mods/loader/loader.hpp"
#include "mods/svc/http.h"

#include <borealis/http.hpp>
#include <borealis/io.hpp>
#include <borealis/url.hpp>
#include <fmt/format.h>
#include <xxhash.h>

#include <algorithm>
#include <cassert>
#include <chrono>
#include <cstdint>
#include <filesystem>
#include <optional>
#include <ranges>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>
#include <vector>

namespace dusk::mods::svc {
namespace {

constexpr size_t MaxRequestsPerMod = 16;
constexpr size_t MaxUrlBytes = 8 * 1024;
constexpr size_t MaxHeaders = 64;
constexpr size_t MaxHeaderBytes = 16 * 1024;
constexpr size_t MaxRequestBodyBytes = 16 * 1024 * 1024;
constexpr size_t DefaultResponseBodyBytes = 1024 * 1024;
constexpr size_t MaxResponseBodyBytes = 64 * 1024 * 1024;
constexpr std::chrono::milliseconds DefaultTimeout{10000};
constexpr std::string_view ReservedHeaders[]{
    "User-Agent",
    "Host",
    "Content-Length",
    "Connection",
    "Accept-Encoding",
    "Range",
    "If-Range",
};

struct PendingRequest {
    HttpCompleteFn callback = nullptr;
    void* userData = nullptr;
    borealis::Task<borealis::http::Result> task;
    std::filesystem::path stagingPath;
    std::filesystem::path downloadPath;
    bool completing = false;
};

static_assert(std::is_nothrow_move_constructible_v<PendingRequest>);

SlotMap<PendingRequest> s_requests;

bool valid_url(std::string_view url) {
    const auto parsed = url.size() <= MaxUrlBytes ? borealis::url::parse(url) : std::nullopt;
    return parsed && parsed->scheme == "https";
}

std::filesystem::path normalized_absolute(const std::filesystem::path& path, std::error_code& ec) {
    auto result = std::filesystem::absolute(path, ec);
    return ec ? std::filesystem::path{} : result.lexically_normal();
}

bool path_is_below(const std::filesystem::path& path, const std::filesystem::path& directory) {
    const auto [directoryEnd, pathPosition] =
        std::mismatch(directory.begin(), directory.end(), path.begin(), path.end());
    return directoryEnd == directory.end() && pathPosition != path.end();
}

bool path_is_at_or_below(
    const std::filesystem::path& path, const std::filesystem::path& directory) {
    const auto [directoryEnd, pathPosition] =
        std::mismatch(directory.begin(), directory.end(), path.begin(), path.end());
    (void)pathPosition;
    return directoryEnd == directory.end();
}

std::filesystem::path data_root(const LoadedMod& mod, std::error_code& ec) {
    if (!mod.dataDirUtf8.empty()) {
        return normalized_absolute(borealis::io::fs_path_from_utf8(mod.dataDirUtf8), ec);
    }
    return normalized_absolute(ConfigPath / "mod_data" / mod.metadata.id, ec);
}

std::optional<std::filesystem::path> validate_download_path(
    const LoadedMod& mod, const char* rawPath) {
    if (rawPath == nullptr) {
        return std::filesystem::path{};
    }
    const auto supplied = borealis::io::fs_path_from_utf8(rawPath);
    if (!supplied.is_absolute()) {
        return std::nullopt;
    }

    std::error_code ec;
    const auto path = normalized_absolute(supplied, ec);
    if (ec) {
        return std::nullopt;
    }
    const auto modRoot = normalized_absolute(mod.dir, ec);
    if (ec) {
        return std::nullopt;
    }
    const auto dataRoot = data_root(mod, ec);
    if (ec) {
        return std::nullopt;
    }
    const auto stagingRoot = (modRoot / "downloads").lexically_normal();
    if ((!path_is_below(path, modRoot) && !path_is_below(path, dataRoot)) ||
        path_is_at_or_below(path, stagingRoot))
    {
        return std::nullopt;
    }
    return path;
}

std::filesystem::path staging_path(const LoadedMod& mod, std::string_view url) {
    const auto& modId = mod.metadata.id;
    const auto modHash = XXH64(modId.data(), modId.size(), 0);
    const auto hash = XXH64(url.data(), url.size(), modHash);
    return mod.dir / "downloads" / fmt::format("{:016x}.part", hash);
}

HttpError map_error(borealis::http::Error error) {
    switch (error) {
    case borealis::http::Error::None:
        return HTTP_ERROR_NONE;
    case borealis::http::Error::InvalidUrl:
        return HTTP_ERROR_INVALID_URL;
    case borealis::http::Error::UnsupportedScheme:
        return HTTP_ERROR_UNSUPPORTED_SCHEME;
    case borealis::http::Error::Timeout:
        return HTTP_ERROR_TIMEOUT;
    case borealis::http::Error::TooLarge:
        return HTTP_ERROR_TOO_LARGE;
    case borealis::http::Error::Canceled:
        return HTTP_ERROR_CANCELED;
    case borealis::http::Error::Io:
        return HTTP_ERROR_IO;
    case borealis::http::Error::NoBackend:
    case borealis::http::Error::Network:
        return HTTP_ERROR_NETWORK;
    default:
        return HTTP_ERROR_NETWORK;
    }
}

std::optional<borealis::http::Method> borealis_method(HttpMethod method) {
    switch (method) {
    case HTTP_METHOD_GET:
        return borealis::http::Method::Get;
    case HTTP_METHOD_POST:
        return borealis::http::Method::Post;
    case HTTP_METHOD_HEAD:
        return borealis::http::Method::Head;
    }
    return std::nullopt;
}

borealis::http::Result publish_download(borealis::http::Result result,
    const std::filesystem::path& staging, const std::filesystem::path& destination) noexcept {
    if (result.error != borealis::http::Error::None) {
        return result;
    }

    try {
        std::string renameError;
        if (borealis::io::atomic_replace(staging, destination, renameError)) {
            return result;
        }

        std::filesystem::path temporary = destination;
        temporary += "." + borealis::io::fs_path_to_string(staging.filename()) + ".part";
        std::error_code ec;
        std::filesystem::copy_file(
            staging, temporary, std::filesystem::copy_options::overwrite_existing, ec);
        if (ec) {
            const std::string copyError = ec.message();
            std::error_code ignored;
            std::filesystem::remove(temporary, ignored);
            result.error = borealis::http::Error::Io;
            result.message = "Failed to publish download: " + copyError;
            return result;
        }

        std::string replaceError;
        if (!borealis::io::atomic_replace(temporary, destination, replaceError)) {
            std::filesystem::remove(temporary, ec);
            result.error = borealis::http::Error::Io;
            result.message = "Failed to publish download: " + replaceError;
            return result;
        }
        std::filesystem::remove(staging, ec);
        return result;
    } catch (const std::exception& exception) {
        result.error = borealis::http::Error::Io;
        result.message = std::string{"Failed to publish download: "} + exception.what();
        return result;
    } catch (...) {
        result.error = borealis::http::Error::Io;
        result.message = "Failed to publish download";
        return result;
    }
}

void http_frame_begin() {
    std::vector<HttpRequestHandle> ready;
    s_requests.for_each([&](const HttpRequestHandle handle, const auto& entry) {
        const auto& pending = entry.value;
        if (!pending.completing && pending.task.ready()) {
            ready.push_back(handle);
        }
    });

    for (const auto handle : ready) {
        auto* entry = s_requests.find(handle);
        if (entry == nullptr || !entry->owner->active || entry->value.completing) {
            continue;
        }
        auto& pending = entry->value;
        borealis::http::Result result;
        try {
            auto completed = pending.task.try_take();
            if (!completed.has_value()) {
                continue;
            }
            result = std::move(*completed);
        } catch (const std::exception& exception) {
            result = {
                .error = borealis::http::Error::Io,
                .message = exception.what(),
            };
        } catch (...) {
            result = {
                .error = borealis::http::Error::Io,
                .message = "HTTP request completion failed",
            };
        }

        auto* owner = entry->owner;
        const auto callback = pending.callback;
        const auto userData = pending.userData;
        pending.completing = true;

        std::vector<HttpHeader> headers;
        headers.reserve(result.response.headers.size());
        for (const auto& header : result.response.headers) {
            headers.push_back({.name = header.name.c_str(), .value = header.value.c_str()});
        }
        const bool downloadSucceeded =
            !pending.downloadPath.empty() && result.error == borealis::http::Error::None;
        const auto publishedPath = downloadSucceeded ?
                                       borealis::io::fs_path_to_string(pending.downloadPath) :
                                       std::string{};
        const HttpResult snapshot{
            .struct_size = sizeof(HttpResult),
            .error = map_error(result.error),
            .error_message = result.message.c_str(),
            .status_code = result.response.statusCode,
            .headers = headers.empty() ? nullptr : headers.data(),
            .header_count = static_cast<uint32_t>(headers.size()),
            .body = pending.downloadPath.empty() && !result.response.body.empty() ?
                        result.response.body.data() :
                        nullptr,
            .body_size = pending.downloadPath.empty() ? result.response.body.size() : 0,
            .download_path = downloadSucceeded ? publishedPath.c_str() : nullptr,
        };

        guarded_callback(*owner, "HTTP completion callback",
            [&] { callback(owner->context.get(), handle, &snapshot, userData); });
        s_requests.erase(handle);
    }
}

size_t active_request_count(const LoadedMod& mod) {
    size_t count = 0;
    s_requests.for_each([&](HttpRequestHandle, const auto& entry) {
        if (entry.owner == &mod && !entry.value.completing) {
            ++count;
        }
    });
    return count;
}

bool staging_path_in_use(const LoadedMod& mod, const std::filesystem::path& path) {
    bool inUse = false;
    s_requests.for_each([&](HttpRequestHandle, const auto& entry) {
        if (entry.owner == &mod && !entry.value.completing && entry.value.stagingPath == path) {
            inUse = true;
        }
    });
    return inUse;
}

ModResult start_request(LoadedMod& mod, const HttpRequestDesc& desc, HttpCompleteFn callback,
    void* userData, HttpRequestHandle& outHandle) {
    const std::string_view url{desc.url};
    const auto method = borealis_method(desc.method);
    if (!valid_url(url) || !method.has_value() ||
        (desc.header_count != 0 && desc.headers == nullptr) ||
        (desc.body_size != 0 && desc.body == nullptr) || desc.body_size > MaxRequestBodyBytes ||
        ((desc.method == HTTP_METHOD_GET || desc.method == HTTP_METHOD_HEAD) &&
            desc.body_size != 0) ||
        (desc.method == HTTP_METHOD_HEAD && desc.download_path != nullptr) ||
        desc.header_count > MaxHeaders)
    {
        return MOD_INVALID_ARGUMENT;
    }
    size_t headerBytes = 0;
    for (uint32_t i = 0; i < desc.header_count; ++i) {
        const auto& header = desc.headers[i];
        if (header.name == nullptr || header.value == nullptr) {
            return MOD_INVALID_ARGUMENT;
        }
        const std::string_view name{header.name};
        const std::string_view value{header.value};
        if (!valid_header(name, value, ReservedHeaders, true) ||
            name.size() > MaxHeaderBytes - headerBytes)
        {
            return MOD_INVALID_ARGUMENT;
        }
        headerBytes += name.size();
        if (value.size() > MaxHeaderBytes - headerBytes) {
            return MOD_INVALID_ARGUMENT;
        }
        headerBytes += value.size();
    }

    auto downloadPath = validate_download_path(mod, desc.download_path);
    if (!downloadPath.has_value() ||
        (downloadPath->empty() && desc.max_body_bytes > MaxResponseBodyBytes))
    {
        return MOD_INVALID_ARGUMENT;
    }
    if (active_request_count(mod) >= MaxRequestsPerMod) {
        return MOD_CONFLICT;
    }

    std::filesystem::path staging;
    if (!downloadPath->empty()) {
        staging = staging_path(mod, url);
        if (staging_path_in_use(mod, staging)) {
            return MOD_CONFLICT;
        }
        std::error_code ec;
        std::filesystem::create_directories(staging.parent_path(), ec);
        if (ec) {
            return MOD_ERROR;
        }
        std::filesystem::create_directories(downloadPath->parent_path(), ec);
        if (ec) {
            return MOD_ERROR;
        }
    }

    PendingRequest pending{
        .callback = callback,
        .userData = userData,
        .stagingPath = staging,
        .downloadPath = *downloadPath,
    };

    if (!borealis::http::available()) {
        return MOD_UNAVAILABLE;
    }
    borealis::http::Request request{
        .method = *method,
        .url = std::string{url},
        .body = desc.body_size != 0 ?
                    std::string{static_cast<const char*>(desc.body), desc.body_size} :
                    std::string{},
        .downloadTo = staging,
        .connectTimeout = desc.connect_timeout_ms != 0 ?
                              std::chrono::milliseconds{desc.connect_timeout_ms} :
                              DefaultTimeout,
        .idleTimeout = desc.idle_timeout_ms != 0 ? std::chrono::milliseconds{desc.idle_timeout_ms} :
                                                   DefaultTimeout,
        .totalTimeout = desc.total_timeout_ms != 0 ?
                            std::optional{std::chrono::milliseconds{desc.total_timeout_ms}} :
                            std::nullopt,
        .maxBodyBytes = desc.max_body_bytes != 0 ? desc.max_body_bytes : DefaultResponseBodyBytes,
    };
    request.headers.reserve(desc.header_count + 1);
    for (uint32_t i = 0; i < desc.header_count; ++i) {
        request.headers.push_back({desc.headers[i].name, desc.headers[i].value});
    }
    request.headers.push_back({
        .name = "User-Agent",
        .value = user_agent(mod),
    });

    auto task = borealis::http::start(std::move(request));
    if (task.ready()) {
        auto immediate = task.try_take();
        if (!immediate.has_value()) {
            return MOD_UNAVAILABLE;
        }
        if (immediate->error == borealis::http::Error::NoBackend) {
            return MOD_UNAVAILABLE;
        }
        task = borealis::detail::make_ready_task(std::move(*immediate));
    }
    if (!downloadPath->empty()) {
        task = std::move(task).map([staging, destination = *downloadPath](auto&& result) {
            return publish_download(std::move(result), staging, destination);
        });
    }
    pending.task = std::move(task);

    outHandle = s_requests.emplace(mod, std::move(pending));
    return MOD_OK;
}

ModResult http_request(ModContext* context, const HttpRequestDesc* desc, HttpCompleteFn callback,
    void* userData, HttpRequestHandle* outHandle) {
    if (outHandle != nullptr) {
        *outHandle = 0;
    }
    auto* mod = mod_from_context(context);
    if (mod == nullptr || desc == nullptr || desc->struct_size < sizeof(HttpRequestDesc) ||
        desc->url == nullptr || callback == nullptr || outHandle == nullptr)
    {
        return MOD_INVALID_ARGUMENT;
    }
    if (!declares_import(*mod, HTTP_SERVICE_ID)) {
        return MOD_UNSUPPORTED;
    }
    return start_request(*mod, *desc, callback, userData, *outHandle);
}

ModResult http_progress(ModContext* context, HttpRequestHandle handle, HttpProgress* outProgress) {
    const uint32_t structSize = outProgress != nullptr ? outProgress->struct_size : 0;
    auto* mod = mod_from_context(context);
    if (mod == nullptr || outProgress == nullptr || structSize < sizeof(HttpProgress)) {
        return MOD_INVALID_ARGUMENT;
    }
    *outProgress = HttpProgress{.struct_size = structSize};
    const auto* entry = s_requests.find_owned(handle, *mod);
    if (entry == nullptr) {
        return MOD_UNAVAILABLE;
    }

    const auto progress = entry->value.task.progress();
    outProgress->completed_bytes = progress.completed;
    outProgress->total_bytes = progress.total.value_or(0);
    outProgress->total_known = progress.total.has_value();
    return MOD_OK;
}

ModResult http_cancel(ModContext* context, HttpRequestHandle handle) {
    auto* mod = mod_from_context(context);
    if (mod == nullptr) {
        return MOD_INVALID_ARGUMENT;
    }
    auto* entry = s_requests.find_owned(handle, *mod);
    if (entry == nullptr) {
        return MOD_UNAVAILABLE;
    }
    entry->value.task.cancel();
    return MOD_OK;
}

void http_mod_deactivating(LoadedMod& mod) {
    (void)s_requests.take_all(mod);
}

void http_mod_detached(LoadedMod& mod) {
    bool found = false;
    s_requests.for_each(
        [&](HttpRequestHandle, const auto& entry) { found = found || entry.owner == &mod; });
    assert(!found);
}

void http_shutdown() {
    s_requests = {};
}

bool http_available() {
    return borealis::http::available();
}

constexpr HttpService s_httpService{
    .header = SERVICE_HEADER(HttpService, HTTP_SERVICE_MAJOR, HTTP_SERVICE_MINOR),
    .request = SERVICE_FUNCTION(http_request),
    .progress = SERVICE_FUNCTION(http_progress),
    .cancel = SERVICE_FUNCTION(http_cancel),
};

}  // namespace

constinit const ServiceModule g_httpModule{
    .id = HTTP_SERVICE_ID,
    .majorVersion = HTTP_SERVICE_MAJOR,
    .minorVersion = HTTP_SERVICE_MINOR,
    .service = &s_httpService,
    .available = http_available,
    .modDeactivating = http_mod_deactivating,
    .modDetached = http_mod_detached,
    .frameBegin = http_frame_begin,
    .shutdown = http_shutdown,
};

}  // namespace dusk::mods::svc
