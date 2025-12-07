#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JMath/JMath.h"
#include "JSystem/JMath/JMATrigonometric.h"

void JMAEulerToQuat(s16 x, s16 y, s16 z, Quaternion* quat) {
    f32 cosX = JMASCos(x / 2);
    f32 cosY = JMASCos(y / 2);
    f32 cosZ = JMASCos(z / 2);
    f32 sinX = JMASSin(x / 2);
    f32 sinY = JMASSin(y / 2);
    f32 sinZ = JMASSin(z / 2);

    f32 cyz = cosY * cosZ;
    f32 syz = sinY * sinZ;
    quat->w = cosX * (cyz) + sinX * (syz);
    quat->x = sinX * (cyz)-cosX * (syz);
    quat->y = cosZ * (cosX * sinY) + sinZ * (sinX * cosY);
    quat->z = sinZ * (cosX * cosY) - cosZ * (sinX * sinY);
}

void JMAQuatLerp(__REGISTER const Quaternion* p, __REGISTER const Quaternion* q, f32 t,
                 Quaternion* dst) {
    __REGISTER f32 pxy, pzw, qxy, qzw;
    __REGISTER f32 dp;

#ifdef __MWERKS__ // clang-format off    
    // compute dot product
    asm {
        psq_l       pxy, 0(p), 0, 0
        psq_l       qxy, 0(q), 0, 0
        ps_mul      dp, pxy, qxy
        
        psq_l       pzw, 8(p), 0, 0
        psq_l       qzw, 8(q), 0, 0
        ps_madd     dp, pzw, qzw, dp
        
        ps_sum0     dp, dp, dp, dp
    }
#endif // clang-format on

    if (dp < 0.0) {
        dst->x = -t * (p->x + q->x) + p->x;
        dst->y = -t * (p->y + q->y) + p->y;
        dst->z = -t * (p->z + q->z) + p->z;
        dst->w = -t * (p->w + q->w) + p->w;
    } else {
        dst->x = -t * (p->x - q->x) + p->x;
        dst->y = -t * (p->y - q->y) + p->y;
        dst->z = -t * (p->z - q->z) + p->z;
        dst->w = -t * (p->w - q->w) + p->w;
    }
}

void JMAFastVECNormalize(__REGISTER const Vec* src, __REGISTER Vec* dst) {
    __REGISTER f32 vxy, rxy, vz, length;
#ifdef __MWERKS__  // clang-format off
	asm {
		psq_l vxy, 0(src), 0, 0
		ps_mul rxy, vxy, vxy
		lfs vz, src->z
		ps_madd length, vz, vz, rxy
		ps_sum0 length, length, rxy, rxy
		frsqrte length, length
		ps_muls0 vxy, vxy, length;
		psq_st vxy, 0(dst), 0, 0
		fmuls vz, vz, length
		stfs vz, dst->z
	}
#endif  // clang-format on
}

void JMAVECScaleAdd(__REGISTER const Vec* vec1, __REGISTER const Vec* vec2, __REGISTER Vec* dst,
                    __REGISTER f32 scale) {
    __REGISTER f32 v1xy;
    __REGISTER f32 v2xy = scale;
    __REGISTER f32 rxy, v1z, v2z, rz;
#ifdef __MWERKS__  // clang-format off
	asm {
        psq_l v1xy, 0(vec1), 0, 0
        psq_l v2xy, 0(vec2), 0, 0
        ps_madds0 rxy, v1xy, scale, v2xy
        psq_st rxy, 0(dst), 0, 0

        psq_l v1z, 8(vec1), 1, 0
        psq_l v2z, 8(vec2), 1, 0
        ps_madds0 rz, v1z,  scale, v2z
        psq_st rz, 8(dst), 1, 0
	}
#endif  // clang-format on
}

void JMAMTXApplyScale(__REGISTER const Mtx src, __REGISTER Mtx dst, __REGISTER f32 xScale,
                      __REGISTER f32 yScale, __REGISTER f32 zScale) {
    __REGISTER f32 scale = yScale;
    __REGISTER f32 x, y, z;
    __REGISTER f32 normal = 1.0f;
#ifdef __MWERKS__  // clang-format off
    asm {
        // scale first 2 components
        ps_merge00 scale, xScale, scale
        psq_l x, 0(src), 0, 0
        psq_l y, 16(src), 0, 0
        psq_l z, 32(src), 0, 0
        ps_mul x, x, scale
        ps_mul y, y, scale
        ps_mul z, z, scale
        psq_st x, 0(dst), 0, 0
        psq_st y, 16(dst), 0, 0
        psq_st z, 32(dst), 0, 0

        // scale last 2 components
        ps_merge00 scale, zScale, normal
        psq_l x, 8(src), 0, 0
        psq_l y, 24(src), 0, 0
        psq_l z, 40(src), 0, 0
        ps_mul x, x, scale
        ps_mul y, y, scale
        ps_mul z, z, scale
        psq_st x, 8(dst), 0, 0
        psq_st y, 24(dst), 0, 0
        psq_st z, 40(dst), 0, 0
    }
#endif  // clang-format on
}
