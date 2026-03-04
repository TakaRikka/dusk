#ifndef JAHIONODE_H
#define JAHIONODE_H

#include "JSystem/JHostIO/JORReflexible.h"
#include "JSystem/JSupport/JSUList.h"

enum JAH_N_Event {
    JAH_N_EVENT0 = 0,
    JAH_N_EVENT1 = 1,
    JAH_N_EVENT2 = 2,
};

enum JAH_P_Event {
    JAH_P_EVENT0 = 0,
    JAH_P_EVENT1 = 1,
};

class JAHControl;

class JAHioNode : public JORReflexible {
public:
    JAHioNode(const char*);
    /* vt[07] */ virtual ~JAHioNode();
    /* vt[02] */ virtual void listenPropertyEvent(const JORPropertyEvent*);
    /* vt[05] */ virtual void genMessage(JORMContext*);
    /* vt[06] */ virtual void listenNodeEvent(const JORNodeEvent*);
    /* vt[08] */ virtual void message(JAHControl&) {}
    /* vt[09] */ virtual void appendNode(JAHioNode*, const char*);
    /* vt[0A] */ virtual void prependNode(JAHioNode*, const char*);
    /* vt[0B] */ virtual void removeNode(JAHioNode*);
    /* vt[0C] */ virtual s32 getNodeType() { return 0; }
    /* vt[0D] */ virtual u32 getNodeKind() const;
    /* vt[0E] */ virtual u32 getNodeIcon() const { return 0; }
    /* vt[0F] */ virtual void propertyEvent(JAH_P_Event, u32) {}
    /* vt[10] */ virtual void nodeEvent(JAH_N_Event) {}
    void updateNode();
    void setNodeName(const char*);
    void generateRealChildren(JORMContext*);
    void generateTempChildren(JORMContext*);
    JAHioNode* getParent();

    static JAHioNode* getCurrentNode() { return smCurrentNode; }

    static JAHioNode* smCurrentNode;

    JSUTree<JAHioNode>* getTree() { return &mTree; }
    char* getNodeName() { return mName; }
    void setLastChild(JAHioNode* node) { mLastChild = node; }

    /* 0x04 */ JSUTree<JAHioNode> mTree;
    /* 0x20 */ char mName[32];
    /* 0x40 */ JAHioNode* mLastChild;
};

#endif /* JAHIONODE_H */
