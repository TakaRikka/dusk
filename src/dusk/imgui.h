#pragma once

#include <aurora/aurora.h>

#ifdef __cplusplus
extern "C"
{
#endif

    void imgui_main(const AuroraInfo* info);
    void frame_limiter();

#ifdef __cplusplus
}
#endif
