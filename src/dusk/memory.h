#pragma once

#if TARGET_PC
#define HEAP_SIZE(original, dusk) (dusk)
#else
#define HEAP_SIZE(original, dusk) (original)
#endif
