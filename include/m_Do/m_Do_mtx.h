#ifndef M_DO_M_DO_MTX_H
#define M_DO_M_DO_MTX_H

#include "SSystem/SComponent/c_sxyz.h"
#include "SSystem/SComponent/c_xyz.h"
#include <dolphin/mtx.h>

void mDoMtx_XYZrotS(Mtx, s16, s16, s16);
void mDoMtx_XYZrotM(Mtx, s16, s16, s16);
void mDoMtx_ZXYrotS(Mtx, s16, s16, s16);
void mDoMtx_ZXYrotM(Mtx, s16, s16, s16);
void mDoMtx_ZrotS(Mtx, s16);
void mDoMtx_YrotS(Mtx, s16);
void mDoMtx_XrotS(Mtx, s16);
void mDoMtx_XrotM(Mtx, s16);
void mDoMtx_YrotM(Mtx, s16);
void mDoMtx_ZrotM(Mtx, s16);
void mDoMtx_MtxToRot(CMtxP, csXyz*);
void mDoMtx_lookAt(Mtx param_0, Vec const* param_1, Vec const* param_2, s16 param_3);
void mDoMtx_lookAt(Mtx param_0, Vec const* param_1, Vec const* param_2, Vec const* param_3,
                   s16 param_4);
void mDoMtx_concatProjView(f32 const (*param_0)[4], f32 const (*param_1)[4], f32 (*param_2)[4]);
void mDoMtx_ZrotM(Mtx mtx, s16 z);
bool mDoMtx_inverseTranspose(f32 const (*param_0)[4], f32 (*param_1)[4]);
void mDoMtx_QuatConcat(Quaternion const* param_0, Quaternion const* param_1, Quaternion* param_2);

inline void mDoMtx_multVecSR(const Mtx m, const Vec* src, Vec* dst) {
    MTXMultVecSR(m, src, dst);
}

inline void mDoMtx_concat(const Mtx a, const Mtx b, Mtx c) {
    PSMTXConcat(a, b, c);
}

inline void cMtx_concat(const Mtx a, const Mtx b, Mtx ab) {
    mDoMtx_concat(a, b, ab);
}

inline void cMtx_scale(Mtx m, f32 x, f32 y, f32 z) {
    MTXScale(m, x, y, z);
}

inline void mDoMtx_multVec(CMtxP m, const Vec* src, Vec* dst) {
    PSMTXMultVec(m, src, dst);
}

inline void mDoMtx_multVecArray(const Mtx m, const Vec* src, Vec* dst, u32 count) {
    MTXMultVecArray(m, src, dst, count);
}

inline void mDoMtx_copy(const Mtx src, Mtx dst) {
    PSMTXCopy(src, dst);
}

inline void mDoMtx_trans(Mtx m, f32 x, f32 y, f32 z) {
    MTXTrans(m, x, y, z);
}

inline void cMtx_XrotM(Mtx mtx, s16 x) {
    mDoMtx_XrotM(mtx, x);
}

inline void cMtx_YrotM(Mtx mtx, s16 y) {
    mDoMtx_YrotM(mtx, y);
}

inline void cMtx_ZrotM(Mtx mtx, s16 z) {
    mDoMtx_ZrotM(mtx, z);
}

inline void cMtx_XrotS(Mtx mtx, s16 x) {
    mDoMtx_XrotS(mtx, x);
}

inline void cMtx_YrotS(Mtx mtx, s16 y) {
    mDoMtx_YrotS(mtx, y);
}

inline void cMtx_ZrotS(Mtx mtx, s16 z) {
    mDoMtx_ZrotS(mtx, z);
}

inline void cMtx_lookAt(Mtx param_0, const Vec* param_1, const Vec* param_2, s16 param_3) {
    mDoMtx_lookAt(param_0, param_1, param_2, param_3);
}

inline void cMtx_multVec(const Mtx mtx, const Vec* src, Vec* dst) {
    mDoMtx_multVec(mtx, src, dst);
}

inline void cMtx_multVecSR(Mtx mtx, const Vec* src, Vec* dst) {
    mDoMtx_multVecSR(mtx, src, dst);
}

inline void cMtx_lookAt(Mtx param_0, const Vec* param_1, const Vec* param_2, const Vec* param_3, s16 param_4) {
    mDoMtx_lookAt(param_0,param_1,param_2,param_3,param_4);
}

inline void cMtx_copy(const Mtx src, Mtx dst) {
    mDoMtx_copy(src, dst);
}

inline void cMtx_multVecArray(const Mtx mtx, const Vec* src, Vec* dst, u32 count) {
    mDoMtx_multVecArray(mtx, src, dst, count);
}

inline void cMtx_inverseTranspose(const Mtx a, Mtx b) {
    mDoMtx_inverseTranspose(a, b);
}

inline void mDoMtx_multVecZero(CMtxP param_0, Vec* param_1) {
    param_1->x = param_0[0][3];
    param_1->y = param_0[1][3];
    param_1->z = param_0[2][3];
}

inline void mDoMtx_quatMultiply(const Quaternion* a, const Quaternion* b, Quaternion* ab) {
    QUATMultiply(a,b,ab);
}

inline void mDoMtx_quatSlerp(const Quaternion* a, const Quaternion* b, Quaternion* ab, f32 param_4) {
    C_QUATSlerp(a,b,ab,param_4);
}

inline void mDoMtx_quatRotAxisRad(Quaternion* q, const Vec* axis, f32 rad) {
    C_QUATRotAxisRad(q, axis, rad);
}

inline void mDoMtx_identity(Mtx m) {
    PSMTXIdentity(m);
}

inline void mDoMtx_inverse(const Mtx a, Mtx b) {
    MTXInverse(a, b);
}

inline void mDoMtx_scale(Mtx m, f32 x, f32 y, f32 z) {
    MTXScale(m, x, y, z);
}

inline void mDoMtx_quat(Mtx m, const Quaternion* q) {
    MTXQuat(m, q);
}

inline void cMtx_inverse(const Mtx a, Mtx b) {
    mDoMtx_inverse(a, b);
}

inline void cMtx_concatProjView(const Mtx a, const Mtx b, Mtx c) {
    mDoMtx_concatProjView(a, b, c);
}

class mDoMtx_stack_c {
public:
    mDoMtx_stack_c() {
        next = buffer;
        end = buffer + 16;
    }

    static bool push();
    static bool pop();

    /**
     * Translates the `now` Matrix by the given cXyz
     * @param xyz The xyz translation vector
     */
    static void transS(cXyz const& xyz);

    /**
     * Translates a new Matrix by the given cXyz and then concatenates it with the `now` matrix
     * @param xyz The xyz translation vector
     */
    static void transM(cXyz const& xyz);

    /**
     * Translates a new Matrix by the given X, Y, and Z values and then concatenates it with the `now` matrix
     * @param x The x-axis translation value
     * @param y The y-axis translation value
     * @param z The z-axis translation value
     */
    static void transM(f32 x, f32 y, f32 z);

    /**
     * Scales the `now` Matrix by the given cXyz
     * @param xyz The xyz scale vector
     */
    static void scaleS(cXyz const& xyz);

    /**
     * Scales a new Matrix by the given cXyz and then concatenates it with the `now` matrix
     * @param xyz The xyz scale vector
     */
    static void scaleM(cXyz const& xyz);

    /**
     * Scales a new Matrix by the given X, Y, and Z values and then concatenates it with the `now` matrix
     * @param x The x-axis scale value
     * @param y The y-axis scale value
     * @param z The z-axis scale value
     */
    static void scaleM(f32 x, f32 y, f32 z);

    static void XYZrotS(csXyz const& xyz);

    /**
     * Rotates the `now` matrix by the given csXyz in the order Z, Y, X
     * @param xyz The xyz rotation vector
     */
    static void XYZrotM(csXyz const& xyz);

    static void ZXYrotS(csXyz const& xyz);

    /**
     * Rotates the `now` matrix by the given csXyz in the order X, Y, Z
     * @param xyz The xyz rotation vector
     */
    static void ZXYrotM(csXyz const& xyz);

    static void quatS(const Quaternion* quat) {
        MTXQuat(now, quat);
    }

    static void quatM(Quaternion const*);
    ~mDoMtx_stack_c() {}  // inline

    /**
     * Returns the `now` Matrix
     * @return The `now` Matrix
     */
    static MtxP get() { return now; }

    /**
     * Translates the `now` Matrix by the given X, Y, and Z values
     * @param x The x-axis translation value
     * @param y The y-axis translation value
     * @param z The z-axis translation value
     */
    static void transS(f32 x, f32 y, f32 z) { MTXTrans(now, x, y, z); }

    /**
     * Scales the `now` Matrix by the given X, Y, and Z values
     * @param x The x-axis scale value
     * @param y The y-axis scale value
     * @param z The z-axis scale value
     */
    static void scaleS(f32 x, f32 y, f32 z) { MTXScale(now, x, y, z); }

    /**
     * Multiplies a given Vec `a` by the `now` Matrix and places the result into Vec `b`
     * @param a The source Vec
     * @param b The output Vec
     */
    static void multVec(const Vec* a, Vec* b) { MTXMultVec(now, a, b); }

    /**
     * Multiplies a given Vec `a` by the `now` Matrix's "Scale-and-Rotate" component and places the result into Vec `b`
     * @param a The source Vec
     * @param b The output Vec
     */
    static void multVecSR(const Vec* a, Vec* b) { MTXMultVecSR(now, a, b); }

    static void multVecZero(Vec* v) { mDoMtx_multVecZero(now, v); }

    /**
     * Multiplies a given Vec array `src` by the `now` Matrix and places the result into Vec array `dst`
     * @param src The source Vec array
     * @param dst The output Vec array
     * @param count The size of the array
     */
    static void multVecArray(const Vec* src, Vec* dst, u32 count) {
        MTXMultVecArray(now, src, dst, count);
    }

    static void XYZrotS(s16 x, s16 y, s16 z) { mDoMtx_XYZrotS(now, x, y, z); }

    /**
     * Rotates the `now` matrix by the given X, Y, and Z values in the order Z, Y, X
     * @param x The x-axis rotation value
     * @param y The y-axis rotation value
     * @param z The z-axis rotation value
     */
    static void XYZrotM(s16 x, s16 y, s16 z) { mDoMtx_XYZrotM(now, x, y, z); }

    static void ZXYrotS(s16 x, s16 y, s16 z) { mDoMtx_ZXYrotS(now, x, y, z); }

    /**
     * Rotates the `now` matrix by the given X, Y, and Z values in the order X, Y, Z
     * @param x The x-axis rotation value
     * @param y The y-axis rotation value
     * @param z The z-axis rotation value
     */
    static void ZXYrotM(s16 x, s16 y, s16 z) { mDoMtx_ZXYrotM(now, x, y, z); }

    /**
     * Rotates a new matrix on the Y-axis then concatenates it with the `now` matrix
     * @param y The rotation value
     */
    static void YrotM(s16 y) { mDoMtx_YrotM(now, y); }

    /**
     * Rotates the `now` matrix on the Y-axis
     * @param y The rotation value
     */
    static void YrotS(s16 y) { mDoMtx_YrotS(now, y); }

    /**
     * Rotates the `now` matrix on the X-axis
     * @param x The rotation value
     */
    static void XrotS(s16 x) { mDoMtx_XrotS(now, x); }

    /**
     * Rotates a new matrix on the X-axis then concatenates it with the `now` matrix
     * @param x The rotation value
     */
    static void XrotM(s16 x) { mDoMtx_XrotM(now, x); }

    /**
     * Rotates the `now` matrix on the Z-axis
     * @param z The rotation value
     */
    static void ZrotS(s16 z) { mDoMtx_ZrotS(now, z); }

    /**
     * Rotates a new matrix on the z-axis then concatenates it with the `now` matrix
     * @param z The rotation value
     */
    static void ZrotM(s16 z) { mDoMtx_ZrotM(now, z); }

    static void inverse() { MTXInverse(now, now); }

    static void inverseTranspose() { mDoMtx_inverseTranspose(now, now); }

    /**
     * Concatenates the `now` matrix with the given Matrix `m`
     * @param m The matrix to concatenate with `now`
     */
    static void concat(const Mtx m) { MTXConcat(now, m, now); }

    static void revConcat(const Mtx m) { MTXConcat(m, now, now); }

    /**
     * Copies a given matrix `m` to the `now` matrix
     * @param m The source matrix to copy
     */
    static void copy(const Mtx m) { PSMTXCopy(m, now); }

    static void rotAxisRadS(const Vec* axis, f32 rad) {
        MTXRotAxisRad(now, axis, rad);
    }

    static void identity() {
        MTXIdentity(now);
    }

    static Mtx now;
    static Mtx buffer[16];
    static Mtx* next;
    static Mtx* end;
};

extern Mtx g_mDoMtx_identity;

inline MtxP mDoMtx_getIdentity() {
    return g_mDoMtx_identity;
}

inline MtxP cMtx_getIdentity() {
    return mDoMtx_getIdentity();
}

class mDoMtx_quatStack_c {
public:
    mDoMtx_quatStack_c() {
        field_0x0 = &field_0x4;
        field_0x114 = field_0x14;
        field_0x118 = &field_0x114;
    }
    ~mDoMtx_quatStack_c() {}  // inline

    /* 0x000 */ Quaternion* field_0x0;
    /* 0x004 */ Quaternion field_0x4;
    /* 0x014 */ Quaternion field_0x14[16];
    /* 0x114 */ Quaternion* field_0x114;
    /* 0x118 */ Quaternion** field_0x118;
};  // Size: 0x11C

#endif /* M_DO_M_DO_MTX_H */
