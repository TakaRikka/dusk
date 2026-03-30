#ifndef Z2INSTANCES_H
#define Z2INSTANCES_H

#include "Z2AudioLib/Z2WolfHowlMgr.h"
#include "Z2AudioLib/Z2DebugSys.h"
#include "JSystem/JAudio2/JASAudioThread.h"
#include "JSystem/JAudio2/JAUSoundTable.h"

#if TARGET_PC
#define AUDIO_INSTANCES
#else
#define AUDIO_INSTANCES                                                                            \
    template<> JASDefaultBankTable* JASGlobalInstance<JASDefaultBankTable>::sInstance;                        \
    template<> JASAudioThread* JASGlobalInstance<JASAudioThread>::sInstance;                                  \
    template<> Z2SeMgr* JASGlobalInstance<Z2SeMgr>::sInstance;                                                \
    template<> Z2SeqMgr* JASGlobalInstance<Z2SeqMgr>::sInstance;                                              \
    template<> Z2SceneMgr* JASGlobalInstance<Z2SceneMgr>::sInstance;                                          \
    template<> Z2StatusMgr* JASGlobalInstance<Z2StatusMgr>::sInstance;                                        \
    template<> Z2DebugSys* JASGlobalInstance<Z2DebugSys>::sInstance;                                          \
    template<> JAISoundStarter* JASGlobalInstance<JAISoundStarter>::sInstance;                                \
    template<> Z2SoundStarter* JASGlobalInstance<Z2SoundStarter>::sInstance;                                  \
    template<> Z2SpeechMgr2* JASGlobalInstance<Z2SpeechMgr2>::sInstance;                                      \
    template<> JAISeMgr* JASGlobalInstance<JAISeMgr>::sInstance;                                              \
    template<> JAISeqMgr* JASGlobalInstance<JAISeqMgr>::sInstance;                                            \
    template<> JAIStreamMgr* JASGlobalInstance<JAIStreamMgr>::sInstance;                                      \
    template<> Z2SoundMgr* JASGlobalInstance<Z2SoundMgr>::sInstance;                                          \
    template<> JAISoundInfo* JASGlobalInstance<JAISoundInfo>::sInstance;                                      \
    template<> JAUSoundTable* JASGlobalInstance<JAUSoundTable>::sInstance;                                    \
    template<> JAUSoundNameTable* JASGlobalInstance<JAUSoundNameTable>::sInstance;                            \
    template<> JAUSoundInfo* JASGlobalInstance<JAUSoundInfo>::sInstance;                                      \
    template<> Z2SoundInfo* JASGlobalInstance<Z2SoundInfo>::sInstance;                                        \
    template<> Z2SoundObjMgr* JASGlobalInstance<Z2SoundObjMgr>::sInstance;                                    \
    template<> Z2Audience* JASGlobalInstance<Z2Audience>::sInstance;                                          \
    template<> Z2FxLineMgr* JASGlobalInstance<Z2FxLineMgr>::sInstance;                                        \
    template<> Z2EnvSeMgr* JASGlobalInstance<Z2EnvSeMgr>::sInstance;                                          \
    template<> Z2SpeechMgr* JASGlobalInstance<Z2SpeechMgr>::sInstance;                                        \
    template<> Z2WolfHowlMgr* JASGlobalInstance<Z2WolfHowlMgr>::sInstance;
#endif

#endif
