#include <revolution/gd.h>
#include <revolution/os.h>

void GDLoadPosMtxImm(const Mtx mtx, u32 id) {
    GDWriteXFCmdHdr(4 * id, 12);
    GDWrite_f32(mtx[0][0]);
    GDWrite_f32(mtx[0][1]);
    GDWrite_f32(mtx[0][2]);
    GDWrite_f32(mtx[0][3]);
    GDWrite_f32(mtx[1][0]);
    GDWrite_f32(mtx[1][1]);
    GDWrite_f32(mtx[1][2]);
    GDWrite_f32(mtx[1][3]);
    GDWrite_f32(mtx[2][0]);
    GDWrite_f32(mtx[2][1]);
    GDWrite_f32(mtx[2][2]);
    GDWrite_f32(mtx[2][3]);
}

void GDLoadPosMtxIndx(u16 mtx_indx, u32 id) {
    GDWriteXFIndxACmd(4 * id, 12, mtx_indx);
}

void GDLoadNrmMtxImm(const Mtx mtx, u32 id) {
    GDWriteXFCmdHdr(id * 3 + 0x400, 9);
    GDWrite_f32(mtx[0][0]);
    GDWrite_f32(mtx[0][1]);
    GDWrite_f32(mtx[0][2]);
    GDWrite_f32(mtx[1][0]);
    GDWrite_f32(mtx[1][1]);
    GDWrite_f32(mtx[1][2]);
    GDWrite_f32(mtx[2][0]);
    GDWrite_f32(mtx[2][1]);
    GDWrite_f32(mtx[2][2]);
}

void GDLoadNrmMtxImm3x3(const f32 mtx[3][3], u32 id) {
    GDWriteXFCmdHdr(id * 3 + 0x400, 9);
    GDWrite_f32(mtx[0][0]);
    GDWrite_f32(mtx[0][1]);
    GDWrite_f32(mtx[0][2]);
    GDWrite_f32(mtx[1][0]);
    GDWrite_f32(mtx[1][1]);
    GDWrite_f32(mtx[1][2]);
    GDWrite_f32(mtx[2][0]);
    GDWrite_f32(mtx[2][1]);
    GDWrite_f32(mtx[2][2]);
}

void GDLoadNrmMtxIndx3x3(u16 mtx_indx, u32 id) {
    GDWriteXFIndxBCmd(id * 3 + 0x400, 9, mtx_indx);
}

void GDLoadTexMtxImm(const Mtx mtx, u32 id, GXTexMtxType type) {
    u16 addr;
    u8 count;

    if (id >= 0x40) {
        ASSERTMSGLINE(178, type == GX_MTX3x4, "GDLoadTexMtxImm: invalid matrix type");
        addr = ((id - 0x40) << 2) + 0x500;
        count = 12;
    } else {
        addr = 4 * id;
        count = type == GX_MTX2x4 ? 8 : 12;
    }

    GDWriteXFCmdHdr(addr,count);
    GDWrite_f32(mtx[0][0]);
    GDWrite_f32(mtx[0][1]);
    GDWrite_f32(mtx[0][2]);
    GDWrite_f32(mtx[0][3]);
    GDWrite_f32(mtx[1][0]);
    GDWrite_f32(mtx[1][1]);
    GDWrite_f32(mtx[1][2]);
    GDWrite_f32(mtx[1][3]);

    if (type == GX_MTX3x4) {
        GDWrite_f32(mtx[2][0]);
        GDWrite_f32(mtx[2][1]);
        GDWrite_f32(mtx[2][2]);
        GDWrite_f32(mtx[2][3]);
    }
}

void GDLoadTexMtxIndx(u16 mtx_indx, u32 id, GXTexMtxType type) {
    u16 addr;
    u8 count;

    if (id >= 0x40) {
        ASSERTMSGLINE(227, type == GX_MTX3x4, "GDLoadTexMtxIndx: invalid matrix type");
        addr = ((id - 0x40) << 2) + 0x500;
        count = 12;
    } else {
        addr = 4 * id;
        count = type == GX_MTX2x4 ? 8 : 12;
    }

    GDWriteXFIndxCCmd(addr, count, mtx_indx);
}

void GDSetCurrentMtx(u32 pn, u32 t0, u32 t1, u32 t2, u32 t3, u32 t4, u32 t5, u32 t6, u32 t7) {
    u32 regA;
    u32 regB;

    regA = CP_MTX_REG_A(pn, t0, t1, t2, t3);
    regB = CP_MTX_REG_B(t4, t5, t6, t7);

    GDWriteCPCmd(CP_MTX_REG_A_ID, regA);
    GDWriteCPCmd(CP_MTX_REG_B_ID, regB);
    GDWriteXFCmdHdr(XF_REG_MATRIXINDEX0_ID, 2);
    GDWrite_u32(regA);
    GDWrite_u32(regB);
}

void GDSetProjection(const Mtx44 mtx, GXProjectionType type) {
    u32 c;
    c = type == GX_ORTHOGRAPHIC ? 3 : 2;

    GDWriteXFCmdHdr(XF_REG_PROJECTIONA_ID, 7);
    GDWrite_f32(mtx[0][0]);
    GDWrite_f32(mtx[0][c]);
    GDWrite_f32(mtx[1][1]);
    GDWrite_f32(mtx[1][c]);
    GDWrite_f32(mtx[2][2]);
    GDWrite_f32(mtx[2][3]);
    GDWrite_u32(type);
}
