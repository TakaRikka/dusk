#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JAHostIO/JAHioNode.h"
#include "JSystem/JAHostIO/JAHioMessage.h"
#include "JSystem/JAHostIO/JAHioMgr.h"
#include "JSystem/JHostIO/JORServer.h"

JAHioNode::JAHioNode(const char* name) : mTree(this) {
    mLastChild = NULL;
    if (name) {
        setNodeName(name);
    } else {
        setNodeName("no named");
    }
}

JAHioNode::~JAHioNode() {
    if (getParent()) {
        getParent()->removeNode(this);
    }
}

void JAHioNode::updateNode() {
    JORMContext* mctx = JORReflexible::getJORServer()->attachMCTX(5);
    mctx->invalidNode(this, 3);
    JORReflexible::getJORServer()->releaseMCTX(mctx);
}

void JAHioNode::setNodeName(const char* name) {
    int size = strlen(name) + 1;
    JUT_ASSERT(51, size < 32);
    strcpy(mName, name);
}

void JAHioNode::genMessage(JORMContext* mctx) {
    JAHControl control(mctx, this);
    message(control);
    if (JAHSingletonBase<JAHioMgr>::getIns()->getNodeSysType() == 0) {
        generateTempChildren(mctx);
    } else {
        generateRealChildren(mctx);
    }
}

void JAHioNode::appendNode(JAHioNode* node, const char* name) {
    mTree.appendChild(&node->mTree);
    JAHSingletonBase<JAHioMgr>::getIns()->appendFrameNode(node);
    if (name) {
        node->setNodeName(name);
    }
    updateNode();
}

void JAHioNode::prependNode(JAHioNode* node, const char* name) {
    mTree.prependChild(&node->mTree);
    JAHSingletonBase<JAHioMgr>::getIns()->appendFrameNode(node);
    if (name) {
        node->setNodeName(name);
    }
    updateNode();
}

void JAHioNode::removeNode(JAHioNode* node) {
    if (smCurrentNode == this) {
        smCurrentNode = NULL;
    }
    mTree.removeChild(&node->mTree);
    JAHSingletonBase<JAHioMgr>::getIns()->removeFrameNode(node);
    updateNode();
}

void JAHioNode::generateRealChildren(JORMContext* mctx) {
    for (JSUTreeIterator<JAHioNode> it(mTree.getFirstChild()); it != mTree.getEndChild(); ++it) {
        JAHioNode* node = it.getObject();
        mctx->startNode(node->mName, node, 4, node->getNodeIcon());
        node->genMessage(mctx);
        mctx->endNode();
    }
}

void JAHioNode::generateTempChildren(JORMContext* mctx) {
    for (JSUTreeIterator<JAHioNode> it(mTree.getFirstChild()); it != mTree.getEndChild(); ++it) {
        JAHioNode* node = it.getObject();
        mctx->genNode(node->mName, node, 4, node->getNodeIcon());
    }
}

u32 JAHioNode::getNodeKind() const { return 0; }

JAHioNode* JAHioNode::getParent() {
    if (mTree.getParent()) {
        return mTree.getParent()->getObject();
    }
    return NULL;
}

void JAHioNode::listenPropertyEvent(const JORPropertyEvent* event) {
    propertyEvent(JAH_P_EVENT0, (u32)event->id);
    JORReflexible::listenPropertyEvent(event);
    propertyEvent(JAH_P_EVENT1, (u32)event->id);
}

void JAHioNode::listenNodeEvent(const JORNodeEvent* event) {
    if (event->field_0x0 == 3) {
        smCurrentNode = this;
        if (getParent()) {
            getParent()->setLastChild(this);
        }
        nodeEvent(JAH_N_EVENT0);
    } else if (event->field_0x0 == 4) {
        nodeEvent(JAH_N_EVENT1);
    } else if (event->field_0x0 == 5) {
        nodeEvent(JAH_N_EVENT2);
    }
}
