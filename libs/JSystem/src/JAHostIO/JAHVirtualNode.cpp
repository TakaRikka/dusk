#include <JSystem/JAHostIO/JAHVirtualNode.h>
#include <JSystem/JUtility/JUTAssert.h>
#include <cstring>

u32 JAHVirtualNode::smVirNodeNum;

void JAHVirtualNode::virtualMessage(JAHControl& control) {
    message(control);
    callAllVirtualMessages(control);
}

void JAHVirtualNode::callAllVirtualMessages(JAHControl& control) {
    if (mTree.getNumChildren() == 0)
        return;

    for (JSUTreeIterator<JAHVirtualNode> iter = mTree.getFirstChild(); iter != mTree.getEndChild();
         ++iter)
    {
        iter->message(control);
    }
}

JAHioNode* JAHVirtualNode::getMaster() {
    JSUTree<JAHVirtualNode>* it = mTree.getParent();
    while (it->getParent())
        it = it->getParent();
    return (JAHioNode*)it->getObject();
}

void JAHVirtualNode::framework() {
    onFrame();
    for (JSUTreeIterator<JAHVirtualNode> iter = getVirTree()->getFirstChild();
         iter != getVirTree()->getEndChild(); ++iter)
    {
        iter->framework();
    }
}

void JAHVirtualNode::onFrame() {}

void JAHVirtualNode::currentFramework() {
    onCurrentNodeFrame();
    for (JSUTreeIterator<JAHVirtualNode> iter = getVirTree()->getFirstChild();
         iter != getVirTree()->getEndChild(); ++iter)
    {
        iter->framework();
    }
}

void JAHVirtualNode::onCurrentNodeFrame() {}

void JAHVirtualNode::listenVirtualPropertyEvent(JAH_P_Event param_1, uintptr_t param_2) {
    propertyEvent(param_1, param_2);
    for (JSUTreeIterator<JAHVirtualNode> iter = mTree.getFirstChild(); iter != mTree.getEndChild();
         ++iter)
    {
        iter->listenVirtualPropertyEvent(param_1, param_2);
    }
}

void JAHVirtualNode::propertyEvent(JAH_P_Event, uintptr_t) {}

void JAHVirtualNode::listenVirtualNodeEvent(JAH_N_Event param_1) {
    nodeEvent(param_1);
    for (JSUTreeIterator<JAHVirtualNode> iter = mTree.getFirstChild(); iter != mTree.getEndChild();
         ++iter)
    {
        iter->listenVirtualNodeEvent(param_1);
    }
}

void JAHVirtualNode::nodeEvent(JAH_N_Event) {}

void JAHVirtualNode::setVirNodeName(const char* name) {
    JUT_ASSERT(139, name);
    size_t size = strlen(name) + 1;
    // clang-format off
    JUT_ASSERT(141, size<32);
    // clang-format on
    strcpy(mName, name);
}

JAHVirtualNode::JAHVirtualNode(const char* name) : mTree(this) {
    if (name)
        setVirNodeName(name);
    else
        setVirNodeName("no name");
    smVirNodeNum += 1;
}

void JAHVirtualNode::updateNode() {
    getMaster()->updateNode();
}
