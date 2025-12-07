#include <dolphin.h>

#ifdef __cplusplus
extern "C" {
#endif

/* macros for GPR/FPR resting and saving */
#define SAVE_FPR(reg)    _savefpr_##reg
#define RESTORE_FPR(reg) _restfpr_##reg
#define SAVE_GPR(reg)    _savegpr_##reg
#define RESTORE_GPR(reg) _restgpr_##reg

#define ENTRY_SAVE_FPR(reg)    entry SAVE_FPR(reg)
#define ENTRY_RESTORE_FPR(reg) entry RESTORE_FPR(reg)
#define ENTRY_SAVE_GPR(reg)    entry SAVE_GPR(reg)
#define ENTRY_RESTORE_GPR(reg) entry RESTORE_GPR(reg)

#define save_restore_reg r11

asm void __div2u(void);
asm void __div2i(void);
asm void __mod2u(void);
asm void __mod2i(void);
asm void __shl2i(void);
asm void __shr2u(void);
asm void __shr2i(void);
asm void __cvt_dbl_usll(void);

void SAVE_FPR(14)(void);
void SAVE_FPR(15)(void);
void SAVE_FPR(16)(void);
void SAVE_FPR(17)(void);
void SAVE_FPR(18)(void);
void SAVE_FPR(19)(void);
void SAVE_FPR(20)(void);
void SAVE_FPR(21)(void);
void SAVE_FPR(22)(void);
void SAVE_FPR(23)(void);
void SAVE_FPR(24)(void);
void SAVE_FPR(25)(void);
void SAVE_FPR(26)(void);
void SAVE_FPR(27)(void);
void SAVE_FPR(28)(void);
void SAVE_FPR(29)(void);
void SAVE_FPR(30)(void);
void SAVE_FPR(31)(void);

void RESTORE_FPR(14)(void);
void RESTORE_FPR(15)(void);
void RESTORE_FPR(16)(void);
void RESTORE_FPR(17)(void);
void RESTORE_FPR(18)(void);
void RESTORE_FPR(19)(void);
void RESTORE_FPR(20)(void);
void RESTORE_FPR(21)(void);
void RESTORE_FPR(22)(void);
void RESTORE_FPR(23)(void);
void RESTORE_FPR(24)(void);
void RESTORE_FPR(25)(void);
void RESTORE_FPR(26)(void);
void RESTORE_FPR(27)(void);
void RESTORE_FPR(28)(void);
void RESTORE_FPR(29)(void);
void RESTORE_FPR(30)(void);
void RESTORE_FPR(31)(void);

void SAVE_GPR(14)(void);
void SAVE_GPR(15)(void);
void SAVE_GPR(16)(void);
void SAVE_GPR(17)(void);
void SAVE_GPR(18)(void);
void SAVE_GPR(19)(void);
void SAVE_GPR(20)(void);
void SAVE_GPR(21)(void);
void SAVE_GPR(22)(void);
void SAVE_GPR(23)(void);
void SAVE_GPR(24)(void);
void SAVE_GPR(25)(void);
void SAVE_GPR(26)(void);
void SAVE_GPR(27)(void);
void SAVE_GPR(28)(void);
void SAVE_GPR(29)(void);
void SAVE_GPR(30)(void);
void SAVE_GPR(31)(void);

void RESTORE_GPR(14)(void);
void RESTORE_GPR(15)(void);
void RESTORE_GPR(16)(void);
void RESTORE_GPR(17)(void);
void RESTORE_GPR(18)(void);
void RESTORE_GPR(19)(void);
void RESTORE_GPR(20)(void);
void RESTORE_GPR(21)(void);
void RESTORE_GPR(22)(void);
void RESTORE_GPR(23)(void);
void RESTORE_GPR(24)(void);
void RESTORE_GPR(25)(void);
void RESTORE_GPR(26)(void);
void RESTORE_GPR(27)(void);
void RESTORE_GPR(28)(void);
void RESTORE_GPR(29)(void);
void RESTORE_GPR(30)(void);
void RESTORE_GPR(31)(void);

static const u32 __constants[] = {
	0x00000000, 0x00000000, 0x41F00000, 0x00000000, 0x41E00000, 0x00000000,
};

asm u32 __cvt_fp2unsigned(__REGISTER f64 d)
{
#ifdef __MWERKS__ // clang-format off
		nofralloc
		stwu    r1,-16(r1)
		lis     r4, __constants@h
		ori     r4, r4, __constants@l
		li      r3,0
		lfd     fp0,0(r4)
		lfd     fp3,8(r4)
		lfd     fp4,16(r4)
		fcmpu   cr0,fp1,fp0
		fcmpu   cr6,fp1,fp3
		blt     cr0, @exit
		addi    r3,r3,-1
		bge     cr6,@exit
		fcmpu   cr7,fp1,fp4
		fmr     fp2,fp1
		blt     cr7,@1
		fsub    fp2,fp1,fp4
@1		fctiwz  fp2,fp2
		stfd    fp2,8(r1)
		lwz     r3,12(r1)
		blt     cr7,@exit
		addis   r3,r3,-0x8000
@exit:
		addi    r1,r1,16
		blr
#endif // clang-format on
}

asm static void __save_fpr(void) {
#ifdef __MWERKS__ // clang-format off
	nofralloc
    ENTRY_SAVE_FPR(14)
		stfd    fp14,-144(save_restore_reg)
    ENTRY_SAVE_FPR(15)
		stfd    fp15,-136(save_restore_reg)
    ENTRY_SAVE_FPR(16)
		stfd    fp16,-128(save_restore_reg)
    ENTRY_SAVE_FPR(17)
		stfd    fp17,-120(save_restore_reg)
    ENTRY_SAVE_FPR(18)
		stfd    fp18,-112(save_restore_reg)
    ENTRY_SAVE_FPR(19)
		stfd    fp19,-104(save_restore_reg)
    ENTRY_SAVE_FPR(20)
		stfd    fp20,-96(save_restore_reg)
    ENTRY_SAVE_FPR(21)
		stfd    fp21,-88(save_restore_reg)
    ENTRY_SAVE_FPR(22)
		stfd    fp22,-80(save_restore_reg)
    ENTRY_SAVE_FPR(23)
		stfd    fp23,-72(save_restore_reg)
    ENTRY_SAVE_FPR(24)
		stfd    fp24,-64(save_restore_reg)
	ENTRY_SAVE_FPR(25)
		stfd    fp25,-56(save_restore_reg)
	ENTRY_SAVE_FPR(26)
		stfd    fp26,-48(save_restore_reg)
    ENTRY_SAVE_FPR(27)
		stfd    fp27,-40(save_restore_reg)
	ENTRY_SAVE_FPR(28)
		stfd    fp28,-32(save_restore_reg)
	ENTRY_SAVE_FPR(29)
		stfd    fp29,-24(save_restore_reg)
    ENTRY_SAVE_FPR(30)
		stfd    fp30,-16(save_restore_reg)
    ENTRY_SAVE_FPR(31)
		stfd    fp31,-8(save_restore_reg)
		blr
#endif // clang-format on
}

asm static void __restore_fpr(void) {
#ifdef __MWERKS__ // clang-format off
	nofralloc
    ENTRY_RESTORE_FPR(14)
		lfd     fp14,-144(save_restore_reg)
    ENTRY_RESTORE_FPR(15)
		lfd     fp15,-136(save_restore_reg)
    ENTRY_RESTORE_FPR(16)
		lfd     fp16,-128(save_restore_reg)
    ENTRY_RESTORE_FPR(17)
		lfd     fp17,-120(save_restore_reg)
    ENTRY_RESTORE_FPR(18)
		lfd     fp18,-112(save_restore_reg)
    ENTRY_RESTORE_FPR(19)
		lfd     fp19,-104(save_restore_reg)
    ENTRY_RESTORE_FPR(20)
		lfd     fp20,-96(save_restore_reg)
    ENTRY_RESTORE_FPR(21)
		lfd     fp21,-88(save_restore_reg)
    ENTRY_RESTORE_FPR(22)
		lfd     fp22,-80(save_restore_reg)
    ENTRY_RESTORE_FPR(23)
		lfd     fp23,-72(save_restore_reg)
    ENTRY_RESTORE_FPR(24)
		lfd     fp24,-64(save_restore_reg)
	ENTRY_RESTORE_FPR(25)
		lfd     fp25,-56(save_restore_reg)
	ENTRY_RESTORE_FPR(26)
		lfd     fp26,-48(save_restore_reg)
    ENTRY_RESTORE_FPR(27)
		lfd     fp27,-40(save_restore_reg)
	ENTRY_RESTORE_FPR(28)
		lfd     fp28,-32(save_restore_reg)
	ENTRY_RESTORE_FPR(29)
		lfd     fp29,-24(save_restore_reg)
    ENTRY_RESTORE_FPR(30)
		lfd     fp30,-16(save_restore_reg)
    ENTRY_RESTORE_FPR(31)
		lfd     fp31,-8(save_restore_reg)
		blr
#endif // clang-format on
}

asm static void __save_gpr(void) {
#ifdef __MWERKS__ // clang-format off
	nofralloc
	ENTRY_SAVE_GPR(14)
		stw     r14,-72(save_restore_reg)
	ENTRY_SAVE_GPR(15)
		stw     r15,-68(save_restore_reg)
	ENTRY_SAVE_GPR(16)
		stw     r16,-64(save_restore_reg)
	ENTRY_SAVE_GPR(17)
		stw     r17,-60(save_restore_reg)
	ENTRY_SAVE_GPR(18)
		stw     r18,-56(save_restore_reg)
	ENTRY_SAVE_GPR(19)
		stw     r19,-52(save_restore_reg)
	ENTRY_SAVE_GPR(20)
		stw     r20,-48(save_restore_reg)
	ENTRY_SAVE_GPR(21)
		stw     r21,-44(save_restore_reg)
	ENTRY_SAVE_GPR(22)
		stw     r22,-40(save_restore_reg)
	ENTRY_SAVE_GPR(23)
		stw     r23,-36(save_restore_reg)
	ENTRY_SAVE_GPR(24)
		stw     r24,-32(save_restore_reg)
	ENTRY_SAVE_GPR(25)
		stw     r25,-28(save_restore_reg)
	ENTRY_SAVE_GPR(26)
		stw     r26,-24(save_restore_reg)
	ENTRY_SAVE_GPR(27)
		stw     r27,-20(save_restore_reg)
	ENTRY_SAVE_GPR(28)
		stw     r28,-16(save_restore_reg)
	ENTRY_SAVE_GPR(29)
		stw     r29,-12(save_restore_reg)
    ENTRY_SAVE_GPR(30)
		stw     r30,-8(save_restore_reg)
    ENTRY_SAVE_GPR(31)
		stw     r31,-4(save_restore_reg)
		blr
#endif // clang-format on
}

asm static void __restore_gpr(void) {
#ifdef __MWERKS__ // clang-format off
	nofralloc
	ENTRY_RESTORE_GPR(14)
		lwz     r14,-72(save_restore_reg)
	ENTRY_RESTORE_GPR(15)
		lwz     r15,-68(save_restore_reg)
	ENTRY_RESTORE_GPR(16)
		lwz     r16,-64(save_restore_reg)
	ENTRY_RESTORE_GPR(17)
		lwz     r17,-60(save_restore_reg)
	ENTRY_RESTORE_GPR(18)
		lwz     r18,-56(save_restore_reg)
	ENTRY_RESTORE_GPR(19)
		lwz     r19,-52(save_restore_reg)
	ENTRY_RESTORE_GPR(20)
		lwz     r20,-48(save_restore_reg)
	ENTRY_RESTORE_GPR(21)
		lwz     r21,-44(save_restore_reg)
	ENTRY_RESTORE_GPR(22)
		lwz     r22,-40(save_restore_reg)
	ENTRY_RESTORE_GPR(23)
		lwz     r23,-36(save_restore_reg)
	ENTRY_RESTORE_GPR(24)
		lwz     r24,-32(save_restore_reg)
	ENTRY_RESTORE_GPR(25)
		lwz     r25,-28(save_restore_reg)
	ENTRY_RESTORE_GPR(26)
		lwz     r26,-24(save_restore_reg)
	ENTRY_RESTORE_GPR(27)
		lwz     r27,-20(save_restore_reg)
	ENTRY_RESTORE_GPR(28)
		lwz     r28,-16(save_restore_reg)
	ENTRY_RESTORE_GPR(29)
		lwz     r29,-12(save_restore_reg)
    ENTRY_RESTORE_GPR(30)
		lwz     r30,-8(save_restore_reg)
    ENTRY_RESTORE_GPR(31)
		lwz     r31,-4(save_restore_reg)
		blr
#endif // clang-format on
}

asm void __div2u(void) {
#ifdef __MWERKS__ // clang-format off
	nofralloc
	cmpwi   cr0,r3,0
	cntlzw  r0,r3
	cntlzw  r9,r4
	bne     cr0,lab1
	addi    r0,r9,32
lab1:
	cmpwi   cr0,r5,0
	cntlzw  r9,r5
	cntlzw  r10,r6
	bne     cr0,lab2
	addi    r9,r10,32
lab2:
	cmpw    cr0,r0,r9
	subfic  r10,r0,64
	bgt     cr0,lab9
	addi    r9,r9,1
	subfic  r9,r9,64
	add     r0,r0,r9
	subf    r9,r9,r10
	mtctr   r9
	cmpwi   cr0,r9,32
	addi    r7,r9,-32
	blt     cr0,lab3
	srw     r8,r3,r7
	li      r7,0
	b       lab4
lab3:
	srw     r8,r4,r9
	subfic  r7,r9,32
	slw     r7,r3,r7
	or      r8,r8,r7
	srw     r7,r3,r9
lab4:
	cmpwi   cr0,r0,32
	addic   r9,r0,-32
	blt     cr0,lab5
	slw     r3,r4,r9
	li      r4,0
	b       lab6
lab5:
	slw     r3,r3,r0
	subfic  r9,r0,32
	srw     r9,r4,r9
	or      r3,r3,r9
	slw     r4,r4,r0
lab6:
	li      r10,-1
	addic   r7,r7,0
lab7:
	adde    r4,r4,r4
	adde    r3,r3,r3
	adde    r8,r8,r8
	adde    r7,r7,r7
	subfc   r0,r6,r8
	subfe.  r9,r5,r7
	blt     cr0,lab8
	mr      r8,r0
	mr      r7,r9
	addic   r0,r10,1
lab8:
	bdnz    lab7
	adde    r4,r4,r4
	adde    r3,r3,r3
	blr
lab9:
	li      r4,0
	li      r3,0
	blr
#endif // clang-format on
}

asm void __div2i(void) {
#ifdef __MWERKS__ // clang-format off
	nofralloc
	stwu    r1,-16(r1)
	rlwinm. r9,r3,0,0,0
	beq     cr0,positive1
	subfic  r4,r4,0
	subfze  r3,r3
positive1:
	stw     r9,8(r1)
	rlwinm. r10,r5,0,0,0
	beq     cr0,positive2
	subfic  r6,r6,0
	subfze  r5,r5
positive2:
	stw     r10,12(r1)
	cmpwi   cr0,r3,0
	cntlzw  r0,r3
	cntlzw  r9,r4
	bne     cr0,lab1
	addi    r0,r9,32
lab1:
	cmpwi   cr0,r5,0
	cntlzw  r9,r5
	cntlzw  r10,r6
	bne     cr0,lab2
	addi    r9,r10,32
lab2:
	cmpw    cr0,r0,r9
	subfic  r10,r0,64
	bgt     cr0,lab9
	addi    r9,r9,1
	subfic  r9,r9,64
	add     r0,r0,r9
	subf    r9,r9,r10
	mtctr   r9
	cmpwi   cr0,r9,32
	addi    r7,r9,-32
	blt     cr0,lab3
	srw     r8,r3,r7
	li      r7,0
	b       lab4
lab3:
	srw     r8,r4,r9
	subfic  r7,r9,32
	slw     r7,r3,r7
	or      r8,r8,r7
	srw     r7,r3,r9
lab4:
	cmpwi   cr0,r0,32
	addic   r9,r0,-32
	blt     cr0,lab5
	slw     r3,r4,r9
	li      r4,0
	b       lab6
lab5:
	slw     r3,r3,r0
	subfic  r9,r0,32
	srw     r9,r4,r9
	or      r3,r3,r9
	slw     r4,r4,r0
lab6:
	li      r10,-1
	addic   r7,r7,0
lab7:
	adde    r4,r4,r4
	adde    r3,r3,r3
	adde    r8,r8,r8
	adde    r7,r7,r7
	subfc   r0,r6,r8
	subfe.  r9,r5,r7
	blt     cr0,lab8
	mr      r8,r0
	mr      r7,r9
	addic   r0,r10,1
lab8:
	bdnz    lab7
	adde    r4,r4,r4
	adde    r3,r3,r3
	lwz     r9,8(r1)
	lwz     r10,12(r1)
	xor.    r7,r9,r10
	beq     cr0,no_adjust
	cmpwi   cr0,r9,0
	subfic  r4,r4,0
	subfze  r3,r3

no_adjust:
	b       func_end

lab9:
	li      r4,0
	li      r3,0
func_end:
	addi    r1,r1,16
	blr
#endif // clang-format on
}

asm void __mod2u(void) {
#ifdef __MWERKS__ // clang-format off
	nofralloc
	cmpwi   cr0,r3,0
	cntlzw  r0,r3
	cntlzw  r9,r4
	bne     cr0,lab1
	addi    r0,r9,32
lab1:
	cmpwi   cr0,r5,0
	cntlzw  r9,r5
	cntlzw  r10,r6
	bne     cr0,lab2
	addi    r9,r10,32
lab2:
	cmpw    cr0,r0,r9
	subfic  r10,r0,64
	bgt     cr0,lab9
	addi    r9,r9,1
	subfic  r9,r9,64
	add     r0,r0,r9
	subf    r9,r9,r10
	mtctr   r9
	cmpwi   cr0,r9,32
	addi    r7,r9,-32
	blt     cr0,lab3
	srw     r8,r3,r7
	li      r7,0
	b       lab4
lab3:
	srw     r8,r4,r9
	subfic  r7,r9,32
	slw     r7,r3,r7
	or      r8,r8,r7
	srw     r7,r3,r9
lab4:
	cmpwi   cr0,r0,32
	addic   r9,r0,-32
	blt     cr0,lab5
	slw     r3,r4,r9
	li      r4,0
	b       lab6
lab5:
	slw     r3,r3,r0
	subfic  r9,r0,32
	srw     r9,r4,r9
	or      r3,r3,r9
	slw     r4,r4,r0
lab6:
	li      r10,-1
	addic   r7,r7,0
lab7:
	adde    r4,r4,r4
	adde    r3,r3,r3
	adde    r8,r8,r8
	adde    r7,r7,r7
	subfc   r0,r6,r8
	subfe.  r9,r5,r7
	blt     cr0,lab8
	mr      r8,r0
	mr      r7,r9
	addic   r0,r10,1
lab8:
	bdnz    lab7
	mr      r4,r8
	mr      r3,r7
	blr
lab9:
	blr
#endif // clang-format on
}

asm void __mod2i(void) {
#ifdef __MWERKS__ // clang-format off
	nofralloc

	cmpwi   cr7,r3,0
	bge     cr7,positive1
	subfic  r4,r4,0
	subfze  r3,r3
positive1:
	cmpwi   cr0,r5,0
	bge     cr0,positive2
	subfic   r6,r6,0
	subfze   r5,r5
positive2:
	cmpwi   cr0,r3,0
	cntlzw  r0,r3
	cntlzw  r9,r4
	bne     cr0,lab1
	addi    r0,r9,32
lab1:
	cmpwi   cr0,r5,0
	cntlzw  r9,r5
	cntlzw  r10,r6
	bne     cr0,lab2
	addi    r9,r10,32
lab2:
	cmpw    cr0,r0,r9
	subfic  r10,r0,64
	bgt     cr0,lab9
	addi    r9,r9,1
	subfic  r9,r9,64
	add     r0,r0,r9
	subf    r9,r9,r10
	mtctr   r9
	cmpwi   cr0,r9,32
	addi    r7,r9,-32
	blt     cr0,lab3
	srw     r8,r3,r7
	li      r7,0
	b       lab4
lab3:
	srw     r8,r4,r9
	subfic  r7,r9,32
	slw     r7,r3,r7
	or      r8,r8,r7
	srw     r7,r3,r9
lab4:
	cmpwi   cr0,r0,32
	addic   r9,r0,-32
	blt     cr0,lab5
	slw     r3,r4,r9
	li      r4,0
	b       lab6
lab5:
	slw     r3,r3,r0
	subfic  r9,r0,32
	srw     r9,r4,r9
	or      r3,r3,r9
	slw     r4,r4,r0
lab6:
	li      r10,-1
	addic   r7,r7,0
lab7:
	adde    r4,r4,r4
	adde    r3,r3,r3
	adde    r8,r8,r8
	adde    r7,r7,r7
	subfc   r0,r6,r8
	subfe.  r9,r5,r7
	blt     cr0,lab8
	mr      r8,r0
	mr      r7,r9
	addic   r0,r10,1
lab8:
	bdnz    lab7
	mr      r4,r8
	mr      r3,r7
lab9:
	bge     cr7,no_adjust
	subfic  r4,r4,0
	subfze  r3,r3
no_adjust:
	blr
#endif // clang-format on
}

asm void __shl2i(void) {
#ifdef __MWERKS__ // clang-format off
	nofralloc
	subfic  r8,r5,32
	subic   r9,r5,32
	slw     r3,r3,r5
	srw     r10,r4,r8
	or      r3,r3,r10
	slw     r10,r4,r9
	or      r3,r3,r10
	slw     r4,r4,r5
	blr
#endif // clang-format on
}

asm void __shr2u(void) {
#ifdef __MWERKS__ // clang-format off
	nofralloc
	subfic  r8,r5,32
	subic   r9,r5,32
	srw     r4,r4,r5
	slw     r10,r3,r8
	or      r4,r4,r10
	srw     r10,r3,r9
	or      r4,r4,r10
	srw     r3,r3,r5
	blr
#endif // clang-format on
}

asm void __shr2i(void) {
#ifdef __MWERKS__ // clang-format off
	nofralloc
	subfic  r8, r5, 0x20
	addic.  r9, r5, -0x20
	srw     r4, r4, r5
	slw     r10, r3, r8
	or      r4, r4, r10
	sraw    r10, r3, r9
	ble     around
	or      r4, r4, r10
around:
	sraw    r3, r3, r5
	blr
#endif // clang-format on
}

asm void __cvt_sll_flt(void)
{
#ifdef __MWERKS__ // clang-format off
	nofralloc
    stwu r1, -0x10(r1)
    rlwinm. r5, r3, 0, 0, 0
    beq lbl_803626D0
    subfic r4, r4, 0
    subfze r3, r3
lbl_803626D0:
    or. r7, r3, r4
    li r6, 0
    beq lbl_80362758
    cntlzw r7, r3
    cntlzw r8, r4
    rlwinm r9, r7, 0x1a, 0, 4
    srawi r9, r9, 0x1f
    and r9, r9, r8
    add r7, r7, r9
    subfic r8, r7, 0x20
    addic r9, r7, -32
    slw r3, r3, r7
    srw r10, r4, r8
    or r3, r3, r10
    slw r10, r4, r9
    or r3, r3, r10
    slw r4, r4, r7
    subf r6, r7, r6
    clrlwi r7, r4, 0x15
    cmpwi r7, 0x400
    addi r6, r6, 0x43e
    blt lbl_80362740
    bgt lbl_80362734
    rlwinm. r7, r4, 0, 0x14, 0x14
    beq lbl_80362740
lbl_80362734:
    addic r4, r4, 0x800
    addze r3, r3
    addze r6, r6
lbl_80362740:
    rotlwi r4, r4, 0x15
    rlwimi r4, r3, 0x15, 0, 0xa
    rlwinm r3, r3, 0x15, 0xc, 0x1f
    slwi r6, r6, 0x14
    or r3, r6, r3
    or r3, r5, r3
lbl_80362758:
    stw r3, 8(r1)
    stw r4, 0xc(r1)
    lfd f1, 8(r1)
    frsp f1, f1
    addi r1, r1, 0x10
    blr
#endif // clang-format on
}

asm void __cvt_dbl_usll(void)
{
#ifdef __MWERKS__ // clang-format off
	nofralloc
	stwu    r1,-16(r1)
	stfd    f1,8(r1)
	lwz     r3,8(r1)
	lwz     r4,12(r1)
	rlwinm  r5,r3,12,21,31
	cmpli   cr0,0,r5,1023
	bge     cr0,not_fraction
	li      r3,0
	li      r4,0
	b       func_end
not_fraction:
	mr      r6,r3
	rlwinm  r3,r3,0,12,31
	oris    r3,r3,0x0010
	addi    r5,r5,-1075
	cmpwi   cr0,r5,0
	bge     cr0,left
	neg     r5,r5
	subfic  r8,r5,32
	subic   r9,r5,32
	srw     r4,r4,r5
	slw     r10,r3,r8
	or      r4,r4,r10
	srw     r10,r3,r9
	or      r4,r4,r10
	srw     r3,r3,r5
	b       around
left:
	cmpwi   cr0,r5,10
	ble+    no_overflow
	rlwinm. r6,r6,0,0,0
	beq     cr0,max_positive
	lis     r3,0x8000
	li      r4,0
	b       func_end
max_positive:
	lis     r3,0x7FFF
	ori     r3,r3,0xFFFF
	li      r4,-1
	b       func_end
no_overflow:
	subfic  r8,r5,32
	subic   r9,r5,32
	slw     r3,r3,r5
	srw     r10,r4,r8
	or      r3,r3,r10
	slw     r10,r4,r9
	or      r3,r3,r10
	slw     r4,r4,r5
around:
	rlwinm. r6,r6,0,0,0
	beq     cr0,positive
	subfic  r4,r4,0
	subfze  r3,r3
positive:
func_end:
	addi    r1,r1,16
	blr
#endif // clang-format on
}

#ifdef __cplusplus
}
#endif
