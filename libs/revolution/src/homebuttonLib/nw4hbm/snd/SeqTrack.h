#ifndef NW4HBM_SND_SEQ_TRACK_H
#define NW4HBM_SND_SEQ_TRACK_H

#include <revolution/types.h>

#include "Channel.h"
#include "Lfo.h"
#include "MoveValue.h"
#include "snd_global.h"

namespace nw4hbm {
    namespace snd {

        typedef enum SeqMute { MUTE_OFF = 0, MUTE_NO_STOP, MUTE_RELEASE, MUTE_STOP } SeqMute;

        typedef enum ParseResult { PARSE_RESULT_CONTINUE = 0, PARSE_RESULT_FINISH } ParseResult;

        namespace detail {
            class SeqPlayer;
            class SeqTrack {
            public:
                static const int VARIABLE_NUM = 16;
                static const int PRGNO_MAX = 0xFFFF;

                typedef struct ParserTrackParam {
                    /* 0x00 */ const u8* baseAddr;
                    /* 0x04 */ const u8* currentAddr;
                    /* 0x08 */ s32 wait;
                    /* 0x0C */ u8 muteFlag;
                    /* 0x0D */ u8 silenceFlag;
                    /* 0x0E */ u8 noteFinishWait;
                    /* 0x0F */ u8 portaFlag;
                    /* 0x10 */ int bankNo;
                    /* 0x14 */ int prgNo;
                    /* 0x18 */ LfoParam lfoParam;
                    /* 0x28 */ u8 lfoTarget;
                    /* 0x2C */ f32 sweepPitch;
                    /* 0x30 */ u8 volume;
                    /* 0x31 */ u8 volume2;
                    /* 0x32 */ s8 pitchBend;
                    /* 0x33 */ u8 bendRange;
                    /* 0x34 */ s8 pan;
                    /* 0x35 */ s8 initPan;
                    /* 0x36 */ s8 surroundPan;
                    /* 0x37 */ s8 transpose;
                    /* 0x38 */ u8 priority;
                    /* 0x39 */ u8 portaKey;
                    /* 0x3A */ u8 portaTime;
                    /* 0x3B */ u8 attack;
                    /* 0x3C */ u8 decay;
                    /* 0x3D */ u8 sustain;
                    /* 0x3E */ u8 release;
                    /* 0x3F */ u8 mainSend;
                    /* 0x40 */ u8 fxSend[AUX_BUS_NUM];
                    /* 0x43 */ u8 lpfFreq;
                } ParserTrackParam;

            public:
                SeqTrack() : mSeqPlayer(NULL) { InitParam(); }

                /* 0x08 */ virtual ~SeqTrack() {}
                /* 0x0C */ virtual ParseResult Parse(bool doNoteOn) = 0;

                void SetPlayerTrackNo(int playerTrackNo);
                u8 GetPlayerTrackNo() const { return mPlayerTrackNo; }

                void InitParam();
                void SetSeqData(const void* base, s32 offset);

                void Open();
                void Close();

                void UpdateChannelLength();
                void UpdateChannelRelease(Channel* channel);

                int ParseNextTick(bool doNoteOn);

                void StopAllChannel();
                void ReleaseAllChannel(int release) NO_INLINE;
                void PauseAllChannel(bool flag);
                void AddChannel(Channel* channel);
                void UpdateChannelParam();
                void FreeAllChannel();

                void SetMute(SeqMute mute);
                void SetVolume(f32 volume);
                void SetPitch(f32 pitch);

                void SetSilence(bool silence, int fadeTime);
                void SetPan(f32 param1);
                void SetSurroundPan(f32 param1);
                void SetLpfFreq(f32 param1);
                void SetBiquadFilter(int param1, f32 param2);
                void SetPanRange(f32 param1);
                void SetModDepth(f32 param1);
                void SetModSpeed(f32 param1);
                void SetMainSend(f32 param1);
                void SetFxSend(AuxBus bus, f32 param2);
                void SetRemoteSend(s32 remoteIndex, f32 param2);
                void SetRemoteFxSend(s32 remoteIndex, f32 param2);

                ParserTrackParam& GetParserTrackParam() { return mParserTrackParam; }

                vs16* GetVariablePtr(int varNo);

                SeqPlayer* GetSeqPlayer() { return mSeqPlayer; }
                void SetSeqPlayer(SeqPlayer* seqPlayer) { mSeqPlayer = seqPlayer; }

                Channel* GetLastChannel() const { return mChannelList; }

                Channel* NoteOn(int key, int velocity, s32 portatime, bool tie);

            private:
                static const int DEFAULT_PRIORITY = 64;
                static const int DEFAULT_BENDRANGE = 2;
                static const int DEFAULT_PORTA_KEY = 60;
                static const int DEFAULT_VARIABLE_VALUE = -1;

            private:
                static void ChannelCallbackFunc(Channel* dropChannel,
                                                Channel::ChannelCallbackStatus status,
                                                u32 userData);

            private:
                /* 0x04 */ u8 mPlayerTrackNo;
                /* 0x08 */ f32 mExtVolume;
                /* 0x0C */ f32 mExtPitch;
                /* 0x10 */ f32 mExtPan;
                /* 0x14 */ f32 mExtSurroundPan;
                /* 0x18 */ f32 mPanRange;
                /* 0x1C */ f32 mExtLpfFreq;
                /* 0x20 */ f32 mExtMainSend;
                /* 0x24 */ f32 mExtFxSend[AUX_BUS_NUM];
                /* 0x30 */ f32 mExtRemoteSend[WPAD_MAX_CONTROLLERS];
                /* 0x40 */ f32 mExtRemoteFxSend[WPAD_MAX_CONTROLLERS];
                /* 0x50 */ ParserTrackParam mParserTrackParam;
                /* 0x94 */ vs16 mTrackVariable[VARIABLE_NUM];
                /* 0xB4 */ SeqPlayer* mSeqPlayer;
                /* 0xB8 */ Channel* mChannelList;
            };

        }  // namespace detail
    }  // namespace snd
}  // namespace nw4hbm

#endif
