#pragma once

// Pure, platform-independent logic for the tvOS save mirror.
//
// Everything in this header is plain C++ with no Objective-C, no SDL, no
// borealis and no tvOS dependency, so it can be compiled and unit-tested on the
// host (see tests/save_mirror). The Objective-C side (save_mirror.mm) owns only
// the NSUserDefaults access, the dispatch queue and the SDL event watch; every
// decision it makes -- which files are eligible, how the envelope is shaped,
// whether a mirror may be restored, what gets shed under the size guard -- lives
// here.

#include <cstdint>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace dusk::tvos::save_mirror {

// ---------------------------------------------------------------------------
// Constants
// ---------------------------------------------------------------------------

// NSUserDefaults key holding the whole envelope.
inline constexpr const char* kDefaultsKey = "tvos.save.mirror.v1";

// Envelope version. Bumped only for incompatible layout changes; an envelope
// with an unknown schema is ignored on restore rather than guessed at.
inline constexpr int kSchemaVersion = 1;

// Distinguishes this app's mirror from a sibling port sharing the design.
inline constexpr const char* kAppId = "dusklight";

// 400 KB. tvOS warns at 512 KB of NSUserDefaults and terminates the app at 1 MB;
// the mirror must never be the reason the app is killed.
inline constexpr std::uint64_t kSizeLimitBytes = 400u * 1024u;

// A GameCube memory card file is a fixed 0x40 header + 0x8000 of data.
inline constexpr std::uint64_t kCardFileBytes = 32832;

// ---------------------------------------------------------------------------
// SHA-256
// ---------------------------------------------------------------------------

// Lower-case hex digest of the given bytes.
std::string sha256_hex(const std::uint8_t* data, std::size_t size);
std::string sha256_hex(const std::vector<std::uint8_t>& data);

// ---------------------------------------------------------------------------
// File selection
// ---------------------------------------------------------------------------

enum class EntryClass {
    // A memory card save. Never shed by the size guard, never restored across
    // game ids, and its presence on disk is what suppresses a restore.
    Save,
    // Config-class state: useful to keep, but expendable under the size guard.
    Support,
    // Not mirrored at all: regenerable caches, logs, user-supplied content, or
    // anything unbounded.
    Excluded,
};

// Classifies a path relative to the data root, written with '/' separators
// (e.g. "EUR/Card A/01-GZ2P-gczelda2.gci", "config.json").
//
// The rule is an allowlist: anything not explicitly recognised is Excluded, so a
// new cache or log file added upstream can never silently start consuming the
// NSUserDefaults budget.
EntryClass classify_entry(std::string_view relativePath);

// True for exactly "<REGION>/Card A|Card B/<name>.gci" with REGION in
// {USA, EUR, JAP} -- the layout aurora's GCI-folder card backend writes.
bool is_card_save_path(std::string_view relativePath);

// ---------------------------------------------------------------------------
// Envelope
// ---------------------------------------------------------------------------

struct MirrorEntry {
    std::string name;              // path relative to the data root, '/' separated
    std::uint64_t size = 0;        // bytes; must equal data.size()
    std::string digest;            // sha256 hex of data
    std::vector<std::uint8_t> data;
};

struct MirrorEnvelope {
    int schema = kSchemaVersion;
    std::string app;
    std::string gameId;
    std::int64_t sequence = 0;
    std::int64_t timestamp = 0;
    std::vector<MirrorEntry> entries;
    std::uint64_t totalBytes = 0;
};

// Fills size + digest from data.
MirrorEntry make_entry(std::string name, std::vector<std::uint8_t> data);

// Fills size/digest for every entry and recomputes totalBytes.
MirrorEnvelope build_envelope(std::string app, std::string gameId, std::int64_t sequence,
    std::int64_t timestamp, std::vector<MirrorEntry> entries);

std::uint64_t total_bytes(const std::vector<MirrorEntry>& entries);

// ---------------------------------------------------------------------------
// Neutral property-list value
// ---------------------------------------------------------------------------

// A minimal plist-shaped tree. The envelope is encoded to this, and the .mm maps
// it mechanically onto NSDictionary/NSArray/NSNumber/NSString/NSData. Keeping the
// field names and the structure on this side means the wire format is covered by
// host tests instead of only by a device run.
struct Value {
    enum class Type { Null, Int, String, Bytes, Array, Dict };

    Type type = Type::Null;
    std::int64_t integer = 0;
    std::string string;
    std::vector<std::uint8_t> bytes;
    std::vector<Value> array;
    std::vector<std::pair<std::string, Value>> dict;  // insertion-ordered

    static Value of_int(std::int64_t v);
    static Value of_string(std::string v);
    static Value of_bytes(std::vector<std::uint8_t> v);
    static Value of_array(std::vector<Value> v);
    static Value of_dict(std::vector<std::pair<std::string, Value>> v);

    const Value* find(std::string_view key) const;
};

Value encode_envelope(const MirrorEnvelope& envelope);

enum class ParseStatus {
    Ok,
    NotADictionary,
    MissingField,
    WrongType,
    BadEntry,
};

struct ParseResult {
    ParseStatus status = ParseStatus::Ok;
    std::string detail;  // field name or entry index, for logging
    MirrorEnvelope envelope;
};

// Structural decode only: does not validate app/gameId/digests (see
// check_envelope), so a mirror that fails validation can still be logged in
// detail rather than dismissed as "unreadable".
ParseResult parse_envelope(const Value& value);

const char* to_string(ParseStatus status);

// ---------------------------------------------------------------------------
// Validation
// ---------------------------------------------------------------------------

enum class EnvelopeStatus {
    Ok,
    SchemaMismatch,
    AppMismatch,
    GameIdMismatch,
    NoEntries,
    SizeMismatch,    // an entry's recorded size disagrees with its payload
    DigestMismatch,  // an entry's payload does not hash to its recorded digest
    TotalMismatch,   // totalBytes disagrees with the sum of entry sizes
};

enum class GameIdPolicy {
    // Saves: a mirror from another game or region is never restored.
    Require,
    // Config-class files: not game state. Blocking them on a game-id mismatch
    // would leave the user with default settings after a purge for no safety
    // gain, so the id is logged but not enforced.
    Ignore,
};

EnvelopeStatus check_envelope(const MirrorEnvelope& envelope, std::string_view expectedApp,
    std::string_view expectedGameId, GameIdPolicy policy);

const char* to_string(EnvelopeStatus status);

// ---------------------------------------------------------------------------
// Size guard
// ---------------------------------------------------------------------------

enum class GuardStatus {
    Ok,            // fits as-is
    Shed,          // fits after dropping Support entries
    SaveTooLarge,  // the saves alone blow the budget; write nothing
};

struct GuardResult {
    GuardStatus status = GuardStatus::Ok;
    // Names dropped, in the order they were dropped. On SaveTooLarge these were
    // only *considered* for dropping -- `entries` is left untouched, because the
    // caller writes nothing at all in that case.
    std::vector<std::string> shed;
    std::uint64_t totalBytes = 0;  // after shedding
};

// Drops Support entries largest-first (ties broken by name, so the outcome is
// deterministic) until the total fits. Save entries are never dropped; if they
// alone exceed the limit the caller must skip the write entirely.
GuardResult apply_size_guard(std::vector<MirrorEntry>& entries, std::uint64_t limitBytes);

const char* to_string(GuardStatus status);

// ---------------------------------------------------------------------------
// Restore decision
// ---------------------------------------------------------------------------

enum class RestoreReason {
    Restore,
    PrimaryPresent,
    NoMirror,
    SchemaMismatch,
    AppMismatch,
    GameIdMismatch,
    NoEntries,
    SizeMismatch,
    DigestMismatch,
    TotalMismatch,
};

struct RestoreDecision {
    bool restore = false;
    RestoreReason reason = RestoreReason::NoMirror;
};

// The one place that decides whether the on-disk state may be touched.
// `primaryPresent` is checked first and short-circuits: a present save is never
// overwritten, however good the mirror looks.
RestoreDecision decide_restore(bool primaryPresent, const MirrorEnvelope* envelope,
    std::string_view expectedApp, std::string_view expectedGameId, GameIdPolicy policy);

const char* to_string(RestoreReason reason);

}  // namespace dusk::tvos::save_mirror
