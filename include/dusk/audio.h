#ifndef DUSK_AUDIO_H
#define DUSK_AUDIO_H

#if TARGET_PC
#define DUSK_AUDIO_DISABLED 1
#else
#define DUSK_AUDIO_DISABLED 0
#endif

#if TARGET_PC
#define DUSK_AUDIO_SKIP(...) return __VA_ARGS__;
#else
#define DUSK_AUDIO_SKIP(...)
#endif

#endif  // DUSK_AUDIO_H
