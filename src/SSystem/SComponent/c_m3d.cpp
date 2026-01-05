/**
 * c_m3d.cpp
 *
 */

#include "SSystem/SComponent/c_m3d.h"
#include "SSystem/SComponent/c_m3d_g_aab.h"
#include "SSystem/SComponent/c_m3d_g_cyl.h"
#include "SSystem/SComponent/c_m3d_g_sph.h"
#include "SSystem/SComponent/c_m3d_g_tri.h"
#include "SSystem/SComponent/c_m3d_g_cps.h"
#include "SSystem/SComponent/c_math.h"
#include "SSystem/SComponent/c_sxyz.h"

const f32 G_CM3D_F_ABS_MIN = 32 * FLT_EPSILON;

void cM3d_InDivPos1(const Vec* pVecA, const Vec* pVecB, f32 pF, Vec* pOut) {
    Vec tmp;
    VECScale(pVecB, &tmp, pF);
    VECAdd(&tmp, pVecA, pOut);
}

void cM3d_InDivPos2(const Vec* pVecA, const Vec* pVecB, f32 pF, Vec* pOut) {
    Vec tmp;
    VECSubtract(pVecB, pVecA, &tmp);
    cM3d_InDivPos1(pVecA, &tmp, pF, pOut);
}

f32 cM3d_Len2dSq(f32 pX1, f32 pY1, f32 pX2, f32 pY2) {
    f32 xDiff = pX1 - pX2;
    f32 yDiff = pY1 - pY2;
    return xDiff * xDiff + yDiff * yDiff;
}

static f32 dummy(f32 x) {
    return sqrtf(x);
}

bool cM3d_Len2dSqPntAndSegLine(f32 param_1, f32 param_2, f32 param_3, f32 param_4, f32 p5, f32 p6,
                               f32* param_7, f32* param_8, f32* param_9) {
    bool retVal = false;
    f32 f31 = p5 - param_3;
    f32 f30 = p6 - param_4;
    f32 len = f31 * f31 + f30 * f30;
    f32 f29;
    if (cM3d_IsZero(len)) {
        *param_9 = 0.0f;
        return false;
    } else {
        f29 = (f31 * (param_1 - param_3) + f30 * (param_2 - param_4)) / len;
        if (f29 >= 0.0f && f29 <= 1.0f) {
            retVal = true;
        }
        *param_7 = param_3 + f31 * f29;
        *param_8 = param_4 + f30 * f29;
        *param_9 = cM3d_Len2dSq(*param_7, *param_8, param_1, param_2);
        return retVal;
    }
}

bool cM3d_Len3dSqPntAndSegLine(const cM3dGLin* pLine, const Vec* pVec, Vec* pOutVec, f32* pOutF) {
    bool retVal = false;
    Vec tmp;
    VECSubtract(&pLine->GetEndP(), &pLine->GetStartP(), &tmp);
    f32 seqLen = VECDotProduct(&tmp, &tmp);
    if (cM3d_IsZero(seqLen)) {
        *pOutF = 0.0f;
        return retVal;
    } else {
        Vec tmp2;
        VECSubtract(pVec, &pLine->GetStartP(), &tmp2);
        f32 tmpF = VECDotProduct(&tmp2, &tmp);
        tmpF /= seqLen;
        if (tmpF < 0.0f || tmpF > 1.0f) {
            retVal = false;
        } else {
            retVal = true;
        }
        VECScale(&tmp, &tmp, tmpF);
        VECAdd(&tmp, &pLine->GetStartP(), pOutVec);
        *pOutF = VECSquareDistance(pOutVec, pVec);
        return retVal;
    }
}

f32 cM3d_SignedLenPlaAndPos(const cM3dGPla* pPlane, const Vec* pPosition) {
    f32 mag = VECMag(pPlane->GetNP());
    if (cM3d_IsZero(mag)) {
        return 0.0f;
    } else {
        return (pPlane->mD + VECDotProduct(pPlane->GetNP(), pPosition)) / mag;
    }
}

f32 cM3d_VectorProduct2d(f32 pX1, f32 pY1, f32 pX2, f32 pY2, f32 pX3, f32 pY3) {
    return (pX2 - pX1) * (pY3 - pY1) - (pY2 - pY1) * (pX3 - pX1);
}

void cM3d_VectorProduct(const cXyz* pVecA, const cXyz* pVecB, const cXyz* pVecC, cXyz* pVecOut) {
    Vec tmp1;
    Vec tmp2;
    VECSubtract(pVecB, pVecA, &tmp1);
    VECSubtract(pVecC, pVecA, &tmp2);
    VECCrossProduct(&tmp1, &tmp2, pVecOut);
}

void cM3d_CalcPla(const Vec* pVecA, const Vec* pVecB, const Vec* pVecC, Vec* pVecOut, f32* pD) {
    Vec tmp2;
    Vec tmp1;
    VECSubtract(pVecB, pVecA, &tmp1);
    VECSubtract(pVecC, pVecA, &tmp2);
    VECCrossProduct(&tmp1, &tmp2, pVecOut);
    f32 mag = VECMag(pVecOut);
    if (fabsf(mag) >= 0.02f) {
        VECScale(pVecOut, pVecOut, 1.0f / mag);
        *pD = -VECDotProduct(pVecOut, pVecA);
    } else {
        pVecOut->y = 0.0f;
        *pD = 0.0f;
        pVecOut->z = 0.0f;
        pVecOut->x = 0.0f;
    }
}

inline bool cM3d_CrossNumSection(f32 lMinX, f32 lMaxX, f32 rMinX, f32 rMaxX) {
    if (lMinX > rMaxX) {
        return false;
    } else if (lMaxX < rMinX) {
        return false;
    } else if (rMinX > lMaxX) {
        return false;
    } else if (rMaxX < lMinX) {
        return false;
    } else {
        return true;
    }
}

bool cM3d_Cross_AabAab(const cM3dGAab* pAabA, const cM3dGAab* pAabB) {
    if (cM3d_CrossNumSection(pAabA->GetMinP()->x, pAabA->GetMaxP()->x, pAabB->GetMinP()->x,
                             pAabB->GetMaxP()->x) &&
        cM3d_CrossNumSection(pAabA->GetMinP()->y, pAabA->GetMaxP()->y, pAabB->GetMinP()->y,
                             pAabB->GetMaxP()->y) &&
        cM3d_CrossNumSection(pAabA->GetMinP()->z, pAabA->GetMaxP()->z, pAabB->GetMinP()->z,
                             pAabB->GetMaxP()->z))
    {
        return true;
    } else {
        return false;
    }
}

bool cM3d_Cross_AabCyl(const cM3dGAab* pAab, const cM3dGCyl* pCyl) {
    if (pAab->GetMinP()->x > pCyl->GetCP()->x + pCyl->GetR()) {
        return false;
    } else if (pAab->GetMaxP()->x < pCyl->GetCP()->x - pCyl->GetR()) {
        return false;
    } else if (pAab->GetMinP()->z > pCyl->GetCP()->z + pCyl->GetR()) {
        return false;
    } else if (pAab->GetMaxP()->z < pCyl->GetCP()->z - pCyl->GetR()) {
        return false;
    } else if (pAab->GetMinP()->y > pCyl->GetCP()->y + pCyl->GetH()) {
        return false;
    } else if (pAab->GetMaxP()->y < pCyl->GetCP()->y) {
        return false;
    } else {
        return true;
    }
}

bool cM3d_Cross_AabSph(const cM3dGAab* pAab, const cM3dGSph* pSph) {
    f32 radius = pSph->GetR();
    f32 cx = pSph->GetC().x;
    if (pAab->GetMinX() > cx + radius) {
        return false;
    } else if (pAab->GetMaxX() < cx - radius) {
        return false;
    }

    f32 cz = pSph->GetC().z;
    if (pAab->GetMinZ() > cz + radius) {
        return false;
    } else if (pAab->GetMaxZ() < cz - radius) {
        return false;
    }

    f32 cy = pSph->GetC().y;
    if (pAab->GetMinY() > cy + radius) {
        return false;
    } else if (pAab->GetMaxY() < cy - radius) {
        return false;
    }
    return true;
}

int cM3d_Check_LinLin(const cM3dGLin* lin_a, const cM3dGLin* lin_b, f32* dst_a, f32* dst_b) {
    Vec linAVec;
    Vec linBVec;
    lin_a->CalcVec(&linAVec);
    lin_b->CalcVec(&linBVec);
    f32 linALen = VECMag(&linAVec);
    f32 linBLen = VECMag(&linBVec);
    if (cM3d_IsZero(linALen) || cM3d_IsZero(linBLen)) {
        return 1;
    } else {
        f32 invLinALen = 1.0f / linALen;
        f32 invLinBLen = 1.0f / linBLen;
        VECScale(&linAVec, &linAVec, invLinALen);
        VECScale(&linBVec, &linBVec, invLinBLen);
        Vec tmp;
        VECSubtract(&lin_a->GetStartP(), &lin_b->GetStartP(), &tmp);
        f32 tmpF = -VECDotProduct(&linAVec, &linBVec);
        f32 tmpF2 = VECDotProduct(&tmp, &linAVec);
        VECSquareMag(&tmp);  // result not used
        f32 tmpF3 = fabsf(1.0f - (tmpF * tmpF));
        if (!cM3d_IsZero(tmpF3)) {
            f32 tmpF4 = -VECDotProduct(&tmp, &linBVec);
            f32 tmpF7 = 1.0f / tmpF3;
            f32 outFloatAtmp = ((tmpF * tmpF4) - tmpF2) * tmpF7;
            *dst_a = outFloatAtmp * invLinALen;
            f32 outFloatBtmp = ((tmpF * tmpF2) - tmpF4) * tmpF7;
            *dst_b = outFloatBtmp * invLinBLen;
            return 3;
        } else {
            f32 tmpF5 = -tmpF2;
            f32 tmpF6 = 0.0f;
            if (tmpF5 < 0.0f || (tmpF5 > linALen)) {
                tmpF6 = linBLen;
                tmpF5 = (tmpF6 * tmpF) - tmpF2;
            }
            f32 tmpF7 = VECDotProduct(&tmp, &linBVec);
            if (tmpF5 < 0.0f || tmpF5 > linALen) {
                tmpF5 = 0.0f;
                tmpF6 = tmpF7;
            }
            if (tmpF6 < 0.0f || tmpF6 > linBLen) {
                tmpF5 = linALen;
                tmpF6 = tmpF7 + (-linALen * tmpF);
            }
            *dst_a = tmpF5 * invLinALen;
            *dst_b = tmpF6 * invLinBLen;
            return 2;
        }
    }
}


static bool cM3d_CrossInfLineVsInfPlane_proc(f32 pFloatA, f32 pFloatB, const Vec* pVecA,
                                             const Vec* pVecB, Vec* pVecOut) {
    if (cM3d_IsZero(pFloatA - pFloatB)) {
        *pVecOut = *pVecB;
        return false;
    } else {
        cM3d_InDivPos2(pVecA, pVecB, pFloatA / (pFloatA - pFloatB), pVecOut);
        return true;
    }
}

bool cM3d_Cross_LinPla(const cM3dGLin* lin, const cM3dGPla* pla, Vec* dst, bool a, bool b) {
    f32 startVal = pla->getPlaneFunc(&lin->GetStartP());
    f32 endVal = pla->getPlaneFunc(&lin->GetEndP());
    if (startVal * endVal > 0.0f) {
        *dst = lin->GetEnd();
        return false;
    } else {
        if (startVal >= 0.0f && endVal <= 0.0f) {
            if (a) {
                return cM3d_CrossInfLineVsInfPlane_proc(startVal, endVal, &lin->GetStartP(), &lin->GetEndP(), dst);
            }
        } else {
            if (b) {
                return cM3d_CrossInfLineVsInfPlane_proc(startVal, endVal, &lin->GetStartP(), &lin->GetEndP(), dst);
            }
        }
        *dst = lin->GetEnd();
        return false;
    }
}

static u32 const BPCP_OUTCODE0 = 0x00000001;

static u32 const BPCP_OUTCODE1 = 0x00000002;

static u32 const BPCP_OUTCODE4 = 0x00000010;

static u32 const BPCP_OUTCODE5 = 0x00000020;

static u32 const BPCP_OUTCODE2 = 0x00000004;

static u32 const BPCP_OUTCODE3 = 0x00000008;

static u32 const BEVEL2D_OUTCODE0 = 0x00000001;

static u32 const BEVEL2D_OUTCODE1 = 0x00000002;

static u32 const BEVEL2D_OUTCODE2 = 0x00000004;

static u32 const BEVEL2D_OUTCODE3 = 0x00000008;

static u32 const BEVEL2D_OUTCODE4 = 0x00000010;

static u32 const BEVEL2D_OUTCODE5 = 0x00000020;

static u32 const BEVEL2D_OUTCODE6 = 0x00000040;

static u32 const BEVEL2D_OUTCODE7 = 0x00000080;

static u32 const BEVEL2D_OUTCODE8 = 0x00000100;

static u32 const BEVEL2D_OUTCODE9 = 0x00000200;

static u32 const BEVEL2D_OUTCODE10 = 0x00000400;

static u32 const BEVEL2D_OUTCODE11 = 0x00000800;

static u32 const BEVEL3D_OUTCODE0 = 0x00000001;

static u32 const BEVEL3D_OUTCODE1 = 0x00000002;

static u32 const BEVEL3D_OUTCODE2 = 0x00000004;

static u32 const BEVEL3D_OUTCODE3 = 0x00000008;

static u32 const BEVEL3D_OUTCODE4 = 0x00000010;

static u32 const BEVEL3D_OUTCODE5 = 0x00000020;

static u32 const BEVEL3D_OUTCODE6 = 0x00000040;

static u32 const BEVEL3D_OUTCODE7 = 0x00000080;

inline u32 cM3d_CheckBoxEdgePlane_Bevel2DCheck(Vec const* param_0, Vec const* param_1,
                                               Vec const* param_2) {
    u32 ret = 0;
    if (-param_0->x + param_0->y > -param_1->x + param_2->y) {
        ret |= BEVEL2D_OUTCODE0;
    }
    if (-param_0->x + param_0->y < -param_2->x + param_1->y) {
        ret |= BEVEL2D_OUTCODE1;
    }
    if (param_0->x + param_0->y > param_2->x + param_2->y) {
        ret |= BEVEL2D_OUTCODE2;
    }
    if (param_0->x + param_0->y < param_1->x + param_1->y) {
        ret |= BEVEL2D_OUTCODE3;
    }
    if (-param_0->z + param_0->y > -param_1->z + param_2->y) {
        ret |= BEVEL2D_OUTCODE4;
    }
    if (-param_0->z + param_0->y < -param_2->z + param_1->y) {
        ret |= BEVEL2D_OUTCODE5;
    }
    if (param_0->z + param_0->y > param_2->z + param_2->y) {
        ret |= BEVEL2D_OUTCODE6;
    }
    if (param_0->z + param_0->y < param_1->z + param_1->y) {
        ret |= BEVEL2D_OUTCODE7;
    }
    if (-param_0->z + param_0->x > -param_1->z + param_2->x) {
        ret |= BEVEL2D_OUTCODE8;
    }
    if (-param_0->z + param_0->x < -param_2->z + param_1->x) {
        ret |= BEVEL2D_OUTCODE9;
    }
    if (param_0->z + param_0->x > param_2->z + param_2->x) {
        ret |= BEVEL2D_OUTCODE10;
    }
    if (param_0->z + param_0->x < param_1->z + param_1->x) {
        ret |= BEVEL2D_OUTCODE11;
    }
    return ret;
}

inline u32 cM3d_CheckBoxEdgePlane_Bevel3DCheck(Vec const* param_0, Vec const* param_1,
                                               Vec const* param_2) {
    u32 ret = 0;
    if (param_0->x + param_0->y + param_0->z > param_2->x + param_2->y + param_2->z) {
        ret |= BEVEL3D_OUTCODE0;
    }
    if (-param_0->x + param_0->y + param_0->z > -param_1->x + param_2->y + param_2->z) {
        ret |= BEVEL3D_OUTCODE1;
    }
    if (-param_0->x + param_0->y - param_0->z > -param_1->x + param_2->y - param_1->z) {
        ret |= BEVEL3D_OUTCODE2;
    }
    if (param_0->x + param_0->y - param_0->z > param_2->x + param_2->y - param_1->z) {
        ret |= BEVEL3D_OUTCODE3;
    }
    if (param_0->x - param_0->y + param_0->z > param_2->x - param_1->y + param_2->z) {
        ret |= BEVEL3D_OUTCODE4;
    }
    if (-param_0->x - param_0->y + param_0->z > -param_1->x - param_1->y + param_2->z) {
        ret |= BEVEL3D_OUTCODE5;
    }
    if (-param_0->x - param_0->y + param_0->z > -param_1->x - param_1->y + param_2->z) {
        ret |= BEVEL3D_OUTCODE6;
    }
    if (-param_0->x - param_0->y - param_0->z > -param_1->x - param_1->y - param_1->z) {
        ret |= BEVEL3D_OUTCODE7;
    }
    return ret;
}

bool cM3d_Cross_MinMaxBoxLine(Vec const* param_0, Vec const* param_1, Vec const* param_2,
                              Vec const* param_3) {
    u32 uVar3 = 0;
    u32 uVar4 = 0;

    if (param_2->x > param_1->x) {
        if (param_3->x > param_1->x) {
            return false;
        }
        uVar3 |= BPCP_OUTCODE0;
    } else if (param_3->x > param_1->x) {
        uVar4 |= BPCP_OUTCODE0;
    }

    if ((uVar3 & BPCP_OUTCODE0) == 0 && param_2->x < param_0->x) {
        if ((uVar4 & BPCP_OUTCODE0) == 0 && param_3->x < param_0->x) {
            return false;
        }
        uVar3 |= BPCP_OUTCODE1;
    } else if ((uVar4 & BPCP_OUTCODE0) == 0 && param_3->x < param_0->x) {
        uVar4 |= BPCP_OUTCODE1;
    }

    if (param_2->z > param_1->z) {
        if (param_3->z > param_1->z) {
            return false;
        }
        uVar3 |= BPCP_OUTCODE4;
    } else if (param_3->z > param_1->z) {
        uVar4 |= BPCP_OUTCODE4;
    }

    if ((uVar3 & BPCP_OUTCODE4) == 0 && param_2->z < param_0->z) {
        if ((uVar4 & BPCP_OUTCODE4) == 0 && param_3->z < param_0->z) {
            return false;
        }
        uVar3 |= BPCP_OUTCODE5;
    } else if ((uVar4 & BPCP_OUTCODE4) == 0 && param_3->z < param_0->z) {
        uVar4 |= BPCP_OUTCODE5;
    }

    if (param_2->y > param_1->y) {
        if (param_3->y > param_1->y) {
            return false;
        }
        uVar3 |= BPCP_OUTCODE2;
    } else if (param_3->y > param_1->y) {
        uVar4 |= BPCP_OUTCODE2;
    }

    if ((uVar3 & BPCP_OUTCODE2) == 0 && param_2->y < param_0->y) {
        if ((uVar4 & BPCP_OUTCODE2) == 0 && param_3->y < param_0->y) {
            return false;
        }
        uVar3 |= BPCP_OUTCODE3;
    } else if ((uVar4 & BPCP_OUTCODE2) == 0 && param_3->y < param_0->y) {
        uVar4 |= BPCP_OUTCODE3;
    }

    if (uVar3 == 0) {
        return true;
    }
    if (uVar4 == 0) {
        return true;
    }

    uVar3 |= cM3d_CheckBoxEdgePlane_Bevel2DCheck(param_2, param_0, param_1) << 8;
    uVar4 |= cM3d_CheckBoxEdgePlane_Bevel2DCheck(param_3, param_0, param_1) << 8;
    if ((uVar3 & uVar4) != 0) {
        return false;
    }

    uVar3 |= cM3d_CheckBoxEdgePlane_Bevel3DCheck(param_2, param_0, param_1) << 0x18;
    uVar4 |= cM3d_CheckBoxEdgePlane_Bevel3DCheck(param_3, param_0, param_1) << 0x18;
    if ((uVar3 & uVar4) != 0) {
        return false;
    }

    cM3dGLin line(*param_2, *param_3);

    if ((uVar3 ^ uVar4) & BPCP_OUTCODE0) {
        cM3dGPla plane;
        plane.mNormal.x = 1.0f;
        plane.mNormal.y = 0.0f;
        plane.mNormal.z = 0.0f;
        plane.mD = -param_1->x;
        Vec cross;
        if (plane.cross(line, cross) && param_0->y <= cross.y && cross.y <= param_1->y
            && param_0->z <= cross.z && cross.z <= param_1->z)
        {
            return true;
        }
    }

    if ((uVar3 ^ uVar4) & BPCP_OUTCODE1) {
        cM3dGPla plane;
        plane.mNormal.x = -1.0f;
        plane.mNormal.y = 0.0f;
        plane.mNormal.z = 0.0f;
        plane.mD = param_0->x;
        Vec cross;
        if (plane.cross(line, cross) && param_0->y <= cross.y && cross.y <= param_1->y
            && param_0->z <= cross.z && cross.z <= param_1->z)
        {
            return true;
        }
    }

    if ((uVar3 ^ uVar4) & BPCP_OUTCODE2) {
        cM3dGPla plane;
        plane.mNormal.x = 0.0f;
        plane.mNormal.y = 1.0f;
        plane.mNormal.z = 0.0f;
        plane.mD = -param_1->y;
        Vec cross;
        if (plane.cross(line, cross) && param_0->x <= cross.x && cross.x <= param_1->x
            && param_0->z <= cross.z && cross.z <= param_1->z)
        {
            return true;
        }
    }

    if ((uVar3 ^ uVar4) & BPCP_OUTCODE3) {
        cM3dGPla plane;
        plane.mNormal.x = 0.0f;
        plane.mNormal.y = -1.0f;
        plane.mNormal.z = 0.0f;
        plane.mD = param_0->y;
        Vec cross;
        if (plane.cross(line, cross) && param_0->x <= cross.x && cross.x <= param_1->x
            && param_0->z <= cross.z && cross.z <= param_1->z)
        {
            return true;
        }
    }
    
    if ((uVar3 ^ uVar4) & BPCP_OUTCODE4) {
        cM3dGPla plane;
        plane.mNormal.x = 0.0f;
        plane.mNormal.y = 0.0f;
        plane.mNormal.z = 1.0f;
        plane.mD = -param_1->z;
        Vec cross;
        if (plane.cross(line, cross) && param_0->x <= cross.x && cross.x <= param_1->x
            && param_0->y <= cross.y && cross.y <= param_1->y)
        {
            return true;
        }
    }

    if ((uVar3 ^ uVar4) & BPCP_OUTCODE5) {
        cM3dGPla plane;
        plane.mNormal.x = 0.0f;
        plane.mNormal.y = 0.0f;
        plane.mNormal.z = -1.0f;
        plane.mD = param_0->z;
        Vec cross;
        if (plane.cross(line, cross) && param_0->x <= cross.x && cross.x <= param_1->x
            && param_0->y <= cross.y && cross.y <= param_1->y)
        {
            return true;
        }
    }

    return false;
}

bool cM3d_InclusionCheckPosIn3PosBox3d(const Vec* pVecA, const Vec* pVecB, const Vec* pVecC,
                                       const Vec* pVecD, f32 pF) {
    f32 min, max;
    if (pVecA->x < pVecB->x) {
        min = pVecA->x;
        max = pVecB->x;
    } else {
        min = pVecB->x;
        max = pVecA->x;
    }
    if (min > pVecC->x) {
        min = pVecC->x;
    } else if (max < pVecC->x) {
        max = pVecC->x;
    }
    if (min - pF > pVecD->x || max + pF < pVecD->x) {
        return false;
    }

    if (pVecA->z < pVecB->z) {
        min = pVecA->z;
        max = pVecB->z;
    } else {
        min = pVecB->z;
        max = pVecA->z;
    }
    if (min > pVecC->z) {
        min = pVecC->z;
    } else if (max < pVecC->z) {
        max = pVecC->z;
    }
    if (min - pF > pVecD->z || max + pF < pVecD->z) {
        return false;
    }

    if (pVecA->y < pVecB->y) {
        min = pVecA->y;
        max = pVecB->y;
    } else {
        min = pVecB->y;
        max = pVecA->y;
    }
    if (min > pVecC->y) {
        min = pVecC->y;
    } else if (max < pVecC->y) {
        max = pVecC->y;
    }
    if (min - pF > pVecD->y || max + pF < pVecD->y) {
        return false;
    }
    return true;
}

static bool cM3d_InclusionCheckPosIn3PosBox2d(f32 param_1, f32 param_2, f32 param_3,
                                              f32 param_4, f32 param_5, f32 param_6,
                                              f32 param_7, f32 param_8, f32 param_9) {
    f32 f31;
    f32 f30;
    f32 f29;
    f29 = param_9;
    if (param_1 < param_3) {
        f31 = param_1;
        f30 = param_3;
    } else {
        f31 = param_3;
        f30 = param_1;
    }

    if (f31 > param_5) {
        f31 = param_5;   
    } else if (f30 < param_5) {
        f30 = param_5;
    }

    if (f31 - f29 > param_7 || f30 + f29 < param_7) {
        return false;
    } 

    if (param_2 < param_4) {
        f31 = param_2;
        f30 = param_4;
    } else {
        f31 = param_4;
        f30 = param_2;
    }

    if (f31 > param_6) {
        f31 = param_6;
    } else if (f30 < param_6) {
        f30 = param_6;
    }
    
    if (f31 - f29 > param_8 || f30 + f29 < param_8)
    {
        return false;
    }
    return true;
}

static bool cM3d_CrossX_Tri(cM3dGTri const* tri, Vec const* vec, f32 param_2) {
    if (cM3d_IsZero(tri->GetNP()->x)) {
        return false;
    }

    if (!cM3d_InclusionCheckPosIn3PosBox2d(tri->mA.y, tri->mA.z, tri->mB.y, tri->mB.z,
                                           tri->mC.y, tri->mC.z, vec->y, vec->z, 0.005f)) {
        return false;
    }

    f32 tmp = cM3d_VectorProduct2d(tri->mA.y, tri->mA.z, tri->mB.y, tri->mB.z, vec->y, vec->z);

    if (tmp <= param_2
        && cM3d_VectorProduct2d(tri->mB.y, tri->mB.z, tri->mC.y, tri->mC.z, vec->y, vec->z) <= param_2
        && cM3d_VectorProduct2d(tri->mC.y, tri->mC.z, tri->mA.y, tri->mA.z, vec->y, vec->z) <= param_2)
    {
        return true;
    }

    if (tmp >= -param_2
        && cM3d_VectorProduct2d(tri->mB.y, tri->mB.z, tri->mC.y, tri->mC.z, vec->y, vec->z) >= -param_2
        && cM3d_VectorProduct2d(tri->mC.y, tri->mC.z, tri->mA.y, tri->mA.z, vec->y, vec->z) >= -param_2)
    {
        return true;
    }

    return false;
}

static bool cM3d_CrossX_Tri(cM3dGTri const* tri, Vec const* vec) {
    if (cM3d_IsZero(tri->GetNP()->x)) {
        return false;
    }

    if (!cM3d_InclusionCheckPosIn3PosBox2d(tri->mA.y, tri->mA.z, tri->mB.y, tri->mB.z,
                                           tri->mC.y, tri->mC.z, vec->y, vec->z, 0.005f)) {
        return false;
    }

    f32 tmp = cM3d_VectorProduct2d(tri->mA.y, tri->mA.z, tri->mB.y, tri->mB.z, vec->y, vec->z);

    if (tmp <= 20.0f
        && cM3d_VectorProduct2d(tri->mB.y, tri->mB.z, tri->mC.y, tri->mC.z, vec->y, vec->z) <= 20.0f
        && cM3d_VectorProduct2d(tri->mC.y, tri->mC.z, tri->mA.y, tri->mA.z, vec->y, vec->z) <= 20.0f)
    {
        return true;
    }

    if (tmp >= -20.0f
        && cM3d_VectorProduct2d(tri->mB.y, tri->mB.z, tri->mC.y, tri->mC.z, vec->y, vec->z) >= -20.0f
        && cM3d_VectorProduct2d(tri->mC.y, tri->mC.z, tri->mA.y, tri->mA.z, vec->y, vec->z) >= -20.0f)
    {
        return true;
    }

    return false;
}

static bool cM3d_CrossX_LinTri_proc(cM3dGTri const* tri, Vec const* vec) {
    if (!cM3d_InclusionCheckPosIn3PosBox2d(tri->mA.y, tri->mA.z, tri->mB.y, tri->mB.z,
                                           tri->mC.y, tri->mC.z, vec->y, vec->z, 0.005f)) {
        return false;
    }

    f32 tmp = cM3d_VectorProduct2d(tri->mA.y, tri->mA.z, tri->mB.y, tri->mB.z, vec->y, vec->z);

    if (tmp <= 20.0f
        && cM3d_VectorProduct2d(tri->mB.y, tri->mB.z, tri->mC.y, tri->mC.z, vec->y, vec->z) <= 20.0f
        && cM3d_VectorProduct2d(tri->mC.y, tri->mC.z, tri->mA.y, tri->mA.z, vec->y, vec->z) <= 20.0f)
    {
        return true;
    }

    if (tmp >= -20.0f
        && cM3d_VectorProduct2d(tri->mB.y, tri->mB.z, tri->mC.y, tri->mC.z, vec->y, vec->z) >= -20.0f
        && cM3d_VectorProduct2d(tri->mC.y, tri->mC.z, tri->mA.y, tri->mA.z, vec->y, vec->z) >= -20.0f)
    {
        return true;
    }

    return false;
}

bool cM3d_CrossY_Tri(cM3dGTri const* tri, Vec const* vec) {
    if (cM3d_IsZero(tri->GetNP()->y)) {
        return false;
    }

    if (!cM3d_InclusionCheckPosIn3PosBox2d(tri->mA.z, tri->mA.x, tri->mB.z, tri->mB.x,
                                           tri->mC.z, tri->mC.x, vec->z, vec->x, 0.005f)) {
        return false;
    }

    f32 tmp = cM3d_VectorProduct2d(tri->mA.z, tri->mA.x, tri->mB.z, tri->mB.x, vec->z, vec->x);

    if (tmp <= 20.0f
        && cM3d_VectorProduct2d(tri->mB.z, tri->mB.x, tri->mC.z, tri->mC.x, vec->z, vec->x) <= 20.0f
        && cM3d_VectorProduct2d(tri->mC.z, tri->mC.x, tri->mA.z, tri->mA.x, vec->z, vec->x) <= 20.0f)
    {
        return true;
    }

    if (tmp >= -20.0f
        && cM3d_VectorProduct2d(tri->mB.z, tri->mB.x, tri->mC.z, tri->mC.x, vec->z, vec->x) >= -20.0f
        && cM3d_VectorProduct2d(tri->mC.z, tri->mC.x, tri->mA.z, tri->mA.x, vec->z, vec->x) >= -20.0f)
    {
        return true;
    }

    return false;
}

static bool cM3d_CrossY_LinTri_proc(cM3dGTri const* tri, Vec const* vec) {
    if (!cM3d_InclusionCheckPosIn3PosBox2d(tri->mA.z, tri->mA.x, tri->mB.z, tri->mB.x,
                                           tri->mC.z, tri->mC.x, vec->z, vec->x, 0.005f)) {
        return false;
    }

    f32 tmp = cM3d_VectorProduct2d(tri->mA.z, tri->mA.x, tri->mB.z, tri->mB.x, vec->z, vec->x);

    if (tmp <= 20.0f
        && cM3d_VectorProduct2d(tri->mB.z, tri->mB.x, tri->mC.z, tri->mC.x, vec->z, vec->x) <= 20.0f
        && cM3d_VectorProduct2d(tri->mC.z, tri->mC.x, tri->mA.z, tri->mA.x, vec->z, vec->x) <= 20.0f)
    {
        return true;
    }

    if (tmp >= -20.0f
        && cM3d_VectorProduct2d(tri->mB.z, tri->mB.x, tri->mC.z, tri->mC.x, vec->z, vec->x) >= -20.0f
        && cM3d_VectorProduct2d(tri->mC.z, tri->mC.x, tri->mA.z, tri->mA.x, vec->z, vec->x) >= -20.0f)
    {
        return true;
    }

    return false;
}

bool cM3d_CrossY_Tri(Vec const& a, Vec const& b, Vec const& c, cM3dGPla const& plane,
                     Vec const* vec) {
    if (cM3d_IsZero(plane.mNormal.y)) {
        return false;
    }

    if (!cM3d_InclusionCheckPosIn3PosBox2d(a.z, a.x, b.z, b.x,
                                           c.z, c.x, vec->z, vec->x, 0.005f)) {
        return false;
    }

    f32 tmp = cM3d_VectorProduct2d(a.z, a.x, b.z, b.x, vec->z, vec->x);

    if (tmp <= 20.0f
        && cM3d_VectorProduct2d(b.z, b.x, c.z, c.x, vec->z, vec->x) <= 20.0f
        && cM3d_VectorProduct2d(c.z, c.x, a.z, a.x, vec->z, vec->x) <= 20.0f)
    {
        return true;
    }

    if (tmp >= -20.0f
        && cM3d_VectorProduct2d(b.z, b.x, c.z, c.x, vec->z, vec->x) >= -20.0f
        && cM3d_VectorProduct2d(c.z, c.x, a.z, a.x, vec->z, vec->x) >= -20.0f)
    {
        return true;
    }

    return false;
}

bool cM3d_CrossY_Tri_Front(Vec const& a, Vec const& b, Vec const& c, Vec const* vec) {
    if (!cM3d_InclusionCheckPosIn3PosBox2d(a.z, a.x, b.z, b.x,
                                           c.z, c.x, vec->z, vec->x, 0.005f)) {
        return false;
    }

    if (cM3d_VectorProduct2d(a.z, a.x, b.z, b.x, vec->z, vec->x) >= -20.0f
        && cM3d_VectorProduct2d(b.z, b.x, c.z, c.x, vec->z, vec->x) >= -20.0f
        && cM3d_VectorProduct2d(c.z, c.x, a.z, a.x, vec->z, vec->x) >= -20.0f)
    {
        return true;
    }

    return false;
}

static bool cM3d_CrossY_Tri(cM3dGTri const* tri, Vec const* vec, f32* param_2) {
    if (cM3d_CrossY_Tri(tri, vec)) {
        *param_2 = (vec->x * -tri->GetNP()->x - vec->z * tri->GetNP()->z - tri->GetD()) / tri->GetNP()->y;
        return true;
    }

    return false;
}

static bool cM3d_CrossY_Tri(cM3dGTri const* tri, Vec const* vec, f32 param_2) {
    if (cM3d_IsZero(tri->GetNP()->y)) {
        return false;
    }

    if (!cM3d_InclusionCheckPosIn3PosBox2d(tri->mA.z, tri->mA.x, tri->mB.z, tri->mB.x,
                                           tri->mC.z, tri->mC.x, vec->z, vec->x, 0.005f)) {
        return false;
    }

    f32 tmp = cM3d_VectorProduct2d(tri->mA.z, tri->mA.x, tri->mB.z, tri->mB.x, vec->z, vec->x);

    if (tmp <= param_2
        && cM3d_VectorProduct2d(tri->mB.z, tri->mB.x, tri->mC.z, tri->mC.x, vec->z, vec->x) <= param_2
        && cM3d_VectorProduct2d(tri->mC.z, tri->mC.x, tri->mA.z, tri->mA.x, vec->z, vec->x) <= param_2)
    {
        return true;
    }

    if (tmp >= -param_2
        && cM3d_VectorProduct2d(tri->mB.z, tri->mB.x, tri->mC.z, tri->mC.x, vec->z, vec->x) >= -param_2
        && cM3d_VectorProduct2d(tri->mC.z, tri->mC.x, tri->mA.z, tri->mA.x, vec->z, vec->x) >= -param_2)
    {
        return true;
    }

    return false;
}

static bool cM3d_CrossY_Tri(cM3dGTri const* tri, Vec const* vec, cM3d_Range const* range,
                            f32* param_3) {
    if (cM3d_IsZero(tri->GetNP()->y)) {
        return false;
    }

    Vec point;
    point.x = vec->x;
    point.y = range->mMin;
    point.z = vec->z;
    f32 f1 = tri->getPlaneFunc(&point);
    point.y = range->mMax;
    f32 f2 = tri->getPlaneFunc(&point);

    if ((f1 > 0.0f && f2 > 0.0f) || (f1 < 0.0f && f2 < 0.0f)) {
        return false;
    }

    return cM3d_CrossY_Tri(tri, vec, param_3);
}

static bool cM3d_CrossZ_Tri(cM3dGTri const* tri, Vec const* vec, f32 param_2) {
    if (cM3d_IsZero(tri->GetNP()->z)) {
        return false;
    }

    if (!cM3d_InclusionCheckPosIn3PosBox2d(tri->mA.x, tri->mA.y, tri->mB.x, tri->mB.y,
                                           tri->mC.x, tri->mC.y, vec->x, vec->y, 0.005f)) {
        return false;
    }

    f32 tmp = cM3d_VectorProduct2d(tri->mA.x, tri->mA.y, tri->mB.x, tri->mB.y, vec->x, vec->y);

    if (tmp <= param_2
        && cM3d_VectorProduct2d(tri->mB.x, tri->mB.y, tri->mC.x, tri->mC.y, vec->x, vec->y) <= param_2
        && cM3d_VectorProduct2d(tri->mC.x, tri->mC.y, tri->mA.x, tri->mA.y, vec->x, vec->y) <= param_2)
    {
        return true;
    }

    if (tmp >= -param_2
        && cM3d_VectorProduct2d(tri->mB.x, tri->mB.y, tri->mC.x, tri->mC.y, vec->x, vec->y) >= -param_2
        && cM3d_VectorProduct2d(tri->mC.x, tri->mC.y, tri->mA.x, tri->mA.y, vec->x, vec->y) >= -param_2)
    {
        return true;
    }

    return false;
}

static bool cM3d_CrossZ_Tri(cM3dGTri const* tri, Vec const* vec) {
    if (cM3d_IsZero(tri->GetNP()->z)) {
        return false;
    }

    if (!cM3d_InclusionCheckPosIn3PosBox2d(tri->mA.x, tri->mA.y, tri->mB.x, tri->mB.y,
                                           tri->mC.x, tri->mC.y, vec->x, vec->y, 0.005f)) {
        return false;
    }

    f32 tmp = cM3d_VectorProduct2d(tri->mA.x, tri->mA.y, tri->mB.x, tri->mB.y, vec->x, vec->y);

    if (tmp <= 20.0f
        && cM3d_VectorProduct2d(tri->mB.x, tri->mB.y, tri->mC.x, tri->mC.y, vec->x, vec->y) <= 20.0f
        && cM3d_VectorProduct2d(tri->mC.x, tri->mC.y, tri->mA.x, tri->mA.y, vec->x, vec->y) <= 20.0f)
    {
        return true;
    }

    if (tmp >= -20.0f
        && cM3d_VectorProduct2d(tri->mB.x, tri->mB.y, tri->mC.x, tri->mC.y, vec->x, vec->y) >= -20.0f
        && cM3d_VectorProduct2d(tri->mC.x, tri->mC.y, tri->mA.x, tri->mA.y, vec->x, vec->y) >= -20.0f)
    {
        return true;
    }

    return false;
}

static bool cM3d_CrossZ_LinTri_proc(cM3dGTri const* tri, Vec const* vec) {
    if (!cM3d_InclusionCheckPosIn3PosBox2d(tri->mA.x, tri->mA.y, tri->mB.x, tri->mB.y,
                                           tri->mC.x, tri->mC.y, vec->x, vec->y, 0.005f)) {
        return false;
    }

    f32 tmp = cM3d_VectorProduct2d(tri->mA.x, tri->mA.y, tri->mB.x, tri->mB.y, vec->x, vec->y);

    if (tmp <= 20.0f
        && cM3d_VectorProduct2d(tri->mB.x, tri->mB.y, tri->mC.x, tri->mC.y, vec->x, vec->y) <= 20.0f
        && cM3d_VectorProduct2d(tri->mC.x, tri->mC.y, tri->mA.x, tri->mA.y, vec->x, vec->y) <= 20.0f)
    {
        return true;
    }

    if (tmp >= -20.0f
        && cM3d_VectorProduct2d(tri->mB.x, tri->mB.y, tri->mC.x, tri->mC.y, vec->x, vec->y) >= -20.0f
        && cM3d_VectorProduct2d(tri->mC.x, tri->mC.y, tri->mA.x, tri->mA.y, vec->x, vec->y) >= -20.0f)
    {
        return true;
    }

    return false;
}

bool cM3d_Cross_LinTri(cM3dGLin const* line, cM3dGTri const* tri, Vec* vec, bool param_3,
                       bool param_4) {
    if (!cM3d_Cross_LinPla(line, tri, vec, param_3, param_4)) {
        return false;
    }
    
    if ((fabsf(tri->GetNP()->x) < 0.008f || cM3d_CrossX_LinTri_proc(tri, vec))
        && (fabsf(tri->GetNP()->y) < 0.008f || cM3d_CrossY_LinTri_proc(tri, vec))
        && (fabsf(tri->GetNP()->z) < 0.008f || cM3d_CrossZ_LinTri_proc(tri, vec)))
    {
        return true;
    }

    return false;
}

static bool cM3d_Cross_LinTri_Easy(cM3dGTri const* tri, Vec const* vec) {
    if ((cM3d_IsZero(tri->GetNP()->x) || cM3d_CrossX_Tri(tri, vec))
        && (cM3d_IsZero(tri->GetNP()->y) || cM3d_CrossY_Tri(tri, vec))
        && (cM3d_IsZero(tri->GetNP()->z) || cM3d_CrossZ_Tri(tri, vec)))
    {
        return true;
    }

    return false;
}

static bool cM3d_Cross_SphPnt(cM3dGSph const* sph, Vec const* point) {
    return cM3d_LenSq(sph->GetCP(), point) <= sph->GetR() * sph->GetR();
}

bool cM3d_Cross_LinSph(cM3dGLin const* line, cM3dGSph const* sph, Vec* param_2) {
    Vec lin_vec;
    cXyz vec;
    const Vec* center = sph->GetCP();

    f32 max_x = center->x + sph->GetR();
    if (max_x < line->GetStartP().x && max_x < line->GetEndP().x) {
        return false;
    }
    f32 min_x = center->x - sph->GetR();
    if (min_x > line->GetStartP().x && min_x > line->GetEndP().x) {
        return false;
    }

    f32 max_y = center->y + sph->GetR();
    if (max_y < line->GetStartP().y && max_y < line->GetEndP().y) {
        return false;
    }
    f32 min_y = center->y - sph->GetR();
    if (min_y > line->GetStartP().y && min_y > line->GetEndP().y) {
        return false;
    }

    f32 max_z = center->z + sph->GetR();
    if (max_z < line->GetStartP().z && max_z < line->GetEndP().z) {
        return false;
    }
    f32 min_z = center->z - sph->GetR();
    if (min_z > line->GetStartP().z && min_z > line->GetEndP().z) {
        return false;
    }

    VECSubtract(&line->GetEndP(), &line->GetStartP(), &lin_vec);
    f32 len_sq = VECSquareMag(&lin_vec);
    if (cM3d_IsZero(len_sq)) {
        return false;
    }

    VECSubtract(center, &line->GetStartP(), &vec);
    f32 proj = VECDotProduct(&vec, &lin_vec) / len_sq;
    if (proj < 0.0f || proj > 1.0f) {
        if (line->GetStartP().abs2(*center) < line->GetEndP().abs2(*center)) {
            if (cM3d_Cross_SphPnt(sph, &line->GetStartP())) {
                param_2->x = line->GetStartP().x;
                param_2->y = line->GetStartP().y;
                param_2->z = line->GetStartP().z;
                return true;
            }
        } else {
            if (cM3d_Cross_SphPnt(sph, &line->GetEndP())) {
                param_2->x = line->GetEndP().x;
                param_2->y = line->GetEndP().y;
                param_2->z = line->GetEndP().z;
                return true;
            }
        }
        return false;
    }

    Vec proj_vec;
    VECScale(&lin_vec, &proj_vec, proj);
    VECAdd(&proj_vec, &line->GetStartP(), param_2);
    if (cM3d_LenSq(param_2, center) <= sph->GetR() * sph->GetR()) {
        return true;
    }
    return false;
}

int cM3d_Cross_LinSph_CrossPos(cM3dGSph const& sph, cM3dGLin const& line, Vec* param_2,
                               Vec* param_3) {
    int ret;
    Vec line_vec, vec;
    VECSubtract(&line.GetEndP(), &line.GetStartP(), &line_vec);
    VECSubtract(&line.GetStartP(), sph.GetCP(), &vec);
    f32 len_sq = VECDotProduct(&line_vec, &line_vec);
    f32 dVar9 = VECDotProduct(&line_vec, &vec) * 2.0f;
    f32 dVar6 = VECDotProduct(&vec, &vec) - sph.GetR() * sph.GetR();

    if (cM3d_IsZero(len_sq)) {
        if (cM3d_IsZero(dVar9)) {
            return 0;
        } else {
            ret = 1;
            Vec vec2;
            VECScale(&line_vec, &vec2, -dVar6 / dVar9);
            VECAdd(&vec2, &line.GetStartP(), param_2);
        }
    } else {
        f32 f11 = dVar9 * dVar9 - len_sq * 4.0f * dVar6;
        if (cM3d_IsZero(f11)) {
            ret = 1;
            Vec vec2;
            VECScale(&line_vec, &vec2, -dVar9 / (len_sq * 2.0f));
            VECAdd(&vec2, &line.GetStartP(), param_2);
        } else if (f11 < 0.0f) {
            ret = 0;
        } else {
            ret = 2;
            f32 f1 = 1.0f / (len_sq * 2.0f);
            Vec vec2;
            f32 scale1 = f1 * (-dVar9 + sqrtf(f11));
            f32 scale2 = f1 * (-dVar9 - sqrtf(f11));
            VECScale(&line_vec, &vec2, scale1);
            VECAdd(&vec2, &line.GetStartP(), param_2);
            VECScale(&line_vec, &vec2, scale2);
            VECAdd(&vec2, &line.GetStartP(), param_3);
        }
    }

    return ret;
}

bool cM3d_Cross_CylSph(cM3dGCyl const* cyl, cM3dGSph const* sph, f32* param_2) {
    const cXyz* sph_center = sph->GetCP();
    const cXyz* cyl_center = cyl->GetCP();
    f32 radius_sum = cyl->GetR() + sph->GetR();
    f32 dist = sqrtf(cM3d_Len2dSq(sph_center->x, sph_center->z, cyl_center->x, cyl_center->z));

    if (radius_sum < dist) {
        return false;
    }

    if (sph_center->y + sph->GetR() >= cyl_center->y
        && sph_center->y - sph->GetR() <= cyl_center->y + cyl->GetH())
    {
        *param_2 = radius_sum - dist;
        return true;
    }

    return false;
}

bool cM3d_Cross_CylSph(cM3dGCyl const* cyl, cM3dGSph const* sph, Vec* param_2, f32* param_3) {
    const Vec* sph_center = sph->GetCP();
    const Vec* cyl_center = cyl->GetCP();
    f32 radius_sum = cyl->GetR() + sph->GetR();
    f32 dist = sqrtf(cM3d_Len2dSq(sph_center->x, sph_center->z, cyl_center->x, cyl_center->z));

    if (radius_sum < dist) {
        return false;
    }

    if (sph_center->y + sph->GetR() >= cyl_center->y
        && sph_center->y - sph->GetR() <= cyl_center->y + cyl->GetH())
    {
        *param_3 = radius_sum - dist;
        if (!cM3d_IsZero(dist)) {
            f32 scale = cyl->GetR() / dist;
            if (scale <= 1.0f) {
                Vec delta;
                VECSubtract(sph_center, cyl_center, &delta);
                VECScale(&delta, &delta, scale);
                VECAdd(&delta, cyl_center, param_2);
            } else {
                *param_2 = *sph_center;
            }
        } else {
            *param_2 = *sph_center;
        }
        return true;
    }

    return false;
}

bool cM3d_Cross_SphSph(cM3dGSph const* sph1, cM3dGSph const* sph2, f32* param_2) {
    Vec delta;
    VECSubtract(sph1->GetCP(), sph2->GetCP(), &delta);
    f32 dist = VECMag(&delta);
    *param_2 = sph1->GetR() + sph2->GetR() - dist;
    if (*param_2 > G_CM3D_F_ABS_MIN) {
        return true;
    } else {
        *param_2 = 0.0f;
        return false;
    }
}

static bool cM3d_Cross_SphSph(cM3dGSph const* sph1, cM3dGSph const* sph2, f32* param_2,
                              f32* param_3) {
    Vec delta;
    VECSubtract(sph1->GetCP(), sph2->GetCP(), &delta);
    *param_2 = VECMag(&delta);
    *param_3 = sph1->GetR() + sph2->GetR() - *param_2;
    if (*param_3 > G_CM3D_F_ABS_MIN) {
        return true;
    } else {
        *param_3 = 0.0f;
        return false;
    }
}

bool cM3d_Cross_SphSph(const cM3dGSph* pSphereA, const cM3dGSph* pSphereB, Vec* pVecOut) {
    f32 centerDist;
    f32 overlapLen;
    if (cM3d_Cross_SphSph(pSphereA, pSphereB, &centerDist, &overlapLen)) {
        if (!cM3d_IsZero(centerDist)) {
            // could be an inlined function
            f32 tmpF = pSphereB->GetR() / centerDist;
            Vec tmp;
            VECSubtract(&pSphereA->GetC(), &pSphereB->GetC(), &tmp);
            VECScale(&tmp, &tmp, tmpF);
            VECAdd(&tmp, &pSphereB->GetC(), pVecOut);
        } else {
            *pVecOut = pSphereA->GetC();
        }
        return true;
    } else {
        return false;
    }
}

void cM3d_CalcSphVsTriCrossPoint(const cM3dGSph* pSphere, const cM3dGTri* pTriangle, Vec* pVecOut) {
    Vec tmp2;
    Vec tmp;
    VECAdd(&pTriangle->mA, &pTriangle->mB, &tmp);
    VECScale(&tmp, &tmp2, 0.5f);
    f32 sqDist = VECSquareDistance(&tmp2, &pSphere->GetC());
    if (cM3d_IsZero(sqDist)) {
        *pVecOut = pSphere->GetC();
    } else {
        cM3d_InDivPos2(&pSphere->GetC(), &tmp2, pSphere->GetR() / sqDist, pVecOut);
    }
}

bool cM3d_Cross_SphTri(cM3dGSph const* sph, cM3dGTri const* tri, Vec* param_2) {
    if (!cM3d_InclusionCheckPosIn3PosBox3d(&tri->mA, &tri->mB, &tri->mC, sph->GetCP(), sph->GetR())) {
        return false;
    }

    f32 dist = cM3d_SignedLenPlaAndPos(tri, sph->GetCP());
    if (fabsf(dist) > sph->GetR()) {
        return false;
    }

    Vec vec1, vec2;
    VECScale(tri->GetNP(), &vec2, dist);
    VECSubtract(sph->GetCP(), &vec2, &vec1);

    if (fabsf(tri->GetNP()->y) > 0.5f) {
        if (cM3d_CrossY_Tri(tri, &vec1, 0.0f)) {
            if (param_2 != NULL) {
                cM3d_CalcSphVsTriCrossPoint(sph, tri, param_2);
            }
            return true;
        }
    } else if (fabsf(tri->GetNP()->x) > 0.5f) {
        if (cM3d_CrossX_Tri(tri, &vec1, 0.0f)) {
            if (param_2 != NULL) {
                cM3d_CalcSphVsTriCrossPoint(sph, tri, param_2);
            }
            return true;
        }
    } else {
        if (cM3d_CrossZ_Tri(tri, &vec1, 0.0f)) {
            if (param_2 != NULL) {
                cM3d_CalcSphVsTriCrossPoint(sph, tri, param_2);
            }
            return true;
        }
    }

    cM3dGLin line1(tri->mA, tri->mB);
    if (cM3d_Cross_LinSph(&line1, sph)) {
        if (param_2 != NULL) {
            cM3d_CalcSphVsTriCrossPoint(sph, tri, param_2);
        }
        return true;
    }

    cM3dGLin line2(tri->mB, tri->mC);
    if (cM3d_Cross_LinSph(&line2, sph)) {
        if (param_2 != NULL) {
            cM3d_CalcSphVsTriCrossPoint(sph, tri, param_2);
        }
        return true;
    }

    cM3dGLin line3(tri->mC, tri->mA);
    if (cM3d_Cross_LinSph(&line3, sph)) {
        if (param_2 != NULL) {
            cM3d_CalcSphVsTriCrossPoint(sph, tri, param_2);
        }
        return true;
    }

    return false;
}

bool cM3d_Cross_CylCyl(cM3dGCyl const* cyl1, cM3dGCyl const* cyl2, f32* param_2) {
    const Vec& center1 = cyl1->GetC();
    const Vec& center2 = cyl2->GetC();
    f32 delta_x = center1.x - center2.x;
    f32 delta_z = center1.z - center2.z;
    f32 dist_sq = delta_x * delta_x + delta_z * delta_z;
    f32 radius_sum = cyl1->GetR() + cyl2->GetR();
    
    if (dist_sq > radius_sum * radius_sum) {
        *param_2 = 0.0f;
        return false;
    }

    if (center1.y + cyl1->GetH() < center2.y || center1.y > center2.y + cyl2->GetH()) {
        *param_2 = 0.0f;
        return false;
    }

    *param_2 = radius_sum - sqrtf(dist_sq);
    return true;
}

bool cM3d_Cross_CylCyl(cM3dGCyl const* cyl1, cM3dGCyl const* cyl2, Vec* param_2) {
    const Vec& center1 = cyl1->GetC();
    const Vec& center2 = cyl2->GetC();
    f32 delta_x = center1.x - center2.x;
    f32 delta_z = center1.z - center2.z;
    f32 dist_sq = delta_x * delta_x + delta_z * delta_z;
    f32 radius_sum = cyl1->GetR() + cyl2->GetR();
    
    if (dist_sq > radius_sum * radius_sum) {
        return false;
    }

    if (center1.y + cyl1->GetH() < center2.y || center1.y > center2.y + cyl2->GetH()) {
        return false;
    }

    f32 dist = sqrtf(dist_sq);
    if (!cM3d_IsZero(dist)) {
        f32 scale = cyl2->GetR() / dist;
        param_2->y = center2.y + cyl2->GetH() * 0.5f;
        f32 tmp = center1.y;
        if (param_2->y < tmp) {
            param_2->y = tmp;
        } else {
            tmp += cyl1->GetH();
            if (param_2->y > tmp) {
                param_2->y = tmp;
            }
        }
        param_2->x = center2.x + scale * (center1.x - center2.x);
        param_2->z = center2.z + scale * (center1.z - center2.z);
    } else {
        *param_2 = center2;
    }

    return true;
}

bool cM3d_Cross_CylTri(cM3dGCyl const* cyl, cM3dGTri const* tri, Vec* param_2) {
    const Vec& center = cyl->GetC();
    f32 top_y = center.y + cyl->GetH();
    
    if ((center.y > tri->mA.y && center.y > tri->mB.y && center.y > tri->mC.y)
        || (top_y < tri->mA.y && top_y < tri->mB.y && top_y < tri->mC.y))
    {
        return false;
    }

    Vec vec1, vec2;
    f32 min_len_sq = 1.0e9f;
    if (cM3d_Cross_CylPntPnt(cyl, &tri->mA, &tri->mB, &vec1, &vec2)) {
        min_len_sq = cM3d_LenSq(&vec1, &tri->mA);
        *param_2 = vec1;
    }
    if (cM3d_Cross_CylPntPnt(cyl, &tri->mC, &tri->mB, &vec1, &vec2)) {
        f32 len_sq = cM3d_LenSq(&vec1, &tri->mC);
        if (min_len_sq > len_sq) {
            min_len_sq = len_sq;
            *param_2 = vec1;
        }
    }
    if (cM3d_Cross_CylPntPnt(cyl, &tri->mA, &tri->mC, &vec1, &vec2)) {
        f32 len_sq = cM3d_LenSq(&vec1, &tri->mA);
        if (min_len_sq > len_sq) {
            min_len_sq = len_sq;
            *param_2 = vec1;
        }
    }

    if (min_len_sq != 1.0e9f) {
        return true;
    }

    cM3d_Range range;
    range.mMin = center.y;
    range.mMax = top_y;
    f32 tmp;
    if (cM3d_CrossY_Tri(tri, &center, &range, &tmp)) {
        Vec vec3, vec4, vec5, vec6;
        vec3 = center;
        vec3.y = tmp;
        VECAdd(&tri->mA, &tri->mB, &vec6);
        VECScale(&vec6, &vec4, 0.5f);
        VECSubtract(&vec4, &vec3, &vec5);
        f32 dist = sqrtf(vec5.x * vec5.x + vec5.z * vec5.z);

        if (cM3d_IsZero(dist)) {
            param_2->x = vec4.x;
            param_2->y = vec4.y;
            param_2->z = vec4.z;
        } else {
            cM3d_InDivPos1(&vec3, &vec5, cyl->GetR() / dist, param_2);
        }

        return true;
    }

    return false;
}

int cM3d_Cross_CylLin(cM3dGCyl const* cyl, cM3dGLin const* line, Vec* param_2, Vec* param_3) {
    f32 ratio;
    f32 f2;
    f32 fVar5;
    f32 fVar2;
    f32 fVar1;
    f32 fVar6;
    f32 fVar4;
    BOOL bVar4;
    BOOL bVar3;
    BOOL bVar6;
    BOOL bVar5;
    u32 uVar11;
    f32 sp28;
    f32 r_sq;
    int count;

    ratio = 0.0f;
    f2 = 0.0f;
    uVar11 = 0;

    if (cM3d_Cross_CylPnt(cyl, &line->GetStartP()) && cM3d_Cross_CylPnt(cyl, &line->GetEndP())) {
        *param_2 = line->GetStartP();
        *param_3 = line->GetEndP();
        return 2;
    }

    Vec vec1, vec2, vec3;
    Vec vec[4];
    const Vec* center = cyl->GetCP();
    VECSubtract(&line->GetStartP(), center, &vec1);
    VECSubtract(&line->GetEndP(), center, &vec2);
    VECSubtract(&vec2, &vec1, &vec3);
    r_sq = cyl->GetR() * cyl->GetR();

    if (!cM3d_IsZero(vec3.y)) {
        ratio = -vec1.y / vec3.y;
        if (ratio >= 0.0f && ratio <= 1.0f) {
            f32 x = vec1.x + vec3.x * ratio;
            f32 z = vec1.z + vec3.z * ratio;
            if (x * x + z * z < r_sq) {
                uVar11 |= 1;
                vec[0].x = x + center->x;
                vec[0].y = center->y;
                vec[0].z = z + center->z;
            }
        }

        ratio = (cyl->GetH() - vec1.y) / vec3.y;
        if (ratio >= 0.0f && ratio <= 1.0f) {
            f32 x = vec1.x + vec3.x * ratio;
            f32 z = vec1.z + vec3.z * ratio;
            if (x * x + z * z < r_sq) {
                uVar11 |= 2;
                vec[1].x = x + center->x;
                vec[1].y = center->y + cyl->GetH();
                vec[1].z = z + center->z;
            }
        }
    }

    bVar4 = false;
    bVar3 = false;
    fVar5 = vec3.x * vec3.x + vec3.z * vec3.z;
    fVar2 = (vec3.x * vec1.x + vec3.z * vec1.z) * 2.0f;
    fVar1 = vec1.x * vec1.x + vec1.z * vec1.z - r_sq;

    f32 sp0C = fVar5 * 2.0f;
    if (!cM3d_IsZero(sp0C)) {
        fVar6 = fVar2 * fVar2 - fVar5 * 4.0f * fVar1;
        if (fVar6 < 0.0f) {
            return 0;
        }
        if (fVar6 > 0.0f) {
            bVar4 = bVar3 = true;
        } else {
            bVar4 = true;
            bVar3 = false;
        }
        fVar4 = sqrtf(fVar6);
        if (bVar4) {
            ratio = (-fVar2 + fVar4) / sp0C;
        }
        if (bVar3) {
            f2 = (-fVar2 - fVar4) / sp0C;
        }
    } else {
        if (!cM3d_IsZero(fVar2)) {
            bVar4 = true;
            bVar3 = false;
            ratio = -fVar1 / fVar2;
        } else {
            return 0;
        }
    }

    if (bVar4 && !bVar3) {
        if (0.0f > ratio || ratio > 1.0f) {
            return 0;
        }
    } else {
        bVar6 = 0.0f > ratio || ratio > 1.0f;
        bVar5 = 0.0f > f2 || f2 > 1.0f;
        if (bVar6 && bVar5) {
            return 0;
        }
        if (bVar6) {
            bVar4 = false;
        }
        if (bVar5) {
            bVar3 = false;
        }
    }

    if (bVar4) {
        sp28 = vec1.y + ratio * vec3.y;
        if (sp28 < 0.0f || sp28 > cyl->GetH()) {
            bVar4 = false;
        }
    }
    if (bVar3) {
        sp28 = vec1.y + f2 * vec3.y;
        if (sp28 < 0.0f || sp28 > cyl->GetH()) {
            bVar3 = false;
        }
    }

    if (!bVar4 && !bVar3) {
        return 0;
    }

    if (bVar4 && bVar3) {
        Vec vec5, vec6;
        VECAdd(&vec1, center, &vec5);
        uVar11 |= 4;
        VECScale(&vec3, &vec6, ratio);
        VECAdd(&vec6, &vec5, &vec[2]);
        uVar11 |= 8;
        VECScale(&vec3, &vec6, f2);
        VECAdd(&vec6, &vec5, &vec[3]);
    } else if (bVar4) {
        uVar11 |= 4;
        Vec vec5, vec6;
        VECScale(&vec3, &vec5, ratio);
        VECAdd(&vec5, &vec1, &vec6);
        VECAdd(&vec6, center, &vec[2]);
    } else if (bVar3) {
        uVar11 |= 4;
        Vec vec5, vec6;
        VECScale(&vec3, &vec5, f2);
        VECAdd(&vec5, &vec1, &vec6);
        VECAdd(&vec6, center, &vec[2]);
    }

    count = 0;
    for (int i = 0; i < 4; i++) {
        if (uVar11 & (1 << i)) {
            if (count == 0) {
                *param_2 = vec[i];
            } else if (count == 1) {
                if (cM3d_LenSq(&line->GetStartP(), param_2) < cM3d_LenSq(&line->GetStartP(), &vec[i])) {
                    *param_3 = vec[i];
                } else {
                    *param_3 = *param_2;
                    *param_2 = vec[i];
                }
                break;
            }
            count++;
        }
    }
    return count;
}

static int cM3d_Cross_CylPntPnt(const cM3dGCyl* pCylinder, const Vec* pVecStart, const Vec* pVecEnd,
                                Vec* pVecOutA, Vec* pVecOutB) {
    cM3dGLin lin;
    lin.SetStartEnd(*pVecStart, *pVecEnd);
    return cM3d_Cross_CylLin(pCylinder, &lin, pVecOutA, pVecOutB);
}

bool cM3d_Cross_CylPnt(const cM3dGCyl* pCylinder, const Vec* pPoint) {
    f32 dX = pCylinder->GetCP()->getXDiff(pPoint);
    f32 dZ = pCylinder->GetCP()->getZDiff(pPoint);
    f32 maxY = pCylinder->GetCP()->y + pCylinder->GetH();
    if (dX * dX + dZ * dZ < pCylinder->GetR() * pCylinder->GetR() &&
        pCylinder->GetCP()->y < pPoint->y && maxY > pPoint->y)
    {
        return true;
    } else {
        return false;
    }
}

bool cM3d_Cross_CpsCps(cM3dGCps const& cps1, cM3dGCps const& cps2, Vec* param_2) {
    f32 f1, f2;
    int iVar1 = cM3d_Check_LinLin(&cps1, &cps2, &f1, &f2);

    if (iVar1 == 1) {
        return false;
    }

    if (iVar1 == 2) {
        if (f1 > 0.0f && f1 < 1.0f && f2 > 0.0f && f2 < 1.0f) {
            Vec pos1, pos2;
            cps1.CalcPos(&pos1, f1);
            cps2.CalcPos(&pos2, f2);
            if (VECDistance(&pos1, &pos2) < cps1.GetR() + cps2.GetR()) {
                VECAdd(&pos1, &pos2, param_2);
                VECScale(param_2, param_2, 0.5f);
                return true;
            }
        }
    } else if (iVar1 == 3) {
        Vec pos1, pos2;
        if (f1 < 0.0f) {
            pos1 = cps1.GetStart();
            if (f2 < 0.0f) {
                pos2 = cps2.GetStart();
            } else if (f2 > 1.0f) {
                pos2 = cps2.GetEnd();
            } else {
                cps2.CalcPos(&pos2, f2);
            }
        } else if (f1 > 1.0f) {
            pos1 = cps1.GetEnd();
            if (f2 < 0.0f) {
                pos2 = cps2.GetStart();
            } else if (f2 > 1.0f) {
                pos2 = cps2.GetEnd();
            } else {
                cps2.CalcPos(&pos2, f2);
            }
        } else {
            cps1.CalcPos(&pos1, f1);
            if (f2 < 0.0f) {
                pos2 = cps2.GetStart();
            } else if (f2 > 1.0f) {
                pos2 = cps2.GetEnd();
            } else {
                cps2.CalcPos(&pos2, f2);
            }
        }

        if (VECDistance(&pos1, &pos2) < cps1.GetR() + cps2.GetR()) {
            VECAdd(&pos1, &pos2, param_2);
            VECScale(param_2, param_2, 0.5f);
            return true;
        }
        return false;
    }

    return false;
}

inline bool cM3d_Cross_CpsCyl_Check(cM3dGCps const& cps, cM3dGCyl const& cyl, Vec& pos1, Vec& pos2,
                                    Vec* param_4) {
    if (VECDistance(&pos1, &pos2) < cps.GetR() + cyl.GetR()) {
        VECAdd(&pos1, &pos2, param_4);
        VECScale(param_4, param_4, 0.5f);
        return true;
    }
    return false;
}

bool cM3d_Cross_CpsCyl(cM3dGCps const& cps, cM3dGCyl const& cyl, Vec* param_2) {
    cM3dGLin line;

    if (cM3d_Cross_CylPnt(&cyl, &cps.GetEndP())) {
        *param_2 = cps.GetEndP();
        return true;
    }

    if (cM3d_Cross_CylPnt(&cyl, &cps.GetStartP())) {
        *param_2 = cps.GetStartP();
        return true;
    }

    line.GetStartP() = *cyl.GetCP();
    line.GetEndP() = *cyl.GetCP();
    line.GetEndP().y += cyl.GetH();
    Vec vec;
    f32 tmp;

    if (cM3d_Len3dSqPntAndSegLine(&cps, &line.GetEndP(), &vec, &tmp)) {
        f32 dist = VECDistance(&line.GetEndP(), &vec);
        if (dist < cps.GetR()) {
            VECAdd(&line.GetEndP(), &vec, param_2);
            VECScale(param_2, param_2, 0.5f);
            *param_2 = line.GetEndP();
            return true;
        }
    }

    if (cM3d_Len3dSqPntAndSegLine(&cps, &line.GetStartP(), &vec, &tmp)) {
        f32 dist = VECDistance(&line.GetStartP(), &vec);
        if (dist < cps.GetR()) {
            VECAdd(&line.GetStartP(), &vec, param_2);
            VECScale(param_2, param_2, 0.5f);
            return true;
        }
    }

    f32 f1, f2;
    int iVar2 = cM3d_Check_LinLin(&cps, &line, &f1, &f2);

    if (iVar2 == 1) {
        cM3dGSph sph;
        f32 tmp2;
        sph.SetR(cps.GetR());
        sph.SetC(cps.GetStartP());
        return cM3d_Cross_CylSph(&cyl, &sph, param_2, &tmp2);
    } else if (iVar2 == 2) {
        if (f1 >= 0.0f && f1 <= 1.0f && f2 >= 0.0f && f2 <= 1.0f) {
            Vec pos1, pos2;
            cps.CalcPos(&pos1, f1);
            line.CalcPos(&pos2, f2);
            if (VECDistance(&pos1, &pos2) < cps.GetR() + cyl.GetR()) {
                VECAdd(&pos1, &pos2, param_2);
                VECScale(param_2, param_2, 0.5f);
                return true;
            }
        }
    } else if (iVar2 == 3) {
        Vec pos1, pos2;
        if (f1 < 0.0f) {
            pos1 = cps.GetStart();
            if (f2 < 0.0f) {
                pos2 = line.GetStart();
                return cM3d_Cross_CpsCyl_Check(cps, cyl, pos1, pos2, param_2);
            }
            if (f2 > 1.0f) {
                pos2 = line.GetEnd();
                return cM3d_Cross_CpsCyl_Check(cps, cyl, pos1, pos2, param_2);
            }
            line.CalcPos(&pos2, f2);
        } else if (f1 > 1.0f) {
            pos1 = cps.GetEnd();
            if (f2 < 0.0f) {
                pos2 = line.GetStart();
                return cM3d_Cross_CpsCyl_Check(cps, cyl, pos1, pos2, param_2);
            }
            if (f2 > 1.0f) {
                pos2 = line.GetEnd();
                return cM3d_Cross_CpsCyl_Check(cps, cyl, pos1, pos2, param_2);
            }
            line.CalcPos(&pos2, f2);
        } else {
            cps.CalcPos(&pos1, f1);
            if (f2 < 0.0f) {
                pos2 = line.GetStart();
                return cM3d_Cross_CpsCyl_Check(cps, cyl, pos1, pos2, param_2);
            }
            if (f2 > 1.0f) {
                pos2 = line.GetEnd();
                return cM3d_Cross_CpsCyl_Check(cps, cyl, pos1, pos2, param_2);
            }
            line.CalcPos(&pos2, f2);
        }

        if (VECDistance(&pos1, &pos2) < cps.GetR() + cyl.GetR()) {
            VECAdd(&pos1, &pos2, param_2);
            VECScale(param_2, param_2, 0.5f);
            return true;
        }
        return false;
    }

    return false;
}

static void cM3d_Cross_CpsSph_CrossPos(cM3dGCps const& param_1, cM3dGSph const& param_2,
                                           Vec const& param_3, Vec* param_4) {
    Vec aVStack_70;
    Vec VStack_7c;
    int iVar5 = cM3d_Cross_LinSph_CrossPos(param_2, param_1, &aVStack_70, &VStack_7c);
    if (iVar5 == 1) {
        *param_4 = aVStack_70;
    } else if (iVar5 == 2) {
        f32 dVar8 = VECSquareDistance(&aVStack_70, &param_1.GetStartP());
        f32 dVar9 = VECSquareDistance(&VStack_7c, &param_1.GetStartP());
        if (dVar8 < dVar9) {
            *param_4 = aVStack_70;
        } else {
            *param_4 = VStack_7c;
        }
    } else {
        f32 dVar8 = param_1.GetR() + param_2.GetR();
        *param_4 = param_3;
        if (cM3d_IsZero(dVar8)) {
            *param_4 = param_3;
        }
        Vec auStack_88;
        VECSubtract(&param_3, param_2.GetCP(), &auStack_88);
        VECScale(&auStack_88, &auStack_88, 0.5f);
        VECAdd(param_4, &auStack_88, param_4);
    }
}


bool cM3d_Cross_CpsSph(cM3dGCps const& param_1, cM3dGSph const& param_2, Vec* param_3) {
    f32 local_38 = VECDistance(&param_1.GetStartP(), param_2.GetCP());
    if (local_38 < param_1.GetR() + param_2.GetR()) {
        cM3d_Cross_CpsSph_CrossPos(param_1, param_2, param_1.GetStartP(), param_3);
        return true;
    }
    local_38 = VECDistance(&param_1.GetEndP(), param_2.GetCP());
    if (local_38 < param_1.GetR() + param_2.GetR()) {
        cM3d_Cross_CpsSph_CrossPos(param_1, param_2, param_1.GetEndP(), param_3);
        return true;
    }
    Vec auStack_34;
    if (cM3d_Len3dSqPntAndSegLine(&param_1, param_2.GetCP(), &auStack_34, &local_38) != 0) {
        if (sqrtf(local_38) < param_1.GetR() + param_2.GetR()) {
            cM3d_Cross_CpsSph_CrossPos(param_1, param_2, auStack_34, param_3);
            return true;
        }
    }
    return false;
}

bool cM3d_Cross_TriTri(cM3dGTri const& tri1, cM3dGTri const& tri2, Vec* param_2) {
    f32 f1 = tri1.getPlaneFunc(&tri2.mA);
    f32 f2 = tri1.getPlaneFunc(&tri2.mB);
    f32 f3 = tri1.getPlaneFunc(&tri2.mC);
    if ((f1 > 0.0f && f2 > 0.0f && f3 > 0.0f) || (f1 < 0.0f && f2 < 0.0f && f3 < 0.0f)) {
        return false;
    }

    f1 = tri2.getPlaneFunc(&tri1.mA);
    f2 = tri2.getPlaneFunc(&tri1.mB);
    f3 = tri2.getPlaneFunc(&tri1.mC);
    if ((f1 > 0.0f && f2 > 0.0f && f3 > 0.0f) || (f1 < 0.0f && f2 < 0.0f && f3 < 0.0f)) {
        return false;
    }

    cM3dGLin line;

    line.SetStartEnd(tri2.mA, tri2.mB);
    if (tri1.cross(&line, param_2, false, false)) {
        return true;
    }

    line.SetStartEnd(tri2.mB, tri2.mC);
    if (tri1.cross(&line, param_2, false, false)) {
        return true;
    }

    line.SetStartEnd(tri2.mA, tri2.mB);
    if (tri1.cross(&line, param_2, false, false)) {
        return true;
    }

    line.SetStartEnd(tri1.mA, tri1.mB);
    if (tri2.cross(&line, param_2, false, false)) {
        return true;
    }

    line.SetStartEnd(tri1.mB, tri1.mC);
    if (tri2.cross(&line, param_2, false, false)) {
        return true;
    }

    line.SetStartEnd(tri1.mA, tri1.mB);
    if (tri2.cross(&line, param_2, false, false)) {
        return true;
    }

    return false;
}

inline f32 cM3d_2LinCenter(cM3dGLin const& pLinA, f32 pLinAF, cM3dGLin const& pLinB, f32 pLinBF,
                            Vec* pVecOut) {
    Vec tmp, tmp2;
    pLinA.CalcPos(&tmp, pLinAF);
    pLinB.CalcPos(&tmp2, pLinBF);
    VECAdd(&tmp, &tmp2, pVecOut);
    VECScale(pVecOut, pVecOut, 0.5f);
    return VECDistance(&tmp, &tmp2);
}

bool cM3d_Cross_CpsTri(cM3dGCps const& cps, cM3dGTri tri, Vec* param_2) {
    cM3dGSph sph;
    sph.SetC(cps.GetStartP());
    sph.SetR(cps.GetR());
    if (cM3d_Cross_SphTri(&sph, &tri, param_2)) {
        return true;
    }

    sph.SetC(cps.GetEndP());
    sph.SetR(cps.GetR());
    if (cM3d_Cross_SphTri(&sph, &tri, param_2)) {
        return true;
    }

    if (!cM3d_Cross_LinPla(&cps, &tri, param_2, true, true)) {
        return false;
    }

    if (cM3d_Cross_LinTri_Easy(&tri, param_2)) {
        return true;
    }

    cM3dGLin line;
    f32 f1, f2;

    line.SetStartEnd(tri.mA, tri.mB);
    if (cM3d_Check_LinLin(&cps, &line, &f1, &f2) >= 2
        && f1 > 0.0f && f1 < 1.0f && f2 > 0.0f && f2 < 1.0f
        && cM3d_2LinCenter(cps, f1, line, f2, param_2) < cps.GetR())
    {
        return true;
    }

    line.SetStartEnd(tri.mB, tri.mC);
    if (cM3d_Check_LinLin(&cps, &line, &f1, &f2) >= 2
        && f1 > 0.0f && f1 < 1.0f && f2 > 0.0f && f2 < 1.0f
        && cM3d_2LinCenter(cps, f1, line, f2, param_2) < cps.GetR())
    {
        return true;
    }

    line.SetStartEnd(tri.mC, tri.mA);
    if (cM3d_Check_LinLin(&cps, &line, &f1, &f2) >= 2
        && f1 > 0.0f && f1 < 1.0f && f2 > 0.0f && f2 < 1.0f
        && cM3d_2LinCenter(cps, f1, line, f2, param_2) < cps.GetR())
    {
        return true;
    }

    return false;
}

void cM3d_CalcVecAngle(const Vec& pVec, s16* pOutA, s16* pOutB) {
    *pOutA = -cM_atan2s(-pVec.z * pVec.y, 1.0f);
    *pOutB = cM_atan2s(-pVec.x * pVec.y, 1.0f);
}

void cM3d_CalcVecZAngle(Vec const& param_0, csXyz* param_1) {
    param_1->x = -cM_atan2s(param_0.y, sqrtf(param_0.x * param_0.x + param_0.z * param_0.z));
    param_1->y = cM_atan2s(param_0.x, param_0.z);
    param_1->z = 0;
}

#ifdef DEBUG

bool cM3d_UpMtx_Base(const Vec& param_1, const Vec& param_2, Mtx param_3) {
    Vec local_3c;
    Vec auStack_48;
    f32 dVar8;
    f32 angle;
    
    if (cM3d_IsZero(PSVECMag(&param_2))) {
        MTXIdentity(param_3);
        return false;
    }

    PSVECNormalize(&param_2, &auStack_48);
    PSVECCrossProduct(&param_1, &auStack_48, &local_3c);

    if (cM3d_IsZero(PSVECMag(&local_3c))) {
        local_3c.x = 1.0f;
        local_3c.y = 0.0f;
        local_3c.z = 0.0f;
    }

    dVar8 = PSVECDotProduct(&param_1, &auStack_48);
    if (dVar8 > 1.0f) {
        dVar8 = 1.0f;
    } else if (dVar8 < -1.0f) {
        dVar8 = -1.0f;
    }

    angle = acosf(dVar8);
    C_MTXRotAxisRad(param_3, &local_3c, angle);
    return true;
}

static Vec base_y = {0.0f, 1.0f, 0.0f};

bool cM3d_UpMtx(const Vec& param_1, Mtx param_2) {
    return cM3d_UpMtx_Base(base_y, param_1, param_2);
}

#endif

void cM3d_PlaneCrossLineProcWork(f32 f1, f32 f2, f32 f3, f32 f4, f32 f5, f32 f6, f32 f7, f32* pF1,
                                 f32* pF2) {
    *pF1 = ((f2 * f7) - (f4 * f6)) / f5;
    *pF2 = ((f3 * f6) - (f1 * f7)) / f5;
}

static int cM3d_2PlaneCrossLine(const cM3dGPla& pPlaneA, const cM3dGPla& pPlaneB,
                                cM3dGLin* pLinOut) {
    Vec tmp;
    VECCrossProduct(pPlaneA.GetNP(), pPlaneB.GetNP(), &tmp);
    if (cM3d_IsZero(tmp.x) && cM3d_IsZero(tmp.y) && cM3d_IsZero(tmp.z)) {
        return 0;
    } else {
        f32 absTX = fabsf(tmp.x);
        f32 absTY = fabsf(tmp.y);
        f32 absTZ = fabsf(tmp.z);
        if (absTX >= absTY && absTX >= absTZ) {
            cM3d_PlaneCrossLineProcWork(pPlaneA.GetNP()->y, pPlaneA.GetNP()->z, pPlaneB.GetNP()->y,
                                        pPlaneB.GetNP()->z, tmp.x, pPlaneA.GetD(), pPlaneB.GetD(),
                                        &pLinOut->GetStartP().y, &pLinOut->GetStartP().z);
            pLinOut->GetStartP().x = 0.0f;
        } else if (absTY >= absTX && absTY >= absTZ) {
            cM3d_PlaneCrossLineProcWork(pPlaneA.GetNP()->z, pPlaneA.GetNP()->x, pPlaneB.GetNP()->z,
                                        pPlaneB.GetNP()->x, tmp.y, pPlaneA.GetD(), pPlaneB.GetD(),
                                        &pLinOut->GetStartP().z, &pLinOut->GetStartP().x);
            pLinOut->GetStartP().y = 0.0f;
        } else {
            cM3d_PlaneCrossLineProcWork(pPlaneA.GetNP()->x, pPlaneA.GetNP()->y, pPlaneB.GetNP()->x,
                                        pPlaneB.GetNP()->y, tmp.z, pPlaneA.GetD(), pPlaneB.GetD(),
                                        &pLinOut->GetStartP().x, &pLinOut->GetStartP().y);
            pLinOut->GetStartP().z = 0.0f;
        }
        f32 scale = VECMag(&pLinOut->GetStartP());
        if (cM3d_IsZero(scale)) {
            scale = 1.0f;
        }
        VECScale(&tmp, &tmp, scale);
        VECAdd(&pLinOut->GetStartP(), &tmp, &pLinOut->GetEndP());
        return 1;
    }
}

BOOL cM3d_3PlaneCrossPos(const cM3dGPla& pPlaneA, const cM3dGPla& pPlaneB, const cM3dGPla& pPlaneC,
                         Vec* pVecOut) {
    cM3dGLin lin;
    if (!cM3d_2PlaneCrossLine(pPlaneA, pPlaneB, &lin)) {
        return false;
    } else {
        const Vec* end = &lin.GetEndP();
        f32 tmpf1 = pPlaneC.getPlaneFunc(&lin.GetStartP());
        f32 tmpf2 = pPlaneC.getPlaneFunc(end);
        if (!cM3d_CrossInfLineVsInfPlane_proc(tmpf1, tmpf2, &lin.GetStartP(), end, pVecOut)) {
            return false;
        } else {
            return true;
        }
    }
}

f32 cM3d_lineVsPosSuisenCross(const cM3dGLin* pLine, const Vec* pPoint, Vec* pVecOut) {
    Vec tmp1;
    Vec tmp2;
    Vec tmp3;
    pLine->CalcVec(&tmp1);
    f32 diffLen = VECSquareMag(&tmp1);
    if (cM3d_IsZero(diffLen)) {
        *pVecOut = *pPoint;
        return 0.0f;
    } else {
        VECSubtract(pPoint, &pLine->GetStartP(), &tmp2);
        f32 retVal = VECDotProduct(&tmp2, &tmp1) / diffLen;
        VECScale(&tmp1, &tmp3, retVal);
        VECAdd(&tmp3, &pLine->GetStartP(), pVecOut);
        return retVal;
    }
}

f32 cM3d_lineVsPosSuisenCross(const Vec& pLinePointA, const Vec& pLinePointB, const Vec& pPoint,
                              Vec* pVecOut) {
    Vec tmp1;
    Vec tmp2;
    Vec tmp3;
    VECSubtract(&pLinePointB, &pLinePointA, &tmp1);
    f32 diffLen = VECSquareMag(&tmp1);
    if (cM3d_IsZero(diffLen)) {
        *pVecOut = pPoint;
        return 0.0f;
    } else {
        VECSubtract(&pPoint, &pLinePointA, &tmp2);
        f32 dotProd = VECDotProduct(&tmp2, &tmp1);
        f32 retVal = dotProd / diffLen;
        VECScale(&tmp1, &tmp3, retVal);
        VECAdd(&tmp3, &pLinePointA, pVecOut);
        return retVal;
    }
}

int cM3d_2PlaneLinePosNearPos(const cM3dGPla& pPlaneA, const cM3dGPla& pPlaneB, const Vec* pVec,
                              Vec* pVecOut) {
    cM3dGLin lin;
    if (!cM3d_2PlaneCrossLine(pPlaneA, pPlaneB, &lin)) {
        return FALSE;
    } else {
        cM3d_lineVsPosSuisenCross(&lin, pVec, pVecOut);
        return TRUE;
    }
}

void cM3d_CrawVec(const Vec& pVecA, const Vec& pVecB, Vec* pVecOut) {
    Vec tmp;
    VECScale(&pVecA, &tmp, fabsf(pVecB.x * pVecA.x + pVecB.y * pVecA.y + pVecB.z * pVecA.z));
    VECAdd(&tmp, &pVecB, pVecOut);
}
