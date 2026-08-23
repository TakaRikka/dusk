#include "dusk/tvos/save_mirror.hpp"

#if DUSK_TVOS_SAVE_MIRROR

#include "dusk/tvos/save_mirror_core.hpp"

#include "dusk/logging.h"

#include <borealis/io.hpp>

#include <SDL3/SDL_error.h>
#include <SDL3/SDL_events.h>

// Note: no dolphin/* header here. dolphin/types.h typedefs BOOL as int, which
// collides with Objective-C's BOOL; that is why on_card_init() takes a plain
// bool for the card format rather than a CARDFileType.
#import <Foundation/Foundation.h>
#include <dispatch/dispatch.h>

#include <algorithm>
#include <atomic>
#include <cerrno>
#include <cstring>
#include <ctime>
#include <exception>
#include <mutex>
#include <string>
#include <system_error>
#include <utility>
#include <vector>

#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

namespace fs = std::filesystem;

namespace dusk::tvos::save_mirror {
namespace {

// Everything logs through the "dusk" module with a fixed [save-mirror] prefix, so
// a device console log can be reduced to just this subsystem with one grep.
constexpr const char* kTag = "[save-mirror]";

// Debounce window for coalescing a burst of save writes.
constexpr std::int64_t kDebounceNanos = 2ll * NSEC_PER_SEC;

// The scan roots and the config-class allowlist live in save_mirror_core.hpp
// (kRegionDirs, kCardDirs, kSupportFileNames, kControllerSuffix) so that the
// scanner below and classify_entry() read the same list and cannot drift apart.

dispatch_queue_t g_queue;
std::atomic<std::uint64_t> g_generation{0};
// Claims init(); the mirror is only usable once g_started goes true, which
// happens after g_queue exists.
std::atomic<bool> g_initClaimed{false};
std::atomic<bool> g_started{false};
std::atomic<bool> g_watchInstalled{false};

std::mutex g_stateMutex;
fs::path g_dataRoot;            // guarded by g_stateMutex
std::string g_gameId;           // guarded by g_stateMutex; empty until on_card_init
bool g_saveMirroringEnabled{};  // guarded by g_stateMutex; false until on_card_init

struct State {
    fs::path dataRoot;
    std::string gameId;
    bool saveMirroringEnabled = false;
};

State snapshot_state() {
    std::lock_guard<std::mutex> lock(g_stateMutex);
    return State{g_dataRoot, g_gameId, g_saveMirroringEnabled};
}

std::string path_string(const fs::path& path) {
    return borealis::io::fs_path_to_string(path);
}

// The one place an escaped exception becomes a log line instead of a crash.
//
// Every entry point below runs either inside a dispatch block, inside an SDL C
// callback, or on the game's own memory-card thread -- none of which has a
// handler above it, so a throw terminates the process. std::filesystem is the
// live risk: a readdir that fails part-way through a scan throws, and a Caches
// reclaim landing mid-scan is precisely the situation this whole feature exists
// to survive. Call from inside a catch block; `throw;` rethrows the exception
// currently being handled.
void log_escaped(const char* where) {
    try {
        throw;
    } catch (const std::exception& e) {
        DuskLog.error("{} {}: unhandled exception ({}); the stored mirror is unchanged", kTag,
            where, e.what());
    } catch (...) {
        DuskLog.error("{} {}: unhandled exception; the stored mirror is unchanged", kTag, where);
    }
}

// ---------------------------------------------------------------------------
// Value <-> Foundation
//
// A mechanical type mapping and nothing more: the field names, the layout and
// every validation rule live in save_mirror_core, where the host tests can reach
// them.
// ---------------------------------------------------------------------------

id to_object(const Value& value) {
    switch (value.type) {
    case Value::Type::Int:
        return @(value.integer);
    case Value::Type::String:
        return [NSString stringWithUTF8String:value.string.c_str()] ?: @"";
    case Value::Type::Bytes:
        return [NSData dataWithBytes:value.bytes.data() length:value.bytes.size()];
    case Value::Type::Array: {
        NSMutableArray* array = [NSMutableArray arrayWithCapacity:value.array.size()];
        for (const auto& element : value.array) {
            [array addObject:to_object(element)];
        }
        return array;
    }
    case Value::Type::Dict: {
        NSMutableDictionary* dict = [NSMutableDictionary dictionaryWithCapacity:value.dict.size()];
        for (const auto& [key, element] : value.dict) {
            NSString* nsKey = [NSString stringWithUTF8String:key.c_str()];
            if (nsKey != nil) {
                dict[nsKey] = to_object(element);
            }
        }
        return dict;
    }
    case Value::Type::Null:
        break;
    }
    return [NSNull null];
}

Value from_object(id object) {
    if ([object isKindOfClass:[NSNumber class]]) {
        return Value::of_int([(NSNumber*)object longLongValue]);
    }
    if ([object isKindOfClass:[NSString class]]) {
        const char* utf8 = [(NSString*)object UTF8String];
        return Value::of_string(utf8 != nullptr ? std::string(utf8) : std::string());
    }
    if ([object isKindOfClass:[NSData class]]) {
        NSData* data = (NSData*)object;
        const auto* bytes = static_cast<const std::uint8_t*>(data.bytes);
        return Value::of_bytes(std::vector<std::uint8_t>(bytes, bytes + data.length));
    }
    if ([object isKindOfClass:[NSArray class]]) {
        std::vector<Value> values;
        for (id element in (NSArray*)object) {
            values.push_back(from_object(element));
        }
        return Value::of_array(std::move(values));
    }
    if ([object isKindOfClass:[NSDictionary class]]) {
        std::vector<std::pair<std::string, Value>> entries;
        NSDictionary* dict = (NSDictionary*)object;
        for (id key in dict) {
            if (![key isKindOfClass:[NSString class]]) {
                continue;
            }
            const char* utf8 = [(NSString*)key UTF8String];
            if (utf8 == nullptr) {
                continue;
            }
            entries.emplace_back(std::string(utf8), from_object(dict[key]));
        }
        return Value::of_dict(std::move(entries));
    }
    return Value{};
}

// ---------------------------------------------------------------------------
// Stored envelope
// ---------------------------------------------------------------------------

// Returns false when there is no mirror at all; `outResult` carries the parse
// status when there is one, valid or not.
bool load_stored(ParseResult& outResult) {
    NSString* key = [NSString stringWithUTF8String:kDefaultsKey];
    id object = [[NSUserDefaults standardUserDefaults] objectForKey:key];
    if (object == nil) {
        return false;
    }
    outResult = parse_envelope(from_object(object));
    return true;
}

// Returns false when the write cannot be confirmed. setObject: reports nothing,
// so success is checked two ways: synchronize must say the write-back worked, and
// the key must read back. Without that a flush onto a full disk would be logged
// as a success and the mirror silently left at its previous contents.
bool store(const MirrorEnvelope& envelope) {
    NSString* key = [NSString stringWithUTF8String:kDefaultsKey];
    NSUserDefaults* defaults = [NSUserDefaults standardUserDefaults];
    [defaults setObject:to_object(encode_envelope(envelope)) forKey:key];
    // Modern tvOS persists defaults on its own schedule, but the whole point of
    // the mirror is to survive a kill that arrives at a bad moment, so push it out
    // now rather than trusting the next automatic write-back.
    const bool synchronized = [defaults synchronize] == YES;
    const bool readable = [defaults objectForKey:key] != nil;
    if (!synchronized || !readable) {
        DuskLog.error("{} the mirror could not be written to NSUserDefaults (synchronize {}, "
                      "key readable {})",
            kTag, synchronized, readable);
        return false;
    }
    return true;
}

// ---------------------------------------------------------------------------
// File I/O
// ---------------------------------------------------------------------------

// Reads at most kMaxEntryBytes. The size is checked before a single byte is read:
// a file larger than the whole NSUserDefaults budget can never be stored, so
// reading it would only be a way to spend memory before failing -- and that is
// the cheap defence against a config.json that has run away.
bool read_file(const fs::path& path, std::vector<std::uint8_t>& out) {
    const int fd = ::open(path.c_str(), O_RDONLY | O_CLOEXEC);
    if (fd < 0) {
        return false;
    }
    struct stat info = {};
    if (::fstat(fd, &info) != 0 || !S_ISREG(info.st_mode)) {
        ::close(fd);
        return false;
    }
    if (static_cast<std::uint64_t>(info.st_size) > kMaxEntryBytes) {
        DuskLog.warn("{} {} is {} bytes, past the {} byte per-entry cap; not mirrored", kTag,
            path_string(path), static_cast<std::uint64_t>(info.st_size), kMaxEntryBytes);
        ::close(fd);
        return false;
    }

    out.clear();
    out.reserve(static_cast<std::size_t>(info.st_size));
    std::uint8_t buffer[64 * 1024];
    for (;;) {
        const ssize_t got = ::read(fd, buffer, sizeof(buffer));
        if (got < 0) {
            if (errno == EINTR) {
                continue;
            }
            ::close(fd);
            return false;
        }
        if (got == 0) {
            break;
        }
        // The file grew while it was being read. Storing the prefix would mirror
        // a save nobody ever wrote, so give up instead of truncating.
        if (out.size() + static_cast<std::size_t>(got) > kMaxEntryBytes) {
            DuskLog.warn("{} {} grew past the {} byte per-entry cap while being read; not mirrored",
                kTag, path_string(path), kMaxEntryBytes);
            ::close(fd);
            out.clear();
            return false;
        }
        out.insert(out.end(), buffer, buffer + got);
    }
    ::close(fd);
    return true;
}

// Temp file in the destination directory, fsync, then rename: a purge or a kill
// mid-restore leaves either the old state or the complete new file, never a
// half-written save.
bool write_atomic(const fs::path& target, const std::vector<std::uint8_t>& data,
    std::string& error) {
    std::error_code ec;
    fs::create_directories(target.parent_path(), ec);
    if (ec && !fs::exists(target.parent_path())) {
        error = "create_directories: " + ec.message();
        return false;
    }

    const fs::path temp = target.parent_path() / (target.filename().string() + ".mirror-tmp");
    const int fd = ::open(temp.c_str(), O_WRONLY | O_CREAT | O_TRUNC | O_CLOEXEC, 0644);
    if (fd < 0) {
        error = std::string("open: ") + std::strerror(errno);
        return false;
    }

    std::size_t written = 0;
    while (written < data.size()) {
        const ssize_t put = ::write(fd, data.data() + written, data.size() - written);
        if (put < 0) {
            if (errno == EINTR) {
                continue;
            }
            error = std::string("write: ") + std::strerror(errno);
            ::close(fd);
            fs::remove(temp, ec);
            return false;
        }
        written += static_cast<std::size_t>(put);
    }
    if (::fsync(fd) != 0) {
        error = std::string("fsync: ") + std::strerror(errno);
        ::close(fd);
        fs::remove(temp, ec);
        return false;
    }
    ::close(fd);

    fs::rename(temp, target, ec);
    if (ec) {
        error = "rename: " + ec.message();
        fs::remove(temp, ec);
        return false;
    }
    return true;
}

// A file counts as present only if it exists and is non-empty. Deliberately not
// "and is exactly kCardFileBytes long": overwriting a save that is merely an
// unexpected size would destroy real progress, and refusing to restore is the
// cheaper mistake.
bool file_present(const fs::path& path) {
    std::error_code ec;
    if (!fs::is_regular_file(path, ec) || ec) {
        return false;
    }
    const auto size = fs::file_size(path, ec);
    if (ec || size == 0) {
        return false;
    }
    const int fd = ::open(path.c_str(), O_RDONLY | O_CLOEXEC);
    if (fd < 0) {
        return false;
    }
    std::uint8_t probe = 0;
    const ssize_t got = ::read(fd, &probe, 1);
    ::close(fd);
    return got == 1;
}

// Both scans use directory_iterator's error_code form throughout, including the
// increment. The range-for spelling increments by calling the *throwing*
// overload, so a readdir that fails part-way -- a Caches reclaim landing mid-scan
// -- throws out of a dispatch block or an SDL callback and takes the process with
// it. `ec` on the increment turns that into an early stop and a log line.
std::vector<fs::path> scan_card_files(const fs::path& dataRoot) {
    std::vector<fs::path> found;
    for (const auto region : kRegionDirs) {
        for (const auto card : kCardDirs) {
            const fs::path dir = dataRoot / std::string(region) / std::string(card);
            std::error_code probe;
            if (!fs::is_directory(dir, probe) || probe) {
                continue;
            }
            std::error_code walk;
            for (auto it = fs::directory_iterator(dir, walk), end = fs::directory_iterator();
                 !walk && it != end; it.increment(walk))
            {
                std::error_code stat;
                if (!it->is_regular_file(stat) || stat) {
                    continue;
                }
                const std::string relative = std::string(region) + "/" + std::string(card) + "/" +
                                             it->path().filename().string();
                if (classify_entry(relative) == EntryClass::Save) {
                    found.push_back(it->path());
                }
            }
            if (walk) {
                DuskLog.warn("{} scan of {} stopped early: {}", kTag, path_string(dir),
                    walk.message());
            }
        }
    }
    return found;
}

std::vector<std::string> scan_support_names(const fs::path& dataRoot) {
    std::vector<std::string> names;
    for (const auto name : kSupportFileNames) {
        names.emplace_back(name);
    }

    // The fixed names above are looked for whether or not they exist; the scan
    // only has to find the pad mappings, which are named after the controller.
    std::error_code walk;
    for (auto it = fs::directory_iterator(dataRoot, walk), end = fs::directory_iterator();
         !walk && it != end; it.increment(walk))
    {
        std::error_code stat;
        if (!it->is_regular_file(stat) || stat) {
            continue;
        }
        std::string name = it->path().filename().string();
        if (classify_entry(name) != EntryClass::Support) {
            continue;
        }
        if (std::find(names.begin(), names.end(), name) != names.end()) {
            continue;
        }
        names.push_back(std::move(name));
    }
    if (walk) {
        DuskLog.warn("{} scan of {} stopped early: {}", kTag, path_string(dataRoot),
            walk.message());
    }
    return names;
}

// ---------------------------------------------------------------------------
// Flush
// ---------------------------------------------------------------------------

// The flush proper. Reached only through flush_on_queue() below, which is the
// exception barrier.
void flush_impl(const char* reason) {
    const State state = snapshot_state();
    if (state.dataRoot.empty()) {
        DuskLog.error("{} flush({}) skipped: data root is not set", kTag, reason);
        return;
    }

    ParseResult stored;
    const bool hadStored = load_stored(stored);

    std::vector<MirrorEntry> entries;

    // Saves.
    std::vector<std::string> savedNames;
    if (state.saveMirroringEnabled) {
        for (const auto& path : scan_card_files(state.dataRoot)) {
            std::vector<std::uint8_t> data;
            if (!read_file(path, data)) {
                DuskLog.warn("{} flush({}): could not read card file {}", kTag, reason,
                    path_string(path));
                continue;
            }
            const std::string relative = path_string(path.lexically_relative(state.dataRoot));
            entries.push_back(make_entry(relative, std::move(data)));
            savedNames.push_back(relative);
        }
    }

    // If no card file is on disk right now, carry the mirrored one forward rather
    // than replacing the mirror with a save-less envelope. A missing card file is
    // exactly the purge this feature exists for; dropping the save from the mirror
    // at that moment would throw away the only surviving copy.
    if (savedNames.empty() && hadStored && stored.status == ParseStatus::Ok) {
        const char* why = state.saveMirroringEnabled ? "no card file on disk"
                                                     : "save mirroring disabled for this session";
        for (const auto& entry : stored.envelope.entries) {
            if (classify_entry(entry.name) != EntryClass::Save) {
                continue;
            }
            if (sha256_hex(entry.data) != entry.digest || entry.data.size() != entry.size) {
                DuskLog.warn("{} flush({}): stored save entry {} failed validation, dropping it",
                    kTag, reason, entry.name);
                continue;
            }
            DuskLog.info("{} flush({}): {}, carrying mirrored {} forward ({} bytes)", kTag,
                reason, why, entry.name, entry.size);
            entries.push_back(entry);
            savedNames.push_back(entry.name);
        }
    }

    // Config-class files.
    for (const auto& name : scan_support_names(state.dataRoot)) {
        const fs::path path = state.dataRoot / name;
        std::vector<std::uint8_t> data;
        if (!file_present(path) || !read_file(path, data)) {
            continue;
        }
        entries.push_back(make_entry(name, std::move(data)));
    }

    if (entries.empty()) {
        DuskLog.info("{} flush({}): nothing to mirror, leaving the stored mirror alone", kTag,
            reason);
        return;
    }

    const std::uint64_t rawTotal = total_bytes(entries);
    const GuardResult guard = apply_size_guard(entries, kSizeLimitBytes);
    if (guard.status == GuardStatus::SaveTooLarge) {
        DuskLog.error("{} flush({}) ABORTED: saves alone are {} bytes, over the {} byte budget; "
                      "the stored mirror is left untouched",
            kTag, reason, guard.totalBytes, kSizeLimitBytes);
        return;
    }
    for (const auto& name : guard.shed) {
        DuskLog.warn("{} flush({}): shed non-save entry {} to stay under the {} byte budget", kTag,
            reason, name, kSizeLimitBytes);
    }

    const std::int64_t sequence =
        (hadStored && stored.status == ParseStatus::Ok) ? stored.envelope.sequence + 1 : 1;

    // The disc is not open during the pre-launch UI, so a background flush can
    // happen before the game id is known. Inherit the stored one rather than
    // stamping "unknown" over it -- that would fail the game-id check on the next
    // restore and quietly strand the mirrored save.
    std::string gameId = state.gameId;
    if (gameId.empty()) {
        if (hadStored && stored.status == ParseStatus::Ok && !stored.envelope.gameId.empty()) {
            gameId = stored.envelope.gameId;
            DuskLog.info("{} flush({}): game id not known yet, keeping the stored id {}", kTag,
                reason, gameId);
        } else {
            gameId = "unknown";
        }
    }

    const auto timestamp = static_cast<std::int64_t>(std::time(nullptr));
    const MirrorEnvelope envelope =
        build_envelope(kAppId, gameId, sequence, timestamp, std::move(entries));

    if (!store(envelope)) {
        DuskLog.error("{} flush({}) FAILED to store sequence {}; the previous mirror is what "
                      "survives a purge from here",
            kTag, reason, envelope.sequence);
        return;
    }

    DuskLog.info("{} flush({}) wrote sequence {} for game {}: {} entries, {} bytes (raw {}, "
                 "shed {}), guard {}",
        kTag, reason, envelope.sequence, envelope.gameId, envelope.entries.size(),
        envelope.totalBytes, rawTotal, guard.shed.size(), to_string(guard.status));
    for (const auto& entry : envelope.entries) {
        DuskLog.info("{}   entry {} ({} bytes, {})", kTag, entry.name, entry.size,
            entry.digest.substr(0, 12));
    }
}

// Runs on g_queue only, and never throws. A dispatch block has no handler above
// it, so an exception escaping this function would terminate the app -- which is
// the worst possible response to the transient filesystem failure that most
// likely caused it.
void flush_on_queue(const char* reason) {
    try {
        flush_impl(reason);
    } catch (...) {
        log_escaped("flush");
    }
}

// ---------------------------------------------------------------------------
// SDL lifecycle watch
// ---------------------------------------------------------------------------

bool SDLCALL lifecycle_event_watch(void*, SDL_Event* event) {
    // aurora installs its own watch (extern/aurora/lib/window.cpp) and handles
    // neither of these events; this one is independent so the submodule stays
    // untouched. SDL invokes watches on the thread that pushes the event, which
    // for these two is UIKit's main thread -- so the flush below happens inside
    // the system callback, while the app is still allowed to run.
    //
    // This is a C callback reached from SDL's own event pump: an exception
    // crossing it is undefined behaviour long before anything could catch it.
    try {
        switch (event->type) {
        case SDL_EVENT_WILL_ENTER_BACKGROUND:
            flush_now("will-enter-background");
            break;
        case SDL_EVENT_TERMINATING:
            flush_now("terminating");
            break;
        default:
            break;
        }
    } catch (...) {
        log_escaped("lifecycle watch");
    }
    return true;
}

}  // namespace

// ---------------------------------------------------------------------------
// Public entry points
// ---------------------------------------------------------------------------

void init(const fs::path& dataRoot) {
    // g_initClaimed makes init() one-shot; g_started is what every other entry
    // point tests, and it is published *after* the queue exists. Publishing it
    // first left a window in which note_save_written() would dispatch onto a null
    // queue.
    if (g_initClaimed.exchange(true)) {
        return;
    }

    {
        std::lock_guard<std::mutex> lock(g_stateMutex);
        g_dataRoot = dataRoot;
    }
    g_queue = dispatch_queue_create("dev.twilitrealm.dusk.save-mirror",
        dispatch_queue_attr_make_with_qos_class(DISPATCH_QUEUE_SERIAL, QOS_CLASS_UTILITY, 0));
    g_started.store(true);

    DuskLog.info("{} started; data root {}, key '{}', budget {} bytes", kTag,
        path_string(dataRoot), kDefaultsKey, kSizeLimitBytes);

    // Config-class restore. Runs before the config is read so a restored
    // config.json is the one that gets loaded; otherwise the next config save
    // would write in-memory defaults straight back over it.
    try {
    @autoreleasepool {
        ParseResult stored;
        const bool hadStored = load_stored(stored);
        if (!hadStored) {
            DuskLog.info("{} restore(config): no mirror stored yet", kTag);
            return;
        }
        if (stored.status != ParseStatus::Ok) {
            DuskLog.warn("{} restore(config): stored mirror is unreadable ({}{}{})", kTag,
                to_string(stored.status), stored.detail.empty() ? "" : ": ", stored.detail);
            return;
        }

        // GameIdPolicy::Ignore: config-class files are not game state, and the
        // disc is not open yet, so there is no id to compare against.
        const RestoreDecision decision = decide_restore(
            /*primaryPresent=*/false, &stored.envelope, kAppId, "", GameIdPolicy::Ignore);
        if (!decision.restore) {
            DuskLog.warn("{} restore(config): declined ({})", kTag, to_string(decision.reason));
            return;
        }

        int restored = 0;
        int skipped = 0;
        for (const auto& entry : stored.envelope.entries) {
            if (classify_entry(entry.name) != EntryClass::Support) {
                continue;
            }
            const fs::path target = dataRoot / entry.name;
            if (file_present(target)) {
                ++skipped;
                continue;
            }
            std::string error;
            if (!write_atomic(target, entry.data, error)) {
                DuskLog.error("{} restore(config): failed to write {}: {}", kTag,
                    path_string(target), error);
                continue;
            }
            DuskLog.info("{} restore(config): restored {} ({} bytes) from mirror sequence {}", kTag,
                entry.name, entry.size, stored.envelope.sequence);
            ++restored;
        }
        DuskLog.info("{} restore(config): {} restored, {} already present (mirror sequence {}, "
                     "game {})",
            kTag, restored, skipped, stored.envelope.sequence, stored.envelope.gameId);
    }
    } catch (...) {
        // Start-up must survive a mirror that cannot be read. The game's own
        // config defaults are a recoverable inconvenience; a crash here is not.
        log_escaped("restore(config)");
    }
}

void install_lifecycle_watch() {
    if (!g_started.load()) {
        DuskLog.warn("{} lifecycle watch requested before init(); ignored", kTag);
        return;
    }
    if (g_watchInstalled.exchange(true)) {
        return;
    }
    if (!SDL_AddEventWatch(lifecycle_event_watch, nullptr)) {
        g_watchInstalled.store(false);
        DuskLog.error("{} failed to install the lifecycle event watch: {}", kTag, SDL_GetError());
        return;
    }
    DuskLog.info("{} lifecycle watch installed (background/terminate force an immediate flush)",
        kTag);
}

void on_card_init(bool rawCardImage, const char* gameName, const char* company) {
    if (!g_started.load()) {
        DuskLog.warn("{} on_card_init() before init(); save mirroring stays off", kTag);
        return;
    }

    std::string gameId;
    if (gameName != nullptr && company != nullptr) {
        gameId.assign(gameName, 4).append(company, 2);
    }

    fs::path dataRoot;
    {
        std::lock_guard<std::mutex> lock(g_stateMutex);
        g_gameId = gameId;
        g_saveMirroringEnabled = !rawCardImage;
        dataRoot = g_dataRoot;
    }

    if (rawCardImage) {
        // A raw card image is the whole 512-block card, orders of magnitude past
        // the NSUserDefaults budget. Skip saves loudly rather than shedding
        // everything else and still failing.
        DuskLog.warn("{} card format is CARD_RAWIMAGE (whole-card image); save mirroring is "
                     "DISABLED for this session. Switch the card format to GCI folder in "
                     "Settings to protect saves from a Caches purge.",
            kTag);
        return;
    }

    DuskLog.info("{} card ready: game {}, GCI folder format", kTag, gameId);

    try {
    @autoreleasepool {
        const auto cardFiles = scan_card_files(dataRoot);
        bool primaryPresent = false;
        for (const auto& path : cardFiles) {
            if (file_present(path)) {
                primaryPresent = true;
                DuskLog.info("{} restore(save): {} is present on disk", kTag,
                    path_string(path.lexically_relative(dataRoot)));
            }
        }

        ParseResult stored;
        const bool hadStored = load_stored(stored);
        if (hadStored && stored.status != ParseStatus::Ok) {
            DuskLog.warn("{} restore(save): stored mirror is unreadable ({}{}{})", kTag,
                to_string(stored.status), stored.detail.empty() ? "" : ": ", stored.detail);
        }
        const MirrorEnvelope* envelope =
            (hadStored && stored.status == ParseStatus::Ok) ? &stored.envelope : nullptr;

        const RestoreDecision decision =
            decide_restore(primaryPresent, envelope, kAppId, gameId, GameIdPolicy::Require);
        if (!decision.restore) {
            DuskLog.info("{} restore(save): declined ({})", kTag, to_string(decision.reason));
            return;
        }

        int restored = 0;
        for (const auto& entry : envelope->entries) {
            if (classify_entry(entry.name) != EntryClass::Save) {
                continue;
            }
            const fs::path target = dataRoot / entry.name;
            std::string error;
            if (!write_atomic(target, entry.data, error)) {
                DuskLog.error("{} restore(save): failed to write {}: {}", kTag,
                    path_string(target), error);
                continue;
            }
            DuskLog.info("{} restore(save): restored {} ({} bytes, {}) from mirror sequence {} "
                         "written at unix {}",
                kTag, entry.name, entry.size, entry.digest.substr(0, 12), envelope->sequence,
                envelope->timestamp);
            ++restored;
        }
        if (restored == 0) {
            DuskLog.warn("{} restore(save): mirror validated but held no card file", kTag);
        }
    }
    } catch (...) {
        // Reached from the game's card thread start-up. A mirror that cannot be
        // read must cost the player a restore, not the launch.
        log_escaped("restore(save)");
    }
}

void note_save_written() {
    if (!g_started.load()) {
        return;
    }
    // Runs on MemCardThread with no lock held: mark dirty, schedule, return.
    // Never touch the filesystem or NSUserDefaults from here.
    const std::uint64_t generation = g_generation.fetch_add(1) + 1;
    dispatch_after(dispatch_time(DISPATCH_TIME_NOW, kDebounceNanos), g_queue, ^{
      try {
          if (g_generation.load() != generation) {
              return;  // superseded by a later save inside the debounce window
          }
          @autoreleasepool {
              flush_on_queue("save-write");
          }
      } catch (...) {
          log_escaped("debounced flush");
      }
    });
}

void flush_now(const char* reason) {
    if (!g_started.load()) {
        return;
    }
    // Cancel any pending debounce: a save completed less than the debounce window
    // before backgrounding must not be lost.
    g_generation.fetch_add(1);
    dispatch_sync(g_queue, ^{
      @autoreleasepool {
          flush_on_queue(reason);
          [[NSUserDefaults standardUserDefaults] synchronize];
      }
    });
    DuskLog.info("{} synchronous flush({}) complete", kTag, reason);
}

}  // namespace dusk::tvos::save_mirror

#endif  // DUSK_TVOS_SAVE_MIRROR
