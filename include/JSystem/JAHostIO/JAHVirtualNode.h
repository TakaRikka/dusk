#ifndef JAHVIRTUALNODE_H
#define JAHVIRTUALNODE_H

#include "JSystem/JAHostIO/JAHioNode.h"
#include "JSystem/JSupport/JSUList.h"

class JAHControl;

class JAHVirtualNode {
public:
    JAHVirtualNode();
    virtual void updateNode();
    virtual void message(JAHControl&);
    virtual void onFrame();
    virtual void onCurrentNodeFrame();
    virtual void propertyEvent(JAH_P_Event, u32);
    virtual void nodeEvent(JAH_N_Event);

    /* 0x04 */ JSUTree<JAHVirtualNode> mTree;
    /* 0x20 */ char mName[32];
};

#endif /* JAHVIRTUALNODE_H */
