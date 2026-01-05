#include "TRK_MINNOW_DOLPHIN/Os/dolphin/dolphin_trk.h"
#include "TRK_MINNOW_DOLPHIN/MetroTRK/Portable/main_TRK.h"
#include "TRK_MINNOW_DOLPHIN/MetroTRK/Portable/mem_TRK.h"
#include "TRK_MINNOW_DOLPHIN/Os/dolphin/dolphin_trk_glue.h"
#include "TRK_MINNOW_DOLPHIN/ppc/Generic/targimpl.h"
#include <dolphin/ar.h>
#include "global.h"

extern u32 _db_stack_addr;

#define EXCEPTIONMASK_ADDR 0x80000044

static u32 lc_base;

static u32 TRK_ISR_OFFSETS[15] = {PPC_SystemReset,
                                  PPC_MachineCheck,
                                  PPC_DataStorage,
                                  PPC_InstructionStorage,
                                  PPC_ExternalInterrupt,
                                  PPC_Alignment,
                                  PPC_Program,
                                  PPC_FloatingPointUnavaiable,
                                  PPC_Decrementer,
                                  PPC_SystemCall,
                                  PPC_Trace,
                                  PPC_PerformanceMonitor,
                                  PPC_InstructionAddressBreakpoint,
                                  PPC_SystemManagementInterrupt,
                                  PPC_ThermalManagementInterrupt};

void __TRK_copy_vectors(void);
__declspec(section ".init") void __TRK_reset(void) { OSResetSystem(FALSE, 0, FALSE); }

void EnableMetroTRKInterrupts(void) {
    EnableEXI2Interrupts();
}

u32 TRKTargetTranslate(u32 param_0) {
    if (param_0 >= lc_base) {
        if ((param_0 < lc_base + 0x4000) && ((gTRKCPUState.Extended1.DBAT3U & 3) != 0)) {
            return param_0;
        }
    }
    if ((0x7E000000 <= param_0) && (param_0 <= 0x80000000)) {
        return param_0;
    }
    return param_0 & 0x3FFFFFFF | 0x80000000;
}

extern u8 gTRKInterruptVectorTable[];

void TRK_copy_vector(u32 offset) {
    void* destPtr = (void*)TRKTargetTranslate(offset);
    TRK_memcpy(destPtr, (void*)(gTRKInterruptVectorTable + offset), 0x100);
    TRK_flush_cache(destPtr, 0x100);
}

void __TRK_copy_vectors(void) {
    u32 r3 = lc_base;
    u32* isrOffsetPtr;
    int i;
    u32 r29;

    if (r3 <= 0x44 && r3 + 0x4000 > 0x44 && gTRKCPUState.Extended1.DBAT3U & 3) {
        r3 = 0x44;
    } else {
        r3 = EXCEPTIONMASK_ADDR;
    }

    i = 0;
    r29 = *(u32*)r3;
    isrOffsetPtr = TRK_ISR_OFFSETS;

    do {
        if ((r29 & (1 << i)) && i != 4) {
            TRK_copy_vector(isrOffsetPtr[i]);
        }

        i++;
    } while (i <= 14);
}

DSError TRKInitializeTarget() {
    gTRKState.isStopped = TRUE;
    gTRKState.msr = __TRK_get_MSR();
    lc_base = 0xE0000000;
    return DS_NoError;
}

#define __dcbi(a, b) asm { dcbi a, b }
#define __dcbfASM(a, b) asm { dcbf a, b }

void TRK__read_aram(__REGISTER int c, __REGISTER u32 p2, void* p3) {
    u32 err;
    int i;
    __REGISTER int counter;
    u16 r;
    u32 g;
    u32 x;
    u32 size;

    if ((size_t)p2 < 0x4000 || p2 + *(u32*)p3 > 0x8000000) {
        return;
    }

    x = p2 & ~0x1F;
    size = *(u32*)p3 + (p2 & 0x1F);
    size = OSRoundUp32B(size);
    counter = 0;

    for (i = 0; i < size; i += 0x20) {
        __dcbi(counter, c);
        counter += 0x20;
    }

    do {
        err = ARGetDMAStatus();
    } while (err);

    r = __ARGetInterruptStatus();
    g = 0x8000000;
    __ARClearInterrupt();

    ARStartDMA(1, c, x, size);

    while (!__ARGetInterruptStatus()) {
    }

    if (!r) {
        __ARClearInterrupt();
    }
}

void TRK__write_aram(__REGISTER int c, __REGISTER u32 p2, void* p3) {
    u8 buff[32] ATTRIBUTE_ALIGN(32);
    u32 err;
    __REGISTER int count = c;
    __REGISTER u32 bf;
    u32 uVar1;
    u32 size;
    u16 r;
    __REGISTER u32 g;
    __REGISTER int counter;
    u32 i;

    if ((size_t)p2 < 0x4000 || p2 + *(u32*)p3 > 0x8000000) {
        return;
    }

    uVar1 = p2 & ~0x1f;
    counter = 0;
    size = *(u32*)p3 + (p2 & 0x1f);
    size = OSRoundUp32B(size);

    for (i = 0; i < size; i += 0x20) {
        __dcbf((void*)counter, count);
        counter += 0x20;
    }

    do {
        err = ARGetDMAStatus();
    } while (err);

    r = __ARGetInterruptStatus();
    g = 0x8000000;

    counter = p2 & 0x1f;
    if (counter) {
        g = uVar1;
        bf = (u32)buff;
        __dcbi(r0, bf);
        __ARClearInterrupt();

        ARStartDMA(1, bf, uVar1, 0x20);

        while (!__ARGetInterruptStatus()) {
        }

        TRK_memcpy((void*)c, buff, counter);
        __dcbfASM(r0, c);
    }

    p2 += *(u32*)p3;
    counter = p2 & 0x1f;
    if (counter) {
        u32 val = p2 & ~0x1F;
        if (val != g) {
            bf = (u32)buff;
            __dcbi(r0, bf);
            __ARClearInterrupt();
            ARStartDMA(1, bf, val, 0x20);

            while (!__ARGetInterruptStatus()) {
            }
        }
        g = c + p2;
        TRK_memcpy((void*)g, (void*)(buff + counter), 0x20 - counter);

        __dcbfASM(r0, g);
    }
    __sync();
    __ARClearInterrupt();
    ARStartDMA(0, c, uVar1, size);
    if (!r) {
        while (!__ARGetInterruptStatus()) {
        }

        __ARClearInterrupt();
    }
}

asm void InitMetroTRK() {
    // clang-format off
    nofralloc
	addi r1, r1, -4
	stw r3, 0(r1)
	lis r3, gTRKCPUState@h
	ori r3, r3, gTRKCPUState@l
	stmw r0, ProcessorState_PPC.Default.GPR(r3) //Save the gprs
	lwz r4, 0(r1)
	addi r1, r1, 4
	stw r1, ProcessorState_PPC.Default.GPR[1](r3)
	stw r4, ProcessorState_PPC.Default.GPR[3](r3)
	mflr r4
	stw r4, ProcessorState_PPC.Default.LR(r3)
	stw r4, ProcessorState_PPC.Default.PC(r3)
	mfcr r4
	stw r4, ProcessorState_PPC.Default.CR(r3)
	//???
	mfmsr r4
	ori r3, r4, (1 << (31 - 16))
	xori r3, r3, (1 << (31 - 16))
	mtmsr r3
	mtsrr1 r4 //Copy msr to srr1
	//Save misc registers to gTRKCPUState
	bl TRKSaveExtended1Block
	lis r3, gTRKCPUState@h
	ori r3, r3, gTRKCPUState@l
	lmw r0, ProcessorState_PPC.Default.GPR(r3) //Restore the gprs
	//Reset IABR and DABR
	li r0, 0
	mtspr  0x3f2, r0
	mtspr  0x3f5, r0
	//Restore stack pointer
	lis r1, _db_stack_addr@h
	ori r1, r1, _db_stack_addr@l
	mr r3, r5
	bl InitMetroTRKCommTable //Initialize comm table
	/*
	If InitMetroTRKCommTable returned 1 (failure), an invalid hardware
	id or the id for GDEV was somehow passed. Since only BBA or NDEV
	are supported, we return early. Otherwise, we proceed with
	starting up TRK.
	*/
	cmpwi r3, 1
	bne initCommTableSuccess
	/*
	BUG: The code probably orginally reloaded gTRKCPUState here, but
	as is it will read the returned value of InitMetroTRKCommTable
	as a TRKCPUState struct pointer, causing the CPU to return to
	a garbage code address.
	*/
	lwz r4, ProcessorState_PPC.Default.LR(r3)
	mtlr r4
	lmw r0, ProcessorState_PPC.Default.GPR(r3) //Restore the gprs
	blr
initCommTableSuccess:
	b TRK_main //Jump to TRK_main
	blr
    // clang-format on
}

asm void InitMetroTRK_BBA() {
    // clang-format off
    nofralloc
	addi r1, r1, -4
	stw r3, 0(r1)
	lis r3, gTRKCPUState@h
	ori r3, r3, gTRKCPUState@l
	stmw r0, ProcessorState_PPC.Default.GPR(r3) //Save the gprs
	lwz r4, 0(r1)
	addi r1, r1, 4
	stw r1, ProcessorState_PPC.Default.GPR[1](r3)
	stw r4, ProcessorState_PPC.Default.GPR[3](r3)
	mflr r4
	stw r4, ProcessorState_PPC.Default.LR(r3)
	stw r4, ProcessorState_PPC.Default.PC(r3)
	mfcr r4
	stw r4, ProcessorState_PPC.Default.CR(r3)
	//Turn on external interrupts
	mfmsr r4
	ori r3, r4, (1 << (31 - 16))
	mtmsr r3
	mtsrr1 r4 //Copy original msr to srr1
	//Save misc registers to gTRKCPUState
	bl TRKSaveExtended1Block
	lis r3, gTRKCPUState@h
	ori r3, r3, gTRKCPUState@l
	lmw r0, ProcessorState_PPC.Default.GPR(r3) //Restore the gprs
	//Reset IABR and DABR
	li r0, 0
	mtspr  0x3f2, r0
	mtspr 0x3f5, r0
	//Restore the stack pointer
	lis r1, _db_stack_addr@h
	ori r1, r1, _db_stack_addr@l
	li r3, 2
	bl InitMetroTRKCommTable //Initialize comm table as BBA hardware
	/*
	If InitMetroTRKCommTable returned 1 (failure), something went wrong
	or whatever reason. If everything goes as expected, we proceed with
	starting up TRK.
	*/
	cmpwi r3, 1
	bne initCommTableSuccess
	/*
	BUG: The code probably orginally reloaded gTRKCPUState here, but
	as is it will read the returned value of InitMetroTRKCommTable
	as a TRKCPUState struct pointer, causing the CPU to return to
	a garbage code address.
	*/
	lwz r4, ProcessorState_PPC.Default.LR(r3)
	mtlr r4
	lmw r0, ProcessorState_PPC.Default.GPR(r3)
	blr
initCommTableSuccess:
	b TRK_main //Jump to TRK_main
	blr
    // clang-format on
}
