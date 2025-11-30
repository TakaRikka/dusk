#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JAudio2/osdsp_task.h"
#include "JSystem/JAudio2/dspproc.h"
#include <dolphin/dsp.h>
#include <dolphin/os.h>

extern DSPTaskInfo* __DSP_first_task;
extern DSPTaskInfo* __DSP_curr_task;
extern "C" void __DSP_exec_task(DSPTaskInfo*, DSPTaskInfo*);
extern "C" void __DSP_remove_task(DSPTaskInfo* task);

static void Dsp_Update_Request();

/* 80451308 0001+00 data_80451308 None */
/* 80451309 0003+00 data_80451309 None */
static vu8 struct_80451308;
static u8 struct_80451309;

DSPTaskInfo* DSP_prior_task;

extern "C" void __DSPHandler(__OSInterrupt interrupt, OSContext* context) {
    OSContext funcContext;
    __DSPRegs[5] = ((u16)(__DSPRegs[5]) & ~0x28) | 0x80;
    OSClearContext(&funcContext);
    OSSetCurrentContext(&funcContext);

    if (struct_80451308 == 1 || struct_80451308 == 0) {
        __DSP_curr_task = DSP_prior_task;
    }

    while (DSPCheckMailFromDSP() == 0);
    u32 mail = DSPReadMailFromDSP();

    if ((__DSP_curr_task->flags & 2) && mail == 0xDCD10002) {
        mail = 0xDCD10003;
    }

    switch (mail) {
    case 0xDCD10000:
        __DSP_curr_task->state = 1;
        if (__DSP_curr_task == DSP_prior_task) {
            struct_80451308 = 1;
        }
        if (__DSP_curr_task->init_cb != NULL) {
            __DSP_curr_task->init_cb(__DSP_curr_task);
        }
        break;
    case 0xDCD10001:
        __DSP_curr_task->state = 1;
        if (__DSP_curr_task == DSP_prior_task) {
            struct_80451308 = 1;
            Dsp_Update_Request();
        }
        if (__DSP_curr_task->res_cb != NULL) {
            __DSP_curr_task->res_cb(__DSP_curr_task);
        }
        break;
    case 0xDCD10002:
        DSPSendMailToDSP(0xCDD10001);
        while (DSPCheckMailToDSP() != 0);
        __DSP_curr_task->state = 2;
        if (__DSP_curr_task->next == NULL && struct_80451309) {
            __DSP_exec_task(__DSP_curr_task, DSP_prior_task);
            struct_80451309 = 0;
            __DSP_curr_task = DSP_prior_task;
        } else {
            __DSP_exec_task(__DSP_curr_task, __DSP_curr_task->next);
            __DSP_curr_task = __DSP_curr_task->next;
        }
        break;
    case 0xDCD10003:
        if (__DSP_curr_task->done_cb != NULL) {
            __DSP_curr_task->done_cb(__DSP_curr_task);
        }
        DSPSendMailToDSP(0xCDD10001);
        while (DSPCheckMailToDSP() != 0);
        __DSP_curr_task->state = 3;
        if (__DSP_curr_task->next == NULL) {
            __DSP_exec_task(NULL, DSP_prior_task);
            __DSP_remove_task(__DSP_curr_task);
            __DSP_curr_task = DSP_prior_task;
        } else {
            __DSP_exec_task(NULL, __DSP_curr_task->next);
            __DSP_curr_task = __DSP_curr_task->next;
            __DSP_remove_task(__DSP_curr_task->prev);
        }
        break;
    case 0xDCD10004:
        if (__DSP_curr_task->req_cb != NULL) {
            __DSP_curr_task->req_cb(__DSP_curr_task);
        }
        break;
    case 0xDCD10005:
        if (__DSP_first_task == NULL || struct_80451309) {
            DSPSendMailToDSP(0xCDD10003);
            while (DSPCheckMailToDSP() != 0);
            struct_80451309 = 0;
            __DSP_curr_task = DSP_prior_task;
            Dsp_Update_Request();
        } else {
            struct_80451308 = 3;
            DSPSendMailToDSP(0xCDD10001);
            while (DSPCheckMailToDSP() != 0);
            __DSP_exec_task(DSP_prior_task, __DSP_first_task);
            __DSP_curr_task = __DSP_first_task;
        }
        break;
    }

    OSClearContext(&funcContext);
    OSSetCurrentContext(context);
}

static u32 sync_stack[5];

void DsyncFrame2(u32 param_0, u32 param_1, u32 param_2) {
    if (struct_80451308 != 1) {
        sync_stack[0] = param_0;
        struct_80451309 = 1;
        sync_stack[1] = param_1;
        sync_stack[2] = param_2;
        return;
    }
    DsyncFrame2ch(param_0, param_1, param_2);
    struct_80451309 = 0;
}

static void DsyncFrame3(u32 param_0, u32 param_1, u32 param_2, u32 param_3, u32 param_4) {
    if (struct_80451308 != 1) {
        sync_stack[0] = param_0;
        struct_80451309 = 2;
        sync_stack[1] = param_1;
        sync_stack[2] = param_2;
        sync_stack[3] = param_3;
        sync_stack[4] = param_4;
        return;
    }
    DsyncFrame4ch(param_0, param_1, param_2, param_3, param_4);
    struct_80451309 = 0;
}

static void Dsp_Update_Request() {
    switch (struct_80451309) {
    case 0:
        break;
    case 1:
        DsyncFrame2(sync_stack[0], sync_stack[1], sync_stack[2]);
        break;
    case 2:
        DsyncFrame3(sync_stack[0], sync_stack[1], sync_stack[2], sync_stack[3], sync_stack[4]);
        break;
    }
}

bool Dsp_Running_Check() {
    return struct_80451308 == 1;
}

void Dsp_Running_Start() {
    struct_80451308 = 1;
}
