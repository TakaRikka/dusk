#include <dolphin.h>
#include <dolphin/os.h>

#include "__os.h"

// external functions
extern void __RAS_OSDisableInterrupts_begin();
extern void __RAS_OSDisableInterrupts_end();
extern void DBPrintf(char*, ...);

#define HID2 920

volatile OSContext* __OSCurrentContext AT_ADDRESS(OS_BASE_CACHED | 0x00D4);
volatile OSContext* __OSFPUContext AT_ADDRESS(OS_BASE_CACHED | 0x00D8);

#ifdef __GEKKO__
static asm void __OSLoadFPUContext(__REGISTER u32 dummy, __REGISTER OSContext* fpucontext) {
    nofralloc
    lhz r5, fpucontext->state;
    clrlwi. r5, r5, 31
    beq _return

    lfd fp0, OS_CONTEXT_FPSCR(fpucontext)
    mtfsf 0xFF, fp0
    mfspr r5, HID2
    rlwinm. r5, r5, 3, 31, 31
    beq _regular_FPRs

    psq_l fp0, OS_CONTEXT_PSF0(fpucontext), 0, 0
    psq_l fp1, OS_CONTEXT_PSF1(fpucontext), 0, 0
    psq_l fp2, OS_CONTEXT_PSF2(fpucontext), 0, 0
    psq_l fp3, OS_CONTEXT_PSF3(fpucontext), 0, 0
    psq_l fp4, OS_CONTEXT_PSF4(fpucontext), 0, 0
    psq_l fp5, OS_CONTEXT_PSF5(fpucontext), 0, 0
    psq_l fp6, OS_CONTEXT_PSF6(fpucontext), 0, 0
    psq_l fp7, OS_CONTEXT_PSF7(fpucontext), 0, 0
    psq_l fp8, OS_CONTEXT_PSF8(fpucontext), 0, 0
    psq_l fp9, OS_CONTEXT_PSF9(fpucontext), 0, 0
    psq_l fp10, OS_CONTEXT_PSF10(fpucontext), 0, 0
    psq_l fp11, OS_CONTEXT_PSF11(fpucontext), 0, 0
    psq_l fp12, OS_CONTEXT_PSF12(fpucontext), 0, 0
    psq_l fp13, OS_CONTEXT_PSF13(fpucontext), 0, 0
    psq_l fp14, OS_CONTEXT_PSF14(fpucontext), 0, 0
    psq_l fp15, OS_CONTEXT_PSF15(fpucontext), 0, 0
    psq_l fp16, OS_CONTEXT_PSF16(fpucontext), 0, 0
    psq_l fp17, OS_CONTEXT_PSF17(fpucontext), 0, 0
    psq_l fp18, OS_CONTEXT_PSF18(fpucontext), 0, 0
    psq_l fp19, OS_CONTEXT_PSF19(fpucontext), 0, 0
    psq_l fp20, OS_CONTEXT_PSF20(fpucontext), 0, 0
    psq_l fp21, OS_CONTEXT_PSF21(fpucontext), 0, 0
    psq_l fp22, OS_CONTEXT_PSF22(fpucontext), 0, 0
    psq_l fp23, OS_CONTEXT_PSF23(fpucontext), 0, 0
    psq_l fp24, OS_CONTEXT_PSF24(fpucontext), 0, 0
    psq_l fp25, OS_CONTEXT_PSF25(fpucontext), 0, 0
    psq_l fp26, OS_CONTEXT_PSF26(fpucontext), 0, 0
    psq_l fp27, OS_CONTEXT_PSF27(fpucontext), 0, 0
    psq_l fp28, OS_CONTEXT_PSF28(fpucontext), 0, 0
    psq_l fp29, OS_CONTEXT_PSF29(fpucontext), 0, 0
    psq_l fp30, OS_CONTEXT_PSF30(fpucontext), 0, 0
    psq_l fp31, OS_CONTEXT_PSF31(fpucontext), 0, 0

_regular_FPRs:
    lfd fp0,  fpucontext->fpr[0]
    lfd fp1,  fpucontext->fpr[1]
    lfd fp2,  fpucontext->fpr[2]
    lfd fp3,  fpucontext->fpr[3]
    lfd fp4,  fpucontext->fpr[4]
    lfd fp5,  fpucontext->fpr[5]
    lfd fp6,  fpucontext->fpr[6]
    lfd fp7,  fpucontext->fpr[7]
    lfd fp8,  fpucontext->fpr[8]
    lfd fp9,  fpucontext->fpr[9]
    lfd fp10, fpucontext->fpr[10]
    lfd fp11, fpucontext->fpr[11]
    lfd fp12, fpucontext->fpr[12]
    lfd fp13, fpucontext->fpr[13]
    lfd fp14, fpucontext->fpr[14]
    lfd fp15, fpucontext->fpr[15]
    lfd fp16, fpucontext->fpr[16]
    lfd fp17, fpucontext->fpr[17]
    lfd fp18, fpucontext->fpr[18]
    lfd fp19, fpucontext->fpr[19]
    lfd fp20, fpucontext->fpr[20]
    lfd fp21, fpucontext->fpr[21]
    lfd fp22, fpucontext->fpr[22]
    lfd fp23, fpucontext->fpr[23]
    lfd fp24, fpucontext->fpr[24]
    lfd fp25, fpucontext->fpr[25]
    lfd fp26, fpucontext->fpr[26]
    lfd fp27, fpucontext->fpr[27]
    lfd fp28, fpucontext->fpr[28]
    lfd fp29, fpucontext->fpr[29]
    lfd fp30, fpucontext->fpr[30]
    lfd fp31, fpucontext->fpr[31]
_return:
    blr
}

static asm void __OSSaveFPUContext(__REGISTER u32 dummy1, __REGISTER u32 dummy2, __REGISTER OSContext* fpucontext) {
    nofralloc

    lhz     r3,   fpucontext->state
    ori     r3,   r3, 1
    sth     r3,   fpucontext->state

    stfd    fp0,  fpucontext->fpr[0]
    stfd    fp1,  fpucontext->fpr[1]
    stfd    fp2,  fpucontext->fpr[2]
    stfd    fp3,  fpucontext->fpr[3]
    stfd    fp4,  fpucontext->fpr[4]
    stfd    fp5,  fpucontext->fpr[5]
    stfd    fp6,  fpucontext->fpr[6]
    stfd    fp7,  fpucontext->fpr[7]
    stfd    fp8,  fpucontext->fpr[8]
    stfd    fp9,  fpucontext->fpr[9]
    stfd    fp10, fpucontext->fpr[10]
    stfd    fp11, fpucontext->fpr[11]
    stfd    fp12, fpucontext->fpr[12]
    stfd    fp13, fpucontext->fpr[13]
    stfd    fp14, fpucontext->fpr[14]
    stfd    fp15, fpucontext->fpr[15]
    stfd    fp16, fpucontext->fpr[16]
    stfd    fp17, fpucontext->fpr[17]
    stfd    fp18, fpucontext->fpr[18]
    stfd    fp19, fpucontext->fpr[19]
    stfd    fp20, fpucontext->fpr[20]
    stfd    fp21, fpucontext->fpr[21]
    stfd    fp22, fpucontext->fpr[22]
    stfd    fp23, fpucontext->fpr[23]
    stfd    fp24, fpucontext->fpr[24]
    stfd    fp25, fpucontext->fpr[25]
    stfd    fp26, fpucontext->fpr[26]
    stfd    fp27, fpucontext->fpr[27]
    stfd    fp28, fpucontext->fpr[28]
    stfd    fp29, fpucontext->fpr[29]
    stfd    fp30, fpucontext->fpr[30]
    stfd    fp31, fpucontext->fpr[31]

    mffs    fp0
    stfd    fp0,  OS_CONTEXT_FPSCR(fpucontext)

    lfd     fp0,  fpucontext->fpr[0]

    mfspr   r3, HID2
    rlwinm. r3, r3, 3, 31, 31
    bc      12, 2, _return

    psq_st  fp0, OS_CONTEXT_PSF0(fpucontext), 0, 0
    psq_st  fp1, OS_CONTEXT_PSF1(fpucontext), 0, 0
    psq_st  fp2, OS_CONTEXT_PSF2(fpucontext), 0, 0
    psq_st  fp3, OS_CONTEXT_PSF3(fpucontext), 0, 0
    psq_st  fp4, OS_CONTEXT_PSF4(fpucontext), 0, 0
    psq_st  fp5, OS_CONTEXT_PSF5(fpucontext), 0, 0
    psq_st  fp6, OS_CONTEXT_PSF6(fpucontext), 0, 0
    psq_st  fp7, OS_CONTEXT_PSF7(fpucontext), 0, 0
    psq_st  fp8, OS_CONTEXT_PSF8(fpucontext), 0, 0
    psq_st  fp9, OS_CONTEXT_PSF9(fpucontext), 0, 0
    psq_st  fp10, OS_CONTEXT_PSF10(fpucontext), 0, 0
    psq_st  fp11, OS_CONTEXT_PSF11(fpucontext), 0, 0
    psq_st  fp12, OS_CONTEXT_PSF12(fpucontext), 0, 0
    psq_st  fp13, OS_CONTEXT_PSF13(fpucontext), 0, 0
    psq_st  fp14, OS_CONTEXT_PSF14(fpucontext), 0, 0
    psq_st  fp15, OS_CONTEXT_PSF15(fpucontext), 0, 0
    psq_st  fp16, OS_CONTEXT_PSF16(fpucontext), 0, 0
    psq_st  fp17, OS_CONTEXT_PSF17(fpucontext), 0, 0
    psq_st  fp18, OS_CONTEXT_PSF18(fpucontext), 0, 0
    psq_st  fp19, OS_CONTEXT_PSF19(fpucontext), 0, 0
    psq_st  fp20, OS_CONTEXT_PSF20(fpucontext), 0, 0
    psq_st  fp21, OS_CONTEXT_PSF21(fpucontext), 0, 0
    psq_st  fp22, OS_CONTEXT_PSF22(fpucontext), 0, 0
    psq_st  fp23, OS_CONTEXT_PSF23(fpucontext), 0, 0
    psq_st  fp24, OS_CONTEXT_PSF24(fpucontext), 0, 0
    psq_st  fp25, OS_CONTEXT_PSF25(fpucontext), 0, 0
    psq_st  fp26, OS_CONTEXT_PSF26(fpucontext), 0, 0
    psq_st  fp27, OS_CONTEXT_PSF27(fpucontext), 0, 0
    psq_st  fp28, OS_CONTEXT_PSF28(fpucontext), 0, 0
    psq_st  fp29, OS_CONTEXT_PSF29(fpucontext), 0, 0
    psq_st  fp30, OS_CONTEXT_PSF30(fpucontext), 0, 0
    psq_st  fp31, OS_CONTEXT_PSF31(fpucontext), 0, 0

_return:
    blr
}

asm void OSLoadFPUContext(__REGISTER OSContext* fpucontext) {
    nofralloc
    addi    r4, fpucontext, 0
    b       __OSLoadFPUContext
}

asm void OSSaveFPUContext(__REGISTER OSContext* fpucontext) {
    nofralloc
    addi    r5, fpucontext, 0
    b       __OSSaveFPUContext
}

asm void OSSetCurrentContext(__REGISTER OSContext* context){
    nofralloc

    addis   r4, r0, OS_CACHED_REGION_PREFIX

    stw     context, 0x00D4(r4)

    clrlwi  r5, context, 2
    stw     r5, 0x00C0(r4)

    lwz     r5, 0x00D8(r4)
    cmpw    r5, context
    bne     _disableFPU

    lwz     r6, context->srr1
    ori     r6, r6, 0x2000
    stw     r6, context->srr1
    mfmsr   r6
    ori     r6, r6, 2
    mtmsr   r6
    blr

_disableFPU:
    lwz     r6, context->srr1
    rlwinm  r6, r6, 0, 19, 17
    stw     r6, context->srr1
    mfmsr   r6
    rlwinm  r6, r6, 0, 19, 17
    ori     r6, r6, 2
    mtmsr   r6
    isync
    blr
}
#endif

OSContext* OSGetCurrentContext(void) {
    return (OSContext*)__OSCurrentContext;
}

#ifdef __GEKKO__
asm u32 OSSaveContext(__REGISTER OSContext* context) {
    nofralloc
    stmw    r13, context->gpr[13]
    mfspr   r0, GQR1
    stw     r0, context->gqr[1]
    mfspr   r0, GQR2
    stw     r0, context->gqr[2]
    mfspr   r0, GQR3
    stw     r0, context->gqr[3]
    mfspr   r0, GQR4
    stw     r0, context->gqr[4]
    mfspr   r0, GQR5
    stw     r0, context->gqr[5]
    mfspr   r0, GQR6
    stw     r0, context->gqr[6]
    mfspr   r0, GQR7
    stw     r0, context->gqr[7]
    mfcr    r0
    stw     r0, context->cr
    mflr    r0
    stw     r0, context->lr
    stw     r0, context->srr0
    mfmsr   r0
    stw     r0, context->srr1
    mfctr   r0
    stw     r0, context->ctr
    mfxer   r0
    stw     r0, context->xer
    stw     r1, context->gpr[1]
    stw     r2, context->gpr[2]
    li      r0, 0x1
    stw     r0, context->gpr[3]
    li      r3, 0
    blr
}

asm void OSLoadContext(__REGISTER OSContext* context) {
    nofralloc

    lis      r4,__RAS_OSDisableInterrupts_begin@ha
    lwz      r6,context->srr0
    addi     r5,r4,__RAS_OSDisableInterrupts_begin@l
    cmplw    r6,r5
    ble      _notInRAS
    lis      r4,__RAS_OSDisableInterrupts_end@ha
    addi     r0,r4,__RAS_OSDisableInterrupts_end@l
    cmplw    r6,r0
    bge      _notInRAS
    stw      r5,context->srr0

_notInRAS:

    lwz     r0, context->gpr[0]
    lwz     r1, context->gpr[1]
    lwz     r2, context->gpr[2]

    lhz     r4, context->state
    rlwinm. r5, r4, 0, 30, 30
    beq     notexc
    rlwinm  r4, r4, 0, 31, 29
    sth     r4, context->state
    lmw     r5, context->gpr[5]
    b       misc
notexc:
    lmw    r13, context->gpr[13]
    misc:

    lwz     r4, context->gqr[1]
    mtspr   GQR1, r4
    lwz     r4, context->gqr[2]
    mtspr   GQR2, r4
    lwz     r4, context->gqr[3]
    mtspr   GQR3, r4
    lwz     r4, context->gqr[4]
    mtspr   GQR4, r4
    lwz     r4, context->gqr[5]
    mtspr   GQR5, r4
    lwz     r4, context->gqr[6]
    mtspr   GQR6, r4
    lwz     r4, context->gqr[7]
    mtspr   GQR7, r4

    lwz     r4, context->cr
    mtcr    r4
    lwz     r4, context->lr
    mtlr    r4
    lwz     r4, context->ctr
    mtctr   r4
    lwz     r4, context->xer
    mtxer   r4

    mfmsr   r4
    rlwinm  r4, r4, 0, 17, 15
    rlwinm  r4, r4, 0, 31, 29
    mtmsr   r4

    lwz     r4, context->srr0
    mtsrr0  r4
    lwz     r4, context->srr1
    mtsrr1  r4

    lwz     r4, context->gpr[4]
    lwz     r3, context->gpr[3]

    rfi
}

asm u32 OSGetStackPointer() {
    nofralloc 
    mr r3, r1 
    blr
}

asm u32 OSSwitchStack(__REGISTER u32 newsp) {
    nofralloc
    mr  r5, r1
    mr  r1, newsp
    mr  r3, r5
    blr
}

asm int OSSwitchFiber(__REGISTER u32 pc, __REGISTER u32 newsp) {
    nofralloc
    mflr    r0
    mr      r5, r1
    stwu    r5, -8(newsp)
    mr      r1, newsp
    stw     r0, 4(r5)
    mtlr    pc
    blrl
    lwz     r5, 0(r1)
    lwz     r0, 4(r5)
    mtlr    r0
    mr      r1, r5
    blr
}
#endif

void OSClearContext(__REGISTER OSContext* context) {
    context->mode = 0;
    context->state = 0;
    if (context == __OSFPUContext)
        __OSFPUContext = NULL;
}

#ifdef __GEKKO__
asm void OSInitContext(__REGISTER OSContext* context, __REGISTER u32 pc, __REGISTER u32 newsp) {
    nofralloc

    stw     pc,  OS_CONTEXT_SRR0(context)
    stw     newsp,  OS_CONTEXT_R1(context)
    li      r11, 0
    ori     r11, r11, 0x00008000 | 0x00000020 | 0x00000010 | 0x00000002 | 0x00001000
    stw     r11, OS_CONTEXT_SRR1(context)
    li      r0,  0x0
    stw     r0,  OS_CONTEXT_CR(context)
    stw     r0,  OS_CONTEXT_XER(context)


    stw     r2,  OS_CONTEXT_R2(context)
    stw     r13, OS_CONTEXT_R13(context)

    stw     r0,  OS_CONTEXT_R3(context)
    stw     r0,  OS_CONTEXT_R4(context)
    stw     r0,  OS_CONTEXT_R5(context)
    stw     r0,  OS_CONTEXT_R6(context)
    stw     r0,  OS_CONTEXT_R7(context)
    stw     r0,  OS_CONTEXT_R8(context)
    stw     r0,  OS_CONTEXT_R9(context)
    stw     r0,  OS_CONTEXT_R10(context)
    stw     r0,  OS_CONTEXT_R11(context)
    stw     r0,  OS_CONTEXT_R12(context)

    stw     r0,  OS_CONTEXT_R14(context)
    stw     r0,  OS_CONTEXT_R15(context)
    stw     r0,  OS_CONTEXT_R16(context)
    stw     r0,  OS_CONTEXT_R17(context)
    stw     r0,  OS_CONTEXT_R18(context)
    stw     r0,  OS_CONTEXT_R19(context)
    stw     r0,  OS_CONTEXT_R20(context)
    stw     r0,  OS_CONTEXT_R21(context)
    stw     r0,  OS_CONTEXT_R22(context)
    stw     r0,  OS_CONTEXT_R23(context)
    stw     r0,  OS_CONTEXT_R24(context)
    stw     r0,  OS_CONTEXT_R25(context)
    stw     r0,  OS_CONTEXT_R26(context)
    stw     r0,  OS_CONTEXT_R27(context)
    stw     r0,  OS_CONTEXT_R28(context)
    stw     r0,  OS_CONTEXT_R29(context)
    stw     r0,  OS_CONTEXT_R30(context)
    stw     r0,  OS_CONTEXT_R31(context)

    stw     r0,  OS_CONTEXT_GQR0(context)
    stw     r0,  OS_CONTEXT_GQR1(context)
    stw     r0,  OS_CONTEXT_GQR2(context)
    stw     r0,  OS_CONTEXT_GQR3(context)
    stw     r0,  OS_CONTEXT_GQR4(context)
    stw     r0,  OS_CONTEXT_GQR5(context)
    stw     r0,  OS_CONTEXT_GQR6(context)
    stw     r0,  OS_CONTEXT_GQR7(context)

    b       OSClearContext
}
#endif

void OSDumpContext(OSContext* context) {
    u32 i;
    u32* p;

    OSReport("------------------------- Context 0x%08x -------------------------\n", context);

    for (i = 0; i < 16; ++i) {
        OSReport("r%-2d  = 0x%08x (%14d)  r%-2d  = 0x%08x (%14d)\n", i, context->gpr[i],
                context->gpr[i], i + 16, context->gpr[i + 16], context->gpr[i + 16]);
    }

    OSReport("LR   = 0x%08x                   CR   = 0x%08x\n", context->lr, context->cr);
    OSReport("SRR0 = 0x%08x                   SRR1 = 0x%08x\n", context->srr0, context->srr1);

    OSReport("\nGQRs----------\n");
    for (i = 0; i < 4; ++i) {
        OSReport("gqr%d = 0x%08x \t gqr%d = 0x%08x\n", i, context->gqr[i], i + 4, context->gqr[i + 4]);
    }

    if (context->state & OS_CONTEXT_STATE_FPSAVED) {
        OSContext* currentContext;
        OSContext fpucontext;
        BOOL enabled;

        enabled = OSDisableInterrupts();
        currentContext = OSGetCurrentContext();
        OSClearContext(&fpucontext);
        OSSetCurrentContext(&fpucontext);

        OSReport("\n\nFPRs----------\n");
        for (i = 0; i < 32; i += 2) {
            OSReport("fr%d \t= %d \t fr%d \t= %d\n", i, (u32)context->fpr[i], i + 1,
                    (u32)context->fpr[i + 1]);
        }
        OSReport("\n\nPSFs----------\n");
        for (i = 0; i < 32; i += 2) {
            OSReport("ps%d \t= 0x%x \t ps%d \t= 0x%x\n", i, (u32)context->psf[i], i + 1,
                    (u32)context->psf[i + 1]);
        }

        OSClearContext(&fpucontext);
        OSSetCurrentContext(currentContext);
        OSRestoreInterrupts(enabled);
    }

    OSReport("\nAddress:      Back Chain    LR Save\n");
    for (i = 0, p = (u32*)context->gpr[1]; p && (u32)p != 0xffffffff && i++ < 16; p = (u32*)*p) {
        OSReport("0x%08x:   0x%08x    0x%08x\n", p, p[0], p[1]);
    }
}

#ifdef __GEKKO__
static asm void OSSwitchFPUContext(__REGISTER __OSException exception, __REGISTER OSContext* context) {
    nofralloc
    mfmsr   r5
    ori     r5, r5, 0x2000
    mtmsr   r5
    isync
    lwz     r5, OS_CONTEXT_SRR1(context)
    ori     r5, r5, 0x2000
    mtsrr1  r5
    addis   r3, r0, OS_CACHED_REGION_PREFIX
    lwz     r5, 0x00D8(r3) 
    stw     context, 0x00D8(r3)
    cmpw    r5, r4
    beq     _restoreAndExit
    cmpwi   r5, 0x0
    beq     _loadNewFPUContext
    bl      __OSSaveFPUContext
_loadNewFPUContext:
    bl      __OSLoadFPUContext
_restoreAndExit:
    lwz     r3, OS_CONTEXT_CR(context)
    mtcr    r3
    lwz     r3, OS_CONTEXT_LR(context)
    mtlr    r3
    lwz     r3, OS_CONTEXT_SRR0(context)
    mtsrr0  r3
    lwz     r3, OS_CONTEXT_CTR(context)
    mtctr   r3
    lwz     r3, OS_CONTEXT_XER(context)
    mtxer   r3
    lhz     r3, context->state
    rlwinm  r3, r3, 0, 31, 29
    sth     r3, context->state
    lwz     r5, OS_CONTEXT_R5(context)
    lwz     r3, OS_CONTEXT_R3(context)
    lwz     r4, OS_CONTEXT_R4(context)
    rfi
}
#endif

void __OSContextInit(void) {
    __OSSetExceptionHandler(__OS_EXCEPTION_FLOATING_POINT, OSSwitchFPUContext);
    __OSFPUContext = NULL;
    DBPrintf("FPU-unavailable handler installed\n");
}

#ifdef __GEKKO__
asm void OSFillFPUContext(__REGISTER OSContext* context) {
    nofralloc
    mfmsr   r5
    ori     r5, r5, 0x2000
    mtmsr   r5
    isync

    stfd    fp0,  context->fpr[0]
    stfd    fp1,  context->fpr[1]
    stfd    fp2,  context->fpr[2]
    stfd    fp3,  context->fpr[3]
    stfd    fp4,  context->fpr[4]
    stfd    fp5,  context->fpr[5]
    stfd    fp6,  context->fpr[6]
    stfd    fp7,  context->fpr[7]
    stfd    fp8,  context->fpr[8]
    stfd    fp9,  context->fpr[9]
    stfd    fp10, context->fpr[10]
    stfd    fp11, context->fpr[11]
    stfd    fp12, context->fpr[12]
    stfd    fp13, context->fpr[13]
    stfd    fp14, context->fpr[14]
    stfd    fp15, context->fpr[15]
    stfd    fp16, context->fpr[16]
    stfd    fp17, context->fpr[17]
    stfd    fp18, context->fpr[18]
    stfd    fp19, context->fpr[19]
    stfd    fp20, context->fpr[20]
    stfd    fp21, context->fpr[21]
    stfd    fp22, context->fpr[22]
    stfd    fp23, context->fpr[23]
    stfd    fp24, context->fpr[24]
    stfd    fp25, context->fpr[25]
    stfd    fp26, context->fpr[26]
    stfd    fp27, context->fpr[27]
    stfd    fp28, context->fpr[28]
    stfd    fp29, context->fpr[29]
    stfd    fp30, context->fpr[30]
    stfd    fp31, context->fpr[31]

    mffs    fp0
    stfd    fp0,  OS_CONTEXT_FPSCR(context)

    lfd     fp0,  context->fpr[0]

    mfspr   r5, HID2
    rlwinm. r5, r5, 3, 31, 31
    bc      12, 2, _return

    psq_st  fp0, OS_CONTEXT_PSF0(context), 0, 0
    psq_st  fp1, OS_CONTEXT_PSF1(context), 0, 0
    psq_st  fp2, OS_CONTEXT_PSF2(context), 0, 0
    psq_st  fp3, OS_CONTEXT_PSF3(context), 0, 0
    psq_st  fp4, OS_CONTEXT_PSF4(context), 0, 0
    psq_st  fp5, OS_CONTEXT_PSF5(context), 0, 0
    psq_st  fp6, OS_CONTEXT_PSF6(context), 0, 0
    psq_st  fp7, OS_CONTEXT_PSF7(context), 0, 0
    psq_st  fp8, OS_CONTEXT_PSF8(context), 0, 0
    psq_st  fp9, OS_CONTEXT_PSF9(context), 0, 0
    psq_st  fp10, OS_CONTEXT_PSF10(context), 0, 0
    psq_st  fp11, OS_CONTEXT_PSF11(context), 0, 0
    psq_st  fp12, OS_CONTEXT_PSF12(context), 0, 0
    psq_st  fp13, OS_CONTEXT_PSF13(context), 0, 0
    psq_st  fp14, OS_CONTEXT_PSF14(context), 0, 0
    psq_st  fp15, OS_CONTEXT_PSF15(context), 0, 0
    psq_st  fp16, OS_CONTEXT_PSF16(context), 0, 0
    psq_st  fp17, OS_CONTEXT_PSF17(context), 0, 0
    psq_st  fp18, OS_CONTEXT_PSF18(context), 0, 0
    psq_st  fp19, OS_CONTEXT_PSF19(context), 0, 0
    psq_st  fp20, OS_CONTEXT_PSF20(context), 0, 0
    psq_st  fp21, OS_CONTEXT_PSF21(context), 0, 0
    psq_st  fp22, OS_CONTEXT_PSF22(context), 0, 0
    psq_st  fp23, OS_CONTEXT_PSF23(context), 0, 0
    psq_st  fp24, OS_CONTEXT_PSF24(context), 0, 0
    psq_st  fp25, OS_CONTEXT_PSF25(context), 0, 0
    psq_st  fp26, OS_CONTEXT_PSF26(context), 0, 0
    psq_st  fp27, OS_CONTEXT_PSF27(context), 0, 0
    psq_st  fp28, OS_CONTEXT_PSF28(context), 0, 0
    psq_st  fp29, OS_CONTEXT_PSF29(context), 0, 0
    psq_st  fp30, OS_CONTEXT_PSF30(context), 0, 0
    psq_st  fp31, OS_CONTEXT_PSF31(context), 0, 0

_return:
    blr
}
#endif
