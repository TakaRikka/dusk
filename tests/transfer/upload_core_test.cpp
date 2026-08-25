#include "dusk/transfer/upload_core.hpp"

#include <cstdio>
#include <string>

using namespace dusk::transfer;

#define CHECK(cond) do { if (!(cond)) { std::fprintf(stderr, "FAIL %s:%d: %s\n", __FILE__, __LINE__, #cond); return 1; } } while (0)

namespace {
UploadState active_state() {
    UploadState s;
    s.id = derive_upload_id("tp.iso", 1000);
    s.name = "tp.iso";
    s.declaredSize = 1000;
    s.received = 400;
    s.active = true;
    return s;
}
}  // namespace

int main() {
    // Determinism is what makes resume work after the browser or the app restarts.
    CHECK(derive_upload_id("tp.iso", 1000) == derive_upload_id("tp.iso", 1000));
    CHECK(derive_upload_id("tp.iso", 1000) != derive_upload_id("tp.iso", 1001));
    CHECK(derive_upload_id("tp.iso", 1000) != derive_upload_id("other.iso", 1000));

    // The id is used as a filename, so it must never contain a path separator or a dot.
    {
        const std::string id = derive_upload_id("../../etc/passwd", 10);
        CHECK(!id.empty());
        CHECK(id.find('/') == std::string::npos);
        CHECK(id.find('.') == std::string::npos);
        for (const char c : id) {
            CHECK((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f'));
        }
    }

    const UploadState s = active_state();

    // The happy path: the chunk starts exactly where the staged file ends.
    CHECK(judge_chunk(s, s.id, 400, 100) == ChunkVerdict::Accept);

    // A replayed chunk (lost response, client retried) must be refused, not appended twice.
    CHECK(judge_chunk(s, s.id, 0, 400) == ChunkVerdict::OffsetMismatch);
    // A skipped chunk would leave a hole.
    CHECK(judge_chunk(s, s.id, 401, 100) == ChunkVerdict::OffsetMismatch);

    // Writing past the declared size means the client is lying about the file.
    CHECK(judge_chunk(s, s.id, 400, 601) == ChunkVerdict::Overrun);
    // Exactly filling the file is fine.
    CHECK(judge_chunk(s, s.id, 400, 600) == ChunkVerdict::Accept);

    // A chunk for a different upload must not land in this one's staging file.
    CHECK(judge_chunk(s, derive_upload_id("other.iso", 5), 400, 10) == ChunkVerdict::WrongUpload);

    // Nothing in progress.
    {
        UploadState idle;
        CHECK(judge_chunk(idle, "anything", 0, 10) == ChunkVerdict::Inactive);
    }

    // A zero-length chunk is accepted rather than treated as an error; it is a no-op append.
    CHECK(judge_chunk(s, s.id, 400, 0) == ChunkVerdict::Accept);

    // Staleness boundaries.
    CHECK(!is_stale(1000, 1000 + kStaleAfterSeconds - 1));
    CHECK(is_stale(1000, 1000 + kStaleAfterSeconds + 1));
    // A file stamped in the future must not be swept.
    CHECK(!is_stale(2000, 1000));

    // Sanitising: the ordinary case is untouched.
    CHECK(sanitize_publish_name("tp.iso", "deadbeef") == "tp.iso");
    // Directory components are stripped, not escaped.
    CHECK(sanitize_publish_name("../../etc/evil.iso", "deadbeef") == "evil.iso");
    CHECK(sanitize_publish_name("C:\\games\\tp.rvz", "deadbeef") == "tp.rvz");
    // Disallowed characters become underscores; the extension survives.
    CHECK(sanitize_publish_name("a;b|c.iso", "deadbeef") == "a_b_c.iso");
    // Leading dots would make the file hidden and could collide with .incoming.
    CHECK(sanitize_publish_name("...hidden.iso", "deadbeef") == "hidden.iso");
    // Nothing usable survives, so the id is used.
    CHECK(sanitize_publish_name("....", "deadbeef") == "deadbeef");
    CHECK(sanitize_publish_name("///", "deadbeef") == "deadbeef");
    // A very long stem is truncated but keeps its extension.
    {
        const std::string longName = std::string(300, 'a') + ".iso";
        const std::string out = sanitize_publish_name(longName, "deadbeef");
        CHECK(out.size() == 104);
        CHECK(out.substr(out.size() - 4) == ".iso");
    }

    std::puts("upload_core_test OK");
    return 0;
}
