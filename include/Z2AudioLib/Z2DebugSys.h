#ifndef Z2DEBUGSYS_H
#define Z2DEBUGSYS_H

#include "JSystem/JAudio2/JAISeqDataMgr.h"
#include "JSystem/JAudio2/JASGadget.h"
#include "JSystem/JAudio2/JAUSeqCollection.h"
#include "JSystem/JAHostIO/JAHFrameNode.h"
#include "JSystem/JAHostIO/JAHioMessage.h"
#include "JSystem/JAWWinLib/JAWHioReceiver.h"

class Z2ParamNode : public JAHFrameNode {
public:
    Z2ParamNode() : JAHFrameNode("サウンド各種パラメータ") {}

    /* vt[07] */ virtual ~Z2ParamNode() {}
    /* vt[08] */ virtual void message(JAHControl& control);
    /* vt[0F] */ virtual void propertyEvent(JAH_P_Event, u32);
    /* vt[12] */ virtual void onFrame();
};

class Z2HioSeSeqDataMgr : public JAUSeqDataMgr_SeqCollection {
public:
    Z2HioSeSeqDataMgr(const void * param_1, const JAWHioReceiver* param_2) {
        field_0x18 = param_2;
        init(param_1);
    };
    virtual ~Z2HioSeSeqDataMgr() {}
    virtual SeqDataReturnValue getSeqData(JAISoundID param_1, JAISeqData* param_2) {
        if (field_0x18->getSeqList()->getSeqData(param_1, param_2)) {
            param_2->field_0x4 = 4;
            return SeqDataReturnValue_2;
        } else {
            return JAUSeqDataMgr_SeqCollection::getSeqData(param_1, param_2);
        }
    }

    /* 0x18 */ const JAWHioReceiver * field_0x18;
};

struct Z2DebugSys : public JASGlobalInstance<Z2DebugSys> {
    Z2DebugSys();
    ~Z2DebugSys() {}

    void initJAW();
    void initSoundHioNode();
    void debugframework();
    JAISeqDataMgr* initSeSeqDataMgr(const void*);
};

#endif
