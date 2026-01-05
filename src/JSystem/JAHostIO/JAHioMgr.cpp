#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JAHostIO/JAHioMgr.h"
#include "JSystem/JAHostIO/JAHFrameNode.h"
#include "JSystem/JHostIO/JORServer.h"

JAHioMgr::JAHioMgr() : field_0x4(0), field_0x8(0) {}

void JAHioMgr::init_OnGame() {
    field_0x4 = 0;
}

bool JAHioMgr::isGameMode() {
    return field_0x4 == 0;
}

void JAHioMgr::appendRootNode(JORReflexible* param_1, JAHioNode* node) {
    JUT_ASSERT(44, isGameMode());
    JORMContext* mctx = JORAttachMContext(12);
    mctx->genNode(param_1, 1, node->getNodeName(), node, 0, 0);
    JORReleaseMContext(mctx);
    appendFrameNode(node);
}

void JAHioMgr::appendFrameNode(JAHioNode* node) {
    if (node->getNodeType() == 1) {
        field_0xc.append(((JAHFrameNode*)node)->getFrameNodeLink());
    }
}

void JAHioMgr::removeFrameNode(JAHioNode* node) {
    if (node->getNodeType() == 1) {
        field_0xc.remove(((JAHFrameNode*)node)->getFrameNodeLink());
    }
    JSUTreeIterator<JAHioNode> it;
    for (it = node->getTree()->getFirstChild(); it != node->getTree()->getEndChild(); ++it) {
        removeFrameNode(*it);
    }
}

u32 JAHioMgr::framework() {
    JSUListIterator<JAHFrameNode> it;
    for (it = field_0xc.getFirst(); it != field_0xc.getEnd(); ++it) {
        it->framework();
    }
    JAHioNode* node = JAHioNode::getCurrentNode();
    if (node && node->getNodeType() == 1) {
        ((JAHFrameNode*)node)->currentFramework();
    }
    u32 r30 = 0;
    if (field_0x4 == 1) {
        r30 = JOR_MESSAGELOOP();
    }
    return r30;
}
