#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JAudio2/JASAiCtrl.h"
#include "JSystem/JAudio2/JASAramStream.h"
#include "JSystem/JAudio2/JASAudioThread.h"
#include "JSystem/JAudio2/JASChannel.h"
#include "JSystem/JAudio2/JASCriticalSection.h"
#include "JSystem/JAudio2/JASDSPInterface.h"
#include "JSystem/JAudio2/JASDriverIF.h"
#include "JSystem/JAudio2/JASDvdThread.h"
#include "JSystem/JKernel/JKRAram.h"
#include "JSystem/JKernel/JKRSolidHeap.h"
#include "JSystem/JSupport/JSupport.h"

JASTaskThread* JASAramStream::sLoadThread;

u8* JASAramStream::sReadBuffer;

u32 JASAramStream::sBlockSize;

u32 JASAramStream::sChannelMax;

bool dvdHasErrored;
bool hasErrored;

#define PAUSE_REQUESTED   1
#define PAUSE_DVD_ERROR   2
#define PAUSE_UNDERFLOW   4
#define PAUSE_OTHER_ERROR 8

// CMDs for mMainCommandQueue.
#define CMD_START   0
#define CMD_STOP    1 // upper 16 bits of cmd contain oscillator direct release value.
#define CMD_PAUSE   2
#define CMD_UNPAUSE 3

// CMDs for mLoadCommandQueue
#define CMD_PREPARE_FINISHED 4
#define CMD_LOOP_END_LOADED  5

void JASAramStream::initSystem(u32 block_size, u32 channel_max) {
    JUT_ASSERT(66, block_size % 32 == 0);
    JUT_ASSERT(67, block_size % 9 == 0);
    JUT_ASSERT(68, channel_max > 0 && channel_max <= CHANNEL_MAX);
    JUT_ASSERT(69, sReadBuffer == 0);
    if (!JASDriver::registerSubFrameCallback(dvdErrorCheck, NULL)) {
        JUT_WARN(72, "%s", "registerSubFrameCallback Failed");
    } else {
        if (sLoadThread == NULL) {
            sLoadThread = JASDvd::getThreadPointer();
        }

        // Pretty sure this 0x20 corresponds to sizeof(BlockHeader).
        // But that shouldn't be getting multiplied by the channel count.
        // Bug in the original game, I guess?
        sReadBuffer = JKR_NEW_ARRAY_ARGS(u8, (block_size + 0x20) * channel_max, JASDram, 0x20);
        JUT_ASSERT(79, sReadBuffer);
        sBlockSize = block_size;
        sChannelMax = channel_max;
        dvdHasErrored = false;
        hasErrored = false;
    }
}

JASAramStream::JASAramStream() {
    mPrimaryChannel = NULL;
    mPrepareFinished = false;
    mLoopEndLoaded = false;
    mPauseFlags = 0;
    field_0x0b0 = 0;
    mLastSamplesLeft = 0;
    mReadSample = 0;
    field_0x0bc = 0;
    mEndSetup = false;
    field_0x0c4 = 0;
    field_0x0c8 = 0.0f;
    mRingEndIndex = 0;
    mBlockRingIndex = 0;
    mBlock = 0;
    mIsCancelled = 0;
    mPendingLoadTasks = 0;
    mChannelUpdateFlags = 0;
    mAramAddress = 0;
    mAramSize = 0;
    mCallback = NULL;
    mCallbackData = NULL;
    mFormat = 0;
    mChannelNum = 0;
    mBufCount = 0;
    mAramBlocksPerChannel = 0;
    mSampleRate = 0;
    mLoop = false;
    mLoopStart = 0;
    mLoopEnd = 0;
    mVolume = 1.0f;
    mPitch = 1.0f;
    for (int i = 0; i < 6; i++) {
        mChannels[i] = NULL;
        mpLasts[i] = 0;
        mpPenults[i] = 0;
        mChannelVolume[i] = 1.0f;
        mChannelPan[i] = 0.5f;
        mChannelFxMix[i] = 0.0f;
        mChannelDolby[i] = 0.0f;
    }
    for (int i = 0; i < 6; i++) {
        mMixConfig[i] = 0;
    }
}

void JASAramStream::init(u32 aramAddress, u32 aramSize, StreamCallback i_callback, void* i_callbackData) {
    JUT_ASSERT(153, sReadBuffer != NULL);
    mAramAddress = aramAddress;
    mAramSize = aramSize;
    field_0x0c8 = 0.0f;
    mPauseFlags = 0;
    mPrepareFinished = false;
    mLoopEndLoaded = false;
    mIsCancelled = 0;
    mChannelNum = 0;
    for (int i = 0; i < 6; i++) {
        mChannelVolume[i] = 1.0f;
        mChannelPan[i] = 0.5f;
        mChannelFxMix[i] = 0.0f;
        mChannelDolby[i] = 0.0f;
    }
    mVolume = 1.0f;
    mPitch = 1.0f;
    mMixConfig[0] = 0xffff;
    mCallback = i_callback;
    mCallbackData = i_callbackData;
    OSInitMessageQueue(&mMainCommandQueue, mMainCommandQueueArray, ARRAY_SIZEU(mMainCommandQueueArray));
    OSInitMessageQueue(&mLoadCommandQueue, mLoadCommandQueueArray, ARRAY_SIZEU(mLoadCommandQueueArray));
}

bool JASAramStream::prepare(s32 param_0, int param_1) {
    if (!DVDFastOpen(param_0, &mDvdFileInfo)) {
        JUT_WARN(240, "%s", "DVDFastOpen Failed");
        return false;
    }
    if (!JASDriver::registerSubFrameCallback(channelProcCallback, this)) {
        JUT_WARN(245, "%s", "registerSubFrameCallback Failed");
        return false;
    }
    TaskData data;
    data.stream = this;
    data.param0 = mAramSize;
    data.param1 = param_1;
    if (!sLoadThread->sendCmdMsg(headerLoadTask, &data, sizeof(data))) {
        JUT_WARN(254, "%s", "sendCmdMsg headerLoadTask Failed");
        JASDriver::rejectCallback(channelProcCallback, this);
        return false;
    }
    return true;
}

bool JASAramStream::start() {
    if (!OSSendMessage(&mMainCommandQueue, (OSMessage)CMD_START, OS_MESSAGE_NOBLOCK)) {
        JUT_WARN(273, "%s", "OSSendMessage Failed")
        return false;
    }
    return true;
}

bool JASAramStream::stop(u16 directRelease) {
    if (!OSSendMessage(&mMainCommandQueue, (OSMessage)(uintptr_t)(directRelease << 0x10 | CMD_STOP), OS_MESSAGE_NOBLOCK)) {
        JUT_WARN(290, "%s", "OSSendMessage Failed");
        return false;
    }
    return true;
}

bool JASAramStream::pause(bool newPauseFlag) {
    OSMessage msg = newPauseFlag ? (OSMessage)CMD_PAUSE : (OSMessage)CMD_UNPAUSE;
    if (!OSSendMessage(&mMainCommandQueue, msg, OS_MESSAGE_NOBLOCK)) {
        JUT_WARN(308, "%s", "OSSendMessage Failed");
        return false;
    }
    return true;
}

bool JASAramStream::cancel() {
    mIsCancelled = 1;
    if (!sLoadThread->sendCmdMsg(finishTask, this)) {
        JUT_WARN(326, "%s", "sendCmdMsg finishTask Failed");
        return false;
    }
    return true;
}

u32 JASAramStream::getBlockSamples() const {
    return mFormat == STREAM_FORMAT_ADPCM4 ? (sBlockSize << 4) / 9 : sBlockSize >> 1;
}

void JASAramStream::headerLoadTask(void* i_data) {
    TaskData* data = (TaskData*)i_data;
    data->stream->headerLoad(data->param0, data->param1);
}

void JASAramStream::firstLoadTask(void* i_data) {
    TaskData* data = (TaskData*)i_data;
    JASAramStream* _this = data->stream;
    if (!_this->load()) {
        return;
    }
    if (data->param1 > 0) {
        data->param1--;
        if (data->param1 == 0) {
            if (!sLoadThread->sendCmdMsg(prepareFinishTask, _this)) {
                JUT_WARN(364, "%s", "sendCmdMsg prepareFinishTask Failed");
                hasErrored = true;
            }
        }
    }
    if (data->param0 != 0) {
        data->param0--;
        if (!sLoadThread->sendCmdMsg(firstLoadTask, data, sizeof(*data))) {
            JUT_WARN(372, "%s", "sendCmdMsg firstLoadTask Failed");
            hasErrored = true;
        }
        JASCriticalSection cs;
        _this->mPendingLoadTasks++;
    }
}

void JASAramStream::loadToAramTask(void* i_this) {
    JASAramStream* stream = (JASAramStream*)i_this;
    stream->load();
}

void JASAramStream::finishTask(void* i_this) {
    JASAramStream* _this = (JASAramStream*)i_this;
    if (!JASDriver::rejectCallback(channelProcCallback, _this)) {
        JUT_WARN(392, "%s", "rejectSubFrameCallback Failed");
    }
    if (_this->mCallback != NULL) {
        _this->mCallback(CB_START, _this, _this->mCallbackData);
        _this->mCallback = NULL;
    }
}

void JASAramStream::prepareFinishTask(void* i_this) {
    JASAramStream* _this = (JASAramStream*)i_this;
    OSSendMessage(&_this->mLoadCommandQueue, (OSMessage)CMD_PREPARE_FINISHED, OS_MESSAGE_BLOCK);
    if (_this->mCallback != NULL) {
        _this->mCallback(CB_STOP, _this, _this->mCallbackData);
    }
}

bool JASAramStream::headerLoad(u32 aramSize, int param_1) {
    if (hasErrored) {
        return false;
    }
    if (mIsCancelled != 0) {
        return false;
    }
    if (DVDReadPrio(&mDvdFileInfo, sReadBuffer, sizeof(Header), 0, 1) < 0) {
        JUT_WARN(420, "%s", "DVDReadPrio Failed");
        hasErrored = true;
        return false;
    }
    Header* header = (Header*)sReadBuffer;
    JUT_ASSERT(426, header->tag == 'STRM');
    JUT_ASSERT(427, header->format <= 1);
    JUT_ASSERT(428, header->bits == 16);
    JUT_ASSERT(429, header->channels <= sChannelMax);
    JUT_ASSERT(430, header->block_size == sBlockSize);
    mFormat = header->format;
    mChannelNum = header->channels;
    mSampleRate = header->mSampleRate;
    mLoop = header->loop != 0;
    mLoopStart = header->loop_start;
    mLoopEnd = header->loop_end;
    mVolume = header->mVolume / 127.0f;
    mPendingLoadTasks = 0;
    mBlock = 0;
    mBlockRingIndex = 0;
    mAramBlocksPerChannel = (aramSize / sBlockSize) / header->channels;
    mBufCount = mAramBlocksPerChannel;
    JUT_ASSERT(445, mBufCount > 0);
    mBufCount--;
    if (mBufCount < 3) {
        JUT_WARN(449, "%s", "Too few Buffer-Size");
    }
    mRingEndIndex = mBufCount;
    u32 local_2c = (mLoopEnd - 1) / getBlockSamples();
    if (local_2c <= mBufCount && mLoop) {
        JUT_WARN(458, "%s", "Too few samples for Loop-buffer");
    }
    if (param_1 < 0 || param_1 > mRingEndIndex) {
        param_1 = mRingEndIndex;
    }
    if (mIsCancelled != 0) {
        return false;
    }
    TaskData data;
    data.stream = this;
    data.param0 = mRingEndIndex - 1;
    data.param1 = param_1;
    if (!sLoadThread->sendCmdMsg(firstLoadTask, &data, sizeof(data))) {
        JUT_WARN(472, "%s", "sendCmdMsg firstLoadTask Failed");
        hasErrored = true;
        return false;
    }
    JASCriticalSection cs;
    mPendingLoadTasks++;
    return true;
}


bool JASAramStream::load() {
    {
        JASCriticalSection cs;
        mPendingLoadTasks--;
    }
    if (hasErrored) {
        return false;
    }
    if (mIsCancelled != 0) {
        return false;
    }
    u32 loop_end_block = (mLoopEnd - 1) / getBlockSamples();
    u32 loop_start_block = mLoopStart / getBlockSamples();
    if (mBlock > loop_end_block) {
        return false;
    }
    u32 offset = mBlock * (sBlockSize * mChannelNum + sizeof(BlockHeader)) + sizeof(Header);
    u32 size = sBlockSize * mChannelNum + sizeof(BlockHeader);
    if (mBlock == loop_end_block) {
        size = mDvdFileInfo.length - offset;
    }
    if (DVDReadPrio(&mDvdFileInfo, sReadBuffer, size, offset, 1) < 0) {
        JUT_WARN(507, "%s", "DVDReadPrio Failed");
        hasErrored = true;
        return false;
    }
    BlockHeader* bhead = (BlockHeader*)sReadBuffer;
    JUT_ASSERT(512, bhead->tag == 'BLCK');
    if (mIsCancelled != 0) {
        return false;
    }
    u32 blockBaseOffset = mAramAddress + mBlockRingIndex * sBlockSize;
    for (int i = 0; i < mChannelNum; i++) {
        (void)i;
        // Fakematch? It seems the only way to get the bhead->field_0x4 load in the right order is
        // with a pointer cast on its address in one of the two places it is read, but not both.
        if (!JKRMainRamToAram(sReadBuffer + bhead->mSize * i + sizeof(BlockHeader),
                              blockBaseOffset + sBlockSize * mAramBlocksPerChannel * i,
                              bhead->mSize, EXPAND_SWITCH_UNKNOWN0, 0, NULL, -1, NULL)) {
            JUT_WARN(522, "%s", "JKRMainRamToAram Failed");
            hasErrored = 1;
            return false;
        }
    }
    mBlockRingIndex++;
    if (mBlockRingIndex >= mRingEndIndex) {
        u32 r28 = mBlock;
        r28 += mRingEndIndex - 1;
        if (mLoop) {
            JUT_ASSERT(537, loop_start_block < loop_end_block);
            while (r28 > loop_end_block) {
                r28 -= loop_end_block;
                r28 += loop_start_block;
            }
        }
        if (r28 == loop_end_block || r28 + 2 == loop_end_block) {
            mRingEndIndex = mAramBlocksPerChannel;
            OSSendMessage(&mLoadCommandQueue, (OSMessage)CMD_LOOP_END_LOADED, OS_MESSAGE_BLOCK);
        } else {
            mRingEndIndex = mAramBlocksPerChannel - 1;
        }
        for (int i = 0; i < mChannelNum; i++) {
            mpLasts[i] = (s16)bhead->mAdpcmContinuationData[i].mpLast;
            mpPenults[i] = (s16)bhead->mAdpcmContinuationData[i].mpPenult;
        }
        mBlockRingIndex = 0;
    }
    mBlock++;
    if (mBlock > loop_end_block && mLoop) {
        mBlock = loop_start_block;
    }
    return true;
}

s32 JASAramStream::channelProcCallback(void* i_this) {
    JASAramStream* stream = (JASAramStream*)i_this;
    return stream->channelProc();
}

s32 JASAramStream::dvdErrorCheck(void* param_0) {
    s32 status = DVDGetDriveStatus();
    switch (status) {
    case DVD_STATE_END:
        dvdHasErrored = false;
        break;
    case DVD_STATE_BUSY:
        break;
    case DVD_STATE_WAITING:
    case DVD_STATE_COVER_CLOSED:
    case DVD_STATE_NO_DISK:
    case DVD_STATE_WRONG_DISK:
    case DVD_STATE_MOTOR_STOPPED:
    case DVD_STATE_IGNORED:
    case DVD_STATE_CANCELED:
    case DVD_STATE_RETRY:
    case DVD_STATE_FATAL_ERROR:
    default:
        dvdHasErrored = true;
        break;
    }
    return 0;
}

void JASAramStream::channelCallback(u32 i_callbackType, JASChannel* i_channel,
                                    JASDsp::TChannel* i_dspChannel, void* i_this) {
    JASAramStream* stream = (JASAramStream*)i_this;
    stream->updateChannel(i_callbackType, i_channel, i_dspChannel);
}

#define CHANNEL_UPDATE_SAMPLES_LEFT 1
#define CHANNEL_UPDATE_LOOP_START   2
#define CHANNEL_UPDATE_END_SAMPLE   4
#define CHANNEL_UPDATE_LOOP_FLAG    8


void JASAramStream::updateChannel(u32 i_callbackType, JASChannel* i_channel,
                                  JASDsp::TChannel* i_dspChannel) {
    u32 block_samples = getBlockSamples();
    switch (i_callbackType) {
    case JASChannel::CB_START:
        if (mPrimaryChannel == NULL) {
            mPrimaryChannel = i_channel;
            mLastSamplesLeft = block_samples * mBufCount;
            mReadSample = 0;
            field_0x0b0 = 0;
            field_0x0bc = (mLoopEnd - 1) / block_samples;
            mEndSetup = 0;
            field_0x0c4 = 0;
            mChannelUpdateFlags = 0;
        }
        break;
    case JASChannel::CB_PLAY:
        if (i_dspChannel->mResetFlag == 0) {
            if (i_channel == mPrimaryChannel) {
                /*
                if (JASAudioThread::snIntCount == 1) {
                    OSReportForceEnableOn();
                    OSReport("mSamplesLeft: %08d, mAramStreamPosition: %08d\n", i_dspChannel->mSamplesLeft, i_dspChannel->mAramStreamPosition);
                }
                */

                mChannelUpdateFlags = 0;
                u32 adjustedSamplesLeft = i_dspChannel->mSamplesLeft + i_dspChannel->mSamplesPerBlock;
                if (adjustedSamplesLeft <= mLastSamplesLeft) {
                    mReadSample += mLastSamplesLeft - adjustedSamplesLeft;
                } else {
                    // The DSP has looped.

                    if (!mEndSetup) {
                        // Just looping the ring buffer, data continues as normal.
                        mReadSample += mLastSamplesLeft;
                        mReadSample += block_samples * mBufCount - adjustedSamplesLeft;
                    } else {
                        // We hit the actual file loop position.
                        mReadSample += mLastSamplesLeft;
                        mReadSample += block_samples * mBufCount - adjustedSamplesLeft
                                       - i_dspChannel->mLoopStartSample;
                        mReadSample -= mLoopEnd;
                        mReadSample += mLoopStart;
                        i_dspChannel->mLoopStartSample = 0;
                        mUpdateLoopStartSample = 0;
                        mChannelUpdateFlags |= CHANNEL_UPDATE_LOOP_START;
#if !TARGET_PC // The variable assigned here is never used.
                        if (field_0x0c4 < 0xffffffff) {
                            field_0x0c4++;
                        }
#endif
                        mEndSetup = false;
                    }
                }
                if (mReadSample > mLoopEnd) {
                    JUT_WARN(686, "%s", "mReadSample > mLoopEnd");
                    hasErrored = true;
                }

#if !TARGET_PC // The variable assigned here is never used.
                f32 fvar1 = field_0x0c4;
                fvar1 *= mLoopEnd - mLoopStart;
                if (field_0x0c4 < 0xffffffff) {
                    fvar1 += mReadSample;
                }
                fvar1 /= mSampleRate;
                field_0x0c8 = fvar1;
#endif

                if (mReadSample + 400 >= mLoopEnd && !mEndSetup) {
                    if (mLoop) {
                        // File needs to loop. Adjust loop start position
                        // (out of the normal ring buffer behavior).
                        u32 uvar5 = field_0x0bc + 1;
                        if (uvar5 >= mBufCount) {
                            uvar5 = 0;
                        }
                        i_dspChannel->mLoopStartSample = mLoopStart % block_samples
                                                    + uvar5 * block_samples;
                        mUpdateLoopStartSample = i_dspChannel->mLoopStartSample;
                        mChannelUpdateFlags |= CHANNEL_UPDATE_LOOP_START;
                    } else {
                        // File doesn't need to loop, just unset loop flag
                        // and let the DSP finish naturally.
                        i_dspChannel->mLoopFlag = 0;
                        mUpdateLoopFlag = 0;
                        mChannelUpdateFlags |= CHANNEL_UPDATE_LOOP_FLAG;
                    }
                    int sp20 = field_0x0bc * block_samples + mLoopEnd % block_samples;
                    i_dspChannel->mSamplesLeft -= block_samples * mBufCount - sp20;
                    mUpdateSamplesLeft = i_dspChannel->mSamplesLeft;
                    mChannelUpdateFlags |= CHANNEL_UPDATE_SAMPLES_LEFT;
                    field_0x0bc += (mLoopEnd - 1) / block_samples - mLoopStart / block_samples + 1;
                    mEndSetup = true;
                }
                u32 uvar4 = i_dspChannel->mAramStreamPosition - i_channel->mWaveAramAddress;
                if (uvar4 != 0) {
                    uvar4--;
                }
                u32 blockCount = uvar4 / sBlockSize;
                u32 sp14 = (mLoopEnd - 1) / getBlockSamples();
                if (blockCount != field_0x0b0) {
                    bool cmp = blockCount < field_0x0b0;
                    while (blockCount != field_0x0b0) {
                        if (!sLoadThread->sendCmdMsg(loadToAramTask, this)) {
                            JUT_WARN(741, "sendCmdMsg Failed %d %d (%d %d)", i_dspChannel->mAramStreamPosition, i_channel->mWaveAramAddress, blockCount, field_0x0b0);
                            hasErrored = true;
                            break;
                        }
                        {
                            JASCriticalSection cs;
                            mPendingLoadTasks++;
                        }
                        field_0x0b0++;
                        if (field_0x0b0 >= mBufCount) {
                            field_0x0b0 = 0;
                        }
                    }
                    if (cmp) {
                        field_0x0bc -= mBufCount;
                        if (mLoopEndLoaded) {
                            if (!mEndSetup) {
                                i_dspChannel->mSamplesLeft += block_samples;
                                mUpdateSamplesLeft = i_dspChannel->mSamplesLeft;
                                mChannelUpdateFlags |= CHANNEL_UPDATE_SAMPLES_LEFT;
                            }
                            i_dspChannel->mEndSample += block_samples;
                            mUpdateEndSample = i_dspChannel->mEndSample;
                            mChannelUpdateFlags |= CHANNEL_UPDATE_END_SAMPLE;
                            mBufCount = mAramBlocksPerChannel;
                            mLoopEndLoaded = false;
                        } else {
                            if (mBufCount != mAramBlocksPerChannel - 1) {
                                mBufCount = mAramBlocksPerChannel - 1;
                                i_dspChannel->mEndSample -= block_samples;
                                mUpdateEndSample = i_dspChannel->mEndSample;
                                mChannelUpdateFlags |= CHANNEL_UPDATE_END_SAMPLE;
                                if (!mEndSetup) {
                                    i_dspChannel->mSamplesLeft -= block_samples;
                                    mUpdateSamplesLeft = i_dspChannel->mSamplesLeft;
                                    mChannelUpdateFlags |= CHANNEL_UPDATE_SAMPLES_LEFT;
                                }
                            }
                        }
                    }
                } else {
                    if (mPendingLoadTasks == 0 && !dvdHasErrored) {
                        mPauseFlags &= ~PAUSE_DVD_ERROR;
                        mPauseFlags &= ~PAUSE_UNDERFLOW;
                    }
                }
                mLastSamplesLeft = i_dspChannel->mSamplesLeft + i_dspChannel->mSamplesPerBlock;
                if (mPendingLoadTasks >= mAramBlocksPerChannel - 2) {
                    JUT_WARN_DEVICE(810, 1, "%s", "buffer under error");
                    mPauseFlags |= (u8)PAUSE_UNDERFLOW;
                }
            } else {
                if (mChannelUpdateFlags & CHANNEL_UPDATE_SAMPLES_LEFT) {
                    i_dspChannel->mSamplesLeft = mUpdateSamplesLeft;
                }
                if (mChannelUpdateFlags & CHANNEL_UPDATE_LOOP_START) {
                    i_dspChannel->mLoopStartSample = mUpdateLoopStartSample;
                }
                if (mChannelUpdateFlags & CHANNEL_UPDATE_END_SAMPLE) {
                    i_dspChannel->mEndSample = mUpdateEndSample;
                }
                if (mChannelUpdateFlags & CHANNEL_UPDATE_LOOP_FLAG) {
                    i_dspChannel->mLoopFlag = mUpdateLoopFlag;
                }
            }
            int ch = 0;
            for (; ch < CHANNEL_MAX; ch++) {
                if (i_channel == mChannels[ch]) {
                    break;
                }
            }
            JUT_ASSERT(834, ch < CHANNEL_MAX);
            i_dspChannel->mpLast = (s16)mpLasts[ch];
            i_dspChannel->mpPenult = (s16)mpPenults[ch];
        }
        break;
    case JASChannel::CB_STOP:
        bool open_channel = false;
        for (int i = 0; i < 6; i++) {
            if (i_channel == mChannels[i]) {
                mChannels[i] = NULL;
            } else if (mChannels[i] != NULL) {
                open_channel = true;
            }
        }
        if (!open_channel) {
            mIsCancelled = 1;
            if (!sLoadThread->sendCmdMsg(finishTask, this)) {
                JUT_WARN(854, "%s", "sendCmdMsg finishTask Failed");
                hasErrored = true;
                return;
            }
        }
        break;
    }
    i_channel->setPauseFlag(mPauseFlags != 0);
}

s32 JASAramStream::channelProc() {
    OSMessage msg;
    while (OSReceiveMessage(&mLoadCommandQueue, &msg, OS_MESSAGE_NOBLOCK)) {
        switch ((uintptr_t)msg) {
        case CMD_PREPARE_FINISHED:
            mPrepareFinished = true;
            break;
        case CMD_LOOP_END_LOADED:
            mLoopEndLoaded = true;
            break;
        }
    }
    
    if (!mPrepareFinished) {
        return 0;
    }

    while (OSReceiveMessage(&mMainCommandQueue, &msg, OS_MESSAGE_NOBLOCK)) {
        switch ((uintptr_t)msg & 0xff) {
        case CMD_START:
            channelStart();
            break;
        case CMD_STOP:
            channelStop(JSUHiHalf((uintptr_t)msg));
            break;
        case CMD_PAUSE:
            mPauseFlags |= PAUSE_REQUESTED;
            break;
        case CMD_UNPAUSE:
            mPauseFlags &= ~PAUSE_REQUESTED;
            break;
        }
    }

    if (hasErrored) {
        mPauseFlags |= PAUSE_OTHER_ERROR;
    }
    if (dvdHasErrored) {
        mPauseFlags |= PAUSE_DVD_ERROR;
    }

    for (int i = 0; i < mChannelNum; i++) {
        JASChannel* channel = mChannels[i];
        if (channel != NULL) {
            JASChannelParams params;
            params.mVolume = mVolume * mChannelVolume[i];
            params.mPitch = mPitch;
            params.field_0x8 = 0.0f;
            params.mPan = mChannelPan[i];
            params.mFxMix = mChannelFxMix[i];
            params.mDolby = mChannelDolby[i];
            channel->setParams(params);
        }
    }
    
    return 0;
}

static JASOscillator::Point const OSC_RELEASE_TABLE[2] = {
    {0x0000, 0x0002, 0x0000},
    {0x000F, 0x0000, 0x0000},
};

static JASOscillator::Data const OSC_ENV = {0, 1.0f, NULL, OSC_RELEASE_TABLE, 1.0f, 0.0f};

void JASAramStream::channelStart() {
    u8 format;
    switch (mFormat) {
    case STREAM_FORMAT_ADPCM4:
        format = WAVE_FORMAT_ADPCM4;
        break;
    case STREAM_FORMAT_PCM16:
        format = WAVE_FORMAT_PCM16;
        break;
    }
    for (int i = 0; i < mChannelNum; i++) {
        JASWaveInfo wave_info;
        wave_info.mWaveFormat = format;
        wave_info.mLoopFlag = 0xff;
        wave_info.mLoopStartSample = 0;
        wave_info.mLoopEndSample = mBufCount * getBlockSamples();
        wave_info.mSampleCount = wave_info.mLoopEndSample;
        wave_info.mpLast = 0;
        wave_info.mpPenult = 0;
        // probably a fake match, this should be set in the JASWaveInfo constructor
        static u32 const one = 1;
        wave_info.field_0x20 = &one;
        JASChannel* jc = JKR_NEW JASChannel(channelCallback, this);
        JUT_ASSERT(963, jc);
        jc->setPriority(0x7f7f);
        for (u32 j = 0; j < DSP_OUTPUT_CHANNELS; j++) {
            jc->setMixConfig(j, mMixConfig[j]);
        }
        jc->setInitPitch(mSampleRate / JASDriver::getDacRate());
        jc->setOscInit(0, &OSC_ENV);
        jc->field_0xdc.mWaveInfo = wave_info;
        jc->mWaveAramAddress = mAramAddress + sBlockSize * mAramBlocksPerChannel * i;
        jc->field_0xdc.mChannelType = 0;
        int ret = jc->playForce();
        JUT_ASSERT(977, ret);
        JUT_ASSERT_MSG(979, mChannels[i] == NULL, "channelStart for already playing channel");
        mChannels[i] = jc;
    }
    mPrimaryChannel = NULL;
}


void JASAramStream::channelStop(u16 i_directRelease) {
    for (int i = 0; i < mChannelNum; i++) {
        JASChannel* channel = mChannels[i];
        if (channel != NULL) {
            channel->release(i_directRelease);
        }
    }
}
