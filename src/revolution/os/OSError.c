#include <stdio.h>
#include <revolution.h>
#include <revolution/os.h>

#include "__os.h"

OSErrorHandler __OSErrorTable[17];

#define FPSCR_ENABLE (FPSCR_VE | FPSCR_OE | FPSCR_UE | FPSCR_ZE | FPSCR_XE)
u32 __OSFpscrEnableBits = FPSCR_ENABLE;

DECL_WEAK void OSReport(const char* msg, ...) {
    va_list marker;
    va_start(marker, msg);
    vprintf(msg, marker);
    va_end(marker);
}

DECL_WEAK void OSVReport(const char* msg, va_list list) {
    vprintf(msg, list);
}

DECL_WEAK void OSPanic(const char* file, int line, const char* msg, ...) {
    va_list marker;
    u32 i;
    u32* p;

    OSDisableInterrupts();
    va_start(marker, msg);
    vprintf(msg, marker);
    va_end(marker);
    OSReport(" in \"%s\" on line %d.\n", file, line);

    OSReport("\nAddress:      Back Chain    LR Save\n");
    for (i = 0, p = (u32*)OSGetStackPointer(); p && (u32)p != 0xffffffff && i++ < 16; p = (u32*)*p) {
        OSReport("0x%08x:   0x%08x    0x%08x\n", p, p[0], p[1]);
    }

    PPCHalt();
}

OSErrorHandler OSSetErrorHandler(OSError error, OSErrorHandler handler) {
    OSErrorHandler oldHandler;
    int enabled;

    ASSERTMSGLINE(215, error < 17, "OSSetErrorHandler(): unknown error.");

    enabled = OSDisableInterrupts();
    oldHandler = __OSErrorTable[error];
    __OSErrorTable[error] = handler;

    if (error == __OS_EXCEPTION_FLOATING_POINT_EXCEPTION) {
        u32 msr;
        u32 fpscr;
        OSThread* thread;
        int i;

        msr = PPCMfmsr();
        PPCMtmsr(msr | MSR_FP);
        fpscr = PPCMffpscr();

        if (handler) {
            for (thread = __OSActiveThreadQueue.head; thread;
                 thread = thread->linkActive.next)
            {
                thread->context.srr1 |= MSR_FE0 | MSR_FE1;
                if ((thread->context.state & OS_CONTEXT_STATE_FPSAVED) == 0) {
                    thread->context.state |= OS_CONTEXT_STATE_FPSAVED;
                    for (i = 0; i < 32; ++i) {
                        *(u64*)&thread->context.fpr[i] = (u64)0xffffffffffffffffLL;
                        *(u64*)&thread->context.psf[i] = (u64)0xffffffffffffffffLL;
                    }
                    thread->context.fpscr = FPSCR_NI;
                }
                thread->context.fpscr |= __OSFpscrEnableBits & FPSCR_ENABLE;
                thread->context.fpscr &=
                    ~(FPSCR_VXVC | FPSCR_VXIMZ | FPSCR_VXZDZ | FPSCR_VXIDI | FPSCR_VXISI |
                      FPSCR_VXSNAN | FPSCR_VXSOFT | FPSCR_VXSQRT | FPSCR_VXCVI | FPSCR_XX |
                      FPSCR_ZX | FPSCR_UX | FPSCR_OX | FPSCR_FX | FPSCR_FI);
            }
            fpscr |= __OSFpscrEnableBits & FPSCR_ENABLE;
            msr |= MSR_FE0 | MSR_FE1;
        } else {
            for (thread = __OSActiveThreadQueue.head; thread;
                 thread = thread->linkActive.next)
            {
                thread->context.srr1 &= ~(MSR_FE0 | MSR_FE1);
                thread->context.fpscr &= ~FPSCR_ENABLE;
                thread->context.fpscr &=
                    ~(FPSCR_VXVC | FPSCR_VXIMZ | FPSCR_VXZDZ | FPSCR_VXIDI | FPSCR_VXISI |
                      FPSCR_VXSNAN | FPSCR_VXSOFT | FPSCR_VXSQRT | FPSCR_VXCVI | FPSCR_XX |
                      FPSCR_ZX | FPSCR_UX | FPSCR_OX | FPSCR_FX | FPSCR_FI);
            }
            fpscr &= ~FPSCR_ENABLE;
            msr &= ~(MSR_FE0 | MSR_FE1);
        }

        fpscr &= ~(FPSCR_VXVC | FPSCR_VXIMZ | FPSCR_VXZDZ | FPSCR_VXIDI | FPSCR_VXISI |
                   FPSCR_VXSNAN | FPSCR_VXSOFT | FPSCR_VXSQRT | FPSCR_VXCVI | FPSCR_XX | FPSCR_ZX |
                   FPSCR_UX | FPSCR_OX | FPSCR_FX | FPSCR_FI);

        PPCMtfpscr(fpscr);
        PPCMtmsr(msr);
    }

    OSRestoreInterrupts(enabled);
    return oldHandler;
}

volatile OSContext* __OSFPUContext AT_ADDRESS(OS_BASE_CACHED | 0x00D8);

void __OSUnhandledException(__OSException exception, OSContext* context, u32 dsisr, u32 dar) {
    OSTime now;
    now = OSGetTime();

    if (!(context->srr1 & MSR_RI)) {
        OSReport("Non-recoverable Exception %d", exception);
    } else {
        u32 fpscr;
        u32 msr;
        if (exception == __OS_EXCEPTION_PROGRAM && (context->srr1 & (0x80000000 >> 11)) &&
            __OSErrorTable[__OS_EXCEPTION_FLOATING_POINT_EXCEPTION] != 0)
        {
            exception = __OS_EXCEPTION_FLOATING_POINT_EXCEPTION;

            msr = PPCMfmsr();
            PPCMtmsr(msr | 0x2000);

            if (__OSFPUContext) {
                OSSaveFPUContext((OSContext*)__OSFPUContext);
            }

            fpscr = PPCMffpscr();
            fpscr &= ~(FPSCR_VXVC | FPSCR_VXIMZ | FPSCR_VXZDZ | FPSCR_VXIDI | FPSCR_VXISI |
                       FPSCR_VXSNAN | FPSCR_VXSOFT | FPSCR_VXSQRT | FPSCR_VXCVI | FPSCR_XX |
                       FPSCR_ZX | FPSCR_UX | FPSCR_OX | FPSCR_FX | FPSCR_FI);
            PPCMtfpscr(fpscr);

            PPCMtmsr(msr);

            if (__OSFPUContext == context) {
                OSDisableScheduler();
                __OSErrorTable[exception](exception, context, dsisr, dar);
                context->srr1 &= ~0x2000;
                __OSFPUContext = NULL;

                context->fpscr &=
                    ~(FPSCR_VXVC | FPSCR_VXIMZ | FPSCR_VXZDZ | FPSCR_VXIDI | FPSCR_VXISI |
                      FPSCR_VXSNAN | FPSCR_VXSOFT | FPSCR_VXSQRT | FPSCR_VXCVI | FPSCR_XX |
                      FPSCR_ZX | FPSCR_UX | FPSCR_OX | FPSCR_FX | FPSCR_FI);
                OSEnableScheduler();
                __OSReschedule();
            } else {
                context->srr1 &= ~0x2000;
                __OSFPUContext = NULL;
            }

            OSLoadContext(context);
        }

        if (__OSErrorTable[exception]) {
            OSDisableScheduler();
            __OSErrorTable[exception](exception, context, dsisr, dar);
            OSEnableScheduler();
            __OSReschedule();
            OSLoadContext(context);
        }
        if (exception == __OS_EXCEPTION_DECREMENTER) {
            OSLoadContext(context);
        }
        OSReport("Unhandled Exception %d", exception);
    }
#if DEBUG
    OSReport("(%s)", __OSExceptionNames[exception]);
#endif
    OSReport("\n");
    OSDumpContext(context);
    OSReport("\nDSISR = 0x%08x                   DAR  = 0x%08x\n", dsisr, dar);
    OSReport("TB = 0x%016llx\n", now);

    switch(exception) {
    case __OS_EXCEPTION_DSI:
        OSReport("\nInstruction at 0x%x (read from SRR0) attempted to access "
                    "invalid address 0x%x (read from DAR)\n",
                    context->srr0, dar);
        break;
    case __OS_EXCEPTION_ISI:
        OSReport("\nAttempted to fetch instruction from invalid address 0x%x "
                    "(read from SRR0)\n",
                    context->srr0);
        break;
    case __OS_EXCEPTION_ALIGNMENT:
        OSReport("\nInstruction at 0x%x (read from SRR0) attempted to access "
                    "unaligned address 0x%x (read from DAR)\n",
                    context->srr0, dar);
        break;
    case __OS_EXCEPTION_PROGRAM:
        OSReport("\nProgram exception : Possible illegal instruction/operation "
                    "at or around 0x%x (read from SRR0)\n",
                    context->srr0, dar);
        break;
    case __OS_EXCEPTION_MEMORY_PROTECTION:
        OSReport("\n");
        OSReport("AI DMA Address =   0x%04x%04x\n", __DSPRegs[DSP_DMA_START_HI],
                __DSPRegs[DSP_DMA_START_LO]);
        OSReport("ARAM DMA Address = 0x%04x%04x\n", __DSPRegs[DSP_ARAM_DMA_MM_HI],
                __DSPRegs[DSP_ARAM_DMA_MM_LO]);
        OSReport("DI DMA Address =   0x%08x\n", __DIRegs[5]);
        break;
    }

    OSReport("\nLast interrupt (%d): SRR0 = 0x%08x  TB = 0x%016llx\n", __OSLastInterrupt,
             __OSLastInterruptSrr0, __OSLastInterruptTime);
    PPCHalt();
}
