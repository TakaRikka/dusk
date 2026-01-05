#ifndef JAHIOMGR_H
#define JAHIOMGR_H

#include "JSystem/JAHostIO/JAHPubDefine.h"
#include "JSystem/JSupport/JSUList.h"

class JAHFrameNode;
class JAHioNode;
class JORReflexible;

class JAHioMgr : public JAHSingletonBase<JAHioMgr> {
public:
    JAHioMgr();
    void init_OnGame();
    bool isGameMode();
    void appendRootNode(JORReflexible*, JAHioNode*);
    void appendFrameNode(JAHioNode*);
    void removeFrameNode(JAHioNode*);
    u32 framework();

    u32 getNodeSysType() { return field_0x8; }

    /* 0x04 */ s32 field_0x4;
    /* 0x08 */ u32 field_0x8;
    JSUList<JAHFrameNode> field_0xc;
};

#endif /* JAHIOMGR_H */
