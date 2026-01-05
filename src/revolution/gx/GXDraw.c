#include <sdk_math.h>

#include <revolution/gx.h>
#include <revolution/os.h>

#include "__gx.h"

static GXVtxDescList vcd[27];
static GXVtxAttrFmtList vat[27];

static void GetVertState(void) {
    GXGetVtxDescv(vcd);
    GXGetVtxAttrFmtv(GX_VTXFMT3, vat);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_NRM, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT3, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT3, GX_VA_NRM, GX_NRM_XYZ, GX_F32, 0);
}

static void RestoreVertState(void) {
    GXSetVtxDescv(vcd);
    GXSetVtxAttrFmtv(GX_VTXFMT3, vat);
}

static void vsub(f32 p1[3], f32 p2[3], f32 u[3]) {
    u32 i;

    for (i = 0; i < 3; i++) {
        u[i] = p2[i] - p1[i];
    }
}

static void vcross(f32 u[3], f32 v[3], f32 n[3]) {
    f32 n1[3];

    n1[0] = (u[1] * v[2]) - (u[2] * v[1]);
    n1[1] = (u[2] * v[0]) - (u[0] * v[2]);
    n1[2] = (u[0] * v[1]) - (u[1] * v[0]);
    n[0] = n1[0];
    n[1] = n1[1];
    n[2] = n1[2];
}

static void normalize(f32 v[3]) {
    f32 d = sqrtf((v[0] * v[0]) + (v[1] * v[1]) + (v[2] * v[2]));

    ASSERTMSGLINE(137, d != 0.0f, "normalize: zero length vector");
    v[0] /= d;
    v[1] /= d;
    v[2] /= d;
}

static void myvertex(f32 v[3], f32 n[3]) {
    GXPosition3f32(v[0], v[1], v[2]);
    GXNormal3f32(n[0], n[1], n[2]);
}

static void DumpTriangle(f32 v0[3], f32 v1[3], f32 v2[3]) {
    GXBegin(GX_TRIANGLES, GX_VTXFMT3, 3);
    myvertex(v0, v0);
    myvertex(v1, v1);
    myvertex(v2, v2);
    GXEnd();
}

static void Subdivide(u8 depth, f32 v0[3], f32 v1[3], f32 v2[3]) {
    f32 v01[3];
    f32 v12[3];
    f32 v20[3];
    u32 i;

    if (depth == 0) {
        DumpTriangle(v0, v1, v2);
        return;
    }

    for (i = 0; i < 3; i++) {
        v01[i] = v0[i] + v1[i];
        v12[i] = v1[i] + v2[i];
        v20[i] = v2[i] + v0[i];
    }

    normalize(v01);
    normalize(v12);
    normalize(v20);
    Subdivide(depth - 1, v0, v01, v20);
    Subdivide(depth - 1, v1, v12, v01);
    Subdivide(depth - 1, v2, v20, v12);
    Subdivide(depth - 1, v01, v12, v20);
}

static void SubDivTriangle(u8 depth, u8 i, f32 (*data)[3], u8 (*ndx)[3]) {
    f32 *x0 = data[ndx[i][0]];
    f32 *x1 = data[ndx[i][1]];
    f32 *x2 = data[ndx[i][2]];

    Subdivide(depth, x0, x1, x2);
}

void GXDrawCylinder(u8 numEdges) {
    s32 i;
    f32 top;
    f32 bottom;
    f32 x[100];
    f32 y[100];
    f32 angle;

    top = 1.0f;
    bottom = -top;
    ASSERTMSGLINE(222, numEdges <= 99, "GXDrawCylinder: too many edges");

    GetVertState();

    for (i = 0; i <= numEdges; i++) {
        angle = (3.1415927f * (2.0f * i)) / numEdges;
        x[i] = cosf(angle);
        y[i] = sinf(angle);
    }

    GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT3, (numEdges + 1) * 2);
    for (i = 0; i <= numEdges; i++) {
        GXPosition3f32(x[i], y[i], bottom);
        GXNormal3f32(x[i], y[i], 0.0f);
        GXPosition3f32(x[i], y[i], top);
        GXNormal3f32(x[i], y[i], 0.0f);

    }
    GXEnd();

    GXBegin(GX_TRIANGLEFAN, GX_VTXFMT3, numEdges + 2);
    GXPosition3f32(0.0f, 0.0f, top);
    GXNormal3f32(0.0f, 0.0f, 1.0f);
    for (i = 0; i <= numEdges; i++) {
        GXPosition3f32(x[i], -y[i], top);
        GXNormal3f32(0.0f, 0.0f, 1.0f);

    }
    GXEnd();

    GXBegin(GX_TRIANGLEFAN, GX_VTXFMT3, numEdges + 2);
    GXPosition3f32(0.0f, 0.0f, bottom);
    GXNormal3f32(0.0f, 0.0f, -1.0f);
    for (i = 0; i <= numEdges; i++) {
        GXPosition3f32(x[i], y[i], bottom);
        GXNormal3f32(0.0f, 0.0f, -1.0f);
    }
    GXEnd();

    RestoreVertState();
}

void GXDrawTorus(f32 rc, u8 numc, u8 numt) {
    GXAttrType ttype;
    s32 i, j, k;
    f32 s, t;
    f32 x, y, z;
    f32 twopi = 6.2831855f;
    f32 rt;

    ASSERTMSGLINE(316, rc < 1.0f, "GXDrawTorus: doughnut too fat");

    rt = 1.0f - rc;
    GXGetVtxDesc(GX_VA_TEX0, &ttype);
    GetVertState();

    if (ttype != GX_NONE) {
        GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
        GXSetVtxAttrFmt(GX_VTXFMT3, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    }

    for (i = 0; i < numc; i++) {
        GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT3, (numt + 1) * 2);
        for (j = 0; j <= numt; j++) {
            for (k = 1; k >= 0; k--) {
                s = (i + k) % numc;
                t = j % numt;
                x = (rt - rc * cosf(s * twopi / numc)) * cosf(t * twopi / numt);
                y = (rt - rc * cosf(s * twopi / numc)) * sinf(t * twopi / numt);
                z = rc * sinf(s * twopi / numc);
                GXPosition3f32(x, y, z);
                x = -cosf(t * twopi / numt) * cosf(s * twopi / numc);
                y = -sinf(t * twopi / numt) * cosf(s * twopi / numc);
                z = sinf(s * twopi / numc);
                GXNormal3f32(x, y, z);
                if (ttype != GX_NONE) {
                    GXTexCoord2f32((i + k) / (f32)numc, j / (f32)numt);
                }
            }
        }
        GXEnd();
    }
    RestoreVertState();
}

void GXDrawSphere(u8 numMajor, u8 numMinor) {
    GXAttrType ttype;
    f32 radius = 1.0f;
    f32 majorStep = 3.1415927f / numMajor;
    f32 minorStep = 6.2831855f / numMinor;
    s32 i, j;
    f32 a, b;
    f32 r0, r1;
    f32 z0, z1;
    f32 c;

    GXGetVtxDesc(GX_VA_TEX0, &ttype);
    GetVertState();

    if (ttype != GX_NONE) {
        GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
        GXSetVtxAttrFmt(GX_VTXFMT3, GX_VA_TEX0, GX_TEX_ST, GX_RGBA6, 0);
    }

    for (i = 0; i < numMajor; i++) {
        a = i * majorStep;
        b = a + majorStep;
        r0 = radius * sinf(a);
        r1 = radius * sinf(b);
        z0 = radius * cosf(a);
        z1 = radius * cosf(b);
        GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT3, (numMinor + 1) * 2);
        for (j = 0; j <= numMinor; j++) {
            f32 x, y;
            c = j * minorStep;
            x = cosf(c);
            y = sinf(c);
            GXPosition3f32(x * r1, y * r1, z1);
            GXNormal3f32((x * r1) / radius, (y * r1) / radius, z1 / radius);
            if (ttype != GX_NONE) {
                GXTexCoord2f32((f32)j / (f32)numMinor, (f32)(i + 1) / (f32)numMajor);
            }
            GXPosition3f32(x * r0, y * r0, z0);
            GXNormal3f32((x * r0) / radius, (y * r0) / radius, z0 / radius);
            if (ttype != GX_NONE) {
                GXTexCoord2f32((f32)j / (f32)numMinor, (f32)i / (f32)numMajor);
            }
        }
        GXEnd();
    }
    RestoreVertState();
}

static void GXDrawCubeFace(f32 nx, f32 ny, f32 nz, f32 tx, f32 ty, f32 tz, f32 bx, f32 by, f32 bz, GXAttrType binormal, GXAttrType texture) {
    GXPosition3f32(0.57735026f * (nx + tx + bx), 0.57735026f * (ny + ty + by), 0.57735026f * (nz + tz + bz));
    GXNormal3f32(nx, ny, nz);

    if (binormal != GX_NONE) {
        GXNormal3f32(tx, ty, tz);
        GXNormal3f32(bx, by, bz);
    }

    if (texture != GX_NONE) {
        GXTexCoord2s8(1, 1);
    }

    GXPosition3f32(0.57735026f * (nx - tx + bx), 0.57735026f * (ny - ty + by), 0.57735026f * (nz - tz + bz));
    GXNormal3f32(nx, ny, nz);

    if (binormal != GX_NONE) {
        GXNormal3f32(tx, ty, tz);
        GXNormal3f32(bx, by, bz);
    }

    if (texture != GX_NONE) {
        GXTexCoord2s8(0, 1);
    }

    GXPosition3f32(0.57735026f * (nx - tx - bx), 0.57735026f * (ny - ty - by), 0.57735026f * (nz - tz - bz));
    GXNormal3f32(nx, ny, nz);

    if (binormal != GX_NONE) {
        GXNormal3f32(tx, ty, tz);
        GXNormal3f32(bx, by, bz);
    }

    if (texture != GX_NONE) {
        GXTexCoord2s8(0, 0);
    }

    GXPosition3f32(0.57735026f * (nx + tx - bx), 0.57735026f * (ny + ty - by), 0.57735026f * (nz + tz - bz));
    GXNormal3f32(nx, ny, nz);

    if (binormal != GX_NONE) {
        GXNormal3f32(tx, ty, tz);
        GXNormal3f32(bx, by, bz);
    }

    if (texture != GX_NONE) {
        GXTexCoord2s8(1, 0);
    }
}

void GXDrawCube(void) {
    GXAttrType ntype;
    GXAttrType ttype;

    GXGetVtxDesc(GX_VA_NBT, &ntype);
    GXGetVtxDesc(GX_VA_TEX0, &ttype);
    GetVertState();
    if (ntype != GX_NONE) {
        GXSetVtxDesc(GX_VA_NBT, GX_DIRECT);
        GXSetVtxAttrFmt(GX_VTXFMT3, GX_VA_NBT, GX_TEX_ST, GX_RGBA6, 0);
    }
    if (ttype != GX_NONE) {
        GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
        GXSetVtxAttrFmt(GX_VTXFMT3, GX_VA_TEX0, GX_TEX_ST, GX_RGB8, 0);
    }

    GXBegin(GX_QUADS, GX_VTXFMT3, 24);
    GXDrawCubeFace(-1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, ntype, ttype);
    GXDrawCubeFace(1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, ntype, ttype);
    GXDrawCubeFace(0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, ntype, ttype);
    GXDrawCubeFace(0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, ntype, ttype);
    GXDrawCubeFace(0.0f, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, ntype, ttype);
    GXDrawCubeFace(0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, ntype, ttype);
    GXEnd();

    RestoreVertState();
}

static u32 polygons[12][5] = {
    {  0, 12, 10, 11, 16 },
    {  1, 17,  8,  9, 13 },
    {  2, 14,  9,  8, 18 },
    {  3, 19, 11, 10, 15 },
    {  4, 14,  2,  3, 15 },
    {  5, 12,  0,  1, 13 },
    {  6, 17,  1,  0, 16 },
    {  7, 19,  3,  2, 18 },
    {  8, 17,  6,  7, 18 },
    {  9, 14,  4,  5, 13 },
    { 10, 12,  5,  4, 15 },
    { 11, 19,  7,  6, 16 },
};

static f32 verts[20][3] = {
    { -0.809015f,  0.0f,       0.309015f },
    { -0.809015f,  0.0f,      -0.309015f },
    {  0.809015f,  0.0f,      -0.309015f },
    {  0.809015f,  0.0f,       0.309015f },
    {  0.309015f, -0.809015f,  0.0f },
    { -0.309015f, -0.809015f,  0.0f },
    { -0.309015f,  0.809015f,  0 },
    {  0.309015f,  0.809015f,  0 },
    {  0.0f,       0.309015f, -0.809015f },
    {  0.0f,      -0.309015f, -0.809015f },
    {  0.0f,      -0.309015f,  0.809015f },
    {  0.0f,       0.309015f,  0.809015f },
    { -0.5f,      -0.5f,       0.5 },
    { -0.5f,      -0.5f,      -0.5 },
    {  0.5f,      -0.5f,      -0.5 },
    {  0.5f,      -0.5f,       0.5 },
    { -0.5f,       0.5f,       0.5 },
    { -0.5f,       0.5f,      -0.5 },
    {  0.5f,       0.5f,      -0.5 },
    {  0.5f,       0.5f,       0.5 },
};

void GXDrawDodeca(void) {
    u32 i;
    f32 *p0;
    f32 *p1;
    f32 *p2;
    f32 u[3];
    f32 v[3];
    f32 n[3];

    GetVertState();
    for (i = 0; i < 12; i++) {
        p0 = verts[polygons[i][0]];
        p1 = verts[polygons[i][1]];
        p2 = verts[polygons[i][2]];
        vsub(p1, p2, u);
        vsub(p1, p0, v);
        vcross(u, v, n);
        normalize(n);
        GXBegin(GX_TRIANGLEFAN, GX_VTXFMT3, 5);
        myvertex(verts[polygons[i][4]], n);
        myvertex(verts[polygons[i][3]], n);
        myvertex(p2, n);
        myvertex(p1, n);
        myvertex(p0, n);
        GXEnd();
    }
    RestoreVertState();
}

static f32 odata[6][3] = {
    {  1.0f,  0.0f,  0.0f },
    { -1.0f,  0.0f,  0.0f },
    {  0.0f,  1.0f,  0.0f },
    {  0.0f, -1.0f,  0.0f },
    {  0.0f,  0.0f,  1.0f },
    {  0.0f,  0.0f, -1.0f },
};

static u8 ondex[8][3] = {
    { 0, 4, 2 },
    { 1, 2, 4 },
    { 0, 3, 4 },
    { 1, 4, 3 },
    { 0, 2, 5 },
    { 1, 5, 2 },
    { 0, 5, 3 },
    { 1, 3, 5 },
};

void GXDrawOctahedron(void) {
    s32 i;

    GetVertState();
    for (i = 7; i >= 0; i--) {
        SubDivTriangle(0, i, odata, ondex);
    }
    RestoreVertState();
}

static f32 idata[12][3] = {
    { -0.5257311f,  0.0f,        0.8506508f },
    {  0.5257311f,  0.0f,        0.8506508f },
    { -0.5257311f,  0.0f,       -0.8506508f },
    {  0.5257311f,  0.0f,       -0.8506508f },
    {  0.0f,        0.8506508f,  0.5257311f },
    {  0.0f,        0.8506508f, -0.5257311f },
    {  0.0f,       -0.8506508f,  0.5257311f },
    {  0.0f,       -0.8506508f, -0.5257311f },
    {  0.8506508f,  0.5257311f,  0.0f },
    { -0.8506508f,  0.5257311f,  0.0f },
    {  0.8506508f, -0.5257311f,  0.0f },
    { -0.8506508f, -0.5257311f,  0.0f },
};

static u8 index[20][3] = {
    {  0,  4,  1 },
    {  0,  9,  4 },
    {  9,  5,  4 },
    {  4,  5,  8 },
    {  4,  8,  1 },
    {  8, 10,  1 },
    {  8,  3, 10 },
    {  5,  3,  8 },
    {  5,  2,  3 },
    {  2,  7,  3 },
    {  7, 10,  3 },
    {  7,  6, 10 },
    {  7, 11,  6 },
    { 11,  0,  6 },
    {  0,  1,  6 },
    {  6,  1, 10 },
    {  9,  0, 11 },
    {  9, 11,  2 },
    {  9,  2,  5 },
    {  7,  2, 11 },
};

void GXDrawIcosahedron(void) {
    s32 i;

    GetVertState();
    for (i = 19; i >= 0; i--) {
        SubDivTriangle(0, i, idata, index);
    }
    RestoreVertState();
}

void GXDrawSphere1(u8 depth) {
    s32 i;

    GetVertState();
    for (i = 19; i >= 0; i--) {
        SubDivTriangle(depth, i, idata, index);
    }
    RestoreVertState();
}

static u32 CmpNormal32(f32 n1[3], f32 n2[3]) {
    u32 i;

    for (i = 0; i < 3; i++) {
        if (n1[i] != n2[i])
            return FALSE;
    }
    return TRUE;
}

static u32 nrm_cnt;
static f32* nrm_tab;

static void AddNormal(f32 n[3]) {
    u32 indx;
    u32 i;

    for (i = 0; i < nrm_cnt; i++) {
        if (CmpNormal32(n, &nrm_tab[i * 3]))
            return;
    }
    indx = nrm_cnt * 3;
    nrm_tab[indx + 0] = n[0];
    nrm_tab[indx + 1] = n[1];
    nrm_tab[indx + 2] = n[2];
    nrm_cnt++;
}

static void SubdivideNrm(u8 depth, f32 v0[3], f32 v1[3], f32 v2[3]) {
    f32 v01[3];
    f32 v12[3];
    f32 v20[3];
    u32 i;

    if (depth == 0) {
        AddNormal(v0);
        AddNormal(v1);
        AddNormal(v2);
        return;
    }

    for (i = 0; i < 3; i++) {
        v01[i] = v0[i] + v1[i];
        v12[i] = v1[i] + v2[i];
        v20[i] = v2[i] + v0[i];
    }

    normalize(v01);
    normalize(v12);
    normalize(v20);
    SubdivideNrm(depth - 1, v0, v01, v20);
    SubdivideNrm(depth - 1, v1, v12, v01);
    SubdivideNrm(depth - 1, v2, v20, v12);
    SubdivideNrm(depth - 1, v01, v12, v20);
}

static void SubDivNrm(u8 depth, u8 i, f32 (*data)[3], u8 (*ndx)[3]) {
    f32* x0 = data[ndx[i][0]];
    f32* x1 = data[ndx[i][1]];
    f32* x2 = data[ndx[i][2]];

    SubdivideNrm(depth, x0, x1, x2);
}

u32 GXGenNormalTable(u8 depth, f32* table) {
    s32 i;

    nrm_cnt = 0;
    nrm_tab = table;
    for (i = 7; i >= 0; i--) {
        SubDivNrm(depth, i, odata, ondex);
    }

    return nrm_cnt;
}
