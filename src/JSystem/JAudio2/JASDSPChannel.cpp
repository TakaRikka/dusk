#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JAudio2/JASDSPChannel.h"
#include "JSystem/JAudio2/JASHeapCtrl.h"
#include "JSystem/JKernel/JKRSolidHeap.h"

JASDSPChannel* JASDSPChannel::sDspChannels;

JASDSPChannel::JASDSPChannel() :
    mStatus(STATUS_INACTIVE),
    mPriority(-1),
    mFlags(0),
    field_0xc(0),
    mCallback(NULL),
    mCallbackData(NULL),
    mChannel(NULL)
{
    /* empty function */
}

void JASDSPChannel::free() {
    mCallback = NULL;
    mCallbackData = NULL;
}

void JASDSPChannel::start() {
    mFlags |= 1;
}

void JASDSPChannel::drop() {
    if (mCallback != NULL) {
        mCallback(CB_DROP, NULL, mCallbackData);
    }
    mCallback = NULL;
    mCallbackData = NULL;
    mPriority = -1;
    mFlags &= ~1;
    if (mStatus == STATUS_ACTIVE) {
        mFlags |= 2;
        mStatus = STATUS_DROP;
    }
}

void JASDSPChannel::initAll() {
    sDspChannels = new (JASDram, 0x20) JASDSPChannel[0x40];
    for (int i = 0; i < 0x40; i++) {
        sDspChannels[i].mChannel = JASDsp::getDSPHandle(i);
    }
}

JASDSPChannel* JASDSPChannel::alloc(u8 i_priority, Callback i_callback, void* i_callbackData) {
    JASDSPChannel* channel = getLowestChannel(i_priority);
    if (channel == NULL) {
        return NULL;
    }
    channel->drop();
    channel->mPriority = i_priority;
    channel->field_0xc = 0;
    channel->mCallback = i_callback;
    channel->mCallbackData = i_callbackData;
    return channel;
}

JASDSPChannel* JASDSPChannel::allocForce(u8 i_priority, Callback i_callback, void* i_callbackData) {
    JASDSPChannel* channel = getLowestChannel(i_priority);
    if (channel == NULL) {
        return NULL;
    }
    channel->mStatus = STATUS_INACTIVE;
    channel->drop();
    channel->mPriority = i_priority;
    channel->field_0xc = 0;
    channel->mCallback = i_callback;
    channel->mCallbackData = i_callbackData;
    return channel;
}

void JASDSPChannel::setPriority(u8 i_priority) {
    mPriority = i_priority;
}

JASDSPChannel* JASDSPChannel::getLowestChannel(int i_priority) {
    s16 best_priority = 0xff;
    int best_index = -1;
    int best_unknown = 0;
    for (int i = 0; i < 0x40; i++) {
        JASDSPChannel* channel = &sDspChannels[i];
        s16 priority = channel->mPriority;
        if (priority < 0) {
            return channel;
        }
        if (priority <= i_priority && priority <= best_priority) {
            if (priority != best_priority || channel->field_0xc > best_unknown) {
                best_unknown = channel->field_0xc;
                best_index = i;
                best_priority = priority;
            }
        }
    }
    if (best_index < 0) {
        return NULL;
    }
    return &sDspChannels[best_index];
}

JASDSPChannel* JASDSPChannel::getLowestActiveChannel() {
    s16 best_priority = 0xff;
    int best_index = -1;
    int best_unknown = 0;
    for (int i = 0; i < 0x40; i++) {
        JASDSPChannel* channel = &sDspChannels[i];
        if (channel->mStatus == STATUS_ACTIVE) {
            s16 priority = channel->mPriority;
            if (priority < 0x7f && priority <= best_priority) {
                if (priority != best_priority || channel->field_0xc > best_unknown) {
                    best_unknown = channel->field_0xc;
                    best_index = i;
                    best_priority = priority;
                }
            }
        }
    }
    if (best_index < 0) {
        return NULL;
    }
    return &sDspChannels[best_index];
}

void JASDSPChannel::updateProc() {
    int ret;
    if (mChannel->isFinish()) {
        mFlags &= ~2;
        if (mStatus == STATUS_ACTIVE) {
            if (mCallback != NULL) {
                ret = mCallback(CB_STOP, NULL, mCallbackData);
            } else {
                ret = -1;
            }
            if (ret < 0) {
                mPriority = -1;
            }
        }
        mStatus = STATUS_INACTIVE;
        mChannel->replyFinishRequest();
        mChannel->flush();
    } else if (mFlags & 2) {
        mFlags &= ~2;
        mChannel->forceStop();
        mChannel->flush();
    } else if (mStatus != STATUS_DROP) {
        if ((mFlags & 1) && mStatus == STATUS_INACTIVE) {
            mFlags &= ~1;
            mStatus = STATUS_ACTIVE;
            mChannel->init();
            if (mCallback != NULL) {
                ret = mCallback(CB_START, mChannel, mCallbackData);
            } else {
                ret = 0;
            }
            if (ret < 0) {
                mStatus = STATUS_INACTIVE;
                if (mCallback != NULL) {
                    ret = mCallback(CB_STOP, NULL, mCallbackData);
                } else {
                    ret = -1;
                }
                if (ret < 0) {
                    mPriority = -1;
                }
                mChannel->flush();
            } else {
                mChannel->playStart();
                mChannel->flush();
            }
        } else if (mStatus != STATUS_INACTIVE) {
            bool flush = false;
            if (mCallback != NULL) {
                ret = mCallback(CB_PLAY, mChannel, mCallbackData);
                flush = true;
            } else {
                ret = 0;
            }
            if (ret < 0) {
                mStatus = STATUS_INACTIVE;
                if (mCallback != NULL) {
                    ret = mCallback(CB_STOP, NULL, mCallbackData);
                } else {
                    ret = -1;
                }
                if (ret < 0) {
                    mPriority = -1;
                }
                mChannel->playStop();
                mChannel->flush();
            } else {
                field_0xc++;
                if (flush) {
                    mChannel->flush();
                }
            }
        }
    }
}

void JASDSPChannel::updateAll() {
    for (u32 i = 0; i < 0x40; i++) {
        if ((i & 0xf) == 0 && i != 0) {
            JASDsp::releaseHalt((i - 1) >> 4);
        }
        sDspChannels[i].updateProc();
    }
    JASDsp::releaseHalt(3);
}

int JASDSPChannel::killActiveChannel() {
    JASDSPChannel* channel = getLowestActiveChannel();
    if (channel == NULL) {
        return 0;
    } else {
        channel->drop();
        return 1;
    }
}

JASDSPChannel* JASDSPChannel::getHandle(u32 i_index) {
    return &sDspChannels[i_index];
}
