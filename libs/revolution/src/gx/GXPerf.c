#include <revolution/gx.h>
#include <revolution/os.h>

#include "__gx.h"

void GXSetGPMetric(GXPerf0 perf0, GXPerf1 perf1) {
    u32 reg;

    CHECK_GXBEGIN(142, "GXSetGPMetric");

    switch (__GXData->perf0) {
    case GX_PERF0_VERTICES:
    case GX_PERF0_CLIP_VTX:
    case GX_PERF0_CLIP_CLKS:
    case GX_PERF0_XF_WAIT_IN:
    case GX_PERF0_XF_WAIT_OUT:
    case GX_PERF0_XF_XFRM_CLKS:
    case GX_PERF0_XF_LIT_CLKS:
    case GX_PERF0_XF_BOT_CLKS:
    case GX_PERF0_XF_REGLD_CLKS:
    case GX_PERF0_XF_REGRD_CLKS:
    case GX_PERF0_CLIP_RATIO:
    case GX_PERF0_CLOCKS:
        reg = 0;
        GX_WRITE_XF_REG(0x1006, reg, 0);
        break;
    case GX_PERF0_TRIANGLES:
    case GX_PERF0_TRIANGLES_CULLED:
    case GX_PERF0_TRIANGLES_PASSED:
    case GX_PERF0_TRIANGLES_SCISSORED:
    case GX_PERF0_TRIANGLES_0TEX:
    case GX_PERF0_TRIANGLES_1TEX:
    case GX_PERF0_TRIANGLES_2TEX:
    case GX_PERF0_TRIANGLES_3TEX:
    case GX_PERF0_TRIANGLES_4TEX:
    case GX_PERF0_TRIANGLES_5TEX:
    case GX_PERF0_TRIANGLES_6TEX:
    case GX_PERF0_TRIANGLES_7TEX:
    case GX_PERF0_TRIANGLES_8TEX:
    case GX_PERF0_TRIANGLES_0CLR:
    case GX_PERF0_TRIANGLES_1CLR:
    case GX_PERF0_TRIANGLES_2CLR:
        reg = 0x23000000;
        GX_WRITE_RAS_REG(reg);
        break;
    case GX_PERF0_QUAD_0CVG:
    case GX_PERF0_QUAD_NON0CVG:
    case GX_PERF0_QUAD_1CVG:
    case GX_PERF0_QUAD_2CVG:
    case GX_PERF0_QUAD_3CVG:
    case GX_PERF0_QUAD_4CVG:
    case GX_PERF0_AVG_QUAD_CNT:
        reg = 0x24000000;
        GX_WRITE_RAS_REG(reg);
        break;
    case GX_PERF0_NONE:
        break;
    default:
        ASSERTMSGLINE(202, 0, "GXSetGPMetric: Invalid GXPerf0 metric name");
        break;
    }

    switch (__GXData->perf1) {
    case GX_PERF1_TEXELS:
    case GX_PERF1_TX_IDLE:
    case GX_PERF1_TX_REGS:
    case GX_PERF1_TX_MEMSTALL:
    case GX_PERF1_TC_CHECK1_2:
    case GX_PERF1_TC_CHECK3_4:
    case GX_PERF1_TC_CHECK5_6:
    case GX_PERF1_TC_CHECK7_8:
    case GX_PERF1_TC_MISS:
    case GX_PERF1_CLOCKS:
        reg = 0x67000000;
        GX_WRITE_RAS_REG(reg);
        break;
    case GX_PERF1_VC_ELEMQ_FULL:
    case GX_PERF1_VC_MISSQ_FULL:
    case GX_PERF1_VC_MEMREQ_FULL:
    case GX_PERF1_VC_STATUS7:
    case GX_PERF1_VC_MISSREP_FULL:
    case GX_PERF1_VC_STREAMBUF_LOW:
    case GX_PERF1_VC_ALL_STALLS:
    case GX_PERF1_VERTICES:
        SET_REG_FIELD(0, __GXData->perfSel, 4, 4, 0);
        GX_WRITE_SOME_REG4(8, 0x20, __GXData->perfSel, -12);
        break;
    case GX_PERF1_FIFO_REQ:
    case GX_PERF1_CALL_REQ:
    case GX_PERF1_VC_MISS_REQ:
    case GX_PERF1_CP_ALL_REQ:
        reg = 0;
        GX_SET_CP_REG(3, reg);
        break;
    case GX_PERF1_NONE:
        break;
    default:
        ASSERTMSGLINE(252, 0, "GXSetGPMetric: Invalid GXPerf1 metric name");
        break;
    }

    __GXData->perf0 = perf0;
    switch (__GXData->perf0) {
    case GX_PERF0_VERTICES:
        ASSERTMSGLINE(266, 0, "The use of GX_PERF0_VERTICES is prohibited. Use GX_PERF1_VERTICES instead.\n");
        reg = 0x273;
        GX_WRITE_XF_REG(0x1006, reg, 0);
        break;
    case GX_PERF0_CLIP_VTX:            reg = 0x14A;      GX_WRITE_XF_REG(0x1006, reg, 0); break;
    case GX_PERF0_CLIP_CLKS:           reg = 0x16B;      GX_WRITE_XF_REG(0x1006, reg, 0); break;
    case GX_PERF0_XF_WAIT_IN:          reg = 0x84;       GX_WRITE_XF_REG(0x1006, reg, 0); break;
    case GX_PERF0_XF_WAIT_OUT:         reg = 0xC6;       GX_WRITE_XF_REG(0x1006, reg, 0); break;
    case GX_PERF0_XF_XFRM_CLKS:        reg = 0x210;      GX_WRITE_XF_REG(0x1006, reg, 0); break;
    case GX_PERF0_XF_LIT_CLKS:         reg = 0x252;      GX_WRITE_XF_REG(0x1006, reg, 0); break;
    case GX_PERF0_XF_BOT_CLKS:         reg = 0x231;      GX_WRITE_XF_REG(0x1006, reg, 0); break;
    case GX_PERF0_XF_REGLD_CLKS:       reg = 0x1AD;      GX_WRITE_XF_REG(0x1006, reg, 0); break;
    case GX_PERF0_XF_REGRD_CLKS:       reg = 0x1CE;      GX_WRITE_XF_REG(0x1006, reg, 0); break;
    case GX_PERF0_CLOCKS:              reg = 0x21;       GX_WRITE_XF_REG(0x1006, reg, 0); break;
    case GX_PERF0_CLIP_RATIO:          reg = 0x153;      GX_WRITE_XF_REG(0x1006, reg, 0); break;
    case GX_PERF0_TRIANGLES:           reg = 0x2300AE7F; GX_WRITE_RAS_REG(reg); break;
    case GX_PERF0_TRIANGLES_CULLED:    reg = 0x23008E7F; GX_WRITE_RAS_REG(reg); break;
    case GX_PERF0_TRIANGLES_PASSED:    reg = 0x23009E7F; GX_WRITE_RAS_REG(reg); break;
    case GX_PERF0_TRIANGLES_SCISSORED: reg = 0x23001E7F; GX_WRITE_RAS_REG(reg); break;
    case GX_PERF0_TRIANGLES_0TEX:      reg = 0x2300AC3F; GX_WRITE_RAS_REG(reg); break;
    case GX_PERF0_TRIANGLES_1TEX:      reg = 0x2300AC7F; GX_WRITE_RAS_REG(reg); break;
    case GX_PERF0_TRIANGLES_2TEX:      reg = 0x2300ACBF; GX_WRITE_RAS_REG(reg); break;
    case GX_PERF0_TRIANGLES_3TEX:      reg = 0x2300ACFF; GX_WRITE_RAS_REG(reg); break;
    case GX_PERF0_TRIANGLES_4TEX:      reg = 0x2300AD3F; GX_WRITE_RAS_REG(reg); break;
    case GX_PERF0_TRIANGLES_5TEX:      reg = 0x2300AD7F; GX_WRITE_RAS_REG(reg); break;
    case GX_PERF0_TRIANGLES_6TEX:      reg = 0x2300ADBF; GX_WRITE_RAS_REG(reg); break;
    case GX_PERF0_TRIANGLES_7TEX:      reg = 0x2300ADFF; GX_WRITE_RAS_REG(reg); break;
    case GX_PERF0_TRIANGLES_8TEX:      reg = 0x2300AE3F; GX_WRITE_RAS_REG(reg); break;
    case GX_PERF0_TRIANGLES_0CLR:      reg = 0x2300A27F; GX_WRITE_RAS_REG(reg); break;
    case GX_PERF0_TRIANGLES_1CLR:      reg = 0x2300A67F; GX_WRITE_RAS_REG(reg); break;
    case GX_PERF0_TRIANGLES_2CLR:      reg = 0x2300AA7F; GX_WRITE_RAS_REG(reg); break;
    case GX_PERF0_QUAD_0CVG:           reg = 0x2402C0C6; GX_WRITE_RAS_REG(reg); break;
    case GX_PERF0_QUAD_NON0CVG:        reg = 0x2402C16B; GX_WRITE_RAS_REG(reg); break;
    case GX_PERF0_QUAD_1CVG:           reg = 0x2402C0E7; GX_WRITE_RAS_REG(reg); break;
    case GX_PERF0_QUAD_2CVG:           reg = 0x2402C108; GX_WRITE_RAS_REG(reg); break;
    case GX_PERF0_QUAD_3CVG:           reg = 0x2402C129; GX_WRITE_RAS_REG(reg); break;
    case GX_PERF0_QUAD_4CVG:           reg = 0x2402C14A; GX_WRITE_RAS_REG(reg); break;
    case GX_PERF0_AVG_QUAD_CNT:        reg = 0x2402C1AD; GX_WRITE_RAS_REG(reg); break;
    case GX_PERF0_NONE: break;
    default:
        ASSERTMSGLINE(512, 0, "GXSetGPMetric: Invalid GXPerf0 metric name");
        break;
    }

    __GXData->perf1 = perf1;
    switch (__GXData->perf1) {
    case GX_PERF1_TEXELS:      reg = 0x67000042; GX_WRITE_RAS_REG(reg); break;
    case GX_PERF1_TX_IDLE:     reg = 0x67000084; GX_WRITE_RAS_REG(reg); break;
    case GX_PERF1_TX_REGS:     reg = 0x67000063; GX_WRITE_RAS_REG(reg); break;
    case GX_PERF1_TX_MEMSTALL: reg = 0x67000129; GX_WRITE_RAS_REG(reg); break;
    case GX_PERF1_TC_MISS:     reg = 0x67000252; GX_WRITE_RAS_REG(reg); break;
    case GX_PERF1_CLOCKS:      reg = 0x67000021; GX_WRITE_RAS_REG(reg); break;
    case GX_PERF1_TC_CHECK1_2: reg = 0x6700014B; GX_WRITE_RAS_REG(reg); break;
    case GX_PERF1_TC_CHECK3_4: reg = 0x6700018D; GX_WRITE_RAS_REG(reg); break;
    case GX_PERF1_TC_CHECK5_6: reg = 0x670001CF; GX_WRITE_RAS_REG(reg); break;
    case GX_PERF1_TC_CHECK7_8: reg = 0x67000211; GX_WRITE_RAS_REG(reg); break;
    case GX_PERF1_VC_ELEMQ_FULL:    SET_REG_FIELD(0, __GXData->perfSel, 4, 4, 2); GX_WRITE_SOME_REG4(8, 0x20, __GXData->perfSel, -12); break;
    case GX_PERF1_VC_MISSQ_FULL:    SET_REG_FIELD(0, __GXData->perfSel, 4, 4, 3); GX_WRITE_SOME_REG4(8, 0x20, __GXData->perfSel, -12); break;
    case GX_PERF1_VC_MEMREQ_FULL:   SET_REG_FIELD(0, __GXData->perfSel, 4, 4, 4); GX_WRITE_SOME_REG4(8, 0x20, __GXData->perfSel, -12); break;
    case GX_PERF1_VC_STATUS7:       SET_REG_FIELD(0, __GXData->perfSel, 4, 4, 5); GX_WRITE_SOME_REG4(8, 0x20, __GXData->perfSel, -12); break;
    case GX_PERF1_VC_MISSREP_FULL:  SET_REG_FIELD(0, __GXData->perfSel, 4, 4, 6); GX_WRITE_SOME_REG4(8, 0x20, __GXData->perfSel, -12); break;
    case GX_PERF1_VC_STREAMBUF_LOW: SET_REG_FIELD(0, __GXData->perfSel, 4, 4, 7); GX_WRITE_SOME_REG4(8, 0x20, __GXData->perfSel, -12); break;
    case GX_PERF1_VC_ALL_STALLS:    SET_REG_FIELD(0, __GXData->perfSel, 4, 4, 9); GX_WRITE_SOME_REG4(8, 0x20, __GXData->perfSel, -12); break;
    case GX_PERF1_VERTICES:         SET_REG_FIELD(0, __GXData->perfSel, 4, 4, 8); GX_WRITE_SOME_REG4(8, 0x20, __GXData->perfSel, -12); break;
    case GX_PERF1_FIFO_REQ:    reg = 2; GX_SET_CP_REG(3, reg); break;
    case GX_PERF1_CALL_REQ:    reg = 3; GX_SET_CP_REG(3, reg); break;
    case GX_PERF1_VC_MISS_REQ: reg = 4; GX_SET_CP_REG(3, reg); break;
    case GX_PERF1_CP_ALL_REQ:  reg = 5; GX_SET_CP_REG(3, reg); break;
    case GX_PERF1_NONE: break;
    default:
        ASSERTMSGLINE(657, 0, "GXSetGPMetric: Invalid GXPerf1 metric name");
        break;
    }

    __GXData->bpSentNot = 0;
}

#pragma scheduling off
void GXReadGPMetric(u32* cnt0, u32* cnt1) {
    u32 cpCtr0, cpCtr1, cpCtr2, cpCtr3;

    cpCtr0 = __GXReadCPCounterU32(32, 33);
    cpCtr1 = __GXReadCPCounterU32(34, 35);
    cpCtr2 = __GXReadCPCounterU32(36, 37);
    cpCtr3 = __GXReadCPCounterU32(38, 39);

    switch (__GXData->perf0) {
    case GX_PERF0_CLIP_RATIO:
        *cnt0 = cpCtr1 * 1000 / cpCtr0;
        break;
    case GX_PERF0_VERTICES:
    case GX_PERF0_CLIP_VTX:
    case GX_PERF0_CLIP_CLKS:
    case GX_PERF0_XF_WAIT_IN:
    case GX_PERF0_XF_WAIT_OUT:
    case GX_PERF0_XF_XFRM_CLKS:
    case GX_PERF0_XF_LIT_CLKS:
    case GX_PERF0_XF_BOT_CLKS:
    case GX_PERF0_XF_REGLD_CLKS:
    case GX_PERF0_XF_REGRD_CLKS:
    case GX_PERF0_TRIANGLES:
    case GX_PERF0_TRIANGLES_CULLED:
    case GX_PERF0_TRIANGLES_PASSED:
    case GX_PERF0_TRIANGLES_SCISSORED:
    case GX_PERF0_TRIANGLES_0TEX:
    case GX_PERF0_TRIANGLES_1TEX:
    case GX_PERF0_TRIANGLES_2TEX:
    case GX_PERF0_TRIANGLES_3TEX:
    case GX_PERF0_TRIANGLES_4TEX:
    case GX_PERF0_TRIANGLES_5TEX:
    case GX_PERF0_TRIANGLES_6TEX:
    case GX_PERF0_TRIANGLES_7TEX:
    case GX_PERF0_TRIANGLES_8TEX:
    case GX_PERF0_TRIANGLES_0CLR:
    case GX_PERF0_TRIANGLES_1CLR:
    case GX_PERF0_TRIANGLES_2CLR:
    case GX_PERF0_QUAD_0CVG:
    case GX_PERF0_QUAD_NON0CVG:
    case GX_PERF0_QUAD_1CVG:
    case GX_PERF0_QUAD_2CVG:
    case GX_PERF0_QUAD_3CVG:
    case GX_PERF0_QUAD_4CVG:
    case GX_PERF0_AVG_QUAD_CNT:
    case GX_PERF0_CLOCKS:
        *cnt0 = cpCtr0;
        break;
    case GX_PERF0_NONE:
        *cnt0 = 0;
        break;
    default:
        ASSERTMSGLINE(765, 0, "GXReadGPMetric: Invalid GXPerf0 metric name");
        *cnt0 = 0;
        break;
    }

    switch (__GXData->perf1) {
    case GX_PERF1_TEXELS:
        *cnt1 = cpCtr3 * 4;
        break;
    case GX_PERF1_TC_CHECK1_2:
        *cnt1 = cpCtr2 + (cpCtr3 * 2);
        break;
    case GX_PERF1_TC_CHECK3_4:
        *cnt1 = (cpCtr2 * 3) + (cpCtr3 * 4);
        break;
    case GX_PERF1_TC_CHECK5_6:
        *cnt1 = (cpCtr2 * 5) + (cpCtr3 * 6);
        break;
    case GX_PERF1_TC_CHECK7_8:
        *cnt1 = (cpCtr2 * 7) + (cpCtr3 * 8);
        break;
    case GX_PERF1_TX_IDLE:
    case GX_PERF1_TX_REGS:
    case GX_PERF1_TX_MEMSTALL:
    case GX_PERF1_TC_MISS:
    case GX_PERF1_VC_ELEMQ_FULL:
    case GX_PERF1_VC_MISSQ_FULL:
    case GX_PERF1_VC_MEMREQ_FULL:
    case GX_PERF1_VC_STATUS7:
    case GX_PERF1_VC_MISSREP_FULL:
    case GX_PERF1_VC_STREAMBUF_LOW:
    case GX_PERF1_VC_ALL_STALLS:
    case GX_PERF1_VERTICES:
    case GX_PERF1_CLOCKS:
        *cnt1 = cpCtr3;
        break;
    case GX_PERF1_FIFO_REQ:
    case GX_PERF1_CALL_REQ:
    case GX_PERF1_VC_MISS_REQ:
    case GX_PERF1_CP_ALL_REQ:
        *cnt1 = cpCtr2;
        break;
    case GX_PERF1_NONE:
        *cnt1 = 0;
        break;
    default:
        ASSERTMSGLINE(824, 0, "GXReadGPMetric: Invalid GXPerf1 metric name");
        *cnt1 = 0;
        break;
    }
}
#pragma scheduling reset

void GXClearGPMetric(void) {
    u32 reg;

    reg = 4;
    GX_SET_CP_REG(2, reg);
}

u32 GXReadGP0Metric(void) {
    u32 cnt0, cnt1;

    GXReadGPMetric(&cnt0, &cnt1);
    return cnt0;
}

u32 GXReadGP1Metric(void) {
    u32 cnt0, cnt1;

    GXReadGPMetric(&cnt0, &cnt1);
    return cnt1;
}

#pragma scheduling off
void GXReadMemMetric(u32* cp_req, u32* tc_req, u32* cpu_rd_req, u32* cpu_wr_req, u32* dsp_req, u32* io_req, u32* vi_req, u32* pe_req, u32* rf_req, u32* fi_req) {
    *cp_req = __GXReadMEMCounterU32(26, 25);
    *tc_req = __GXReadMEMCounterU32(28, 27);
    *cpu_rd_req = __GXReadMEMCounterU32(30, 29);
    *cpu_wr_req = __GXReadMEMCounterU32(32, 31);
    *dsp_req = __GXReadMEMCounterU32(34, 33);
    *io_req = __GXReadMEMCounterU32(36, 35);
    *vi_req = __GXReadMEMCounterU32(38, 37);
    *pe_req = __GXReadMEMCounterU32(40, 39);
    *rf_req = __GXReadMEMCounterU32(42, 41);
    *fi_req = __GXReadMEMCounterU32(44, 43);
}
#pragma scheduling reset

void GXClearMemMetric(void) {
    GX_SET_MEM_REG(25, 0);
    GX_SET_MEM_REG(26, 0);
    GX_SET_MEM_REG(27, 0);
    GX_SET_MEM_REG(28, 0);
    GX_SET_MEM_REG(30, 0);
    GX_SET_MEM_REG(29, 0);
    GX_SET_MEM_REG(32, 0);
    GX_SET_MEM_REG(31, 0);
    GX_SET_MEM_REG(34, 0);
    GX_SET_MEM_REG(33, 0);
    GX_SET_MEM_REG(36, 0);
    GX_SET_MEM_REG(35, 0);
    GX_SET_MEM_REG(38, 0);
    GX_SET_MEM_REG(37, 0);
    GX_SET_MEM_REG(40, 0);
    GX_SET_MEM_REG(39, 0);
    GX_SET_MEM_REG(42, 0);
    GX_SET_MEM_REG(41, 0);
    GX_SET_MEM_REG(44, 0);
    GX_SET_MEM_REG(43, 0);
}

#pragma scheduling off
void GXReadPixMetric(u32* top_pixels_in, u32* top_pixels_out, u32* bot_pixels_in, u32* bot_pixels_out, u32* clr_pixels_in, u32* copy_clks) {
    *top_pixels_in = __GXReadPECounterU32(12, 13) * 4;
    *top_pixels_out = __GXReadPECounterU32(14, 15) * 4;
    *bot_pixels_in = __GXReadPECounterU32(16, 17) * 4;
    *bot_pixels_out = __GXReadPECounterU32(18, 19) * 4;
    *clr_pixels_in = __GXReadPECounterU32(20, 21) * 4;
    *copy_clks = __GXReadPECounterU32(22, 23);
}
#pragma scheduling reset

void GXClearPixMetric(void) {
    u32 reg;

    CHECK_GXBEGIN(1163, "GXClearPixMetric");

    reg = 0x57000000;
    GX_WRITE_RAS_REG(reg);
    reg = 0x57000AAA;
    GX_WRITE_RAS_REG(reg);
    __GXData->bpSentNot = 0;
}

void GXSetVCacheMetric(GXVCachePerf attr) {
    u32 reg;

    SET_REG_FIELD(1194, __GXData->perfSel, 4, 0, attr);
    GX_WRITE_SOME_REG4(8, 0x20, __GXData->perfSel, -12);
    reg = 1;
    GX_WRITE_SOME_REG4(8, 0x10, reg, -12);
}

#pragma scheduling off
void GXReadVCacheMetric(u32* check, u32* miss, u32* stall) {
    *check = __GXReadCPCounterU32(40, 41);
    *miss = __GXReadCPCounterU32(42, 43);
    *stall = __GXReadCPCounterU32(44, 45);
}
#pragma scheduling reset

void GXClearVCacheMetric(void) {
    GX_WRITE_SOME_REG4(8, 0, 0, -12);
}

void GXInitXfRasMetric(void) {
    u32 reg;

    CHECK_GXBEGIN(1293, "GXInitXfRasMetric");

    reg = 0x2402C022;
    GX_WRITE_RAS_REG(reg);
    reg = 0x31000;
    GX_WRITE_XF_REG(6, reg, 0);
    __GXData->bpSentNot = 1;
}

#pragma scheduling off
void GXReadXfRasMetric(u32* xf_wait_in, u32* xf_wait_out, u32* ras_busy, u32* clocks) {
    *ras_busy = __GXReadCPCounterU32(32, 33);
    *clocks = __GXReadCPCounterU32(34, 35);
    *xf_wait_in = __GXReadCPCounterU32(36, 37);
    *xf_wait_out = __GXReadCPCounterU32(38, 39);
}
#pragma scheduling reset

u32 GXReadClksPerVtx(void) {
    u32 perfCnt;
    u32 ctrh;

    GXDrawDone();
    GX_SET_CP_REG(49, 0x1007);
    GX_SET_CP_REG(48, 0x1007);

    ctrh = GX_GET_CP_REG(50);
    perfCnt = ctrh >> 8;
    return perfCnt;
}

void __GXSetBWDials(u16 cpDial, u16 tcDial, u16 peDial, u16 cpuRdDial, u16 cpuWrDial) {
    __MEMRegs[9] = cpDial;
    __MEMRegs[10] = tcDial;
    __MEMRegs[11] = peDial;
    __MEMRegs[12] = cpuRdDial;
    __MEMRegs[13] = cpuWrDial;
}
