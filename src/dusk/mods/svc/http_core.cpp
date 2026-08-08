#include "http_core.hpp"

#include "dusk/http/http.hpp"

#include <algorithm>
#include <cstring>
#include <string_view>

namespace dusk::mods::svc {
namespace {

constexpr uint32_t kTimeoutMinMs = 1;
constexpr uint32_t kTimeoutMaxMs = 60000;

bool pointer_length_pair_valid(const void* pointer, const size_t size) {
    return pointer != nullptr || size == 0;
}

bool contains_cr_or_lf(std::string_view value) {
    return value.find_first_of("\r\n") != std::string_view::npos;
}

bool is_ascii_alpha(const unsigned char value) {
    return (value >= 'A' && value <= 'Z') || (value >= 'a' && value <= 'z');
}

bool is_token_byte(const unsigned char value) {
    return is_ascii_alpha(value) || (value >= '0' && value <= '9') ||
        std::string_view("!#$%&'*+-.^_`|~").find(static_cast<char>(value)) != std::string_view::npos;
}

unsigned char ascii_lower(const unsigned char value) {
    return value >= 'A' && value <= 'Z'
        ? static_cast<unsigned char>(value + ('a' - 'A'))
        : value;
}

bool is_token(const std::string_view value) {
    if (value.empty()) {
        return false;
    }
    for (const unsigned char ch : value) {
        if (is_token_byte(ch)) {
            continue;
        }
        return false;
    }
    return true;
}

bool equals_ascii_case_insensitive(const std::string_view lhs, const std::string_view rhs) {
    if (lhs.size() != rhs.size()) {
        return false;
    }
    for (size_t index = 0; index < lhs.size(); ++index) {
        if (ascii_lower(static_cast<unsigned char>(lhs[index])) !=
            ascii_lower(static_cast<unsigned char>(rhs[index])))
        {
            return false;
        }
    }
    return true;
}

bool is_forbidden_header(const std::string_view name) {
    constexpr std::string_view kForbidden[] = {
        "connection",
        "keep-alive",
        "proxy-authenticate",
        "proxy-authorization",
        "proxy-connection",
        "te",
        "trailer",
        "transfer-encoding",
        "upgrade",
        "content-length",
    };
    return std::ranges::any_of(kForbidden,
        [&](const std::string_view forbidden) { return equals_ascii_case_insensitive(name, forbidden); });
}

bool is_https_url(const std::string_view url) {
    if (!url.starts_with("https://") || url.size() <= std::string_view("https://").size() ||
        !std::ranges::all_of(url, [](const unsigned char value) {
            return value >= 0x21 && value <= 0x7e;
        }))
    {
        return false;
    }

    const auto authorityStart = std::string_view("https://").size();
    const auto authorityEnd = url.find_first_of("/?#", authorityStart);
    const auto authority = url.substr(authorityStart, authorityEnd - authorityStart);
    return !authority.empty() && authority.find('@') == std::string_view::npos &&
        url.find('#') == std::string_view::npos;
}

bool valid_response_headers(const std::vector<dusk::http::Header>& headers) {
    if (headers.size() > HTTP_HEADER_MAX_COUNT) {
        return false;
    }
    size_t aggregateSize = 0;
    for (const auto& header : headers) {
        if (header.name.size() > HTTP_HEADER_NAME_MAX_SIZE ||
            header.value.size() > HTTP_HEADER_VALUE_MAX_SIZE ||
            !is_token(header.name) || contains_cr_or_lf(header.value) ||
            header.value.find('\0') != std::string::npos ||
            aggregateSize > HTTP_HEADER_AGGREGATE_MAX_SIZE - header.name.size() ||
            aggregateSize + header.name.size() >
                HTTP_HEADER_AGGREGATE_MAX_SIZE - header.value.size())
        {
            return false;
        }
        aggregateSize += header.name.size() + header.value.size();
    }
    return true;
}

}  // namespace

HttpCore::HttpCore(Executor executor)
    : m_executor(executor ? std::move(executor) : defaultExecutor), m_worker(&HttpCore::workerMain, this) {}

HttpCore::~HttpCore() {
    shutdown();
}

ModResult HttpCore::begin(ModContext* context, const HttpRequestDesc* desc,
    const HttpCompletionFn callback, void* userData, HttpRequestHandle* outHandle) {
    if (outHandle == nullptr) {
        return MOD_INVALID_ARGUMENT;
    }
    outHandle->value = 0;

    Request request;
    if (context == nullptr || callback == nullptr || !validateRequest(desc, request)) {
        return MOD_INVALID_ARGUMENT;
    }

    std::lock_guard lock(m_mutex);
    if (m_stopping) {
        return MOD_UNAVAILABLE;
    }

    if (m_nextHandle == 0) {
        return MOD_UNAVAILABLE;
    }
    const HttpRequestHandle handle{.value = m_nextHandle++};
    auto entry = std::make_shared<Entry>();
    entry->owner = context;
    entry->handle = handle;
    entry->request = std::move(request);
    entry->callback = callback;
    entry->userData = userData;
    m_entries.emplace(handle.value, entry);
    m_work.push_back(entry);
    *outHandle = handle;
    m_workReady.notify_one();
    return MOD_OK;
}

ModResult HttpCore::cancel(ModContext* context, const HttpRequestHandle handle) {
    if (context == nullptr || handle.value == 0) {
        return MOD_INVALID_ARGUMENT;
    }

    std::lock_guard lock(m_mutex);
    const auto it = m_entries.find(handle.value);
    if (it == m_entries.end() || it->second->owner != context || it->second->suppressed) {
        return MOD_INVALID_ARGUMENT;
    }
    const auto& entry = it->second;
    if (entry->state == State::Queued) {
        entry->canceled.store(true, std::memory_order_release);
        m_work.erase(std::find(m_work.begin(), m_work.end(), entry));
        queueCompletionLocked(entry, Response{.result = HTTP_RESULT_CANCELED});
        return MOD_OK;
    }
    if (entry->state == State::Running) {
        entry->canceled.store(true, std::memory_order_release);
        return MOD_OK;
    }
    return MOD_INVALID_ARGUMENT;
}

void HttpCore::frameBegin() {
    for (;;) {
        std::shared_ptr<Entry> entry;
        {
            std::lock_guard lock(m_mutex);
            if (m_completions.empty()) {
                return;
            }
            entry = std::move(m_completions.front());
            m_completions.pop_front();
            const auto it = m_entries.find(entry->handle.value);
            if (it == m_entries.end() || it->second != entry || entry->suppressed ||
                entry->state != State::CompletionQueued)
            {
                continue;
            }
            entry->state = State::Delivered;
            m_entries.erase(it);
        }

        std::vector<HttpHeader> headers;
        headers.reserve(entry->response.headers.size());
        for (const auto& header : entry->response.headers) {
            headers.push_back(HttpHeader{
                .struct_size = sizeof(HttpHeader),
                .name = header.name.data(),
                .name_size = header.name.size(),
                .value = header.value.data(),
                .value_size = header.value.size(),
            });
        }
        const HttpResponseView response{
            .struct_size = sizeof(HttpResponseView),
            .result = entry->response.result,
            .status_code = entry->response.statusCode,
            .headers = headers.empty() ? nullptr : headers.data(),
            .header_count = headers.size(),
            .body = entry->response.body.empty() ? nullptr : entry->response.body.data(),
            .body_size = entry->response.body.size(),
        };
        entry->callback(entry->owner, entry->handle, &response, entry->userData);
    }
}

void HttpCore::modDetached(ModContext* context) {
    if (context == nullptr) {
        return;
    }

    std::lock_guard lock(m_mutex);
    for (auto it = m_entries.begin(); it != m_entries.end();) {
        const auto& entry = it->second;
        if (entry->owner != context) {
            ++it;
            continue;
        }
        entry->suppressed = true;
        entry->canceled.store(true, std::memory_order_release);
        entry->callback = nullptr;
        entry->userData = nullptr;
        if (entry->state == State::Queued) {
            m_work.erase(std::find(m_work.begin(), m_work.end(), entry));
        }
        it = m_entries.erase(it);
    }
    std::erase_if(m_completions,
        [&](const std::shared_ptr<Entry>& entry) { return entry->owner == context; });
}

void HttpCore::shutdown() {
    {
        std::lock_guard lock(m_mutex);
        if (m_stopping) {
            return;
        }
        m_stopping = true;
        for (auto& [_, entry] : m_entries) {
            entry->suppressed = true;
            entry->canceled.store(true, std::memory_order_release);
            entry->callback = nullptr;
            entry->userData = nullptr;
        }
        m_entries.clear();
        m_work.clear();
        m_completions.clear();
    }
    m_workReady.notify_all();
    if (m_worker.joinable()) {
        m_worker.join();
    }
}

bool HttpCore::validateRequest(const HttpRequestDesc* desc, Request& request) {
    static_assert(sizeof(HttpMethod) == sizeof(uint32_t));
    uint32_t method = 0;
    if (desc != nullptr) {
        std::memcpy(&method, &desc->method, sizeof(method));
    }
    if (desc == nullptr || desc->struct_size != sizeof(HttpRequestDesc) ||
        !pointer_length_pair_valid(desc->url, desc->url_size) ||
        !pointer_length_pair_valid(desc->headers, desc->header_count) ||
        !pointer_length_pair_valid(desc->body, desc->body_size) || desc->url_size == 0 ||
        desc->url_size > HTTP_URL_MAX_SIZE || desc->header_count > HTTP_HEADER_MAX_COUNT ||
        desc->body_size > HTTP_REQUEST_BODY_MAX_SIZE || desc->timeout_ms < kTimeoutMinMs ||
        desc->timeout_ms > kTimeoutMaxMs || desc->max_response_size == 0 ||
        desc->max_response_size > HTTP_RESPONSE_MAX_SIZE ||
        (method != HTTP_METHOD_GET && method != HTTP_METHOD_POST) ||
        (method == HTTP_METHOD_GET && desc->body_size != 0))
    {
        return false;
    }

    const std::string_view url{desc->url, desc->url_size};
    if (!is_https_url(url)) {
        return false;
    }

    size_t aggregateSize = 0;
    std::vector<Header> headers;
    headers.reserve(desc->header_count);
    for (size_t index = 0; index < desc->header_count; ++index) {
        const auto& source = desc->headers[index];
        if (source.struct_size != sizeof(HttpHeader) ||
            !pointer_length_pair_valid(source.name, source.name_size) ||
            !pointer_length_pair_valid(source.value, source.value_size) || source.name_size == 0 ||
            source.name_size > HTTP_HEADER_NAME_MAX_SIZE ||
            source.value_size > HTTP_HEADER_VALUE_MAX_SIZE ||
            aggregateSize > HTTP_HEADER_AGGREGATE_MAX_SIZE - source.name_size ||
            aggregateSize + source.name_size > HTTP_HEADER_AGGREGATE_MAX_SIZE - source.value_size)
        {
            return false;
        }
        const std::string_view name{source.name, source.name_size};
        const std::string_view value{source.value, source.value_size};
        if (!is_token(name) ||
            !std::ranges::all_of(value, [](const unsigned char byte) {
                return byte == '\t' || (byte >= 0x20 && byte <= 0x7e);
            }) ||
            is_forbidden_header(name)) {
            return false;
        }
        aggregateSize += source.name_size + source.value_size;
        headers.push_back(Header{.name = std::string(name), .value = std::string(value)});
    }

    request.method = static_cast<HttpMethod>(method);
    request.url = std::string(url);
    request.headers = std::move(headers);
    if (desc->body_size != 0) {
        request.body.assign(desc->body, desc->body + desc->body_size);
    }
    request.timeoutMs = desc->timeout_ms;
    request.maxResponseSize = desc->max_response_size;
    return true;
}

HttpCore::Response HttpCore::defaultExecutor(const Request& request,
    const std::atomic_bool& canceled) {
    if (canceled.load(std::memory_order_acquire)) {
        return Response{.result = HTTP_RESULT_CANCELED};
    }

    dusk::http::Request transportRequest{
        .method = request.method == HTTP_METHOD_POST ? dusk::http::Method::Post : dusk::http::Method::Get,
        .url = request.url,
        .timeout = std::chrono::milliseconds(request.timeoutMs),
        .maxBodyBytes = request.maxResponseSize,
    };
    transportRequest.headers.reserve(request.headers.size());
    for (const auto& header : request.headers) {
        transportRequest.headers.push_back({.name = header.name, .value = header.value});
    }
    if (!request.body.empty()) {
        transportRequest.body.assign(
            reinterpret_cast<const char*>(request.body.data()), request.body.size());
    }

    const dusk::http::Result transport = dusk::http::request(transportRequest);
    if (canceled.load(std::memory_order_acquire)) {
        return Response{.result = HTTP_RESULT_CANCELED};
    }
    if (transport.error == dusk::http::Error::NoBackend) {
        return Response{.result = HTTP_RESULT_UNAVAILABLE};
    }
    if (transport.error == dusk::http::Error::Timeout) {
        return Response{.result = HTTP_RESULT_TIMEOUT};
    }
    if (transport.error == dusk::http::Error::TooLarge) {
        return Response{.result = HTTP_RESULT_RESPONSE_TOO_LARGE};
    }
    if (transport.error != dusk::http::Error::None || transport.response.statusCode < 100 ||
        transport.response.statusCode > 599 ||
        transport.response.body.size() > request.maxResponseSize ||
        !valid_response_headers(transport.response.headers))
    {
        return Response{.result = HTTP_RESULT_FAILED};
    }

    Response response{
        .result = HTTP_RESULT_OK,
        .statusCode = static_cast<uint32_t>(transport.response.statusCode),
        .body = std::vector<uint8_t>(transport.response.body.begin(), transport.response.body.end()),
    };
    response.headers.reserve(transport.response.headers.size());
    for (const auto& header : transport.response.headers) {
        response.headers.push_back({.name = header.name, .value = header.value});
    }
    return response;
}

void HttpCore::workerMain() {
    for (;;) {
        std::shared_ptr<Entry> entry;
        {
            std::unique_lock lock(m_mutex);
            m_workReady.wait(lock, [&] { return m_stopping || !m_work.empty(); });
            if (m_stopping && m_work.empty()) {
                return;
            }
            entry = std::move(m_work.front());
            m_work.pop_front();
            if (entry->suppressed || !m_entries.contains(entry->handle.value)) {
                continue;
            }
            entry->state = State::Running;
        }

        Response response;
        if (entry->canceled.load(std::memory_order_acquire)) {
            response.result = HTTP_RESULT_CANCELED;
        } else {
            try {
                response = m_executor(entry->request, entry->canceled);
            } catch (...) {
                response.result = HTTP_RESULT_FAILED;
                response.statusCode = 0;
                response.headers.clear();
                response.body.clear();
            }
        }

        std::lock_guard lock(m_mutex);
        const auto it = m_entries.find(entry->handle.value);
        if (it == m_entries.end() || it->second != entry || entry->suppressed || m_stopping) {
            continue;
        }
        if (entry->canceled.load(std::memory_order_acquire)) {
            response = Response{.result = HTTP_RESULT_CANCELED};
        } else if (response.result != HTTP_RESULT_OK) {
            response.statusCode = 0;
            response.headers.clear();
            response.body.clear();
        } else if (response.body.size() > entry->request.maxResponseSize) {
            response = Response{.result = HTTP_RESULT_RESPONSE_TOO_LARGE};
        }
        queueCompletionLocked(entry, std::move(response));
    }
}

void HttpCore::queueCompletionLocked(const std::shared_ptr<Entry>& entry, Response response) {
    entry->response = std::move(response);
    entry->state = State::CompletionQueued;
    m_completions.push_back(entry);
}

}  // namespace dusk::mods::svc
