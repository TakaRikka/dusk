// This should go into aurora, but for now we'll place it here:
#include <dolphin/mtx.h>
#include <math.h>
#include <cstdio>

#define ASSERTLINE(line, cond) (void)0
#define ASSERTMSGLINE(line, cond, msg) (void)0
#define ASSERTMSG1LINE(line, cond, msg, arg1) (void)0
#define ASSERTMSG2LINE(line, cond, msg, arg1, arg2) (void)0
#define ASSERTMSGLINEV(line, cond, ...) (void)0

// SNIP : after here, place it into aurora

void C_MTXLightOrtho(Mtx m, f32 t, f32 b, f32 l, f32 r, f32 scaleS, f32 scaleT, f32 transS, f32 transT) {
    f32 tmp;

    ASSERTMSGLINE(2672, m, "MTXLightOrtho():  NULL MtxPtr 'm' ");
    ASSERTMSGLINE(2673, (t != b), "MTXLightOrtho():  't' and 'b' clipping planes are equal ");
    ASSERTMSGLINE(2674, (l != r), "MTXLightOrtho():  'l' and 'r' clipping planes are equal ");
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

void C_MTXLightPerspective(Mtx m, f32 fovY, f32 aspect, f32 scaleS, f32 scaleT, f32 transS, f32 transT) {
    f32 angle;
    f32 cot;

    ASSERTMSGLINE(2604, m, "MTXLightPerspective():  NULL MtxPtr 'm' ");
    ASSERTMSGLINE(2605, (fovY > 0.0) && (fovY < 180.0), "MTXLightPerspective():  'fovY' out of range ");
    ASSERTMSGLINE(2606, 0 != aspect, "MTXLightPerspective():  'aspect' is 0 ");

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

void C_MTXLookAt(Mtx m, const Point3d* camPos, const Vec* camUp, const Point3d* target) {
    Vec vLook;
    Vec vRight;
    Vec vUp;

    ASSERTMSGLINE(2437, m, "MTXLookAt():  NULL MtxPtr 'm' ");
    ASSERTMSGLINE(2438, camPos, "MTXLookAt():  NULL VecPtr 'camPos' ");
    ASSERTMSGLINE(2439, camUp, "MTXLookAt():  NULL VecPtr 'camUp' ");
    ASSERTMSGLINE(2440, target, "MTXLookAt():  NULL Point3dPtr 'target' ");

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

void C_MTXPerspective(Mtx44 m, f32 fovY, f32 aspect, f32 n, f32 f) {
    f32 angle;
    f32 cot;
    f32 tmp;

    ASSERTMSGLINE(179, m, "MTXPerspective():  NULL Mtx44Ptr 'm' ");
    ASSERTMSGLINE(180, (fovY > 0.0) && (fovY < 180.0), "MTXPerspective():  'fovY' out of range ");
    ASSERTMSGLINE(181, 0.0f != aspect, "MTXPerspective():  'aspect' is 0 ");

    angle = (0.5f * fovY);
    angle = MTXDegToRad(angle);
    cot = 1 / tanf(angle);
    m[0][0] = (cot / aspect);
    m[0][1] = 0;
    m[0][2] = 0;
    m[0][3] = 0;
    m[1][0] = 0;
    m[1][1] = (cot);
    m[1][2] = 0;
    m[1][3] = 0;
    m[2][0] = 0;
    m[2][1] = 0;
    tmp = 1 / (f - n);
    m[2][2] = (-n * tmp);
    m[2][3] = (tmp * -(f * n));
    m[3][0] = 0;
    m[3][1] = 0;
    m[3][2] = -1;
    m[3][3] = 0;
}

void C_MTXRotRad(Mtx m, char axis, f32 rad) {
    f32 sinA;
    f32 cosA;

    ASSERTMSGLINE(1447, m, "MTXRotRad():  NULL MtxPtr 'm' ");
    sinA = sinf(rad);
    cosA = cosf(rad);
    C_MTXRotTrig(m, axis, sinA, cosA);
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

void C_VECAdd(const Vec* a, const Vec* b, Vec* ab) {
    ASSERTMSGLINE(114, a, "VECAdd():  NULL VecPtr 'a' ");
    ASSERTMSGLINE(115, b, "VECAdd():  NULL VecPtr 'b' ");
    ASSERTMSGLINE(116, ab, "VECAdd():  NULL VecPtr 'ab' ");
    ab->x = a->x + b->x;
    ab->y = a->y + b->y;
    ab->z = a->z + b->z;
}

// MTX QUAT

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

void C_QUATRotAxisRad(Quaternion* r, const Vec* axis, f32 rad) {
    f32 half, sh, ch;
    Vec nAxis;

    ASSERTMSGLINE(758, r, "QUATRotAxisRad():  NULL QuaternionPtr 'r' ");
    ASSERTMSGLINE(759, axis, "QUATRotAxisRad():  NULL VecPtr 'axis' ");

    VECNormalize(axis, &nAxis);

    half = rad * 0.5f;
    sh = sinf(half);
    ch = cosf(half);

    r->x = sh * nAxis.x;
    r->y = sh * nAxis.y;
    r->z = sh * nAxis.z;
    r->w = ch;
}

void C_QUATSlerp(const Quaternion* p, const Quaternion* q, Quaternion* r, f32 t) {
    f32 theta, sin_th, cos_th;
    f32 tp, tq;

    ASSERTMSGLINE(869, p, "QUATSlerp():  NULL QuaternionPtr 'p' ");
    ASSERTMSGLINE(870, q, "QUATSlerp():  NULL QuaternionPtr 'q' ");
    ASSERTMSGLINE(871, r, "QUATSlerp():  NULL QuaternionPtr 'r' ");
    
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

void C_VECHalfAngle(const Vec* a, const Vec* b, Vec* half) {
    Vec aTmp;
    Vec bTmp;
    Vec hTmp;

    ASSERTMSGLINE(713, a, "VECHalfAngle():  NULL VecPtr 'a' ");
    ASSERTMSGLINE(714, b, "VECHalfAngle():  NULL VecPtr 'b' ");
    ASSERTMSGLINE(715, half, "VECHalfAngle():  NULL VecPtr 'half' ");

    aTmp.x = -a->x;
    aTmp.y = -a->y;
    aTmp.z = -a->z;
    bTmp.x = -b->x;
    bTmp.y = -b->y;
    bTmp.z = -b->z;

    VECNormalize(&aTmp, &aTmp);
    VECNormalize(&bTmp, &bTmp);
    VECAdd(&aTmp, &bTmp, &hTmp);

    if (VECDotProduct(&hTmp, &hTmp) > 0.0f) {
        VECNormalize(&hTmp, half);
        return;
    }
    *half = hTmp;
}

void C_VECNormalize(const Vec* src, Vec* unit) {
    f32 mag;

    ASSERTMSGLINE(321, src, "VECNormalize():  NULL VecPtr 'src' ");
    ASSERTMSGLINE(322, unit, "VECNormalize():  NULL VecPtr 'unit' ");

    mag = (src->z * src->z) + ((src->x * src->x) + (src->y * src->y));
    ASSERTMSGLINE(327, 0.0f != mag, "VECNormalize():  zero magnitude vector ");

    mag = 1.0f/ sqrtf(mag);
    unit->x = src->x * mag;
    unit->y = src->y * mag;
    unit->z = src->z * mag;
}

void C_VECReflect(const Vec* src, const Vec* normal, Vec* dst) {
    f32 cosA;
    Vec uI;
    Vec uN;

    ASSERTMSGLINE(769, src, "VECReflect():  NULL VecPtr 'src' ");
    ASSERTMSGLINE(770, normal, "VECReflect():  NULL VecPtr 'normal' ");
    ASSERTMSGLINE(771, dst, "VECReflect():  NULL VecPtr 'dst' ");

    uI.x = -src->x;
    uI.y = -src->y;
    uI.z = -src->z;

    VECNormalize(&uI, &uI);
    VECNormalize(normal, &uN);

    cosA = VECDotProduct(&uI, &uN);
    dst->x = (2.0f * uN.x * cosA) - uI.x;
    dst->y = (2.0f * uN.y * cosA) - uI.y;
    dst->z = (2.0f * uN.z * cosA) - uI.z;
    VECNormalize(dst, dst);
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

void C_MTXMultVecArraySR(const Mtx m, const Vec* srcBase, Vec* dstBase, u32 count) {
    u32 i;
    Vec vTmp;

    ASSERTMSGLINE(410, m, "MTXMultVecArraySR():  NULL MtxPtr 'm' ");
    ASSERTMSGLINE(411, srcBase, "MTXMultVecArraySR():  NULL VecPtr 'srcBase' ");
    ASSERTMSGLINE(412, dstBase, "MTXMultVecArraySR():  NULL VecPtr 'dstBase' ");
    ASSERTMSGLINE(413, count > 1, "MTXMultVecArraySR():  count must be greater than 1.");

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

// VEC
void C_VECCrossProduct(const Vec* a, const Vec* b, Vec* axb) {
    Vec vTmp;

    ASSERTMSGLINE(608, a, "VECCrossProduct():  NULL VecPtr 'a' ");
    ASSERTMSGLINE(609, b, "VECCrossProduct():  NULL VecPtr 'b' ");
    ASSERTMSGLINE(610, axb, "VECCrossProduct():  NULL VecPtr 'axb' ");

    vTmp.x = (a->y * b->z) - (a->z * b->y);
    vTmp.y = (a->z * b->x) - (a->x * b->z);
    vTmp.z = (a->x * b->y) - (a->y * b->x);
    axb->x = vTmp.x;
    axb->y = vTmp.y;
    axb->z = vTmp.z;
}

f32 C_VECDistance(const Vec* a, const Vec* b) {
    return sqrtf(C_VECSquareDistance(a, b));
}

f32 C_VECDotProduct(const Vec* a, const Vec* b) {
    f32 dot;

    ASSERTMSGLINE(546, a, "VECDotProduct():  NULL VecPtr 'a' ");
    ASSERTMSGLINE(547, b, "VECDotProduct():  NULL VecPtr 'b' ");
    dot = (a->z * b->z) + ((a->x * b->x) + (a->y * b->y));
    return dot;
}

f32 C_VECMag(const Vec* v) {
    return sqrtf(C_VECSquareMag(v));
}

void C_VECScale(const Vec* src, Vec* dst, f32 scale) {
    ASSERTMSGLINE(253, src, "VECScale():  NULL VecPtr 'src' ");
    ASSERTMSGLINE(254, dst, "VECScale():  NULL VecPtr 'dst' ");
    dst->x = (src->x * scale);
    dst->y = (src->y * scale);
    dst->z = (src->z * scale);
}

f32 C_VECSquareDistance(const Vec* a, const Vec* b) {
    Vec diff;

    diff.x = a->x - b->x;
    diff.y = a->y - b->y;
    diff.z = a->z - b->z;
    return (diff.z * diff.z) + ((diff.x * diff.x) + (diff.y * diff.y));
}

f32 C_VECSquareMag(const Vec* v) {
    f32 sqmag;

    ASSERTMSGLINE(411, v, "VECMag():  NULL VecPtr 'v' ");

    sqmag = v->z * v->z + ((v->x * v->x) + (v->y * v->y));
    return sqmag;
}

void C_VECSubtract(const Vec* a, const Vec* b, Vec* a_b) {
    ASSERTMSGLINE(183, a, "VECSubtract():  NULL VecPtr 'a' ");
    ASSERTMSGLINE(184, b, "VECSubtract():  NULL VecPtr 'b' ");
    ASSERTMSGLINE(185, a_b, "VECSubtract():  NULL VecPtr 'a_b' ");
    a_b->x = a->x - b->x;
    a_b->y = a->y - b->y;
    a_b->z = a->z - b->z;
}

#pragma mark PSMTX
// I think these are PPC ASM implemntations?
// this can be done just with defining DEBUG, but that has some other
// implecations, so we'll just define them here for now. These are all just wrappers around the C versions, so we can just call those directly.
void PSMTXConcatArray(const __REGISTER Mtx a, const __REGISTER Mtx* srcBase, __REGISTER Mtx* dstBase, __REGISTER u32 count) {
  C_MTXConcatArray(a, srcBase, dstBase, count);
}
void PSMTXCopy(const __REGISTER Mtx src, __REGISTER Mtx dst) {
    C_MTXCopy(src, dst);
}
void PSMTXIdentity(__REGISTER Mtx m) {
    C_MTXIdentity(m);
}
u32 PSMTXInverse(const __REGISTER Mtx src, __REGISTER Mtx inv) {
    return C_MTXInverse(src, inv);
}
void PSMTXMultVec(const __REGISTER Mtx m, const __REGISTER Vec* src, __REGISTER Vec* dst) {
    C_MTXMultVec(m, src, dst);
}
void PSMTXConcat(const __REGISTER Mtx a, const __REGISTER Mtx b, __REGISTER Mtx ab) {
    C_MTXConcat(a, b, ab);
}
void PSMTXMultVecArray(const Mtx m, const Vec* srcBase, Vec* dstBase, u32 count) {
    C_MTXMultVecArray(m, srcBase, dstBase, count);
}
void PSMTXMultVecArraySR(const __REGISTER Mtx m, const __REGISTER Vec* srcBase, __REGISTER Vec* dstBase, __REGISTER u32 count) {
    C_MTXMultVecArraySR(m, srcBase, dstBase, count);
}
void PSMTXMultVecSR(const __REGISTER Mtx m, const __REGISTER Vec* src, __REGISTER Vec* dst) {
    C_MTXMultVecSR(m, src, dst);
}
void PSMTXQuat(__REGISTER Mtx m, const __REGISTER Quaternion* q) {
    C_MTXQuat(m, q);
}
void PSMTXRotAxisRad(Mtx m, const Vec* axis, f32 rad) {
    C_MTXRotAxisRad(m, axis, rad);
}
void PSMTXRotRad(Mtx m, char axis, f32 rad) {
    C_MTXRotRad(m, axis, rad);
}
void PSMTXScale(__REGISTER Mtx m, __REGISTER f32 xS, __REGISTER f32 yS, __REGISTER f32 zS) {
    C_MTXScale(m, xS, yS, zS);
}
void PSMTXScaleApply(const __REGISTER Mtx src, __REGISTER Mtx dst, __REGISTER f32 xS, __REGISTER f32 yS, __REGISTER f32 zS) {
    C_MTXScaleApply(src, dst, xS, yS, zS);
}
void PSMTXTrans(__REGISTER Mtx m, __REGISTER f32 xT, __REGISTER f32 yT, __REGISTER f32 zT) {
    C_MTXTrans(m, xT, yT, zT);
}
void PSMTXTransApply(const __REGISTER Mtx src, __REGISTER Mtx dst, __REGISTER f32 xT, __REGISTER f32 yT, __REGISTER f32 zT) {
    C_MTXTransApply(src, dst, xT, yT, zT);
}
void PSQUATMultiply(const __REGISTER Quaternion* p, const __REGISTER Quaternion* q, __REGISTER Quaternion* pq) {
    C_QUATMultiply(p, q, pq);
}
void PSVECAdd(const __REGISTER Vec* a, const __REGISTER Vec* b, __REGISTER Vec* ab) {
    C_VECAdd(a, b, ab);
}
void PSVECCrossProduct(const __REGISTER Vec* a, const __REGISTER Vec* b, __REGISTER Vec* axb) {
    C_VECCrossProduct(a, b, axb);
}
f32 PSVECDistance(const __REGISTER Vec* a, const __REGISTER Vec* b) {
    return C_VECDistance(a, b);
}
f32 PSVECDotProduct(const __REGISTER Vec* a, const __REGISTER Vec* b) {
    return C_VECDotProduct(a, b);
}
f32 PSVECMag(const __REGISTER Vec* v) {
    return C_VECMag(v);
}
void PSVECNormalize(const __REGISTER Vec* src, __REGISTER Vec* unit) {
    C_VECNormalize(src, unit);
}
void PSVECScale(const __REGISTER Vec* src, __REGISTER Vec* dst, __REGISTER f32 scale) {
    C_VECScale(src, dst, scale);
}
f32 PSVECSquareDistance(const __REGISTER Vec* a, const __REGISTER Vec* b) {
    return C_VECSquareDistance(a, b);
}
f32 PSVECSquareMag(const __REGISTER Vec* v) {
    return C_VECSquareMag(v);
}
void PSVECSubtract(const __REGISTER Vec* a, const __REGISTER Vec* b, __REGISTER Vec* a_b) {
    C_VECSubtract(a, b, a_b);
}
