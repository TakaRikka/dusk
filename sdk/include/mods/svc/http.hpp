#pragma once

#include <mods/svc/http.h>

#include <algorithm>
#include <cctype>
#include <cstdint>
#include <functional>
#include <limits>
#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>
#include <vector>

namespace mods::http {

struct Header {
    std::string name;
    std::string value;
};

struct Request {
    HttpMethod method = HTTP_METHOD_GET;
    std::string url;
    std::vector<Header> headers;
    std::string body;
    std::string downloadPath;
    uint32_t connectTimeoutMs = 0;
    uint32_t idleTimeoutMs = 0;
    uint32_t totalTimeoutMs = 0;
    size_t maxBodyBytes = 0;
};

struct Response {
    HttpError error = HTTP_ERROR_NETWORK;
    std::string errorMessage;
    int statusCode = 0;
    std::vector<Header> headers;
    std::vector<uint8_t> body;
    std::string downloadPath;

    bool ok() const { return error == HTTP_ERROR_NONE && statusCode >= 200 && statusCode < 300; }

    const std::string* header(std::string_view name) const {
        const auto equal = [](std::string_view left, std::string_view right) {
            return left.size() == right.size() &&
                   std::equal(left.begin(), left.end(), right.begin(), [](char a, char b) {
                       return std::tolower(static_cast<unsigned char>(a)) ==
                              std::tolower(static_cast<unsigned char>(b));
                   });
        };
        const auto iter = std::find_if(headers.begin(), headers.end(),
            [&](const Header& value) { return equal(value.name, name); });
        return iter != headers.end() ? &iter->value : nullptr;
    }
};

namespace detail {

struct Completion {
    HttpRequestHandle handle = 0;
    std::function<void(Response)> callback;
};

inline std::unordered_map<HttpRequestHandle, std::unique_ptr<Completion>> completions;

inline void complete(ModContext*, HttpRequestHandle handle, const HttpResult* raw, void* userData) {
    const auto iter = completions.find(handle);
    if (iter == completions.end() || iter->second.get() != userData) {
        return;
    }
    auto completion = std::move(iter->second);
    completions.erase(iter);

    Response response;
    if (raw != nullptr) {
        response.error = raw->error;
        response.errorMessage = raw->error_message != nullptr ? raw->error_message : "";
        response.statusCode = raw->status_code;
        response.headers.reserve(raw->header_count);
        for (uint32_t i = 0; i < raw->header_count; ++i) {
            response.headers.push_back({
                .name = raw->headers[i].name != nullptr ? raw->headers[i].name : "",
                .value = raw->headers[i].value != nullptr ? raw->headers[i].value : "",
            });
        }
        if (raw->body != nullptr && raw->body_size != 0) {
            const auto* begin = static_cast<const uint8_t*>(raw->body);
            response.body.assign(begin, begin + raw->body_size);
        }
        response.downloadPath = raw->download_path != nullptr ? raw->download_path : "";
    }
    if (completion->callback) {
        completion->callback(std::move(response));
    }
}

}  // namespace detail

class Pending {
public:
    Pending() = default;
    Pending(HttpRequestHandle handle, ModResult result) : mHandle{handle}, mResult{result} {}
    ~Pending() { reset(); }
    Pending(const Pending&) = delete;
    Pending& operator=(const Pending&) = delete;
    Pending(Pending&& other) noexcept { *this = std::move(other); }
    Pending& operator=(Pending&& other) noexcept {
        if (this != &other) {
            reset();
            mHandle = std::exchange(other.mHandle, 0);
            mResult = other.mResult;
        }
        return *this;
    }

    explicit operator bool() const {
        if (mResult != MOD_OK || mHandle == 0 || svc_http == nullptr ||
            !detail::completions.contains(mHandle))
        {
            return false;
        }
        HttpProgress value = HTTP_PROGRESS_INIT;
        return svc_http->progress(mod_ctx, mHandle, &value) == MOD_OK;
    }
    ModResult result() const { return mResult; }
    HttpRequestHandle handle() const { return mHandle; }

    HttpProgress progress() const {
        HttpProgress value = HTTP_PROGRESS_INIT;
        if (mHandle != 0 && svc_http != nullptr) {
            svc_http->progress(mod_ctx, mHandle, &value);
        }
        return value;
    }

    void cancel() {
        if (mHandle != 0 && svc_http != nullptr) {
            if (svc_http->cancel(mod_ctx, mHandle) != MOD_OK) {
                detail::completions.erase(mHandle);
            }
        }
    }

    void detach() { mHandle = 0; }

private:
    void reset() {
        cancel();
        mHandle = 0;
    }

    HttpRequestHandle mHandle = 0;
    ModResult mResult = MOD_UNAVAILABLE;
};

inline Pending request(const Request& request, std::function<void(Response)> callback) {
    if (svc_http == nullptr) {
        return {0, MOD_UNAVAILABLE};
    }
    if (!callback) {
        return {0, MOD_INVALID_ARGUMENT};
    }

    if (request.headers.size() > std::numeric_limits<uint32_t>::max()) {
        return {0, MOD_INVALID_ARGUMENT};
    }
    std::vector<HttpHeader> headers;
    headers.reserve(request.headers.size());
    for (const auto& header : request.headers) {
        headers.push_back({
            .name = header.name.c_str(),
            .value = header.value.c_str(),
        });
    }
    HttpRequestDesc desc = HTTP_REQUEST_DESC_INIT;
    desc.method = request.method;
    desc.url = request.url.c_str();
    desc.headers = headers.empty() ? nullptr : headers.data();
    desc.header_count = static_cast<uint32_t>(headers.size());
    desc.body = request.body.empty() ? nullptr : request.body.data();
    desc.body_size = request.body.size();
    desc.download_path = request.downloadPath.empty() ? nullptr : request.downloadPath.c_str();
    desc.connect_timeout_ms = request.connectTimeoutMs;
    desc.idle_timeout_ms = request.idleTimeoutMs;
    desc.total_timeout_ms = request.totalTimeoutMs;
    desc.max_body_bytes = request.maxBodyBytes;

    auto completion = std::make_unique<detail::Completion>();
    auto* userData = completion.get();
    completion->callback = std::move(callback);
    HttpRequestHandle handle = 0;
    const auto result = svc_http->request(mod_ctx, &desc, detail::complete, userData, &handle);
    if (result != MOD_OK) {
        return {0, result};
    }
    completion->handle = handle;
    detail::completions.emplace(handle, std::move(completion));
    return {handle, result};
}

}  // namespace mods::http
