#ifndef DUSK_MEMORY_H
#define DUSK_MEMORY_H

#if TARGET_PC
#define HEAP_SIZE(original, dusk) (dusk)
#else
#define HEAP_SIZE(original, dusk) (original)
#endif

#endif
