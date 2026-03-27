#ifndef JOREVENT_H
#define JOREVENT_H

#include "types.h"

class JORReflexible;

struct JOREvent {};

struct JORPropertyEvent : JOREvent {
    enum EKind {
        EKind_HasListener = (1 << 30),
        EKind_ValueID = (1 << 29),
        EKind_FloatValue = (1 << 9),
    };

    /* 0x00 */ u8 field_0x0[0x4 - 0x0];
    /* 0x04 */ u32 type;
    /* 0x08 */ u32 kind;
    /* 0x0C */ char* id;                   // id?
    /* 0x10 */ JORReflexible* field_0x10;  // ?
    /* 0x14 */ u32 field_0x14;
    union {
        u32 U32;
        u16 U16[2];
    }
    /* 0x18 */ field_0x18;
};

struct JORGenEvent : JOREvent {};

struct JORNodeEvent : JOREvent {
    /* 0x00 */ u32 field_0x0;
};

#endif /* JOREVENT_H */
