#include <revolution.h>
#include <revolution/mtx.h>
#include <sdk_math.h>

void C_QUATAdd(const Quaternion* p, const Quaternion* q, Quaternion* r) {
    ASSERTMSGLINE(77, p, "QUATAdd():  NULL QuaternionPtr 'p' ");
    ASSERTMSGLINE(78, q, "QUATAdd():  NULL QuaternionPtr 'q' ");
    ASSERTMSGLINE(79, r, "QUATAdd():  NULL QuaternionPtr 'r' ");

    r->x = p->x + q->x;
    r->y = p->y + q->y;
    r->z = p->z + q->z;
    r->w = p->w + q->w;
}

void PSQUATAdd(const __REGISTER Quaternion* p, const __REGISTER Quaternion* q, __REGISTER Quaternion* r) {
    __REGISTER f32 pxy, qxy, rxy, pzw, qzw, rzw;

    asm {
        psq_l pxy, 0(p), 0, 0
        psq_l qxy, 0(q), 0, 0
        ps_add rxy, pxy, qxy
        psq_st rxy, 0(r), 0, 0
        psq_l pzw, 8(p), 0, 0
        psq_l qzw, 8(q), 0, 0
        ps_add rzw, pzw, qzw
        psq_st rzw, 8(r), 0, 0
    }
}

void C_QUATSubtract(const Quaternion* p, const Quaternion* q, Quaternion* r) {
    ASSERTMSGLINE(133, p, "QUATSubtract():  NULL QuaternionPtr 'p' ");
    ASSERTMSGLINE(134, q, "QUATSubtract():  NULL QuaternionPtr 'q' ");
    ASSERTMSGLINE(135, r, "QUATSubtract():  NULL QuaternionPtr 'r' ");

    r->x = p->x - q->x;
    r->y = p->y - q->y;
    r->z = p->z - q->z;
    r->w = p->w - q->w;
}

void PSQUATSubtract(const __REGISTER Quaternion* p, const __REGISTER Quaternion* q, __REGISTER Quaternion* r) {
    __REGISTER f32 pxy, qxy, rxy, pzw, qzw, rzw;

    asm {
        psq_l pxy, 0(p), 0, 0
        psq_l qxy, 0(q), 0, 0
        ps_sub rxy, pxy, qxy
        psq_st rxy, 0(r), 0, 0
        psq_l pzw, 8(p), 0, 0
        psq_l qzw, 8(q), 0, 0
        ps_sub rzw, pzw, qzw
        psq_st rzw, 8(r), 0, 0
    }
}

void C_QUATMultiply(const Quaternion* p, const Quaternion* q, Quaternion* pq) {
    Quaternion* r;
    Quaternion pqTmp;

    ASSERTMSGLINE(193, p, "QUATMultiply():  NULL QuaternionPtr 'p' ");
    ASSERTMSGLINE(194, q, "QUATMultiply():  NULL QuaternionPtr 'q' ");
    ASSERTMSGLINE(195, pq, "QUATMultiply():  NULL QuaternionPtr 'pq' ");

    if (p == pq || q == pq){
        r = &pqTmp;
    } else {
        r = pq;
    }

    r->w = (p->w * q->w) - (p->x * q->x) - (p->y * q->y) - (p->z * q->z);
    r->x = (p->w * q->x) + (p->x * q->w) + (p->y * q->z) - (p->z * q->y);
    r->y = (p->w * q->y) + (p->y * q->w) + (p->z * q->x) - (p->x * q->z);
    r->z = (p->w * q->z) + (p->z * q->w) + (p->x * q->y) - (p->y * q->x);
    
    if (r == &pqTmp) {
        *pq = pqTmp;
    }
}

void PSQUATMultiply(const __REGISTER Quaternion* p, const __REGISTER Quaternion* q, __REGISTER Quaternion* pq) {
    __REGISTER f32 pxy, pzw;
    __REGISTER f32 qxy, qzw;
    __REGISTER f32 pnxy, pnzw, pnxny, pnznw;
    __REGISTER f32 rxy, rzw;
    __REGISTER f32 sxy, szw;
    
    asm {
        psq_l pxy, 0x0(p), 0, 0
        psq_l pzw, 0x8(p), 0, 0
        psq_l qxy, 0x0(q), 0, 0
        ps_neg pnxny, pxy
        psq_l qzw, 0x8(q), 0, 0
        ps_neg pnznw, pzw
        ps_merge01 pnxy, pnxny, pxy
        ps_muls0 rxy, pzw, qxy
        ps_muls0 rzw, pnxny, qxy
        ps_merge01 pnzw, pnznw, pzw
        ps_muls1 szw, pnxy, qxy
        ps_madds0 rxy, pnxy, qzw, rxy
        ps_muls1 sxy, pnzw, qxy
        ps_madds0 rzw, pnzw, qzw, rzw
        ps_madds1 szw, pnznw, qzw, szw
        ps_merge10 rxy, rxy, rxy
        ps_madds1 sxy, pxy, qzw, sxy
        ps_merge10 rzw, rzw, rzw
        ps_add rxy, rxy, sxy
        psq_st rxy, 0x0(pq), 0, 0
        ps_sub rzw, rzw, szw
        psq_st rzw, 0x8(pq), 0, 0
    }
}

void C_QUATScale(const Quaternion* q, Quaternion* r, f32 scale) {
    ASSERTMSGLINE(306, q, "QUATScale():  NULL QuaternionPtr 'q' ");
    ASSERTMSGLINE(307, r, "QUATScale():  NULL QuaternionPtr 'r' ");

    r->x = q->x * scale;
    r->y = q->y * scale;
    r->z = q->z * scale;
    r->w = q->w * scale;
}

void PSQUATScale(const __REGISTER Quaternion* q, __REGISTER Quaternion* r, __REGISTER f32 scale) {
    __REGISTER f32 rxy, rzw;
    
    asm {
        psq_l rxy, 0(q), 0, 0
        psq_l rzw, 8(q), 0, 0
        ps_muls0 rxy, rxy, scale
        psq_st rxy, 0(r), 0, 0
        ps_muls0 rzw, rzw, scale
        psq_st rzw, 8(r), 0, 0
    }
}

f32 C_QUATDotProduct(const Quaternion* p, const Quaternion* q) {
    ASSERTMSGLINE(357, p, "QUATDotProduct():  NULL QuaternionPtr 'p' ");
    ASSERTMSGLINE(358, q, "QUATDotProduct():  NULL QuaternionPtr 'q' ");

    return (q->x * p->x) + (q->y * p->y) + (q->z * p->z) + (q->w * p->w);
}

f32 PSQUATDotProduct(const __REGISTER Quaternion* p, const __REGISTER Quaternion* q) {
    __REGISTER f32 pxy, pzw, qxy, qzw, dp;

    asm {
        psq_l pxy, 0(p), 0, 0
        psq_l qxy, 0(q), 0, 0
        ps_mul dp, pxy, qxy
        psq_l pzw, 8(p), 0, 0
        psq_l qzw, 8(q), 0, 0
        ps_madd dp, pzw, qzw, dp
        ps_sum0 dp, dp, dp, dp
    }

    return dp;
}

void C_QUATNormalize(const Quaternion* src, Quaternion* unit) {
    f32 mag;
    ASSERTMSGLINE(407, src, "QUATNormalize():  NULL QuaternionPtr 'src' ");
    ASSERTMSGLINE(408, unit, "QUATNormalize():  NULL QuaternionPtr 'unit' ");

    mag = (src->x * src->x) + (src->y * src->y) + (src->z * src->z) + (src->w * src->w);
    if (mag >= 0.00001f) {
        mag = 1.0f / sqrtf(mag);
    
        unit->x = src->x * mag;
        unit->y = src->y * mag;
        unit->z = src->z * mag;
        unit->w = src->w * mag;
    } else {
        unit->x = unit->y = unit->z = unit->w = 0.0f;    
    }
}

void PSQUATNormalize(const __REGISTER Quaternion* src, __REGISTER Quaternion* unit) {
    __REGISTER f32 sxy, szw;
    __REGISTER f32 mag, rsqmag;
    __REGISTER f32 diff;
    __REGISTER f32 c_zero;
    __REGISTER f32 nwork0, nwork1;

    __REGISTER f32 epsilon = 0.00001f;
    __REGISTER f32 c_half = 0.5f;
    __REGISTER f32 c_three = 3.0f;

    asm {
        psq_l sxy, 0x0(src), 0, 0
        ps_mul mag, sxy, sxy
        psq_l szw, 0x8(src), 0, 0
        ps_sub c_zero, epsilon, epsilon
        ps_madd mag, szw, szw, mag
        ps_sum0 mag, mag, mag, mag
        frsqrte rsqmag, mag
        ps_sub diff, mag, epsilon
        fmul nwork0, rsqmag, rsqmag
        fmul nwork1, rsqmag, c_half
        fnmsub nwork0, nwork0, mag, c_three
        fmul rsqmag, nwork0, nwork1
        ps_sel rsqmag, diff, rsqmag, c_zero
        ps_muls0 sxy, sxy, rsqmag
        ps_muls0 szw, szw, rsqmag
        psq_st sxy, 0x0(unit), 0, 0
        psq_st szw, 0x8(unit), 0, 0
    }
}

void C_QUATInverse(const Quaternion* src, Quaternion* inv) {
    f32 mag, norminv;
    ASSERTMSGLINE(498, src, "QUATInverse():  NULL QuaternionPtr 'src' ");
    ASSERTMSGLINE(499, inv, "QUATInverse():  NULL QuaternionPtr 'inv' ");

    mag = (src->x * src->x) + (src->y * src->y) + (src->z * src->z) + (src->w * src->w);
    if (mag == 0.0f) {
        mag = 1.0f;
    }

    norminv = 1.0f / mag;
    inv->x = -src->x * norminv;
    inv->y = -src->y * norminv;
    inv->z = -src->z * norminv;
    inv->w =  src->w * norminv;
}

void PSQUATInverse(const __REGISTER Quaternion* src, __REGISTER Quaternion* inv) {
    __REGISTER f32 sxy, szw;
    __REGISTER f32 izz, iww;
    __REGISTER f32 mag, nmag;
    __REGISTER f32 norminv, nninv;
    __REGISTER f32 nwork0;
    __REGISTER f32 c_two;
    __REGISTER f32 c_zero;
    __REGISTER f32 c_one = 1.0f;

    asm {
        psq_l sxy, 0x0(src), 0, 0
        ps_mul mag, sxy, sxy
        ps_sub c_zero, c_one, c_one
        psq_l szw, 0x8(src), 0, 0
        ps_madd mag, szw, szw, mag
        ps_add c_two, c_one, c_one
        ps_sum0 mag, mag, mag, mag
        fcmpu cr0, mag, c_zero
        beq L_00000948
        fres norminv, mag
        ps_neg nmag, mag
        ps_nmsub nwork0, mag, norminv, c_two
        ps_mul norminv, norminv, nwork0
        b L_0000094C
    L_00000948:
        fmr norminv, c_one
    L_0000094C:
        ps_neg nninv, norminv
        ps_muls1 iww, norminv, szw
        ps_muls0 sxy, sxy, nninv
        psq_st iww, 0xc(inv), 1, 0
        ps_muls0 izz, szw, nninv
        psq_st sxy, 0x0(inv), 0, 0
        psq_st izz, 0x8(inv), 1, 0
    }
}

void C_QUATDivide(const Quaternion* p, const Quaternion* q, Quaternion* r) {
    Quaternion qtmp;
    ASSERTMSGLINE(606, p, "QUATDivide():  NULL QuaternionPtr 'p' ");
    ASSERTMSGLINE(607, q, "QUATDivide():  NULL QuaternionPtr 'q' ");
    ASSERTMSGLINE(608, r, "QUATDivide():  NULL QuaternionPtr 'r' ");

    C_QUATInverse(q, &qtmp);
    C_QUATMultiply(&qtmp, p, r);
}

void PSQUATDivide(const Quaternion* p, const Quaternion* q, Quaternion* r) {
    Quaternion qtmp;

    PSQUATInverse(q, &qtmp);
    PSQUATMultiply(&qtmp, p, r);
}

void C_QUATExp(const Quaternion* q, Quaternion* r)  {
    f32 theta, scale;
    ASSERTMSGLINE(643, q, "QUATExp():  NULL QuaternionPtr 'q' ");
    ASSERTMSGLINE(644, r, "QUATExp():  NULL QuaternionPtr 'r' ");
    ASSERTMSGLINE(647, q->w == 0.0f, "QUATExp():  'q' is not a pure quaternion. ");

    theta = sqrtf((q->x * q->x) + (q->y * q->y) + (q->z * q->z));
    scale = 1.0f;

    if (theta > 0.00001f) {
        scale = sinf(theta) / theta;
    }
    
    r->x = scale * q->x;
    r->y = scale * q->y;
    r->z = scale * q->z;
    r->w = cosf(theta);
}

void C_QUATLogN(const Quaternion* q, Quaternion* r) {
    f32 theta, scale, mag;
    ASSERTMSGLINE(676, q, "QUATLogN():  NULL QuaternionPtr 'q' ");
    ASSERTMSGLINE(677, r, "QUATLogN():  NULL QuaternionPtr 'r' ");

    scale = (q->x * q->x) + (q->y * q->y) + (q->z * q->z);

#if DEBUG
    mag = scale + (q->z * q->z);
    if (mag < 1.0f - 0.00001f || mag > 1.0f + 0.00001f || mag > 1.00001f) {}
#endif

    scale = sqrtf(scale);
    theta = atan2f(scale, q->w);

    if (scale > 0.0f) {
        scale = theta / scale;
    }

    r->x = scale * q->x;
    r->y = scale * q->y;
    r->z = scale * q->z;
    r->w = 0.0f;
}

void C_QUATMakeClosest(const Quaternion* q, const Quaternion* qto, Quaternion* r) {
    f32 dot;
    ASSERTMSGLINE(722, q, "QUATMakeClosest():  NULL QuaternionPtr 'q' ");
    ASSERTMSGLINE(723, qto, "QUATMakeClosest():  NULL QuaternionPtr 'qto' ");
    ASSERTMSGLINE(724, r, "QUATMakeClosest():  NULL QuaternionPtr 'r' ");
    
    dot = (q->x * qto->x) + (q->y * qto->y) + (q->z * qto->z) + (q->w * qto->w);
    if (dot < 0.0f) {
        r->x = -q->x;
        r->y = -q->y;
        r->z = -q->z;
        r->w = -q->w;
    } else {
        *r = *q;
    }
}

void C_QUATRotAxisRad(Quaternion* r, const Vec* axis, f32 rad) {
    f32 half, sh, ch;
    Vec nAxis;

    ASSERTMSGLINE(763, r, "QUATRotAxisRad():  NULL QuaternionPtr 'r' ");
    ASSERTMSGLINE(764, axis, "QUATRotAxisRad():  NULL VecPtr 'axis' ");

    VECNormalize(axis, &nAxis);

    half = rad * 0.5f;
    sh = sinf(half);
    ch = cosf(half);

    r->x = sh * nAxis.x;
    r->y = sh * nAxis.y;
    r->z = sh * nAxis.z;
    r->w = ch;
}

void C_QUATMtx(Quaternion* r, const Mtx m) {
    f32 tr,s;
    s32 i, j, k;
    s32 nxt[3] = {1, 2, 0};
    f32 q[3];

    ASSERTMSGLINE(791, r, "QUATMtx():  NULL QuaternionPtr 'r' ");
    ASSERTMSGLINE(792, m, "QUATMtx():  NULL MtxPtr 'm' ");

    tr = m[0][0] + m[1][1] + m[2][2];
    if (tr > 0.0f) {
        s = sqrtf(tr + 1.0f);
        r->w = s * 0.5f;
        s = 0.5f / s;

        r->x = (m[2][1] - m[1][2]) * s;
        r->y = (m[0][2] - m[2][0]) * s;
        r->z = (m[1][0] - m[0][1]) * s;
    } else  {
        i = 0;
        if (m[1][1] > m[0][0]) {
            i = 1;
        }

        if (m[2][2] > m[i][i]) {
            i = 2;
        }

        j = nxt[i];
        k = nxt[j];

        s = sqrtf((m[i][i] - (m[j][j] + m[k][k])) + 1.0f);
        q[i] = s * 0.5f;
        
        if (s != 0.0f) {
            s = 0.5f / s;
        }
        
        r->w = (m[k][j] - m[j][k]) * s;
        q[j] = (m[i][j] + m[j][i]) * s;
        q[k] = (m[i][k] + m[k][i]) * s;

        r->x = q[0];
        r->y = q[1];
        r->z = q[2];
    }
}

void C_QUATLerp(const Quaternion* p, const Quaternion* q, Quaternion* r, f32 t) {
    ASSERTMSGLINE(842, p, "QUATLerp():  NULL QuaternionPtr 'p' ");
    ASSERTMSGLINE(843, q, "QUATLerp():  NULL QuaternionPtr 'q' ");
    ASSERTMSGLINE(844, r, "QUATLerp():  NULL QuaternionPtr 'r' ");

    r->x = t * (q->x - p->x) + p->x;
    r->y = t * (q->y - p->y) + p->y;
    r->z = t * (q->z - p->z) + p->z;
    r->w = t * (q->w - p->w) + p->w;
}

void C_QUATSlerp(const Quaternion* p, const Quaternion* q, Quaternion* r, f32 t) {
    f32 theta, sin_th, cos_th;
    f32 tp, tq;

    ASSERTMSGLINE(874, p, "QUATSlerp():  NULL QuaternionPtr 'p' ");
    ASSERTMSGLINE(875, q, "QUATSlerp():  NULL QuaternionPtr 'q' ");
    ASSERTMSGLINE(876, r, "QUATSlerp():  NULL QuaternionPtr 'r' ");
    
    cos_th = p->x * q->x + p->y * q->y + p->z * q->z + p->w * q->w;
    tq = 1.0f;

    if (cos_th < 0.0f) {
        cos_th = -cos_th;
        tq = -tq;
    }

    if (cos_th <= 0.99999f) {
        theta = acosf(cos_th);
        sin_th = sinf(theta);

        tp = sinf((1.0f - t) * theta) / sin_th;
        tq *= sinf(t * theta) / sin_th;
    } else {
        tp = 1.0f - t;
        tq *= t;
    }

    r->x = (tp * p->x) + (tq * q->x);
    r->y = (tp * p->y) + (tq * q->y);
    r->z = (tp * p->z) + (tq * q->z);
    r->w = (tp * p->w) + (tq * q->w);
}

void C_QUATSquad(const Quaternion* p, const Quaternion* a, const Quaternion* b, const Quaternion* q, Quaternion* r, f32 t) {
    Quaternion pq, ab;
    f32 t2;

    ASSERTMSGLINE(927, p, "QUATSquad():  NULL QuaternionPtr 'p' ");
    ASSERTMSGLINE(928, a, "QUATSquad():  NULL QuaternionPtr 'a' ");
    ASSERTMSGLINE(929, b, "QUATSquad():  NULL QuaternionPtr 'b' ");
    ASSERTMSGLINE(930, q, "QUATSquad():  NULL QuaternionPtr 'q' ");
    ASSERTMSGLINE(931, r, "QUATSquad():  NULL QuaternionPtr 'r' ");

    t2 = 2.0f * t * (1.0f - t);
    C_QUATSlerp(p, q, &pq, t);
    C_QUATSlerp(a, b, &ab, t);
    C_QUATSlerp(&pq, &ab, r, t2);
}

void C_QUATCompA(const Quaternion* qprev, const Quaternion* q, const Quaternion* qnext, Quaternion* a) {
    Quaternion qm, qp, lqm, lqp, qpqm, exq;

    ASSERTMSGLINE(958, qprev, "QUATCompA():  NULL QuaternionPtr 'qprev' ");
    ASSERTMSGLINE(959, q, "QUATCompA():  NULL QuaternionPtr 'q' ");
    ASSERTMSGLINE(960, qnext, "QUATCompA():  NULL QuaternionPtr 'qnext' ");
    ASSERTMSGLINE(961, a, "QUATCompA():  NULL QuaternionPtr 'a' ");

    C_QUATDivide(qprev, q, &qm);
    C_QUATLogN(&qm, &lqm);
    C_QUATDivide(qnext, q, &qp);
    C_QUATLogN(&qp, &lqp);
    C_QUATAdd(&lqp, &lqm, &qpqm);
    C_QUATScale(&qpqm, &qpqm, -0.25f);
    C_QUATExp(&qpqm, &exq);
    C_QUATMultiply(q, &exq, a);
}
