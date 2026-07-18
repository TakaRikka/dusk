#pragma once

#if TARGET_PC
#define DUSK_AUDIO_DISABLED 0
#else
#define DUSK_AUDIO_DISABLED 0
#endif

#if TARGET_PC
#define DUSK_AUDIO_SKIP(...)
#else
#define DUSK_AUDIO_SKIP(...)
#endif
