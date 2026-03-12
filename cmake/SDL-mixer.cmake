set(SDLMIXER_VENDORED          ON)
set(SDLMIXER_STRICT            ON)
set(BUILD_SHARED_LIBS          OFF)

set(SDLMIXER_AIFF              OFF)
set(SDLMIXER_WAVE              OFF)
set(SDLMIXER_VOC               OFF)
set(SDLMIXER_AU                OFF)
set(SDLMIXER_FLAC              OFF)
set(SDLMIXER_GME               OFF)
set(SDLMIXER_MOD               OFF)
set(SDLMIXER_MP3               OFF)
set(SDLMIXER_MIDI              OFF)
set(SDLMIXER_OPUS              OFF)
set(SDLMIXER_WAVPACK           OFF)
set(SDLMIXER_VORBIS_STB        OFF)
set(SDLMIXER_VORBIS_VORBISFILE OFF)

FetchContent_Declare(SDL-mixer
    URL https://github.com/libsdl-org/SDL_mixer/archive/refs/tags/release-3.2.0.tar.gz
    URL_HASH SHA256=1077fef4a6e9513027da0a08e6f1b8645f8c88c8bef001c17befeb52b129b5dd
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

FetchContent_MakeAvailable(SDL-mixer)
if (NOT TARGET SDL3_mixer::SDL3_mixer)
    message(FATAL_ERROR "Failed to make SDL-mixer available")
endif ()
