#ifndef JAHFRAMENODE_H
#define JAHFRAMENODE_H

#include "dolphin/types.h"
#include "JSystem/JAHostIO/JAHioNode.h"

class JAHVirtualNode;

class JAHFrameNode : public JAHioNode {
public:
    JAHFrameNode(const char*);
    /* vt[07] */ virtual ~JAHFrameNode();
    /* vt[02] */ virtual void listenPropertyEvent(const JORPropertyEvent*);
    /* vt[05] */ virtual void genMessage(JORMContext*);
    /* vt[06] */ virtual void listenNodeEvent(const JORNodeEvent*);
    /* vt[0C] */ virtual s32 getNodeType();
    /* vt[11] */ virtual void onCurrentNodeFrame();
    /* vt[12] */ virtual void onFrame();

    void framework();
    void currentFramework();

    JSULink<JAHFrameNode>* getFrameNodeLink() { return &mFrameNodeLink; }

    /* 0x44 */ JSUTree<JAHVirtualNode> mTree;
    /* 0x60 */ JSULink<JAHFrameNode> mFrameNodeLink;
};

#endif /* JAHFRAMENODE_H */
