#include <revolution.h>
#include <revolution/mtx.h>
#include <sdk_math.h>

void C_MTXMultVec(const Mtx m, const Vec* src, Vec* dst) {
    Vec vTmp;

    ASSERTMSGLINE(72, m, "MTXMultVec():  NULL MtxPtr 'm' ");
    ASSERTMSGLINE(73, src, "MTXMultVec():  NULL VecPtr 'src' ");
    ASSERTMSGLINE(74, dst, "MTXMultVec():  NULL VecPtr 'dst' ");

    vTmp.x = m[0][3] + ((m[0][2] * src->z) + ((m[0][0] * src->x) + (m[0][1] * src->y)));
    vTmp.y = m[1][3] + ((m[1][2] * src->z) + ((m[1][0] * src->x) + (m[1][1] * src->y)));
    vTmp.z = m[2][3] + ((m[2][2] * src->z) + ((m[2][0] * src->x) + (m[2][1] * src->y)));
    dst->x = vTmp.x;
    dst->y = vTmp.y;
    dst->z = vTmp.z;
}

asm void PSMTXMultVec(const __REGISTER Mtx m, const __REGISTER Vec* src, __REGISTER Vec* dst) {
    nofralloc
    psq_l f0, Vec.x(src), 0, 0
    psq_l f2, 0(m), 0, 0
    psq_l f1, Vec.z(src), 1, 0
    ps_mul f4, f2, f0
    psq_l f3, 8(m), 0, 0
    ps_madd f5, f3, f1, f4
    psq_l f8, 16(m), 0, 0
    ps_sum0 f6, f5, f6, f5
    psq_l f9, 24(m), 0, 0
    ps_mul f10, f8, f0
    psq_st f6, Vec.x(dst), 1, 0
    ps_madd f11, f9, f1, f10
    psq_l f2, 32(m), 0, 0
    ps_sum0 f12, f11, f12, f11
    psq_l f3, 40(m), 0, 0
    ps_mul f4, f2, f0
    psq_st f12, Vec.y(dst), 1, 0
    ps_madd f5, f3, f1, f4
    ps_sum0 f6, f5, f6, f5
    psq_st f6, Vec.z(dst), 1, 0
    blr
}

void C_MTXMultVecArray(const Mtx m, const Vec* srcBase, Vec* dstBase, u32 count) {
    u32 i;
    Vec vTmp;

    ASSERTMSGLINE(168, m, "MTXMultVecArray():  NULL MtxPtr 'm' ");
    ASSERTMSGLINE(169, srcBase, "MTXMultVecArray():  NULL VecPtr 'srcBase' ");
    ASSERTMSGLINE(170, dstBase, "MTXMultVecArray():  NULL VecPtr 'dstBase' ");
    ASSERTMSGLINE(171, count > 1, "MTXMultVecArray():  count must be greater than 1.");

    for(i = 0; i < count; i++) {
        vTmp.x = m[0][3] + ((m[0][2] * srcBase->z) + ((m[0][0] * srcBase->x) + (m[0][1] * srcBase->y)));
        vTmp.y = m[1][3] + ((m[1][2] * srcBase->z) + ((m[1][0] * srcBase->x) + (m[1][1] * srcBase->y)));
        vTmp.z = m[2][3] + ((m[2][2] * srcBase->z) + ((m[2][0] * srcBase->x) + (m[2][1] * srcBase->y)));
        dstBase->x = vTmp.x;
        dstBase->y = vTmp.y;
        dstBase->z = vTmp.z;
        srcBase++;
        dstBase++;
    }
}

asm void PSMTXMultVecArray(const __REGISTER Mtx m, const __REGISTER Vec* srcBase, __REGISTER Vec* dstBase, __REGISTER u32 count) {
    nofralloc
    psq_l f13, 0x0(m), 0, 0
    psq_l f12, 0x10(m), 0, 0
    subi count, count, 0x1
    psq_l f11, 0x8(m), 0, 0
    ps_merge00 f0, f13, f12
    subi dstBase, dstBase, 0x4
    psq_l f10, 0x18(m), 0, 0
    ps_merge11 f1, f13, f12
    mtctr count
    psq_l f4, 0x20(m), 0, 0
    ps_merge00 f2, f11, f10
    psq_l f5, 0x28(m), 0, 0
    ps_merge11 f3, f11, f10
    psq_l f6, 0x0(srcBase), 0, 0
    psq_lu f7, 0x8(srcBase), 1, 0
    ps_madds0 f8, f0, f6, f3
    ps_mul f9, f4, f6
    ps_madds1 f8, f1, f6, f8
    ps_madd f10, f5, f7, f9
L_000003C4:
    psq_lu f6, 0x4(srcBase), 0, 0
    ps_madds0 f12, f2, f7, f8
    psq_lu f7, 0x8(srcBase), 1, 0
    ps_sum0 f13, f10, f9, f10
    ps_madds0 f8, f0, f6, f3
    ps_mul f9, f4, f6
    psq_stu f12, 0x4(dstBase), 0, 0
    ps_madds1 f8, f1, f6, f8
    psq_stu f13, 0x8(dstBase), 1, 0
    ps_madd f10, f5, f7, f9
    bdnz L_000003C4
    ps_madds0 f12, f2, f7, f8
    ps_sum0 f13, f10, f9, f10
    psq_stu f12, 0x4(dstBase), 0, 0
    psq_stu f13, 0x8(dstBase), 1, 0
    blr
}

void C_MTXMultVecSR(const Mtx m, const Vec* src, Vec* dst) {
    Vec vTmp;

    ASSERTMSGLINE(319, m, "MTXMultVecSR():  NULL MtxPtr 'm' ");
    ASSERTMSGLINE(320, src, "MTXMultVecSR():  NULL VecPtr 'src' ");
    ASSERTMSGLINE(321, dst, "MTXMultVecSR():  NULL VecPtr 'dst' ");

    vTmp.x = (m[0][2] * src->z) + ((m[0][0] * src->x) + (m[0][1] * src->y));
    vTmp.y = (m[1][2] * src->z) + ((m[1][0] * src->x) + (m[1][1] * src->y));
    vTmp.z = (m[2][2] * src->z) + ((m[2][0] * src->x) + (m[2][1] * src->y));
    dst->x = vTmp.x;
    dst->y = vTmp.y;
    dst->z = vTmp.z;
}

asm void PSMTXMultVecSR(const __REGISTER Mtx m, const __REGISTER Vec* src, __REGISTER Vec* dst) {
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

void C_MTXMultVecArraySR(const Mtx m, const Vec* srcBase, Vec* dstBase, u32 count) {
    u32 i;
    Vec vTmp;

    ASSERTMSGLINE(416, m, "MTXMultVecArraySR():  NULL MtxPtr 'm' ");
    ASSERTMSGLINE(417, srcBase, "MTXMultVecArraySR():  NULL VecPtr 'srcBase' ");
    ASSERTMSGLINE(418, dstBase, "MTXMultVecArraySR():  NULL VecPtr 'dstBase' ");
    ASSERTMSGLINE(419, count > 1, "MTXMultVecArraySR():  count must be greater than 1.");

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

asm void PSMTXMultVecArraySR(const __REGISTER Mtx m, const __REGISTER Vec* srcBase, __REGISTER Vec* dstBase, __REGISTER u32 count) {
    nofralloc
    psq_l f13, 0x0(m), 0, 0
    psq_l f12, 0x10(m), 0, 0
    subi count, count, 0x1
    psq_l f11, 0x8(m), 1, 0
    ps_merge00 f0, f13, f12
    subi dstBase, dstBase, 0x4
    psq_l f10, 0x18(m), 1, 0
    ps_merge11 f1, f13, f12
    mtctr count
    psq_l f3, 0x20(m), 0, 0
    ps_merge00 f2, f11, f10
    psq_l f4, 0x28(m), 1, 0
    psq_l f6, 0x0(srcBase), 0, 0
    psq_lu f7, 0x8(srcBase), 1, 0
    ps_muls0 f8, f0, f6
    ps_mul f9, f3, f6
    ps_madds1 f8, f1, f6, f8
    ps_madd f10, f4, f7, f9
L_000007D0:
    psq_lu f6, 0x4(srcBase), 0, 0
    ps_madds0 f12, f2, f7, f8
    psq_lu f7, 0x8(srcBase), 1, 0
    ps_sum0 f13, f10, f9, f9
    ps_muls0 f8, f0, f6
    ps_mul f9, f3, f6
    psq_stu f12, 0x4(dstBase), 0, 0
    ps_madds1 f8, f1, f6, f8
    psq_stu f13, 0x8(dstBase), 1, 0
    ps_madd f10, f4, f7, f9
    bdnz L_000007D0
    ps_madds0 f12, f2, f7, f8
    ps_sum0 f13, f10, f9, f9
    psq_stu f12, 0x4(dstBase), 0, 0
    psq_stu f13, 0x8(dstBase), 1, 0
    blr
}
