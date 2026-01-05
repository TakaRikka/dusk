#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JAudio2/JASAramStream.h"
#include "JSystem/JAudio2/JASAiCtrl.h"
#include "JSystem/JAudio2/JASChannel.h"
#include "JSystem/JAudio2/JASCriticalSection.h"
#include "JSystem/JAudio2/JASDriverIF.h"
#include "JSystem/JAudio2/JASDSPInterface.h"
#include "JSystem/JAudio2/JASDvdThread.h"
#include "JSystem/JKernel/JKRAram.h"
#include "JSystem/JKernel/JKRSolidHeap.h"
#include "JSystem/JSupport/JSupport.h"

JASTaskThread* JASAramStream::sLoadThread;

u8* JASAramStream::sReadBuffer;

u32 JASAramStream::sBlockSize;

u32 JASAramStream::sChannelMax;

bool struct_80451260;
bool struct_80451261;

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
        sReadBuffer = new (JASDram, 0x20) u8[(block_size + 0x20) * channel_max];
        JUT_ASSERT(79, sReadBuffer);
        sBlockSize = block_size;
        sChannelMax = channel_max;
        struct_80451260 = false;
        struct_80451261 = false;
    }
}

JASAramStream::JASAramStream() {
    field_0x0a8 = NULL;
    field_0x0ac = false;
    field_0x0ad = false;
    field_0x0ae = 0;
    field_0x0b0 = 0;
    field_0x0b4 = 0;
    field_0x0b8 = 0;
    field_0x0bc = 0;
    field_0x0c0 = false;
    field_0x0c4 = 0;
    field_0x0c8 = 0.0f;
    field_0x108 = 0;
    field_0x10c = 0;
    mBlock = 0;
    field_0x114 = 0;
    field_0x118 = 0;
    field_0x12c = 0;
    field_0x148 = 0;
    field_0x14c = 0;
    mCallback = NULL;
    mCallbackData = NULL;
    field_0x158 = 0;
    mChannelNum = 0;
    mBufCount = 0;
    field_0x160 = 0;
    field_0x164 = 0;
    mLoop = false;
    mLoopStart = 0;
    mLoopEnd = 0;
    mVolume = 1.0f;
    mPitch = 1.0f;
    for (int i = 0; i < 6; i++) {
        mChannels[i] = NULL;
        field_0x130[i] = 0;
        field_0x13c[i] = 0;
        mChannelVolume[i] = 1.0f;
        mChannelPan[i] = 0.5f;
        mChannelFxMix[i] = 0.0f;
        mChannelDolby[i] = 0.0f;
    }
    for (int i = 0; i < 6; i++) {
        field_0x1dc[i] = 0;
    }
}

void JASAramStream::init(u32 param_0, u32 param_1, StreamCallback i_callback, void* i_callbackData) {
    JUT_ASSERT(153, sReadBuffer != NULL);
    field_0x148 = param_0;
    field_0x14c = param_1;
    field_0x0c8 = 0.0f;
    field_0x0ae = 0;
    field_0x0ac = false;
    field_0x0ad = false;
    field_0x114 = 0;
    mChannelNum = 0;
    for (int i = 0; i < 6; i++) {
        mChannelVolume[i] = 1.0f;
        mChannelPan[i] = 0.5f;
        mChannelFxMix[i] = 0.0f;
        mChannelDolby[i] = 0.0f;
    }
    mVolume = 1.0f;
    mPitch = 1.0f;
    field_0x1dc[0] = 0xffff;
    mCallback = i_callback;
    mCallbackData = i_callbackData;
    OSInitMessageQueue(&field_0x000, field_0x040, 0x10);
    OSInitMessageQueue(&field_0x020, field_0x080, 4);
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
    data.field_0x4 = field_0x14c;
    data.field_0x8 = param_1;
    if (!sLoadThread->sendCmdMsg(headerLoadTask, &data, 0xc)) {
        JUT_WARN(254, "%s", "sendCmdMsg headerLoadTask Failed");
        JASDriver::rejectCallback(channelProcCallback, this);
        return false;
    }
    return true;
}

bool JASAramStream::start() {
    if (!OSSendMessage(&field_0x000, (OSMessage)0, OS_MESSAGE_NOBLOCK)) {
        JUT_WARN(273, "%s", "OSSendMessage Failed")
        return false;
    }
    return true;
}

bool JASAramStream::stop(u16 param_0) {
    if (!OSSendMessage(&field_0x000, (OSMessage)(param_0 << 0x10 | 1), OS_MESSAGE_NOBLOCK)) {
        JUT_WARN(290, "%s", "OSSendMessage Failed");
        return false;
    }
    return true;
}

bool JASAramStream::pause(bool param_0) {
    OSMessage msg = param_0 ? (OSMessage)2 : (OSMessage)3;
    if (!OSSendMessage(&field_0x000, msg, OS_MESSAGE_NOBLOCK)) {
        JUT_WARN(308, "%s", "OSSendMessage Failed");
        return false;
    }
    return true;
}

bool JASAramStream::cancel() {
    field_0x114 = 1;
    if (!sLoadThread->sendCmdMsg(finishTask, this)) {
        JUT_WARN(326, "%s", "sendCmdMsg finishTask Failed");
        return false;
    }
    return true;
}

u32 JASAramStream::getBlockSamples() const {
    return field_0x158 == 0 ? (sBlockSize << 4) / 9 : sBlockSize >> 1;
}

void JASAramStream::headerLoadTask(void* i_data) {
    TaskData* data = (TaskData*)i_data;
    data->stream->headerLoad(data->field_0x4, data->field_0x8);
}

void JASAramStream::firstLoadTask(void* i_data) {
    TaskData* data = (TaskData*)i_data;
    JASAramStream* _this = data->stream;
    if (!_this->load()) {
        return;
    }
    if (data->field_0x8 > 0) {
        data->field_0x8--;
        if (data->field_0x8 == 0) {
            if (!sLoadThread->sendCmdMsg(prepareFinishTask, _this)) {
                JUT_WARN(364, "%s", "sendCmdMsg prepareFinishTask Failed");
                struct_80451261 = true;
            }
        }
    }
    if (data->field_0x4 != 0) {
        data->field_0x4--;
        if (!sLoadThread->sendCmdMsg(firstLoadTask, data, 0xc)) {
            JUT_WARN(372, "%s", "sendCmdMsg firstLoadTask Failed");
            struct_80451261 = true;
        }
        JASCriticalSection cs;
        _this->field_0x118++;
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
    OSSendMessage(&_this->field_0x020, (OSMessage)4, OS_MESSAGE_BLOCK);
    if (_this->mCallback != NULL) {
        _this->mCallback(CB_STOP, _this, _this->mCallbackData);
    }
}

bool JASAramStream::headerLoad(u32 param_0, int param_1) {
    if (struct_80451261) {
        return false;
    }
    if (field_0x114 != 0) {
        return false;
    }
    if (DVDReadPrio(&mDvdFileInfo, sReadBuffer, sizeof(Header), 0, 1) < 0) {
        JUT_WARN(420, "%s", "DVDReadPrio Failed");
        struct_80451261 = true;
        return false;
    }
    Header* header = (Header*)sReadBuffer;
    JUT_ASSERT(426, header->tag == 'STRM');
    JUT_ASSERT(427, header->format <= 1);
    JUT_ASSERT(428, header->bits == 16);
    JUT_ASSERT(429, header->channels <= sChannelMax);
    JUT_ASSERT(430, header->block_size == sBlockSize);
    field_0x158 = header->format;
    mChannelNum = header->channels;
    field_0x164 = header->field_0x10;
    mLoop = header->loop != 0;
    mLoopStart = header->loop_start;
    mLoopEnd = header->loop_end;
    mVolume = header->field_0x28 / 127.0f;
    field_0x118 = 0;
    mBlock = 0;
    field_0x10c = 0;
    field_0x160 = (param_0 / sBlockSize) / header->channels;
    mBufCount = field_0x160;
    JUT_ASSERT(445, mBufCount > 0);
    mBufCount--;
    if (mBufCount < 3) {
        JUT_WARN(449, "%s", "Too few Buffer-Size");
    }
    field_0x108 = mBufCount;
    u32 local_2c = (mLoopEnd - 1) / getBlockSamples();
    if (local_2c <= mBufCount && mLoop) {
        JUT_WARN(458, "%s", "Too few samples for Loop-buffer");
    }
    if (param_1 < 0 || param_1 > field_0x108) {
        param_1 = field_0x108;
    }
    if (field_0x114 != 0) {
        return false;
    }
    TaskData data;
    data.stream = this;
    data.field_0x4 = field_0x108 - 1;
    data.field_0x8 = param_1;
    if (!sLoadThread->sendCmdMsg(firstLoadTask, &data, 0xc)) {
        JUT_WARN(472, "%s", "sendCmdMsg firstLoadTask Failed");
        struct_80451261 = true;
        return false;
    }
    JASCriticalSection cs;
    field_0x118++;
    return true;
}


bool JASAramStream::load() {
    {
        JASCriticalSection cs;
        field_0x118--;
    }
    if (struct_80451261) {
        return false;
    }
    if (field_0x114 != 0) {
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
        struct_80451261 = true;
        return false;
    }
    BlockHeader* bhead = (BlockHeader*)sReadBuffer;
    JUT_ASSERT(512, bhead->tag == 'BLCK');
    if (field_0x114 != 0) {
        return false;
    }
    u32 sp08 = field_0x148 + field_0x10c * sBlockSize;
    for (int i = 0; i < mChannelNum; i++) {
        (void)i;
        // Fakematch? It seems the only way to get the bhead->field_0x4 load in the right order is
        // with a pointer cast on its address in one of the two places it is read, but not both.
        if (!JKRMainRamToAram(sReadBuffer + *(u32*)&bhead->field_0x4 * i + sizeof(BlockHeader),
                              sp08 + sBlockSize * field_0x160 * i,
                              bhead->field_0x4, EXPAND_SWITCH_UNKNOWN0, 0, NULL, -1, NULL)) {
            JUT_WARN(522, "%s", "JKRMainRamToAram Failed");
            struct_80451261 = 1;
            return false;
        }
    }
    field_0x10c++;
    if (field_0x10c >= field_0x108) {
        u32 r28 = mBlock;
        r28 += field_0x108 - 1;
        if (mLoop) {
            JUT_ASSERT(537, loop_start_block < loop_end_block);
            while (r28 > loop_end_block) {
                r28 -= loop_end_block;
                r28 += loop_start_block;
            }
        }
        if (r28 == loop_end_block || r28 + 2 == loop_end_block) {
            field_0x108 = field_0x160;
            OSSendMessage(&field_0x020, (OSMessage)5, OS_MESSAGE_BLOCK);
        } else {
            field_0x108 = field_0x160 - 1;
        }
        for (int i = 0; i < mChannelNum; i++) {
            field_0x130[i] = (s16)bhead->field_0x8[i].field_0x0;
            field_0x13c[i] = (s16)bhead->field_0x8[i].field_0x2;
        }
        field_0x10c = 0;
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
        struct_80451260 = false;
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
        struct_80451260 = true;
        break;
    }
    return 0;
}

void JASAramStream::channelCallback(u32 i_callbackType, JASChannel* i_channel,
                                    JASDsp::TChannel* i_dspChannel, void* i_this) {
    JASAramStream* stream = (JASAramStream*)i_this;
    stream->updateChannel(i_callbackType, i_channel, i_dspChannel);
}

void JASAramStream::updateChannel(u32 i_callbackType, JASChannel* i_channel,
                                  JASDsp::TChannel* i_dspChannel) {
    u32 block_samples = getBlockSamples();
    switch (i_callbackType) {
    case JASChannel::CB_START:
        if (field_0x0a8 == NULL) {
            field_0x0a8 = i_channel;
            field_0x0b4 = block_samples * mBufCount;
            field_0x0b8 = 0;
            field_0x0b0 = 0;
            field_0x0bc = (mLoopEnd - 1) / block_samples;
            field_0x0c0 = 0;
            field_0x0c4 = 0;
            field_0x12c = 0;
        }
        break;
    case JASChannel::CB_PLAY:
        if (i_dspChannel->field_0x008 == 0) {
            if (i_channel == field_0x0a8) {
                field_0x12c = 0;
                u32 sp28 = i_dspChannel->field_0x074 + i_dspChannel->field_0x064;
                if (sp28 <= field_0x0b4) {
                    field_0x0b8 += field_0x0b4 - sp28;
                } else {
                    if (!field_0x0c0) {
                        field_0x0b8 += field_0x0b4;
                        field_0x0b8 += block_samples * mBufCount - sp28;
                    } else {
                        field_0x0b8 += field_0x0b4;
                        field_0x0b8 += block_samples * mBufCount - sp28
                                       - i_dspChannel->field_0x110;
                        field_0x0b8 -= mLoopEnd;
                        field_0x0b8 += mLoopStart;
                        i_dspChannel->field_0x110 = 0;
                        field_0x120 = 0;
                        field_0x12c |= 2;
                        if (field_0x0c4 < 0xffffffff) {
                            field_0x0c4++;
                        }
                        field_0x0c0 = false;
                    }
                }
                if (field_0x0b8 > mLoopEnd) {
                    JUT_WARN(686, "%s", "mReadSample > mLoopEnd");
                    struct_80451261 = true;
                }
                f32 fvar1 = field_0x0c4;
                fvar1 *= mLoopEnd - mLoopStart;
                if (field_0x0c4 < 0xffffffff) {
                    fvar1 += field_0x0b8;
                }
                fvar1 /= field_0x164;
                field_0x0c8 = fvar1;
                if (field_0x0b8 + 400 >= mLoopEnd && !field_0x0c0) {
                    if (mLoop) {
                        u32 uvar5 = field_0x0bc + 1;
                        if (uvar5 >= mBufCount) {
                            uvar5 = 0;
                        }
                        i_dspChannel->field_0x110 = mLoopStart % block_samples
                                                    + uvar5 * block_samples;
                        field_0x120 = i_dspChannel->field_0x110;
                        field_0x12c |= 2;
                    } else {
                        i_dspChannel->field_0x102 = 0;
                        field_0x128 = 0;
                        field_0x12c |= 8;
                    }
                    int sp20 = field_0x0bc * block_samples + mLoopEnd % block_samples;
                    i_dspChannel->field_0x074 -= block_samples * mBufCount - sp20;
                    field_0x11c = i_dspChannel->field_0x074;
                    field_0x12c |= 1;
                    field_0x0bc += (mLoopEnd - 1) / block_samples - mLoopStart / block_samples + 1;
                    field_0x0c0 = true;
                }
                u32 uvar4 = i_dspChannel->field_0x070 - i_channel->field_0x104;
                if (uvar4 != 0) {
                    uvar4--;
                }
                u32 sp18 = uvar4 / sBlockSize;
                u32 sp14 = (mLoopEnd - 1) / getBlockSamples();
                if (sp18 != field_0x0b0) {
                    bool cmp = sp18 < field_0x0b0;
                    while (sp18 != field_0x0b0) {
                        if (!sLoadThread->sendCmdMsg(loadToAramTask, this)) {
                            JUT_WARN(741, "sendCmdMsg Failed %d %d (%d %d)", i_dspChannel->field_0x070, i_channel->field_0x104, sp18, field_0x0b0);
                            struct_80451261 = true;
                            break;
                        }
                        {
                            JASCriticalSection cs;
                            field_0x118++;
                        }
                        field_0x0b0++;
                        if (field_0x0b0 >= mBufCount) {
                            field_0x0b0 = 0;
                        }
                    }
                    if (cmp) {
                        field_0x0bc -= mBufCount;
                        if (field_0x0ad) {
                            if (!field_0x0c0) {
                                i_dspChannel->field_0x074 += block_samples;
                                field_0x11c = i_dspChannel->field_0x074;
                                field_0x12c |= 1;
                            }
                            i_dspChannel->field_0x114 += block_samples;
                            field_0x124 = i_dspChannel->field_0x114;
                            field_0x12c |= 4;
                            mBufCount = field_0x160;
                            field_0x0ad = false;
                        } else {
                            if (mBufCount != field_0x160 - 1) {
                                mBufCount = field_0x160 - 1;
                                i_dspChannel->field_0x114 -= block_samples;
                                field_0x124 = i_dspChannel->field_0x114;
                                field_0x12c |= 4;
                                if (!field_0x0c0) {
                                    i_dspChannel->field_0x074 -= block_samples;
                                    field_0x11c = i_dspChannel->field_0x074;
                                    field_0x12c |= 1;
                                }
                            }
                        }
                    }
                } else {
                    if (field_0x118 == 0 && !struct_80451260) {
                        field_0x0ae &= ~2;
                        field_0x0ae &= ~4;
                    }
                }
                field_0x0b4 = i_dspChannel->field_0x074 + i_dspChannel->field_0x064;
                if (field_0x118 >= field_0x160 - 2) {
                    JUT_WARN_DEVICE(810, 1, "%s", "buffer under error");
                    field_0x0ae |= (u8)4;
                }
            } else {
                if (field_0x12c & 1) {
                    i_dspChannel->field_0x074 = field_0x11c;
                }
                if (field_0x12c & 2) {
                    i_dspChannel->field_0x110 = field_0x120;
                }
                if (field_0x12c & 4) {
                    i_dspChannel->field_0x114 = field_0x124;
                }
                if (field_0x12c & 8) {
                    i_dspChannel->field_0x102 = field_0x128;
                }
            }
            int ch = 0;
            for (; ch < 6; ch++) {
                if (i_channel == mChannels[ch]) {
                    break;
                }
            }
            JUT_ASSERT(834, ch < CHANNEL_MAX);
            i_dspChannel->field_0x104 = (s16)field_0x130[ch];
            i_dspChannel->field_0x106 = (s16)field_0x13c[ch];
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
            field_0x114 = 1;
            if (!sLoadThread->sendCmdMsg(finishTask, this)) {
                JUT_WARN(854, "%s", "sendCmdMsg finishTask Failed");
                struct_80451261 = true;
                return;
            }
        }
        break;
    }
    i_channel->setPauseFlag(field_0x0ae != 0);
}

s32 JASAramStream::channelProc() {
    OSMessage msg;
    while (OSReceiveMessage(&field_0x020, &msg, OS_MESSAGE_NOBLOCK)) {
        switch ((u32)msg) {
        case 4:
            field_0x0ac = true;
            break;
        case 5:
            field_0x0ad = true;
            break;
        }
    }
    
    if (!field_0x0ac) {
        return 0;
    }

    while (OSReceiveMessage(&field_0x000, &msg, OS_MESSAGE_NOBLOCK)) {
        switch ((u32)msg & 0xff) {
        case 0:
            channelStart();
            break;
        case 1:
            channelStop(JSUHiHalf((u32)msg));
            break;
        case 2:
            field_0x0ae |= 1;
            break;
        case 3:
            field_0x0ae &= ~1;
            break;
        }
    }

    if (struct_80451261) {
        field_0x0ae |= 8;
    }
    if (struct_80451260) {
        field_0x0ae |= 2;
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
    u8 r31;
    switch (field_0x158) {
    case 0:
        r31 = 0;
        break;
    case 1:
        r31 = 3;
        break;
    }
    for (int i = 0; i < mChannelNum; i++) {
        JASWaveInfo wave_info;
        wave_info.field_0x00 = r31;
        wave_info.field_0x02 = 0xff;
        wave_info.field_0x10 = 0;
        wave_info.field_0x14 = mBufCount * getBlockSamples();
        wave_info.field_0x18 = wave_info.field_0x14;
        wave_info.field_0x1c = 0;
        wave_info.field_0x1e = 0;
        // probably a fake match, this should be set in the JASWaveInfo constructor
        static u32 const one = 1;
        wave_info.field_0x20 = &one;
        JASChannel* jc = new JASChannel(channelCallback, this);
        JUT_ASSERT(963, jc);
        jc->setPriority(0x7f7f);
        for (u32 j = 0; j < 6; j++) {
            jc->setMixConfig(j, field_0x1dc[j]);
        }
        jc->setInitPitch(field_0x164 / JASDriver::getDacRate());
        jc->setOscInit(0, &OSC_ENV);
        jc->field_0xdc.field_0x4 = wave_info;
        jc->field_0x104 = field_0x148 + sBlockSize * field_0x160 * i;
        jc->field_0xdc.field_0x0 = 0;
        int ret = jc->playForce();
        JUT_ASSERT(977, ret);
        JUT_ASSERT_MSG(979, mChannels[i] == NULL, "channelStart for already playing channel");
        mChannels[i] = jc;
    }
    field_0x0a8 = NULL;
}


void JASAramStream::channelStop(u16 i_directRelease) {
    for (int i = 0; i < mChannelNum; i++) {
        JASChannel* channel = mChannels[i];
        if (channel != NULL) {
            channel->release(i_directRelease);
        }
    }
}
