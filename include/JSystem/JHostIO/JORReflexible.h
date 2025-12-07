#ifndef JORREFLEXIBLE_H
#define JORREFLEXIBLE_H

#include <dolphin/types.h>

class JORReflexible;

struct JOREvent {};

struct JORPropertyEvent : JOREvent {
    enum EKind {
        EKind_HasListener = (1 << 30),
        EKind_ValueID     = (1 << 29),
        EKind_FloatValue  = (1 << 9),
    };
    
    /* 0x00 */ u8 field_0x0[0x4 - 0x0];
    /* 0x04 */ u32 type;
    /* 0x08 */ u32 kind;
    /* 0x0C */ char* id; // id?
    /* 0x10 */ JORReflexible* field_0x10;  // ?
    /* 0x14 */ u32 field_0x14;
    union {
        u32 U32;
        u16 U16[2];
    }
    /* 0x18 */ field_0x18;
};

struct JORGenEvent : JOREvent {};
struct JORNodeEvent : JOREvent {};

struct JORMContext;
struct JORServer;

class JOREventListener {
public:
#if DEBUG
    virtual void listenPropertyEvent(const JORPropertyEvent*) = 0;
#endif
};

class JORReflexible : public JOREventListener {
public:
#if DEBUG
    JORReflexible();
    static JORServer* getJORServer();
    
    virtual void listenPropertyEvent(const JORPropertyEvent*);
    virtual void listen(u32, const JOREvent*);
    virtual void genObjectInfo(const JORGenEvent*);
    virtual void genMessage(JORMContext*) = 0;
    virtual void listenNodeEvent(const JORNodeEvent*);
#endif
};

#endif /* JORREFLEXIBLE_H */
