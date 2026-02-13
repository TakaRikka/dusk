#ifndef J3DUD_H
#define J3DUD_H

#include "dolphin/types.h"
#ifndef __MWERKS__
#include <math.h>
#endif

namespace J3DUD {
inline f32 JMAAbs(f32 x) {
#ifdef __MWERKS__
    return __fabsf(x);
#else
    return fabsf(x);
#endif
}
}  // namespace J3DUD

#endif /* J3DUD_H */
