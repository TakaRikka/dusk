#include "dusk/endian.h"
#include <dolphin/gx.h>
#include "SSystem/SComponent/c_xyz.h"
#include "dusk/endian_gx.hpp"

#define IMPL_ENUM(type) \
    template <> \
    type BE<type>::swap(type val) { \
        return static_cast<type>(be32(val)); \
    }

IMPL_ENUM(GXCullMode);
IMPL_ENUM(GXAttr);
IMPL_ENUM(GXAttrType);
IMPL_ENUM(GXCompType);
IMPL_ENUM(GXCompCnt);

template <>
GXColorS10 BE<GXColorS10>::swap(GXColorS10 val) {
    return {
        be16s(val.r),
        be16s(val.g),
        be16s(val.b),
        be16s(val.a),
    };
}

GXVtxDescList BE<GXVtxDescList>::swap(GXVtxDescList val) {
    return {
        BE<GXAttr>::swap(val.attr),
        BE<GXAttrType>::swap(val.type),
    };
}

GXVtxAttrFmtList BE<GXVtxAttrFmtList>::swap(GXVtxAttrFmtList val) {
    return {
        BE<GXAttr>::swap(val.attr),
        BE<GXCompCnt>::swap(val.cnt),
        BE<GXCompType>::swap(val.type),
        val.frac
    };
}

template<>
cXy BE<cXy>::swap(cXy val) {
    return {
        BE<f32>::swap(val.x),
        BE<f32>::swap(val.y),
    };
}
