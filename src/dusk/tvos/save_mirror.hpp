#pragma once

// tvOS save mirror -- public entry points.
//
// tvOS gives an app roughly 500 KB of durable storage (NSUserDefaults) and puts
// everything else in Library/Caches, which the system may reclaim at any time.
// Dusklight writes its memory card there, so a purge silently destroys the
// player's progress. This mirrors the card file and the small config-class files
// into NSUserDefaults the moment a save write completes, and restores them at
// startup only when the on-disk copy is gone.
//
// The implementation lives in save_mirror.mm and is compiled only for tvOS; the
// decision logic it uses is in save_mirror_core.{hpp,cpp}, which is plain C++ and
// host-testable. Call sites guard on DUSK_TVOS_SAVE_MIRROR, which is defined
// here so it can never drift from the CMake condition that compiles the .mm.

#if defined(__APPLE__)
#include <TargetConditionals.h>
#endif

#if defined(__APPLE__) && defined(TARGET_OS_TV) && TARGET_OS_TV
#define DUSK_TVOS_SAVE_MIRROR 1
#else
#define DUSK_TVOS_SAVE_MIRROR 0
#endif

#if DUSK_TVOS_SAVE_MIRROR

#include <filesystem>

namespace dusk::tvos::save_mirror {

/**
 * Starts the mirror and restores any config-class files that are missing.
 *
 * Call once, after the data directories and logging are up but *before* the
 * config is read, so a restored config.json is the one that gets loaded (and is
 * therefore not overwritten by the next config save).
 *
 * The game id is not known this early, so this pass covers config-class files
 * only; saves are handled by on_card_init().
 */
void init(const std::filesystem::path& dataRoot);

/**
 * Installs the SDL event watch that forces an immediate flush on
 * SDL_EVENT_WILL_ENTER_BACKGROUND / SDL_EVENT_TERMINATING. Call after SDL is up
 * (i.e. after aurora_initialize).
 *
 * aurora has its own event watch; this is a second, independent one installed
 * from the fork so extern/aurora stays untouched.
 */
void install_lifecycle_watch();

/**
 * Records the card configuration and restores the memory card if it is missing.
 *
 * Call from mDoMemCd_Ctrl_c::ThdInit() *before* CARDInit(), which creates and
 * formats a fresh card when none is found -- after that point the save no longer
 * looks missing and the mirror would never be applied.
 *
 * @param rawCardImage true when the card backend is CARD_RAWIMAGE, which writes a
 *                      whole-card image far too large for the NSUserDefaults
 *                      budget; save mirroring is then disabled with a log line.
 *                      Passed as a bool rather than a CARDFileType because
 *                      dolphin/types.h typedefs BOOL as int and cannot be
 *                      included from Objective-C++.
 * @param gameName      DVDDiskID::gameName, 4 chars, not NUL-terminated.
 * @param company       DVDDiskID::company, 2 chars, not NUL-terminated.
 */
void on_card_init(bool rawCardImage, const char* gameName, const char* company);

/**
 * Marks the mirror dirty and schedules a debounced flush. Called from the tail of
 * mDoMemCd_Ctrl_c::store() on MemCardThread; does no I/O and takes no lock.
 */
void note_save_written();

/** Cancels any pending debounce and flushes synchronously. */
void flush_now(const char* reason);

}  // namespace dusk::tvos::save_mirror

#endif  // DUSK_TVOS_SAVE_MIRROR
