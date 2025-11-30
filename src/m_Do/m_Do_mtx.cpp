/**
 * m_Do_mtx.cpp
 * Matrix Helper Functions
 */

#include "m_Do/m_Do_mtx.h"
#include "JSystem/JGeometry.h"
#include "JSystem/JMath/JMath.h"
#include "JSystem/JUtility/JUTAssert.h"
#include "SSystem/SComponent/c_m3d.h"
#include "SSystem/SComponent/c_math.h"
#include "global.h"

Mtx mDoMtx_stack_c::now;

Mtx mDoMtx_stack_c::buffer[16];

Mtx* mDoMtx_stack_c::next = mDoMtx_stack_c::buffer;

Mtx* mDoMtx_stack_c::end = mDoMtx_stack_c::buffer + 16;

static mDoMtx_stack_c mDoMtx_stack;

static mDoMtx_quatStack_c mDoMtx_quatStack;

extern Mtx g_mDoMtx_identity = {
    {1.0f, 0.0f, 0.0f, 0.0f},
    {0.0f, 1.0f, 0.0f, 0.0f},
    {0.0f, 0.0f, 1.0f, 0.0f},
};

#if VERSION != VERSION_GCN_JPN
static void dummy() {
    OSReport("mDoMtx_Dump %s\n");
}
#endif

void mDoMtx_XYZrotS(Mtx mtx, s16 x, s16 y, s16 z) {
    Mtx tmp;
    if (z != 0) {
        mDoMtx_ZrotS(mtx, z);
    } else {
        mDoMtx_identity(mtx);
    }

    if (y != 0) {
        mDoMtx_YrotS(tmp, y);
        mDoMtx_concat(mtx, tmp, mtx);
    }

    if (x != 0) {
        mDoMtx_XrotS(tmp, x);
        mDoMtx_concat(mtx, tmp, mtx);
    }
}

void mDoMtx_XYZrotM(Mtx mtx, s16 x, s16 y, s16 z) {
    Mtx tmp;
    if (z != 0) {
        mDoMtx_ZrotS(tmp, z);
        mDoMtx_concat(mtx, tmp, mtx);
    }

    if (y != 0) {
        mDoMtx_YrotS(tmp, y);
        mDoMtx_concat(mtx, tmp, mtx);
    }

    if (x != 0) {
        mDoMtx_XrotS(tmp, x);
        mDoMtx_concat(mtx, tmp, mtx);
    }
}

void mDoMtx_ZXYrotS(Mtx mtx, s16 x, s16 y, s16 z) {
    Mtx tmp;
    if (y != 0) {
        mDoMtx_YrotS(mtx, y);
    } else {
        mDoMtx_identity(mtx);
    }

    if (x != 0) {
        mDoMtx_XrotS(tmp, x);
        mDoMtx_concat(mtx, tmp, mtx);
    }

    if (z != 0) {
        mDoMtx_ZrotS(tmp, z);
        mDoMtx_concat(mtx, tmp, mtx);
    }
}

void mDoMtx_ZXYrotM(Mtx mtx, s16 x, s16 y, s16 z) {
    Mtx tmp;
    if (y != 0) {
        mDoMtx_YrotS(tmp, y);
        mDoMtx_concat(mtx, tmp, mtx);
    }

    if (x != 0) {
        mDoMtx_XrotS(tmp, x);
        mDoMtx_concat(mtx, tmp, mtx);
    }

    if (z != 0) {
        mDoMtx_ZrotS(tmp, z);
        mDoMtx_concat(mtx, tmp, mtx);
    }
}

void mDoMtx_XrotS(Mtx mtx, s16 x) {
    f32 l_cos = JMASCos(x);
    f32 l_sin = JMASSin(x);

    mtx[0][0] = 1.0f;
    mtx[0][1] = 0.0f;
    mtx[0][2] = 0.0f;
    mtx[0][3] = 0.0f;

    mtx[1][0] = 0.0f;
    mtx[1][1] = l_cos;
    mtx[1][2] = -l_sin;
    mtx[1][3] = 0.0f;

    mtx[2][0] = 0.0f;
    mtx[2][1] = l_sin;
    mtx[2][2] = l_cos;
    mtx[2][3] = 0.0f;
}

void mDoMtx_XrotM(Mtx mtx, s16 x) {
    Mtx tmp;
    mDoMtx_XrotS(tmp, x);
    mDoMtx_concat(mtx, tmp, mtx);
}

/**
 * @brief Sets the given matrix to a rotation transformation around the Y-axis.
 *
 * This function constructs a 3x4 rotation matrix for rotating around the Y-axis by the specified angle.
 * The matrix is filled directly with cosine and sine values computed from the angle.
 *
 * The resulting matrix layout is:
 * - Row 0: cos(y), 0, sin(y), 0
 * - Row 1: 0, 1, 0, 0
 * - Row 2: -sin(y), 0, cos(y), 0
 *
 * @param mtx The 3x4 matrix to modify (output parameter).
 * @param y The rotation angle as a signed 16-bit integer.
 */
void mDoMtx_YrotS(Mtx mtx, s16 y) {
    f32 l_cos = JMASCos(y);
    f32 l_sin = JMASSin(y);

    mtx[0][0] = l_cos;
    mtx[0][1] = 0.0f;
    mtx[0][2] = l_sin;
    mtx[0][3] = 0.0f;

    mtx[1][0] = 0.0f;
    mtx[1][1] = 1.0;
    mtx[1][2] = 0.0;
    mtx[1][3] = 0.0f;

    mtx[2][0] = -l_sin;
    mtx[2][1] = 0.0f;
    mtx[2][2] = l_cos;
    mtx[2][3] = 0.0f;
}

void mDoMtx_YrotM(Mtx mtx, s16 y) {
    Mtx tmp;
    mDoMtx_YrotS(tmp, y);
    mDoMtx_concat(mtx, tmp, mtx);
}

void mDoMtx_ZrotS(Mtx mtx, s16 z) {
    f32 l_cos = JMASCos(z);
    f32 l_sin = JMASSin(z);

    mtx[0][0] = l_cos;
    mtx[0][1] = -l_sin;
    mtx[0][2] = 0.0f;
    mtx[0][3] = 0.0f;

    mtx[1][0] = l_sin;
    mtx[1][1] = l_cos;
    mtx[1][2] = 0.0;
    mtx[1][3] = 0.0f;

    mtx[2][0] = 0.0f;
    mtx[2][1] = 0.0f;
    mtx[2][2] = 1.0f;
    mtx[2][3] = 0.0f;
}

void mDoMtx_ZrotM(Mtx mtx, s16 z) {
    Mtx tmp;
    mDoMtx_ZrotS(tmp, z);
    mDoMtx_concat(mtx, tmp, mtx);
}

void mDoMtx_lookAt(Mtx mtx, Vec const* param_1, Vec const* param_2, s16 param_3) {
    cXyz stack_48(*(cXyz*)param_1);
    cXyz local_54;
    cXyz local_60;
    cXyz local_6c;

    local_6c = stack_48 - cXyz(*(cXyz*)param_2);
    local_6c.normalize();
    local_54.set(0.0f, 1.0f, 0.0f);
    local_54 = local_54.outprod(local_6c);

    if (!local_54.normalizeRS()) {
        local_54.set(-local_6c.y, 0.0f, 0.0f);
        local_54 = local_54.outprod(local_6c);
        local_54.normalize();
    }

    local_60 = local_6c.outprod(local_54);
    local_60.normalize();

    mtx[0][0] = local_54.x;
    mtx[0][1] = local_54.y;
    mtx[0][2] = local_54.z;
    mtx[0][3] = -local_54.inprod(stack_48);

    mtx[1][0] = local_60.x;
    mtx[1][1] = local_60.y;
    mtx[1][2] = local_60.z;
    mtx[1][3] = -local_60.inprod(stack_48);

    mtx[2][0] = local_6c.x;
    mtx[2][1] = local_6c.y;
    mtx[2][2] = local_6c.z;
    mtx[2][3] = -local_6c.inprod(stack_48);

    Mtx tmp;
    mDoMtx_ZrotS(tmp, param_3);
    mDoMtx_concat(tmp, mtx, mtx);
}

void mDoMtx_lookAt(Mtx mtx, Vec const* i_eye, Vec const* i_center, Vec const* i_up, s16 i_bank) {
    cXyz eye = *i_eye;
    cXyz center = *i_center;
    cXyz up = *i_up;
    cXyz local_70 = eye - center;
    if (!local_70.normalizeRS()) {
        OS_REPORT("%s: lookat: bad position!\n", __FILE__);
        OS_REPORT("%s:         ctr %f %f %f+1.0\n", __FILE__, center.x, center.y, center.z);
        OS_REPORT("%s:         eye %f %f %f\n", __FILE__, eye.x, eye.y, eye.z);
        center.z += 1.0f;
    }
    if (cM3d_IsZero(up.x) && cM3d_IsZero(up.y) && cM3d_IsZero(up.z)) {
        OS_REPORT("%s: lookat: Zero Vector @ UP!\n", __FILE__);
        up.y = 1.0f;
    }
    C_MTXLookAt(mtx, &eye, &up, &center);
    Mtx bank_mtx;
    mDoMtx_ZrotS(bank_mtx, i_bank);
    mDoMtx_concat(bank_mtx, mtx, mtx);
    JGeometry::TVec3<f32> local_7c(0.0f, mtx[1][1], mtx[2][1]);
    if (local_7c.isZero()) {
        center.y += 1.0f;
        C_MTXLookAt(mtx, &eye, &up, &center);
        mDoMtx_ZrotS(bank_mtx, i_bank);
        mDoMtx_concat(bank_mtx, mtx, mtx);
    }
}

void mDoMtx_concatProjView(const Mtx a, const Mtx b, Mtx c) {
    mDoMtx_concat(a, b, c);
    c[3][0] = a[3][0] * b[0][0] + a[3][1] * b[1][0] + a[3][2] * b[2][0];
    c[3][1] = a[3][0] * b[0][1] + a[3][1] * b[1][1] + a[3][2] * b[2][1];
    c[3][2] = a[3][0] * b[0][2] + a[3][1] * b[1][2] + a[3][2] * b[2][2];
    c[3][3] = a[3][0] * b[0][3] + a[3][1] * b[1][3] + a[3][2] * b[2][3] + a[3][3];
}

bool mDoMtx_inverseTranspose(const Mtx a, Mtx b) {
    f32 f31 = a[0][0] * a[1][1] * a[2][2] + a[0][1] * a[1][2] * a[2][0] +
               a[0][2] * a[1][0] * a[2][1] - a[2][0] * a[1][1] * a[0][2] -
               a[1][0] * a[0][1] * a[2][2] - a[0][0] * a[2][1] * a[1][2];

    if (f31 == 0.0f) {
        return false;
    }

    f31 = 1.0f / f31;

    Mtx23 tmp;
    tmp[0][0] = f31 * (a[1][1] * a[2][2] - a[2][1] * a[1][2]);
    tmp[0][1] = f31 * -(a[1][0] * a[2][2] - a[2][0] * a[1][2]);
    tmp[0][2] = f31 * (a[1][0] * a[2][1] - a[2][0] * a[1][1]);
    tmp[1][0] = f31 * -(a[0][1] * a[2][2] - a[2][1] * a[0][2]);
    tmp[1][1] = f31 * (a[0][0] * a[2][2] - a[2][0] * a[0][2]);
    tmp[1][2] = f31 * -(a[0][0] * a[2][1] - a[2][0] * a[0][1]);

    b[2][0] = f31 * (a[0][1] * a[1][2] - a[1][1] * a[0][2]);
    b[2][1] = f31 * -(a[0][0] * a[1][2] - a[1][0] * a[0][2]);
    b[2][2] = f31 * (a[0][0] * a[1][1] - a[1][0] * a[0][1]);
    b[0][0] = tmp[0][0];
    b[0][1] = tmp[0][1];
    b[0][2] = tmp[0][2];
    b[0][3] = 0.0f;
    b[1][0] = tmp[1][0];
    b[1][1] = tmp[1][1];
    b[1][2] = tmp[1][2];
    b[1][3] = 0.0f;
    b[2][3] = 0.0f;

    return true;
}

void mDoMtx_QuatConcat(const Quaternion* a, const Quaternion* b, Quaternion* c) {
    c->w = (a->w * b->w) - (a->x * b->x) - (a->y * b->y) - (a->z * b->z);
    c->x = (a->w * b->x) + (a->x * b->w) + (a->y * b->z) - (a->z * b->y);
    c->y = (a->w * b->y) + (a->y * b->w) + (a->z * b->x) - (a->x * b->z);
    c->z = (a->w * b->z) + (a->z * b->w) + (a->x * b->y) - (a->y * b->x);
}

void mDoMtx_MtxToRot(CMtxP m, csXyz* o_rot) {
    f32 f31 = m[0][2];
    f31 *= f31;
    f32 f30 = m[2][2];
    f31 += f30 * f30;
    f31 = JMAFastSqrt(f31);

    o_rot->x = cM_atan2s(-m[1][2], f31);

    if (o_rot->x == 0x4000 || o_rot->x == -0x4000) {
        o_rot->z = 0;
        o_rot->y = cM_atan2s(-m[2][0], m[0][0]);
    } else {
        o_rot->y = cM_atan2s(m[0][2], m[2][2]);
        o_rot->z = cM_atan2s(m[1][0], m[1][1]);
    }
}

bool mDoMtx_stack_c::push() {
    if (next >= end) {
        JUT_ASSERT(729, next < end)
        return false;
    }

    mDoMtx_copy(now, *next++);
    return true;
}

bool mDoMtx_stack_c::pop() {
    if (next <= buffer) {
        JUT_ASSERT(751, next > buffer);
        return false;
    }

    mDoMtx_copy(*--next, now);
    return true;
}

void mDoMtx_stack_c::transS(cXyz const& param_0) {
    transS(param_0.x, param_0.y, param_0.z);
}

void mDoMtx_stack_c::transM(f32 x, f32 y, f32 z) {
    Mtx tmp;
    mDoMtx_trans(tmp, x, y, z);
    mDoMtx_concat(now, tmp, now);
}

void mDoMtx_stack_c::transM(cXyz const& param_0) {
    transM(param_0.x, param_0.y, param_0.z);
}

void mDoMtx_stack_c::scaleS(cXyz const& param_0) {
    scaleS(param_0.x, param_0.y, param_0.z);
}

void mDoMtx_stack_c::scaleM(f32 x, f32 y, f32 z) {
    Mtx tmp;
    mDoMtx_scale(tmp, x, y, z);
    mDoMtx_concat(now, tmp, now);
}

void mDoMtx_stack_c::scaleM(cXyz const& param_0) {
    scaleM(param_0.x, param_0.y, param_0.z);
}

void mDoMtx_stack_c::XYZrotS(csXyz const& param_0) {
    XYZrotS(param_0.x, param_0.y, param_0.z);
}

void mDoMtx_stack_c::XYZrotM(csXyz const& param_0) {
    XYZrotM(param_0.x, param_0.y, param_0.z);
}

void mDoMtx_stack_c::ZXYrotS(csXyz const& param_0) {
    ZXYrotS(param_0.x, param_0.y, param_0.z);
}

void mDoMtx_stack_c::ZXYrotM(csXyz const& param_0) {
    ZXYrotM(param_0.x, param_0.y, param_0.z);
}

void mDoMtx_stack_c::quatM(Quaternion const* param_0) {
    Mtx tmp;
    mDoMtx_quat(tmp, param_0);
    mDoMtx_concat(now, tmp, now);
}
