#include <JSystem/JAHostIO/JAHFrameNode.h>
#include <JSystem/JAHostIO/JAHVirtualNode.h>
#include <JSystem/JAHostIO/JAHioMessage.h>
#include <JSystem/JAHostIO/JAHioMgr.h>

JAHFrameNode::JAHFrameNode(const char* name)
    : JAHioNode(name), mTree((JAHVirtualNode*)this), mFrameNodeLink(this) {}

void JAHFrameNode::genMessage(JORMContext* context) {
    JAHControl control(context, this);
    callAllVirtualMessages(control);
    message(control);
    if (JAHSingletonBase<JAHioMgr>::getIns()->getNodeSysType() == 0)
        generateTempChildren(context);
    else
        generateRealChildren(context);
}

void JAHFrameNode::callAllVirtualMessages(JAHControl& control) {
    if (mTree.getNumChildren() == 0)
        return;

    for (JSUTreeIterator<JAHVirtualNode> iter = mTree.getFirstChild(); iter != mTree.getEndChild();
         ++iter)
    {
        iter->message(control);
    }
}

void JAHFrameNode::appendVirNode(JAHVirtualNode* node) {
    mTree.appendChild(node->getVirTree());
}

void JAHFrameNode::listenPropertyEvent(const JORPropertyEvent* event) {
    for (JSUTreeIterator<JAHVirtualNode> iter = mTree.getFirstChild(); iter != mTree.getEndChild();
         ++iter)
    {
        iter->listenVirtualPropertyEvent(JAH_P_EVENT0, (uintptr_t)event->id);
    }

    JAHioNode::listenPropertyEvent(event);

    for (JSUTreeIterator<JAHVirtualNode> iter = mTree.getFirstChild(); iter != mTree.getEndChild();
         ++iter)
    {
        iter->listenVirtualPropertyEvent(JAH_P_EVENT1, (uintptr_t)event->id);
    }
}

void JAHFrameNode::listenNodeEvent(const JORNodeEvent* event) {
    JAHioNode::listenNodeEvent(event);
    if (event->field_0x0 == 3) {
        for (JSUTreeIterator<JAHVirtualNode> iter = mTree.getFirstChild();
             iter != mTree.getEndChild(); ++iter)
            iter->listenVirtualNodeEvent(JAH_N_EVENT0);
    } else if (event->field_0x0 == 4) {
        for (JSUTreeIterator<JAHVirtualNode> iter = mTree.getFirstChild();
             iter != mTree.getEndChild(); ++iter)
            iter->listenVirtualNodeEvent(JAH_N_EVENT1);
    } else if (event->field_0x0 == 5) {
        for (JSUTreeIterator<JAHVirtualNode> iter = mTree.getFirstChild();
             iter != mTree.getEndChild(); ++iter)
            iter->listenVirtualNodeEvent(JAH_N_EVENT2);
    }
}

void JAHFrameNode::framework() {
    onFrame();
    if (mTree.getNumChildren() != 0) {
        int i = 0;  // unused?
        for (JSUTreeIterator<JAHVirtualNode> iter = mTree.getFirstChild();
             iter != mTree.getEndChild(); ++iter)
        {
            iter->framework();
        }
    }
}

void JAHFrameNode::onFrame() {}

void JAHFrameNode::currentFramework() {
    onCurrentNodeFrame();
    if (mTree.getNumChildren() != 0) {
        int i = 0;  // unused?
        for (JSUTreeIterator<JAHVirtualNode> iter = mTree.getFirstChild();
             iter != mTree.getEndChild(); ++iter)
        {
            iter->currentFramework();
        }
    }
}

void JAHFrameNode::onCurrentNodeFrame() {}
