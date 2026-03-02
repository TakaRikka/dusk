#ifndef NW4HBM_SND_MML_PARSER_H
#define NW4HBM_SND_MML_PARSER_H

#include <revolution/types.h>
#include "SeqTrack.h"


namespace nw4hbm {
    namespace snd {
        namespace detail {

            class MmlSeqTrack;
            class SeqPlayer;

            class MmlParser {
            public:
                typedef enum SeqArgType {
                    SEQ_ARG_U8 = 0,
                    SEQ_ARG_S16,
                    SEQ_ARG_VMIDI,
                    SEQ_ARG_RANDOM,
                    SEQ_ARG_VARIABLE
                } SeqArgType;

                static const int CALL_STACK_MAX_DEPTH = 3;

            public:
                /* 0x08 */ virtual void CommandProc(MmlSeqTrack* track, u32 command,
                                                    s32 commandArg1, s32 commandArg2) const;
                /* 0x0C */ virtual Channel* NoteOnCommandProc(MmlSeqTrack* track, int key,
                                                              int velocity, s32 length,
                                                              bool tieFlag) const;

                ParseResult Parse(MmlSeqTrack* track, bool doNoteOn) const;

                vs16* GetVariablePtr(SeqPlayer* player, SeqTrack* track, int varNo) const;

                static void EnablePrintVar(bool enable) { mPrintVarEnabledFlag = enable; }

            private:
                typedef enum MmlSeqData {
                    MML_CMD_MIN = 0x80,  // <80 -> MML note, not a command

                    MML_WAIT = 0x80,       // 0x80
                    MML_SET_PRGNO,         // 0x81
                    MML_OPENTRACK = 0x88,  // 0x88
                    MML_JUMP,              // 0x89
                    MML_CALL,              // 0x8A

                    MML_RNDPARAM = 0xA0,  // 0xA0
                    MML_INDPARAM,         // 0xA1
                    MML_EXECIF,           // 0xA2

                    MML_SET_TIMEBASE = 0xB0,  // 0xB0 (unused by this version)

                    MML_SET_PAN = 0xC0,     // 0xC0
                    MML_SET_TRACK_VOLUME,   // 0xC1
                    MML_SET_PLAYER_VOLUME,  // 0xC2
                    MML_SET_TRANSPOSE,      // 0xC3
                    MML_SET_PITCHBEND,      // 0xC4
                    MML_SET_BENDRANGE,      // 0xC5
                    MML_SET_PRIORITY,       // 0xC6
                    MML_SET_NOTEWAIT,       // 0xC7
                    MML_SET_TIE,            // 0xC8
                    MML_SET_PORTAMENTO,     // 0xC9
                    MML_SET_LFODEPTH,       // 0xCA
                    MML_SET_LFOSPEED,       // 0xCB
                    MML_SET_LFOTARGET,      // 0xCC
                    MML_SET_LFORANGE,       // 0xCD
                    MML_SET_PORTASPEED,     // 0xCE
                    MML_SET_PORTATIME,      // 0xCF

                    MML_SET_ATTACK = 0xD0,  // 0xD0
                    MML_SET_DECAY,          // 0xD1
                    MML_SET_SUSTAIN,        // 0xD2
                    MML_SET_RELEASE,        // 0xD3
                    MML_LOOP_START,         // 0xD4
                    MML_SET_TRACK_VOLUME2,  // 0xD5
                    MML_PRINTVAR,           // 0xD6
                    MML_SET_SURROUNDPAN,    // 0xD7
                    MML_SET_LPFFREQ,        // 0xD8
                    MML_SET_FXSEND_A,       // 0xD9
                    MML_SET_FXSEND_B,       // 0xDA
                    MML_SET_MAINSEND,       // 0xDB
                    MML_SET_INITPAN,        // 0xDC
                    MML_SET_MUTE,           // 0xDD
                    MML_SET_FXSEND_C,       // 0xDE
                    MML_SET_DAMPER,         // 0xDF (unused by this version)

                    MML_SET_LFODELAY = 0xE0,  // 0xE0
                    MML_SET_TEMPO,            // 0xE1
                    MML_CMD_E2h,              // 0xE2
                    MML_SET_SWEEPPITCH,       // 0xE3

                    MML_LOOP_END = 0xFC,  // 0xFC
                    MML_RET,              // 0xFD
                    MML_ALLOCTRACK,       // 0xFE
                    MML_EOF,              // 0xFF

                    MML_CMD_MASK = 0x80,
                    MML_CMD_SET_MASK = 0xF0,

                    MML_CMD_MAX = 0xFF,  // >FF -> EX command

                    // (MML_EX_COMMAND << 8) | Command
                    MML_EX_COMMAND = 0xF0,

                    // MML EX arithmetic command set
                    MML_EX_ARITHMETIC = 0x80,  // 0xF0 0x8X
                    MML_EX_SET = 0x80,         // 0xF0 0x80
                    MML_EX_APL,                // 0xF0 0x81
                    MML_EX_AMI,                // 0xF0 0x82
                    MML_EX_AMU,                // 0xF0 0x83
                    MML_EX_ADV,                // 0xF0 0x84
                    MML_EX_ALS,                // 0xF0 0x85
                    MML_EX_RND,                // 0xF0 0x86
                    MML_EX_AAD,                // 0xF0 0x87
                    MML_EX_AOR,                // 0xF0 0x88
                    MML_EX_AER,                // 0xF0 0x89
                    MML_EX_ACO,                // 0xF0 0x8A
                    MML_EX_AMD,                // 0xF0 0x8B

                    // MML EX logic command set
                    MML_EX_LOGIC = 0x90,  // 0xF0 0x9X
                    MML_EX_EQ = 0x90,     // 0xF0 0x90
                    MML_EX_GE,            // 0xF0 0x91
                    MML_EX_GT,            // 0xF0 0x92
                    MML_EX_LE,            // 0xF0 0x93
                    MML_EX_LT,            // 0xF0 0x94
                    MML_EX_NE,            // 0xF0 0x95

                    // MML EX userproc callback
                    MML_EX_USERPROC = 0xE0,  // 0xF0 0xE0

                    MML_EX_CMD_MAX = 0xFFFF,  // >FFFF -> Invalid command
                } MmlSeqData;

            private:
                u8 ReadByte(const u8** data) const { return *(*data)++; }

                u16 Read16(const u8** data) const;
                u32 Read24(const u8** data) const;
                s32 ReadVar(const u8** data) const;
                s32 ReadArg(const u8** data, SeqPlayer* player, SeqTrack* track,
                            SeqArgType type) const;

            private:
                static bool mPrintVarEnabledFlag;
            };

        }  // namespace detail
    }  // namespace snd
}  // namespace nw4hbm

#endif
