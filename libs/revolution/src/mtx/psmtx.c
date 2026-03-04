#include <revolution.h>
#include <revolution/mtx.h>
#include "sdk_math.h"

asm void PSMTXReorder(const __REGISTER Mtx src, __REGISTER ROMtx dest) {
    psq_l f0, 0(src), 0, 0
    psq_l f2, 16(src), 0, 0
    psq_l f4, 32(src), 0, 0
    psq_l f1, 8(src), 0, 0
    ps_merge00 f6, f0, f2
    psq_l f3, 24(src), 0, 0
    ps_merge01 f12, f4, f0
    psq_l f5, 40(src), 0, 0
    ps_merge11 f7, f2, f4
    psq_st f6, 0(dest), 0, 0
    ps_merge00 f8, f1, f3
    psq_st f12, 8(dest), 0, 0
    ps_merge01 f9, f5, f1
    psq_st f7, 16(dest), 0, 0
    ps_merge11 f10, f3, f5
    psq_st f8, 24(dest), 0, 0
    psq_st f9, 32(dest), 0, 0
    psq_st f10, 40(dest), 0, 0
}

asm void PSMTXROMultVecArray(const __REGISTER ROMtx m, const __REGISTER Vec* srcBase, __REGISTER Vec* dstBase, __REGISTER u32 count) {
    nofralloc
    stwu r1, -64(r1)
    stfd f14, 8(r1)
    subi r7, count, 1
    stfd f15, 16(r1)
    srwi r7, r7, 1
    stfd f16, 24(r1)
    stfd f17, 32(r1)
    stfd f18, 40(r1)
    mtctr r7
    psq_l f0, 0(m), 0, 0
    subi srcBase, srcBase, 8
    psq_l f1, 8(m), 1, 0
    subi dstBase, dstBase, 4
    psq_l f6, 36(m), 0, 0
    psq_lu f8, 8(srcBase), 0, 0
    psq_l f7, 44(m), 1, 0
    psq_lu f9, 8(srcBase), 0, 0
    ps_madds0 f11, f0, f8, f6
    psq_l f2, 12(m), 0, 0
    ps_madds0 f12, f1, f8, f7
    psq_l f3, 20(m), 1, 0
    ps_madds1 f13, f0, f9, f6
    psq_lu f10, 8(srcBase), 0, 0
    ps_madds1 f14, f1, f9, f7
    psq_l f5, 32(m), 1, 0
    ps_madds1 f11, f2, f8, f11
    ps_madds1 f12, f3, f8, f12
    psq_l f4, 24(m), 0, 0
    ps_madds0 f13, f2, f10, f13
    psq_lu f8, 8(srcBase), 0, 0
    ps_madds0 f14, f3, f10, f14
    ps_madds0 f15, f4, f9, f11
    ps_madds0 f16, f5, f9, f12
    psq_lu f9, 8(srcBase), 0, 0
    ps_madds1 f17, f4, f10, f13
    ps_madds1 f18, f5, f10, f14
    psq_lu f10, 8(srcBase), 0, 0
loop:
    ps_madds0 f11, f0, f8, f6
    psq_stu f15, 4(dstBase), 0, 0
    ps_madds0 f12, f1, f8, f7
    psq_stu f16, 8(dstBase), 1, 0
    ps_madds1 f13, f0, f9, f6
    psq_stu f17, 4(dstBase), 0, 0
    ps_madds1 f14, f1, f9, f7
    psq_stu f18, 8(dstBase), 1, 0
    ps_madds1 f11, f2, f8, f11
    ps_madds1 f12, f3, f8, f12
    psq_lu f8, 8(srcBase), 0, 0
    ps_madds0 f13, f2, f10, f13
    ps_madds0 f14, f3, f10, f14
    ps_madds0 f15, f4, f9, f11
    ps_madds0 f16, f5, f9, f12
    psq_lu f9, 8(srcBase), 0, 0
    ps_madds1 f17, f4, f10, f13
    ps_madds1 f18, f5, f10, f14
    psq_lu f10, 8(srcBase), 0, 0
    bdnz loop
    psq_stu f15, 4(dstBase), 0, 0
    clrlwi. r7, count, 31
    psq_stu f16, 8(dstBase), 1, 0
    bne exit
    psq_stu f17, 4(dstBase), 0, 0
    psq_stu f18, 8(dstBase), 1, 0
exit:
    lfd f14, 8(r1)
    lfd f15, 16(r1)
    lfd f16, 24(r1)
    lfd f17, 32(r1)
    lfd f18, 40(r1)
    addi r1, r1, 64
    blr
}

asm void PSMTXROSkin2VecArray(const __REGISTER ROMtx m0, const __REGISTER ROMtx m1, const __REGISTER f32* wtBase, const __REGISTER Vec* srcBase, __REGISTER Vec* dstBase, __REGISTER u32 count) {
    nofralloc
    stwu r1, -160(r1)
    stfd f14, 8(r1)
    stfd f15, 16(r1)
    stfd f16, 24(r1)
    stfd f17, 32(r1)
    stfd f18, 40(r1)
    stfd f19, 48(r1)
    stfd f20, 56(r1)
    stfd f21, 64(r1)
    stfd f22, 72(r1)
    stfd f23, 80(r1)
    stfd f24, 88(r1)
    stfd f25, 96(r1)
    stfd f26, 104(r1)
    stfd f27, 112(r1)
    stfd f28, 120(r1)
    stfd f29, 128(r1)
    stfd f30, 136(r1)
    subi r9, r8, 1
    mtctr r9
    subi srcBase, srcBase, 4
    subi dstBase, dstBase, 4
    subi wtBase, wtBase, 4
    psq_l f14, 0(m0), 0, 0
    psq_l f22, 0(m1), 0, 0
    psq_l f15, 8(m0), 1, 0
    psq_l f23, 8(m1), 1, 0
    psq_l f16, 12(m0), 0, 0
    psq_l f24, 12(m1), 0, 0
    ps_sub f22, f22, f14
    psq_l f17, 20(m0), 1, 0
    psq_l f25, 20(m1), 1, 0
    ps_sub f23, f23, f15
    psq_l f18, 24(m0), 0, 0
    psq_l f26, 24(m1), 0, 0
    ps_sub f24, f24, f16
    psq_l f19, 32(m0), 1, 0
    psq_l f27, 32(m1), 1, 0
    ps_sub f25, f25, f17
    psq_l f20, 36(m0), 0, 0
    psq_l f28, 36(m1), 0, 0
    ps_sub f26, f26, f18
    psq_l f21, 44(m0), 1, 0
    psq_l f29, 44(m1), 1, 0
    ps_sub f27, f27, f19
    ps_sub f28, f28, f20
    ps_sub f29, f29, f21
    psq_lu f30, 4(wtBase), 1, 0
    psq_lu f8, 4(srcBase), 0, 0
    psq_lu f9, 8(srcBase), 1, 0
    ps_madds0 f0, f22, f30, f14
    ps_madds0 f1, f23, f30, f15
    ps_madds0 f2, f24, f30, f16
    ps_madds0 f3, f25, f30, f17
    ps_madds0 f4, f26, f30, f18
    ps_madds0 f5, f27, f30, f19
    ps_madds0 f6, f28, f30, f20
    ps_madds0 f7, f29, f30, f21
    ps_madds0 f12, f0, f8, f6
    ps_madds0 f13, f1, f8, f7
    psq_lu f30, 4(wtBase), 1, 0
loop:
    ps_madds1 f12, f2, f8, f12
    ps_madds1 f13, f3, f8, f13
    psq_lu f8, 4(srcBase), 0, 0
    ps_madds0 f10, f4, f9, f12
    ps_madds0 f11, f5, f9, f13
    psq_lu f9, 8(srcBase), 1, 0
    ps_madds0 f0, f22, f30, f14
    ps_madds0 f1, f23, f30, f15
    ps_madds0 f2, f24, f30, f16
    ps_madds0 f3, f25, f30, f17
    ps_madds0 f4, f26, f30, f18
    ps_madds0 f5, f27, f30, f19
    ps_madds0 f6, f28, f30, f20
    ps_madds0 f7, f29, f30, f21
    psq_stu f10, 4(dstBase), 0, 0
    ps_madds0 f12, f0, f8, f6
    ps_madds0 f13, f1, f8, f7
    psq_stu f11, 8(dstBase), 1, 0
    psq_lu f30, 4(wtBase), 1, 0
    bdnz loop
    ps_madds1 f12, f2, f8, f12
    ps_madds1 f13, f3, f8, f13
    ps_madds0 f10, f4, f9, f12
    psq_stu f10, 4(dstBase), 0, 0
    ps_madds0 f11, f5, f9, f13
    psq_stu f11, 8(dstBase), 1, 0
    lfd f14, 8(r1)
    lfd f15, 16(r1)
    lfd f16, 24(r1)
    lfd f17, 32(r1)
    lfd f18, 40(r1)
    lfd f19, 48(r1)
    lfd f20, 56(r1)
    lfd f21, 64(r1)
    lfd f22, 72(r1)
    lfd f23, 80(r1)
    lfd f24, 88(r1)
    lfd f25, 96(r1)
    lfd f26, 104(r1)
    lfd f27, 112(r1)
    lfd f28, 120(r1)
    lfd f29, 128(r1)
    lfd f30, 136(r1)
    addi r1, r1, 160
    blr
}

asm void PSMTXROMultS16VecArray(const __REGISTER Mtx m, const __REGISTER S16Vec* srcBase, __REGISTER Vec* dstBase, __REGISTER u32 count) {
    nofralloc
    stwu r1, -64(r1)
    stfd f14, 8(r1)
    subi r7, count, 1
    stfd f15, 16(r1)
    srwi r7, r7, 1
    stfd f16, 24(r1)
    lis r8, 7
    stfd f17, 32(r1)
    mtspr GQR6, r8
    stfd f18, 40(r1)
    mtctr r7
    psq_l f0, 0(m), 0, 0
    subi srcBase, srcBase, 4
    psq_l f1, 8(m), 1, 0
    subi dstBase, dstBase, 4
    psq_l f6, 36(m), 0, 0
    psq_lu f8, 4(srcBase), 0, 6
    psq_l f7, 44(m), 1, 0
    psq_lu f9, 4(srcBase), 0, 6
    ps_madds0 f11, f0, f8, f6
    psq_l f2, 12(m), 0, 0
    ps_madds0 f12, f1, f8, f7
    psq_l f3, 20(m), 1, 0
    ps_madds1 f13, f0, f9, f6
    psq_lu f10, 4(srcBase), 0, 6
    ps_madds1 f14, f1, f9, f7
    psq_l f5, 32(m), 1, 0
    ps_madds1 f11, f2, f8, f11
    ps_madds1 f12, f3, f8, f12
    psq_l f4, 24(m), 0, 0
    ps_madds0 f13, f2, f10, f13
    psq_lu f8, 4(srcBase), 0, 6
    ps_madds0 f14, f3, f10, f14
    ps_madds0 f15, f4, f9, f11
    ps_madds0 f16, f5, f9, f12
    psq_lu f9, 4(srcBase), 0, 6
    ps_madds1 f17, f4, f10, f13
    ps_madds1 f18, f5, f10, f14
    psq_lu f10, 4(srcBase), 0, 6
loop:
    ps_madds0 f11, f0, f8, f6
    psq_stu f15, 4(dstBase), 0, 0
    ps_madds0 f12, f1, f8, f7
    psq_stu f16, 8(dstBase), 1, 0
    ps_madds1 f13, f0, f9, f6
    psq_stu f17, 4(dstBase), 0, 0
    ps_madds1 f14, f1, f9, f7
    psq_stu f18, 8(dstBase), 1, 0
    ps_madds1 f11, f2, f8, f11
    ps_madds1 f12, f3, f8, f12
    psq_lu f8, 4(srcBase), 0, 6
    ps_madds0 f13, f2, f10, f13
    ps_madds0 f14, f3, f10, f14
    ps_madds0 f15, f4, f9, f11
    ps_madds0 f16, f5, f9, f12
    psq_lu f9, 4(srcBase), 0, 6
    ps_madds1 f17, f4, f10, f13
    ps_madds1 f18, f5, f10, f14
    psq_lu f10, 4(srcBase), 0, 6
    bdnz loop
    psq_stu f15, 4(dstBase), 0, 0
    clrlwi. r7, count, 31
    psq_stu f16, 8(dstBase), 1, 0
    bne exit
    psq_stu f17, 4(dstBase), 0, 0
    psq_stu f18, 8(dstBase), 1, 0
exit:
    lfd f14, 8(r1)
    lfd f15, 16(r1)
    lfd f16, 24(r1)
    lfd f17, 32(r1)
    lfd f18, 40(r1)
    addi r1, r1, 64
    blr
}

asm void PSMTXMultS16VecArray(const __REGISTER ROMtx* m, const __REGISTER S16Vec* srcBase, __REGISTER Vec* dstBase, __REGISTER u32 count) {
    psq_l f0, 0(m), 0, 0
    lis r7, 7
    mtspr GQR6, r7
    psq_l f6, 0(srcBase), 0, 6
    subi count, count, 1
    psq_l f7, 4(srcBase), 1, 6
    mtctr count
    psq_l f1, 8(m), 0, 0
    addi srcBase, srcBase, 4
    psq_l f2, 16(m), 0, 0
    subi dstBase, dstBase, 4
    psq_l f3, 24(m), 0, 0
    ps_mul f8, f0, f6
    psq_l f4, 32(m), 0, 0
    ps_mul f10, f2, f6
    psq_l f5, 40(m), 0, 0
    ps_mul f12, f4, f6
    psq_lu f6, 2(srcBase), 0, 6
    ps_madd f8, f1, f7, f8
    ps_madd f10, f3, f7, f10
    ps_madd f12, f5, f7, f12
    psq_lu f7, 4(srcBase), 1, 6
    ps_sum0 f9, f8, f8, f8
loop:
    ps_sum0 f11, f10, f10, f10
    ps_mul f8, f0, f6
    ps_sum0 f13, f12, f12, f12
    ps_mul f10, f2, f6
    psq_stu f9, 4(dstBase), 1, 0
    ps_mul f12, f4, f6
    psq_stu f11, 4(dstBase), 1, 0
    ps_madd f8, f1, f7, f8
    psq_stu f13, 4(dstBase), 1, 0
    ps_madd f10, f3, f7, f10
    psq_lu f6, 2(srcBase), 0, 6
    ps_madd f12, f5, f7, f12
    psq_lu f7, 4(srcBase), 1, 6
    ps_sum0 f9, f8, f8, f8
    bdnz loop
    ps_sum0 f11, f10, f10, f10
    ps_sum0 f13, f12, f12, f12
    psq_stu f9, 4(dstBase), 1, 0
    psq_stu f11, 4(dstBase), 1, 0
    psq_stu f13, 4(dstBase), 1, 0
}
