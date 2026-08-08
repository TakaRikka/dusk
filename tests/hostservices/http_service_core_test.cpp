#include "dusk/mods/svc/http_core.hpp"
#include "dusk/http/http.hpp"

#include <atomic>
#ifdef NDEBUG
#undef NDEBUG
#endif
#include <cassert>
#include <chrono>
#include <condition_variable>
#include <cstdint>
#include <cstring>
#include <functional>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

using dusk::mods::svc::HttpCore;

namespace dusk::http::test {
void reset(Result result);
Request captured();
int call_count();
}  // namespace dusk::http::test

namespace {

using namespace std::chrono_literals;

struct CallbackState {
    int calls = 0;
    std::vector<HttpResult> results;
    std::vector<uint32_t> statusCodes;
    std::vector<std::string> headerValues;
    std::vector<std::string> bodies;
    std::vector<std::thread::id> threads;
};

void record_callback(ModContext*, HttpRequestHandle, const HttpResponseView* response, void* userData) {
    auto& state = *static_cast<CallbackState*>(userData);
    ++state.calls;
    state.results.push_back(response->result);
    state.statusCodes.push_back(response->status_code);
    state.headerValues.emplace_back(response->header_count == 0 ? "" : response->headers[0].value,
        response->header_count == 0 ? 0 : response->headers[0].value_size);
    state.bodies.emplace_back(response->body_size == 0 ? "" : reinterpret_cast<const char*>(response->body),
        response->body_size);
    state.threads.push_back(std::this_thread::get_id());
}

HttpRequestDesc request_desc(std::string& url, HttpMethod method = HTTP_METHOD_GET,
    const uint8_t* body = nullptr, size_t bodySize = 0, const HttpHeader* headers = nullptr,
    size_t headerCount = 0, uint32_t responseMax = 32) {
    return HttpRequestDesc{
        .struct_size = sizeof(HttpRequestDesc),
        .method = method,
        .url = url.data(),
        .url_size = url.size(),
        .headers = headers,
        .header_count = headerCount,
        .body = body,
        .body_size = bodySize,
        .timeout_ms = 1,
        .max_response_size = responseMax,
    };
}

template <class Predicate>
void pump_until(HttpCore& core, Predicate&& done) {
    const auto deadline = std::chrono::steady_clock::now() + 2s;
    while (!done()) {
        core.frameBegin();
        assert(std::chrono::steady_clock::now() < deadline);
        std::this_thread::sleep_for(1ms);
    }
    core.frameBegin();
}

void test_validation_boundaries() {
    HttpCore core;
    CallbackState callbacks;
    ModContext* context = reinterpret_cast<ModContext*>(1);
    std::string url = "https://synthetic.invalid/";
    HttpRequestHandle handle{};
    auto desc = request_desc(url);
    assert(core.begin(context, &desc, record_callback, &callbacks, &handle) == MOD_OK);
    assert(handle.value != 0);

    desc.struct_size -= 1;
    assert(core.begin(context, &desc, record_callback, &callbacks, &handle) == MOD_INVALID_ARGUMENT);
    desc.struct_size = sizeof(desc);

    std::string tooLong(HTTP_URL_MAX_SIZE + 1, 'x');
    tooLong.replace(0, 8, "https://");
    desc = request_desc(tooLong);
    assert(core.begin(context, &desc, record_callback, &callbacks, &handle) == MOD_INVALID_ARGUMENT);

    std::string invalid = "HTTPS://synthetic.invalid/";
    desc = request_desc(invalid);
    assert(core.begin(context, &desc, record_callback, &callbacks, &handle) == MOD_INVALID_ARGUMENT);
    invalid = "https://user@synthetic.invalid/";
    desc = request_desc(invalid);
    assert(core.begin(context, &desc, record_callback, &callbacks, &handle) == MOD_INVALID_ARGUMENT);
    invalid = "https://synthetic.invalid/#fragment";
    desc = request_desc(invalid);
    assert(core.begin(context, &desc, record_callback, &callbacks, &handle) == MOD_INVALID_ARGUMENT);
    const std::string invalidUrls[] = {
        "https://synthetic.invalid/control\t",
        "https://synthetic.invalid/space here",
        std::string("https://synthetic.invalid/obs") + static_cast<char>(0x80),
        std::string("https://synthetic.invalid/malformed") + static_cast<char>(0xc3) + '(',
    };
    for (auto invalidUrl : invalidUrls) {
        desc = request_desc(invalidUrl);
        assert(core.begin(context, &desc, record_callback, &callbacks, &handle) == MOD_INVALID_ARGUMENT);
    }

    uint8_t body[] = {'x'};
    desc = request_desc(url, HTTP_METHOD_GET, body, sizeof(body));
    assert(core.begin(context, &desc, record_callback, &callbacks, &handle) == MOD_INVALID_ARGUMENT);
    desc = request_desc(url);
    const uint32_t invalidMethod = 99;
    static_assert(sizeof(desc.method) == sizeof(invalidMethod));
    std::memcpy(&desc.method, &invalidMethod, sizeof(invalidMethod));
    assert(core.begin(context, &desc, record_callback, &callbacks, &handle) == MOD_INVALID_ARGUMENT);

    const HttpHeader badName{
        .struct_size = sizeof(HttpHeader), .name = "bad name", .name_size = 8, .value = "", .value_size = 0};
    desc = request_desc(url, HTTP_METHOD_GET, nullptr, 0, &badName, 1);
    assert(core.begin(context, &desc, record_callback, &callbacks, &handle) == MOD_INVALID_ARGUMENT);
    const HttpHeader badValue{
        .struct_size = sizeof(HttpHeader), .name = "X-Test", .name_size = 6, .value = "a\r\nb", .value_size = 4};
    desc = request_desc(url, HTTP_METHOD_GET, nullptr, 0, &badValue, 1);
    assert(core.begin(context, &desc, record_callback, &callbacks, &handle) == MOD_INVALID_ARGUMENT);
    const char nulValue[] = {'a', '\0', 'b'};
    const HttpHeader nulValueHeader{
        .struct_size = sizeof(HttpHeader),
        .name = "X-Test",
        .name_size = 6,
        .value = nulValue,
        .value_size = sizeof(nulValue),
    };
    desc = request_desc(url, HTTP_METHOD_GET, nullptr, 0, &nulValueHeader, 1);
    assert(core.begin(context, &desc, record_callback, &callbacks, &handle) == MOD_INVALID_ARGUMENT);
    const HttpHeader allowedValues[] = {
        {sizeof(HttpHeader), "X-Test", 6, "\tprintable ASCII !~", 19},
        {sizeof(HttpHeader), "X-Empty", 7, "", 0},
    };
    desc = request_desc(url, HTTP_METHOD_GET, nullptr, 0, allowedValues, 2);
    assert(core.begin(context, &desc, record_callback, &callbacks, &handle) == MOD_OK);
    const std::string invalidValues[] = {
        std::string("control") + static_cast<char>(0x1f),
        std::string("obs") + static_cast<char>(0x80),
        std::string("malformed") + static_cast<char>(0xc3) + '(',
    };
    for (const auto& invalidValue : invalidValues) {
        const HttpHeader header{
            sizeof(HttpHeader), "X-Test", 6, invalidValue.data(), invalidValue.size()};
        desc = request_desc(url, HTTP_METHOD_GET, nullptr, 0, &header, 1);
        assert(core.begin(context, &desc, record_callback, &callbacks, &handle) == MOD_INVALID_ARGUMENT);
    }
    const HttpHeader hopByHop{
        .struct_size = sizeof(HttpHeader), .name = "Content-Length", .name_size = 14, .value = "1", .value_size = 1};
    desc = request_desc(url, HTTP_METHOD_GET, nullptr, 0, &hopByHop, 1);
    assert(core.begin(context, &desc, record_callback, &callbacks, &handle) == MOD_INVALID_ARGUMENT);
    const HttpHeader proxyConnection{
        .struct_size = sizeof(HttpHeader), .name = "Proxy-Connection", .name_size = 16, .value = "close", .value_size = 5};
    desc = request_desc(url, HTTP_METHOD_GET, nullptr, 0, &proxyConnection, 1);
    assert(core.begin(context, &desc, record_callback, &callbacks, &handle) == MOD_INVALID_ARGUMENT);
    const std::string nonAsciiName{'X', '-', static_cast<char>(0x80)};
    const HttpHeader nonAsciiNameHeader{
        .struct_size = sizeof(HttpHeader),
        .name = nonAsciiName.data(),
        .name_size = nonAsciiName.size(),
        .value = "value",
        .value_size = 5,
    };
    desc = request_desc(url, HTTP_METHOD_GET, nullptr, 0, &nonAsciiNameHeader, 1);
    assert(core.begin(context, &desc, record_callback, &callbacks, &handle) == MOD_INVALID_ARGUMENT);
    const HttpHeader mixedCaseForbiddenHeader{
        .struct_size = sizeof(HttpHeader), .name = "cOnNeCtIoN", .name_size = 10, .value = "close", .value_size = 5};
    desc = request_desc(url, HTTP_METHOD_GET, nullptr, 0, &mixedCaseForbiddenHeader, 1);
    assert(core.begin(context, &desc, record_callback, &callbacks, &handle) == MOD_INVALID_ARGUMENT);

    desc = request_desc(url);
    desc.url = nullptr;
    assert(core.begin(context, &desc, record_callback, &callbacks, &handle) == MOD_INVALID_ARGUMENT);
    desc = request_desc(url);
    desc.timeout_ms = 0;
    assert(core.begin(context, &desc, record_callback, &callbacks, &handle) == MOD_INVALID_ARGUMENT);
    desc.timeout_ms = 60001;
    assert(core.begin(context, &desc, record_callback, &callbacks, &handle) == MOD_INVALID_ARGUMENT);
    desc.timeout_ms = 1;
    desc.max_response_size = 0;
    assert(core.begin(context, &desc, record_callback, &callbacks, &handle) == MOD_INVALID_ARGUMENT);
    desc.max_response_size = HTTP_RESPONSE_MAX_SIZE + 1;
    assert(core.begin(context, &desc, record_callback, &callbacks, &handle) == MOD_INVALID_ARGUMENT);
    core.shutdown();
}

void test_copy_and_methods() {
    HttpCore::Request captured;
    HttpCore core([&](const HttpCore::Request& request, const std::atomic_bool&) {
        captured = request;
        return HttpCore::Response{.result = HTTP_RESULT_OK, .body = {'o', 'k'}};
    });
    CallbackState callbacks;
    ModContext* context = reinterpret_cast<ModContext*>(2);
    std::string url = "https://synthetic.invalid/post";
    std::string name = "X-Original";
    std::string value = "before";
    uint8_t body[] = {'a', 'b'};
    HttpHeader header{sizeof(HttpHeader), name.data(), name.size(), value.data(), value.size()};
    auto desc = request_desc(url, HTTP_METHOD_POST, body, sizeof(body), &header, 1);
    HttpRequestHandle handle{};
    assert(core.begin(context, &desc, record_callback, &callbacks, &handle) == MOD_OK);
    url.assign("https://synthetic.invalid/mutated");
    name.assign("X-Mutated");
    value.assign("after!");
    body[0] = 'z';
    pump_until(core, [&] { return callbacks.calls == 1; });
    assert(captured.method == HTTP_METHOD_POST);
    assert(captured.url == "https://synthetic.invalid/post");
    assert(captured.headers.size() == 1 && captured.headers[0].name == "X-Original");
    assert(captured.headers[0].value == "before");
    assert(captured.body == std::vector<uint8_t>({'a', 'b'}));
    assert(callbacks.bodies == std::vector<std::string>({"ok"}));
    core.shutdown();
}

void test_fifo_and_pump_thread() {
    std::vector<std::string> executed;
    HttpCore core([&](const HttpCore::Request& request, const std::atomic_bool&) {
        executed.push_back(request.url);
        return HttpCore::Response{.result = HTTP_RESULT_OK};
    });
    CallbackState callbacks;
    ModContext* context = reinterpret_cast<ModContext*>(3);
    const auto pumpThread = std::this_thread::get_id();
    for (int index = 0; index < 3; ++index) {
        std::string url = "https://synthetic.invalid/" + std::to_string(index);
        auto desc = request_desc(url);
        HttpRequestHandle handle{};
        assert(core.begin(context, &desc, record_callback, &callbacks, &handle) == MOD_OK);
    }
    assert(callbacks.calls == 0);
    pump_until(core, [&] { return callbacks.calls == 3; });
    assert(executed == std::vector<std::string>({
                           "https://synthetic.invalid/0",
                           "https://synthetic.invalid/1",
                           "https://synthetic.invalid/2",
                       }));
    for (const auto thread : callbacks.threads) {
        assert(thread == pumpThread);
    }
    core.shutdown();
}

struct ReentrantState {
    HttpCore* core = nullptr;
    ModContext* context = nullptr;
    std::string url = "https://synthetic.invalid/reentrant";
    int calls = 0;
};

void reentrant_callback(ModContext*, HttpRequestHandle, const HttpResponseView*, void* userData) {
    auto& state = *static_cast<ReentrantState*>(userData);
    ++state.calls;
    if (state.calls == 1) {
        auto desc = request_desc(state.url);
        HttpRequestHandle handle{};
        assert(state.core->begin(state.context, &desc, reentrant_callback, &state, &handle) == MOD_OK);
    }
}

void test_callback_reentrancy() {
    HttpCore core;
    ModContext* context = reinterpret_cast<ModContext*>(4);
    ReentrantState state{.core = &core, .context = context};
    auto desc = request_desc(state.url);
    HttpRequestHandle handle{};
    assert(core.begin(context, &desc, reentrant_callback, &state, &handle) == MOD_OK);
    pump_until(core, [&] { return state.calls == 2; });
    core.shutdown();
}

void test_cancel_and_owner() {
    std::mutex mutex;
    std::condition_variable started;
    std::condition_variable release;
    bool running = false;
    bool allowFinish = false;
    HttpCore core([&](const HttpCore::Request&, const std::atomic_bool&) {
        std::unique_lock lock(mutex);
        running = true;
        started.notify_one();
        release.wait(lock, [&] { return allowFinish; });
        return HttpCore::Response{.result = HTTP_RESULT_OK};
    });
    CallbackState callbacks;
    ModContext* owner = reinterpret_cast<ModContext*>(5);
    ModContext* other = reinterpret_cast<ModContext*>(6);
    std::string url = "https://synthetic.invalid/cancel";
    auto desc = request_desc(url);
    HttpRequestHandle runningHandle{};
    HttpRequestHandle queuedHandle{};
    assert(core.begin(owner, &desc, record_callback, &callbacks, &runningHandle) == MOD_OK);
    {
        std::unique_lock lock(mutex);
        assert(started.wait_for(lock, 2s, [&] { return running; }));
    }
    assert(core.begin(owner, &desc, record_callback, &callbacks, &queuedHandle) == MOD_OK);
    assert(core.cancel(other, runningHandle) == MOD_INVALID_ARGUMENT);
    assert(core.cancel(owner, queuedHandle) == MOD_OK);
    assert(core.cancel(owner, runningHandle) == MOD_OK);
    {
        std::lock_guard lock(mutex);
        allowFinish = true;
    }
    release.notify_one();
    pump_until(core, [&] { return callbacks.calls == 2; });
    assert(callbacks.results[0] == HTTP_RESULT_CANCELED);
    assert(callbacks.results[1] == HTTP_RESULT_CANCELED);
    assert(core.cancel(owner, runningHandle) == MOD_INVALID_ARGUMENT);
    core.shutdown();
}

void test_detach_and_response_limits() {
    HttpCore core([](const HttpCore::Request& request, const std::atomic_bool&) {
        if (request.url.ends_with("large")) {
            return HttpCore::Response{.result = HTTP_RESULT_OK, .body = {'1', '2', '3', '4'}};
        }
        return HttpCore::Response{
            .result = HTTP_RESULT_OK,
            .statusCode = 201,
            .headers = {{"X-Response", "owned"}},
            .body = {'o', 'w', 'n'},
        };
    });
    CallbackState detached;
    CallbackState active;
    ModContext* removedOwner = reinterpret_cast<ModContext*>(7);
    ModContext* activeOwner = reinterpret_cast<ModContext*>(8);
    std::string url = "https://synthetic.invalid/detach";
    auto desc = request_desc(url);
    HttpRequestHandle detachedHandle{};
    assert(core.begin(removedOwner, &desc, record_callback, &detached, &detachedHandle) == MOD_OK);
    core.modDetached(removedOwner);
    std::this_thread::sleep_for(10ms);
    core.frameBegin();
    assert(detached.calls == 0);

    url = "https://synthetic.invalid/owned";
    desc = request_desc(url);
    HttpRequestHandle activeHandle{};
    assert(core.begin(activeOwner, &desc, record_callback, &active, &activeHandle) == MOD_OK);
    pump_until(core, [&] { return active.calls == 1; });
    assert(active.results[0] == HTTP_RESULT_OK && active.bodies[0] == "own");
    assert(active.statusCodes[0] == 201 && active.headerValues[0] == "owned");

    url = "https://synthetic.invalid/large";
    desc = request_desc(url, HTTP_METHOD_GET, nullptr, 0, nullptr, 0, 3);
    assert(core.begin(activeOwner, &desc, record_callback, &active, &activeHandle) == MOD_OK);
    pump_until(core, [&] { return active.calls == 2; });
    assert(active.results[1] == HTTP_RESULT_RESPONSE_TOO_LARGE);
    core.shutdown();
}

void test_unavailable_and_clean_shutdown() {
    dusk::http::test::reset({.error = dusk::http::Error::NoBackend});
    HttpCore core;
    CallbackState callbacks;
    ModContext* context = reinterpret_cast<ModContext*>(9);
    std::string url = "https://synthetic.invalid/unavailable";
    auto desc = request_desc(url);
    HttpRequestHandle handle{};
    assert(core.begin(context, &desc, record_callback, &callbacks, &handle) == MOD_OK);
    pump_until(core, [&] { return callbacks.calls == 1; });
    assert(callbacks.results[0] == HTTP_RESULT_UNAVAILABLE);
    const auto started = std::chrono::steady_clock::now();
    core.shutdown();
    assert(std::chrono::steady_clock::now() - started < 1s);
    core.shutdown();
}

void test_default_transport_mapping() {
    ModContext* context = reinterpret_cast<ModContext*>(12);
    std::string url = "https://synthetic.invalid/mapped";
    std::string authorization = "Bearer distinctive-secret";
    const HttpHeader headers[] = {
        {sizeof(HttpHeader), "Authorization", 13, authorization.data(), authorization.size()},
        {sizeof(HttpHeader), "X-Test", 6, "mapped", 6},
    };
    const uint8_t body[] = {0, 1, 2, 3};
    auto desc = request_desc(url, HTTP_METHOD_POST, body, sizeof(body), headers, 2, 32);
    CallbackState callbacks;
    dusk::http::test::reset({
        .response = {
            .statusCode = 418,
            .headers = {{"X-Answer", "yes"}},
            .body = std::string("a\0b", 3),
        },
    });
    HttpCore core;
    HttpRequestHandle handle{};
    assert(core.begin(context, &desc, record_callback, &callbacks, &handle) == MOD_OK);
    pump_until(core, [&] { return callbacks.calls == 1; });
    const dusk::http::Request captured = dusk::http::test::captured();
    assert(dusk::http::test::call_count() == 1);
    assert(captured.method == dusk::http::Method::Post && captured.url == url);
    assert(captured.timeout == 1ms && captured.maxBodyBytes == 32);
    assert(captured.body == std::string("\0\1\2\3", 4));
    assert(captured.headers.size() == 2 && captured.headers[0].value == authorization);
    assert(callbacks.results[0] == HTTP_RESULT_OK && callbacks.statusCodes[0] == 418);
    assert(callbacks.bodies[0] == std::string("a\0b", 3));
    core.shutdown();

    const dusk::http::Result malformed[] = {
        {.response = {.statusCode = 99}},
        {.response = {.statusCode = 600}},
        {.response = {.statusCode = 200, .headers = {{"bad name", "value"}}}},
        {.response = {.statusCode = 200, .headers = std::vector<dusk::http::Header>(33)}},
        {.response = {.statusCode = 200, .headers = {{"X-Test", std::string(4097, 'x')}}}},
        {.error = dusk::http::Error::Network, .response = {.statusCode = 200, .body = "leak"}},
        {.error = dusk::http::Error::TooLarge, .response = {.statusCode = 200, .body = "leak"}},
    };
    for (const auto& transport : malformed) {
        dusk::http::test::reset(transport);
        HttpCore checked;
        CallbackState result;
        assert(checked.begin(context, &desc, record_callback, &result, &handle) == MOD_OK);
        pump_until(checked, [&] { return result.calls == 1; });
        const HttpResult expected = transport.error == dusk::http::Error::TooLarge
            ? HTTP_RESULT_RESPONSE_TOO_LARGE
            : HTTP_RESULT_FAILED;
        assert(result.results[0] == expected && result.statusCodes[0] == 0);
        assert(result.headerValues[0].empty() && result.bodies[0].empty());
        checked.shutdown();
    }

    dusk::http::test::reset({.error = dusk::http::Error::Timeout});
    HttpCore timed;
    CallbackState timeout_result;
    assert(timed.begin(context, &desc, record_callback, &timeout_result, &handle) == MOD_OK);
    pump_until(timed, [&] { return timeout_result.calls == 1; });
    assert(timeout_result.results[0] == HTTP_RESULT_TIMEOUT);
    assert(timeout_result.statusCodes[0] == 0);
    assert(timeout_result.headerValues[0].empty() && timeout_result.bodies[0].empty());
    timed.shutdown();
}

void test_shutdown_joins_active_executor_without_delivery() {
    std::mutex mutex;
    std::condition_variable executorStarted;
    std::condition_variable executorRelease;
    std::condition_variable shutdownFinished;
    bool executorRunning = false;
    bool releaseExecutor = false;
    bool shutdownReturned = false;
    CallbackState callbacks;
    HttpCore core([&](const HttpCore::Request&, const std::atomic_bool&) {
        std::unique_lock lock(mutex);
        executorRunning = true;
        executorStarted.notify_one();
        executorRelease.wait(lock, [&] { return releaseExecutor; });
        return HttpCore::Response{.result = HTTP_RESULT_OK};
    });
    ModContext* context = reinterpret_cast<ModContext*>(10);
    std::string url = "https://synthetic.invalid/shutdown";
    auto desc = request_desc(url);
    HttpRequestHandle handle{};
    assert(core.begin(context, &desc, record_callback, &callbacks, &handle) == MOD_OK);
    {
        std::unique_lock lock(mutex);
        assert(executorStarted.wait_for(lock, 2s, [&] { return executorRunning; }));
    }

    std::thread shutdownThread([&] {
        core.shutdown();
        {
            std::lock_guard lock(mutex);
            shutdownReturned = true;
        }
        shutdownFinished.notify_one();
    });

    const auto stoppingDeadline = std::chrono::steady_clock::now() + 2s;
    ModContext* probeContext = reinterpret_cast<ModContext*>(11);
    for (;;) {
        assert(std::chrono::steady_clock::now() < stoppingDeadline);
        HttpRequestHandle probe{};
        const auto result = core.begin(probeContext, &desc, record_callback, &callbacks, &probe);
        if (result == MOD_UNAVAILABLE) {
            break;
        }
        assert(result == MOD_OK);
        core.modDetached(probeContext);
        std::this_thread::yield();
    }

    {
        std::unique_lock lock(mutex);
        assert(!shutdownReturned);
        releaseExecutor = true;
    }
    executorRelease.notify_one();
    {
        std::unique_lock lock(mutex);
        assert(shutdownFinished.wait_for(lock, 2s, [&] { return shutdownReturned; }));
    }
    shutdownThread.join();
    core.frameBegin();
    assert(callbacks.calls == 0);
}

}  // namespace

int main() {
    test_validation_boundaries();
    test_copy_and_methods();
    test_fifo_and_pump_thread();
    test_callback_reentrancy();
    test_cancel_and_owner();
    test_detach_and_response_limits();
    test_unavailable_and_clean_shutdown();
    test_default_transport_mapping();
    test_shutdown_joins_active_executor_without_delivery();
}
