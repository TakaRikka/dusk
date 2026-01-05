#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JAudio2/JASSeqCtrl.h"
#include "JSystem/JAudio2/JASSeqParser.h"
#include "JSystem/JAudio2/JASTrack.h"

JASSeqParser JASSeqCtrl::sDefaultParser;

JASSeqCtrl::JASSeqCtrl() {
    field_0x3c = &sDefaultParser;
    field_0x40 = 0;
    field_0x44 = NULL;
    field_0x48 = 0;
    field_0x4c = 0;
    field_0x4e = 0;
    field_0x50 = 0;
    field_0x51 = 0;
    field_0x54 = 0;
    field_0x58 = 0;
    mReader.init();
}

void JASSeqCtrl::init() {
    mReader.init();
    field_0x3c = &sDefaultParser;
    field_0x40 = 0;
    field_0x44 = NULL;
    field_0x48 = 0;
    field_0x4c = 0;
    field_0x4e = 0;
    field_0x50 = 0;
    field_0x54 = 0;
    field_0x58 = 0;
    field_0x51 = 0;
}

void JASSeqCtrl::start(void* param_0, u32 param_1) {
    mReader.init(param_0);
    mReader.jump(param_1);
}

int JASSeqCtrl::tickProc(JASTrack* param_0) {
    if (!mReader.getBase()) {
        return 0;
    }

    interrupt(INTRTYPE_VALUE_6);
    timerProcess();

    if (field_0x51) {
        if (!param_0->checkNoteStop(0)) {
            return 0;
        }

        field_0x51 = 0;
    }

    if (field_0x40 > 0) {
        field_0x40--;
    }

    checkIntr();

    while ((field_0x40 == 0 || field_0x44 != NULL) && field_0x51 == 0) {
        if (field_0x3c->parse(param_0) < 0) {
            return -1;
        }
    }
    
    return 0;
}

void JASSeqCtrl::interrupt(JASSeqCtrl::IntrType param_0) {
    if (field_0x4e & (1 << param_0)) {
        field_0x4c |= (1 << param_0);
    }
}

void JASSeqCtrl::setIntrMask(u32 param_0) {
    field_0x4e |= param_0;
}

void JASSeqCtrl::clrIntrMask(u32 param_0) {
    field_0x4e &= ~param_0;
}

int JASSeqCtrl::retIntr() {
    if (field_0x44 == NULL) {
        return 0;
    }

    mReader.jump(field_0x44);
    field_0x44 = NULL;
    return 1;
}

int JASSeqCtrl::findIntr() {
    u32 uVar1 = field_0x4e;
    uVar1 &= field_0x4c;
    int i;
    for (i = 0; uVar1 != 0; i++) {
        if (uVar1 & 1) {
            field_0x4c &= ~(1 << i);
            return i;
        }
        uVar1 >>= 1;
    }
    return -1;
}

void JASSeqCtrl::checkIntr() {
    if (field_0x44) {
        return;
    }
    int intr = findIntr();

    if (intr < 0) {
        return;
    }

    u32 vec = field_0x48;
    JUT_ASSERT(144, vec != 0);
    vec += intr * 3;
    vec = get24(vec);
    field_0x44 = mReader.getCur();
    mReader.jump(vec);
}

void JASSeqCtrl::timerProcess() {
    if (field_0x54 == 0) return;
    
    field_0x54--;
    if (field_0x54) return;
    
    interrupt(INTRTYPE_VALUE_5);

    if (field_0x50) {
        field_0x50--;
        if (field_0x50) {
            field_0x54 = field_0x58;
        }
    } else {
        field_0x54 = field_0x58;
    }
}
