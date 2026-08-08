#pragma once

#include "mods/svc/http.h"

#include <atomic>
#include <condition_variable>
#include <cstdint>
#include <deque>
#include <functional>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

namespace dusk::mods::svc {

class HttpCore {
public:
    struct Header {
        std::string name;
        std::string value;
    };

    struct Request {
        HttpMethod method = HTTP_METHOD_GET;
        std::string url;
        std::vector<Header> headers;
        std::vector<uint8_t> body;
        uint32_t timeoutMs = 0;
        uint32_t maxResponseSize = 0;
    };

    struct Response {
        HttpResult result = HTTP_RESULT_UNAVAILABLE;
        uint32_t statusCode = 0;
        std::vector<Header> headers;
        std::vector<uint8_t> body;
    };

    using Executor = std::function<Response(const Request&, const std::atomic_bool& canceled)>;

    explicit HttpCore(Executor executor = {});
    ~HttpCore();

    HttpCore(const HttpCore&) = delete;
    HttpCore& operator=(const HttpCore&) = delete;

    ModResult begin(ModContext* context, const HttpRequestDesc* desc, HttpCompletionFn callback,
        void* userData, HttpRequestHandle* outHandle);
    ModResult cancel(ModContext* context, HttpRequestHandle handle);
    void frameBegin();
    void modDetached(ModContext* context);
    void shutdown();

private:
    enum class State {
        Queued,
        Running,
        CompletionQueued,
        Delivered,
    };

    struct Entry {
        ModContext* owner = nullptr;
        HttpRequestHandle handle{};
        Request request;
        HttpCompletionFn callback = nullptr;
        void* userData = nullptr;
        Response response;
        std::atomic_bool canceled = false;
        bool suppressed = false;
        State state = State::Queued;
    };

    static bool validateRequest(const HttpRequestDesc* desc, Request& request);
    static Response defaultExecutor(const Request&, const std::atomic_bool&);
    void workerMain();
    void queueCompletionLocked(const std::shared_ptr<Entry>& entry, Response response);

    Executor m_executor;
    std::mutex m_mutex;
    std::condition_variable m_workReady;
    std::deque<std::shared_ptr<Entry>> m_work;
    std::deque<std::shared_ptr<Entry>> m_completions;
    std::unordered_map<uint64_t, std::shared_ptr<Entry>> m_entries;
    std::thread m_worker;
    uint64_t m_nextHandle = 1;
    bool m_stopping = false;
};

}  // namespace dusk::mods::svc
