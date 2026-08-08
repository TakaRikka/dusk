#include <borealis/http.hpp>

#include <mutex>

namespace borealis::http::test {
std::mutex mutex;
Request lastRequest;
Result nextResult{.error = Error::NoBackend};
int calls = 0;

void reset(Result result) {
    std::lock_guard lock(mutex);
    lastRequest = {};
    nextResult = std::move(result);
    calls = 0;
}

Request captured() {
    std::lock_guard lock(mutex);
    return lastRequest;
}

int call_count() {
    std::lock_guard lock(mutex);
    return calls;
}
}  // namespace borealis::http::test

namespace borealis::http {
Result request(const Request& request) {
    std::lock_guard lock(test::mutex);
    test::lastRequest = request;
    ++test::calls;
    return test::nextResult;
}
}  // namespace borealis::http
