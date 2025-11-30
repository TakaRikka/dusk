/**
 * d_spline_path.cpp
 * dolzel2 - Spline Path Management
 */

#include "d/dolzel.h" // IWYU pragma: keep

#include "d/d_spline_path.h"
#include "d/d_path.h"
#include "d/d_stage.h"

void d2DBSplinePath::Init(s32 param_0, s32 param_1) {
    s32 tmp;
    s32 tmp2;

    field_0x00 = 0;
    field_0x04 = param_0;
    field_0x08 = 1;
    field_0x0c = param_1;

    tmp2 = field_0x0c != 0 ? field_0x0c - 1 : 1;
    tmp = field_0x04 < 2 ? 1 : field_0x04 - 2;
    field_0x10 = (f32)tmp / (f32)tmp2;

    field_0x2c = 0;
}

bool d2DBSplinePath::Step() {
    if (field_0x00 >= field_0x0c) {
        field_0x08 = 0;
    }
    if (field_0x08 == 1) {
        field_0x08 = 2;
    }
    if (field_0x08 == 2) {
        if (field_0x00 <= field_0x0c - 1) {
            f32 fVar2 = field_0x10 * field_0x00;
            int uVar1 = fVar2;
            fVar2 -= uVar1;
            if (field_0x00 == field_0x0c - 1) {
                field_0x08 = 3;
                fVar2 = 1.0f;
            }
            mCurveKey[0] = (uVar1 < field_0x04 - 1) ? uVar1 : field_0x04 - 1;
            mCurveKey[1] = (uVar1 + 1 < field_0x04 - 1) ? uVar1 + 1 : field_0x04 - 1;
            mCurveKey[2] = (uVar1 + 2 < field_0x04 - 1) ? uVar1 + 2 : field_0x04 - 1;
            f32 fVar7 = 1.0f - fVar2;
            mCurveWeight[0] = fVar7 * fVar7 * 0.5f;
            mCurveWeight[1] = (fVar2 * (1.0f - fVar2)) + 0.5f;
            mCurveWeight[2] = fVar2 * fVar2 * 0.5f;
            field_0x00++;
        } else {
            field_0x08 = 0;
        }
    }
    return field_0x08 == 2 || field_0x08 == 3;
}

cXyz d2DBSplinePath::Calc(cXyz* v) {
    cXyz ret;
    ret.x = mCurveWeight[0] * v[mCurveKey[0]].x + mCurveWeight[1] * v[mCurveKey[1]].x + mCurveWeight[2] * v[mCurveKey[2]].x;
    ret.y = mCurveWeight[0] * v[mCurveKey[0]].y + mCurveWeight[1] * v[mCurveKey[1]].y + mCurveWeight[2] * v[mCurveKey[2]].y;
    ret.z = mCurveWeight[0] * v[mCurveKey[0]].z + mCurveWeight[1] * v[mCurveKey[1]].z + mCurveWeight[2] * v[mCurveKey[2]].z;
    return ret;
}

f32 d2DBSplinePath::Calc(f32* param_0) {
    return mCurveWeight[0] * param_0[mCurveKey[0]] +
           mCurveWeight[1] * param_0[mCurveKey[1]] +
           mCurveWeight[2] * param_0[mCurveKey[2]];
}

f32 d2DBSplinePath::Spot(f32* param_1, f32 param_2) {
    int uVar10;
    if (field_0x04 < 2) {
        uVar10 = 1;
    } else {
        uVar10 = field_0x04 - 2;
    }
    f32 fVar5 = param_2 * uVar10;
    int uVar11 = fVar5;
    fVar5 = fVar5 - uVar11;

    int iVar9 = (uVar11 < field_0x04 - 1) ? uVar11 : field_0x04 - 1;
    int iVar8 = (uVar11 + 1 < field_0x04 - 1) ? uVar11 + 1 : field_0x04 - 1;
    int iVar7 = (uVar11 + 2 < field_0x04 - 1) ? uVar11 + 2 : field_0x04 - 1;
    f32 fVar6 = 1.0f - fVar5;
    return fVar6 * fVar6 * 0.5f * param_1[iVar9] + (fVar5 * fVar6 + 0.5f) * param_1[iVar8] +
           fVar5 * fVar5 * 0.5f * param_1[iVar7];
}

void dPathCurve::Init(dPath* i_path) {
    mpPath = i_path;
    field_0x00 = 1;
    field_0x04 = 0;
}

cXyz dPathCurve::bSpline2(f32 param_0) {
    cXyz local_7c;
    int uVar12 = mpPath->m_num;
    if (uVar12 <= 1) {
        return mpPath->m_points[0].m_position;
    } else if (uVar12 == 2) {
        local_7c.x = param_0 * (mpPath->m_points[1].m_position.x -
                                               mpPath->m_points[0].m_position.x);
        local_7c.y = param_0 * (mpPath->m_points[1].m_position.y -
                                               mpPath->m_points[0].m_position.y);
        local_7c.z = param_0 * (mpPath->m_points[1].m_position.z -
                                               mpPath->m_points[0].m_position.z);
        local_7c += mpPath->m_points[0].m_position;
        return local_7c;
    } else {
        f32 fVar2 = uVar12 * param_0;
        int uVar4 = fVar2;
        fVar2 -= uVar4;
        f32 fVar8 = 1.0f - fVar2;
        f32 fVar7 = fVar8 * fVar8 * 0.5f;
        f32 fVar6 = fVar2 * fVar8 + 0.5f;
        f32 fVar5 = fVar2 * fVar2 * 0.5f;

        int iVar10;
        int uVar9;
        int iVar8;
        if (uVar4 <= 0) {
            iVar10 = 0;
            uVar9 = 0;
            iVar8 = 1;
        } else if (uVar4 >= uVar12) {
            iVar10 = uVar12 - 2;
            uVar9 = uVar12 - 1;
            iVar8 = uVar12 - 1;
        } else {
            iVar10 = uVar4 - 1;
            uVar9 = uVar4;
            iVar8 = uVar4 + 1;
        }
        Vec* point11 = &mpPath->m_points[iVar10].m_position;
        Vec* point10 = &mpPath->m_points[uVar9].m_position;
        Vec* point8 = &mpPath->m_points[iVar8].m_position;
        local_7c.x = point11->x * fVar7 + point10->x * fVar6 + point8->x * fVar5;
        local_7c.y = point11->y * fVar7 + point10->y * fVar6 + point8->y * fVar5;
        local_7c.z = point11->z * fVar7 + point10->z * fVar6 + point8->z * fVar5;
        return local_7c;
    }
}
