#include <cstdlib>
#include <dolphin/dolphin.h>
#include <dolphin/card.h>

#include "__card.h"

static u8 CardData[352] ATTRIBUTE_ALIGN(DOLPHIN_ALIGNMENT) = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x21, 0x02, 0xFF, 0x00, 0x21, 0x13, 0x06, 0x12, 0x03, 0x12, 0x04,
    0x13, 0x05, 0x00, 0x92, 0x00, 0xFF, 0x00, 0x88, 0xFF, 0xFF, 0x00, 0x89, 0xFF, 0xFF, 0x00, 0x8A, 0xFF, 0xFF, 0x00,
    0x8B, 0xFF, 0xFF, 0x8F, 0x00, 0x02, 0xBF, 0x00, 0x88, 0x16, 0xFC, 0xDC, 0xD1, 0x16, 0xFD, 0x00, 0x00, 0x16, 0xFB,
    0x00, 0x01, 0x02, 0xBF, 0x00, 0x8E, 0x25, 0xFF, 0x03, 0x80, 0xFF, 0x00, 0x02, 0x94, 0x00, 0x27, 0x02, 0xBF, 0x00,
    0x8E, 0x1F, 0xDF, 0x24, 0xFF, 0x02, 0x40, 0x0F, 0xFF, 0x00, 0x98, 0x04, 0x00, 0x00, 0x9A, 0x00, 0x10, 0x00, 0x99,
    0x00, 0x00, 0x8E, 0x00, 0x02, 0xBF, 0x00, 0x94, 0x02, 0xBF, 0x86, 0x44, 0x02, 0xBF, 0x00, 0x88, 0x16, 0xFC, 0xDC,
    0xD1, 0x16, 0xFD, 0x00, 0x03, 0x16, 0xFB, 0x00, 0x01, 0x8F, 0x00, 0x02, 0xBF, 0x00, 0x8E, 0x03, 0x80, 0xCD, 0xD1,
    0x02, 0x94, 0x00, 0x48, 0x27, 0xFF, 0x03, 0x80, 0x00, 0x01, 0x02, 0x95, 0x00, 0x5A, 0x03, 0x80, 0x00, 0x02, 0x02,
    0x95, 0x80, 0x00, 0x02, 0x9F, 0x00, 0x48, 0x00, 0x21, 0x8E, 0x00, 0x02, 0xBF, 0x00, 0x8E, 0x25, 0xFF, 0x02, 0xBF,
    0x00, 0x8E, 0x25, 0xFF, 0x02, 0xBF, 0x00, 0x8E, 0x25, 0xFF, 0x02, 0xBF, 0x00, 0x8E, 0x00, 0xC5, 0xFF, 0xFF, 0x03,
    0x40, 0x0F, 0xFF, 0x1C, 0x9F, 0x02, 0xBF, 0x00, 0x8E, 0x00, 0xC7, 0xFF, 0xFF, 0x02, 0xBF, 0x00, 0x8E, 0x00, 0xC6,
    0xFF, 0xFF, 0x02, 0xBF, 0x00, 0x8E, 0x00, 0xC0, 0xFF, 0xFF, 0x02, 0xBF, 0x00, 0x8E, 0x20, 0xFF, 0x03, 0x40, 0x0F,
    0xFF, 0x1F, 0x5F, 0x02, 0xBF, 0x00, 0x8E, 0x21, 0xFF, 0x02, 0xBF, 0x00, 0x8E, 0x23, 0xFF, 0x12, 0x05, 0x12, 0x06,
    0x02, 0x9F, 0x80, 0xB5, 0x00, 0x21, 0x27, 0xFC, 0x03, 0xC0, 0x80, 0x00, 0x02, 0x9D, 0x00, 0x88, 0x02, 0xDF, 0x27,
    0xFE, 0x03, 0xC0, 0x80, 0x00, 0x02, 0x9C, 0x00, 0x8E, 0x02, 0xDF, 0x2E, 0xCE, 0x2C, 0xCF, 0x00, 0xF8, 0xFF, 0xCD,
    0x00, 0xF9, 0xFF, 0xC9, 0x00, 0xFA, 0xFF, 0xCB, 0x26, 0xC9, 0x02, 0xC0, 0x00, 0x04, 0x02, 0x9D, 0x00, 0x9C, 0x02,
    0xDF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

static u32 next = 1;

// prototypes
static u32 exnor_1st(u32 data, u32 rshift);
static u32 exnor(u32 data, u32 lshift);
static u32 bitrev(u32 data);
static s32 ReadArrayUnlock(s32 chan, u32 data, void* rbuf, s32 rlen, int mode);
static u32 GetInitVal(void);
static s32 DummyLen(void);
static void InitCallback(void* _task);
static void DoneCallback(void* _task);

static int CARDRand(void) {
    next = (next * 0x41C64E6D) + 0x3039;
    return (next / 0x10000) & 0x7FFF;
}

static void CARDSrand(unsigned int seed) {
    next = seed;
}

static u32 exnor_1st(u32 data, u32 rshift) {
    u32 wk;
    u32 work;
    u32 i;

    work = data;
    for (i = 0; i < rshift; i++) {
        wk = ~(work ^ (work >> 7) ^ (work >> 15) ^ (work >> 23));
        work = (work >> 1) | ((wk << 30) & 0x40000000);
    }

    return work;
}

static u32 exnor(u32 data, u32 lshift) {
    u32 wk;
    u32 work;
    u32 i;

    work = data;
    for (i = 0; i < lshift; i++) {
        // 1bit Left Shift
        wk = ~(work ^ (work << 7) ^ (work << 15) ^ (work << 23));
        work = (work << 1) | ((wk >> 30) & 0x00000002);
    }

    return work;
}

static u32 bitrev(u32 data) {
    u32 wk;
    u32 i;
    u32 k = 0;
    u32 j = 1;

    wk = 0;
    for (i = 0; i < 32; i++) {
        if (i > 15) {
            if (i == 31)
                wk |= (((data & (0x01 << 31)) >> 31) & 0x01);
            else {
                wk |= ((data & (0x01 << i)) >> j);
                j += 2;
            }
        } else {
            wk |= ((data & (0x01 << i)) << (31 - i - k));
            k++;
        }
    }

    return wk;
}

#define SEC_AD1(x) ((u8)(((x) >> 29) & 0x03))
#define SEC_AD2(x) ((u8)(((x) >> 21) & 0xff))
#define SEC_AD3(x) ((u8)(((x) >> 19) & 0x03))
#define SEC_BA(x) ((u8)(((x) >> 12) & 0x7f))

static s32 ReadArrayUnlock(s32 chan, u32 data, void* rbuf, s32 rlen, int mode) {
    CARDControl* card;
    BOOL err;
    u8 cmd[5];

    ASSERTLINE(240, 0 <= chan && chan < 2);

    card = &__CARDBlock[chan];
    if (!EXISelect(chan, 0, CARDFreq))
        return CARD_RESULT_NOCARD;

    data &= 0xfffff000;
    memset(cmd, 0, 5);
    cmd[0] = 0x52;
    if (mode == 0) {
        cmd[1] = SEC_AD1(data);
        cmd[2] = SEC_AD2(data);
        cmd[3] = SEC_AD3(data);
        cmd[4] = SEC_BA(data);
    } else {
        cmd[1] = (u8)((data & 0xff000000) >> 24);
        cmd[2] = (u8)((data & 0x00ff0000) >> 16);
    }

    err = FALSE;
    err |= !EXIImmEx(chan, cmd, 5, 1);
    err |= !EXIImmEx(chan, (u8* )card->workArea + (u32)sizeof(CARDID), card->latency, 1);
    err |= !EXIImmEx(chan, rbuf, rlen, 0);
    err |= !EXIDeselect(chan);

    return err ? CARD_RESULT_NOCARD : CARD_RESULT_READY;
}

static u32 GetInitVal(void) {
    u32 tmp;
    u32 tick;

    tick = OSGetTick();
    CARDSrand(tick);
    tmp = 0x7fec8000;
    tmp |= CARDRand();
    tmp &= 0xfffff000;
    return tmp;
}

static s32 DummyLen(void) {
    u32 tick;
    u32 wk;
    s32 tmp;
    u32 max;

    wk = 1;
    max = 0;
    tick = OSGetTick();
    CARDSrand(tick);

    tmp = CARDRand();
    tmp &= 0x0000001f;
    tmp += 1;
    while ((tmp < 4) && (max < 10)) {
        tick = OSGetTick();
        tmp = (s32)(tick << wk);
        wk++;
        if (wk > 16)
            wk = 1;
        CARDSrand((u32)tmp);
        tmp = CARDRand();
        tmp &= 0x0000001f;
        tmp += 1;
        max++;
    }

    if (tmp < 4)
        tmp = 4;

    return tmp;
}

s32 __CARDUnlock(s32 chan, u8 flashID[12]) {
    u32 init_val;
    u32 data;

    s32 dummy;
    s32 rlen;
    u32 rshift;

    u8 fsts;
    u32 wk, wk1;
    u32 Ans1 = 0;
    u32 Ans2 = 0;
    u32* dp;
    u8 rbuf[64];
    u32 para1A = 0;
    u32 para1B = 0;
    u32 para2A = 0;
    u32 para2B = 0;

    CARDControl* card;
    DSPTaskInfo* task;
    CARDDecParam* param;
    u8* input;
    u8* output;

    card = &__CARDBlock[chan];
    task = &card->task;
    param = (CARDDecParam*)card->workArea;
    input = (u8*)((u8* )param + sizeof(CARDDecParam));
    input = (u8*)OSRoundUp32B(input);
    output = input + 32;

    fsts = 0;
    init_val = GetInitVal();

    dummy = DummyLen();
    rlen = dummy;
    if (ReadArrayUnlock(chan, init_val, rbuf, rlen, 0) < 0)
        return CARD_RESULT_NOCARD;

    rshift = (u32)(dummy * 8 + 1);
    wk = exnor_1st(init_val, rshift);
    wk1 = ~(wk ^ (wk >> 7) ^ (wk >> 15) ^ (wk >> 23));
    card->scramble = (wk | ((wk1 << 31) & 0x80000000));
    card->scramble = bitrev(card->scramble);
    dummy = DummyLen();
    rlen = 20 + dummy;
    data = 0;
    if (ReadArrayUnlock(chan, data, rbuf, rlen, 1) < 0)
        return CARD_RESULT_NOCARD;

    dp = (u32* )rbuf;
    para1A = *dp++;
    para1B = *dp++;
    Ans1 = *dp++;
    para2A = *dp++;
    para2B = *dp++;
    para1A = (para1A ^ card->scramble);
    rshift = 32;
    wk = exnor(card->scramble, rshift);
    wk1 = ~(wk ^ (wk << 7) ^ (wk << 15) ^ (wk << 23));
    card->scramble = (wk | ((wk1 >> 31) & 0x00000001));

    para1B = (para1B ^ card->scramble);
    rshift = 32;
    wk = exnor(card->scramble, rshift);
    wk1 = ~(wk ^ (wk << 7) ^ (wk << 15) ^ (wk << 23));
    card->scramble = (wk | ((wk1 >> 31) & 0x00000001));

    Ans1 ^= card->scramble;
    rshift = 32;
    wk = exnor(card->scramble, rshift);
    wk1 = ~(wk ^ (wk << 7) ^ (wk << 15) ^ (wk << 23));
    card->scramble = (wk | ((wk1 >> 31) & 0x00000001));

    para2A = (para2A ^ card->scramble);
    rshift = 32;
    wk = exnor(card->scramble, rshift);
    wk1 = ~(wk ^ (wk << 7) ^ (wk << 15) ^ (wk << 23));
    card->scramble = (wk | ((wk1 >> 31) & 0x00000001));

    para2B = (para2B ^ card->scramble);
    rshift = (u32)(dummy * 8);
    wk = exnor(card->scramble, rshift);
    wk1 = ~(wk ^ (wk << 7) ^ (wk << 15) ^ (wk << 23));
    card->scramble = (wk | ((wk1 >> 31) & 0x00000001));

    rshift = 32 + 1;
    wk = exnor(card->scramble, rshift);
    wk1 = ~(wk ^ (wk << 7) ^ (wk << 15) ^ (wk << 23));
    card->scramble = (wk | ((wk1 >> 31) & 0x00000001));

    *(u32*)&input[0] = para2A;
    *(u32*)&input[4] = para2B;

    param->inputAddr = input;
    param->inputLength = 8;
    param->outputAddr = output;
    param->aramAddr = 0;

    DCFlushRange(input, 8);
    DCInvalidateRange(output, 4);
    DCFlushRange(param, sizeof(CARDDecParam));

    task->priority = 255;
    task->iram_mmem_addr = (u16*)OSCachedToPhysical(CardData);
    task->iram_length = 0x160;
    task->iram_addr = 0;
    task->dsp_init_vector = 0x10;
    task->init_cb = InitCallback;
    task->res_cb = NULL;
    task->done_cb = DoneCallback;
    task->req_cb = NULL;
    DSPAddTask(task);

    dp = (u32*)flashID;
    *dp++ = para1A;
    *dp++ = para1B;
    *dp = Ans1;

    return CARD_RESULT_READY;
}

static void InitCallback(void* _task) {
    s32 chan;
    CARDControl* card;
    DSPTaskInfo* task;
    CARDDecParam* param;

    task = _task;
    for (chan = 0; chan < 2; ++chan) {
        card = &__CARDBlock[chan];
        if ((DSPTaskInfo*)&card->task == task)
            break;
    }

    ASSERTLINE(514, 0 <= chan && chan < 2);
    
    param = (CARDDecParam*)card->workArea;

    DSPSendMailToDSP(0xff000000);
    while (DSPCheckMailToDSP())
        ;

    DSPSendMailToDSP((u32)param);
    while (DSPCheckMailToDSP())
        ;
}

static void DoneCallback(void* _task) {
    u8 rbuf[64];
    u32 data;
    s32 dummy;
    s32 rlen;
    u32 rshift;

    u8 unk;
    u32 wk, wk1;
    u32 Ans2;

    s32 chan;
    CARDControl* card;
    s32 result;
    DSPTaskInfo* task;
    CARDDecParam* param;

    u8* input;
    u8* output;
    task = _task;
    for (chan = 0; chan < 2; ++chan) {
        card = &__CARDBlock[chan];
        if ((DSPTaskInfo* )&card->task == task)
            break;
    }

    ASSERTLINE(563, 0 <= chan && chan < 2);

    param = (CARDDecParam*)card->workArea;
    input = (u8*)((u8*)param + sizeof(CARDDecParam));
    input = (u8*)OSRoundUp32B(input);
    output = input + 32;

    Ans2 = *(u32*)output;
    dummy = DummyLen();
    rlen = dummy;
    data = ((Ans2 ^ card->scramble) & 0xffff0000);
    if (ReadArrayUnlock(chan, data, rbuf, rlen, 1) < 0) {
        EXIUnlock(chan);
        __CARDMountCallback(chan, CARD_RESULT_NOCARD);
        return;
    }

    rshift = (u32)((dummy + 4 + card->latency) * 8 + 1);
    wk = exnor(card->scramble, rshift);
    wk1 = ~(wk ^ (wk << 7) ^ (wk << 15) ^ (wk << 23));
    card->scramble = (wk | ((wk1 >> 31) & 0x00000001));

    dummy = DummyLen();
    rlen = dummy;
    data = (((Ans2 << 16) ^ card->scramble) & 0xffff0000);
    if (ReadArrayUnlock(chan, data, rbuf, rlen, 1) < 0) {
        EXIUnlock(chan);
        __CARDMountCallback(chan, CARD_RESULT_NOCARD);
        return;
    }

    result = __CARDReadStatus(chan, &unk);
    if (!EXIProbe(chan)) {
        EXIUnlock(chan);
        __CARDMountCallback(chan, CARD_RESULT_NOCARD);
        return;
    }

    if (result == CARD_RESULT_READY && !(unk & 0x40)) {
        EXIUnlock(chan);
        result = CARD_RESULT_IOERROR;
    }

    __CARDMountCallback(chan, result);
}
