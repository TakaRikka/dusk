#include <dolphin.h>
#include <dolphin/ax.h>
#include <dolphin/axfx.h>
#include "fake_tgmath.h"

#include "__axfx.h"

static void ReverbHIDpl2Free(AXFX_REVHI_WORK_DPL2* rv);

static void DLsetdelayDpl2(AXFX_REVHI_DELAYLINE* dl, s32 lag) {
    dl->outPoint = dl->inPoint - (lag * 4);
    while (dl->outPoint < 0) {
        dl->outPoint += dl->length;
    }
}

static int DLcreateDpl2(AXFX_REVHI_DELAYLINE* dl, s32 max_length) {
    dl->length = (max_length * 4);
    dl->inputs = __AXFXAlloc(max_length << 2);
    ASSERTMSGLINE(62, dl->inputs, "Can't allocate the memory.");
    if (dl->inputs == NULL) {
        return 0;
    }

    memset(dl->inputs, 0, max_length << 2);
    dl->lastOutput = 0.0f;
    DLsetdelayDpl2(dl, max_length >> 1);
    dl->inPoint = 0;
    dl->outPoint = 0;
    return 1;
}

static void DLdeleteDpl2(AXFX_REVHI_DELAYLINE* dl) {
    __AXFXFree(dl->inputs);
}

static int ReverbHICreateDpl2(AXFX_REVHI_WORK_DPL2* rv, f32 coloration, f32 time, f32 mix, f32 damping, f32 preDelay) {
    u8 i;
    u8 k;

    static s32 lens[9] = {
        0x000006FD,
        0x000007CF,
        0x0000091D,
        0x000001B1,
        0x00000095,
        0x0000002F,
        0x00000049,
        0x00000043,
        0x00000047
    };

    ASSERTMSGLINE(117, coloration >= 0.0f && coloration <= 1.0f &&
                           time >= 0.01f && time <= 10.0f &&
                           mix >= 0.0f && mix <= 1.0f &&
                           damping >= 0.0f && damping <= 1.0f &&
                           preDelay >= 0.0f && preDelay <= 0.1f,
                           "The value of specified parameter is out of range.");

    if ((coloration < 0.0f ) || (coloration > 1.0f )
     || (time       < 0.01f) || (time       > 10.0f)
     || (mix        < 0.0f ) || (mix        > 1.0f )
     || (damping    < 0.0f ) || (damping    > 1.0f )
     || (preDelay   < 0.0f ) || (preDelay   > 0.1f )) {
        return 0;
    }

    memset(rv, 0, sizeof(AXFX_REVHI_WORK_DPL2));

    for (k = 0; k < 4; k++) {
        for (i = 0; i < 3; i++) {
            if (DLcreateDpl2(&rv->C[i + (k * 3)], lens[i] + 2) == 0) {
                ReverbHIDpl2Free(rv);
                return 0;
            }

            DLsetdelayDpl2(&rv->C[i + (k * 3)], lens[i]);
            rv->combCoef[i + (k * 3)] = powf(10.0f, (lens[i] * -3) / (32000.0f * time));
        }

        for (i = 0; i < 2; i++) {
            if (DLcreateDpl2(&rv->AP[i + (k * 3)], lens[i + 3] + 2) == 0) {
                ReverbHIDpl2Free(rv);
                return 0;
            }

            DLsetdelayDpl2(&rv->AP[i + (k * 3)], lens[i + 3]);
        }

        if (DLcreateDpl2(&rv->AP[2 + (k * 3)], lens[k + 5] + 2) == 0) {
            ReverbHIDpl2Free(rv);
            return 0;
        }

        DLsetdelayDpl2(&rv->AP[2 + (k * 3)], lens[k + 5]);
        rv->lpLastout[k] = 0.0f;
    }

    rv->allPassCoeff = coloration;
    rv->level = mix;
    rv->damping = damping;
    if (rv->damping < 0.05f) {
        rv->damping = 0.05f;
    }

    rv->damping = (1.0f - (0.05f + (0.8f * rv->damping)));
    if (0.0f != preDelay) {
        rv->preDelayTime = (32000.0f * preDelay);
        for (i = 0; i < 4; i++) {
            rv->preDelayLine[i] = __AXFXAlloc(rv->preDelayTime * 4);
            ASSERTMSGLINE(188, rv->preDelayLine[i], "Can't allocate the memory.");
            if (rv->preDelayLine[i] == NULL) {
                ReverbHIDpl2Free(rv);
                return 0;
            }

            memset(rv->preDelayLine[i], 0, rv->preDelayTime * 4);
            rv->preDelayPtr[i] = rv->preDelayLine[i];
        }
    } else {
        rv->preDelayTime = 0;
        for (i = 0; i < 4; i++) {
            rv->preDelayPtr[i] = 0;
            rv->preDelayLine[i] = 0;
        }
    }

    return 1;
}

static int ReverbHIModifyDpl2(AXFX_REVHI_WORK_DPL2* rv, f32 coloration, f32 time, f32 mix, f32 damping, f32 preDelay) {
    u8 i;

    ASSERTMSGLINE(222, coloration >= 0.0f && coloration <= 1.0f &&
                  time >= 0.01f && time <= 10.0f &&
                  mix >= 0.0f && mix <= 1.0f &&
                  damping >= 0.0f && damping <= 1.0f &&
                  preDelay >= 0.0f && preDelay <= 0.1f,
                  "The value of specified parameter is out of range.");

    if ((coloration < 0.0f ) || (coloration > 1.0f  )
     || (time       < 0.01f) || (time       > 10.0f )
     || (mix        < 0.0f ) || (mix        > 1.0f  )
     || (damping    < 0.0f ) || (damping    > 1.0f  )
     || (preDelay   < 0.0f ) || (preDelay   > 0.1f)) {
        return 0;
    }

    rv->allPassCoeff = coloration;
    rv->level = mix;
    rv->damping = damping;
    if (rv->damping < 0.05f) {
        rv->damping = 0.05f;
    }
    rv->damping = (1.0f - (0.05f + (0.8f * rv->damping)));

    for (i = 0; i < 12; i++) {
        DLdeleteDpl2(&rv->AP[i]);
    }

    for (i = 0; i < 12; i++) {
        DLdeleteDpl2(&rv->C[i]);
    }

    if (rv->preDelayTime) {
        for (i = 0; i < 4; i++) {
            __AXFXFree(rv->preDelayLine[i]);
        }
    }

    return ReverbHICreateDpl2(rv, coloration, time, mix, damping, preDelay);
}


const static f32 value0_6 = 0.6f;
const static f32 value0_3 = 0.3f;
const static double i2fMagic = 4503601774854144.0;

asm static void HandleReverbDpl2(__REGISTER s32* sptr, __REGISTER AXFX_REVHI_WORK_DPL2* rv, __REGISTER s32 k) {
    nofralloc
	stwu r1, -0xc0(r1)
	stmw r14, 0x8(r1)
	stfd f14, 0x60(r1)
	stfd f15, 0x68(r1)
	stfd f16, 0x70(r1)
	stfd f17, 0x78(r1)
	stfd f18, 0x80(r1)
	stfd f19, 0x88(r1)
	stfd f20, 0x90(r1)
	stfd f21, 0x98(r1)
	stfd f22, 0xa0(r1)
	stfd f23, 0xa8(r1)
	stfd f24, 0xb0(r1)
	stfd f25, 0xb8(r1)
	stw k, 0x50(r1)
	stw rv, 0x54(r1)
	lis r31, value0_3@ha
	lfs f6, value0_3@l(r31)
	lis r31, value0_6@ha
	lfs f9, value0_6@l(r31)
	lis r31, i2fMagic@ha
	lfd f5, i2fMagic@l(r31)
	lfs f2, AXFX_REVHI_WORK_DPL2.allPassCoeff(rv)
	lfs f15, AXFX_REVHI_WORK_DPL2.damping(rv)
	lfs f8, AXFX_REVHI_WORK_DPL2.level(rv)
	fmuls f3, f8, f9
	fsubs f4, f9, f3
	slwi r30, k, 1
	add r30, r30, k
	mulli r31, r30, 0x14
	addi r29, rv, AXFX_REVHI_WORK_DPL2.C
	add r29, r29, r31
	addi r27, rv, AXFX_REVHI_WORK_DPL2.AP
	add r27, r27, r31
	slwi r31, r30, 2
	add r31, r31, rv
	lfs f22, AXFX_REVHI_WORK_DPL2.combCoef[0](r31)
	lfs f23, AXFX_REVHI_WORK_DPL2.combCoef[1](r31)
	lfs f24, AXFX_REVHI_WORK_DPL2.combCoef[2](r31)
	slwi r31, k, 2
	add r31, r31, rv
	lfs f25, AXFX_REVHI_WORK_DPL2.lpLastout[0](r31)
	lwz r31, AXFX_REVHI_WORK_DPL2.preDelayTime(rv)
	lis r30, 0x4330
	stw r30, 0x58(r1)
	subi r22, r31, 1
	slwi r22, r22, 2
	slwi r28, k, 2
	add r28, r28, rv
	cmpwi cr7, r31, 0
	lwz r21, AXFX_REVHI_DELAYLINE.inPoint    + 0x00(r29) // C[0]
	lwz r20, AXFX_REVHI_DELAYLINE.outPoint   + 0x00(r29) // C[0]
	lwz r19, AXFX_REVHI_DELAYLINE.inPoint    + 0x14(r29) // C[1]
	lwz r18, AXFX_REVHI_DELAYLINE.outPoint   + 0x14(r29) // C[1]
	lwz r17, AXFX_REVHI_DELAYLINE.inPoint    + 0x28(r29) // C[2]
	lwz r16, AXFX_REVHI_DELAYLINE.outPoint   + 0x28(r29) // C[2]
	lfs f16, AXFX_REVHI_DELAYLINE.lastOutput + 0x00(r29) // C[0]
	lfs f17, AXFX_REVHI_DELAYLINE.lastOutput + 0x14(r29) // C[1]
	lfs f18, AXFX_REVHI_DELAYLINE.lastOutput + 0x28(r29) // C[2]
	lwz r25, AXFX_REVHI_DELAYLINE.length     + 0x00(r29) // C[0]
	lwz r24, AXFX_REVHI_DELAYLINE.length     + 0x14(r29) // C[1]
	lwz r23, AXFX_REVHI_DELAYLINE.length     + 0x28(r29) // C[2]
	lwz r4,  AXFX_REVHI_DELAYLINE.inputs     + 0x00(r29) // C[0]
	lwz r5,  AXFX_REVHI_DELAYLINE.inputs     + 0x14(r29) // C[1]
	lwz r6,  AXFX_REVHI_DELAYLINE.inputs     + 0x28(r29) // C[2]
	lwz r12, AXFX_REVHI_DELAYLINE.inPoint    + 0x00(r27) // AP[0]
	lwz r11, AXFX_REVHI_DELAYLINE.outPoint   + 0x00(r27) // AP[0]
	lwz r10, AXFX_REVHI_DELAYLINE.inPoint    + 0x14(r27) // AP[1]
	lwz r9,  AXFX_REVHI_DELAYLINE.outPoint   + 0x14(r27) // AP[1]
	lwz r8,  AXFX_REVHI_DELAYLINE.inPoint    + 0x28(r27) // AP[2]
	lwz r7,  AXFX_REVHI_DELAYLINE.outPoint   + 0x28(r27) // AP[2]
	lfs f19, AXFX_REVHI_DELAYLINE.lastOutput + 0x00(r27) // AP[0]
	lfs f20, AXFX_REVHI_DELAYLINE.lastOutput + 0x14(r27) // AP[1]
	lfs f21, AXFX_REVHI_DELAYLINE.lastOutput + 0x28(r27) // AP[2]
	lwz r15, AXFX_REVHI_DELAYLINE.length     + 0x00(r27) // AP[0]
	lwz r14, AXFX_REVHI_DELAYLINE.length     + 0x14(r27) // AP[1]
    //? missing load for length of AP[3]? Maybe intentional?
	lwz r30, 0(r3)
	xoris r30, r30, 0x8000
	stw r30, 0x5c(r1)
	lfd f12, 0x58(r1)
	fsubs f12, f12, f5
	li r31, 159
	mtctr r31
L_00000964:
	fmr f13, f12
	beq cr7, L_00000994
	lwz r30, AXFX_REVHI_WORK_DPL2.preDelayLine(r28)
	lwz r29, AXFX_REVHI_WORK_DPL2.preDelayPtr(r28)
	add r31, r22, r30
	addi r29, r29, 4
	lfs f13, -4(r29)
	cmpw r29, r31
	stfs f12, -4(r29)
	bne+ L_00000990
	mr r29, r30
L_00000990:
	stw r29, AXFX_REVHI_WORK_DPL2.preDelayPtr(r30)
L_00000994:
	fmadds f8, f22, f16, f13
	lwzu r29, 4(r3)
	fmadds f9, f23, f17, f13
	stfsx f8, rv, r21
	addi r21, r21, 4
	stfsx f9, k, r19
	lfsx f14, rv, r20
	addi r20, r20, 4
	lfsx f17, k, r18
	cmpw r21, r25
	cmpw cr1, r20, r25
	addi r19, r19, 4
	addi r18, r18, 4
	fmr f16, f14
	cmpw cr5, r19, r24
	fadds f14, f14, f17
	cmpw cr6, r18, r24
	bne+ L_000009E0
	li r21, 0
L_000009E0:
	fmadds f8, f24, f18, f13
	bne+ cr1, L_000009EC
	li r20, 0
L_000009EC:
	stfsx f8, r6, r17
	addi r17, r17, 4
	bne+ cr5, L_000009FC
	li r19, 0
L_000009FC:
	lfsx f18, r6, r16
	addi r16, r16, 4
	cmpw r17, r23
	bne+ cr6, L_00000A10
	li r18, 0
L_00000A10:
	fadds f14, f14, f18
	cmpw cr1, r16, r23
	lwz r26, AXFX_REVHI_DELAYLINE.inputs(r27)
	fmadds f9, f2, f19, f14
	bne+ L_00000A28
	li r17, 0
L_00000A28:
	bne+ cr1, L_00000A30
	li r16, 0
L_00000A30:
	xoris r29, r29, 0x8000
	stfsx f9, r26, r12
	fnmsubs f14, f2, f9, f19
	addi r12, r12, 4
	lfsx f19, r26, r11
	cmpw cr5, r12, r15
	addi r11, r11, 4
	lwz r26, AXFX_REVHI_DELAYLINE.inputs + 0x14(r27)
	cmpw cr6, r11, r15
	fmadds f8, f2, f20, f14
	bne+ cr5, L_00000A60
	li r12, 0x0
L_00000A60:
	stw r29, 0x5c(r1)
	stfsx f8, r26, r10
	fnmsubs f14, f2, f8, f20
	addi r10, r10, 4
	bne+ cr6, L_00000A78
	li r11, 0
L_00000A78:
	lfsx f20, r26, r9
	cmpw r10, r14
	fmuls f14, f14, f6
	addi r9, r9, 4
	cmpw cr1, r9, r14
	lfd f10, 0x58(r1)
	fmadds f14, f15, f25, f14
	bne+ L_00000A9C
	li r10, 0
L_00000A9C:
	lwz r26, AXFX_REVHI_DELAYLINE.inputs + 0x28(r27)
	fmadds f9, f2, f21, f14
	fmr f25, f14
	bne+ cr1, L_00000AB0
	li r9, 0
L_00000AB0:
	stfsx f9, r26, r8
	fnmsubs f14, f2, f9, f21
	lwz r31, AXFX_REVHI_DELAYLINE.length + 0x28(r27)
	fmuls f8, f4, f12
	lfsx f21, r26, r7
	addi r8, r8, 4
	addi r7, r7, 4
	fmadds f14, f3, f14, f8
	cmpw cr5, r8, r31
	cmpw cr6, r7, r31
	fctiwz f14, f14
	bne+ cr5, L_00000AE4
	li r8, 0
L_00000AE4:
	bne+ cr6, L_00000AEC
	li r7, 0
L_00000AEC:
	li r31, -4
	fsubs f12, f10, f5
	stfiwx f14, sptr, r31
	bdnz L_00000964
	fmr f13, f12
	beq cr7, L_00000B2C
	lwz r30, AXFX_REVHI_WORK_DPL2.preDelayLine(r28)
	lwz r29, AXFX_REVHI_WORK_DPL2.preDelayPtr(r28)
	add r31, r22, r30
	addi r29, r29, 4
	lfs f13, -4(r29)
	cmpw r29, r31
	stfs f12, -4(r29)
	bne+ L_00000B28
	mr r29, r30
L_00000B28:
	stw r29, AXFX_REVHI_WORK_DPL2.preDelayPtr(r30)
L_00000B2C:
	fmadds f8, f22, f16, f13
	fmadds f9, f23, f17, f13
	stfsx f8, rv, r21
	addi r21, r21, 4
	stfsx f9, k, r19
	lfsx f14, rv, r20
	addi r20, r20, 4
	lfsx f17, k, r18
	cmpw r21, r25
	cmpw cr1, r20, r25
	addi r19, r19, 4
	addi r18, r18, 4
	fmr f16, f14
	cmpw cr5, r19, r24
	fadds f14, f14, f17
	cmpw cr6, r18, r24
	bne+ L_00000B74
	li r21, 0
L_00000B74:
	fmadds f8, f24, f18, f13
	bne+ cr1, L_00000B80
	li r20, 0
L_00000B80:
	stfsx f8, r6, r17
	addi r17, r17, 4
	bne+ cr5, L_00000B90
	li r19, 0
L_00000B90:
	lfsx f18, r6, r16
	addi r16, r16, 4
	cmpw r17, r23
	bne+ cr6, L_00000BA4
	li r18, 0
L_00000BA4:
	fadds f14, f14, f18
	cmpw cr1, r16, r23
	lwz r26, AXFX_REVHI_DELAYLINE.inputs(r27)
	fmadds f9, f2, f19, f14
	bne+ L_00000BBC
	li r17, 0
L_00000BBC:
	bne+ cr1, L_00000BC4
	li r16, 0
L_00000BC4:
	stfsx f9, r26, r12
	fnmsubs f14, f2, f9, f19
	addi r12, r12, 4
	lfsx f19, r26, r11
	cmpw cr5, r12, r15
	addi r11, r11, 4
	lwz r26, AXFX_REVHI_DELAYLINE.inputs + 0x14(r27)
	cmpw cr6, r11, r15
	fmadds f8, f2, f20, f14
	bne+ cr5, L_00000BF0
	li r12, 0
L_00000BF0:
	stfsx f8, r26, r10
	fnmsubs f14, f2, f8, f20
	addi r10, r10, 4
	bne+ cr6, L_00000C04
	li r11, 0
L_00000C04:
	lfsx f20, r26, r9
	cmpw r10, r14
	fmuls f14, f14, f6
	addi r9, r9, 4
	cmpw cr1, r9, r14
	fmadds f14, f15, f25, f14
	bne+ L_00000C24
	li r10, 0
L_00000C24:
	lwz r26, AXFX_REVHI_DELAYLINE.inputs + 0x28(r27)
	lwz k, 0x50(r1)
	lwz rv, 0x54(r1)
	fmadds f9, f2, f21, f14
	fmr f25, f14
	bne+ cr1, L_00000C40
	li r9, 0
L_00000C40:
	stfsx f9, r26, r8
	fnmsubs f14, f2, f9, f21
	lwz r29, AXFX_REVHI_DELAYLINE.length + 0x28(r27)
	fmuls f8, f4, f12
	lfsx f21, r26, r7
	addi r8, r8, 4
	addi r7, r7, 4
	fmadds f14, f3, f14, f8
	cmpw cr5, r8, r29
	cmpw cr6, r7, r29
	fctiwz f14, f14
	bne+ cr5, L_00000C74
	li r8, 0
L_00000C74:
	bne+ cr6, L_00000C7C
	li r7, 0
L_00000C7C:
	slwi r30, k, 1
	add r30, r30, k
	mulli r31, r30, 0x14 // sizeof AXFX_REVHI_DELAYLINE
	stfiwx f14, r0, sptr
	addi r29, rv, AXFX_REVHI_WORK_DPL2.C
	add r29, r29, r31
	stw r21,  AXFX_REVHI_DELAYLINE.inPoint    + 0x00(r29) // C[0]
	stw r20,  AXFX_REVHI_DELAYLINE.outPoint   + 0x00(r29) // C[0]
	stw r19,  AXFX_REVHI_DELAYLINE.inPoint    + 0x14(r29) // C[1]
	stw r18,  AXFX_REVHI_DELAYLINE.outPoint   + 0x14(r29) // C[1]
	stw r17,  AXFX_REVHI_DELAYLINE.inPoint    + 0x28(r29) // C[2]
	stw r16,  AXFX_REVHI_DELAYLINE.outPoint   + 0x28(r29) // C[2]
	stfs f16, AXFX_REVHI_DELAYLINE.lastOutput + 0x00(r29) // C[0]
	stfs f17, AXFX_REVHI_DELAYLINE.lastOutput + 0x14(r29) // C[1]
	stfs f18, AXFX_REVHI_DELAYLINE.lastOutput + 0x28(r29) // C[2]
	stw r12,  AXFX_REVHI_DELAYLINE.inPoint    + 0x00(r27) // AP[0]
	stw r11,  AXFX_REVHI_DELAYLINE.outPoint   + 0x00(r27) // AP[0]
	stw r10,  AXFX_REVHI_DELAYLINE.inPoint    + 0x14(r27) // AP[1]
	stw r9,   AXFX_REVHI_DELAYLINE.outPoint   + 0x14(r27) // AP[1]
	stw r8,   AXFX_REVHI_DELAYLINE.inPoint    + 0x28(r27) // AP[2]
	stw r7,   AXFX_REVHI_DELAYLINE.outPoint   + 0x28(r27) // AP[2]
	stfs f19, AXFX_REVHI_DELAYLINE.lastOutput + 0x00(r27) // AP[0]
	stfs f20, AXFX_REVHI_DELAYLINE.lastOutput + 0x14(r27) // AP[1]
	stfs f21, AXFX_REVHI_DELAYLINE.lastOutput + 0x28(r27) // AP[2]
	slwi r31, k, 2
	add r31, r31, rv
	stfs f25, AXFX_REVHI_WORK_DPL2.lpLastout(r31)
	lfd f14, 0x60(r1)
	lfd f15, 0x68(r1)
	lfd f16, 0x70(r1)
	lfd f17, 0x78(r1)
	lfd f18, 0x80(r1)
	lfd f19, 0x88(r1)
	lfd f20, 0x90(r1)
	lfd f21, 0x98(r1)
	lfd f22, 0xa0(r1)
	lfd f23, 0xa8(r1)
	lfd f24, 0xb0(r1)
	lfd f25, 0xb8(r1)
	lmw r14, 0x8(r1)
	addi r1, r1, 0xc0
	blr
}

static void ReverbHICallbackDpl2(s32* l, s32* r, s32* ls, s32* rs, AXFX_REVHI_WORK_DPL2* rv) {
    HandleReverbDpl2(l, rv, 0);
    HandleReverbDpl2(r, rv, 1);
    HandleReverbDpl2(ls, rv, 2);
    HandleReverbDpl2(rs, rv, 3);
}

static void ReverbHIDpl2Free(AXFX_REVHI_WORK_DPL2* rv) {
    u8 i;

    for (i = 0; i < 12; i++) {
        if (rv->AP[i].inputs != 0) {
            DLdeleteDpl2(&rv->AP[i]);
            rv->AP[i].inputs = 0;
        }
    }

    for (i = 0; i < 12; i++) {
        if (rv->C[i].inputs != 0) {
            DLdeleteDpl2(&rv->C[i]);
            rv->C[i].inputs = 0;
        }
    }

    if (rv->preDelayTime) {
        for (i = 0; i < 4; i++) {
            if (rv->preDelayLine[i] != 0) {
                __AXFXFree(rv->preDelayLine[i]);
                rv->preDelayLine[i] = 0;
            }
        }
    }
}

int AXFXReverbHiInitDpl2(AXFX_REVERBHI_DPL2* reverb) {
    int ret;
    BOOL old;

    old = OSDisableInterrupts();
    reverb->tempDisableFX = 0;
    ret = ReverbHICreateDpl2(&reverb->rv, reverb->coloration, reverb->time, reverb->mix, reverb->damping, reverb->preDelay);
    OSRestoreInterrupts(old);
    return ret;
}

int AXFXReverbHiShutdownDpl2(AXFX_REVERBHI_DPL2* reverb) {
    BOOL old;

    old = OSDisableInterrupts();
    ReverbHIDpl2Free(&reverb->rv);
    OSRestoreInterrupts(old);
    return 1;
}

int AXFXReverbHiSettingsDpl2(AXFX_REVERBHI_DPL2* rev) {
    int ret;
    BOOL old;

    old = OSDisableInterrupts();
    rev->tempDisableFX = 1;
    ret = ReverbHIModifyDpl2(&rev->rv, rev->coloration, rev->time, rev->mix, rev->damping, rev->preDelay);
    rev->tempDisableFX = 0;
    OSRestoreInterrupts(old);
    return ret;
}

void AXFXReverbHiCallbackDpl2(AXFX_BUFFERUPDATE_DPL2* bufferUpdate, AXFX_REVERBHI_DPL2* reverb) {
    ASSERTMSGLINE(1399, AXGetMode() == 2, "AX mode isn't AX_MODE_DPL2. AX mode must be AX_MODE_DPL2 for using AXFXReverbHiCallbackDpl2");

    if (reverb->tempDisableFX == 0) {
        HandleReverbDpl2(bufferUpdate->L, &reverb->rv, 0);
        HandleReverbDpl2(bufferUpdate->R, &reverb->rv, 1);
        HandleReverbDpl2(bufferUpdate->Ls, &reverb->rv, 2);
        HandleReverbDpl2(bufferUpdate->Rs, &reverb->rv, 3);
    }
}
