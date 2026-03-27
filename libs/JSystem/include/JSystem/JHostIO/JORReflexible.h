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

class JOREventListener {
public:
#if DEBUG
    JOREventListener() {}
    virtual void listenPropertyEvent(const JORPropertyEvent*) = 0;
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
    virtual void genMessage(JORMContext*) = 0;
    virtual void listenNodeEvent(const JORNodeEvent*);
#endif
};

#endif /* JORREFLEXIBLE_H */
