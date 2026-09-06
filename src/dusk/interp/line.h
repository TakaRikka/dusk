#pragma once

#include "dolphin/types.h"

class mDoExt_3Dline_c;

namespace dusk::interp::line {

struct Points {
    mDoExt_3Dline_c* strands;
    u16 strand_count;
    u16 point_count;
};

void reset(const void* owner);
void capture(const void* owner, Points points);
void write(const void* owner, Points points);

}  // namespace dusk::interp::line
