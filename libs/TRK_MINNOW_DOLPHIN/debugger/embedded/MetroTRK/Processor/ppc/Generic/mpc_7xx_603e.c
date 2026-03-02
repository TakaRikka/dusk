#include "TRK_MINNOW_DOLPHIN/ppc/Generic/mpc_7xx_603e.h"
#include "TRK_MINNOW_DOLPHIN/ppc/Generic/targimpl.h"

extern u8 gTRKRestoreFlags[9 + 3 /* padding */];

asm void TRKSaveExtended1Block() {
    // clang-format off
    nofralloc
    lis r2, gTRKCPUState@h /* 0x8044F338@h */
    ori r2, r2, gTRKCPUState@l /* 0x8044F338@l */
    mfsr r16, 0
    mfsr r17, 1
    mfsr r18, 2
    mfsr r19, 3
    mfsr r20, 4
    mfsr r21, 5
    mfsr r22, 6
    mfsr r23, 7
    mfsr r24, 8
    mfsr r25, 9
    mfsr r26, 0xa
    mfsr r27, 0xb
    mfsr r28, 0xc
    mfsr r29, 0xd
    mfsr r30, 0xe
    mfsr r31, 0xf
    stmw r16, 0x1a8(r2)
    mftb r10, 0x10c
    mftbu r11
    mfspr r12, 0x3f0
    mfspr r13, 0x3f1
    mfspr r14, 0x1b
    mfpvr r15
    mfibatu r16, 0
    mfibatl r17, 0
    mfibatu r18, 1
    mfibatl r19, 1
    mfibatu r20, 2
    mfibatl r21, 2
    mfibatu r22, 3
    mfibatl r23, 3
    mfdbatu r24, 0
    mfdbatl r25, 0
    mfdbatu r26, 1
    mfdbatl r27, 1
    mfdbatu r28, 2
    mfdbatl r29, 2
    mfdbatu r30, 3
    mfdbatl r31, 3
    stmw r10, 0x1e8(r2)
    mfspr r22, 0x19
    mfdar r23
    mfdsisr r24
    mfspr r25, 0x110
    mfspr r26, 0x111
    mfspr r27, 0x112
    mfspr r28, 0x113
    li r29, 0
    mfspr r30, 0x3f2
    mfspr r31, 0x11a
    stmw r22, 0x25c(r2)
    mfspr r20, 0x390
    mfspr r21, 0x391
    mfspr r22, 0x392
    mfspr r23, 0x393
    mfspr r24, 0x394
    mfspr r25, 0x395
    mfspr r26, 0x396
    mfspr r27, 0x397
    mfspr r28, 0x398
    mfspr r29, 0x399
    mfspr r30, 0x39a
    mfspr r31, 0x39b
    stmw r20, 0x2fc(r2)
    b lbl_80371340
    mfspr r16, 0x3a0
    mfspr r17, 0x3a7
    mfspr r18, 0x3a8
    mfspr r19, 0x3a9
    mfspr r20, 0x3aa
    mfspr r21, 0x3ab
    mfspr r22, 0x3ac
    mfspr r23, 0x3ad
    mfspr r24, 0x3ae
    mfspr r25, 0x3af
    mfspr r26, 0x3b0
    mfspr r27, 0x3b7
    mfspr r28, 0x3bf
    mfspr r29, 0x3f6
    mfspr r30, 0x3f7
    mfspr r31, 0x3ff
    stmw r16, 0x2b8(r2)

lbl_80371340:
    mfspr r19, 0x3f5
    mfspr r20, 0x3b9
    mfspr r21, 0x3ba
    mfspr r22, 0x3bd
    mfspr r23, 0x3be
    mfspr r24, 0x3bb
    mfspr r25, 0x3b8
    mfspr r26, 0x3bc
    mfspr r27, 0x3fc
    mfspr r28, 0x3fd
    mfspr r29, 0x3fe
    mfspr r30, 0x3FB
    mfspr r31, 0x3f9
    stmw r19, 0x284(r2)
    blr 
    mfspr r25, 0x3d0
    mfspr r26, 0x3d1
    mfspr r27, 0x3d2
    mfspr r28, 0x3d3
    mfspr r29, 0x3D4
    mfspr r30, 0x3D5
    mfspr r31, 0x3d6
    stmw r25, 0x240(r2)
    mfspr r31, 0x16
    stw r31, 0x278(r2)
    blr
    // clang-format on
}

asm void TRKRestoreExtended1Block() {
    // clang-format off
    nofralloc
    lis r2, gTRKCPUState@h /* 0x8044F338@h */
    ori r2, r2, gTRKCPUState@l /* 0x8044F338@l */
    lis r5, gTRKRestoreFlags@h /* 0x803D3238@h */
    ori r5, r5, gTRKRestoreFlags@l /* 0x803D3238@l */
    lbz r3, 0(r5)
    lbz r6, 1(r5)
    li r0, 0
    stb r0, 0(r5)
    stb r0, 1(r5)
    cmpwi r3, 0
    beq lbl_803713E4
    lwz r24, 0x1e8(r2)
    lwz r25, 0x1ec(r2)
    mttbl r24
    mttbu r25
lbl_803713E4:
    lmw r20, 0x2fc(r2)
    mtspr 0x390, r20
    mtspr 0x391, r21
    mtspr 0x392, r22
    mtspr 0x393, r23
    mtspr 0x394, r24
    mtspr 0x395, r25
    mtspr 0x396, r26
    mtspr 0x397, r27
    mtspr 0x398, r28
    mtspr 0x39a, r30
    mtspr 0x39b, r31
    b lbl_80371430
    lmw r26, 0x2e0(r2)
    mtspr 0x3b0, r26
    mtspr 0x3b7, r27
    mtspr 0x3f6, r29
    mtspr 0x3f7, r30
    mtspr 0x3ff, r31
lbl_80371430:
    lmw r19, 0x284(r2)
    mtspr 0x3f5, r19
    mtspr 0x3b9, r20
    mtspr 0x3ba, r21
    mtspr 0x3bd, r22
    mtspr 0x3be, r23
    mtspr 0x3bb, r24
    mtspr 0x3b8, r25
    mtspr 0x3bc, r26
    mtspr 0x3fc, r27
    mtspr 0x3fd, r28
    mtspr 0x3fe, r29
    mtspr 0x3FB, r30
    mtspr 0x3f9, r31
    b lbl_8037149C
    cmpwi r6, 0
    beq lbl_8037147C
    lwz r26, 0x278(r2)
    mtspr 0x16, r26
lbl_8037147C:
    lmw r25, 0x240(r2)
    mtspr 0x3d0, r25
    mtspr 0x3d1, r26
    mtspr 0x3d2, r27
    mtspr 0x3d3, r28
    mtspr 0x3D4, r29
    mtspr 0x3D5, r30
    mtspr 0x3d6, r31
lbl_8037149C:
    lmw r16, 0x1a8(r2)
    mtsr 0, r16
    mtsr 1, r17
    mtsr 2, r18
    mtsr 3, r19
    mtsr 4, r20
    mtsr 5, r21
    mtsr 6, r22
    mtsr 7, r23
    mtsr 8, r24
    mtsr 9, r25
    mtsr 0xa, r26
    mtsr 0xb, r27
    mtsr 0xc, r28
    mtsr 0xd, r29
    mtsr 0xe, r30
    mtsr 0xf, r31
    lmw r12, 0x1f0(r2)
    mtspr 0x3f0, r12
    mtspr 0x3f1, r13
    mtspr 0x1b, r14
    mtspr 0x11f, r15
    mtibatu 0, r16
    mtibatl 0, r17
    mtibatu 1, r18
    mtibatl 1, r19
    mtibatu 2, r20
    mtibatl 2, r21
    mtibatu 3, r22
    mtibatl 3, r23
    mtdbatu 0, r24
    mtdbatl 0, r25
    mtdbatu 1, r26
    mtdbatl 1, r27
    mtdbatu 2, r28
    mtdbatl 2, r29
    mtdbatu 3, r30
    mtdbatl 3, r31
    lmw r22, 0x25c(r2)
    mtspr 0x19, r22
    mtdar r23
    mtdsisr r24
    mtspr 0x110, r25
    mtspr 0x111, r26
    mtspr 0x112, r27
    mtspr 0x113, r28
    mtspr 0x3f2, r30
    mtspr 0x11a, r31
    blr
    // clang-format on
}
