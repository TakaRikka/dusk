#ifndef JAHVIRTUALNODE_H
#define JAHVIRTUALNODE_H

#include "JSystem/JAHostIO/JAHioNode.h"
#include "JSystem/JSupport/JSUList.h"

class JAHControl;

class JAHVirtualNode {
public:
    JAHVirtualNode(const char*);

    virtual void updateNode();
    virtual void message(JAHControl&) {}
    virtual void onFrame();
    virtual void onCurrentNodeFrame();
    virtual void propertyEvent(JAH_P_Event, uintptr_t);
    virtual void nodeEvent(JAH_N_Event);
    virtual void virtualMessage(JAHControl&);

    void callAllVirtualMessages(JAHControl&);
    JAHioNode* getMaster();
    void framework();
    void currentFramework();
    void listenVirtualPropertyEvent(JAH_P_Event, uintptr_t);
    void listenVirtualNodeEvent(JAH_N_Event);
    void setVirNodeName(const char*);

    JSUTree<JAHVirtualNode>* getVirTree() { return &mTree; }
    static u32 getVirNodeNum() { return smVirNodeNum; }

    static u32 smVirNodeNum;

    /* 0x04 */ JSUTree<JAHVirtualNode> mTree;
    /* 0x20 */ char mName[32];
};

#endif /* JAHVIRTUALNODE_H */
