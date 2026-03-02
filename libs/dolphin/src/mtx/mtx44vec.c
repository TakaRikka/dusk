#include <dolphin/dolphin.h>
#include <dolphin/mtx.h>
#include "sdk_math.h"

void C_MTX44MultVec(const Mtx44 m, const Vec* src, Vec* dst) {
    Vec vTmp;
    f32 w;

    ASSERTMSGLINE(67, m, "MTX44MultVec():  NULL Mtx44Ptr 'm' ");
    ASSERTMSGLINE(68, src, "MTX44MultVec():  NULL VecPtr 'src' ");
    ASSERTMSGLINE(69, dst, "MTX44MultVec():  NULL VecPtr 'dst' ");

    vTmp.x = m[0][0] * src->x + m[0][1] * src->y + m[0][2] * src->z + m[0][3];
    vTmp.y = m[1][0] * src->x + m[1][1] * src->y + m[1][2] * src->z + m[1][3];
    vTmp.z = m[2][0] * src->x + m[2][1] * src->y + m[2][2] * src->z + m[2][3];
    w = m[3][0] * src->x + m[3][1] * src->y + m[3][2] * src->z + m[3][3];
    w = 1.0f / w;

    dst->x = vTmp.x * w;
    dst->y = vTmp.y * w;
    dst->z = vTmp.z * w;
}

asm void PSMTX44MultVec(const __REGISTER Mtx44 m, const __REGISTER Vec* src, __REGISTER Vec* dst) {
    nofralloc
    psq_l f0, 0x0(src), 0, 0
    psq_l f2, 0x30(m), 0, 0
    psq_l f1, 0x8(src), 1, 0
    ps_mul f4, f0, f2
    psq_l f3, 0x38(m), 0, 0
    ps_madd f5, f1, f3, f4
    ps_merge11 f12, f1, f1
    ps_sum0 f13, f5, f5, f5
    psq_l f4, 0x0(m), 0, 0
    ps_merge00 f13, f13, f13
    psq_l f5, 0x8(m), 0, 0
    ps_div f13, f12, f13
    psq_l f6, 0x10(m), 0, 0
    psq_l f7, 0x18(m), 0, 0
    psq_l f8, 0x20(m), 0, 0
    psq_l f9, 0x28(m), 0, 0
    ps_mul f4, f0, f4
    ps_madd f2, f1, f5, f4
    ps_mul f6, f0, f6
    ps_madd f3, f1, f7, f6
    ps_mul f8, f0, f8
    ps_sum0 f2, f2, f2, f2
    ps_madd f9, f1, f9, f8
    ps_sum1 f2, f3, f2, f3
    ps_sum0 f3, f9, f9, f9
    ps_mul f2, f2, f13
    psq_st f2, 0x0(dst), 0, 0
    ps_mul f3, f3, f13
    psq_st f3, 0x8(dst), 1, 0
    blr
}

void C_MTX44MultVecArray(const Mtx44 m, const Vec* srcBase, Vec* dstBase, u32 count) {
    u32 i;
    Vec vTmp;
    f32 w;

    ASSERTMSGLINE(154, m, "MTX44MultVecArray():  NULL Mtx44Ptr 'm' ");
    ASSERTMSGLINE(155, srcBase, "MTX44MultVecArray():  NULL VecPtr 'srcBase' ");
    ASSERTMSGLINE(156, dstBase, "MTX44MultVecArray():  NULL VecPtr 'dstBase' ");

    for(i = 0; i < count; i++) {
        vTmp.x = m[0][0] * srcBase->x + m[0][1] * srcBase->y + m[0][2] * srcBase->z + m[0][3];
        vTmp.y = m[1][0] * srcBase->x + m[1][1] * srcBase->y + m[1][2] * srcBase->z + m[1][3];
        vTmp.z = m[2][0] * srcBase->x + m[2][1] * srcBase->y + m[2][2] * srcBase->z + m[2][3];
        w = m[3][0] * srcBase->x + m[3][1] * srcBase->y + m[3][2] * srcBase->z + m[3][3];
        w = 1.0f / w;
        dstBase->x = vTmp.x * w;
        dstBase->y = vTmp.y * w;
        dstBase->z = vTmp.z * w;
        srcBase++;
        dstBase++;
    }
}

asm void PSMTX44MultVecArray(const __REGISTER Mtx44 m, const __REGISTER Vec* srcBase, __REGISTER Vec* dstBase, __REGISTER u32 count) {
    nofralloc
    stwu r1, -0x10(r1)
    subi count, count, 0x1
    psq_l f6, 0x30(m), 0, 0
    mtctr count
    psq_l f8, 0x0(srcBase), 0, 0
    subi dstBase, dstBase, 0x4
    psq_l f7, 0x38(m), 0, 0
    psq_lu f9, 0x8(srcBase), 1, 0
    ps_mul f13, f6, f8
    psq_l f0, 0x0(m), 0, 0
    stfd f14, 0x8(r1)
    ps_madd f13, f7, f9, f13
    psq_l f2, 0x10(m), 0, 0
    ps_merge11 f14, f9, f9
    ps_mul f10, f0, f8
    psq_l f4, 0x20(m), 0, 0
    ps_mul f11, f2, f8
    psq_l f1, 0x8(m), 0, 0
    ps_mul f12, f4, f8
    psq_l f3, 0x18(m), 0, 0
    ps_sum0 f13, f13, f13, f13
    psq_l f5, 0x28(m), 0, 0
L_00000468:
    ps_madd f10, f1, f9, f10
    ps_madd f11, f3, f9, f11
    ps_madd f12, f5, f9, f12
    ps_sum0 f10, f10, f10, f10
    ps_sum0 f11, f11, f11, f11
    ps_sum0 f12, f12, f12, f12
    ps_div f13, f14, f13
    psq_lu f8, 0x4(srcBase), 0, 0
    psq_lu f9, 0x8(srcBase), 1, 0
    ps_mul f10, f10, f13
    psq_stu f10, 0x4(dstBase), 1, 0
    ps_mul f11, f11, f13
    psq_stu f11, 0x4(dstBase), 1, 0
    ps_mul f12, f12, f13
    psq_stu f12, 0x4(dstBase), 1, 0
    ps_mul f13, f6, f8
    ps_mul f10, f0, f8
    ps_mul f11, f2, f8
    ps_madd f13, f7, f9, f13
    ps_mul f12, f4, f8
    ps_sum0 f13, f13, f13, f13
    bdnz L_00000468
    ps_madd f10, f1, f9, f10
    ps_madd f11, f3, f9, f11
    ps_madd f12, f5, f9, f12
    ps_sum0 f10, f10, f10, f10
    ps_sum0 f11, f11, f11, f11
    ps_sum0 f12, f12, f12, f12
    ps_div f13, f14, f13
    ps_mul f10, f10, f13
    psq_st f10, 0x4(dstBase), 1, 0
    ps_mul f11, f11, f13
    psq_st f11, 0x8(dstBase), 1, 0
    ps_mul f12, f12, f13
    psq_st f12, 0xc(dstBase), 1, 0
    lfd f14, 0x8(r1)
    addi r1, r1, 0x10
    blr
}

void C_MTX44MultVecSR(const Mtx44 m, const Vec* src, Vec* dst) {
    Vec vTmp;

    ASSERTMSGLINE(288, m, "MTX44MultVecSR():  NULL Mtx44Ptr 'm' ");
    ASSERTMSGLINE(289, src, "MTX44MultVecSR():  NULL VecPtr 'src' ");
    ASSERTMSGLINE(290, dst, "MTX44MultVecSR():  NULL VecPtr 'dst' ");
    vTmp.x = (m[0][2] * src->z) + ((m[0][0] * src->x) + (m[0][1] * src->y));
    vTmp.y = (m[1][2] * src->z) + ((m[1][0] * src->x) + (m[1][1] * src->y));
    vTmp.z = (m[2][2] * src->z) + ((m[2][0] * src->x) + (m[2][1] * src->y));
    dst->x = vTmp.x;
    dst->y = vTmp.y;
    dst->z = vTmp.z;
}

asm void PSMTX44MultVecSR(const __REGISTER Mtx m, const __REGISTER Vec* src, __REGISTER Vec* dst) {
    nofralloc
    psq_l f0, 0x0(m), 0, 0
    psq_l f6, 0x0(src), 0, 0
    psq_l f2, 0x10(m), 0, 0
    ps_mul f8, f0, f6
    psq_l f4, 0x20(m), 0, 0
    ps_mul f10, f2, f6
    psq_l f7, 0x8(src), 1, 0
    ps_mul f12, f4, f6
    psq_l f3, 0x18(m), 0, 0
    ps_sum0 f8, f8, f8, f8
    psq_l f5, 0x28(m), 0, 0
    ps_sum0 f10, f10, f10, f10
    psq_l f1, 0x8(m), 0, 0
    ps_sum0 f12, f12, f12, f12
    ps_madd f9, f1, f7, f8
    psq_st f9, 0x0(dst), 1, 0
    ps_madd f11, f3, f7, f10
    psq_st f11, 0x4(dst), 1, 0
    ps_madd f13, f5, f7, f12
    psq_st f13, 0x8(dst), 1, 0
    blr
}

void C_MTX44MultVecArraySR(const Mtx44 m, const Vec* srcBase, Vec* dstBase, u32 count) {
    u32 i;
    Vec vTmp;

    ASSERTMSGLINE(379, m, "MTX44MultVecArraySR():  NULL Mtx44Ptr 'm' ");
    ASSERTMSGLINE(380, srcBase, "MTX44MultVecArraySR():  NULL VecPtr 'srcBase' ");
    ASSERTMSGLINE(381, dstBase, "MTX44MultVecArraySR():  NULL VecPtr 'dstBase' ");

    for(i = 0; i < count; i++) {
        vTmp.x = (m[0][2] * srcBase->z) + ((m[0][0] * srcBase->x) + (m[0][1] * srcBase->y));
        vTmp.y = (m[1][2] * srcBase->z) + ((m[1][0] * srcBase->x) + (m[1][1] * srcBase->y));
        vTmp.z = (m[2][2] * srcBase->z) + ((m[2][0] * srcBase->x) + (m[2][1] * srcBase->y));
        dstBase->x = vTmp.x;
        dstBase->y = vTmp.y;
        dstBase->z = vTmp.z;
        srcBase++;
        dstBase++;
    }
}

asm void PSMTX44MultVecArraySR(const __REGISTER Mtx44 m, const __REGISTER Vec* srcBase, __REGISTER Vec* dstBase, __REGISTER u32 count) {
    nofralloc
    psq_l f0, 0x0(m), 0, 0
    subi count, count, 0x1
    psq_l f6, 0x0(srcBase), 0, 0
    ps_mul f8, f0, f6
    psq_l f2, 0x10(m), 0, 0
    ps_mul f9, f2, f6
    psq_l f4, 0x20(m), 0, 0
    psq_lu f7, 0x8(srcBase), 1, 0
    ps_mul f10, f4, f6
    psq_l f1, 0x8(m), 1, 0
    mtctr count
    psq_l f3, 0x18(m), 1, 0
    subi dstBase, dstBase, 0x4
    psq_l f5, 0x28(m), 1, 0
L_00000890:
    ps_madd f11, f1, f7, f8
    psq_lu f6, 0x4(srcBase), 0, 0
    ps_madd f12, f3, f7, f9
    ps_madd f13, f5, f7, f10
    psq_lu f7, 0x8(srcBase), 1, 0
    ps_sum0 f11, f11, f8, f8
    psq_stu f11, 0x4(dstBase), 1, 0
    ps_sum0 f12, f12, f9, f9
    psq_stu f12, 0x4(dstBase), 1, 0
    ps_sum0 f13, f13, f10, f10
    psq_stu f13, 0x4(dstBase), 1, 0
    ps_mul f8, f0, f6
    ps_mul f9, f2, f6
    ps_mul f10, f4, f6
    bdnz L_00000890
    ps_madd f11, f1, f7, f8
    ps_madd f12, f3, f7, f9
    ps_madd f13, f5, f7, f10
    ps_sum0 f11, f11, f8, f8
    psq_stu f11, 0x4(dstBase), 1, 0
    ps_sum0 f12, f12, f9, f9
    psq_stu f12, 0x4(dstBase), 1, 0
    ps_sum0 f13, f13, f10, f10
    psq_stu f13, 0x4(dstBase), 1, 0
    blr
}
