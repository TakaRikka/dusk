#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JAudio2/JAUAudioArcInterpreter.h"
#include "JSystem/JUtility/JUTAssert.h"

JAUAudioArcInterpreter::JAUAudioArcInterpreter() {
    mBase = NULL;
    mReadPtr = NULL;
}

JAUAudioArcInterpreter::~JAUAudioArcInterpreter() {}

bool JAUAudioArcInterpreter::parse(void const* param_0) {
    mBase = (const u8*)param_0;
    mReadPtr = mBase;
    if (readU32_() != 'AA_<') {
        return false;
    }
    while (readCommand_()) {}
    return true;
}

bool JAUAudioArcInterpreter::readCommandMore(u32 param_0) {
    return false;
}

bool JAUAudioArcInterpreter::readCommand_() {
    u32 cmd = readU32_();
    switch (cmd) {
    case '>_AA':
        return false;
    case 'ws  ': {
        u32 var1 = readU32_();
        u32 var2 = readU32_();
        u32 var3 = readU32_();
        readWS(var1, getContent_(var2), var3);
        break;
    }
    case 'bnk ': {
        u32 var1 = readU32_();
        u32 var2 = readU32_();
        readBNK(var1, getContent_(var2));
        break;
    }
    case 'bl_<': {
        u32 var1 = readU32_();
        u32 var2 = readU32_();
        beginBNKList(var1, var2);
        break;
    }
    case '>_bl':
        endBNKList();
        break;
    case 'bsc ': {
        u32 var1 = readU32_();
        u32 var2 = readU32_();
        readBSC(getContent_(var1), var2 - var1);
        break;
    }
    case 'bst ': {
        u32 var1 = readU32_();
        u32 var2 = readU32_();
        readBST(getContent_(var1), var2 - var1);
        break;
    }
    case 'bstn': {
        u32 var1 = readU32_();
        u32 var2 = readU32_();
        readBSTN(getContent_(var1), var2 - var1);
        break;
    }
    case 'bms ': {
        u32 var1 = readU32_();
        u32 var2 = readU32_();
        u32 var3 = readU32_();
        readBMS(var1, getContent_(var2), var3 - var2);
        break;
    }
    case 'bmsa': {
        u32 var1 = readU32_();
        readBMS_fromArchive(var1);
        break;
    }
    case 'vbnk': {
        u32 var1 = readU32_();
        u32 var2 = readU32_();
        newVoiceBank(var1, var2);
        break;
    }
    case 'dsqb': {
        u32 var1 = readU32_();
        newDynamicSeqBlock(var1);
        break;
    }
    case 'bsft': {
        u32 var1 = readU32_();
        readBSFT(getContent_(var1));
        break;
    }
    case 'sect': {
        u8 var1 = readU8_();
        u8 var2 = readU8_();
        u8 var3 = readU8_();
        u8 var4 = readU8_();
        readMaxSeCategory(var2, var3, var4);
        break;
    }
    default:
        if (!readCommandMore(cmd)) {
            JUT_ASSERT(145, false);
        }
        break;
    }
    return true;
}
