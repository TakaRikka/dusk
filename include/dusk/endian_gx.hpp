#pragma once

#include "dolphin/gx/GXStruct.h"
#include "endian.h"

template <>
struct BE<GXVtxDescList> {
    BE<GXAttr> attr;
    BE<GXAttrType> type;

    static GXVtxDescList swap[[nodiscard]](GXVtxDescList val);
};

template <>
struct BE<GXVtxAttrFmtList> {
    BE<GXAttr> attr;
    BE<GXCompCnt> cnt;
    BE<GXCompType> type;
    u8 frac;

    static GXVtxAttrFmtList swap[[nodiscard]](GXVtxAttrFmtList val);
};
