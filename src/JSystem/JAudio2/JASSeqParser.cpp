#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JAudio2/JASSeqParser.h"
#include "JSystem/JAudio2/JASCalc.h"
#include "JSystem/JAudio2/JASReport.h"
#include "JSystem/JAudio2/JASTrack.h"
#include "JSystem/JMath/random.h"
#include "JSystem/JSupport/JSupport.h"
#include "JSystem/JUtility/JUTAssert.h"


JASSeqParser::CmdInfo JASSeqParser::sCmdInfo[96] = {
    NULL, 0x0000, 0x0000,
    NULL, 0x0000, 0x0000,
    NULL, 0x0000, 0x0000,
    NULL, 0x0000, 0x0000,
    NULL, 0x0000, 0x0000,
    NULL, 0x0000, 0x0000,
    NULL, 0x0000, 0x0000,
    NULL, 0x0000, 0x0000,
    NULL, 0x0000, 0x0000,
    NULL, 0x0000, 0x0000,
    NULL, 0x0000, 0x0000,
    NULL, 0x0000, 0x0000,
    NULL, 0x0000, 0x0000,
    NULL, 0x0000, 0x0000,
    NULL, 0x0000, 0x0000,
    NULL, 0x0000, 0x0000,
    NULL, 0x0000, 0x0000,
    &JASSeqParser::cmdNoteOn, 0x0003, 0x0000,
    &JASSeqParser::cmdNoteOff, 0x0001, 0x0000,
    &JASSeqParser::cmdNote, 0x0004, 0x0040,
    &JASSeqParser::cmdSetLastNote, 0x0001, 0x0000,
    NULL, 0x0000, 0x0000,
    NULL, 0x0000, 0x0000,
    NULL, 0x0000, 0x0000,
    &JASSeqParser::cmdParamE, 0x0002, 0x0000,
    &JASSeqParser::cmdParamI, 0x0002, 0x0004,
    &JASSeqParser::cmdParamEI, 0x0003, 0x0010,
    &JASSeqParser::cmdParamII, 0x0003, 0x0014,
    NULL, 0x0000, 0x0000,
    NULL, 0x0000, 0x0000,
    NULL, 0x0000, 0x0000,
    NULL, 0x0000, 0x0000,
    NULL, 0x0000, 0x0000,
    &JASSeqParser::cmdOpenTrack, 0x0002, 0x0008,
    &JASSeqParser::cmdCloseTrack, 0x0001, 0x0000,
    &JASSeqParser::cmdCall, 0x0001, 0x0002,
    &JASSeqParser::cmdCallF, 0x0002, 0x0008,
    &JASSeqParser::cmdRet, 0x0000, 0x0000,
    &JASSeqParser::cmdRetF, 0x0001, 0x0000,
    &JASSeqParser::cmdJmp, 0x0001, 0x0002,
    &JASSeqParser::cmdJmpF, 0x0002, 0x0008,
    &JASSeqParser::cmdJmpTable, 0x0002, 0x000B,
    &JASSeqParser::cmdCallTable, 0x0002, 0x000B,
    &JASSeqParser::cmdLoopS, 0x0001, 0x0001,
    &JASSeqParser::cmdLoopE, 0x0000, 0x0000,
    NULL, 0x0000, 0x0000,
    NULL, 0x0000, 0x0000,
    NULL, 0x0000, 0x0000,
    &JASSeqParser::cmdReadPort, 0x0002, 0x0000,
    &JASSeqParser::cmdWritePort, 0x0002, 0x000C,
    &JASSeqParser::cmdCheckPortImport, 0x0001, 0x0000,
    &JASSeqParser::cmdCheckPortExport, 0x0001, 0x0000,
    &JASSeqParser::cmdParentWritePort, 0x0002, 0x000C,
    &JASSeqParser::cmdChildWritePort, 0x0002, 0x000C,
    &JASSeqParser::cmdParentReadPort, 0x0002, 0x0000,
    &JASSeqParser::cmdChildReadPort, 0x0002, 0x0000,
    &JASSeqParser::cmdRegLoad, 0x0002, 0x0004,
    &JASSeqParser::cmdReg, 0x0003, 0x0030,
    &JASSeqParser::cmdReg, 0x0003, 0x0010,
    &JASSeqParser::cmdRegUni, 0x0002, 0x0000,
    &JASSeqParser::cmdRegTblLoad, 0x0004, 0x00E0,
    NULL, 0x0000, 0x0000,
    NULL, 0x0000, 0x0000,
    NULL, 0x0000, 0x0000,
    &JASSeqParser::cmdTempo, 0x0001, 0x0001,
    &JASSeqParser::cmdBankPrg, 0x0001, 0x0001,
    &JASSeqParser::cmdBank, 0x0001, 0x0000,
    &JASSeqParser::cmdPrg, 0x0001, 0x0000,
    NULL, 0x0000, 0x0000,
    NULL, 0x0000, 0x0000,
    NULL, 0x0000, 0x0000,
    &JASSeqParser::cmdEnvScaleSet, 0x0002, 0x0004,
    &JASSeqParser::cmdEnvSet, 0x0002, 0x0008,
    &JASSeqParser::cmdSimpleADSR, 0x0005, 0x0155,
    &JASSeqParser::cmdBusConnect, 0x0002, 0x0004,
    &JASSeqParser::cmdIIRCutOff, 0x0001, 0x0000,
    &JASSeqParser::cmdIIRSet, 0x0004, 0x0055,
    &JASSeqParser::cmdFIRSet, 0x0001, 0x0002,
    NULL, 0x0000, 0x0000,
    NULL, 0x0000, 0x0000,
    &JASSeqParser::cmdWait, 0x0000, 0x0000,
    &JASSeqParser::cmdWaitByte, 0x0001, 0x0000,
    NULL, 0x0000, 0x0000,
    &JASSeqParser::cmdSetIntTable, 0x0001, 0x0002,
    &JASSeqParser::cmdSetInterrupt, 0x0001, 0x0001,
    &JASSeqParser::cmdDisInterrupt, 0x0001, 0x0001,
    &JASSeqParser::cmdRetI, 0x0000, 0x0000,
    &JASSeqParser::cmdClrI, 0x0000, 0x0000,
    &JASSeqParser::cmdIntTimer, 0x0002, 0x0004,
    &JASSeqParser::cmdSyncCPU, 0x0001, 0x0001,
    NULL, 0x0000, 0x0000,
    NULL, 0x0000, 0x0000,
    NULL, 0x0000, 0x0000,
    &JASSeqParser::cmdPrintf, 0x0000, 0x0000,
    &JASSeqParser::cmdNop, 0x0000, 0x0000,
    &JASSeqParser::cmdFinish, 0x0000, 0x0000,
};

JASSeqParser::CmdInfo JASSeqParser::sExtCmdInfo[255] = {
    NULL, 0x0000, 0x0000,
    &JASSeqParser::cmdDump, 0x0000, 0x0000,
    NULL, 0x0000, 0x0000,
    NULL, 0x0000, 0x0000,
    NULL, 0x0000, 0x0000,
    NULL, 0x0000, 0x0000,
    NULL, 0x0000, 0x0000,
};

bool JASSeqParser::conditionCheck(JASTrack* param_0, JASSeqParser::BranchCondition param_1) {
    u16 reg = readReg(param_0, 3);
    switch (param_1) {
    case 0:
        return 1;
    case 1:
        return reg == 0;
    case 2:
        return reg != 0;
    case 3:
        return reg == 1;
    case 4:
        return 0x8000 <= reg;
    case 5:
        return 0x8000 > reg;
    default:
        JUT_WARN(463, "%s", "Unexpected condition flag");
        return 0;
    }
}

void JASSeqParser::writeReg(JASTrack* param_0, u32 param_1, u32 param_2) {
    param_0->writeReg(JASRegisterParam::REG3, param_2);
    if (param_1 < 0x40) {
        param_0->writeReg(JASRegisterParam::RegID(param_1), param_2);
        return;
    }
    switch (param_1) {
    case 0x40:
    case 0x41:
    case 0x42:
    case 0x43:
    case 0x44:
    case 0x45:
    case 0x46:
    case 0x47:
    case 0x48:
    case 0x49:
    case 0x4a:
    case 0x4b:
    case 0x4c:
    case 0x4d:
    case 0x4e:
    case 0x4f:
        param_0->setPort(param_1 - 0x40, param_2);
        break;
    case 0x60:
    case 0x61:
        JUT_WARN(502, "%s", "writeReg for Read-Only Register");
        break;
    case 0x62:
        param_0->setTimebase(param_2);
        break;
    case 0x63:
        param_0->setTranspose(param_2);
        break;
    case 0x64:
        param_0->setBendSense(param_2);
        break;
    case 0x65:
        param_0->setGateRate(param_2);
        break;
    case 0x66:
        param_0->setSkipSample(param_2);
        break;
    case 0x67:
        param_0->setBankNumber(u8(param_2));
        break;
    case 0x68:
        param_0->setProgNumber(u8(param_2));
        break;
    case 0x69:
        param_0->setPanPower(param_2 / 32767.0f);
        break;
    case 0x6a:
        param_0->setReleasePrio(JSUHiByte(param_2));
        param_0->setNoteOnPrio(JSULoByte(param_2));
        break;
    case 0x6b:
        param_0->setNoteOnPrio(param_2);
        break;
    case 0x6c:
        param_0->setReleasePrio(param_2);
        break;
    case 0x6d:
        param_0->setDirectRelease(param_2);
        break;
    case 0x6e:
        param_0->setVibDepth(param_2 / 1524.0f);
        break;
    case 0x6f:
        param_0->setVibDepth(param_2 / 12192.0f);
        break;
    case 0x70:
        param_0->setTremDepth(param_2 / 256.0f);
        break;
    case 0x71:
        param_0->setVibPitch(param_2 / 64.0f);
        break;
    case 0x72:
        param_0->setTremPitch(param_2 / 64.0f);
        break;
    case 0x73:
        param_0->setVibDelay(param_2);
        break;
    case 0x74:
        param_0->setTremDelay(param_2);
        break;
    default:
        break;
    }
}

u32 JASSeqParser::readReg(JASTrack* param_0, u32 param_1) const {
    if (param_1 < 0x40) {
        return param_0->readReg(JASRegisterParam::RegID(param_1));
    }
    u32 result = 0;
    u16 r27;
    switch (param_1) {
    case 0x40:
    case 0x41:
    case 0x42:
    case 0x43:
    case 0x44:
    case 0x45:
    case 0x46:
    case 0x47:
    case 0x48:
    case 0x49:
    case 0x4a:
    case 0x4b:
    case 0x4c:
    case 0x4d:
    case 0x4e:
    case 0x4f:
        result = param_0->getPort(param_1 - 0x40);
        break;
    case 0x60:
        r27 = 1;
        for (u32 i = 0; i < 16; i++, r27 <<= 1) {
            JASTrack* child = param_0->getChild(i);
            if (child && child->getStatus() == 1) {
                result |= r27;
            }
        }
        break;
    case 0x61:
        result = param_0->getSeqCtrl()->getSeqReader()->getLoopCount();
        break;
    case 0x62:
        result = param_0->getTimebase();
        break;
    case 0x63:
        result = param_0->getTranspose();
        break;
    case 0x64:
        result = param_0->getBendSense();
        break;
    case 0x65:
        result = param_0->getGateRate();
        break;
    case 0x66:
        result = param_0->getSkipSample();
        break;
    case 0x67:
        result = param_0->getBankNumber();
        break;
    case 0x68:
        result = param_0->getProgNumber();
        break;
    case 0x69:
        result = param_0->getPanPower() * 32767.0f;
        break;
    case 0x6a:
        result = param_0->getReleasePrio();
        // @bug this should have been <<=
        result << 8;
        result |= param_0->getNoteOnPrio();
        break;
    case 0x6b:
        result = param_0->getNoteOnPrio();
        break;
    case 0x6c:
        result = param_0->getReleasePrio();
        break;
    case 0x6d:
        result = param_0->getDirectRelease();
        break;
    case 0x6e:
        result = param_0->getVibDepth() * 1524.0f;
        break;
    case 0x6f:
        result = param_0->getVibDepth() * 12192.0f;
        break;
    case 0x70:
        result = param_0->getTremDepth() * 256.0f;
        break;
    case 0x71:
        result = param_0->getVibPitch() * 64.0f;
        break;
    case 0x72:
        result = param_0->getTremPitch() * 64.0f;
        break;
    case 0x73:
        result = param_0->getVibDelay();
        break;
    case 0x74:
        result = param_0->getTremDelay();
        break;
    default:
        break;
    }
    return result;
}

s32 JASSeqParser::cmdOpenTrack(JASTrack* param_0, u32* param_1) {
    u32 r29 = param_1[0];
    u32 r28 = param_1[1];
    JASTrack* child = param_0->openChild(r29);
    if (!child) {
        JUT_WARN(679, "%s", "Cannot open child track");
        return 0;
    }
    child->setSeqData(param_0->getSeqCtrl()->getBase(), r28);
    child->start();
    return 0;
}

s32 JASSeqParser::cmdCloseTrack(JASTrack* param_0, u32* param_1) {
    param_0->closeChild(*param_1);
    return 0;
}

s32 JASSeqParser::cmdCall(JASTrack* param_0, u32* param_1) {
    param_0->getSeqCtrl()->call(*param_1);
    return 0;
}

s32 JASSeqParser::cmdCallF(JASTrack* param_0, u32* param_1) {
    if (conditionCheck(param_0, BranchCondition(param_1[0]))) {
        param_0->getSeqCtrl()->call(param_1[1]);
    }
    return 0;
}

s32 JASSeqParser::cmdRet(JASTrack* param_0, u32* param_1) {
    if (!param_0->getSeqCtrl()->ret()) {
        JUT_WARN(718, "%s", "cannot ret for call-stack is NULL");
        return 0;
    }
    return 0;
}

s32 JASSeqParser::cmdRetF(JASTrack* param_0, u32* param_1) {
    if (conditionCheck(param_0, BranchCondition(param_1[0]))) {
        if (!param_0->getSeqCtrl()->ret()) {
            JUT_WARN(729, "%s", "cannot ret for call-stack is NULL");
            return 0;
        }
    }
    return 0;
}

s32 JASSeqParser::cmdJmp(JASTrack* param_0, u32* param_1) {
    param_0->getSeqCtrl()->jump(param_1[0]);
    return 0;
}

s32 JASSeqParser::cmdJmpF(JASTrack* param_0, u32* param_1) {
    if (conditionCheck(param_0, BranchCondition(param_1[0]))) {
        param_0->getSeqCtrl()->jump(param_1[1]);
    }
    return 0;
}

s32 JASSeqParser::cmdJmpTable(JASTrack* param_0, u32* param_1) {
    JASSeqCtrl* seqCtrl = param_0->getSeqCtrl();
    u32 uVar1 = param_1[0];
    uVar1 += uVar1 * 2;
    seqCtrl->jump(seqCtrl->get24(param_1[1] + uVar1));
    return 0;
}

s32 JASSeqParser::cmdCallTable(JASTrack* param_0, u32* param_1) {
    JASSeqCtrl* seqCtrl = param_0->getSeqCtrl();
    u32 uVar1 = param_1[0];
    uVar1 += uVar1 * 2;
    seqCtrl->call(seqCtrl->get24(param_1[1] + uVar1));
    return 0;
}

s32 JASSeqParser::cmdLoopS(JASTrack* param_0, u32* param_1) {
    param_0->getSeqCtrl()->loopStart(param_1[0]);
    return 0;
}

s32 JASSeqParser::cmdLoopE(JASTrack* param_0, u32* param_1) {
    param_0->getSeqCtrl()->loopEnd();
    return 0;
}

s32 JASSeqParser::cmdNote(JASTrack* param_0, u32* param_1) {
    execNoteOnGate(param_0, param_1[1], param_1[2], param_1[3], param_1[0]);
    return 0;
}

s32 JASSeqParser::cmdNoteOn(JASTrack* param_0, u32* param_1) {
    execNoteOnMidi(param_0, param_1[1], param_1[0], param_1[2]);
    return 0;
}

s32 JASSeqParser::cmdNoteOff(JASTrack* param_0, u32* param_1) {
    execNoteOff(param_0, param_1[0]);
    return 0;
}

s32 JASSeqParser::cmdReadPort(JASTrack* param_0, u32* param_1) {
    u16 tmp = param_0->readPortSelf(param_1[0]);
    writeReg(param_0, param_1[1], tmp);
    return 0;
}

s32 JASSeqParser::cmdWritePort(JASTrack* param_0, u32* param_1) {
    param_0->writePortSelf(param_1[0], param_1[1]);
    return 0;
}

s32 JASSeqParser::cmdParentWritePort(JASTrack* param_0, u32* param_1) {
    {
        u32* p1 = param_1;
        u32* p2 = param_1 + 1;
        JASTrack* parent = param_0->getParent();
        if (parent) {
            parent->writePort(*p1, *p2);
        } else {
            JUT_WARN(835, "%s", "parent == NULL");
        }
    }
    return 0;
}

s32 JASSeqParser::cmdChildWritePort(JASTrack* param_0, u32* param_1) {
    {
        u8 val = param_1[0];
        JASTrack* child = param_0->getChild(JSUHiNibble(val));
        if (child) {
            child->writePort(JSULoNibble(val), param_1[1]);
        } else {
            JUT_WARN(848, "%s", "child == NULL");
        }
    }
    return 0;
}

s32 JASSeqParser::cmdParentReadPort(JASTrack* param_0, u32* param_1) {
    u32* p1 = param_1;
    u32* p2 = param_1 + 1;
    JASTrack* parent = param_0->getParent();
    if (parent) {
        u16 tmp = parent->readPort(*p1);
        writeReg(param_0, *p2, tmp);
    } else {
        JUT_WARN(864, "%s", "parent == NULL");
    }
    return 0;
}

s32 JASSeqParser::cmdChildReadPort(JASTrack* param_0, u32* param_1) {
    u8 val = param_1[0];
    u32* p2 = param_1 + 1;
    JASTrack* child = param_0->getChild(JSUHiNibble(val));
    if (child) {
        u16 tmp = child->readPort(JSULoNibble(val));
        writeReg(param_0, *p2, tmp);
    } else {
        JUT_WARN(880, "%s", "child == NULL");
    }
    return 0;
}

s32 JASSeqParser::cmdCheckPortImport(JASTrack* param_0, u32* param_1) {
    u8 tmp = param_0->checkPortIn(param_1[0]);
    param_0->writeReg(JASRegisterParam::REG3, tmp);
    return 0;
}

s32 JASSeqParser::cmdCheckPortExport(JASTrack* param_0, u32* param_1) {
    u8 tmp = param_0->checkPort(param_1[0]);
    param_0->writeReg(JASRegisterParam::REG3, tmp);
    return 0;
}

s32 JASSeqParser::cmdWait(JASTrack* param_0, u32* param_1) {
    JASSeqCtrl* seqCtrl = param_0->getSeqCtrl();
    int midiValue = seqCtrl->readMidiValue();
    seqCtrl->wait(midiValue);
    return 0;
}

s32 JASSeqParser::cmdWaitByte(JASTrack* param_0, u32* param_1) {
    u32 val = param_1[0];
    param_0->getSeqCtrl()->wait(val);
    return 0;
}

s32 JASSeqParser::cmdSetLastNote(JASTrack* param_0, u32* param_1) {
    u32 r31 = param_1[0];
    if (r31 < 256) {
        param_0->setLatestKey(r31);
    } else {
        JUT_WARN(928, "%s", "Too large latest key");
    }
    return 0;
}

s32 JASSeqParser::cmdEnvScaleSet(JASTrack* param_0, u32* param_1) {
    param_0->setOscScale(param_1[0], param_1[1] / 16383.0f);
    return 0;
}


s32 JASSeqParser::cmdEnvSet(JASTrack* param_0, u32* param_1) {
    param_0->setOscTable(param_1[0], (JASOscillator::Point*)param_0->getSeqCtrl()->getAddr(param_1[1]));
    return 0;
}

s32 JASSeqParser::cmdSimpleADSR(JASTrack* param_0, u32* param_1) {
    param_0->setOscAdsr(param_1[0], param_1[1], param_1[2], param_1[3], param_1[4]);
    return 0;
}

s32 JASSeqParser::cmdBusConnect(JASTrack* param_0, u32* param_1) {
    param_0->connectBus(param_1[0], param_1[1]);
    return 0;
}

s32 JASSeqParser::cmdSetIntTable(JASTrack* param_0, u32* param_1) {
    param_0->getSeqCtrl()->setIntrTable(param_1[0]);
    return 0;
}

s32 JASSeqParser::cmdSetInterrupt(JASTrack* param_0, u32* param_1) {
    param_0->getSeqCtrl()->setIntrMask(param_1[0]);
    return 0;
}

s32 JASSeqParser::cmdDisInterrupt(JASTrack* param_0, u32* param_1) {
    param_0->getSeqCtrl()->clrIntrMask(param_1[0]);
    return 0;
}

s32 JASSeqParser::cmdClrI(JASTrack* param_0, u32* param_1) {
    JASSeqCtrl* seqCtrl = param_0->getSeqCtrl();
    seqCtrl->clrIntr();
    seqCtrl->checkIntr();
    return 0;
}

s32 JASSeqParser::cmdRetI(JASTrack* param_0, u32* param_1) {
    JASSeqCtrl* seqCtrl = param_0->getSeqCtrl();
    seqCtrl->retIntr();
    seqCtrl->checkIntr();
    return 0;
}

s32 JASSeqParser::cmdIntTimer(JASTrack* param_0, u32* param_1) {
    param_0->getSeqCtrl()->setIntrTimer(param_1[0], param_1[1]);
    return 0;
}

u16 (*JASSeqParser::sCallBackFunc)(JASTrack*, u16);

s32 JASSeqParser::cmdSyncCPU(JASTrack* param_0, u32* param_1) {
    u16 r31 = 0xffff;
    if (sCallBackFunc) {
        r31 = sCallBackFunc(param_0, param_1[0]);
    }
    param_0->writeReg(JASRegisterParam::REG3, r31);
    return 0;
}

s32 JASSeqParser::cmdTempo(JASTrack* param_0, u32* param_1) {
    param_0->setTempo(param_1[0]);
    return 0;
}

s32 JASSeqParser::cmdFinish(JASTrack* param_0, u32* param_1) {
    return -1;
}

s32 JASSeqParser::cmdNop(JASTrack* param_0, u32* param_1) {
    return 0;
}

s32 JASSeqParser::cmdFIRSet(JASTrack* param_0, u32* param_1) {
    s16* addr = (s16*)param_0->getSeqCtrl()->getAddr(param_1[0]);
    param_0->setFIR(addr);
    return 0;
}

s32 JASSeqParser::cmdIIRSet(JASTrack* param_0, u32* param_1) {
    s16 stack_10[4];
    for (int i = 0; i < 4; i++) {
        stack_10[i] = param_1[i];
    }
    param_0->setIIR(stack_10);
    return 0;
}

s32 JASSeqParser::cmdIIRCutOff(JASTrack* param_0, u32* param_1) {
    u32 r31 = param_1[0];
    if (r31 < 128) {
        param_0->setIIR(JASCalc::CUTOFF_TO_IIR_TABLE[r31]);
    } else {
        JUT_WARN(1077, "%s", "Too large cutoff");
    }
    return 0;
}

s32 JASSeqParser::cmdBankPrg(JASTrack* param_0, u32* param_1) {
    u16 r31 = JSULoHalf(param_1[0]);
    param_0->setBankNumber(JSUHiByte(r31));
    param_0->setProgNumber(JSULoByte(r31));
    return 0;
}

s32 JASSeqParser::cmdBank(JASTrack* param_0, u32* param_1) {
    param_0->setBankNumber(param_1[0] & 0xff);
    return 0;
}

s32 JASSeqParser::cmdPrg(JASTrack* param_0, u32* param_1) {
    param_0->setProgNumber(param_1[0] & 0xff);
    return 0;
}

s32 JASSeqParser::cmdParamI(JASTrack* param_0, u32* param_1) {
    s16 val = param_1[1];
    param_0->setParam(param_1[0], val / 32767.0f, 0);
    return 0;
}


s32 JASSeqParser::cmdParamII(JASTrack* param_0, u32* param_1) {
    s16 val = param_1[1];
    param_0->setParam(param_1[0], val / 32767.0f, param_1[2]);
    return 0;
}


s32 JASSeqParser::cmdParamE(JASTrack* param_0, u32* param_1) {
    s32* r31 = (s32*)param_1 + 1;
    s16 r30 = *r31 << 8;
    if ((*r31 & 0x80) == 0) {
        r30 |= s16(*r31 << 1);
    }
    param_0->setParam(param_1[0], r30 / 32767.0f, 0);
    return 0;
}


s32 JASSeqParser::cmdParamEI(JASTrack* param_0, u32* param_1) {
    s32* r31 = (s32*)param_1 + 1;
    s16 r30 = *r31 << 8;
    if ((*r31 & 0x80) == 0) {
        r30 |= s16(*r31 << 1);
    }
    param_0->setParam(param_1[0], r30 / 32767.0f, param_1[2]);
    return 0;
}

s32 JASSeqParser::cmdReg(JASTrack* param_0, u32* param_1) {
    u32 val = param_1[0];
    u32 r29 = param_1[1];
    u32 r30 = param_1[2];
    u32 tmp;
    switch (val) {
    case 0:
        break;
    case 1:
        r30 += readReg(param_0, r29);
        break;
    case 2:
        r30 = readReg(param_0, r29) - r30;
        break;
    case 3:
        r30 = readReg(param_0, r29) - r30;
        r29 = 3;
        break;
    case 4:
        r30 *= readReg(param_0, r29);
        r29 = 0x21;
        break;
    case 5:
        r30 &= readReg(param_0, r29);
        break;
    case 6:
        r30 |= readReg(param_0, r29);
        break;
    case 7:
        r30 ^= readReg(param_0, r29);
        break;
    case 8:
        static JMath::TRandom_<JMath::TRandom_fast_> oRandom(0);
        r30 = (oRandom.get_bit32() >> 9) % r30;
        break;
    case 9:
        r30 = readReg(param_0, r29) << r30;
        break;
    case 10:
        r30 = readReg(param_0, r29) >> r30;
        break;
    default:
        JUT_WARN(1191, "%s", "cmdReg Not Supported RegCmdType");
        return 0;
    }
    writeReg(param_0, r29, r30);
    return 0;
}

s32 JASSeqParser::cmdRegLoad(JASTrack* param_0, u32* param_1) {
    u32 val1 = param_1[0];
    u32 val2 = param_1[1];
    writeReg(param_0, val1, val2);
    return 0;
}

s32 JASSeqParser::cmdRegUni(JASTrack* param_0, u32* param_1) {
    u32 val1 = param_1[0];
    u32 r31 = param_1[1];
    s32 r30 = 0;
    switch (val1) {
    case 11:
        r30 = -readReg(param_0, r31);
        break;
    default:
        JUT_WARN(1217, "%s", "cmdRegUni Not Supported RegCmdType");
        return 0;
    }
    writeReg(param_0, r31, r30);
    return 0;
}

s32 JASSeqParser::cmdRegTblLoad(JASTrack* param_0, u32* param_1) {
    JASSeqCtrl* seqCtrl = param_0->getSeqCtrl();
    u32 p0 = param_1[0];
    u32 p1 = param_1[1];
    u32 p2 = param_1[2];
    u32 p3 = param_1[3];
    u32 r27;
    switch (p0) {
    case 12:
        r27 = seqCtrl->getByte(p2 + p3);
        break;
    case 13:
        p3 *= 2;
        r27 = seqCtrl->get16(p2 + p3);
        break;
    case 14:
        p3 += p3 * 2;
        r27 = seqCtrl->get24(p2 + p3);
        break;
    case 15:
        p3 *= 4;
        r27 = seqCtrl->get32(p2 + p3);
        break;
    case 16:
        r27 = seqCtrl->get32(p2 + p3);
        break;
    default:
        JUT_WARN(1255, "%s", "cmdRegTblLoad Not Supported RegCmdType");
        return 0;
    }
    writeReg(param_0, p1, r27);
    return 0;
}

s32 JASSeqParser::cmdDump(JASTrack* param_0, u32* param_1) {
    JASSeqCtrl* seqCtrl = param_0->getSeqCtrl();
    JASReport("--------------- JASTrack (%8x) dump ----------------", param_0);
    JASReport(" Base: 0x%08x Cur: 0x%08x(0x%06x)", seqCtrl->getBase(), seqCtrl->getCur(), (u8*)seqCtrl->getCur() - (u8*)seqCtrl->getBase());
    JASReport("");
    JASReport(" REG_A: 0x%04x REG_B: 0x%04x REG_S: 0x%04x", readReg(param_0, 0), readReg(param_0, 1), readReg(param_0, 2));
    JASReport(" REG_X: 0x%04x REG_Y: 0x%04x REG_F: 0x%04x", readReg(param_0, 4), readReg(param_0, 5), readReg(param_0, 3));
    JASReport("");
    JASReport(" PORT 0-3: 0x%04x 0x%04x 0x%04x 0x%04x", param_0->getPort(0x0), param_0->getPort(0x1), param_0->getPort(0x2), param_0->getPort(0x3));
    JASReport(" PORT 4-7: 0x%04x 0x%04x 0x%04x 0x%04x", param_0->getPort(0x4), param_0->getPort(0x5), param_0->getPort(0x6), param_0->getPort(0x7));
    JASReport(" PORT 8-B: 0x%04x 0x%04x 0x%04x 0x%04x", param_0->getPort(0x8), param_0->getPort(0x9), param_0->getPort(0xa), param_0->getPort(0xb));
    JASReport(" PORT C-F: 0x%04x 0x%04x 0x%04x 0x%04x", param_0->getPort(0xc), param_0->getPort(0xd), param_0->getPort(0xe), param_0->getPort(0xf));
    JASReport("");
    return 0;
}

s32 JASSeqParser::cmdPrintf(JASTrack* param_0, u32* param_1) {
    u8 stack_c[4];
    u32 stack_10[4];
    char buffer[128];

    JASSeqCtrl* seqCtrl = param_0->getSeqCtrl();
    u32 r30 = 0;
    u32 i = 0;
    for (; i < 128; i++) {
        buffer[i] = seqCtrl->readByte();
        if (buffer[i] == 0) {
            break;
        }
        if (buffer[i] == '\\') {
            buffer[i] = seqCtrl->readByte();
            if (buffer[i] == 0) {
                break;
            }
            switch (buffer[i]) {
            case 'n':
                buffer[i] = '\r';
                break;
            }
        } else if (buffer[i] == '%') {
            i++;
            buffer[i] = seqCtrl->readByte();
            if (buffer[i] == 0) {
                break;
            }
            switch (buffer[i]) {
            case 'd':
                stack_c[r30] = 0;
                break;
            case 'x':
                stack_c[r30] = 1;
                break;
            case 's':
                stack_c[r30] = 2;
                break;
            case 'r':
                stack_c[r30] = 3;
                buffer[i] = 'd';
                break;
            case 'R':
                stack_c[r30] = 4;
                buffer[i] = 'x';
                break;
            }
            r30++;
        }
    }

    for (i = 0; i < r30; i++) {
        stack_10[i] = (u8)seqCtrl->readByte();
        switch (stack_c[i]) {
        case 2:
            stack_10[i] = (u32)seqCtrl->getAddr(stack_10[i]);
            break;
        case 3:
        case 4:
            stack_10[i] = readReg(param_0, stack_10[i] & 0xff);
            break;
        }
    }

    JASReport(buffer, stack_10[0], stack_10[1], stack_10[2], stack_10[3]);
    return 0;
}

s32 JASSeqParser::execNoteOnGate(JASTrack* param_0, u32 param_1, u32 param_2, u32 param_3,
                                 u32 param_4) {
    JASSeqCtrl* seqCtrl = param_0->getSeqCtrl();
    // likely fake match, this may use some actual union defined somewhere else
    union {
        u8 val;
        struct {
            u8 bit0 : 1;
            u8 bit1 : 1;
        } bits;
    } tmp;
    tmp.val = param_4;
    int r31 = 0;
    if (tmp.bits.bit1) {
        r31 |= 2;
    }
    if (tmp.bits.bit0) {
        r31 |= 1;
    }
    if (param_3 == 0) {
        r31 |= 4;
    }
    int result = param_0->gateOn(param_1, param_2, param_3, r31);
    if (param_3) {
        seqCtrl->wait(param_3);
    } else {
        seqCtrl->waitNoteFinish();
    }
    return result;
}

s32 JASSeqParser::execNoteOnMidi(JASTrack* param_0, u32 param_1, u32 param_2, u32 param_3) {
     return param_0->noteOn(param_1, param_2, param_3);
}

s32 JASSeqParser::execNoteOff(JASTrack* param_0, u32 param_1) {
    return param_0->noteOff(param_1, 0);
}

s32 JASSeqParser::execCommand(JASTrack* param_0, s32 (JASSeqParser::*param_1)(JASTrack*, u32*),
                                   u32 param_2, u32* param_3) {
    return (this->*param_1)(param_0, param_3);
}

s32 JASSeqParser::parseNoteOff(JASTrack* param_0, u8 param_1) {
    u8 note_id = param_1 & 7;
    if (note_id) {
        execNoteOff(param_0, note_id);
    } else {
        JUT_WARN(1433, "%s", "noteoff for note_id == 0");
    }
    return 0;
}

s32 JASSeqParser::parseNoteOn(JASTrack* param_0, u8 param_1) {
    JASSeqCtrl* seqCtrl = param_0->getSeqCtrl();
    u32 r28 = seqCtrl->readByte();
    u32 r30 = r28;
    r30 &= 7;
    u32 r27 = seqCtrl->readByte();
    if ((u8)r30 == 0) {
        u32 midiVal = seqCtrl->readMidiValue();
        execNoteOnGate(param_0, (u8)param_1, (u8)r27, midiVal, (u8)r28);
    } else {
        execNoteOnMidi(param_0, (u8)r30, (u8)param_1, (u8)r27);
    }
    return 0;
}

s32 JASSeqParser::parseCommand(JASTrack* param_0, u8 cmd, u16 param_2) {
    JASSeqCtrl* seqCtrl = param_0->getSeqCtrl();
    CmdInfo* cmdInfo = NULL;
    if (cmd != 0xb0) {
        JUT_ASSERT(1477, cmd >= 0xa0);
        cmdInfo = &sCmdInfo[cmd - 0xa0];
    } else {
        cmdInfo = &sExtCmdInfo[seqCtrl->readByte() & 0xff];
    }
    u16 r28 = (u16)cmdInfo->field_0xe;
    r28 |= param_2;
    u32 stack_28[8];
    for (int i = 0; i < cmdInfo->field_0xc; i++, r28 >>= 2) {
        int r27 = 0;
        switch (r28 & 3) {
        case 0:
            r27 = (u8)seqCtrl->readByte();
            break;
        case 1:
            r27 = (u16)seqCtrl->read16();
            break;
        case 2:
            r27 = seqCtrl->read24();
            break;
        case 3:
            r27 = readReg(param_0, (u8)seqCtrl->readByte());
            break;
        }
        stack_28[i] = r27;
    }
    s32 (JASSeqParser::*ptr)(JASTrack*, u32*) = cmdInfo->field_0x0;
    if (!ptr) {
        return 0;
    }
    return execCommand(param_0, ptr, cmdInfo->field_0xc, stack_28);
}

s32 JASSeqParser::parseRegCommand(JASTrack* param_0, int param_1) {
    JASSeqCtrl* seqCtrl = param_0->getSeqCtrl();
    u8 r30 = seqCtrl->readByte();
    u16 r29 = 0;
    u16 r28 = 3;
    for (int i = 0; i < param_1; i++) {
        if (r30 & 0x80) {
            r29 |= r28;
        }
        r30 <<= 1;
        r28 <<= 2;
    }
    return parseCommand(param_0, seqCtrl->readByte(), r29);
}

s32 JASSeqParser::parse(JASTrack* param_0) {
    u32 r31 = param_0->getSeqCtrl()->readByte();
    s32 r30 = 0;
    if ((r31 & 0x80) == 0) {
        r30 = parseNoteOn(param_0, r31);
    } else {
        switch (r31 & 0xf0) {
        case 0x80:
            r30 = parseNoteOff(param_0, r31 & 0xf);
            break;
        case 0x90:
            r30 = parseRegCommand(param_0, (r31 & 7) + 1);
            break;
        default:
            r30 = parseCommand(param_0, r31, 0);
            break;
        }
    }
    return r30;
}
