#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JAudio2/osdsp.h"
#include "JSystem/JAudio2/osdsp_task.h"
#include "dolphin/os.h"
#include <dolphin/dsp.h>

extern "C" void __DSP_insert_task(DSPTaskInfo*);
extern "C" void __DSP_boot_task(DSPTaskInfo*);

DSPTaskInfo* DSPAddTask(DSPTaskInfo* task) {
    if (DSP_prior_task == NULL) {
        OSReport("Prior Task is not inited\n");
        return NULL;
    }
    BOOL status = OSDisableInterrupts();
    __DSP_insert_task(task);
    task->state = 0;
    task->flags = 1;
    OSRestoreInterrupts(status);
    return task;
}


void DSPAddPriorTask(STRUCT_DSP_TASK* task) {
    if (DSP_prior_task != NULL) {
        OSReport("Already inited prior DSP task\n");
        return;
    }
    BOOL status = OSDisableInterrupts();
    DSP_prior_task = (DSPTaskInfo*)task;
    task->state = 0;
    task->flags = 1;
    __DSP_boot_task((DSPTaskInfo*)task);
    OSRestoreInterrupts(status);
}
