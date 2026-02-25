#include <dolphin/gx.h>
#include "dusk/endian.h"

template <>
GXColorS10 BE<GXColorS10>::swap(GXColorS10 val) {
    return {
        be16s(val.r),
        be16s(val.g),
        be16s(val.b),
        be16s(val.a),
    };
}

#define IMPL_ENUM(type) \
    template <> \
    type BE<type>::swap(type val) { \
        return static_cast<type>(be32(val)); \
    }

IMPL_ENUM(GXCullMode);
IMPL_ENUM(GXAttr);
IMPL_ENUM(GXAttrType);
IMPL_ENUM(GXCompType);
