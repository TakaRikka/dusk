#include "dusk/tvos/save_mirror_core.hpp"

#include <cstdio>
#include <string>
#include <vector>

using namespace dusk::tvos::save_mirror;

#define CHECK(cond) do { if (!(cond)) { std::fprintf(stderr, "FAIL %s:%d: %s\n", __FILE__, __LINE__, #cond); return 1; } } while (0)

namespace {

std::vector<std::uint8_t> filled(std::size_t size, std::uint8_t byte) {
    return std::vector<std::uint8_t>(size, byte);
}

constexpr const char* kSavePath = "EUR/Card A/01-GZ2P-gczelda2.gci";
constexpr const char* kSavePathB = "EUR/Card B/01-GZ2P-gczelda2.gci";
constexpr const char* kGameId = "GZ2P01";

MirrorEnvelope sample_envelope() {
    std::vector<MirrorEntry> entries;
    entries.push_back(make_entry(kSavePath, filled(kCardFileBytes, 0xA5)));
    entries.push_back(make_entry("config.json", {'{', '}', '\n'}));
    return build_envelope(kAppId, kGameId, 7, 1766000000, std::move(entries));
}

// Two cards plus a config file. The flush routinely produces this shape -- both
// card slots are scanned -- and it is the only shape that can catch validation
// that stops after the first entry.
MirrorEnvelope multi_save_envelope() {
    std::vector<MirrorEntry> entries;
    entries.push_back(make_entry(kSavePath, filled(kCardFileBytes, 0xA5)));
    entries.push_back(make_entry(kSavePathB, filled(kCardFileBytes, 0x5A)));
    entries.push_back(make_entry("config.json", {'{', '}', '\n'}));
    return build_envelope(kAppId, kGameId, 9, 1766000001, std::move(entries));
}

}  // namespace

int main() {
    // ---------------------------------------------------------------- sha256
    // Known-answer tests: without these a self-written digest could be
    // self-consistently wrong and every "digest matches" check would pass.
    CHECK(sha256_hex(nullptr, 0) ==
        "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855");
    {
        const std::vector<std::uint8_t> abc{'a', 'b', 'c'};
        CHECK(sha256_hex(abc) ==
            "ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad");
    }
    {
        // 448 bits: exercises the two-block padding path.
        const std::string message =
            "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq";
        const std::vector<std::uint8_t> bytes(message.begin(), message.end());
        CHECK(sha256_hex(bytes) ==
            "248d6a61d20638b8e5c026930c3e6039a33ce45964ff2167f6ecedd419db06c1");
    }
    {
        // One byte short of a block boundary and exactly one block.
        CHECK(sha256_hex(filled(63, 0x00)) ==
            "c7723fa1e0127975e49e62e753db53924c1bd84b8ac1ac08df78d09270f3d971");
        CHECK(sha256_hex(filled(64, 0x00)) ==
            "f5a5fd42d16a20302798ef6ed309979b43003d2320d9f0e8ea9831a92759fb4b");
    }

    // ------------------------------------------------------- classification
    CHECK(classify_entry(kSavePath) == EntryClass::Save);
    CHECK(classify_entry("USA/Card B/01-GZ2E-gczelda2.gci") == EntryClass::Save);
    CHECK(classify_entry("JAP/Card A/x.gci") == EntryClass::Save);
    CHECK(classify_entry("config.json") == EntryClass::Support);
    CHECK(classify_entry("achievements.json") == EntryClass::Support);
    CHECK(classify_entry("mod_saves.json") == EntryClass::Support);
    CHECK(classify_entry("controller_ports.dat") == EntryClass::Support);
    CHECK(classify_entry("keyboard_bindings.dat") == EntryClass::Support);
    CHECK(classify_entry("Xbox One.controller") == EntryClass::Support);

    // Every "never mirror" name from the design spec must be Excluded.
    for (const char* denied : {"dawn_cache.db", "dawn_cache.db-wal", "pipeline_cache.db",
             "pipeline_cache.db-shm", "imgui.ini", "gamecontrollerdb.txt", "logs/dusk.log",
             "sentry/db", "mods/foo.zip", "mod_data/foo/save.bin",
             "texture_replacements/pack/tex.png"})
    {
        CHECK(classify_entry(denied) == EntryClass::Excluded);
    }
    // Look-alikes that must not slip through the allowlist.
    CHECK(classify_entry("EUR/Card A/save.gci.bak") == EntryClass::Excluded);
    CHECK(classify_entry("KOR/Card A/x.gci") == EntryClass::Excluded);
    CHECK(classify_entry("EUR/Card C/x.gci") == EntryClass::Excluded);
    CHECK(classify_entry("EUR/x.gci") == EntryClass::Excluded);
    CHECK(classify_entry("nested/config.json") == EntryClass::Excluded);
    CHECK(classify_entry(".controller") == EntryClass::Excluded);
    CHECK(classify_entry("") == EntryClass::Excluded);

    // --------------------------------------------------- envelope roundtrip
    {
        const MirrorEnvelope original = sample_envelope();
        CHECK(original.totalBytes == kCardFileBytes + 3);
        CHECK(original.entries[0].size == kCardFileBytes);
        CHECK(original.entries[0].digest.size() == 64);

        const ParseResult parsed = parse_envelope(encode_envelope(original));
        CHECK(parsed.status == ParseStatus::Ok);
        const MirrorEnvelope& roundTripped = parsed.envelope;
        CHECK(roundTripped.schema == original.schema);
        CHECK(roundTripped.app == original.app);
        CHECK(roundTripped.gameId == original.gameId);
        CHECK(roundTripped.sequence == original.sequence);
        CHECK(roundTripped.timestamp == original.timestamp);
        CHECK(roundTripped.totalBytes == original.totalBytes);
        CHECK(roundTripped.entries.size() == original.entries.size());
        for (std::size_t i = 0; i < original.entries.size(); ++i) {
            CHECK(roundTripped.entries[i].name == original.entries[i].name);
            CHECK(roundTripped.entries[i].size == original.entries[i].size);
            CHECK(roundTripped.entries[i].digest == original.entries[i].digest);
            CHECK(roundTripped.entries[i].data == original.entries[i].data);
        }
        CHECK(check_envelope(roundTripped, kAppId, kGameId, GameIdPolicy::Require) ==
            EnvelopeStatus::Ok);
    }

    // ------------------------------------------------------ parse rejection
    {
        Value encoded = encode_envelope(sample_envelope());
        // Drop "gameId".
        for (auto it = encoded.dict.begin(); it != encoded.dict.end(); ++it) {
            if (it->first == "gameId") {
                encoded.dict.erase(it);
                break;
            }
        }
        const ParseResult parsed = parse_envelope(encoded);
        CHECK(parsed.status == ParseStatus::MissingField);
        CHECK(parsed.detail == "gameId");
    }
    {
        Value encoded = encode_envelope(sample_envelope());
        for (auto& [key, value] : encoded.dict) {
            if (key == "entries") {
                value.array[1].dict[3].second = Value::of_string("not-bytes");
            }
        }
        const ParseResult parsed = parse_envelope(encoded);
        CHECK(parsed.status == ParseStatus::BadEntry);
        CHECK(parsed.detail == "entries[1]");
    }
    CHECK(parse_envelope(Value::of_int(3)).status == ParseStatus::NotADictionary);

    // ------------------------------------------------------- validation
    {
        MirrorEnvelope tampered = sample_envelope();
        tampered.entries[0].data[0] ^= 0xff;  // digest no longer matches
        CHECK(check_envelope(tampered, kAppId, kGameId, GameIdPolicy::Require) ==
            EnvelopeStatus::DigestMismatch);
    }
    {
        MirrorEnvelope truncated = sample_envelope();
        truncated.entries[0].data.pop_back();  // recorded size no longer matches
        CHECK(check_envelope(truncated, kAppId, kGameId, GameIdPolicy::Require) ==
            EnvelopeStatus::SizeMismatch);
    }
    {
        MirrorEnvelope wrongGame = sample_envelope();
        wrongGame.gameId = "GZ2E01";
        CHECK(check_envelope(wrongGame, kAppId, kGameId, GameIdPolicy::Require) ==
            EnvelopeStatus::GameIdMismatch);
        // Config-class restores deliberately ignore the game id.
        CHECK(check_envelope(wrongGame, kAppId, kGameId, GameIdPolicy::Ignore) ==
            EnvelopeStatus::Ok);
    }
    {
        MirrorEnvelope wrongApp = sample_envelope();
        wrongApp.app = "banjo";
        CHECK(check_envelope(wrongApp, kAppId, kGameId, GameIdPolicy::Require) ==
            EnvelopeStatus::AppMismatch);
    }
    {
        MirrorEnvelope wrongSchema = sample_envelope();
        wrongSchema.schema = kSchemaVersion + 1;
        CHECK(check_envelope(wrongSchema, kAppId, kGameId, GameIdPolicy::Require) ==
            EnvelopeStatus::SchemaMismatch);
    }
    {
        MirrorEnvelope wrongTotal = sample_envelope();
        wrongTotal.totalBytes += 1;
        CHECK(check_envelope(wrongTotal, kAppId, kGameId, GameIdPolicy::Require) ==
            EnvelopeStatus::TotalMismatch);
    }
    {
        MirrorEnvelope empty = build_envelope(kAppId, kGameId, 1, 1, {});
        CHECK(check_envelope(empty, kAppId, kGameId, GameIdPolicy::Require) ==
            EnvelopeStatus::NoEntries);
    }

    // ------------------------------------- validation beyond the first entry
    // Everything above tampers with entries[0]. A "validate only the first
    // entry" bug would pass every one of those assertions, so each rejection is
    // re-proven on an entry that is *not* first -- including the last one.
    {
        const MirrorEnvelope good = multi_save_envelope();
        CHECK(good.entries.size() == 3);
        CHECK(good.totalBytes == kCardFileBytes * 2 + 3);
        CHECK(check_envelope(good, kAppId, kGameId, GameIdPolicy::Require) == EnvelopeStatus::Ok);

        // A multi-entry envelope must survive the plist round-trip intact, with
        // every entry's payload distinguishable from its neighbours'.
        const ParseResult parsed = parse_envelope(encode_envelope(good));
        CHECK(parsed.status == ParseStatus::Ok);
        CHECK(parsed.envelope.entries.size() == 3);
        for (std::size_t i = 0; i < good.entries.size(); ++i) {
            CHECK(parsed.envelope.entries[i].name == good.entries[i].name);
            CHECK(parsed.envelope.entries[i].data == good.entries[i].data);
            CHECK(parsed.envelope.entries[i].digest == good.entries[i].digest);
        }
        CHECK(parsed.envelope.entries[0].digest != parsed.envelope.entries[1].digest);

        // Second entry (a Save), payload flipped: the digest must catch it.
        MirrorEnvelope secondCorrupt = good;
        secondCorrupt.entries[1].data[0] ^= 0xff;
        CHECK(check_envelope(secondCorrupt, kAppId, kGameId, GameIdPolicy::Require) ==
            EnvelopeStatus::DigestMismatch);

        // Last entry, payload flipped.
        MirrorEnvelope lastCorrupt = good;
        lastCorrupt.entries[2].data[1] ^= 0xff;
        CHECK(check_envelope(lastCorrupt, kAppId, kGameId, GameIdPolicy::Require) ==
            EnvelopeStatus::DigestMismatch);

        // Second entry, recorded size inflated: caught before the digest.
        MirrorEnvelope secondSize = good;
        secondSize.entries[1].size += 1;
        CHECK(check_envelope(secondSize, kAppId, kGameId, GameIdPolicy::Require) ==
            EnvelopeStatus::SizeMismatch);

        // Last entry, payload truncated so its recorded size no longer matches.
        MirrorEnvelope lastSize = good;
        lastSize.entries[2].data.pop_back();
        CHECK(check_envelope(lastSize, kAppId, kGameId, GameIdPolicy::Require) ==
            EnvelopeStatus::SizeMismatch);

        // ...and the restore decision must inherit every one of those refusals,
        // not just the ones that happen to hit entries[0].
        CHECK(decide_restore(false, &secondCorrupt, kAppId, kGameId, GameIdPolicy::Require)
                  .reason == RestoreReason::DigestMismatch);
        CHECK(!decide_restore(false, &lastCorrupt, kAppId, kGameId, GameIdPolicy::Require).restore);
        CHECK(decide_restore(false, &secondSize, kAppId, kGameId, GameIdPolicy::Require).reason ==
            RestoreReason::SizeMismatch);
        CHECK(!decide_restore(false, &lastSize, kAppId, kGameId, GameIdPolicy::Require).restore);
    }

    // ------------------------------------------------------------ size guard
    {
        // Fits: nothing is shed.
        std::vector<MirrorEntry> entries;
        entries.push_back(make_entry(kSavePath, filled(kCardFileBytes, 1)));
        entries.push_back(make_entry("config.json", filled(1024, 2)));
        const GuardResult guard = apply_size_guard(entries, kSizeLimitBytes);
        CHECK(guard.status == GuardStatus::Ok);
        CHECK(guard.shed.empty());
        CHECK(entries.size() == 2);
        CHECK(guard.totalBytes == kCardFileBytes + 1024);
    }
    {
        // Over budget: support entries go largest-first, and only as many as
        // needed. Budget 100 -> save 40 + 30 + 20 + 10 = 100 after dropping the
        // 30-byte entry (40 + 20 + 10 + 30 = 100 exactly... so force a drop).
        std::vector<MirrorEntry> entries;
        entries.push_back(make_entry("config.json", filled(30, 'c')));
        entries.push_back(make_entry(kSavePath, filled(40, 's')));
        entries.push_back(make_entry("achievements.json", filled(50, 'a')));
        entries.push_back(make_entry("controller_ports.dat", filled(10, 'p')));
        const GuardResult guard = apply_size_guard(entries, 80);
        CHECK(guard.status == GuardStatus::Shed);
        // 130 total; drop 50 (largest support) -> 80. One drop is enough.
        CHECK(guard.shed.size() == 1);
        CHECK(guard.shed[0] == "achievements.json");
        CHECK(guard.totalBytes == 80);
        CHECK(entries.size() == 3);
        bool saveKept = false;
        for (const auto& entry : entries) {
            CHECK(entry.name != "achievements.json");
            if (entry.name == kSavePath) {
                saveKept = true;
            }
        }
        CHECK(saveKept);
    }
    {
        // The save is never shed even when it is by far the largest entry, and
        // shedding continues until the budget is met.
        std::vector<MirrorEntry> entries;
        entries.push_back(make_entry(kSavePath, filled(60, 's')));
        entries.push_back(make_entry("config.json", filled(40, 'c')));
        entries.push_back(make_entry("achievements.json", filled(30, 'a')));
        entries.push_back(make_entry("keyboard_bindings.dat", filled(20, 'k')));
        const GuardResult guard = apply_size_guard(entries, 70);
        CHECK(guard.status == GuardStatus::Shed);
        CHECK(guard.shed.size() == 3);
        CHECK(guard.shed[0] == "config.json");
        CHECK(guard.shed[1] == "achievements.json");
        CHECK(guard.shed[2] == "keyboard_bindings.dat");
        CHECK(entries.size() == 1);
        CHECK(entries[0].name == kSavePath);
        CHECK(guard.totalBytes == 60);
    }
    {
        // Equal-sized support entries shed in name order, so the outcome is
        // deterministic across runs.
        std::vector<MirrorEntry> entries;
        entries.push_back(make_entry("keyboard_bindings.dat", filled(10, 'k')));
        entries.push_back(make_entry("achievements.json", filled(10, 'a')));
        entries.push_back(make_entry(kSavePath, filled(10, 's')));
        const GuardResult guard = apply_size_guard(entries, 20);
        CHECK(guard.status == GuardStatus::Shed);
        CHECK(guard.shed.size() == 1);
        CHECK(guard.shed[0] == "achievements.json");
    }
    {
        // The save alone blows the budget: refuse the whole write and leave the
        // caller's entry list untouched.
        std::vector<MirrorEntry> entries;
        entries.push_back(make_entry(kSavePath, filled(500, 's')));
        entries.push_back(make_entry("config.json", filled(10, 'c')));
        const GuardResult guard = apply_size_guard(entries, 400);
        CHECK(guard.status == GuardStatus::SaveTooLarge);
        CHECK(entries.size() == 2);
        CHECK(guard.totalBytes == 500);
    }

    // ------------------------------------------------- restore truth table
    {
        const MirrorEnvelope good = sample_envelope();

        // Save present: never restore, whatever the mirror says.
        CHECK(decide_restore(true, &good, kAppId, kGameId, GameIdPolicy::Require).reason ==
            RestoreReason::PrimaryPresent);
        CHECK(!decide_restore(true, &good, kAppId, kGameId, GameIdPolicy::Require).restore);
        CHECK(!decide_restore(true, nullptr, kAppId, kGameId, GameIdPolicy::Require).restore);

        // Save missing + valid mirror: restore.
        const RestoreDecision missing =
            decide_restore(false, &good, kAppId, kGameId, GameIdPolicy::Require);
        CHECK(missing.restore);
        CHECK(missing.reason == RestoreReason::Restore);

        // Save missing + no mirror at all: nothing to do.
        const RestoreDecision none =
            decide_restore(false, nullptr, kAppId, kGameId, GameIdPolicy::Require);
        CHECK(!none.restore);
        CHECK(none.reason == RestoreReason::NoMirror);

        // Save missing + bad digest: refuse, do not write a corrupt save.
        MirrorEnvelope corrupt = good;
        corrupt.entries[0].data[16] ^= 0x01;
        const RestoreDecision bad =
            decide_restore(false, &corrupt, kAppId, kGameId, GameIdPolicy::Require);
        CHECK(!bad.restore);
        CHECK(bad.reason == RestoreReason::DigestMismatch);

        // Save missing + mirror from another region: refuse.
        MirrorEnvelope otherGame = good;
        otherGame.gameId = "GZ2J01";
        const RestoreDecision wrongGame =
            decide_restore(false, &otherGame, kAppId, kGameId, GameIdPolicy::Require);
        CHECK(!wrongGame.restore);
        CHECK(wrongGame.reason == RestoreReason::GameIdMismatch);
        // ...but the config-class pass still accepts it.
        CHECK(decide_restore(false, &otherGame, kAppId, kGameId, GameIdPolicy::Ignore).restore);

        // Save missing + mirror from the sibling port: refuse.
        MirrorEnvelope otherApp = good;
        otherApp.app = "banjo";
        CHECK(decide_restore(false, &otherApp, kAppId, kGameId, GameIdPolicy::Ignore).reason ==
            RestoreReason::AppMismatch);

        // Save missing + unknown schema: refuse.
        MirrorEnvelope otherSchema = good;
        otherSchema.schema = 99;
        CHECK(decide_restore(false, &otherSchema, kAppId, kGameId, GameIdPolicy::Require).reason ==
            RestoreReason::SchemaMismatch);

        // The three remaining EnvelopeStatus values must each surface as their
        // own RestoreReason rather than collapsing into a generic refusal --
        // the reason string is the only diagnosis a device log ever gets.
        MirrorEnvelope shortEntry = good;
        shortEntry.entries[1].data.pop_back();
        const RestoreDecision shortDecision =
            decide_restore(false, &shortEntry, kAppId, kGameId, GameIdPolicy::Require);
        CHECK(!shortDecision.restore);
        CHECK(shortDecision.reason == RestoreReason::SizeMismatch);

        MirrorEnvelope wrongTotal = good;
        wrongTotal.totalBytes -= 1;
        const RestoreDecision totalDecision =
            decide_restore(false, &wrongTotal, kAppId, kGameId, GameIdPolicy::Require);
        CHECK(!totalDecision.restore);
        CHECK(totalDecision.reason == RestoreReason::TotalMismatch);

        const MirrorEnvelope empty = build_envelope(kAppId, kGameId, 3, 4, {});
        const RestoreDecision emptyDecision =
            decide_restore(false, &empty, kAppId, kGameId, GameIdPolicy::Require);
        CHECK(!emptyDecision.restore);
        CHECK(emptyDecision.reason == RestoreReason::NoEntries);
    }

    std::puts("core_test OK");
    return 0;
}
