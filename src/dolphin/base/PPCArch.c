#include <dolphin.h>
#include <dolphin/base/PPCArch.h>

asm u32 PPCMfmsr() {
    nofralloc
    mfmsr r3
    blr    
}

asm void PPCMtmsr(__REGISTER u32 newMSR) {
    nofralloc
    mtmsr newMSR
    blr
}

asm u32 PPCOrMsr(__REGISTER u32 value) {
    nofralloc
    mfmsr r4
    or value, r4, value
    blr
}

asm u32 PPCAndMsr(__REGISTER u32 value) {
    nofralloc
    mfmsr r4
    and value, r4, value
    blr
}

asm u32 PPCAndCMsr(__REGISTER u32 value) {
    nofralloc
    mfmsr r4
    andc value, r4, value
    blr
}

asm u32 PPCMfhid0() {
    nofralloc
    mfspr r3, HID0
    blr
}

asm void PPCMthid0(__REGISTER u32 newHID0) {
    nofralloc
    mtspr HID0, newHID0
    blr
}

asm u32 PPCMfhid1() {
    nofralloc
    mfspr r3, HID1
    blr
}

asm u32 PPCMfl2cr() {
    nofralloc
    mfspr r3, L2CR
    blr
}

asm void PPCMtl2cr(__REGISTER u32 newL2cr) {
    nofralloc
    mtspr L2CR, newL2cr
    blr
}

asm void PPCMtdec(__REGISTER u32 newDec) {
    nofralloc
    mtdec newDec
    blr
}

asm u32 PPCMfdec() {
    nofralloc
    mfdec r3
    blr
}

asm void PPCSync() {
    nofralloc
    sc
    blr
}

asm void PPCEieio() {
    nofralloc
    mfmsr r5
    rlwinm r6, r5, 0, 17, 15
    mtmsr r6
    mfspr r3, HID0
    ori r4, r3, 0x8
    mtspr HID0, r4
    isync
    eieio
    isync
    mtspr HID0, r3
    mtmsr r5
    isync
    blr
}

asm void PPCHalt() {
    nofralloc
    sync
loop:
    nop
    li r3, 0
    nop
    b loop
}

asm u32 PPCMfmmcr0() {
    nofralloc
    mfspr r3, MMCR0
    blr
}

asm void PPCMtmmcr0(__REGISTER u32 newMmcr0) {
    nofralloc
    mtspr MMCR0, newMmcr0
    blr
}

asm u32 PPCMfmmcr1() {
    nofralloc
    mfspr r3, MMCR1
    blr
}

asm void PPCMtmmcr1(__REGISTER u32 newMmcr1) {
    nofralloc
    mtspr MMCR1, newMmcr1
    blr
}

asm u32 PPCMfpmc1() {
    nofralloc
    mfspr r3, PMC1
    blr
}

asm void PPCMtpmc1(__REGISTER u32 newPmc1) {
    nofralloc
    mtspr PMC1, newPmc1
    blr
}

asm u32 PPCMfpmc2() {
    nofralloc
    mfspr r3, PMC2
    blr
}

asm void PPCMtpmc2(__REGISTER u32 newPmc2) {
    nofralloc
    mtspr PMC2, newPmc2
    blr
}

asm u32 PPCMfpmc3() {
    nofralloc
    mfspr r3, PMC3
    blr
}

asm void PPCMtpmc3(__REGISTER u32 newPmc3) {
    nofralloc
    mtspr PMC3, newPmc3
    blr
}

asm u32 PPCMfpmc4() {
    nofralloc
    mfspr r3, PMC4
    blr
}

asm void PPCMtpmc4(__REGISTER u32 newPmc4) {
    nofralloc
    mtspr PMC4, newPmc4
    blr
}

asm u32 PPCMfsia() {
    nofralloc
    mfspr r3, SIA
    blr
}

asm void PPCMtsia(__REGISTER u32 newSia) {
    nofralloc
    mtspr SIA, newSia
    blr
}

u32 PPCMffpscr() {
    union FpscrUnion m;

    asm {
        mffs fp31
        stfd fp31, m.f;
    }

    return m.u.fpscr; 
}

void PPCMtfpscr(__REGISTER u32 newFPSCR) {
    union FpscrUnion m;

    asm {
        li    r4, 0
        stw   r4, m.u.fpscr_pad;
        stw   newFPSCR, m.u.fpscr
        lfd   fp31, m.f
        mtfsf 0xff, fp31
    }
}

asm u32 PPCMfhid2() {
    nofralloc
    mfspr r3, HID2
    blr
}

asm void PPCMthid2(__REGISTER u32 newhid2) {
    nofralloc
    mtspr HID2, newhid2
    blr
}

asm u32 PPCMfwpar() {
    nofralloc
    sync
    mfspr r3, WPAR
    blr
}

asm void PPCMtwpar(__REGISTER u32 newwpar) {
    nofralloc
    mtspr WPAR, newwpar
    blr
}

asm u32 PPCMfdmaU() {
    nofralloc
    mfspr r3, DMA_U
    blr
}

asm u32 PPCMfdmaL() {
    nofralloc
    mfspr r3, DMA_L
    blr
}

asm void PPCMtdmaU(__REGISTER u32 newdmau) {
    nofralloc
    mtspr DMA_U, newdmau
    blr
}

asm void PPCMtdmaL(__REGISTER u32 newdmal) {
    nofralloc
    mtspr DMA_L, newdmal
    blr
}

asm u32 PPCMfpvr() {
    nofralloc
    mfspr r3, PVR
    blr
}

void PPCEnableSpeculation(void) {
    PPCMthid0(PPCMfhid0() & ~HID0_SPD);
}

void PPCDisableSpeculation(void) {
    PPCMthid0(PPCMfhid0() | HID0_SPD);
}

asm void PPCSetFpIEEEMode() {
	nofralloc
	mtfsb0 29
	blr
}

asm void PPCSetFpNonIEEEMode() {
	nofralloc
	mtfsb1 29
	blr
}
