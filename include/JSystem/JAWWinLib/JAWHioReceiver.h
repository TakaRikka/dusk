#ifndef JAWHIORECEIVER_H
#define JAWHIORECEIVER_H

#include "JSystem/JAWExtSystem/JAWWindow.h"
#include "JSystem/JHostIO/JHIComm.h"
#include "JSystem/JHostIO/JHICommonMem.h"
#include "JSystem/JAudio2/JAISound.h"

class JAISeqData;

class JADHioReceiver : JHITag<JHICmnMem> {
public:
    JADHioReceiver();
    virtual ~JADHioReceiver();
    virtual void receive(const char*, s32);
    virtual u32 parse(u32,char*, u32) = 0;
};


class JAWHioReceiver : public JAWWindow {
public:
    class TSeqList {
    public:
        class TSeqData {
        public:
        };

        bool getSeqData(JAISoundID, JAISeqData*) const;

        /* 0x00 */ JSUList<TSeqData> mList;
    };

    class THioReceiver : public JADHioReceiver {
    public:
        THioReceiver();
        virtual ~THioReceiver();
        virtual u32 parse(u32,char*, u32);

        /* 0x0C */ int field_0xc;
        /* 0x10 */ u8 field_0x10;
        /* 0x14 */ TSeqList field_0x14;
        /* 0x20 */ int field_0x20;
        /* 0x24 */ int field_0x24;
        /* 0x28 */ int field_0x28;
        /* 0x2C */ int field_0x2c;
    };

    JAWHioReceiver();

    const TSeqList* getSeqList() const { return &field_0x3f0.field_0x14; }

    /* 0x3F0 */ THioReceiver field_0x3f0;
};

#endif /* JAWHIORECEIVER_H */
