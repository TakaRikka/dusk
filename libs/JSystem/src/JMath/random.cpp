#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JMath/random.h"

JMath::TRandom_fast_::TRandom_fast_(u32 seed) {
    setSeed(seed);
}
