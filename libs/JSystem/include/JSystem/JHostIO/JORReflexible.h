#ifndef JORREFLEXIBLE_H
#define JORREFLEXIBLE_H

#include <types.h>
#include "JSystem/JHostIO/JORServer.h"

struct JOREvent;
struct JORPropertyEvent;
struct JORGenEvent;
struct JORNodeEvent;

class JORMContext;
class JORServer;

// NOTE (stable game ABI): these classes stay non-polymorphic outside DEBUG
// on purpose. Making them polymorphic under PARTIAL_DEBUG would give every one of the ~250
// derived HIO classes a vptr and turn their plain `void genMessage(JORMContext*);`
// declarations into implicit virtual overrides whose definitions are #if DEBUG-gated; every
// instantiated one then fails to link (missing vtable). Closure types shared with DEBUG TUs
// either declare their own unconditional virtuals (vptr in all TUs anyway) or add a
// PARTIAL_DEBUG-only virtual dtor for vptr parity (see dAttParam_c).
class JOREventListener {
public:
#if DEBUG
    JOREventListener() {}
#if TARGET_PC
    virtual void listenPropertyEvent(const JORPropertyEvent*) {}
#else
    virtual void listenPropertyEvent(const JORPropertyEvent*) = 0;
#endif
#endif
};

class JORReflexible : public JOREventListener {
public:
    static JORServer* getJORServer() { return JORServer::getInstance(); }

#if DEBUG
    JORReflexible() {}

    virtual void listenPropertyEvent(const JORPropertyEvent*);
    virtual void listen(u32, const JOREvent*);
    virtual void genObjectInfo(const JORGenEvent*);
#if TARGET_PC
    virtual void genMessage(JORMContext*) {}
#else
    virtual void genMessage(JORMContext*) = 0;
#endif
    virtual void listenNodeEvent(const JORNodeEvent*);
#endif
};

#endif /* JORREFLEXIBLE_H */
