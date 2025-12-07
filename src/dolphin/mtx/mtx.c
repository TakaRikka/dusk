#include <dolphin.h>
#include <dolphin/mtx.h>
#include <math.h>

static f32 Unit01[2] = {
    0.0f,
    1.0f
};

void C_MTXIdentity(Mtx m) {
    ASSERTMSGLINE(189, m, "MtxIdentity():  NULL Mtx 'm' ");
    m[0][0] = 1;
    m[0][1] = 0;
    m[0][2] = 0;
    m[0][3] = 0;
    m[1][0] = 0;
    m[1][1] = 1;
    m[1][2] = 0;
    m[1][3] = 0;
    m[2][0] = 0;
    m[2][1] = 0;
    m[2][2] = 1;
    m[2][3] = 0;
}

void PSMTXIdentity(__REGISTER Mtx m) {
    __REGISTER f32 c_zero = 0.0f;
    __REGISTER f32 c_one = 1.0f;
    __REGISTER f32 c_01;
    __REGISTER f32 c_10;

    asm {
        psq_st c_zero, 8(m), 0, 0
        ps_merge01 c_01, c_zero, c_one
        psq_st c_zero, 24(m), 0, 0
        ps_merge10 c_10, c_one, c_zero
        psq_st c_zero, 32(m), 0, 0
        psq_st c_01, 16(m), 0, 0
        psq_st c_10, 0(m), 0, 0
        psq_st c_10, 40(m), 0, 0
    }
}

void C_MTXCopy(const Mtx src, Mtx dst) {
    ASSERTMSGLINE(250, src, "MTXCopy():  NULL MtxPtr 'src' ");
    ASSERTMSGLINE(251, dst, "MTXCopy():  NULL MtxPtr 'dst' ");
    if (src != dst) {
        dst[0][0] = src[0][0];
        dst[0][1] = src[0][1];
        dst[0][2] = src[0][2];
        dst[0][3] = src[0][3];
        dst[1][0] = src[1][0];
        dst[1][1] = src[1][1];
        dst[1][2] = src[1][2];
        dst[1][3] = src[1][3];
        dst[2][0] = src[2][0];
        dst[2][1] = src[2][1];
        dst[2][2] = src[2][2];
        dst[2][3] = src[2][3];
    }
}

asm void PSMTXCopy(const __REGISTER Mtx src, __REGISTER Mtx dst) {
    psq_l f0, 0(src), 0, 0
    psq_st f0, 0(dst), 0, 0
    psq_l f1, 8(src), 0, 0
    psq_st f1, 8(dst), 0, 0
    psq_l f2, 16(src), 0, 0
    psq_st f2, 16(dst), 0, 0
    psq_l f3, 24(src), 0, 0
    psq_st f3, 24(dst), 0, 0
    psq_l f4, 32(src), 0, 0
    psq_st f4, 32(dst), 0, 0
    psq_l f5, 40(src), 0, 0
    psq_st f5, 40(dst), 0, 0
}

void C_MTXConcat(const Mtx a, const Mtx b, Mtx ab) {
    Mtx mTmp;
    MtxPtr m;

    ASSERTMSGLINE(324, a, "MTXConcat():  NULL MtxPtr 'a'  ");
    ASSERTMSGLINE(325, b, "MTXConcat():  NULL MtxPtr 'b'  ");
    ASSERTMSGLINE(326, ab, "MTXConcat():  NULL MtxPtr 'ab' ");

    if (ab == a || ab == b) {
        m = mTmp;
    } else {
        m = ab;
    }

    m[0][0] =       0 +  a[0][2] * b[2][0] + ((a[0][0] * b[0][0]) + (a[0][1] * b[1][0]));
    m[0][1] =       0 +  a[0][2] * b[2][1] + ((a[0][0] * b[0][1]) + (a[0][1] * b[1][1]));
    m[0][2] =       0 +  a[0][2] * b[2][2] + ((a[0][0] * b[0][2]) + (a[0][1] * b[1][2]));
    m[0][3] = a[0][3] + (a[0][2] * b[2][3] +  (a[0][0] * b[0][3]  + (a[0][1] * b[1][3])));

    m[1][0] =       0 +  a[1][2] * b[2][0] + ((a[1][0] * b[0][0]) + (a[1][1] * b[1][0]));
    m[1][1] =       0 +  a[1][2] * b[2][1] + ((a[1][0] * b[0][1]) + (a[1][1] * b[1][1]));
    m[1][2] =       0 +  a[1][2] * b[2][2] + ((a[1][0] * b[0][2]) + (a[1][1] * b[1][2]));
    m[1][3] = a[1][3] + (a[1][2] * b[2][3] +  (a[1][0] * b[0][3]  + (a[1][1] * b[1][3])));

    m[2][0] =       0 +  a[2][2] * b[2][0] + ((a[2][0] * b[0][0]) + (a[2][1] * b[1][0]));
    m[2][1] =       0 +  a[2][2] * b[2][1] + ((a[2][0] * b[0][1]) + (a[2][1] * b[1][1]));
    m[2][2] =       0 +  a[2][2] * b[2][2] + ((a[2][0] * b[0][2]) + (a[2][1] * b[1][2]));
    m[2][3] = a[2][3] + (a[2][2] * b[2][3] +  (a[2][0] * b[0][3]  + (a[2][1] * b[1][3])));

    if (m == mTmp) {
        C_MTXCopy(mTmp, ab);
    }
}

asm void PSMTXConcat(const __REGISTER Mtx a, const __REGISTER Mtx b, __REGISTER Mtx ab) {
    nofralloc
    stwu r1, -64(r1)
    psq_l f0, 0(a), 0, 0
    stfd f14, 8(r1)
    psq_l f6, 0(b), 0, 0
    lis r6, Unit01@ha
    psq_l f7, 8(b), 0, 0
    stfd f15, 16(r1)
    addi r6, r6, Unit01@l
    stfd f31, 40(r1)
    psq_l f8, 16(b), 0, 0
    ps_muls0 f12, f6, f0
    psq_l f2, 16(a), 0, 0
    ps_muls0 f13, f7, f0
    psq_l f31, 0(r6), 0, 0
    ps_muls0 f14, f6, f2
    psq_l f9, 24(b), 0, 0
    ps_muls0 f15, f7, f2
    psq_l f1, 8(a), 0, 0
    ps_madds1 f12, f8, f0, f12
    psq_l f3, 24(a), 0, 0
    ps_madds1 f14, f8, f2, f14
    psq_l f10, 32(b), 0, 0
    ps_madds1 f13, f9, f0, f13
    psq_l f11, 40(b), 0, 0
    ps_madds1 f15, f9, f2, f15
    psq_l f4, 32(a), 0, 0
    psq_l f5, 40(a), 0, 0
    ps_madds0 f12, f10, f1, f12
    ps_madds0 f13, f11, f1, f13
    ps_madds0 f14, f10, f3, f14
    ps_madds0 f15, f11, f3, f15
    psq_st f12, 0(ab), 0, 0
    ps_muls0 f2, f6, f4
    ps_madds1 f13, f31, f1, f13
    ps_muls0 f0, f7, f4
    psq_st f14, 16(ab), 0, 0
    ps_madds1 f15, f31, f3, f15
    psq_st f13, 8(ab), 0, 0
    ps_madds1 f2, f8, f4, f2
    ps_madds1 f0, f9, f4, f0
    ps_madds0 f2, f10, f5, f2
    lfd f14, 8(r1)
    psq_st f15, 24(ab), 0, 0
    ps_madds0 f0, f11, f5, f0
    psq_st f2, 32(ab), 0, 0
    ps_madds1 f0, f31, f5, f0
    lfd f15, 16(r1)
    psq_st f0, 40(ab), 0, 0
    lfd f31, 40(r1)
    addi r1, r1, 64
    blr
}

void C_MTXConcatArray(const Mtx a, const Mtx* srcBase, Mtx* dstBase, u32 count) {
    u32 i;

    ASSERTMSGLINE(580, a != 0, "MTXConcatArray(): NULL MtxPtr 'a' ");
    ASSERTMSGLINE(581, srcBase != 0, "MTXConcatArray(): NULL MtxPtr 'srcBase' ");
    ASSERTMSGLINE(582, dstBase != 0, "MTXConcatArray(): NULL MtxPtr 'dstBase' ");
    ASSERTMSGLINE(583, count > 1, "MTXConcatArray(): count must be greater than 1.");

    for (i = 0; i < count; i++) {
        C_MTXConcat(a, *srcBase, *dstBase);
        srcBase++;
        dstBase++;
    }
}

#if DEBUG
#pragma push
#pragma optimization_level 1
// This function will not compile at optimization level 0
#endif
void PSMTXConcatArray(const __REGISTER Mtx a, const __REGISTER Mtx* srcBase, __REGISTER Mtx* dstBase, __REGISTER u32 count) {
    __REGISTER f32 va0, va1, va2, va3, va4, va5;
    __REGISTER f32 vb0, vb1, vb2, vb3, vb4, vb5;
    __REGISTER f32 vd0, vd1, vd2, vd3, vd4, vd5;
    __REGISTER f32 u01;
    __REGISTER f32* u01Ptr = Unit01;

    asm {
        psq_l va0, 0(a), 0, 0;
        psq_l va1, 8(a), 0, 0;
        psq_l va2, 16(a), 0, 0;
        psq_l va3, 24(a), 0, 0;
        subi count, count, 1;
        psq_l va4, 32(a), 0, 0;
        psq_l va5, 40(a), 0, 0;
        mtctr count;
        psq_l u01, 0(u01Ptr), 0, 0;
        psq_l vb0, 0(srcBase), 0, 0;
        psq_l vb2, 16(srcBase), 0, 0;
        ps_muls0 vd0, vb0, va0;
        ps_muls0 vd2, vb0, va2;
        ps_muls0 vd4, vb0, va4;
        psq_l vb4, 32(srcBase), 0, 0;
        ps_madds1 vd0, vb2, va0, vd0;
        ps_madds1 vd2, vb2, va2, vd2;
        ps_madds1 vd4, vb2, va4, vd4;
        psq_l vb1, 8(srcBase), 0, 0;
        ps_madds0 vd0, vb4, va1, vd0;
        ps_madds0 vd2, vb4, va3, vd2;
        ps_madds0 vd4, vb4, va5, vd4;
        psq_l vb3, 24(srcBase), 0, 0;
        psq_st vd0, 0(dstBase), 0, 0;
        ps_muls0 vd1, vb1, va0;
        ps_muls0 vd3, vb1, va2;
        ps_muls0 vd5, vb1, va4;
        psq_l vb5, 40(srcBase), 0, 0;
        psq_st vd2, 16(dstBase), 0, 0;
        ps_madds1 vd1, vb3, va0, vd1;
        ps_madds1 vd3, vb3, va2, vd3;
        ps_madds1 vd5, vb3, va4, vd5;
    _loop:
        addi srcBase, srcBase, sizeof(Mtx);
        ps_madds0 vd1, vb5, va1, vd1;
        ps_madds0 vd3, vb5, va3, vd3;
        ps_madds0 vd5, vb5, va5, vd5;
        psq_l vb0, 0(srcBase), 0, 0;
        psq_st vd4, 32(dstBase), 0, 0;
        ps_madd vd1, u01, va1, vd1;
        ps_madd vd3, u01, va3, vd3;
        ps_madd vd5, u01, va5, vd5;
        psq_l vb2, 16(srcBase), 0, 0;
        psq_st vd1, 8(dstBase), 0, 0;
        ps_muls0 vd0, vb0, va0;
        ps_muls0 vd2, vb0, va2;
        ps_muls0 vd4, vb0, va4;
        psq_l vb4, 32(srcBase), 0, 0;
        psq_st vd3, 24(dstBase), 0, 0;
        ps_madds1 vd0, vb2, va0, vd0;
        ps_madds1 vd2, vb2, va2, vd2;
        ps_madds1 vd4, vb2, va4, vd4;
        psq_l vb1, 8(srcBase), 0, 0;
        psq_st vd5, 40(dstBase), 0, 0;
        addi dstBase, dstBase, sizeof(Mtx);
        ps_madds0 vd0, vb4, va1, vd0;
        ps_madds0 vd2, vb4, va3, vd2;
        ps_madds0 vd4, vb4, va5, vd4;
        psq_l vb3, 24(srcBase), 0, 0;
        psq_st vd0, 0(dstBase), 0, 0;
        ps_muls0 vd1, vb1, va0;
        ps_muls0 vd3, vb1, va2;
        ps_muls0 vd5, vb1, va4;
        psq_l vb5, 40(srcBase), 0, 0;
        psq_st vd2, 16(dstBase), 0, 0;
        ps_madds1 vd1, vb3, va0, vd1;
        ps_madds1 vd3, vb3, va2, vd3;
        ps_madds1 vd5, vb3, va4, vd5;
        bdnz _loop;
        psq_st vd4, 32(dstBase), 0, 0;
        ps_madds0 vd1, vb5, va1, vd1;
        ps_madds0 vd3, vb5, va3, vd3;
        ps_madds0 vd5, vb5, va5, vd5;
        ps_madd vd1, u01, va1, vd1;
        ps_madd vd3, u01, va3, vd3;
        ps_madd vd5, u01, va5, vd5;
        psq_st vd1, 8(dstBase), 0, 0;
        psq_st vd3, 24(dstBase), 0, 0;
        psq_st vd5, 40(dstBase), 0, 0;
    }
}
#if DEBUG
#pragma pop
#endif

void C_MTXTranspose(const Mtx src, Mtx xPose) {
    Mtx mTmp;
    MtxPtr m;

    ASSERTMSGLINE(851, src, "MTXTranspose():  NULL MtxPtr 'src' ");
    ASSERTMSGLINE(852, xPose, "MTXTranspose():  NULL MtxPtr 'xPose' ");

    if (src == xPose) {
        m = mTmp;
    } else {
        m = xPose;
    }

    m[0][0] = src[0][0];
    m[0][1] = src[1][0];
    m[0][2] = src[2][0];
    m[0][3] = 0;
    m[1][0] = src[0][1];
    m[1][1] = src[1][1];
    m[1][2] = src[2][1];
    m[1][3] = 0;
    m[2][0] = src[0][2];
    m[2][1] = src[1][2];
    m[2][2] = src[2][2];
    m[2][3] = 0;
    if (m == mTmp) {
        C_MTXCopy(mTmp, xPose);
    }
}

void PSMTXTranspose(const __REGISTER Mtx src, __REGISTER Mtx xPose) {
    __REGISTER f32 c_zero = 0;
    __REGISTER f32 row0a;
    __REGISTER f32 row1a;
    __REGISTER f32 row0b;
    __REGISTER f32 row1b;
    __REGISTER f32 trns0;
    __REGISTER f32 trns1;
    __REGISTER f32 trns2;

    asm {
        psq_l row0a, 0(src), 0, 0
    }
    xPose[2][3] = c_zero;
    asm {
        psq_l row1a, 16(src), 0, 0
        ps_merge00 trns0, row0a, row1a
        psq_l row0b, 8(src), 1, 0
        ps_merge11 trns1, row0a, row1a
        psq_l row1b, 24(src), 1, 0
        psq_st trns0, 0(xPose), 0, 0
        psq_l row0a, 32(src), 0, 0
        ps_merge00 trns2, row0b, row1b
        psq_st trns1, 16(xPose), 0, 0
        ps_merge00 trns0, row0a, c_zero
        psq_st trns2, 32(xPose), 0, 0
        ps_merge10 trns1, row0a, c_zero
        psq_st trns0, 8(xPose), 0, 0
    }
    row0b = src[2][2];
    asm {
        psq_st trns1, 24(xPose), 0, 0
    }
    xPose[2][2] = row0b;
}

u32 C_MTXInverse(const Mtx src, Mtx inv) {
    Mtx mTmp;
    MtxPtr m;
    f32 det;

    ASSERTMSGLINE(950, src, "MTXInverse():  NULL MtxPtr 'src' ");
    ASSERTMSGLINE(951, inv, "MTXInverse():  NULL MtxPtr 'inv' ");

    if (src == inv) {
        m = mTmp;
    } else {
        m = inv;
    }
    det = ((((src[2][1] * (src[0][2] * src[1][0])) 
          + ((src[2][2] * (src[0][0] * src[1][1])) 
           + (src[2][0] * (src[0][1] * src[1][2])))) 
           - (src[0][2] * (src[2][0] * src[1][1]))) 
           - (src[2][2] * (src[1][0] * src[0][1]))) 
           - (src[1][2] * (src[0][0] * src[2][1]));
    if (0 == det) {
        return 0;
    }
    det = 1 / det;
    m[0][0] = (det * +((src[1][1] * src[2][2]) - (src[2][1] * src[1][2])));
    m[0][1] = (det * -((src[0][1] * src[2][2]) - (src[2][1] * src[0][2])));
    m[0][2] = (det * +((src[0][1] * src[1][2]) - (src[1][1] * src[0][2])));

    m[1][0] = (det * -((src[1][0] * src[2][2]) - (src[2][0] * src[1][2])));
    m[1][1] = (det * +((src[0][0] * src[2][2]) - (src[2][0] * src[0][2])));
    m[1][2] = (det * -((src[0][0] * src[1][2]) - (src[1][0] * src[0][2])));

    m[2][0] = (det * +((src[1][0] * src[2][1]) - (src[2][0] * src[1][1])));
    m[2][1] = (det * -((src[0][0] * src[2][1]) - (src[2][0] * src[0][1])));
    m[2][2] = (det * +((src[0][0] * src[1][1]) - (src[1][0] * src[0][1])));

    m[0][3] = ((-m[0][0] * src[0][3]) - (m[0][1] * src[1][3])) - (m[0][2] * src[2][3]);
    m[1][3] = ((-m[1][0] * src[0][3]) - (m[1][1] * src[1][3])) - (m[1][2] * src[2][3]);
    m[2][3] = ((-m[2][0] * src[0][3]) - (m[2][1] * src[1][3])) - (m[2][2] * src[2][3]);

    if (m == mTmp) {
        C_MTXCopy(mTmp, inv);
    }
    return 1;
}

asm u32 PSMTXInverse(const __REGISTER Mtx src, __REGISTER Mtx inv) {
    psq_l f0, 0(src), 1, 0
    psq_l f1, 4(src), 0, 0
    psq_l f2, 16(src), 1, 0
    ps_merge10 f6, f1, f0
    psq_l f3, 20(src), 0, 0
    psq_l f4, 32(src), 1, 0
    ps_merge10 f7, f3, f2
    psq_l f5, 36(src), 0, 0
    ps_mul f11, f3, f6
    ps_mul f13, f5, f7
    ps_merge10 f8, f5, f4
    ps_msub f11, f1, f7, f11
    ps_mul f12, f1, f8
    ps_msub f13, f3, f8, f13
    ps_mul f10, f3, f4
    ps_msub f12, f5, f6, f12
    ps_mul f9, f0, f5
    ps_mul f8, f1, f2
    ps_sub f6, f6, f6
    ps_msub f10, f2, f5, f10
    ps_mul f7, f0, f13
    ps_msub f9, f1, f4, f9
    ps_madd f7, f2, f12, f7
    ps_msub f8, f0, f3, f8
    ps_madd f7, f4, f11, f7
    ps_cmpo0 cr0, f7, f6
    bne skip_return
    li r3, 0
    blr
skip_return:
    fres f0, f7
    ps_add f6, f0, f0
    ps_mul f5, f0, f0
    ps_nmsub f0, f7, f5, f6
    lfs f1, 12(src)
    ps_muls0 f13, f13, f0
    lfs f2, 28(src)
    ps_muls0 f12, f12, f0
    lfs f3, 44(src)
    ps_muls0 f11, f11, f0
    ps_merge00 f5, f13, f12
    ps_muls0 f10, f10, f0
    ps_merge11 f4, f13, f12
    ps_muls0 f9, f9, f0
    psq_st f5, 0(inv), 0, 0
    ps_mul f6, f13, f1
    psq_st f4, 16(inv), 0, 0
    ps_muls0 f8, f8, f0
    ps_madd f6, f12, f2, f6
    psq_st f10, 32(inv), 1, 0
    ps_nmadd f6, f11, f3, f6
    psq_st f9, 36(inv), 1, 0
    ps_mul f7, f10, f1
    ps_merge00 f5, f11, f6
    psq_st f8, 40(inv), 1, 0
    ps_merge11 f4, f11, f6
    psq_st f5, 8(inv), 0, 0
    ps_madd f7, f9, f2, f7
    psq_st f4, 24(inv), 0, 0
    ps_nmadd f7, f8, f3, f7
    li r3, 1
    psq_st f7, 44(inv), 1, 0
}

u32 C_MTXInvXpose(const Mtx src, Mtx invX) {
    Mtx mTmp;
    MtxPtr m;
    f32 det;

    ASSERTMSGLINE(1185, src, "MTXInvXpose(): NULL MtxPtr 'src' ");
    ASSERTMSGLINE(1186, invX, "MTXInvXpose(): NULL MtxPtr 'invX' ");

    if (src == invX) {
        m = mTmp;
    } else {
        m = invX;
    }
    det = ((((src[2][1] * (src[0][2] * src[1][0])) 
          + ((src[2][2] * (src[0][0] * src[1][1])) 
          +  (src[2][0] * (src[0][1] * src[1][2])))) 
          -  (src[0][2] * (src[2][0] * src[1][1]))) 
          -  (src[2][2] * (src[1][0] * src[0][1]))) 
          -  (src[1][2] * (src[0][0] * src[2][1]));
    if (0 == det) {
        return 0;
    }
    det = 1 / det;
    m[0][0] = (det * +((src[1][1] * src[2][2]) - (src[2][1] * src[1][2])));
    m[0][1] = (det * -((src[1][0] * src[2][2]) - (src[2][0] * src[1][2])));
    m[0][2] = (det * +((src[1][0] * src[2][1]) - (src[2][0] * src[1][1])));

    m[1][0] = (det * -((src[0][1] * src[2][2]) - (src[2][1] * src[0][2])));
    m[1][1] = (det * +((src[0][0] * src[2][2]) - (src[2][0] * src[0][2])));
    m[1][2] = (det * -((src[0][0] * src[2][1]) - (src[2][0] * src[0][1])));

    m[2][0] = (det * +((src[0][1] * src[1][2]) - (src[1][1] * src[0][2])));
    m[2][1] = (det * -((src[0][0] * src[1][2]) - (src[1][0] * src[0][2])));
    m[2][2] = (det * +((src[0][0] * src[1][1]) - (src[1][0] * src[0][1])));

    m[0][3] = 0;
    m[1][3] = 0;
    m[2][3] = 0;

    if (m == mTmp) {
        C_MTXCopy(mTmp, invX);
    }
    return 1;
}

asm u32 PSMTXInvXpose(const __REGISTER Mtx src, __REGISTER Mtx invX) {
	psq_l f0, 0(src), 1, 0
	psq_l f1, 4(src), 0, 0
	psq_l f2, 16(src), 1, 0
	ps_merge10 f6, f1, f0
	psq_l f3, 20(src), 0, 0
	psq_l f4, 32(src), 1, 0
	ps_merge10 f7, f3, f2
	psq_l f5, 36(src), 0, 0
	ps_mul f11, f3, f6
	ps_merge10 f8, f5, f4
	ps_mul f13, f5, f7
	ps_msub f11, f1, f7, f11
	ps_mul f12, f1, f8
	ps_msub f13, f3, f8, f13
	ps_msub f12, f5, f6, f12
	ps_mul f10, f3, f4
	ps_mul f9, f0, f5
	ps_mul f8, f1, f2
	ps_msub f10, f2, f5, f10
	ps_msub f9, f1, f4, f9
	ps_msub f8, f0, f3, f8
	ps_mul f7, f0, f13
	ps_sub f1, f1, f1
	ps_madd f7, f2, f12, f7
	ps_madd f7, f4, f11, f7
	ps_cmpo0 cr0, f7, f1
	bne skip_return
	li r3, 0
	blr
skip_return:
	fres f0, f7
	psq_st f1, 12(invX), 1, 0
	ps_add f6, f0, f0
	ps_mul f5, f0, f0
	psq_st f1, 28(invX), 1, 0
	ps_nmsub f0, f7, f5, f6
	psq_st f1, 44(invX), 1, 0
	ps_muls0 f13, f13, f0
	ps_muls0 f12, f12, f0
	ps_muls0 f11, f11, f0
    psq_st f13, 0(invX), 0, 0
	psq_st f12, 16(invX), 0, 0
	ps_muls0 f10, f10, f0
    ps_muls0 f9, f9, f0
	psq_st f11, 32(invX), 0, 0
	psq_st f10, 8(invX), 1, 0
	ps_muls0 f8, f8, f0
	li r3, 1
    psq_st f9, 24(invX), 1, 0
	psq_st f8, 40(invX), 1, 0
}

void C_MTXRotRad(Mtx m, char axis, f32 rad) {
    f32 sinA;
    f32 cosA;

    ASSERTMSGLINE(1447, m, "MTXRotRad():  NULL MtxPtr 'm' ");
    sinA = sinf(rad);
    cosA = cosf(rad);
    C_MTXRotTrig(m, axis, sinA, cosA);
}

void PSMTXRotRad(Mtx m, char axis, f32 rad) {
    f32 sinA, cosA;
    sinA = sinf(rad);
    cosA = cosf(rad);
    PSMTXRotTrig(m, axis, sinA, cosA);
}

void C_MTXRotTrig(Mtx m, char axis, f32 sinA, f32 cosA) {
    ASSERTMSGLINE(1502, m, "MTXRotTrig():  NULL MtxPtr 'm' ");
    switch(axis) {
        case 'x':
        case 'X':
            m[0][0] = 1;
            m[0][1] = 0;
            m[0][2] = 0;
            m[0][3] = 0;
            m[1][0] = 0;
            m[1][1] = cosA;
            m[1][2] = -sinA;
            m[1][3] = 0;
            m[2][0] = 0;
            m[2][1] = sinA;
            m[2][2] = cosA;
            m[2][3] = 0;
            break;
        case 'y':
        case 'Y':
            m[0][0] = cosA;
            m[0][1] = 0;
            m[0][2] = sinA;
            m[0][3] = 0;
            m[1][0] = 0;
            m[1][1] = 1;
            m[1][2] = 0;
            m[1][3] = 0;
            m[2][0] = -sinA;
            m[2][1] = 0;
            m[2][2] = cosA;
            m[2][3] = 0;
            break;
        case 'z':
        case 'Z':
            m[0][0] = cosA;
            m[0][1] = -sinA;
            m[0][2] = 0;
            m[0][3] = 0;
            m[1][0] = sinA;
            m[1][1] = cosA;
            m[1][2] = 0;
            m[1][3] = 0;
            m[2][0] = 0;
            m[2][1] = 0;
            m[2][2] = 1;
            m[2][3] = 0;
            break;
        default:
            ASSERTMSGLINE(1529, FALSE, "MTXRotTrig():  invalid 'axis' value ");
            break;
    }
}

void PSMTXRotTrig(__REGISTER Mtx m, __REGISTER char axis, __REGISTER f32 sinA, __REGISTER f32 cosA) {
    __REGISTER f32 fc0, fc1, nsinA;
    __REGISTER f32 fw0, fw1, fw2, fw3;

	asm {
		frsp        sinA, sinA
		frsp        cosA, cosA
	}

    fc0 = 0.0f;
    fc1 = 1.0f;

	asm {
		ori         axis, axis, 0x20
		ps_neg      nsinA, sinA
		cmplwi      axis, 'x'
		beq         _case_x
		cmplwi      axis, 'y'
		beq         _case_y
		cmplwi      axis, 'z'
		beq         _case_z
		b           _end

	_case_x:
		psq_st      fc1,  0(m), 1, 0
		psq_st      fc0,  4(m), 0, 0
		ps_merge00  fw0, sinA, cosA
		psq_st      fc0, 12(m), 0, 0
		ps_merge00  fw1, cosA, nsinA
		psq_st      fc0, 28(m), 0, 0
		psq_st      fc0, 44(m), 1, 0
		psq_st      fw0, 36(m), 0, 0
		psq_st      fw1, 20(m), 0, 0
		b           _end;

	_case_y:
		ps_merge00  fw0, cosA, fc0
		ps_merge00  fw1, fc0, fc1
		psq_st      fc0, 24(m), 0, 0
		psq_st      fw0,  0(m), 0, 0
		ps_merge00  fw2, nsinA, fc0
		ps_merge00  fw3, sinA, fc0
		psq_st      fw0, 40(m), 0, 0;
		psq_st      fw1, 16(m), 0, 0;
		psq_st      fw3,  8(m), 0, 0;
		psq_st      fw2, 32(m), 0, 0;
		b           _end;

	_case_z:
		psq_st      fc0,  8(m), 0, 0
		ps_merge00  fw0, sinA, cosA
		ps_merge00  fw2, cosA, nsinA
		psq_st      fc0, 24(m), 0, 0
		psq_st      fc0, 32(m), 0, 0
		ps_merge00  fw1, fc1, fc0
		psq_st      fw0, 16(m), 0, 0
		psq_st      fw2,  0(m), 0, 0
		psq_st      fw1, 40(m), 0, 0

	_end:
	}
}

static void __PSMTXRotAxisRadInternal(__REGISTER Mtx m, const __REGISTER Vec* axis, __REGISTER f32 sT, __REGISTER f32 cT) {
    __REGISTER f32 tT, fc0;
    __REGISTER f32 tmp0, tmp1, tmp2, tmp3, tmp4;
    __REGISTER f32 tmp5, tmp6, tmp7, tmp8, tmp9;
    tmp9 = 0.5f;
    tmp8 = 3.0f;

    asm {
        frsp cT, cT;
        psq_l tmp0, 0(axis), 0, 0;
        frsp sT, sT;
        lfs tmp1, 8(axis);
        ps_mul tmp2, tmp0, tmp0;
        fadds tmp7, tmp9, tmp9;
        ps_madd tmp3, tmp1, tmp1, tmp2;
        fsubs fc0, tmp9, tmp9;
        ps_sum0 tmp4, tmp3, tmp1, tmp2;
        fsubs tT, tmp7, cT;
        frsqrte tmp5, tmp4;
        fmuls tmp2, tmp5, tmp5;
        fmuls tmp3, tmp5, tmp9;
        fnmsubs tmp2, tmp2, tmp4, tmp8;
        fmuls tmp5, tmp2, tmp3;
        ps_merge00  cT, cT, cT;
        ps_muls0 tmp0, tmp0, tmp5;
        ps_muls0 tmp1, tmp1, tmp5;
        ps_muls0 tmp4, tmp0, tT;
        ps_muls0 tmp9, tmp0, sT;
        ps_muls0 tmp5, tmp1, tT;
        ps_muls1 tmp3, tmp4, tmp0;
        ps_muls0 tmp2, tmp4, tmp0;
        ps_muls0 tmp4, tmp4, tmp1;
        fnmsubs tmp6, tmp1, sT, tmp3;
        fmadds tmp7, tmp1, sT, tmp3;
        ps_neg tmp0, tmp9;
        ps_sum0 tmp8, tmp4, fc0, tmp9;
        ps_sum0 tmp2, tmp2, tmp6, cT;
        ps_sum1 tmp3, cT, tmp7, tmp3;
        ps_sum0 tmp6, tmp0, fc0, tmp4;
        psq_st tmp8, 8(m), 0, 0;
        ps_sum0 tmp0, tmp4, tmp4, tmp0;
        psq_st tmp2, 0(m), 0, 0;
        ps_muls0 tmp5, tmp5, tmp1;
        psq_st tmp3, 16(m), 0, 0;
        ps_sum1 tmp4, tmp9, tmp0, tmp4;
        psq_st tmp6, 24(m), 0, 0;
        ps_sum0 tmp5, tmp5, fc0, cT;
        psq_st tmp4, 32(m), 0, 0;
        psq_st tmp5, 40(m), 0, 0;
    }
}

void PSMTXRotAxisRad(Mtx m, const Vec* axis, f32 rad) {
    f32 sinT, cosT;

    sinT = sinf(rad);
    cosT = cosf(rad);

    __PSMTXRotAxisRadInternal(m, axis, sinT, cosT);
}

void C_MTXRotAxisRad(Mtx m, const Vec* axis, f32 rad) {
    Vec vN;
    f32 s;
    f32 c;
    f32 t;
    f32 x;
    f32 y;
    f32 z;
    f32 xSq;
    f32 ySq;
    f32 zSq;

    ASSERTMSGLINE(1677, m, "MTXRotAxisRad():  NULL MtxPtr 'm' ");
    ASSERTMSGLINE(1678, axis, "MTXRotAxisRad():  NULL VecPtr 'axis' ");

    s = sinf(rad);
    c = cosf(rad);
    t = 1 - c;
    C_VECNormalize(axis, &vN);
    x = vN.x;
    y = vN.y;
    z = vN.z;
    xSq = (x * x);
    ySq = (y * y);
    zSq = (z * z);
    m[0][0] = (c + (t * xSq));
    m[0][1] = (y * (t * x)) - (s * z);
    m[0][2] = (z * (t * x)) + (s * y);
    m[0][3] = 0;
    m[1][0] = ((y * (t * x)) + (s * z));
    m[1][1] = (c + (t * ySq));
    m[1][2] = ((z * (t * y)) - (s * x));
    m[1][3] = 0;
    m[2][0] = ((z * (t * x)) - (s * y));
    m[2][1] = ((z * (t * y)) + (s * x));
    m[2][2] = (c + (t * zSq));
    m[2][3] = 0;
}

void C_MTXTrans(Mtx m, f32 xT, f32 yT, f32 zT) {
    ASSERTMSGLINE(1866, m, "MTXTrans():  NULL MtxPtr 'm' ");
    m[0][0] = 1;
    m[0][1] = 0;
    m[0][2] = 0;
    m[0][3] = xT;
    m[1][0] = 0;
    m[1][1] = 1;
    m[1][2] = 0;
    m[1][3] = yT;
    m[2][0] = 0;
    m[2][1] = 0;
    m[2][2] = 1;
    m[2][3] = zT;
}

void PSMTXTrans(__REGISTER Mtx m, __REGISTER f32 xT, __REGISTER f32 yT, __REGISTER f32 zT) {
    __REGISTER f32 c0 = 0.0f;
    __REGISTER f32 c1 = 1.0f;

	asm {
		stfs xT, 12(m)
		stfs yT, 28(m)
		psq_st c0, 4(m), 0, 0
		psq_st c0, 32(m), 0, 0
		stfs c0, 16(m)
		stfs c1, 20(m)
		stfs c0, 24(m)
		stfs c1, 40(m)
		stfs zT, 44(m)
		stfs c1, 0(m)
	}
}

void C_MTXTransApply(const Mtx src, Mtx dst, f32 xT, f32 yT, f32 zT) {
    ASSERTMSGLINE(1933, src, "MTXTransApply(): NULL MtxPtr 'src' ");
    ASSERTMSGLINE(1934, dst, "MTXTransApply(): NULL MtxPtr 'src' "); //! wrong assert string

    if (src != dst) {
        dst[0][0] = src[0][0];
        dst[0][1] = src[0][1];
        dst[0][2] = src[0][2];
        dst[1][0] = src[1][0];
        dst[1][1] = src[1][1];
        dst[1][2] = src[1][2];
        dst[2][0] = src[2][0];
        dst[2][1] = src[2][1];
        dst[2][2] = src[2][2];
    }

    dst[0][3] = (src[0][3] + xT);
    dst[1][3] = (src[1][3] + yT);
    dst[2][3] = (src[2][3] + zT);
}

asm void PSMTXTransApply(const __REGISTER Mtx src, __REGISTER Mtx dst, __REGISTER f32 xT, __REGISTER f32 yT, __REGISTER f32 zT) {
    nofralloc
    psq_l fp4, 0(src), 0, 0
    frsp xT, xT
    psq_l fp5, 8(src), 0, 0
    frsp yT, yT
    psq_l fp7, 24(src), 0, 0
    frsp zT, zT
    psq_l fp8, 40(src), 0, 0
    psq_st fp4, 0(dst),  0, 0
    ps_sum1 fp5, xT, fp5, fp5
    psq_l fp6, 16(src), 0, 0
    psq_st fp5, 8(dst), 0, 0
    ps_sum1 fp7, yT, fp7, fp7
    psq_l fp9, 32(src), 0, 0
    psq_st fp6, 16(dst), 0, 0
    ps_sum1 fp8, zT, fp8, fp8
    psq_st fp7, 24(dst), 0, 0
    psq_st fp9, 32(dst), 0, 0
    psq_st fp8, 40(dst), 0, 0
    blr
}

void C_MTXScale(Mtx m, f32 xS, f32 yS, f32 zS) {
    ASSERTMSGLINE(2008, m, "MTXScale():  NULL MtxPtr 'm' ");
    m[0][0] = xS;
    m[0][1] = 0;
    m[0][2] = 0;
    m[0][3] = 0;
    m[1][0] = 0;
    m[1][1] = yS;
    m[1][2] = 0;
    m[1][3] = 0;
    m[2][0] = 0;
    m[2][1] = 0;
    m[2][2] = zS;
    m[2][3] = 0;
}

void PSMTXScale(__REGISTER Mtx m, __REGISTER f32 xS, __REGISTER f32 yS, __REGISTER f32 zS) {
    __REGISTER f32 c0 = 0.0f;

	asm {
		stfs xS, 0(m)
		psq_st c0, 4(m), 0, 0
		psq_st c0, 12(m), 0, 0
		stfs yS, 20(m)
		psq_st c0, 24(m), 0, 0
		psq_st c0, 32(m), 0, 0
		stfs zS, 40(m)
		stfs c0, 44(m)
	}
}

void C_MTXScaleApply(const Mtx src, Mtx dst, f32 xS, f32 yS, f32 zS) {
    ASSERTMSGLINE(2070, src, "MTXScaleApply(): NULL MtxPtr 'src' ");
    ASSERTMSGLINE(2071, dst, "MTXScaleApply(): NULL MtxPtr 'dst' ");
    dst[0][0] = (src[0][0] * xS);
    dst[0][1] = (src[0][1] * xS);
    dst[0][2] = (src[0][2] * xS);
    dst[0][3] = (src[0][3] * xS);
    dst[1][0] = (src[1][0] * yS);
    dst[1][1] = (src[1][1] * yS);
    dst[1][2] = (src[1][2] * yS);
    dst[1][3] = (src[1][3] * yS);
    dst[2][0] = (src[2][0] * zS);
    dst[2][1] = (src[2][1] * zS);
    dst[2][2] = (src[2][2] * zS);
    dst[2][3] = (src[2][3] * zS);
}

asm void PSMTXScaleApply(const __REGISTER Mtx src, __REGISTER Mtx dst, __REGISTER f32 xS, __REGISTER f32 yS, __REGISTER f32 zS) {
    nofralloc
    frsp xS, xS
    psq_l fp4, 0(src), 0, 0
    frsp yS, yS
    psq_l fp5, 8(src), 0, 0
    frsp zS, zS
    ps_muls0 fp4, fp4, xS
    psq_l fp6, 16(src), 0, 0
    ps_muls0 fp5, fp5, xS
    psq_l fp7, 24(src), 0, 0
    ps_muls0 fp6, fp6, yS
    psq_l fp8, 32(src), 0, 0
    psq_st fp4, 0(dst), 0, 0
    ps_muls0 fp7, fp7, yS
    psq_l fp2, 40(src), 0, 0
    psq_st fp5, 8(dst), 0, 0
    ps_muls0 fp8, fp8, zS
    psq_st fp6, 16(dst), 0, 0
    ps_muls0 fp2, fp2, zS
    psq_st fp7, 24(dst), 0, 0
    psq_st fp8, 32(dst), 0, 0
    psq_st fp2, 40(dst), 0, 0
    blr
}

void C_MTXQuat(Mtx m, const Quaternion* q) {
    f32 s;
    f32 xs;
    f32 ys;
    f32 zs;
    f32 wx;
    f32 wy;
    f32 wz;
    f32 xx;
    f32 xy;
    f32 xz;
    f32 yy;
    f32 yz;
    f32 zz;

    ASSERTMSGLINE(2145, m, "MTXQuat():  NULL MtxPtr 'm' ");
    ASSERTMSGLINE(2146, q, "MTXQuat():  NULL QuaternionPtr 'q' ");
    ASSERTMSGLINE(2147, q->x || q->y || q->z || q->w, "MTXQuat():  zero-value quaternion ");
    s = 2 / ((q->w * q->w) + ((q->z * q->z) + ((q->x * q->x) + (q->y * q->y))));
    xs = q->x * s;
    ys = q->y * s;
    zs = q->z * s;
    wx = q->w * xs;
    wy = q->w * ys;
    wz = q->w * zs;
    xx = q->x * xs;
    xy = q->x * ys;
    xz = q->x * zs;
    yy = q->y * ys;
    yz = q->y * zs;
    zz = q->z * zs;
    m[0][0] = (1 - (yy + zz));
    m[0][1] = (xy - wz);
    m[0][2] = (xz + wy);
    m[0][3] = 0;
    m[1][0] = (xy + wz);
    m[1][1] = (1 - (xx + zz));
    m[1][2] = (yz - wx);
    m[1][3] = 0;
    m[2][0] = (xz - wy);
    m[2][1] = (yz + wx);
    m[2][2] = (1 - (xx + yy));
    m[2][3] = 0;
}

void PSMTXQuat(__REGISTER Mtx m, const __REGISTER Quaternion* q) {
    __REGISTER f32 c_zero, c_one, c_two, scale;
    __REGISTER f32 tmp0, tmp1, tmp2, tmp3, tmp4;
    __REGISTER f32 tmp5, tmp6, tmp7, tmp8, tmp9;
    c_one = 1.0f;

	asm {
        psq_l tmp0, 0(q), 0, 0
        psq_l tmp1, 8(q), 0, 0
        fsubs c_zero, c_one, c_one
        fadds c_two, c_one, c_one
        ps_mul tmp2, tmp0, tmp0
        ps_merge10 tmp5, tmp0, tmp0
        ps_madd tmp4, tmp1, tmp1, tmp2
        ps_mul tmp3, tmp1, tmp1
        ps_sum0 scale, tmp4, tmp4, tmp4
        ps_muls1 tmp7, tmp5, tmp1
        fres tmp9, scale
        ps_sum1 tmp4, tmp3, tmp4, tmp2
        ps_nmsub scale, scale, tmp9, c_two
        ps_muls1 tmp6, tmp1, tmp1
        ps_mul scale, tmp9, scale
        ps_sum0 tmp2, tmp2, tmp2, tmp2
        fmuls scale, scale, c_two
        ps_madd tmp8, tmp0, tmp5, tmp6
        ps_msub tmp6, tmp0, tmp5, tmp6
        psq_st c_zero, 12(m), 1, 0
        ps_nmsub tmp2, tmp2, scale, c_one
        ps_nmsub tmp4, tmp4, scale, c_one
        psq_st c_zero, 44(m), 1, 0
        ps_mul tmp8, tmp8, scale
        ps_mul tmp6, tmp6, scale
        psq_st tmp2, 40(m), 1, 0
        ps_madds0 tmp5, tmp0, tmp1, tmp7
        ps_merge00 tmp1, tmp8, tmp4
        ps_nmsub tmp7, tmp7, c_two, tmp5
        ps_merge10 tmp0, tmp4, tmp6
        psq_st tmp1, 16(m), 0, 0
        ps_mul tmp5, tmp5, scale
        ps_mul tmp7, tmp7, scale
        psq_st tmp0, 0(m), 0, 0
        psq_st tmp5, 8(m), 1, 0
        ps_merge10 tmp3, tmp7, c_zero
        ps_merge01 tmp9, tmp7, tmp5
        psq_st tmp3, 24(m), 0, 0
        psq_st tmp9, 32(m), 0, 0
	}
}

void C_MTXReflect(Mtx m, const Vec* p, const Vec* n) {
    f32 vxy;
    f32 vxz;
    f32 vyz;
    f32 pdotn;

    vxy = -2 * n->x * n->y;
    vxz = -2 * n->x * n->z;
    vyz = -2 * n->y * n->z;
    pdotn = 2 * C_VECDotProduct(p, n);
    m[0][0] = (1 - (2 * n->x * n->x));
    m[0][1] = vxy;
    m[0][2] = vxz;
    m[0][3] = (pdotn * n->x);
    m[1][0] = vxy;
    m[1][1] = (1 - (2 * n->y * n->y));
    m[1][2] = vyz;
    m[1][3] = (pdotn * n->y);
    m[2][0] = vxz;
    m[2][1] = vyz;
    m[2][2] = (1 - (2 * n->z * n->z));
    m[2][3] = (pdotn * n->z);
}

void PSMTXReflect(__REGISTER Mtx m, const __REGISTER Vec* p, const __REGISTER Vec* n) {
    __REGISTER f32 c_one;
    __REGISTER f32 vn_xy, vn_z1;
    __REGISTER f32 n2vn_xy, n2vn_z1;
    __REGISTER f32 pdotn;
    __REGISTER f32 tmp0, tmp1, tmp2, tmp3;
    __REGISTER f32 tmp4, tmp5, tmp6, tmp7;

    c_one = 1.0f;

    asm {
        psq_l vn_z1, 0x8(n), 1, 0
        psq_l vn_xy, 0x0(n), 0, 0
        psq_l tmp0, 0x0(p), 0, 0
        ps_nmadd n2vn_z1, vn_z1, c_one, vn_z1
        psq_l tmp1, 0x8(p), 1, 0
        ps_nmadd n2vn_xy, vn_xy, c_one, vn_xy
        ps_muls0 tmp4, vn_xy, n2vn_z1
        ps_mul pdotn, n2vn_xy, tmp0
        ps_muls0 tmp2, vn_xy, n2vn_xy
        ps_sum0 pdotn, pdotn, pdotn, pdotn
        ps_muls1 tmp3, vn_xy, n2vn_xy
        psq_st tmp4, 0x20(m), 0, 0
        ps_sum0 tmp2, tmp2, tmp2, c_one
        ps_nmadd pdotn, n2vn_z1, tmp1, pdotn
        ps_sum1 tmp3, c_one, tmp3, tmp3
        psq_st tmp2, 0x0(m), 0, 0
        ps_muls0 tmp5, vn_xy, pdotn
        ps_merge00 tmp6, n2vn_z1, pdotn
        psq_st tmp3, 0x10(m), 0, 0
        ps_merge00 tmp7, tmp4, tmp5
        ps_muls0 tmp6, tmp6, vn_z1
        ps_merge11 tmp5, tmp4, tmp5
        psq_st tmp7, 0x8(m), 0, 0
        ps_sum0 tmp6, tmp6, tmp6, c_one
        psq_st tmp5, 0x18(m), 0, 0
        psq_st tmp6, 0x28(m), 0, 0
    }
}

void C_MTXLookAt(Mtx m, const Point3d* camPos, const Vec* camUp, const Point3d* target) {
    Vec vLook;
    Vec vRight;
    Vec vUp;

    ASSERTMSGLINE(2438, m, "MTXLookAt():  NULL MtxPtr 'm' ");
    ASSERTMSGLINE(2439, camPos, "MTXLookAt():  NULL VecPtr 'camPos' ");
    ASSERTMSGLINE(2440, camUp, "MTXLookAt():  NULL VecPtr 'camUp' ");
    ASSERTMSGLINE(2441, target, "MTXLookAt():  NULL Point3dPtr 'target' ");

    vLook.x = camPos->x - target->x;
    vLook.y = camPos->y - target->y;
    vLook.z = camPos->z - target->z;
    VECNormalize(&vLook, &vLook);
    VECCrossProduct(camUp, &vLook, &vRight);
    VECNormalize(&vRight, &vRight);
    VECCrossProduct(&vLook, &vRight, &vUp);
    m[0][0] = vRight.x;
    m[0][1] = vRight.y;
    m[0][2] = vRight.z;
    m[0][3] = -((camPos->z * vRight.z) + ((camPos->x * vRight.x) + (camPos->y * vRight.y)));
    m[1][0] = vUp.x;
    m[1][1] = vUp.y;
    m[1][2] = vUp.z;
    m[1][3] = -((camPos->z * vUp.z) + ((camPos->x * vUp.x) + (camPos->y * vUp.y)));
    m[2][0] = vLook.x;
    m[2][1] = vLook.y;
    m[2][2] = vLook.z;
    m[2][3] = -((camPos->z * vLook.z) + ((camPos->x * vLook.x) + (camPos->y * vLook.y)));
}

void C_MTXLightFrustum(Mtx m, f32 t, f32 b, f32 l, f32 r, f32 n, f32 scaleS, f32 scaleT, f32 transS, f32 transT) {
    f32 tmp;

    ASSERTMSGLINE(2541, m, "MTXLightFrustum():  NULL MtxPtr 'm' ");
    ASSERTMSGLINE(2542, (t != b), "MTXLightFrustum():  't' and 'b' clipping planes are equal ");
    ASSERTMSGLINE(2543, (l != r), "MTXLightFrustum():  'l' and 'r' clipping planes are equal ");

    tmp = 1 / (r - l);
    m[0][0] = (scaleS * (2 * n * tmp));
    m[0][1] = 0;
    m[0][2] = (scaleS * (tmp * (r + l))) - transS;
    m[0][3] = 0;
    tmp = 1 / (t - b);
    m[1][0] = 0;
    m[1][1] = (scaleT * (2 * n * tmp));
    m[1][2] = (scaleT * (tmp * (t + b))) - transT;
    m[1][3] = 0;
    m[2][0] = 0;
    m[2][1] = 0;
    m[2][2] = -1;
    m[2][3] = 0;
}

void C_MTXLightPerspective(Mtx m, f32 fovY, f32 aspect, f32 scaleS, f32 scaleT, f32 transS, f32 transT) {
    f32 angle;
    f32 cot;

    ASSERTMSGLINE(2605, m, "MTXLightPerspective():  NULL MtxPtr 'm' ");
    ASSERTMSGLINE(2606, (fovY > 0.0) && (fovY < 180.0), "MTXLightPerspective():  'fovY' out of range ");
    ASSERTMSGLINE(2607, 0 != aspect, "MTXLightPerspective():  'aspect' is 0 ");

    angle = (0.5f * fovY);
    angle = MTXDegToRad(angle);
    cot = 1 / tanf(angle);
    m[0][0] = (scaleS * (cot / aspect));
    m[0][1] = 0;
    m[0][2] = -transS;
    m[0][3] = 0;
    m[1][0] = 0;
    m[1][1] = (cot * scaleT);
    m[1][2] = -transT;
    m[1][3] = 0;
    m[2][0] = 0;
    m[2][1] = 0;
    m[2][2] = -1;
    m[2][3] = 0;
}

void C_MTXLightOrtho(Mtx m, f32 t, f32 b, f32 l, f32 r, f32 scaleS, f32 scaleT, f32 transS, f32 transT) {
    f32 tmp;

    ASSERTMSGLINE(2673, m, "MTXLightOrtho():  NULL MtxPtr 'm' ");
    ASSERTMSGLINE(2674, (t != b), "MTXLightOrtho():  't' and 'b' clipping planes are equal ");
    ASSERTMSGLINE(2675, (l != r), "MTXLightOrtho():  'l' and 'r' clipping planes are equal ");
    tmp = 1 / (r - l);
    m[0][0] = (2 * tmp * scaleS);
    m[0][1] = 0;
    m[0][2] = 0;
    m[0][3] = (transS + (scaleS * (tmp * -(r + l))));
    tmp = 1/ (t - b);
    m[1][0] = 0;
    m[1][1] = (2 * tmp * scaleT);
    m[1][2] = 0;
    m[1][3] = (transT + (scaleT * (tmp * -(t + b))));
    m[2][0] = 0;
    m[2][1] = 0;
    m[2][2] = 0;
    m[2][3] = 1;
}
