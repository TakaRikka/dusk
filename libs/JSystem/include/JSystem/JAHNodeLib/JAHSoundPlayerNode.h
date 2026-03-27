#ifndef JAHSOUNDPLAYERNODE_H
#define JAHSOUNDPLAYERNODE_H

#include "JSystem/JAHostIO/JAHFrameNode.h"
#include "JSystem/JAHostIO/JAHUAsnData.h"
#include "JSystem/JAHostIO/JAHUTableEdit.h"

class JAHPlayBox : public JAHUSeBox {
public:
    JAHPlayBox();

    virtual void onCurrentNodeFrame();

public:
    /* 0x85 */ u8 unk85;
};

class JAHSoundPlayerNode : public JAHFrameNode {
public:
    JAHSoundPlayerNode();

    virtual void message(JAHControl&);
    virtual void onCurrentNodeFrame();

    u8 getContMode() const;

public:
    /* 0x70 */ s32 unk70;
    /* 0x74 */ s32 unk74;
    /* 0x78 */ s32 unk78;
    /* 0x7C */ u8 unk7C;
    /* 0x7D */ u8 unk7D;
    /* 0x7E */ u8 unk7E;
    /* 0x7F */ u8 unk7F;
    /* 0x80 */ s32 unk80;
    /* 0x84 */ s32 unk84;
    /* 0x88 */ s64 unk88;
    /* 0x90 */ s64 unk90;
    /* 0x98 */ JAHPlayBox unk98;
    /* 0x120 */ bool unk120;
    /* 0x122 */ JAHUAsn unk122;
};

#endif /* JAHSOUNDPLAYERNODE_H */
