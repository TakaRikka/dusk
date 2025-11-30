#ifndef JASBASICBANK_H
#define JASBASICBANK_H

#include "JSystem/JAudio2/JASBank.h"
#include "JSystem/JAudio2/JASBasicInst.h"
#include "JSystem/JKernel/JKRHeap.h"

class JASBasicBank : public JASBank {
public:
    JASBasicBank();
    void newInstTable(u8, JKRHeap*);
    bool getInstParam(int, int, int, JASInstParam*) const;
    void setInst(int, JASInst*);
    JASInst* getInst(int) const;
    ~JASBasicBank() {}
    u32 getType() const { return 'BSIC'; }

    /* 0x8 */ JASInst** mInstTable;
    /* 0xC */ u8 mInstNumMax;
};

#endif /* JASBASICBANK_H */
