#include "d/dolzel.h"  // IWYU pragma: keep

#include "d/d_bg_w_sv.h"

// need to pull in dBgWSv RTTI somehow but not sure if this is correct
class dBgWHf : public dBgWSv {
};

static void dummy() {
    dBgWHf dummy;
}
