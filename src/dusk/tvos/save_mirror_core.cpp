#include "dusk/tvos/save_mirror_core.hpp"

#include <algorithm>
#include <array>
#include <cstring>

namespace dusk::tvos::save_mirror {
namespace {

// ---------------------------------------------------------------------------
// SHA-256 (FIPS 180-4). Self-contained so the pure layer pulls in no crypto
// dependency and stays buildable on the host test target.
// ---------------------------------------------------------------------------

constexpr std::uint32_t kK[64] = {
    0x428a2f98u, 0x71374491u, 0xb5c0fbcfu, 0xe9b5dba5u, 0x3956c25bu, 0x59f111f1u, 0x923f82a4u,
    0xab1c5ed5u, 0xd807aa98u, 0x12835b01u, 0x243185beu, 0x550c7dc3u, 0x72be5d74u, 0x80deb1feu,
    0x9bdc06a7u, 0xc19bf174u, 0xe49b69c1u, 0xefbe4786u, 0x0fc19dc6u, 0x240ca1ccu, 0x2de92c6fu,
    0x4a7484aau, 0x5cb0a9dcu, 0x76f988dau, 0x983e5152u, 0xa831c66du, 0xb00327c8u, 0xbf597fc7u,
    0xc6e00bf3u, 0xd5a79147u, 0x06ca6351u, 0x14292967u, 0x27b70a85u, 0x2e1b2138u, 0x4d2c6dfcu,
    0x53380d13u, 0x650a7354u, 0x766a0abbu, 0x81c2c92eu, 0x92722c85u, 0xa2bfe8a1u, 0xa81a664bu,
    0xc24b8b70u, 0xc76c51a3u, 0xd192e819u, 0xd6990624u, 0xf40e3585u, 0x106aa070u, 0x19a4c116u,
    0x1e376c08u, 0x2748774cu, 0x34b0bcb5u, 0x391c0cb3u, 0x4ed8aa4au, 0x5b9cca4fu, 0x682e6ff3u,
    0x748f82eeu, 0x78a5636fu, 0x84c87814u, 0x8cc70208u, 0x90befffau, 0xa4506cebu, 0xbef9a3f7u,
    0xc67178f2u,
};

constexpr std::uint32_t rotr(std::uint32_t v, int n) {
    return (v >> n) | (v << (32 - n));
}

void sha256_block(std::uint32_t state[8], const std::uint8_t* block) {
    std::uint32_t w[64];
    for (int i = 0; i < 16; ++i) {
        w[i] = (static_cast<std::uint32_t>(block[i * 4]) << 24) |
               (static_cast<std::uint32_t>(block[i * 4 + 1]) << 16) |
               (static_cast<std::uint32_t>(block[i * 4 + 2]) << 8) |
               static_cast<std::uint32_t>(block[i * 4 + 3]);
    }
    for (int i = 16; i < 64; ++i) {
        const std::uint32_t s0 = rotr(w[i - 15], 7) ^ rotr(w[i - 15], 18) ^ (w[i - 15] >> 3);
        const std::uint32_t s1 = rotr(w[i - 2], 17) ^ rotr(w[i - 2], 19) ^ (w[i - 2] >> 10);
        w[i] = w[i - 16] + s0 + w[i - 7] + s1;
    }

    std::uint32_t a = state[0], b = state[1], c = state[2], d = state[3];
    std::uint32_t e = state[4], f = state[5], g = state[6], h = state[7];

    for (int i = 0; i < 64; ++i) {
        const std::uint32_t S1 = rotr(e, 6) ^ rotr(e, 11) ^ rotr(e, 25);
        const std::uint32_t ch = (e & f) ^ (~e & g);
        const std::uint32_t temp1 = h + S1 + ch + kK[i] + w[i];
        const std::uint32_t S0 = rotr(a, 2) ^ rotr(a, 13) ^ rotr(a, 22);
        const std::uint32_t maj = (a & b) ^ (a & c) ^ (b & c);
        const std::uint32_t temp2 = S0 + maj;

        h = g;
        g = f;
        f = e;
        e = d + temp1;
        d = c;
        c = b;
        b = a;
        a = temp1 + temp2;
    }

    state[0] += a;
    state[1] += b;
    state[2] += c;
    state[3] += d;
    state[4] += e;
    state[5] += f;
    state[6] += g;
    state[7] += h;
}

std::vector<std::string> split_path(std::string_view path) {
    std::vector<std::string> parts;
    std::size_t start = 0;
    while (start <= path.size()) {
        const std::size_t slash = path.find('/', start);
        if (slash == std::string_view::npos) {
            parts.emplace_back(path.substr(start));
            break;
        }
        parts.emplace_back(path.substr(start, slash - start));
        start = slash + 1;
    }
    return parts;
}

bool ends_with(std::string_view value, std::string_view suffix) {
    return value.size() >= suffix.size() &&
           value.compare(value.size() - suffix.size(), suffix.size(), suffix) == 0;
}

template <typename Range, typename T>
bool contains(const Range& range, const T& value) {
    return std::find(std::begin(range), std::end(range), value) != std::end(range);
}

}  // namespace

std::string sha256_hex(const std::uint8_t* data, std::size_t size) {
    std::uint32_t state[8] = {
        0x6a09e667u,
        0xbb67ae85u,
        0x3c6ef372u,
        0xa54ff53au,
        0x510e527fu,
        0x9b05688cu,
        0x1f83d9abu,
        0x5be0cd19u,
    };

    std::size_t offset = 0;
    for (; offset + 64 <= size; offset += 64) {
        sha256_block(state, data + offset);
    }

    std::array<std::uint8_t, 128> tail{};
    const std::size_t remaining = size - offset;
    if (remaining > 0) {
        std::memcpy(tail.data(), data + offset, remaining);
    }
    tail[remaining] = 0x80;
    const std::size_t tailLength = (remaining < 56) ? 64 : 128;
    const std::uint64_t bitLength = static_cast<std::uint64_t>(size) * 8;
    for (int i = 0; i < 8; ++i) {
        tail[tailLength - 1 - static_cast<std::size_t>(i)] =
            static_cast<std::uint8_t>((bitLength >> (8 * i)) & 0xff);
    }
    for (std::size_t i = 0; i < tailLength; i += 64) {
        sha256_block(state, tail.data() + i);
    }

    static constexpr char kHex[] = "0123456789abcdef";
    std::string out;
    out.reserve(64);
    for (const std::uint32_t word : state) {
        for (int i = 3; i >= 0; --i) {
            const auto byte = static_cast<std::uint8_t>((word >> (8 * i)) & 0xff);
            out.push_back(kHex[byte >> 4]);
            out.push_back(kHex[byte & 0x0f]);
        }
    }
    return out;
}

std::string sha256_hex(const std::vector<std::uint8_t>& data) {
    return sha256_hex(data.data(), data.size());
}

// ---------------------------------------------------------------------------
// File selection
// ---------------------------------------------------------------------------

bool is_card_save_path(std::string_view relativePath) {
    const auto parts = split_path(relativePath);
    if (parts.size() != 3) {
        return false;
    }
    if (!contains(kRegionDirs, parts[0]) || !contains(kCardDirs, parts[1])) {
        return false;
    }
    return parts[2].size() > 4 && ends_with(parts[2], ".gci");
}

EntryClass classify_entry(std::string_view relativePath) {
    if (relativePath.empty()) {
        return EntryClass::Excluded;
    }
    if (is_card_save_path(relativePath)) {
        return EntryClass::Save;
    }

    // Support files live at the top level of the data root only. Anything nested
    // is either a cache, a log, or user-supplied content.
    const auto parts = split_path(relativePath);
    if (parts.size() != 1) {
        return EntryClass::Excluded;
    }

    const std::string& name = parts[0];
    if (contains(kSupportFileNames, name)) {
        return EntryClass::Support;
    }
    // "> size" and not ">=": a file called exactly ".controller" is a dotfile,
    // not a pad mapping.
    if (name.size() > kControllerSuffix.size() && ends_with(name, kControllerSuffix)) {
        return EntryClass::Support;
    }
    return EntryClass::Excluded;
}

// ---------------------------------------------------------------------------
// Envelope
// ---------------------------------------------------------------------------

MirrorEntry make_entry(std::string name, std::vector<std::uint8_t> data) {
    MirrorEntry entry;
    entry.name = std::move(name);
    entry.data = std::move(data);
    entry.size = entry.data.size();
    entry.digest = sha256_hex(entry.data);
    return entry;
}

std::uint64_t total_bytes(const std::vector<MirrorEntry>& entries) {
    std::uint64_t total = 0;
    for (const auto& entry : entries) {
        total += entry.size;
    }
    return total;
}

MirrorEnvelope build_envelope(std::string app, std::string gameId, std::int64_t sequence,
    std::int64_t timestamp, std::vector<MirrorEntry> entries) {
    MirrorEnvelope envelope;
    envelope.schema = kSchemaVersion;
    envelope.app = std::move(app);
    envelope.gameId = std::move(gameId);
    envelope.sequence = sequence;
    envelope.timestamp = timestamp;
    envelope.entries = std::move(entries);
    for (auto& entry : envelope.entries) {
        entry.size = entry.data.size();
        entry.digest = sha256_hex(entry.data);
    }
    envelope.totalBytes = total_bytes(envelope.entries);
    return envelope;
}

// ---------------------------------------------------------------------------
// Neutral property-list value
// ---------------------------------------------------------------------------

Value Value::of_int(std::int64_t v) {
    Value value;
    value.type = Type::Int;
    value.integer = v;
    return value;
}

Value Value::of_string(std::string v) {
    Value value;
    value.type = Type::String;
    value.string = std::move(v);
    return value;
}

Value Value::of_bytes(std::vector<std::uint8_t> v) {
    Value value;
    value.type = Type::Bytes;
    value.bytes = std::move(v);
    return value;
}

Value Value::of_array(std::vector<Value> v) {
    Value value;
    value.type = Type::Array;
    value.array = std::move(v);
    return value;
}

Value Value::of_dict(std::vector<std::pair<std::string, Value>> v) {
    Value value;
    value.type = Type::Dict;
    value.dict = std::move(v);
    return value;
}

const Value* Value::find(std::string_view key) const {
    if (type != Type::Dict) {
        return nullptr;
    }
    for (const auto& [name, value] : dict) {
        if (name == key) {
            return &value;
        }
    }
    return nullptr;
}

Value encode_envelope(const MirrorEnvelope& envelope) {
    std::vector<Value> entries;
    entries.reserve(envelope.entries.size());
    for (const auto& entry : envelope.entries) {
        entries.push_back(Value::of_dict({
            {"name", Value::of_string(entry.name)},
            {"size", Value::of_int(static_cast<std::int64_t>(entry.size))},
            {"digest", Value::of_string(entry.digest)},
            {"data", Value::of_bytes(entry.data)},
        }));
    }

    return Value::of_dict({
        {"schema", Value::of_int(envelope.schema)},
        {"app", Value::of_string(envelope.app)},
        {"gameId", Value::of_string(envelope.gameId)},
        {"sequence", Value::of_int(envelope.sequence)},
        {"timestamp", Value::of_int(envelope.timestamp)},
        {"entries", Value::of_array(std::move(entries))},
        {"totalBytes", Value::of_int(static_cast<std::int64_t>(envelope.totalBytes))},
    });
}

namespace {

const Value* require(const Value& dict, std::string_view key, Value::Type type,
    ParseResult& result) {
    const Value* found = dict.find(key);
    if (found == nullptr) {
        result.status = ParseStatus::MissingField;
        result.detail = std::string(key);
        return nullptr;
    }
    if (found->type != type) {
        result.status = ParseStatus::WrongType;
        result.detail = std::string(key);
        return nullptr;
    }
    return found;
}

}  // namespace

ParseResult parse_envelope(const Value& value) {
    ParseResult result;
    if (value.type != Value::Type::Dict) {
        result.status = ParseStatus::NotADictionary;
        return result;
    }

    const Value* schema = require(value, "schema", Value::Type::Int, result);
    if (schema == nullptr) {
        return result;
    }
    const Value* app = require(value, "app", Value::Type::String, result);
    if (app == nullptr) {
        return result;
    }
    const Value* gameId = require(value, "gameId", Value::Type::String, result);
    if (gameId == nullptr) {
        return result;
    }
    const Value* sequence = require(value, "sequence", Value::Type::Int, result);
    if (sequence == nullptr) {
        return result;
    }
    const Value* timestamp = require(value, "timestamp", Value::Type::Int, result);
    if (timestamp == nullptr) {
        return result;
    }
    const Value* entries = require(value, "entries", Value::Type::Array, result);
    if (entries == nullptr) {
        return result;
    }
    const Value* totalBytes = require(value, "totalBytes", Value::Type::Int, result);
    if (totalBytes == nullptr) {
        return result;
    }

    result.envelope.schema = static_cast<int>(schema->integer);
    result.envelope.app = app->string;
    result.envelope.gameId = gameId->string;
    result.envelope.sequence = sequence->integer;
    result.envelope.timestamp = timestamp->integer;
    result.envelope.totalBytes =
        totalBytes->integer < 0 ? 0 : static_cast<std::uint64_t>(totalBytes->integer);

    result.envelope.entries.reserve(entries->array.size());
    for (std::size_t i = 0; i < entries->array.size(); ++i) {
        const Value& raw = entries->array[i];
        const Value* name = raw.find("name");
        const Value* size = raw.find("size");
        const Value* digest = raw.find("digest");
        const Value* data = raw.find("data");
        if (raw.type != Value::Type::Dict || name == nullptr ||
            name->type != Value::Type::String || size == nullptr ||
            size->type != Value::Type::Int || digest == nullptr ||
            digest->type != Value::Type::String || data == nullptr ||
            data->type != Value::Type::Bytes)
        {
            result.status = ParseStatus::BadEntry;
            result.detail = "entries[" + std::to_string(i) + "]";
            result.envelope.entries.clear();
            return result;
        }

        MirrorEntry entry;
        entry.name = name->string;
        entry.size = size->integer < 0 ? 0 : static_cast<std::uint64_t>(size->integer);
        entry.digest = digest->string;
        entry.data = data->bytes;
        result.envelope.entries.push_back(std::move(entry));
    }

    return result;
}

const char* to_string(ParseStatus status) {
    switch (status) {
    case ParseStatus::Ok:
        return "ok";
    case ParseStatus::NotADictionary:
        return "not-a-dictionary";
    case ParseStatus::MissingField:
        return "missing-field";
    case ParseStatus::WrongType:
        return "wrong-type";
    case ParseStatus::BadEntry:
        return "bad-entry";
    }
    return "unknown";
}

// ---------------------------------------------------------------------------
// Validation
// ---------------------------------------------------------------------------

namespace {

bool in_scope(std::string_view name, EntryScope scope) {
    switch (scope) {
    case EntryScope::All:
        return true;
    case EntryScope::SavesOnly:
        return classify_entry(name) == EntryClass::Save;
    case EntryScope::SupportOnly:
        return classify_entry(name) == EntryClass::Support;
    }
    return false;
}

}  // namespace

EnvelopeStatus check_envelope(const MirrorEnvelope& envelope, std::string_view expectedApp,
    std::string_view expectedGameId, GameIdPolicy policy, EntryScope scope) {
    if (envelope.schema != kSchemaVersion) {
        return EnvelopeStatus::SchemaMismatch;
    }
    if (envelope.app != expectedApp) {
        return EnvelopeStatus::AppMismatch;
    }
    if (policy == GameIdPolicy::Require && envelope.gameId != expectedGameId) {
        return EnvelopeStatus::GameIdMismatch;
    }

    std::size_t inScope = 0;
    for (const auto& entry : envelope.entries) {
        if (!in_scope(entry.name, scope)) {
            continue;
        }
        ++inScope;
        if (entry.size != entry.data.size()) {
            return EnvelopeStatus::SizeMismatch;
        }
        if (sha256_hex(entry.data) != entry.digest) {
            return EnvelopeStatus::DigestMismatch;
        }
    }
    if (inScope == 0) {
        return EnvelopeStatus::NoEntries;
    }

    // totalBytes is a cross-check over the whole entry array. Under a narrowed
    // scope the entries it sums are deliberately not all validated, so it can
    // only report a disagreement this pass is not responsible for -- and letting
    // it fail here would put a corrupt config entry between the player and a
    // perfectly good save.
    if (scope == EntryScope::All && envelope.totalBytes != total_bytes(envelope.entries)) {
        return EnvelopeStatus::TotalMismatch;
    }
    return EnvelopeStatus::Ok;
}

const char* to_string(EnvelopeStatus status) {
    switch (status) {
    case EnvelopeStatus::Ok:
        return "ok";
    case EnvelopeStatus::SchemaMismatch:
        return "schema-mismatch";
    case EnvelopeStatus::AppMismatch:
        return "app-mismatch";
    case EnvelopeStatus::GameIdMismatch:
        return "game-id-mismatch";
    case EnvelopeStatus::NoEntries:
        return "no-entries";
    case EnvelopeStatus::SizeMismatch:
        return "size-mismatch";
    case EnvelopeStatus::DigestMismatch:
        return "digest-mismatch";
    case EnvelopeStatus::TotalMismatch:
        return "total-mismatch";
    }
    return "unknown";
}

// ---------------------------------------------------------------------------
// Size guard
// ---------------------------------------------------------------------------

GuardResult apply_size_guard(std::vector<MirrorEntry>& entries, std::uint64_t limitBytes) {
    GuardResult result;
    result.totalBytes = total_bytes(entries);
    if (result.totalBytes <= limitBytes) {
        result.status = GuardStatus::Ok;
        return result;
    }

    // Shed order: Support entries only, largest first, name-ascending on ties so
    // two runs with the same inputs always drop the same files.
    std::vector<std::size_t> sheddable;
    for (std::size_t i = 0; i < entries.size(); ++i) {
        if (classify_entry(entries[i].name) != EntryClass::Save) {
            sheddable.push_back(i);
        }
    }
    std::sort(sheddable.begin(), sheddable.end(), [&entries](std::size_t a, std::size_t b) {
        if (entries[a].size != entries[b].size) {
            return entries[a].size > entries[b].size;
        }
        return entries[a].name < entries[b].name;
    });

    std::vector<bool> dropped(entries.size(), false);
    for (const std::size_t index : sheddable) {
        if (result.totalBytes <= limitBytes) {
            break;
        }
        dropped[index] = true;
        result.shed.push_back(entries[index].name);
        result.totalBytes -= entries[index].size;
    }

    if (result.totalBytes > limitBytes) {
        // Only Save entries remain and they still do not fit: the caller must
        // write nothing rather than store a mirror that cannot hold the save.
        result.status = GuardStatus::SaveTooLarge;
        return result;
    }

    std::vector<MirrorEntry> kept;
    kept.reserve(entries.size() - result.shed.size());
    for (std::size_t i = 0; i < entries.size(); ++i) {
        if (!dropped[i]) {
            kept.push_back(std::move(entries[i]));
        }
    }
    entries = std::move(kept);
    result.status = result.shed.empty() ? GuardStatus::Ok : GuardStatus::Shed;
    return result;
}

const char* to_string(GuardStatus status) {
    switch (status) {
    case GuardStatus::Ok:
        return "ok";
    case GuardStatus::Shed:
        return "shed";
    case GuardStatus::SaveTooLarge:
        return "save-too-large";
    }
    return "unknown";
}

// ---------------------------------------------------------------------------
// Restore decision
// ---------------------------------------------------------------------------

RestoreDecision decide_restore(bool primaryPresent, const MirrorEnvelope* envelope,
    std::string_view expectedApp, std::string_view expectedGameId, GameIdPolicy policy,
    EntryScope scope) {
    RestoreDecision decision;

    // Checked before anything else: a save that is already on disk is never
    // overwritten, no matter how good the mirror looks.
    if (primaryPresent) {
        decision.reason = RestoreReason::PrimaryPresent;
        return decision;
    }
    if (envelope == nullptr) {
        decision.reason = RestoreReason::NoMirror;
        return decision;
    }

    switch (check_envelope(*envelope, expectedApp, expectedGameId, policy, scope)) {
    case EnvelopeStatus::Ok:
        decision.restore = true;
        decision.reason = RestoreReason::Restore;
        break;
    case EnvelopeStatus::SchemaMismatch:
        decision.reason = RestoreReason::SchemaMismatch;
        break;
    case EnvelopeStatus::AppMismatch:
        decision.reason = RestoreReason::AppMismatch;
        break;
    case EnvelopeStatus::GameIdMismatch:
        decision.reason = RestoreReason::GameIdMismatch;
        break;
    case EnvelopeStatus::NoEntries:
        decision.reason = RestoreReason::NoEntries;
        break;
    case EnvelopeStatus::SizeMismatch:
        decision.reason = RestoreReason::SizeMismatch;
        break;
    case EnvelopeStatus::DigestMismatch:
        decision.reason = RestoreReason::DigestMismatch;
        break;
    case EnvelopeStatus::TotalMismatch:
        decision.reason = RestoreReason::TotalMismatch;
        break;
    }
    return decision;
}

RestorePlan plan_restore(const MirrorEnvelope& envelope, EntryClass entryClass,
    const std::vector<std::string>& presentNames) {
    RestorePlan plan;
    for (const auto& entry : envelope.entries) {
        if (classify_entry(entry.name) != entryClass) {
            continue;
        }
        if (contains(presentNames, entry.name)) {
            plan.present.push_back(entry.name);
        } else {
            plan.missing.push_back(entry.name);
        }
    }
    // "Nothing to do" needs at least one entry to have been already there. With
    // no entries of this class at all the caller must still validate and report,
    // not claim the save was present.
    plan.primaryPresent = plan.missing.empty() && !plan.present.empty();
    return plan;
}

// ---------------------------------------------------------------------------
// Flush plan
// ---------------------------------------------------------------------------

FlushPlan plan_flush(const MirrorEnvelope* stored, const std::vector<std::string>& freshSaveNames,
    std::string_view liveGameId, std::string_view expectedApp) {
    FlushPlan plan;
    plan.gameId = std::string(liveGameId);

    if (stored == nullptr) {
        if (plan.gameId.empty()) {
            plan.gameId = kUnknownGameId;
        }
        return plan;
    }

    plan.sequence = stored->sequence + 1;

    // Identity gate, applied to the stored mirror as a whole. A mirror may only
    // hand its saves to the next envelope when it is unambiguously about this
    // app, this envelope version and this game -- where "this game" includes the
    // case where the disc is not open yet and the live id is simply not known.
    CarryForward identity = CarryForward::Carried;
    if (stored->schema != kSchemaVersion) {
        identity = CarryForward::ForeignSchema;
    } else if (stored->app != expectedApp) {
        identity = CarryForward::ForeignApp;
    } else if (!liveGameId.empty() && !stored->gameId.empty() && stored->gameId != liveGameId) {
        identity = CarryForward::ForeignGame;
    }

    // The new envelope inherits the stored id only when the live one is unknown,
    // and only from a mirror that passed the gate. Stamping this session's id
    // over another game's save would make it restorable for the wrong game --
    // exactly what §3.1 says must be impossible -- so an entry that cannot keep
    // its own id is dropped instead of relabelled.
    if (plan.gameId.empty()) {
        plan.gameId = (identity == CarryForward::Carried && !stored->gameId.empty())
                          ? stored->gameId
                          : std::string(kUnknownGameId);
    }

    for (const auto& entry : stored->entries) {
        // Support entries are re-read from disk on every flush, so there is
        // nothing to carry: they are not state the mirror is the last copy of.
        if (classify_entry(entry.name) != EntryClass::Save) {
            continue;
        }

        CarriedEntry decision{entry.name, identity};
        if (identity == CarryForward::Carried) {
            if (contains(freshSaveNames, entry.name)) {
                // Its file is on disk and was captured this flush; the fresh
                // bytes win. Decided per entry, so a card that lost one file and
                // kept another carries forward exactly the one that is gone.
                decision.status = CarryForward::Superseded;
            } else if (entry.size != entry.data.size() ||
                       sha256_hex(entry.data) != entry.digest)
            {
                decision.status = CarryForward::Corrupt;
            }
        }

        switch (decision.status) {
        case CarryForward::Carried:
            plan.carried.push_back(entry);
            break;
        case CarryForward::ForeignApp:
        case CarryForward::ForeignGame:
        case CarryForward::ForeignSchema:
            plan.droppedForeignSave = true;
            break;
        case CarryForward::Superseded:
        case CarryForward::Corrupt:
            break;
        }
        plan.considered.push_back(std::move(decision));
    }

    // Refusing to adopt a save and then overwriting the mirror that holds it
    // destroys the only copy. When this session has a save of its own to
    // protect, that trade is worth making and is logged; when it has none, the
    // flush would be spending someone else's save on a config update, so the
    // stored mirror is left alone instead.
    plan.mayReplaceStored = !(plan.droppedForeignSave && freshSaveNames.empty());
    return plan;
}

const char* to_string(CarryForward status) {
    switch (status) {
    case CarryForward::Carried:
        return "carried-forward";
    case CarryForward::Superseded:
        return "superseded-by-disk";
    case CarryForward::Corrupt:
        return "corrupt";
    case CarryForward::ForeignApp:
        return "foreign-app";
    case CarryForward::ForeignGame:
        return "foreign-game";
    case CarryForward::ForeignSchema:
        return "foreign-schema";
    }
    return "unknown";
}

const char* to_string(RestoreReason reason) {
    switch (reason) {
    case RestoreReason::Restore:
        return "restore";
    case RestoreReason::PrimaryPresent:
        return "primary-present";
    case RestoreReason::NoMirror:
        return "no-mirror";
    case RestoreReason::SchemaMismatch:
        return "schema-mismatch";
    case RestoreReason::AppMismatch:
        return "app-mismatch";
    case RestoreReason::GameIdMismatch:
        return "game-id-mismatch";
    case RestoreReason::NoEntries:
        return "no-entries";
    case RestoreReason::SizeMismatch:
        return "size-mismatch";
    case RestoreReason::DigestMismatch:
        return "digest-mismatch";
    case RestoreReason::TotalMismatch:
        return "total-mismatch";
    }
    return "unknown";
}

}  // namespace dusk::tvos::save_mirror
