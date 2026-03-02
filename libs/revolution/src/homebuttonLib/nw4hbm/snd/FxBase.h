#ifndef NW4HBM_SND_FX_BASE_H
#define NW4HBM_SND_FX_BASE_H

#include <revolution/types.h>

#include "snd_global.h"
#include "snd_types.h"

#include "../ut/inlines.h"
#include "../ut/LinkList.h"

namespace nw4hbm {
namespace snd {
class FxBase : ut::NonCopyable {
public:
    /* 0x08 */ virtual ~FxBase() {}

    /* 0x0C */ virtual bool StartUp() { return true; }
    /* 0x10 */ virtual void Shutdown() {}
    /* 0x14 */ virtual void UpdateBuffer(int channels, void** buffer, u32 size, SampleFormat format, f32 sampleRate,
                                         OutputMode mode) {}

public:
    /* 0x04 */ ut::LinkListNode mFxLink;
};

typedef ut::LinkList<FxBase, offsetof(FxBase, mFxLink)> FxList;
} // namespace snd
} // namespace nw4hbm

#endif
