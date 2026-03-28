#ifndef JAHUTABLEDIT_H
#define JAHUTABLEDIT_H

#include "JSystem/JAHostIO/JAHVirtualNode.h"
#include "JSystem/JAudio2/JAISound.h"

class JAHUAsn;

class JAHUSeBox : public JAHVirtualNode {
public:
    JAHUSeBox(u32, u32*);

    virtual void message(JAHControl&);
    virtual void onCurrentNodeFrame();
    virtual void propertyEvent(JAH_P_Event, uintptr_t);
    virtual void nodeEvent(JAH_N_Event);
    virtual void trigPlayButton();
    virtual void trigStopButton();
    virtual void soundPlay();
    virtual void soundStop();

    u8 checkNameSetChange();
    void updateNameSet(JORReflexible*);
    void setId(u32, bool, JORReflexible*);
    u32 getId() const { return unk58; }

public:
    /* 0x40 */ JAISoundHandle unk40;
    /* 0x44 */ JAISoundHandle unk44;
    /* 0x48 */ bool unk48;
    /* 0x4C */ u32 unk4C;
    /* 0x50 */ u32 unk50;
    /* 0x54 */ u32 unk54;
    /* 0x58 */ u32 unk58;
    /* 0x5C */ u32* unk5C;
    /* 0x60 */ JAHUAsn* mAsnCalc;
    /* 0x64 */ s32 unk64;
    /* 0x68 */ s32 unk68;
    /* 0x6C */ s32 unk6C;
    /* 0x70 */ s32 unk70;
    /* 0x74 */ s32 unk74;
    /* 0x78 */ s32 unk78;
    /* 0x7C */ s32 unk7C;
    /* 0x80 */ s32 unk80;
    /* 0x84 */ bool unk84;
};

#endif /* JAHUTABLEDIT_H */
