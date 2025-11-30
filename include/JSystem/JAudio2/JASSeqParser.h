#ifndef JASSEQPARSER_H
#define JASSEQPARSER_H

#include "dolphin/types.h"

class JASTrack;

/**
 * @ingroup jsystem-jaudio
 * 
 */
class JASSeqParser {
public:
    enum BranchCondition {};

    struct CmdInfo {
        s32 (JASSeqParser::*field_0x0)(JASTrack*, u32*);
        u16 field_0xc;
        u16 field_0xe;
    };

    virtual ~JASSeqParser() {}
    virtual s32 parse(JASTrack*);
    virtual s32 execNoteOnMidi(JASTrack*, u32, u32, u32);
    virtual s32 execNoteOnGate(JASTrack*, u32, u32, u32, u32);
    virtual s32 execNoteOff(JASTrack*, u32);
    virtual s32 execCommand(JASTrack*, s32 (JASSeqParser::*)(JASTrack*, u32*), u32, u32*);

    bool conditionCheck(JASTrack*, JASSeqParser::BranchCondition);
    void writeReg(JASTrack*, u32, u32);
    u32 readReg(JASTrack*, u32) const;
    s32 cmdOpenTrack(JASTrack*, u32*);
    s32 cmdCloseTrack(JASTrack*, u32*);
    s32 cmdCall(JASTrack*, u32*);
    s32 cmdCallF(JASTrack*, u32*);
    s32 cmdRet(JASTrack*, u32*);
    s32 cmdRetF(JASTrack*, u32*);
    s32 cmdJmp(JASTrack*, u32*);
    s32 cmdJmpF(JASTrack*, u32*);
    s32 cmdJmpTable(JASTrack*, u32*);
    s32 cmdCallTable(JASTrack*, u32*);
    s32 cmdLoopS(JASTrack*, u32*);
    s32 cmdLoopE(JASTrack*, u32*);
    s32 cmdNote(JASTrack*, u32*);
    s32 cmdNoteOn(JASTrack*, u32*);
    s32 cmdNoteOff(JASTrack*, u32*);
    s32 cmdReadPort(JASTrack*, u32*);
    s32 cmdWritePort(JASTrack*, u32*);
    s32 cmdParentWritePort(JASTrack*, u32*);
    s32 cmdChildWritePort(JASTrack*, u32*);
    s32 cmdParentReadPort(JASTrack*, u32*);
    s32 cmdChildReadPort(JASTrack*, u32*);
    s32 cmdCheckPortImport(JASTrack*, u32*);
    s32 cmdCheckPortExport(JASTrack*, u32*);
    s32 cmdWait(JASTrack*, u32*);
    s32 cmdWaitByte(JASTrack*, u32*);
    s32 cmdSetLastNote(JASTrack*, u32*);
    s32 cmdEnvScaleSet(JASTrack*, u32*);
    s32 cmdEnvSet(JASTrack*, u32*);
    s32 cmdSimpleADSR(JASTrack*, u32*);
    s32 cmdBusConnect(JASTrack*, u32*);
    s32 cmdSetIntTable(JASTrack*, u32*);
    s32 cmdSetInterrupt(JASTrack*, u32*);
    s32 cmdDisInterrupt(JASTrack*, u32*);
    s32 cmdClrI(JASTrack*, u32*);
    s32 cmdRetI(JASTrack*, u32*);
    s32 cmdIntTimer(JASTrack*, u32*);
    s32 cmdSyncCPU(JASTrack*, u32*);
    s32 cmdTempo(JASTrack*, u32*);
    s32 cmdFinish(JASTrack*, u32*);
    s32 cmdNop(JASTrack*, u32*);
    s32 cmdFIRSet(JASTrack*, u32*);
    s32 cmdIIRSet(JASTrack*, u32*);
    s32 cmdIIRCutOff(JASTrack*, u32*);
    s32 cmdBankPrg(JASTrack*, u32*);
    s32 cmdBank(JASTrack*, u32*);
    s32 cmdPrg(JASTrack*, u32*);
    s32 cmdParamI(JASTrack*, u32*);
    s32 cmdParamII(JASTrack*, u32*);
    s32 cmdParamE(JASTrack*, u32*);
    s32 cmdParamEI(JASTrack*, u32*);
    s32 cmdReg(JASTrack*, u32*);
    s32 cmdRegLoad(JASTrack*, u32*);
    s32 cmdRegUni(JASTrack*, u32*);
    s32 cmdRegTblLoad(JASTrack*, u32*);
    s32 cmdDump(JASTrack*, u32*);
    s32 cmdPrintf(JASTrack*, u32*);
    s32 parseNoteOff(JASTrack*, u8);
    s32 parseNoteOn(JASTrack*, u8);
    s32 parseCommand(JASTrack*, u8, u16);
    s32 parseRegCommand(JASTrack*, int);

    static void registerSeqCallback(u16 (*param_0)(JASTrack*, u16)) { sCallBackFunc = param_0; }

    static CmdInfo sCmdInfo[96];
    static CmdInfo sExtCmdInfo[255];
    static u16 (*sCallBackFunc)(JASTrack*, u16);
};

#endif /* JASSEQPARSER_H */
