#ifndef __FDL_CP_REG_H__
#define __FDL_CP_REG_H__

#include <revolution/private/GXFDLShortcut.h>

#define FDL_ASSERT(c) 

#define CP_CMD_NOP  0x00
#define CP_CMD_QUADS    0x10
#define CP_CMD_QUAD_STRIP   0x11
#define CP_CMD_TRIANGLES    0x12
#define CP_CMD_TRIANGLE_STRIP   0x13
#define CP_CMD_TRIANGLE_FAN 0x14
#define CP_CMD_LINES    0x15
#define CP_CMD_LINE_STRIP   0x16
#define CP_CMD_POINTS   0x17
#define CP_CMD_LOADREGS 0x01
#define CP_CMD_XF_LOADREGS  0x02
#define CP_CMD_XF_IDXLDREG_A    0x04
#define CP_CMD_XF_IDXLDREG_B    0x05
#define CP_CMD_XF_IDXLDREG_C    0x06
#define CP_CMD_XF_IDXLDREG_D    0x07
#define CP_CMD_CALL_OBJECT  0x08
#define CP_CMD_VCACHE_INVALIDATE    0x09
#define CP_CMD_SU_BYPCMD0   0x0c
#define CP_CMD_SU_BYPCMD1   0x0d

#define CP_COUNTER0L	0x20
#define CP_COUNTER0H	0x21
#define CP_COUNTER1L	0x22
#define CP_COUNTER1H	0x23
#define CP_COUNTER2L	0x24
#define CP_COUNTER2H	0x25
#define CP_COUNTER3L	0x26
#define CP_COUNTER3H	0x27
#define CP_VC_CHKCNTL	0x28
#define CP_VC_CHKCNTH	0x29

#define CP_OPCODE_INDEX_SIZE    3
#define CP_OPCODE_INDEX_SHIFT   0
#define CP_OPCODE_INDEX_MASK    0x00000007
#define CP_OPCODE_GET_INDEX(cp_opcode) \
    ((((unsigned long)(cp_opcode)) & CP_OPCODE_INDEX_MASK) >> CP_OPCODE_INDEX_SHIFT)
#define CP_OPCODE_SET_INDEX(cp_opcode, index) { \
    FDL_ASSERT(!((index) & ~((1 << CP_OPCODE_INDEX_SIZE)-1))); \
    cp_opcode = (((unsigned long)(cp_opcode)) & ~CP_OPCODE_INDEX_MASK) | (((unsigned long)(index)) << CP_OPCODE_INDEX_SHIFT);\
}
#define CP_OPCODE_CMD_SIZE  5
#define CP_OPCODE_CMD_SHIFT 3
#define CP_OPCODE_CMD_MASK  0x000000f8
#define CP_OPCODE_GET_CMD(cp_opcode) \
    ((((unsigned long)(cp_opcode)) & CP_OPCODE_CMD_MASK) >> CP_OPCODE_CMD_SHIFT)
#define CP_OPCODE_SET_CMD(cp_opcode, cmd) { \
    FDL_ASSERT(!((cmd) & ~((1 << CP_OPCODE_CMD_SIZE)-1))); \
    cp_opcode = (((unsigned long)(cp_opcode)) & ~CP_OPCODE_CMD_MASK) | (((unsigned long)(cmd)) << CP_OPCODE_CMD_SHIFT);\
}
#define CP_OPCODE_TOTAL_SIZE    8
#define CP_OPCODE(index, cmd) \
    ((((unsigned long)(index)) << CP_OPCODE_INDEX_SHIFT) | \
    (((unsigned long)(cmd)) << CP_OPCODE_CMD_SHIFT))

#define CP_VC_STAT_RESET    0x00
#define CP_STAT_ENABLE  0x01
#define CP_STAT_SEL 0x02
#define CP_MATINDEX_A   0x03
#define CP_MATINDEX_B   0x04
#define CP_VCD_LO   0x05
#define CP_VCD_HI   0x06
#define CP_VAT_A    0x07
#define CP_VAT_B    0x08
#define CP_VAT_C    0x09
#define CP_ARRAY_BASE   0x0a
#define CP_ARRAY_STRIDE 0x0b

#define CP_STREAM_REG_INDEX_SIZE    4
#define CP_STREAM_REG_INDEX_SHIFT   0
#define CP_STREAM_REG_INDEX_MASK    0x0000000f
#define CP_STREAM_REG_GET_INDEX(cp_stream_reg) \
    ((((unsigned long)(cp_stream_reg)) & CP_STREAM_REG_INDEX_MASK) >> CP_STREAM_REG_INDEX_SHIFT)
#define CP_STREAM_REG_SET_INDEX(cp_stream_reg, index) { \
    FDL_ASSERT(!((index) & ~((1 << CP_STREAM_REG_INDEX_SIZE)-1))); \
    cp_stream_reg = (((unsigned long)(cp_stream_reg)) & ~CP_STREAM_REG_INDEX_MASK) | (((unsigned long)(index)) << CP_STREAM_REG_INDEX_SHIFT);\
}
#define CP_STREAM_REG_ADDR_SIZE 4
#define CP_STREAM_REG_ADDR_SHIFT    4
#define CP_STREAM_REG_ADDR_MASK 0x000000f0
#define CP_STREAM_REG_GET_ADDR(cp_stream_reg) \
    ((((unsigned long)(cp_stream_reg)) & CP_STREAM_REG_ADDR_MASK) >> CP_STREAM_REG_ADDR_SHIFT)
#define CP_STREAM_REG_SET_ADDR(cp_stream_reg, addr) { \
    FDL_ASSERT(!((addr) & ~((1 << CP_STREAM_REG_ADDR_SIZE)-1))); \
    cp_stream_reg = (((unsigned long)(cp_stream_reg)) & ~CP_STREAM_REG_ADDR_MASK) | (((unsigned long)(addr)) << CP_STREAM_REG_ADDR_SHIFT);\
}
#define CP_STREAM_REG_TOTAL_SIZE    8
#define CP_STREAM_REG(index, addr) \
    ((((unsigned long)(index)) << CP_STREAM_REG_INDEX_SHIFT) | \
    (((unsigned long)(addr)) << CP_STREAM_REG_ADDR_SHIFT))

#define CP_STATUS   0x00
#define CP_ENABLE   0x01
#define CP_CLR  0x02
#define CP_MEMPERF_SEL  0x03
#define CP_STM_LOW  0x05
#define CP_FIFO_BASEL   0x10
#define CP_FIFO_BASEH   0x11
#define CP_FIFO_TOPL    0x12
#define CP_FIFO_TOPH    0x13
#define CP_FIFO_HICNTL  0x14
#define CP_FIFO_HICNTH  0x15
#define CP_FIFO_LOCNTL  0x16
#define CP_FIFO_LOCNTH  0x17
#define CP_FIFO_COUNTL  0x18
#define CP_FIFO_COUNTH  0x19
#define CP_FIFO_WPTRL   0x1a
#define CP_FIFO_WPTRH   0x1b
#define CP_FIFO_RPTRL   0x1c
#define CP_FIFO_RPTRH   0x1d
#define CP_FIFO_BRKL    0x1e
#define CP_FIFO_BRKH    0x1f
#define CP_COUNTER0L    0x20
#define CP_COUNTER0H    0x21
#define CP_COUNTER1L    0x22
#define CP_COUNTER1H    0x23
#define CP_COUNTER2L    0x24
#define CP_COUNTER2H    0x25
#define CP_COUNTER3L    0x26
#define CP_COUNTER3H    0x27
#define CP_VC_CHKCNTL   0x28
#define CP_VC_CHKCNTH   0x29
#define CP_VC_MISSL 0x2a
#define CP_VC_MISSH 0x2b
#define CP_VC_STALLL    0x2c
#define CP_VC_STALLH    0x2d
#define CP_FRCLK_CNTL   0x2e
#define CP_FRCLK_CNTH   0x2f
#define CP_XF_ADDR  0x30
#define CP_XF_DATAL 0x31
#define CP_XF_DATAH 0x32
#define CP_NUM_REGS 0x33

/*
*  cp_reg_status struct
*/
#define CP_REG_STATUS_OVFL_SIZE 1
#define CP_REG_STATUS_OVFL_SHIFT    0
#define CP_REG_STATUS_OVFL_MASK 0x00000001
#define CP_REG_STATUS_GET_OVFL(cp_reg_status) \
    ((((unsigned long)(cp_reg_status)) & CP_REG_STATUS_OVFL_MASK) >> CP_REG_STATUS_OVFL_SHIFT)
#define CP_REG_STATUS_SET_OVFL(cp_reg_status, ovfl) { \
    FDL_ASSERT(!((ovfl) & ~((1 << CP_REG_STATUS_OVFL_SIZE)-1))); \
    cp_reg_status = (((unsigned long)(cp_reg_status)) & ~CP_REG_STATUS_OVFL_MASK) | (((unsigned long)(ovfl)) << CP_REG_STATUS_OVFL_SHIFT);\
}
#define CP_REG_STATUS_UNFL_SIZE 1
#define CP_REG_STATUS_UNFL_SHIFT    1
#define CP_REG_STATUS_UNFL_MASK 0x00000002
#define CP_REG_STATUS_GET_UNFL(cp_reg_status) \
    ((((unsigned long)(cp_reg_status)) & CP_REG_STATUS_UNFL_MASK) >> CP_REG_STATUS_UNFL_SHIFT)
#define CP_REG_STATUS_SET_UNFL(cp_reg_status, unfl) { \
    FDL_ASSERT(!((unfl) & ~((1 << CP_REG_STATUS_UNFL_SIZE)-1))); \
    cp_reg_status = (((unsigned long)(cp_reg_status)) & ~CP_REG_STATUS_UNFL_MASK) | (((unsigned long)(unfl)) << CP_REG_STATUS_UNFL_SHIFT);\
}
#define CP_REG_STATUS_FIFO_RDIDLE_SIZE  1
#define CP_REG_STATUS_FIFO_RDIDLE_SHIFT 2
#define CP_REG_STATUS_FIFO_RDIDLE_MASK  0x00000004
#define CP_REG_STATUS_GET_FIFO_RDIDLE(cp_reg_status) \
    ((((unsigned long)(cp_reg_status)) & CP_REG_STATUS_FIFO_RDIDLE_MASK) >> CP_REG_STATUS_FIFO_RDIDLE_SHIFT)
#define CP_REG_STATUS_SET_FIFO_RDIDLE(cp_reg_status, fifo_rdidle) { \
    FDL_ASSERT(!((fifo_rdidle) & ~((1 << CP_REG_STATUS_FIFO_RDIDLE_SIZE)-1))); \
    cp_reg_status = (((unsigned long)(cp_reg_status)) & ~CP_REG_STATUS_FIFO_RDIDLE_MASK) | (((unsigned long)(fifo_rdidle)) << CP_REG_STATUS_FIFO_RDIDLE_SHIFT);\
}
#define CP_REG_STATUS_CPIDLE_SIZE   1
#define CP_REG_STATUS_CPIDLE_SHIFT  3
#define CP_REG_STATUS_CPIDLE_MASK   0x00000008
#define CP_REG_STATUS_GET_CPIDLE(cp_reg_status) \
    ((((unsigned long)(cp_reg_status)) & CP_REG_STATUS_CPIDLE_MASK) >> CP_REG_STATUS_CPIDLE_SHIFT)
#define CP_REG_STATUS_SET_CPIDLE(cp_reg_status, cpidle) { \
    FDL_ASSERT(!((cpidle) & ~((1 << CP_REG_STATUS_CPIDLE_SIZE)-1))); \
    cp_reg_status = (((unsigned long)(cp_reg_status)) & ~CP_REG_STATUS_CPIDLE_MASK) | (((unsigned long)(cpidle)) << CP_REG_STATUS_CPIDLE_SHIFT);\
}
#define CP_REG_STATUS_FIFOBRK_SIZE  1
#define CP_REG_STATUS_FIFOBRK_SHIFT 4
#define CP_REG_STATUS_FIFOBRK_MASK  0x00000010
#define CP_REG_STATUS_GET_FIFOBRK(cp_reg_status) \
    ((((unsigned long)(cp_reg_status)) & CP_REG_STATUS_FIFOBRK_MASK) >> CP_REG_STATUS_FIFOBRK_SHIFT)
#define CP_REG_STATUS_SET_FIFOBRK(cp_reg_status, fifobrk) { \
    FDL_ASSERT(!((fifobrk) & ~((1 << CP_REG_STATUS_FIFOBRK_SIZE)-1))); \
    cp_reg_status = (((unsigned long)(cp_reg_status)) & ~CP_REG_STATUS_FIFOBRK_MASK) | (((unsigned long)(fifobrk)) << CP_REG_STATUS_FIFOBRK_SHIFT);\
}
#define CP_REG_STATUS_TOTAL_SIZE    5
#define CP_REG_STATUS(ovfl, unfl, fifo_rdidle, cpidle, fifobrk) \
    ((((unsigned long)(ovfl)) << CP_REG_STATUS_OVFL_SHIFT) | \
    (((unsigned long)(unfl)) << CP_REG_STATUS_UNFL_SHIFT) | \
    (((unsigned long)(fifo_rdidle)) << CP_REG_STATUS_FIFO_RDIDLE_SHIFT) | \
    (((unsigned long)(cpidle)) << CP_REG_STATUS_CPIDLE_SHIFT) | \
    (((unsigned long)(fifobrk)) << CP_REG_STATUS_FIFOBRK_SHIFT))

/*
*  cp_reg_enable struct
*/
#define CP_REG_ENABLE_FIFORD_SIZE   1
#define CP_REG_ENABLE_FIFORD_SHIFT  0
#define CP_REG_ENABLE_FIFORD_MASK   0x00000001
#define CP_REG_ENABLE_GET_FIFORD(cp_reg_enable) \
    ((((unsigned long)(cp_reg_enable)) & CP_REG_ENABLE_FIFORD_MASK) >> CP_REG_ENABLE_FIFORD_SHIFT)
#define CP_REG_ENABLE_SET_FIFORD(cp_reg_enable, fiford) { \
    FDL_ASSERT(!((fiford) & ~((1 << CP_REG_ENABLE_FIFORD_SIZE)-1))); \
    cp_reg_enable = (((unsigned long)(cp_reg_enable)) & ~CP_REG_ENABLE_FIFORD_MASK) | (((unsigned long)(fiford)) << CP_REG_ENABLE_FIFORD_SHIFT);\
}
#define CP_REG_ENABLE_FIFOBRK_SIZE  1
#define CP_REG_ENABLE_FIFOBRK_SHIFT 1
#define CP_REG_ENABLE_FIFOBRK_MASK  0x00000002
#define CP_REG_ENABLE_GET_FIFOBRK(cp_reg_enable) \
    ((((unsigned long)(cp_reg_enable)) & CP_REG_ENABLE_FIFOBRK_MASK) >> CP_REG_ENABLE_FIFOBRK_SHIFT)
#define CP_REG_ENABLE_SET_FIFOBRK(cp_reg_enable, fifobrk) { \
    FDL_ASSERT(!((fifobrk) & ~((1 << CP_REG_ENABLE_FIFOBRK_SIZE)-1))); \
    cp_reg_enable = (((unsigned long)(cp_reg_enable)) & ~CP_REG_ENABLE_FIFOBRK_MASK) | (((unsigned long)(fifobrk)) << CP_REG_ENABLE_FIFOBRK_SHIFT);\
}
#define CP_REG_ENABLE_OVFLINT_SIZE  1
#define CP_REG_ENABLE_OVFLINT_SHIFT 2
#define CP_REG_ENABLE_OVFLINT_MASK  0x00000004
#define CP_REG_ENABLE_GET_OVFLINT(cp_reg_enable) \
    ((((unsigned long)(cp_reg_enable)) & CP_REG_ENABLE_OVFLINT_MASK) >> CP_REG_ENABLE_OVFLINT_SHIFT)
#define CP_REG_ENABLE_SET_OVFLINT(cp_reg_enable, ovflint) { \
    FDL_ASSERT(!((ovflint) & ~((1 << CP_REG_ENABLE_OVFLINT_SIZE)-1))); \
    cp_reg_enable = (((unsigned long)(cp_reg_enable)) & ~CP_REG_ENABLE_OVFLINT_MASK) | (((unsigned long)(ovflint)) << CP_REG_ENABLE_OVFLINT_SHIFT);\
}
#define CP_REG_ENABLE_UNFLINT_SIZE  1
#define CP_REG_ENABLE_UNFLINT_SHIFT 3
#define CP_REG_ENABLE_UNFLINT_MASK  0x00000008
#define CP_REG_ENABLE_GET_UNFLINT(cp_reg_enable) \
    ((((unsigned long)(cp_reg_enable)) & CP_REG_ENABLE_UNFLINT_MASK) >> CP_REG_ENABLE_UNFLINT_SHIFT)
#define CP_REG_ENABLE_SET_UNFLINT(cp_reg_enable, unflint) { \
    FDL_ASSERT(!((unflint) & ~((1 << CP_REG_ENABLE_UNFLINT_SIZE)-1))); \
    cp_reg_enable = (((unsigned long)(cp_reg_enable)) & ~CP_REG_ENABLE_UNFLINT_MASK) | (((unsigned long)(unflint)) << CP_REG_ENABLE_UNFLINT_SHIFT);\
}
#define CP_REG_ENABLE_WRPTRINC_SIZE 1
#define CP_REG_ENABLE_WRPTRINC_SHIFT    4
#define CP_REG_ENABLE_WRPTRINC_MASK 0x00000010
#define CP_REG_ENABLE_GET_WRPTRINC(cp_reg_enable) \
    ((((unsigned long)(cp_reg_enable)) & CP_REG_ENABLE_WRPTRINC_MASK) >> CP_REG_ENABLE_WRPTRINC_SHIFT)
#define CP_REG_ENABLE_SET_WRPTRINC(cp_reg_enable, wrptrinc) { \
    FDL_ASSERT(!((wrptrinc) & ~((1 << CP_REG_ENABLE_WRPTRINC_SIZE)-1))); \
    cp_reg_enable = (((unsigned long)(cp_reg_enable)) & ~CP_REG_ENABLE_WRPTRINC_MASK) | (((unsigned long)(wrptrinc)) << CP_REG_ENABLE_WRPTRINC_SHIFT);\
}
#define CP_REG_ENABLE_FIFOBRKINT_SIZE   1
#define CP_REG_ENABLE_FIFOBRKINT_SHIFT  5
#define CP_REG_ENABLE_FIFOBRKINT_MASK   0x00000020
#define CP_REG_ENABLE_GET_FIFOBRKINT(cp_reg_enable) \
    ((((unsigned long)(cp_reg_enable)) & CP_REG_ENABLE_FIFOBRKINT_MASK) >> CP_REG_ENABLE_FIFOBRKINT_SHIFT)
#define CP_REG_ENABLE_SET_FIFOBRKINT(cp_reg_enable, fifobrkint) { \
    FDL_ASSERT(!((fifobrkint) & ~((1 << CP_REG_ENABLE_FIFOBRKINT_SIZE)-1))); \
    cp_reg_enable = (((unsigned long)(cp_reg_enable)) & ~CP_REG_ENABLE_FIFOBRKINT_MASK) | (((unsigned long)(fifobrkint)) << CP_REG_ENABLE_FIFOBRKINT_SHIFT);\
}
#define CP_REG_ENABLE_TOTAL_SIZE    6
#define CP_REG_ENABLE(fiford, fifobrk, ovflint, unflint, wrptrinc, fifobrkint) \
    ((((unsigned long)(fiford)) << CP_REG_ENABLE_FIFORD_SHIFT) | \
    (((unsigned long)(fifobrk)) << CP_REG_ENABLE_FIFOBRK_SHIFT) | \
    (((unsigned long)(ovflint)) << CP_REG_ENABLE_OVFLINT_SHIFT) | \
    (((unsigned long)(unflint)) << CP_REG_ENABLE_UNFLINT_SHIFT) | \
    (((unsigned long)(wrptrinc)) << CP_REG_ENABLE_WRPTRINC_SHIFT) | \
    (((unsigned long)(fifobrkint)) << CP_REG_ENABLE_FIFOBRKINT_SHIFT))

/*
*  cp_reg_clr struct
*/
#define CP_REG_CLR_OVFLINT_SIZE 1
#define CP_REG_CLR_OVFLINT_SHIFT    0
#define CP_REG_CLR_OVFLINT_MASK 0x00000001
#define CP_REG_CLR_GET_OVFLINT(cp_reg_clr) \
    ((((unsigned long)(cp_reg_clr)) & CP_REG_CLR_OVFLINT_MASK) >> CP_REG_CLR_OVFLINT_SHIFT)
#define CP_REG_CLR_SET_OVFLINT(cp_reg_clr, ovflint) { \
    FDL_ASSERT(!((ovflint) & ~((1 << CP_REG_CLR_OVFLINT_SIZE)-1))); \
    cp_reg_clr = (((unsigned long)(cp_reg_clr)) & ~CP_REG_CLR_OVFLINT_MASK) | (((unsigned long)(ovflint)) << CP_REG_CLR_OVFLINT_SHIFT);\
}
#define CP_REG_CLR_UNFLINT_SIZE 1
#define CP_REG_CLR_UNFLINT_SHIFT    1
#define CP_REG_CLR_UNFLINT_MASK 0x00000002
#define CP_REG_CLR_GET_UNFLINT(cp_reg_clr) \
    ((((unsigned long)(cp_reg_clr)) & CP_REG_CLR_UNFLINT_MASK) >> CP_REG_CLR_UNFLINT_SHIFT)
#define CP_REG_CLR_SET_UNFLINT(cp_reg_clr, unflint) { \
    FDL_ASSERT(!((unflint) & ~((1 << CP_REG_CLR_UNFLINT_SIZE)-1))); \
    cp_reg_clr = (((unsigned long)(cp_reg_clr)) & ~CP_REG_CLR_UNFLINT_MASK) | (((unsigned long)(unflint)) << CP_REG_CLR_UNFLINT_SHIFT);\
}
#define CP_REG_CLR_PERFCNT_SIZE 1
#define CP_REG_CLR_PERFCNT_SHIFT    2
#define CP_REG_CLR_PERFCNT_MASK 0x00000004
#define CP_REG_CLR_GET_PERFCNT(cp_reg_clr) \
    ((((unsigned long)(cp_reg_clr)) & CP_REG_CLR_PERFCNT_MASK) >> CP_REG_CLR_PERFCNT_SHIFT)
#define CP_REG_CLR_SET_PERFCNT(cp_reg_clr, perfcnt) { \
    FDL_ASSERT(!((perfcnt) & ~((1 << CP_REG_CLR_PERFCNT_SIZE)-1))); \
    cp_reg_clr = (((unsigned long)(cp_reg_clr)) & ~CP_REG_CLR_PERFCNT_MASK) | (((unsigned long)(perfcnt)) << CP_REG_CLR_PERFCNT_SHIFT);\
}
#define CP_REG_CLR_TOTAL_SIZE   3
#define CP_REG_CLR(ovflint, unflint, perfcnt) \
    ((((unsigned long)(ovflint)) << CP_REG_CLR_OVFLINT_SHIFT) | \
    (((unsigned long)(unflint)) << CP_REG_CLR_UNFLINT_SHIFT) | \
    (((unsigned long)(perfcnt)) << CP_REG_CLR_PERFCNT_SHIFT))

/*
*  cp_reg_memperfsel struct
*/
#define CP_REG_MEMPERFSEL_PERFSEL_SIZE  3
#define CP_REG_MEMPERFSEL_PERFSEL_SHIFT 0
#define CP_REG_MEMPERFSEL_PERFSEL_MASK  0x00000007
#define CP_REG_MEMPERFSEL_GET_PERFSEL(cp_reg_memperfsel) \
    ((((unsigned long)(cp_reg_memperfsel)) & CP_REG_MEMPERFSEL_PERFSEL_MASK) >> CP_REG_MEMPERFSEL_PERFSEL_SHIFT)
#define CP_REG_MEMPERFSEL_SET_PERFSEL(cp_reg_memperfsel, perfsel) { \
    FDL_ASSERT(!((perfsel) & ~((1 << CP_REG_MEMPERFSEL_PERFSEL_SIZE)-1))); \
    cp_reg_memperfsel = (((unsigned long)(cp_reg_memperfsel)) & ~CP_REG_MEMPERFSEL_PERFSEL_MASK) | (((unsigned long)(perfsel)) << CP_REG_MEMPERFSEL_PERFSEL_SHIFT);\
}
#define CP_REG_MEMPERFSEL_TOTAL_SIZE    3
#define CP_REG_MEMPERFSEL(perfsel) \
    ((((unsigned long)(perfsel)) << CP_REG_MEMPERFSEL_PERFSEL_SHIFT))

/*
*  cp_reg_fifo_basel struct
*/
#define CP_REG_FIFO_BASEL_PAD0_SIZE 5
#define CP_REG_FIFO_BASEL_PAD0_SHIFT    0
#define CP_REG_FIFO_BASEL_PAD0_MASK 0x0000001f
#define CP_REG_FIFO_BASEL_GET_PAD0(cp_reg_fifo_basel) \
    ((((unsigned long)(cp_reg_fifo_basel)) & CP_REG_FIFO_BASEL_PAD0_MASK) >> CP_REG_FIFO_BASEL_PAD0_SHIFT)
#define CP_REG_FIFO_BASEL_SET_PAD0(cp_reg_fifo_basel, pad0) { \
    FDL_ASSERT(!((pad0) & ~((1 << CP_REG_FIFO_BASEL_PAD0_SIZE)-1))); \
    cp_reg_fifo_basel = (((unsigned long)(cp_reg_fifo_basel)) & ~CP_REG_FIFO_BASEL_PAD0_MASK) | (((unsigned long)(pad0)) << CP_REG_FIFO_BASEL_PAD0_SHIFT);\
}
#define CP_REG_FIFO_BASEL_ADDR_SIZE 11
#define CP_REG_FIFO_BASEL_ADDR_SHIFT    5
#define CP_REG_FIFO_BASEL_ADDR_MASK 0x0000ffe0
#define CP_REG_FIFO_BASEL_GET_ADDR(cp_reg_fifo_basel) \
    ((((unsigned long)(cp_reg_fifo_basel)) & CP_REG_FIFO_BASEL_ADDR_MASK) >> CP_REG_FIFO_BASEL_ADDR_SHIFT)
#define CP_REG_FIFO_BASEL_SET_ADDR(cp_reg_fifo_basel, addr) { \
    FDL_ASSERT(!((addr) & ~((1 << CP_REG_FIFO_BASEL_ADDR_SIZE)-1))); \
    cp_reg_fifo_basel = (((unsigned long)(cp_reg_fifo_basel)) & ~CP_REG_FIFO_BASEL_ADDR_MASK) | (((unsigned long)(addr)) << CP_REG_FIFO_BASEL_ADDR_SHIFT);\
}
#define CP_REG_FIFO_BASEL_TOTAL_SIZE    16
#define CP_REG_FIFO_BASEL(addr) \
    ((((unsigned long)(addr)) << CP_REG_FIFO_BASEL_ADDR_SHIFT))

/*
*  cp_reg_fifo_baseh struct
*/
#define CP_REG_FIFO_BASEH_ADDR_SIZE 13
#define CP_REG_FIFO_BASEH_ADDR_SHIFT    0
#define CP_REG_FIFO_BASEH_ADDR_MASK 0x00001fff
#define CP_REG_FIFO_BASEH_GET_ADDR(cp_reg_fifo_baseh) \
    ((((unsigned long)(cp_reg_fifo_baseh)) & CP_REG_FIFO_BASEH_ADDR_MASK) >> CP_REG_FIFO_BASEH_ADDR_SHIFT)
#define CP_REG_FIFO_BASEH_SET_ADDR(cp_reg_fifo_baseh, addr) { \
    FDL_ASSERT(!((addr) & ~((1 << CP_REG_FIFO_BASEH_ADDR_SIZE)-1))); \
    cp_reg_fifo_baseh = (((unsigned long)(cp_reg_fifo_baseh)) & ~CP_REG_FIFO_BASEH_ADDR_MASK) | (((unsigned long)(addr)) << CP_REG_FIFO_BASEH_ADDR_SHIFT);\
}
#define CP_REG_FIFO_BASEH_TOTAL_SIZE    13
#define CP_REG_FIFO_BASEH(addr) \
    ((((unsigned long)(addr)) << CP_REG_FIFO_BASEH_ADDR_SHIFT))

/*
*  cp_reg_fifo_topl struct
*/
#define CP_REG_FIFO_TOPL_PAD0_SIZE  5
#define CP_REG_FIFO_TOPL_PAD0_SHIFT 0
#define CP_REG_FIFO_TOPL_PAD0_MASK  0x0000001f
#define CP_REG_FIFO_TOPL_GET_PAD0(cp_reg_fifo_topl) \
    ((((unsigned long)(cp_reg_fifo_topl)) & CP_REG_FIFO_TOPL_PAD0_MASK) >> CP_REG_FIFO_TOPL_PAD0_SHIFT)
#define CP_REG_FIFO_TOPL_SET_PAD0(cp_reg_fifo_topl, pad0) { \
    FDL_ASSERT(!((pad0) & ~((1 << CP_REG_FIFO_TOPL_PAD0_SIZE)-1))); \
    cp_reg_fifo_topl = (((unsigned long)(cp_reg_fifo_topl)) & ~CP_REG_FIFO_TOPL_PAD0_MASK) | (((unsigned long)(pad0)) << CP_REG_FIFO_TOPL_PAD0_SHIFT);\
}
#define CP_REG_FIFO_TOPL_ADDR_SIZE  11
#define CP_REG_FIFO_TOPL_ADDR_SHIFT 5
#define CP_REG_FIFO_TOPL_ADDR_MASK  0x0000ffe0
#define CP_REG_FIFO_TOPL_GET_ADDR(cp_reg_fifo_topl) \
    ((((unsigned long)(cp_reg_fifo_topl)) & CP_REG_FIFO_TOPL_ADDR_MASK) >> CP_REG_FIFO_TOPL_ADDR_SHIFT)
#define CP_REG_FIFO_TOPL_SET_ADDR(cp_reg_fifo_topl, addr) { \
    FDL_ASSERT(!((addr) & ~((1 << CP_REG_FIFO_TOPL_ADDR_SIZE)-1))); \
    cp_reg_fifo_topl = (((unsigned long)(cp_reg_fifo_topl)) & ~CP_REG_FIFO_TOPL_ADDR_MASK) | (((unsigned long)(addr)) << CP_REG_FIFO_TOPL_ADDR_SHIFT);\
}
#define CP_REG_FIFO_TOPL_TOTAL_SIZE 16
#define CP_REG_FIFO_TOPL(addr) \
    ((((unsigned long)(addr)) << CP_REG_FIFO_TOPL_ADDR_SHIFT))

/*
*  cp_reg_fifo_toph struct
*/
#define CP_REG_FIFO_TOPH_ADDR_SIZE  13
#define CP_REG_FIFO_TOPH_ADDR_SHIFT 0
#define CP_REG_FIFO_TOPH_ADDR_MASK  0x00001fff
#define CP_REG_FIFO_TOPH_GET_ADDR(cp_reg_fifo_toph) \
    ((((unsigned long)(cp_reg_fifo_toph)) & CP_REG_FIFO_TOPH_ADDR_MASK) >> CP_REG_FIFO_TOPH_ADDR_SHIFT)
#define CP_REG_FIFO_TOPH_SET_ADDR(cp_reg_fifo_toph, addr) { \
    FDL_ASSERT(!((addr) & ~((1 << CP_REG_FIFO_TOPH_ADDR_SIZE)-1))); \
    cp_reg_fifo_toph = (((unsigned long)(cp_reg_fifo_toph)) & ~CP_REG_FIFO_TOPH_ADDR_MASK) | (((unsigned long)(addr)) << CP_REG_FIFO_TOPH_ADDR_SHIFT);\
}
#define CP_REG_FIFO_TOPH_TOTAL_SIZE 13
#define CP_REG_FIFO_TOPH(addr) \
    ((((unsigned long)(addr)) << CP_REG_FIFO_TOPH_ADDR_SHIFT))

/*
*  cp_reg_fifo_hicntl struct
*/
#define CP_REG_FIFO_HICNTL_PAD0_SIZE    5
#define CP_REG_FIFO_HICNTL_PAD0_SHIFT   0
#define CP_REG_FIFO_HICNTL_PAD0_MASK    0x0000001f
#define CP_REG_FIFO_HICNTL_GET_PAD0(cp_reg_fifo_hicntl) \
    ((((unsigned long)(cp_reg_fifo_hicntl)) & CP_REG_FIFO_HICNTL_PAD0_MASK) >> CP_REG_FIFO_HICNTL_PAD0_SHIFT)
#define CP_REG_FIFO_HICNTL_SET_PAD0(cp_reg_fifo_hicntl, pad0) { \
    FDL_ASSERT(!((pad0) & ~((1 << CP_REG_FIFO_HICNTL_PAD0_SIZE)-1))); \
    cp_reg_fifo_hicntl = (((unsigned long)(cp_reg_fifo_hicntl)) & ~CP_REG_FIFO_HICNTL_PAD0_MASK) | (((unsigned long)(pad0)) << CP_REG_FIFO_HICNTL_PAD0_SHIFT);\
}
#define CP_REG_FIFO_HICNTL_ADDR_SIZE    11
#define CP_REG_FIFO_HICNTL_ADDR_SHIFT   5
#define CP_REG_FIFO_HICNTL_ADDR_MASK    0x0000ffe0
#define CP_REG_FIFO_HICNTL_GET_ADDR(cp_reg_fifo_hicntl) \
    ((((unsigned long)(cp_reg_fifo_hicntl)) & CP_REG_FIFO_HICNTL_ADDR_MASK) >> CP_REG_FIFO_HICNTL_ADDR_SHIFT)
#define CP_REG_FIFO_HICNTL_SET_ADDR(cp_reg_fifo_hicntl, addr) { \
    FDL_ASSERT(!((addr) & ~((1 << CP_REG_FIFO_HICNTL_ADDR_SIZE)-1))); \
    cp_reg_fifo_hicntl = (((unsigned long)(cp_reg_fifo_hicntl)) & ~CP_REG_FIFO_HICNTL_ADDR_MASK) | (((unsigned long)(addr)) << CP_REG_FIFO_HICNTL_ADDR_SHIFT);\
}
#define CP_REG_FIFO_HICNTL_TOTAL_SIZE   16
#define CP_REG_FIFO_HICNTL(addr) \
    ((((unsigned long)(addr)) << CP_REG_FIFO_HICNTL_ADDR_SHIFT))

/*
*  cp_reg_fifo_hicnth struct
*/
#define CP_REG_FIFO_HICNTH_ADDR_SIZE    13
#define CP_REG_FIFO_HICNTH_ADDR_SHIFT   0
#define CP_REG_FIFO_HICNTH_ADDR_MASK    0x00001fff
#define CP_REG_FIFO_HICNTH_GET_ADDR(cp_reg_fifo_hicnth) \
    ((((unsigned long)(cp_reg_fifo_hicnth)) & CP_REG_FIFO_HICNTH_ADDR_MASK) >> CP_REG_FIFO_HICNTH_ADDR_SHIFT)
#define CP_REG_FIFO_HICNTH_SET_ADDR(cp_reg_fifo_hicnth, addr) { \
    FDL_ASSERT(!((addr) & ~((1 << CP_REG_FIFO_HICNTH_ADDR_SIZE)-1))); \
    cp_reg_fifo_hicnth = (((unsigned long)(cp_reg_fifo_hicnth)) & ~CP_REG_FIFO_HICNTH_ADDR_MASK) | (((unsigned long)(addr)) << CP_REG_FIFO_HICNTH_ADDR_SHIFT);\
}
#define CP_REG_FIFO_HICNTH_TOTAL_SIZE   13
#define CP_REG_FIFO_HICNTH(addr) \
    ((((unsigned long)(addr)) << CP_REG_FIFO_HICNTH_ADDR_SHIFT))

#define CP_REG_FIFO_LOCNTL_PAD0_SIZE    5
#define CP_REG_FIFO_LOCNTL_PAD0_SHIFT   0
#define CP_REG_FIFO_LOCNTL_PAD0_MASK    0x0000001f
#define CP_REG_FIFO_LOCNTL_GET_PAD0(cp_reg_fifo_locntl) \
    ((((unsigned long)(cp_reg_fifo_locntl)) & CP_REG_FIFO_LOCNTL_PAD0_MASK) >> CP_REG_FIFO_LOCNTL_PAD0_SHIFT)
#define CP_REG_FIFO_LOCNTL_SET_PAD0(cp_reg_fifo_locntl, pad0) { \
    FDL_ASSERT(!((pad0) & ~((1 << CP_REG_FIFO_LOCNTL_PAD0_SIZE)-1))); \
    cp_reg_fifo_locntl = (((unsigned long)(cp_reg_fifo_locntl)) & ~CP_REG_FIFO_LOCNTL_PAD0_MASK) | (((unsigned long)(pad0)) << CP_REG_FIFO_LOCNTL_PAD0_SHIFT);\
}
#define CP_REG_FIFO_LOCNTL_ADDR_SIZE    11
#define CP_REG_FIFO_LOCNTL_ADDR_SHIFT   5
#define CP_REG_FIFO_LOCNTL_ADDR_MASK    0x0000ffe0
#define CP_REG_FIFO_LOCNTL_GET_ADDR(cp_reg_fifo_locntl) \
    ((((unsigned long)(cp_reg_fifo_locntl)) & CP_REG_FIFO_LOCNTL_ADDR_MASK) >> CP_REG_FIFO_LOCNTL_ADDR_SHIFT)
#define CP_REG_FIFO_LOCNTL_SET_ADDR(cp_reg_fifo_locntl, addr) { \
    FDL_ASSERT(!((addr) & ~((1 << CP_REG_FIFO_LOCNTL_ADDR_SIZE)-1))); \
    cp_reg_fifo_locntl = (((unsigned long)(cp_reg_fifo_locntl)) & ~CP_REG_FIFO_LOCNTL_ADDR_MASK) | (((unsigned long)(addr)) << CP_REG_FIFO_LOCNTL_ADDR_SHIFT);\
}
#define CP_REG_FIFO_LOCNTL_TOTAL_SIZE   16
#define CP_REG_FIFO_LOCNTL(addr) \
    ((((unsigned long)(addr)) << CP_REG_FIFO_LOCNTL_ADDR_SHIFT))

#define CP_REG_FIFO_LOCNTH_ADDR_SIZE    13
#define CP_REG_FIFO_LOCNTH_ADDR_SHIFT   0
#define CP_REG_FIFO_LOCNTH_ADDR_MASK    0x00001fff
#define CP_REG_FIFO_LOCNTH_GET_ADDR(cp_reg_fifo_locnth) \
    ((((unsigned long)(cp_reg_fifo_locnth)) & CP_REG_FIFO_LOCNTH_ADDR_MASK) >> CP_REG_FIFO_LOCNTH_ADDR_SHIFT)
#define CP_REG_FIFO_LOCNTH_SET_ADDR(cp_reg_fifo_locnth, addr) { \
    FDL_ASSERT(!((addr) & ~((1 << CP_REG_FIFO_LOCNTH_ADDR_SIZE)-1))); \
    cp_reg_fifo_locnth = (((unsigned long)(cp_reg_fifo_locnth)) & ~CP_REG_FIFO_LOCNTH_ADDR_MASK) | (((unsigned long)(addr)) << CP_REG_FIFO_LOCNTH_ADDR_SHIFT);\
}
#define CP_REG_FIFO_LOCNTH_TOTAL_SIZE   13
#define CP_REG_FIFO_LOCNTH(addr) \
    ((((unsigned long)(addr)) << CP_REG_FIFO_LOCNTH_ADDR_SHIFT))

#define CP_REG_FIFO_COUNTL_PAD0_SIZE    5
#define CP_REG_FIFO_COUNTL_PAD0_SHIFT   0
#define CP_REG_FIFO_COUNTL_PAD0_MASK    0x0000001f
#define CP_REG_FIFO_COUNTL_GET_PAD0(cp_reg_fifo_countl) \
    ((((unsigned long)(cp_reg_fifo_countl)) & CP_REG_FIFO_COUNTL_PAD0_MASK) >> CP_REG_FIFO_COUNTL_PAD0_SHIFT)
#define CP_REG_FIFO_COUNTL_SET_PAD0(cp_reg_fifo_countl, pad0) { \
    FDL_ASSERT(!((pad0) & ~((1 << CP_REG_FIFO_COUNTL_PAD0_SIZE)-1))); \
    cp_reg_fifo_countl = (((unsigned long)(cp_reg_fifo_countl)) & ~CP_REG_FIFO_COUNTL_PAD0_MASK) | (((unsigned long)(pad0)) << CP_REG_FIFO_COUNTL_PAD0_SHIFT);\
}
#define CP_REG_FIFO_COUNTL_ADDR_SIZE    11
#define CP_REG_FIFO_COUNTL_ADDR_SHIFT   5
#define CP_REG_FIFO_COUNTL_ADDR_MASK    0x0000ffe0
#define CP_REG_FIFO_COUNTL_GET_ADDR(cp_reg_fifo_countl) \
    ((((unsigned long)(cp_reg_fifo_countl)) & CP_REG_FIFO_COUNTL_ADDR_MASK) >> CP_REG_FIFO_COUNTL_ADDR_SHIFT)
#define CP_REG_FIFO_COUNTL_SET_ADDR(cp_reg_fifo_countl, addr) { \
    FDL_ASSERT(!((addr) & ~((1 << CP_REG_FIFO_COUNTL_ADDR_SIZE)-1))); \
    cp_reg_fifo_countl = (((unsigned long)(cp_reg_fifo_countl)) & ~CP_REG_FIFO_COUNTL_ADDR_MASK) | (((unsigned long)(addr)) << CP_REG_FIFO_COUNTL_ADDR_SHIFT);\
}
#define CP_REG_FIFO_COUNTL_TOTAL_SIZE   16
#define CP_REG_FIFO_COUNTL(addr) \
    ((((unsigned long)(addr)) << CP_REG_FIFO_COUNTL_ADDR_SHIFT))

#define CP_REG_FIFO_COUNTH_ADDR_SIZE    13
#define CP_REG_FIFO_COUNTH_ADDR_SHIFT   0
#define CP_REG_FIFO_COUNTH_ADDR_MASK    0x00001fff
#define CP_REG_FIFO_COUNTH_GET_ADDR(cp_reg_fifo_counth) \
    ((((unsigned long)(cp_reg_fifo_counth)) & CP_REG_FIFO_COUNTH_ADDR_MASK) >> CP_REG_FIFO_COUNTH_ADDR_SHIFT)
#define CP_REG_FIFO_COUNTH_SET_ADDR(cp_reg_fifo_counth, addr) { \
    FDL_ASSERT(!((addr) & ~((1 << CP_REG_FIFO_COUNTH_ADDR_SIZE)-1))); \
    cp_reg_fifo_counth = (((unsigned long)(cp_reg_fifo_counth)) & ~CP_REG_FIFO_COUNTH_ADDR_MASK) | (((unsigned long)(addr)) << CP_REG_FIFO_COUNTH_ADDR_SHIFT);\
}
#define CP_REG_FIFO_COUNTH_TOTAL_SIZE   13
#define CP_REG_FIFO_COUNTH(addr) \
    ((((unsigned long)(addr)) << CP_REG_FIFO_COUNTH_ADDR_SHIFT))

#define CP_REG_FIFO_WPTRL_PAD0_SIZE 5
#define CP_REG_FIFO_WPTRL_PAD0_SHIFT    0
#define CP_REG_FIFO_WPTRL_PAD0_MASK 0x0000001f
#define CP_REG_FIFO_WPTRL_GET_PAD0(cp_reg_fifo_wptrl) \
    ((((unsigned long)(cp_reg_fifo_wptrl)) & CP_REG_FIFO_WPTRL_PAD0_MASK) >> CP_REG_FIFO_WPTRL_PAD0_SHIFT)
#define CP_REG_FIFO_WPTRL_SET_PAD0(cp_reg_fifo_wptrl, pad0) { \
    FDL_ASSERT(!((pad0) & ~((1 << CP_REG_FIFO_WPTRL_PAD0_SIZE)-1))); \
    cp_reg_fifo_wptrl = (((unsigned long)(cp_reg_fifo_wptrl)) & ~CP_REG_FIFO_WPTRL_PAD0_MASK) | (((unsigned long)(pad0)) << CP_REG_FIFO_WPTRL_PAD0_SHIFT);\
}
#define CP_REG_FIFO_WPTRL_ADDR_SIZE 11
#define CP_REG_FIFO_WPTRL_ADDR_SHIFT    5
#define CP_REG_FIFO_WPTRL_ADDR_MASK 0x0000ffe0
#define CP_REG_FIFO_WPTRL_GET_ADDR(cp_reg_fifo_wptrl) \
    ((((unsigned long)(cp_reg_fifo_wptrl)) & CP_REG_FIFO_WPTRL_ADDR_MASK) >> CP_REG_FIFO_WPTRL_ADDR_SHIFT)
#define CP_REG_FIFO_WPTRL_SET_ADDR(cp_reg_fifo_wptrl, addr) { \
    FDL_ASSERT(!((addr) & ~((1 << CP_REG_FIFO_WPTRL_ADDR_SIZE)-1))); \
    cp_reg_fifo_wptrl = (((unsigned long)(cp_reg_fifo_wptrl)) & ~CP_REG_FIFO_WPTRL_ADDR_MASK) | (((unsigned long)(addr)) << CP_REG_FIFO_WPTRL_ADDR_SHIFT);\
}
#define CP_REG_FIFO_WPTRL_TOTAL_SIZE    16
#define CP_REG_FIFO_WPTRL(addr) \
    ((((unsigned long)(addr)) << CP_REG_FIFO_WPTRL_ADDR_SHIFT))

#define CP_REG_FIFO_WPTRH_ADDR_SIZE 13
#define CP_REG_FIFO_WPTRH_ADDR_SHIFT    0
#define CP_REG_FIFO_WPTRH_ADDR_MASK 0x00001fff
#define CP_REG_FIFO_WPTRH_GET_ADDR(cp_reg_fifo_wptrh) \
    ((((unsigned long)(cp_reg_fifo_wptrh)) & CP_REG_FIFO_WPTRH_ADDR_MASK) >> CP_REG_FIFO_WPTRH_ADDR_SHIFT)
#define CP_REG_FIFO_WPTRH_SET_ADDR(cp_reg_fifo_wptrh, addr) { \
    FDL_ASSERT(!((addr) & ~((1 << CP_REG_FIFO_WPTRH_ADDR_SIZE)-1))); \
    cp_reg_fifo_wptrh = (((unsigned long)(cp_reg_fifo_wptrh)) & ~CP_REG_FIFO_WPTRH_ADDR_MASK) | (((unsigned long)(addr)) << CP_REG_FIFO_WPTRH_ADDR_SHIFT);\
}
#define CP_REG_FIFO_WPTRH_TOTAL_SIZE    13
#define CP_REG_FIFO_WPTRH(addr) \
    ((((unsigned long)(addr)) << CP_REG_FIFO_WPTRH_ADDR_SHIFT))

#define CP_REG_FIFO_RPTRL_PAD0_SIZE 5
#define CP_REG_FIFO_RPTRL_PAD0_SHIFT    0
#define CP_REG_FIFO_RPTRL_PAD0_MASK 0x0000001f
#define CP_REG_FIFO_RPTRL_GET_PAD0(cp_reg_fifo_rptrl) \
    ((((unsigned long)(cp_reg_fifo_rptrl)) & CP_REG_FIFO_RPTRL_PAD0_MASK) >> CP_REG_FIFO_RPTRL_PAD0_SHIFT)
#define CP_REG_FIFO_RPTRL_SET_PAD0(cp_reg_fifo_rptrl, pad0) { \
    FDL_ASSERT(!((pad0) & ~((1 << CP_REG_FIFO_RPTRL_PAD0_SIZE)-1))); \
    cp_reg_fifo_rptrl = (((unsigned long)(cp_reg_fifo_rptrl)) & ~CP_REG_FIFO_RPTRL_PAD0_MASK) | (((unsigned long)(pad0)) << CP_REG_FIFO_RPTRL_PAD0_SHIFT);\
}
#define CP_REG_FIFO_RPTRL_ADDR_SIZE 11
#define CP_REG_FIFO_RPTRL_ADDR_SHIFT    5
#define CP_REG_FIFO_RPTRL_ADDR_MASK 0x0000ffe0
#define CP_REG_FIFO_RPTRL_GET_ADDR(cp_reg_fifo_rptrl) \
    ((((unsigned long)(cp_reg_fifo_rptrl)) & CP_REG_FIFO_RPTRL_ADDR_MASK) >> CP_REG_FIFO_RPTRL_ADDR_SHIFT)
#define CP_REG_FIFO_RPTRL_SET_ADDR(cp_reg_fifo_rptrl, addr) { \
    FDL_ASSERT(!((addr) & ~((1 << CP_REG_FIFO_RPTRL_ADDR_SIZE)-1))); \
    cp_reg_fifo_rptrl = (((unsigned long)(cp_reg_fifo_rptrl)) & ~CP_REG_FIFO_RPTRL_ADDR_MASK) | (((unsigned long)(addr)) << CP_REG_FIFO_RPTRL_ADDR_SHIFT);\
}
#define CP_REG_FIFO_RPTRL_TOTAL_SIZE    16
#define CP_REG_FIFO_RPTRL(addr) \
    ((((unsigned long)(addr)) << CP_REG_FIFO_RPTRL_ADDR_SHIFT))

#define CP_REG_FIFO_RPTRH_ADDR_SIZE 13
#define CP_REG_FIFO_RPTRH_ADDR_SHIFT    0
#define CP_REG_FIFO_RPTRH_ADDR_MASK 0x00001fff
#define CP_REG_FIFO_RPTRH_GET_ADDR(cp_reg_fifo_rptrh) \
    ((((unsigned long)(cp_reg_fifo_rptrh)) & CP_REG_FIFO_RPTRH_ADDR_MASK) >> CP_REG_FIFO_RPTRH_ADDR_SHIFT)
#define CP_REG_FIFO_RPTRH_SET_ADDR(cp_reg_fifo_rptrh, addr) { \
    FDL_ASSERT(!((addr) & ~((1 << CP_REG_FIFO_RPTRH_ADDR_SIZE)-1))); \
    cp_reg_fifo_rptrh = (((unsigned long)(cp_reg_fifo_rptrh)) & ~CP_REG_FIFO_RPTRH_ADDR_MASK) | (((unsigned long)(addr)) << CP_REG_FIFO_RPTRH_ADDR_SHIFT);\
}
#define CP_REG_FIFO_RPTRH_TOTAL_SIZE    13
#define CP_REG_FIFO_RPTRH(addr) \
    ((((unsigned long)(addr)) << CP_REG_FIFO_RPTRH_ADDR_SHIFT))

#define CP_REG_FIFO_BRKL_PAD0_SIZE  5
#define CP_REG_FIFO_BRKL_PAD0_SHIFT 0
#define CP_REG_FIFO_BRKL_PAD0_MASK  0x0000001f
#define CP_REG_FIFO_BRKL_GET_PAD0(cp_reg_fifo_brkl) \
    ((((unsigned long)(cp_reg_fifo_brkl)) & CP_REG_FIFO_BRKL_PAD0_MASK) >> CP_REG_FIFO_BRKL_PAD0_SHIFT)
#define CP_REG_FIFO_BRKL_SET_PAD0(cp_reg_fifo_brkl, pad0) { \
    FDL_ASSERT(!((pad0) & ~((1 << CP_REG_FIFO_BRKL_PAD0_SIZE)-1))); \
    cp_reg_fifo_brkl = (((unsigned long)(cp_reg_fifo_brkl)) & ~CP_REG_FIFO_BRKL_PAD0_MASK) | (((unsigned long)(pad0)) << CP_REG_FIFO_BRKL_PAD0_SHIFT);\
}
#define CP_REG_FIFO_BRKL_ADDR_SIZE  11
#define CP_REG_FIFO_BRKL_ADDR_SHIFT 5
#define CP_REG_FIFO_BRKL_ADDR_MASK  0x0000ffe0
#define CP_REG_FIFO_BRKL_GET_ADDR(cp_reg_fifo_brkl) \
    ((((unsigned long)(cp_reg_fifo_brkl)) & CP_REG_FIFO_BRKL_ADDR_MASK) >> CP_REG_FIFO_BRKL_ADDR_SHIFT)
#define CP_REG_FIFO_BRKL_SET_ADDR(cp_reg_fifo_brkl, addr) { \
    FDL_ASSERT(!((addr) & ~((1 << CP_REG_FIFO_BRKL_ADDR_SIZE)-1))); \
    cp_reg_fifo_brkl = (((unsigned long)(cp_reg_fifo_brkl)) & ~CP_REG_FIFO_BRKL_ADDR_MASK) | (((unsigned long)(addr)) << CP_REG_FIFO_BRKL_ADDR_SHIFT);\
}
#define CP_REG_FIFO_BRKL_TOTAL_SIZE 16
#define CP_REG_FIFO_BRKL(addr) \
    ((((unsigned long)(addr)) << CP_REG_FIFO_BRKL_ADDR_SHIFT))

/*
*  cp_reg_fifo_brkh struct
*/
#define CP_REG_FIFO_BRKH_ADDR_SIZE  13
#define CP_REG_FIFO_BRKH_ADDR_SHIFT 0
#define CP_REG_FIFO_BRKH_ADDR_MASK  0x00001fff
#define CP_REG_FIFO_BRKH_GET_ADDR(cp_reg_fifo_brkh) \
    ((((unsigned long)(cp_reg_fifo_brkh)) & CP_REG_FIFO_BRKH_ADDR_MASK) >> CP_REG_FIFO_BRKH_ADDR_SHIFT)
#define CP_REG_FIFO_BRKH_SET_ADDR(cp_reg_fifo_brkh, addr) { \
    FDL_ASSERT(!((addr) & ~((1 << CP_REG_FIFO_BRKH_ADDR_SIZE)-1))); \
    cp_reg_fifo_brkh = (((unsigned long)(cp_reg_fifo_brkh)) & ~CP_REG_FIFO_BRKH_ADDR_MASK) | (((unsigned long)(addr)) << CP_REG_FIFO_BRKH_ADDR_SHIFT);\
}
#define CP_REG_FIFO_BRKH_TOTAL_SIZE 13
#define CP_REG_FIFO_BRKH(addr) \
    ((((unsigned long)(addr)) << CP_REG_FIFO_BRKH_ADDR_SHIFT))

/*
*  memperf_sel enum
*/
#define MEMPERF_ZERO    0x00000000
#define MEMPERF_ONE 0x00000001
#define DFIFO_REQ_CNT   0x00000002
#define OBJCALL_REQ_CNT 0x00000003
#define VCMISS_REQ_CNT  0x00000004
#define ALL_MEMREQ_CNT  0x00000005
#define MEMPERF_SEL_UNUSED_6    0x00000006
#define MEMPERF_SEL_UNUSED_7    0x00000007

/*
*  vtx_attr_name value
*/
#define VTX_ATTR_POSMATIDX  0x00
#define VTX_ATTR_TEX0MATIDX 0x01
#define VTX_ATTR_TEX1MATIDX 0x02
#define VTX_ATTR_TEX2MATIDX 0x03
#define VTX_ATTR_TEX3MATIDX 0x04
#define VTX_ATTR_TEX4MATIDX 0x05
#define VTX_ATTR_TEX5MATIDX 0x06
#define VTX_ATTR_TEX6MATIDX 0x07
#define VTX_ATTR_TEX7MATIDX 0x08
#define VTX_ATTR_POS    0x09
#define VTX_ATTR_NRM    0x0a
#define VTX_ATTR_COL0   0x0b
#define VTX_ATTR_COL1   0x0c
#define VTX_ATTR_TEX0   0x0d
#define VTX_ATTR_TEX1   0x0e
#define VTX_ATTR_TEX2   0x0f
#define VTX_ATTR_TEX3   0x10
#define VTX_ATTR_TEX4   0x11
#define VTX_ATTR_TEX5   0x12
#define VTX_ATTR_TEX6   0x13
#define VTX_ATTR_TEX7   0x14
#define VTX_ATTR_POSARRAY   0x15
#define VTX_ATTR_NRMARRAY   0x16
#define VTX_ATTR_TEXARRAY   0x17
#define VTX_ATTR_LIGHTARRAY 0x18
#define VTX_NUM_ATTR    0x19

/*
*  vtx_attr_type value
*/
#define ATTR_NONE   0x0
#define ATTR_DIRECT 0x1
#define ATTR_INDEX8 0x2
#define ATTR_INDEX16    0x3

/*
*  attr_comp_fmt value
*/
#define ATTR_COMP_UBYTE 0x0
#define ATTR_COMP_BYTE  0x1
#define ATTR_COMP_USHORT    0x2
#define ATTR_COMP_SHORT 0x3
#define ATTR_COMP_FLOAT 0x4

/*
*  attr_clr_fmt value
*/
#define ATTR_CLR_565    0x0
#define ATTR_CLR_888    0x1
#define ATTR_CLR_888X   0x2
#define ATTR_CLR_4444   0x3
#define ATTR_CLR_6666   0x4
#define ATTR_CLR_8888   0x5

/*
*  cp_vcd_reg_lo struct
*/
#define CP_VCD_REG_LO_PMIDX_SIZE    1
#define CP_VCD_REG_LO_PMIDX_SHIFT   0
#define CP_VCD_REG_LO_PMIDX_MASK    0x00000001
#define CP_VCD_REG_LO_GET_PMIDX(cp_vcd_reg_lo) \
    ((((unsigned long)(cp_vcd_reg_lo)) & CP_VCD_REG_LO_PMIDX_MASK) >> CP_VCD_REG_LO_PMIDX_SHIFT)
#define CP_VCD_REG_LO_SET_PMIDX(cp_vcd_reg_lo, pmidx) { \
    FDL_ASSERT(!((pmidx) & ~((1 << CP_VCD_REG_LO_PMIDX_SIZE)-1))); \
    cp_vcd_reg_lo = (((unsigned long)(cp_vcd_reg_lo)) & ~CP_VCD_REG_LO_PMIDX_MASK) | (((unsigned long)(pmidx)) << CP_VCD_REG_LO_PMIDX_SHIFT);\
}
#define CP_VCD_REG_LO_T0MIDX_SIZE   1
#define CP_VCD_REG_LO_T0MIDX_SHIFT  1
#define CP_VCD_REG_LO_T0MIDX_MASK   0x00000002
#define CP_VCD_REG_LO_GET_T0MIDX(cp_vcd_reg_lo) \
    ((((unsigned long)(cp_vcd_reg_lo)) & CP_VCD_REG_LO_T0MIDX_MASK) >> CP_VCD_REG_LO_T0MIDX_SHIFT)
#define CP_VCD_REG_LO_SET_T0MIDX(cp_vcd_reg_lo, t0midx) { \
    FDL_ASSERT(!((t0midx) & ~((1 << CP_VCD_REG_LO_T0MIDX_SIZE)-1))); \
    cp_vcd_reg_lo = (((unsigned long)(cp_vcd_reg_lo)) & ~CP_VCD_REG_LO_T0MIDX_MASK) | (((unsigned long)(t0midx)) << CP_VCD_REG_LO_T0MIDX_SHIFT);\
}
#define CP_VCD_REG_LO_T1MIDX_SIZE   1
#define CP_VCD_REG_LO_T1MIDX_SHIFT  2
#define CP_VCD_REG_LO_T1MIDX_MASK   0x00000004
#define CP_VCD_REG_LO_GET_T1MIDX(cp_vcd_reg_lo) \
    ((((unsigned long)(cp_vcd_reg_lo)) & CP_VCD_REG_LO_T1MIDX_MASK) >> CP_VCD_REG_LO_T1MIDX_SHIFT)
#define CP_VCD_REG_LO_SET_T1MIDX(cp_vcd_reg_lo, t1midx) { \
    FDL_ASSERT(!((t1midx) & ~((1 << CP_VCD_REG_LO_T1MIDX_SIZE)-1))); \
    cp_vcd_reg_lo = (((unsigned long)(cp_vcd_reg_lo)) & ~CP_VCD_REG_LO_T1MIDX_MASK) | (((unsigned long)(t1midx)) << CP_VCD_REG_LO_T1MIDX_SHIFT);\
}
#define CP_VCD_REG_LO_T2MIDX_SIZE   1
#define CP_VCD_REG_LO_T2MIDX_SHIFT  3
#define CP_VCD_REG_LO_T2MIDX_MASK   0x00000008
#define CP_VCD_REG_LO_GET_T2MIDX(cp_vcd_reg_lo) \
    ((((unsigned long)(cp_vcd_reg_lo)) & CP_VCD_REG_LO_T2MIDX_MASK) >> CP_VCD_REG_LO_T2MIDX_SHIFT)
#define CP_VCD_REG_LO_SET_T2MIDX(cp_vcd_reg_lo, t2midx) { \
    FDL_ASSERT(!((t2midx) & ~((1 << CP_VCD_REG_LO_T2MIDX_SIZE)-1))); \
    cp_vcd_reg_lo = (((unsigned long)(cp_vcd_reg_lo)) & ~CP_VCD_REG_LO_T2MIDX_MASK) | (((unsigned long)(t2midx)) << CP_VCD_REG_LO_T2MIDX_SHIFT);\
}
#define CP_VCD_REG_LO_T3MIDX_SIZE   1
#define CP_VCD_REG_LO_T3MIDX_SHIFT  4
#define CP_VCD_REG_LO_T3MIDX_MASK   0x00000010
#define CP_VCD_REG_LO_GET_T3MIDX(cp_vcd_reg_lo) \
    ((((unsigned long)(cp_vcd_reg_lo)) & CP_VCD_REG_LO_T3MIDX_MASK) >> CP_VCD_REG_LO_T3MIDX_SHIFT)
#define CP_VCD_REG_LO_SET_T3MIDX(cp_vcd_reg_lo, t3midx) { \
    FDL_ASSERT(!((t3midx) & ~((1 << CP_VCD_REG_LO_T3MIDX_SIZE)-1))); \
    cp_vcd_reg_lo = (((unsigned long)(cp_vcd_reg_lo)) & ~CP_VCD_REG_LO_T3MIDX_MASK) | (((unsigned long)(t3midx)) << CP_VCD_REG_LO_T3MIDX_SHIFT);\
}
#define CP_VCD_REG_LO_T4MIDX_SIZE   1
#define CP_VCD_REG_LO_T4MIDX_SHIFT  5
#define CP_VCD_REG_LO_T4MIDX_MASK   0x00000020
#define CP_VCD_REG_LO_GET_T4MIDX(cp_vcd_reg_lo) \
    ((((unsigned long)(cp_vcd_reg_lo)) & CP_VCD_REG_LO_T4MIDX_MASK) >> CP_VCD_REG_LO_T4MIDX_SHIFT)
#define CP_VCD_REG_LO_SET_T4MIDX(cp_vcd_reg_lo, t4midx) { \
    FDL_ASSERT(!((t4midx) & ~((1 << CP_VCD_REG_LO_T4MIDX_SIZE)-1))); \
    cp_vcd_reg_lo = (((unsigned long)(cp_vcd_reg_lo)) & ~CP_VCD_REG_LO_T4MIDX_MASK) | (((unsigned long)(t4midx)) << CP_VCD_REG_LO_T4MIDX_SHIFT);\
}
#define CP_VCD_REG_LO_T5MIDX_SIZE   1
#define CP_VCD_REG_LO_T5MIDX_SHIFT  6
#define CP_VCD_REG_LO_T5MIDX_MASK   0x00000040
#define CP_VCD_REG_LO_GET_T5MIDX(cp_vcd_reg_lo) \
    ((((unsigned long)(cp_vcd_reg_lo)) & CP_VCD_REG_LO_T5MIDX_MASK) >> CP_VCD_REG_LO_T5MIDX_SHIFT)
#define CP_VCD_REG_LO_SET_T5MIDX(cp_vcd_reg_lo, t5midx) { \
    FDL_ASSERT(!((t5midx) & ~((1 << CP_VCD_REG_LO_T5MIDX_SIZE)-1))); \
    cp_vcd_reg_lo = (((unsigned long)(cp_vcd_reg_lo)) & ~CP_VCD_REG_LO_T5MIDX_MASK) | (((unsigned long)(t5midx)) << CP_VCD_REG_LO_T5MIDX_SHIFT);\
}
#define CP_VCD_REG_LO_T6MIDX_SIZE   1
#define CP_VCD_REG_LO_T6MIDX_SHIFT  7
#define CP_VCD_REG_LO_T6MIDX_MASK   0x00000080
#define CP_VCD_REG_LO_GET_T6MIDX(cp_vcd_reg_lo) \
    ((((unsigned long)(cp_vcd_reg_lo)) & CP_VCD_REG_LO_T6MIDX_MASK) >> CP_VCD_REG_LO_T6MIDX_SHIFT)
#define CP_VCD_REG_LO_SET_T6MIDX(cp_vcd_reg_lo, t6midx) { \
    FDL_ASSERT(!((t6midx) & ~((1 << CP_VCD_REG_LO_T6MIDX_SIZE)-1))); \
    cp_vcd_reg_lo = (((unsigned long)(cp_vcd_reg_lo)) & ~CP_VCD_REG_LO_T6MIDX_MASK) | (((unsigned long)(t6midx)) << CP_VCD_REG_LO_T6MIDX_SHIFT);\
}
#define CP_VCD_REG_LO_T7MIDX_SIZE   1
#define CP_VCD_REG_LO_T7MIDX_SHIFT  8
#define CP_VCD_REG_LO_T7MIDX_MASK   0x00000100
#define CP_VCD_REG_LO_GET_T7MIDX(cp_vcd_reg_lo) \
    ((((unsigned long)(cp_vcd_reg_lo)) & CP_VCD_REG_LO_T7MIDX_MASK) >> CP_VCD_REG_LO_T7MIDX_SHIFT)
#define CP_VCD_REG_LO_SET_T7MIDX(cp_vcd_reg_lo, t7midx) { \
    FDL_ASSERT(!((t7midx) & ~((1 << CP_VCD_REG_LO_T7MIDX_SIZE)-1))); \
    cp_vcd_reg_lo = (((unsigned long)(cp_vcd_reg_lo)) & ~CP_VCD_REG_LO_T7MIDX_MASK) | (((unsigned long)(t7midx)) << CP_VCD_REG_LO_T7MIDX_SHIFT);\
}
#define CP_VCD_REG_LO_POS_SIZE  2
#define CP_VCD_REG_LO_POS_SHIFT 9
#define CP_VCD_REG_LO_POS_MASK  0x00000600
#define CP_VCD_REG_LO_GET_POS(cp_vcd_reg_lo) \
    ((((unsigned long)(cp_vcd_reg_lo)) & CP_VCD_REG_LO_POS_MASK) >> CP_VCD_REG_LO_POS_SHIFT)
#define CP_VCD_REG_LO_SET_POS(cp_vcd_reg_lo, pos) { \
    FDL_ASSERT(!((pos) & ~((1 << CP_VCD_REG_LO_POS_SIZE)-1))); \
    cp_vcd_reg_lo = (((unsigned long)(cp_vcd_reg_lo)) & ~CP_VCD_REG_LO_POS_MASK) | (((unsigned long)(pos)) << CP_VCD_REG_LO_POS_SHIFT);\
}
#define CP_VCD_REG_LO_NRM_SIZE  2
#define CP_VCD_REG_LO_NRM_SHIFT 11
#define CP_VCD_REG_LO_NRM_MASK  0x00001800
#define CP_VCD_REG_LO_GET_NRM(cp_vcd_reg_lo) \
    ((((unsigned long)(cp_vcd_reg_lo)) & CP_VCD_REG_LO_NRM_MASK) >> CP_VCD_REG_LO_NRM_SHIFT)
#define CP_VCD_REG_LO_SET_NRM(cp_vcd_reg_lo, nrm) { \
    FDL_ASSERT(!((nrm) & ~((1 << CP_VCD_REG_LO_NRM_SIZE)-1))); \
    cp_vcd_reg_lo = (((unsigned long)(cp_vcd_reg_lo)) & ~CP_VCD_REG_LO_NRM_MASK) | (((unsigned long)(nrm)) << CP_VCD_REG_LO_NRM_SHIFT);\
}
#define CP_VCD_REG_LO_COL0_SIZE 2
#define CP_VCD_REG_LO_COL0_SHIFT    13
#define CP_VCD_REG_LO_COL0_MASK 0x00006000
#define CP_VCD_REG_LO_GET_COL0(cp_vcd_reg_lo) \
    ((((unsigned long)(cp_vcd_reg_lo)) & CP_VCD_REG_LO_COL0_MASK) >> CP_VCD_REG_LO_COL0_SHIFT)
#define CP_VCD_REG_LO_SET_COL0(cp_vcd_reg_lo, col0) { \
    FDL_ASSERT(!((col0) & ~((1 << CP_VCD_REG_LO_COL0_SIZE)-1))); \
    cp_vcd_reg_lo = (((unsigned long)(cp_vcd_reg_lo)) & ~CP_VCD_REG_LO_COL0_MASK) | (((unsigned long)(col0)) << CP_VCD_REG_LO_COL0_SHIFT);\
}
#define CP_VCD_REG_LO_COL1_SIZE 2
#define CP_VCD_REG_LO_COL1_SHIFT    15
#define CP_VCD_REG_LO_COL1_MASK 0x00018000
#define CP_VCD_REG_LO_GET_COL1(cp_vcd_reg_lo) \
    ((((unsigned long)(cp_vcd_reg_lo)) & CP_VCD_REG_LO_COL1_MASK) >> CP_VCD_REG_LO_COL1_SHIFT)
#define CP_VCD_REG_LO_SET_COL1(cp_vcd_reg_lo, col1) { \
    FDL_ASSERT(!((col1) & ~((1 << CP_VCD_REG_LO_COL1_SIZE)-1))); \
    cp_vcd_reg_lo = (((unsigned long)(cp_vcd_reg_lo)) & ~CP_VCD_REG_LO_COL1_MASK) | (((unsigned long)(col1)) << CP_VCD_REG_LO_COL1_SHIFT);\
}
#define CP_VCD_REG_LO_TOTAL_SIZE    17
#define CP_VCD_REG_LO(pmidx, t0midx, t1midx, t2midx, t3midx, t4midx, t5midx, t6midx, t7midx, pos, nrm, col0, col1) \
    ((((unsigned long)(pmidx)) << CP_VCD_REG_LO_PMIDX_SHIFT) | \
    (((unsigned long)(t0midx)) << CP_VCD_REG_LO_T0MIDX_SHIFT) | \
    (((unsigned long)(t1midx)) << CP_VCD_REG_LO_T1MIDX_SHIFT) | \
    (((unsigned long)(t2midx)) << CP_VCD_REG_LO_T2MIDX_SHIFT) | \
    (((unsigned long)(t3midx)) << CP_VCD_REG_LO_T3MIDX_SHIFT) | \
    (((unsigned long)(t4midx)) << CP_VCD_REG_LO_T4MIDX_SHIFT) | \
    (((unsigned long)(t5midx)) << CP_VCD_REG_LO_T5MIDX_SHIFT) | \
    (((unsigned long)(t6midx)) << CP_VCD_REG_LO_T6MIDX_SHIFT) | \
    (((unsigned long)(t7midx)) << CP_VCD_REG_LO_T7MIDX_SHIFT) | \
    (((unsigned long)(pos)) << CP_VCD_REG_LO_POS_SHIFT) | \
    (((unsigned long)(nrm)) << CP_VCD_REG_LO_NRM_SHIFT) | \
    (((unsigned long)(col0)) << CP_VCD_REG_LO_COL0_SHIFT) | \
    (((unsigned long)(col1)) << CP_VCD_REG_LO_COL1_SHIFT))

/*
*  cp_vcd_reg_hi struct
*/
#define CP_VCD_REG_HI_TEX0_SIZE 2
#define CP_VCD_REG_HI_TEX0_SHIFT    0
#define CP_VCD_REG_HI_TEX0_MASK 0x00000003
#define CP_VCD_REG_HI_GET_TEX0(cp_vcd_reg_hi) \
    ((((unsigned long)(cp_vcd_reg_hi)) & CP_VCD_REG_HI_TEX0_MASK) >> CP_VCD_REG_HI_TEX0_SHIFT)
#define CP_VCD_REG_HI_SET_TEX0(cp_vcd_reg_hi, tex0) { \
    FDL_ASSERT(!((tex0) & ~((1 << CP_VCD_REG_HI_TEX0_SIZE)-1))); \
    cp_vcd_reg_hi = (((unsigned long)(cp_vcd_reg_hi)) & ~CP_VCD_REG_HI_TEX0_MASK) | (((unsigned long)(tex0)) << CP_VCD_REG_HI_TEX0_SHIFT);\
}
#define CP_VCD_REG_HI_TEX1_SIZE 2
#define CP_VCD_REG_HI_TEX1_SHIFT    2
#define CP_VCD_REG_HI_TEX1_MASK 0x0000000c
#define CP_VCD_REG_HI_GET_TEX1(cp_vcd_reg_hi) \
    ((((unsigned long)(cp_vcd_reg_hi)) & CP_VCD_REG_HI_TEX1_MASK) >> CP_VCD_REG_HI_TEX1_SHIFT)
#define CP_VCD_REG_HI_SET_TEX1(cp_vcd_reg_hi, tex1) { \
    FDL_ASSERT(!((tex1) & ~((1 << CP_VCD_REG_HI_TEX1_SIZE)-1))); \
    cp_vcd_reg_hi = (((unsigned long)(cp_vcd_reg_hi)) & ~CP_VCD_REG_HI_TEX1_MASK) | (((unsigned long)(tex1)) << CP_VCD_REG_HI_TEX1_SHIFT);\
}
#define CP_VCD_REG_HI_TEX2_SIZE 2
#define CP_VCD_REG_HI_TEX2_SHIFT    4
#define CP_VCD_REG_HI_TEX2_MASK 0x00000030
#define CP_VCD_REG_HI_GET_TEX2(cp_vcd_reg_hi) \
    ((((unsigned long)(cp_vcd_reg_hi)) & CP_VCD_REG_HI_TEX2_MASK) >> CP_VCD_REG_HI_TEX2_SHIFT)
#define CP_VCD_REG_HI_SET_TEX2(cp_vcd_reg_hi, tex2) { \
    FDL_ASSERT(!((tex2) & ~((1 << CP_VCD_REG_HI_TEX2_SIZE)-1))); \
    cp_vcd_reg_hi = (((unsigned long)(cp_vcd_reg_hi)) & ~CP_VCD_REG_HI_TEX2_MASK) | (((unsigned long)(tex2)) << CP_VCD_REG_HI_TEX2_SHIFT);\
}
#define CP_VCD_REG_HI_TEX3_SIZE 2
#define CP_VCD_REG_HI_TEX3_SHIFT    6
#define CP_VCD_REG_HI_TEX3_MASK 0x000000c0
#define CP_VCD_REG_HI_GET_TEX3(cp_vcd_reg_hi) \
    ((((unsigned long)(cp_vcd_reg_hi)) & CP_VCD_REG_HI_TEX3_MASK) >> CP_VCD_REG_HI_TEX3_SHIFT)
#define CP_VCD_REG_HI_SET_TEX3(cp_vcd_reg_hi, tex3) { \
    FDL_ASSERT(!((tex3) & ~((1 << CP_VCD_REG_HI_TEX3_SIZE)-1))); \
    cp_vcd_reg_hi = (((unsigned long)(cp_vcd_reg_hi)) & ~CP_VCD_REG_HI_TEX3_MASK) | (((unsigned long)(tex3)) << CP_VCD_REG_HI_TEX3_SHIFT);\
}
#define CP_VCD_REG_HI_TEX4_SIZE 2
#define CP_VCD_REG_HI_TEX4_SHIFT    8
#define CP_VCD_REG_HI_TEX4_MASK 0x00000300
#define CP_VCD_REG_HI_GET_TEX4(cp_vcd_reg_hi) \
    ((((unsigned long)(cp_vcd_reg_hi)) & CP_VCD_REG_HI_TEX4_MASK) >> CP_VCD_REG_HI_TEX4_SHIFT)
#define CP_VCD_REG_HI_SET_TEX4(cp_vcd_reg_hi, tex4) { \
    FDL_ASSERT(!((tex4) & ~((1 << CP_VCD_REG_HI_TEX4_SIZE)-1))); \
    cp_vcd_reg_hi = (((unsigned long)(cp_vcd_reg_hi)) & ~CP_VCD_REG_HI_TEX4_MASK) | (((unsigned long)(tex4)) << CP_VCD_REG_HI_TEX4_SHIFT);\
}
#define CP_VCD_REG_HI_TEX5_SIZE 2
#define CP_VCD_REG_HI_TEX5_SHIFT    10
#define CP_VCD_REG_HI_TEX5_MASK 0x00000c00
#define CP_VCD_REG_HI_GET_TEX5(cp_vcd_reg_hi) \
    ((((unsigned long)(cp_vcd_reg_hi)) & CP_VCD_REG_HI_TEX5_MASK) >> CP_VCD_REG_HI_TEX5_SHIFT)
#define CP_VCD_REG_HI_SET_TEX5(cp_vcd_reg_hi, tex5) { \
    FDL_ASSERT(!((tex5) & ~((1 << CP_VCD_REG_HI_TEX5_SIZE)-1))); \
    cp_vcd_reg_hi = (((unsigned long)(cp_vcd_reg_hi)) & ~CP_VCD_REG_HI_TEX5_MASK) | (((unsigned long)(tex5)) << CP_VCD_REG_HI_TEX5_SHIFT);\
}
#define CP_VCD_REG_HI_TEX6_SIZE 2
#define CP_VCD_REG_HI_TEX6_SHIFT    12
#define CP_VCD_REG_HI_TEX6_MASK 0x00003000
#define CP_VCD_REG_HI_GET_TEX6(cp_vcd_reg_hi) \
    ((((unsigned long)(cp_vcd_reg_hi)) & CP_VCD_REG_HI_TEX6_MASK) >> CP_VCD_REG_HI_TEX6_SHIFT)
#define CP_VCD_REG_HI_SET_TEX6(cp_vcd_reg_hi, tex6) { \
    FDL_ASSERT(!((tex6) & ~((1 << CP_VCD_REG_HI_TEX6_SIZE)-1))); \
    cp_vcd_reg_hi = (((unsigned long)(cp_vcd_reg_hi)) & ~CP_VCD_REG_HI_TEX6_MASK) | (((unsigned long)(tex6)) << CP_VCD_REG_HI_TEX6_SHIFT);\
}
#define CP_VCD_REG_HI_TEX7_SIZE 2
#define CP_VCD_REG_HI_TEX7_SHIFT    14
#define CP_VCD_REG_HI_TEX7_MASK 0x0000c000
#define CP_VCD_REG_HI_GET_TEX7(cp_vcd_reg_hi) \
    ((((unsigned long)(cp_vcd_reg_hi)) & CP_VCD_REG_HI_TEX7_MASK) >> CP_VCD_REG_HI_TEX7_SHIFT)
#define CP_VCD_REG_HI_SET_TEX7(cp_vcd_reg_hi, tex7) { \
    FDL_ASSERT(!((tex7) & ~((1 << CP_VCD_REG_HI_TEX7_SIZE)-1))); \
    cp_vcd_reg_hi = (((unsigned long)(cp_vcd_reg_hi)) & ~CP_VCD_REG_HI_TEX7_MASK) | (((unsigned long)(tex7)) << CP_VCD_REG_HI_TEX7_SHIFT);\
}
#define CP_VCD_REG_HI_TOTAL_SIZE    16
#define CP_VCD_REG_HI(tex0, tex1, tex2, tex3, tex4, tex5, tex6, tex7) \
    ((((unsigned long)(tex0)) << CP_VCD_REG_HI_TEX0_SHIFT) | \
    (((unsigned long)(tex1)) << CP_VCD_REG_HI_TEX1_SHIFT) | \
    (((unsigned long)(tex2)) << CP_VCD_REG_HI_TEX2_SHIFT) | \
    (((unsigned long)(tex3)) << CP_VCD_REG_HI_TEX3_SHIFT) | \
    (((unsigned long)(tex4)) << CP_VCD_REG_HI_TEX4_SHIFT) | \
    (((unsigned long)(tex5)) << CP_VCD_REG_HI_TEX5_SHIFT) | \
    (((unsigned long)(tex6)) << CP_VCD_REG_HI_TEX6_SHIFT) | \
    (((unsigned long)(tex7)) << CP_VCD_REG_HI_TEX7_SHIFT))

/*
*  cp_vat_table value
*/
#define CP_NUM_VATS 0x08

/*
*  cp_vat_reg_a struct
*/
#define CP_VAT_REG_A_POSCNT_SIZE    1
#define CP_VAT_REG_A_POSCNT_SHIFT   0
#define CP_VAT_REG_A_POSCNT_MASK    0x00000001
#define CP_VAT_REG_A_GET_POSCNT(cp_vat_reg_a) \
    ((((unsigned long)(cp_vat_reg_a)) & CP_VAT_REG_A_POSCNT_MASK) >> CP_VAT_REG_A_POSCNT_SHIFT)
#define CP_VAT_REG_A_SET_POSCNT(cp_vat_reg_a, posCnt) { \
    FDL_ASSERT(!((posCnt) & ~((1 << CP_VAT_REG_A_POSCNT_SIZE)-1))); \
    cp_vat_reg_a = (((unsigned long)(cp_vat_reg_a)) & ~CP_VAT_REG_A_POSCNT_MASK) | (((unsigned long)(posCnt)) << CP_VAT_REG_A_POSCNT_SHIFT);\
}
#define CP_VAT_REG_A_POSFMT_SIZE    3
#define CP_VAT_REG_A_POSFMT_SHIFT   1
#define CP_VAT_REG_A_POSFMT_MASK    0x0000000e
#define CP_VAT_REG_A_GET_POSFMT(cp_vat_reg_a) \
    ((((unsigned long)(cp_vat_reg_a)) & CP_VAT_REG_A_POSFMT_MASK) >> CP_VAT_REG_A_POSFMT_SHIFT)
#define CP_VAT_REG_A_SET_POSFMT(cp_vat_reg_a, posFmt) { \
    FDL_ASSERT(!((posFmt) & ~((1 << CP_VAT_REG_A_POSFMT_SIZE)-1))); \
    cp_vat_reg_a = (((unsigned long)(cp_vat_reg_a)) & ~CP_VAT_REG_A_POSFMT_MASK) | (((unsigned long)(posFmt)) << CP_VAT_REG_A_POSFMT_SHIFT);\
}
#define CP_VAT_REG_A_POSSHFT_SIZE   5
#define CP_VAT_REG_A_POSSHFT_SHIFT  4
#define CP_VAT_REG_A_POSSHFT_MASK   0x000001f0
#define CP_VAT_REG_A_GET_POSSHFT(cp_vat_reg_a) \
    ((((unsigned long)(cp_vat_reg_a)) & CP_VAT_REG_A_POSSHFT_MASK) >> CP_VAT_REG_A_POSSHFT_SHIFT)
#define CP_VAT_REG_A_SET_POSSHFT(cp_vat_reg_a, posShft) { \
    FDL_ASSERT(!((posShft) & ~((1 << CP_VAT_REG_A_POSSHFT_SIZE)-1))); \
    cp_vat_reg_a = (((unsigned long)(cp_vat_reg_a)) & ~CP_VAT_REG_A_POSSHFT_MASK) | (((unsigned long)(posShft)) << CP_VAT_REG_A_POSSHFT_SHIFT);\
}
#define CP_VAT_REG_A_NRMCNT_SIZE    1
#define CP_VAT_REG_A_NRMCNT_SHIFT   9
#define CP_VAT_REG_A_NRMCNT_MASK    0x00000200
#define CP_VAT_REG_A_GET_NRMCNT(cp_vat_reg_a) \
    ((((unsigned long)(cp_vat_reg_a)) & CP_VAT_REG_A_NRMCNT_MASK) >> CP_VAT_REG_A_NRMCNT_SHIFT)
#define CP_VAT_REG_A_SET_NRMCNT(cp_vat_reg_a, nrmCnt) { \
    FDL_ASSERT(!((nrmCnt) & ~((1 << CP_VAT_REG_A_NRMCNT_SIZE)-1))); \
    cp_vat_reg_a = (((unsigned long)(cp_vat_reg_a)) & ~CP_VAT_REG_A_NRMCNT_MASK) | (((unsigned long)(nrmCnt)) << CP_VAT_REG_A_NRMCNT_SHIFT);\
}
#define CP_VAT_REG_A_NRMFMT_SIZE    3
#define CP_VAT_REG_A_NRMFMT_SHIFT   10
#define CP_VAT_REG_A_NRMFMT_MASK    0x00001c00
#define CP_VAT_REG_A_GET_NRMFMT(cp_vat_reg_a) \
    ((((unsigned long)(cp_vat_reg_a)) & CP_VAT_REG_A_NRMFMT_MASK) >> CP_VAT_REG_A_NRMFMT_SHIFT)
#define CP_VAT_REG_A_SET_NRMFMT(cp_vat_reg_a, nrmFmt) { \
    FDL_ASSERT(!((nrmFmt) & ~((1 << CP_VAT_REG_A_NRMFMT_SIZE)-1))); \
    cp_vat_reg_a = (((unsigned long)(cp_vat_reg_a)) & ~CP_VAT_REG_A_NRMFMT_MASK) | (((unsigned long)(nrmFmt)) << CP_VAT_REG_A_NRMFMT_SHIFT);\
}
#define CP_VAT_REG_A_COL0CNT_SIZE   1
#define CP_VAT_REG_A_COL0CNT_SHIFT  13
#define CP_VAT_REG_A_COL0CNT_MASK   0x00002000
#define CP_VAT_REG_A_GET_COL0CNT(cp_vat_reg_a) \
    ((((unsigned long)(cp_vat_reg_a)) & CP_VAT_REG_A_COL0CNT_MASK) >> CP_VAT_REG_A_COL0CNT_SHIFT)
#define CP_VAT_REG_A_SET_COL0CNT(cp_vat_reg_a, Col0Cnt) { \
    FDL_ASSERT(!((Col0Cnt) & ~((1 << CP_VAT_REG_A_COL0CNT_SIZE)-1))); \
    cp_vat_reg_a = (((unsigned long)(cp_vat_reg_a)) & ~CP_VAT_REG_A_COL0CNT_MASK) | (((unsigned long)(Col0Cnt)) << CP_VAT_REG_A_COL0CNT_SHIFT);\
}
#define CP_VAT_REG_A_COL0FMT_SIZE   3
#define CP_VAT_REG_A_COL0FMT_SHIFT  14
#define CP_VAT_REG_A_COL0FMT_MASK   0x0001c000
#define CP_VAT_REG_A_GET_COL0FMT(cp_vat_reg_a) \
    ((((unsigned long)(cp_vat_reg_a)) & CP_VAT_REG_A_COL0FMT_MASK) >> CP_VAT_REG_A_COL0FMT_SHIFT)
#define CP_VAT_REG_A_SET_COL0FMT(cp_vat_reg_a, Col0Fmt) { \
    FDL_ASSERT(!((Col0Fmt) & ~((1 << CP_VAT_REG_A_COL0FMT_SIZE)-1))); \
    cp_vat_reg_a = (((unsigned long)(cp_vat_reg_a)) & ~CP_VAT_REG_A_COL0FMT_MASK) | (((unsigned long)(Col0Fmt)) << CP_VAT_REG_A_COL0FMT_SHIFT);\
}
#define CP_VAT_REG_A_COL1CNT_SIZE   1
#define CP_VAT_REG_A_COL1CNT_SHIFT  17
#define CP_VAT_REG_A_COL1CNT_MASK   0x00020000
#define CP_VAT_REG_A_GET_COL1CNT(cp_vat_reg_a) \
    ((((unsigned long)(cp_vat_reg_a)) & CP_VAT_REG_A_COL1CNT_MASK) >> CP_VAT_REG_A_COL1CNT_SHIFT)
#define CP_VAT_REG_A_SET_COL1CNT(cp_vat_reg_a, Col1Cnt) { \
    FDL_ASSERT(!((Col1Cnt) & ~((1 << CP_VAT_REG_A_COL1CNT_SIZE)-1))); \
    cp_vat_reg_a = (((unsigned long)(cp_vat_reg_a)) & ~CP_VAT_REG_A_COL1CNT_MASK) | (((unsigned long)(Col1Cnt)) << CP_VAT_REG_A_COL1CNT_SHIFT);\
}
#define CP_VAT_REG_A_COL1FMT_SIZE   3
#define CP_VAT_REG_A_COL1FMT_SHIFT  18
#define CP_VAT_REG_A_COL1FMT_MASK   0x001c0000
#define CP_VAT_REG_A_GET_COL1FMT(cp_vat_reg_a) \
    ((((unsigned long)(cp_vat_reg_a)) & CP_VAT_REG_A_COL1FMT_MASK) >> CP_VAT_REG_A_COL1FMT_SHIFT)
#define CP_VAT_REG_A_SET_COL1FMT(cp_vat_reg_a, Col1Fmt) { \
    FDL_ASSERT(!((Col1Fmt) & ~((1 << CP_VAT_REG_A_COL1FMT_SIZE)-1))); \
    cp_vat_reg_a = (((unsigned long)(cp_vat_reg_a)) & ~CP_VAT_REG_A_COL1FMT_MASK) | (((unsigned long)(Col1Fmt)) << CP_VAT_REG_A_COL1FMT_SHIFT);\
}
#define CP_VAT_REG_A_TEX0CNT_SIZE   1
#define CP_VAT_REG_A_TEX0CNT_SHIFT  21
#define CP_VAT_REG_A_TEX0CNT_MASK   0x00200000
#define CP_VAT_REG_A_GET_TEX0CNT(cp_vat_reg_a) \
    ((((unsigned long)(cp_vat_reg_a)) & CP_VAT_REG_A_TEX0CNT_MASK) >> CP_VAT_REG_A_TEX0CNT_SHIFT)
#define CP_VAT_REG_A_SET_TEX0CNT(cp_vat_reg_a, tex0Cnt) { \
    FDL_ASSERT(!((tex0Cnt) & ~((1 << CP_VAT_REG_A_TEX0CNT_SIZE)-1))); \
    cp_vat_reg_a = (((unsigned long)(cp_vat_reg_a)) & ~CP_VAT_REG_A_TEX0CNT_MASK) | (((unsigned long)(tex0Cnt)) << CP_VAT_REG_A_TEX0CNT_SHIFT);\
}
#define CP_VAT_REG_A_TEX0FMT_SIZE   3
#define CP_VAT_REG_A_TEX0FMT_SHIFT  22
#define CP_VAT_REG_A_TEX0FMT_MASK   0x01c00000
#define CP_VAT_REG_A_GET_TEX0FMT(cp_vat_reg_a) \
    ((((unsigned long)(cp_vat_reg_a)) & CP_VAT_REG_A_TEX0FMT_MASK) >> CP_VAT_REG_A_TEX0FMT_SHIFT)
#define CP_VAT_REG_A_SET_TEX0FMT(cp_vat_reg_a, tex0Fmt) { \
    FDL_ASSERT(!((tex0Fmt) & ~((1 << CP_VAT_REG_A_TEX0FMT_SIZE)-1))); \
    cp_vat_reg_a = (((unsigned long)(cp_vat_reg_a)) & ~CP_VAT_REG_A_TEX0FMT_MASK) | (((unsigned long)(tex0Fmt)) << CP_VAT_REG_A_TEX0FMT_SHIFT);\
}
#define CP_VAT_REG_A_TEX0SHFT_SIZE  5
#define CP_VAT_REG_A_TEX0SHFT_SHIFT 25
#define CP_VAT_REG_A_TEX0SHFT_MASK  0x3e000000
#define CP_VAT_REG_A_GET_TEX0SHFT(cp_vat_reg_a) \
    ((((unsigned long)(cp_vat_reg_a)) & CP_VAT_REG_A_TEX0SHFT_MASK) >> CP_VAT_REG_A_TEX0SHFT_SHIFT)
#define CP_VAT_REG_A_SET_TEX0SHFT(cp_vat_reg_a, tex0Shft) { \
    FDL_ASSERT(!((tex0Shft) & ~((1 << CP_VAT_REG_A_TEX0SHFT_SIZE)-1))); \
    cp_vat_reg_a = (((unsigned long)(cp_vat_reg_a)) & ~CP_VAT_REG_A_TEX0SHFT_MASK) | (((unsigned long)(tex0Shft)) << CP_VAT_REG_A_TEX0SHFT_SHIFT);\
}
#define CP_VAT_REG_A_BYTEDEQUANT_SIZE   1
#define CP_VAT_REG_A_BYTEDEQUANT_SHIFT  30
#define CP_VAT_REG_A_BYTEDEQUANT_MASK   0x40000000
#define CP_VAT_REG_A_GET_BYTEDEQUANT(cp_vat_reg_a) \
    ((((unsigned long)(cp_vat_reg_a)) & CP_VAT_REG_A_BYTEDEQUANT_MASK) >> CP_VAT_REG_A_BYTEDEQUANT_SHIFT)
#define CP_VAT_REG_A_SET_BYTEDEQUANT(cp_vat_reg_a, byteDequant) { \
    FDL_ASSERT(!((byteDequant) & ~((1 << CP_VAT_REG_A_BYTEDEQUANT_SIZE)-1))); \
    cp_vat_reg_a = (((unsigned long)(cp_vat_reg_a)) & ~CP_VAT_REG_A_BYTEDEQUANT_MASK) | (((unsigned long)(byteDequant)) << CP_VAT_REG_A_BYTEDEQUANT_SHIFT);\
}
#define CP_VAT_REG_A_NORMALINDEX3_SIZE  1
#define CP_VAT_REG_A_NORMALINDEX3_SHIFT 31
#define CP_VAT_REG_A_NORMALINDEX3_MASK  0x80000000
#define CP_VAT_REG_A_GET_NORMALINDEX3(cp_vat_reg_a) \
    ((((unsigned long)(cp_vat_reg_a)) & CP_VAT_REG_A_NORMALINDEX3_MASK) >> CP_VAT_REG_A_NORMALINDEX3_SHIFT)
#define CP_VAT_REG_A_SET_NORMALINDEX3(cp_vat_reg_a, normalIndex3) { \
    FDL_ASSERT(!((normalIndex3) & ~((1 << CP_VAT_REG_A_NORMALINDEX3_SIZE)-1))); \
    cp_vat_reg_a = (((unsigned long)(cp_vat_reg_a)) & ~CP_VAT_REG_A_NORMALINDEX3_MASK) | (((unsigned long)(normalIndex3)) << CP_VAT_REG_A_NORMALINDEX3_SHIFT);\
}
#define CP_VAT_REG_A_TOTAL_SIZE 32
#define CP_VAT_REG_A(posCnt, posFmt, posShft, nrmCnt, nrmFmt, Col0Cnt, Col0Fmt, Col1Cnt, Col1Fmt, tex0Cnt, tex0Fmt, tex0Shft, byteDequant, normalIndex3) \
    ((((unsigned long)(posCnt)) << CP_VAT_REG_A_POSCNT_SHIFT) | \
    (((unsigned long)(posFmt)) << CP_VAT_REG_A_POSFMT_SHIFT) | \
    (((unsigned long)(posShft)) << CP_VAT_REG_A_POSSHFT_SHIFT) | \
    (((unsigned long)(nrmCnt)) << CP_VAT_REG_A_NRMCNT_SHIFT) | \
    (((unsigned long)(nrmFmt)) << CP_VAT_REG_A_NRMFMT_SHIFT) | \
    (((unsigned long)(Col0Cnt)) << CP_VAT_REG_A_COL0CNT_SHIFT) | \
    (((unsigned long)(Col0Fmt)) << CP_VAT_REG_A_COL0FMT_SHIFT) | \
    (((unsigned long)(Col1Cnt)) << CP_VAT_REG_A_COL1CNT_SHIFT) | \
    (((unsigned long)(Col1Fmt)) << CP_VAT_REG_A_COL1FMT_SHIFT) | \
    (((unsigned long)(tex0Cnt)) << CP_VAT_REG_A_TEX0CNT_SHIFT) | \
    (((unsigned long)(tex0Fmt)) << CP_VAT_REG_A_TEX0FMT_SHIFT) | \
    (((unsigned long)(tex0Shft)) << CP_VAT_REG_A_TEX0SHFT_SHIFT) | \
    (((unsigned long)(byteDequant)) << CP_VAT_REG_A_BYTEDEQUANT_SHIFT) | \
    (((unsigned long)(normalIndex3)) << CP_VAT_REG_A_NORMALINDEX3_SHIFT))

/*
*  cp_vat_reg_b struct
*/
#define CP_VAT_REG_B_TEX1CNT_SIZE   1
#define CP_VAT_REG_B_TEX1CNT_SHIFT  0
#define CP_VAT_REG_B_TEX1CNT_MASK   0x00000001
#define CP_VAT_REG_B_GET_TEX1CNT(cp_vat_reg_b) \
    ((((unsigned long)(cp_vat_reg_b)) & CP_VAT_REG_B_TEX1CNT_MASK) >> CP_VAT_REG_B_TEX1CNT_SHIFT)
#define CP_VAT_REG_B_SET_TEX1CNT(cp_vat_reg_b, tex1Cnt) { \
    FDL_ASSERT(!((tex1Cnt) & ~((1 << CP_VAT_REG_B_TEX1CNT_SIZE)-1))); \
    cp_vat_reg_b = (((unsigned long)(cp_vat_reg_b)) & ~CP_VAT_REG_B_TEX1CNT_MASK) | (((unsigned long)(tex1Cnt)) << CP_VAT_REG_B_TEX1CNT_SHIFT);\
}
#define CP_VAT_REG_B_TEX1FMT_SIZE   3
#define CP_VAT_REG_B_TEX1FMT_SHIFT  1
#define CP_VAT_REG_B_TEX1FMT_MASK   0x0000000e
#define CP_VAT_REG_B_GET_TEX1FMT(cp_vat_reg_b) \
    ((((unsigned long)(cp_vat_reg_b)) & CP_VAT_REG_B_TEX1FMT_MASK) >> CP_VAT_REG_B_TEX1FMT_SHIFT)
#define CP_VAT_REG_B_SET_TEX1FMT(cp_vat_reg_b, tex1Fmt) { \
    FDL_ASSERT(!((tex1Fmt) & ~((1 << CP_VAT_REG_B_TEX1FMT_SIZE)-1))); \
    cp_vat_reg_b = (((unsigned long)(cp_vat_reg_b)) & ~CP_VAT_REG_B_TEX1FMT_MASK) | (((unsigned long)(tex1Fmt)) << CP_VAT_REG_B_TEX1FMT_SHIFT);\
}
#define CP_VAT_REG_B_TEX1SHFT_SIZE  5
#define CP_VAT_REG_B_TEX1SHFT_SHIFT 4
#define CP_VAT_REG_B_TEX1SHFT_MASK  0x000001f0
#define CP_VAT_REG_B_GET_TEX1SHFT(cp_vat_reg_b) \
    ((((unsigned long)(cp_vat_reg_b)) & CP_VAT_REG_B_TEX1SHFT_MASK) >> CP_VAT_REG_B_TEX1SHFT_SHIFT)
#define CP_VAT_REG_B_SET_TEX1SHFT(cp_vat_reg_b, tex1Shft) { \
    FDL_ASSERT(!((tex1Shft) & ~((1 << CP_VAT_REG_B_TEX1SHFT_SIZE)-1))); \
    cp_vat_reg_b = (((unsigned long)(cp_vat_reg_b)) & ~CP_VAT_REG_B_TEX1SHFT_MASK) | (((unsigned long)(tex1Shft)) << CP_VAT_REG_B_TEX1SHFT_SHIFT);\
}
#define CP_VAT_REG_B_TEX2CNT_SIZE   1
#define CP_VAT_REG_B_TEX2CNT_SHIFT  9
#define CP_VAT_REG_B_TEX2CNT_MASK   0x00000200
#define CP_VAT_REG_B_GET_TEX2CNT(cp_vat_reg_b) \
    ((((unsigned long)(cp_vat_reg_b)) & CP_VAT_REG_B_TEX2CNT_MASK) >> CP_VAT_REG_B_TEX2CNT_SHIFT)
#define CP_VAT_REG_B_SET_TEX2CNT(cp_vat_reg_b, tex2Cnt) { \
    FDL_ASSERT(!((tex2Cnt) & ~((1 << CP_VAT_REG_B_TEX2CNT_SIZE)-1))); \
    cp_vat_reg_b = (((unsigned long)(cp_vat_reg_b)) & ~CP_VAT_REG_B_TEX2CNT_MASK) | (((unsigned long)(tex2Cnt)) << CP_VAT_REG_B_TEX2CNT_SHIFT);\
}
#define CP_VAT_REG_B_TEX2FMT_SIZE   3
#define CP_VAT_REG_B_TEX2FMT_SHIFT  10
#define CP_VAT_REG_B_TEX2FMT_MASK   0x00001c00
#define CP_VAT_REG_B_GET_TEX2FMT(cp_vat_reg_b) \
    ((((unsigned long)(cp_vat_reg_b)) & CP_VAT_REG_B_TEX2FMT_MASK) >> CP_VAT_REG_B_TEX2FMT_SHIFT)
#define CP_VAT_REG_B_SET_TEX2FMT(cp_vat_reg_b, tex2Fmt) { \
    FDL_ASSERT(!((tex2Fmt) & ~((1 << CP_VAT_REG_B_TEX2FMT_SIZE)-1))); \
    cp_vat_reg_b = (((unsigned long)(cp_vat_reg_b)) & ~CP_VAT_REG_B_TEX2FMT_MASK) | (((unsigned long)(tex2Fmt)) << CP_VAT_REG_B_TEX2FMT_SHIFT);\
}
#define CP_VAT_REG_B_TEX2SHFT_SIZE  5
#define CP_VAT_REG_B_TEX2SHFT_SHIFT 13
#define CP_VAT_REG_B_TEX2SHFT_MASK  0x0003e000
#define CP_VAT_REG_B_GET_TEX2SHFT(cp_vat_reg_b) \
    ((((unsigned long)(cp_vat_reg_b)) & CP_VAT_REG_B_TEX2SHFT_MASK) >> CP_VAT_REG_B_TEX2SHFT_SHIFT)
#define CP_VAT_REG_B_SET_TEX2SHFT(cp_vat_reg_b, tex2Shft) { \
    FDL_ASSERT(!((tex2Shft) & ~((1 << CP_VAT_REG_B_TEX2SHFT_SIZE)-1))); \
    cp_vat_reg_b = (((unsigned long)(cp_vat_reg_b)) & ~CP_VAT_REG_B_TEX2SHFT_MASK) | (((unsigned long)(tex2Shft)) << CP_VAT_REG_B_TEX2SHFT_SHIFT);\
}
#define CP_VAT_REG_B_TEX3CNT_SIZE   1
#define CP_VAT_REG_B_TEX3CNT_SHIFT  18
#define CP_VAT_REG_B_TEX3CNT_MASK   0x00040000
#define CP_VAT_REG_B_GET_TEX3CNT(cp_vat_reg_b) \
    ((((unsigned long)(cp_vat_reg_b)) & CP_VAT_REG_B_TEX3CNT_MASK) >> CP_VAT_REG_B_TEX3CNT_SHIFT)
#define CP_VAT_REG_B_SET_TEX3CNT(cp_vat_reg_b, tex3Cnt) { \
    FDL_ASSERT(!((tex3Cnt) & ~((1 << CP_VAT_REG_B_TEX3CNT_SIZE)-1))); \
    cp_vat_reg_b = (((unsigned long)(cp_vat_reg_b)) & ~CP_VAT_REG_B_TEX3CNT_MASK) | (((unsigned long)(tex3Cnt)) << CP_VAT_REG_B_TEX3CNT_SHIFT);\
}
#define CP_VAT_REG_B_TEX3FMT_SIZE   3
#define CP_VAT_REG_B_TEX3FMT_SHIFT  19
#define CP_VAT_REG_B_TEX3FMT_MASK   0x00380000
#define CP_VAT_REG_B_GET_TEX3FMT(cp_vat_reg_b) \
    ((((unsigned long)(cp_vat_reg_b)) & CP_VAT_REG_B_TEX3FMT_MASK) >> CP_VAT_REG_B_TEX3FMT_SHIFT)
#define CP_VAT_REG_B_SET_TEX3FMT(cp_vat_reg_b, tex3Fmt) { \
    FDL_ASSERT(!((tex3Fmt) & ~((1 << CP_VAT_REG_B_TEX3FMT_SIZE)-1))); \
    cp_vat_reg_b = (((unsigned long)(cp_vat_reg_b)) & ~CP_VAT_REG_B_TEX3FMT_MASK) | (((unsigned long)(tex3Fmt)) << CP_VAT_REG_B_TEX3FMT_SHIFT);\
}
#define CP_VAT_REG_B_TEX3SHFT_SIZE  5
#define CP_VAT_REG_B_TEX3SHFT_SHIFT 22
#define CP_VAT_REG_B_TEX3SHFT_MASK  0x07c00000
#define CP_VAT_REG_B_GET_TEX3SHFT(cp_vat_reg_b) \
    ((((unsigned long)(cp_vat_reg_b)) & CP_VAT_REG_B_TEX3SHFT_MASK) >> CP_VAT_REG_B_TEX3SHFT_SHIFT)
#define CP_VAT_REG_B_SET_TEX3SHFT(cp_vat_reg_b, tex3Shft) { \
    FDL_ASSERT(!((tex3Shft) & ~((1 << CP_VAT_REG_B_TEX3SHFT_SIZE)-1))); \
    cp_vat_reg_b = (((unsigned long)(cp_vat_reg_b)) & ~CP_VAT_REG_B_TEX3SHFT_MASK) | (((unsigned long)(tex3Shft)) << CP_VAT_REG_B_TEX3SHFT_SHIFT);\
}
#define CP_VAT_REG_B_TEX4CNT_SIZE   1
#define CP_VAT_REG_B_TEX4CNT_SHIFT  27
#define CP_VAT_REG_B_TEX4CNT_MASK   0x08000000
#define CP_VAT_REG_B_GET_TEX4CNT(cp_vat_reg_b) \
    ((((unsigned long)(cp_vat_reg_b)) & CP_VAT_REG_B_TEX4CNT_MASK) >> CP_VAT_REG_B_TEX4CNT_SHIFT)
#define CP_VAT_REG_B_SET_TEX4CNT(cp_vat_reg_b, tex4Cnt) { \
    FDL_ASSERT(!((tex4Cnt) & ~((1 << CP_VAT_REG_B_TEX4CNT_SIZE)-1))); \
    cp_vat_reg_b = (((unsigned long)(cp_vat_reg_b)) & ~CP_VAT_REG_B_TEX4CNT_MASK) | (((unsigned long)(tex4Cnt)) << CP_VAT_REG_B_TEX4CNT_SHIFT);\
}
#define CP_VAT_REG_B_TEX4FMT_SIZE   3
#define CP_VAT_REG_B_TEX4FMT_SHIFT  28
#define CP_VAT_REG_B_TEX4FMT_MASK   0x70000000
#define CP_VAT_REG_B_GET_TEX4FMT(cp_vat_reg_b) \
    ((((unsigned long)(cp_vat_reg_b)) & CP_VAT_REG_B_TEX4FMT_MASK) >> CP_VAT_REG_B_TEX4FMT_SHIFT)
#define CP_VAT_REG_B_SET_TEX4FMT(cp_vat_reg_b, tex4Fmt) { \
    FDL_ASSERT(!((tex4Fmt) & ~((1 << CP_VAT_REG_B_TEX4FMT_SIZE)-1))); \
    cp_vat_reg_b = (((unsigned long)(cp_vat_reg_b)) & ~CP_VAT_REG_B_TEX4FMT_MASK) | (((unsigned long)(tex4Fmt)) << CP_VAT_REG_B_TEX4FMT_SHIFT);\
}
#define CP_VAT_REG_B_VCACHE_ENHANCE_SIZE    1
#define CP_VAT_REG_B_VCACHE_ENHANCE_SHIFT   31
#define CP_VAT_REG_B_VCACHE_ENHANCE_MASK    0x80000000
#define CP_VAT_REG_B_GET_VCACHE_ENHANCE(cp_vat_reg_b) \
    ((((unsigned long)(cp_vat_reg_b)) & CP_VAT_REG_B_VCACHE_ENHANCE_MASK) >> CP_VAT_REG_B_VCACHE_ENHANCE_SHIFT)
#define CP_VAT_REG_B_SET_VCACHE_ENHANCE(cp_vat_reg_b, vcache_enhance) { \
    FDL_ASSERT(!((vcache_enhance) & ~((1 << CP_VAT_REG_B_VCACHE_ENHANCE_SIZE)-1))); \
    cp_vat_reg_b = (((unsigned long)(cp_vat_reg_b)) & ~CP_VAT_REG_B_VCACHE_ENHANCE_MASK) | (((unsigned long)(vcache_enhance)) << CP_VAT_REG_B_VCACHE_ENHANCE_SHIFT);\
}
#define CP_VAT_REG_B_TOTAL_SIZE 32
#define CP_VAT_REG_B(tex1Cnt, tex1Fmt, tex1Shft, tex2Cnt, tex2Fmt, tex2Shft, tex3Cnt, tex3Fmt, tex3Shft, tex4Cnt, tex4Fmt, vcache_enhance) \
    ((((unsigned long)(tex1Cnt)) << CP_VAT_REG_B_TEX1CNT_SHIFT) | \
    (((unsigned long)(tex1Fmt)) << CP_VAT_REG_B_TEX1FMT_SHIFT) | \
    (((unsigned long)(tex1Shft)) << CP_VAT_REG_B_TEX1SHFT_SHIFT) | \
    (((unsigned long)(tex2Cnt)) << CP_VAT_REG_B_TEX2CNT_SHIFT) | \
    (((unsigned long)(tex2Fmt)) << CP_VAT_REG_B_TEX2FMT_SHIFT) | \
    (((unsigned long)(tex2Shft)) << CP_VAT_REG_B_TEX2SHFT_SHIFT) | \
    (((unsigned long)(tex3Cnt)) << CP_VAT_REG_B_TEX3CNT_SHIFT) | \
    (((unsigned long)(tex3Fmt)) << CP_VAT_REG_B_TEX3FMT_SHIFT) | \
    (((unsigned long)(tex3Shft)) << CP_VAT_REG_B_TEX3SHFT_SHIFT) | \
    (((unsigned long)(tex4Cnt)) << CP_VAT_REG_B_TEX4CNT_SHIFT) | \
    (((unsigned long)(tex4Fmt)) << CP_VAT_REG_B_TEX4FMT_SHIFT) | \
    (((unsigned long)(vcache_enhance)) << CP_VAT_REG_B_VCACHE_ENHANCE_SHIFT))

/*
*  cp_vat_reg_c struct
*/
#define CP_VAT_REG_C_TEX4SHFT_SIZE  5
#define CP_VAT_REG_C_TEX4SHFT_SHIFT 0
#define CP_VAT_REG_C_TEX4SHFT_MASK  0x0000001f
#define CP_VAT_REG_C_GET_TEX4SHFT(cp_vat_reg_c) \
    ((((unsigned long)(cp_vat_reg_c)) & CP_VAT_REG_C_TEX4SHFT_MASK) >> CP_VAT_REG_C_TEX4SHFT_SHIFT)
#define CP_VAT_REG_C_SET_TEX4SHFT(cp_vat_reg_c, tex4Shft) { \
    FDL_ASSERT(!((tex4Shft) & ~((1 << CP_VAT_REG_C_TEX4SHFT_SIZE)-1))); \
    cp_vat_reg_c = (((unsigned long)(cp_vat_reg_c)) & ~CP_VAT_REG_C_TEX4SHFT_MASK) | (((unsigned long)(tex4Shft)) << CP_VAT_REG_C_TEX4SHFT_SHIFT);\
}
#define CP_VAT_REG_C_TEX5CNT_SIZE   1
#define CP_VAT_REG_C_TEX5CNT_SHIFT  5
#define CP_VAT_REG_C_TEX5CNT_MASK   0x00000020
#define CP_VAT_REG_C_GET_TEX5CNT(cp_vat_reg_c) \
    ((((unsigned long)(cp_vat_reg_c)) & CP_VAT_REG_C_TEX5CNT_MASK) >> CP_VAT_REG_C_TEX5CNT_SHIFT)
#define CP_VAT_REG_C_SET_TEX5CNT(cp_vat_reg_c, tex5Cnt) { \
    FDL_ASSERT(!((tex5Cnt) & ~((1 << CP_VAT_REG_C_TEX5CNT_SIZE)-1))); \
    cp_vat_reg_c = (((unsigned long)(cp_vat_reg_c)) & ~CP_VAT_REG_C_TEX5CNT_MASK) | (((unsigned long)(tex5Cnt)) << CP_VAT_REG_C_TEX5CNT_SHIFT);\
}
#define CP_VAT_REG_C_TEX5FMT_SIZE   3
#define CP_VAT_REG_C_TEX5FMT_SHIFT  6
#define CP_VAT_REG_C_TEX5FMT_MASK   0x000001c0
#define CP_VAT_REG_C_GET_TEX5FMT(cp_vat_reg_c) \
    ((((unsigned long)(cp_vat_reg_c)) & CP_VAT_REG_C_TEX5FMT_MASK) >> CP_VAT_REG_C_TEX5FMT_SHIFT)
#define CP_VAT_REG_C_SET_TEX5FMT(cp_vat_reg_c, tex5Fmt) { \
    FDL_ASSERT(!((tex5Fmt) & ~((1 << CP_VAT_REG_C_TEX5FMT_SIZE)-1))); \
    cp_vat_reg_c = (((unsigned long)(cp_vat_reg_c)) & ~CP_VAT_REG_C_TEX5FMT_MASK) | (((unsigned long)(tex5Fmt)) << CP_VAT_REG_C_TEX5FMT_SHIFT);\
}
#define CP_VAT_REG_C_TEX5SHFT_SIZE  5
#define CP_VAT_REG_C_TEX5SHFT_SHIFT 9
#define CP_VAT_REG_C_TEX5SHFT_MASK  0x00003e00
#define CP_VAT_REG_C_GET_TEX5SHFT(cp_vat_reg_c) \
    ((((unsigned long)(cp_vat_reg_c)) & CP_VAT_REG_C_TEX5SHFT_MASK) >> CP_VAT_REG_C_TEX5SHFT_SHIFT)
#define CP_VAT_REG_C_SET_TEX5SHFT(cp_vat_reg_c, tex5Shft) { \
    FDL_ASSERT(!((tex5Shft) & ~((1 << CP_VAT_REG_C_TEX5SHFT_SIZE)-1))); \
    cp_vat_reg_c = (((unsigned long)(cp_vat_reg_c)) & ~CP_VAT_REG_C_TEX5SHFT_MASK) | (((unsigned long)(tex5Shft)) << CP_VAT_REG_C_TEX5SHFT_SHIFT);\
}
#define CP_VAT_REG_C_TEX6CNT_SIZE   1
#define CP_VAT_REG_C_TEX6CNT_SHIFT  14
#define CP_VAT_REG_C_TEX6CNT_MASK   0x00004000
#define CP_VAT_REG_C_GET_TEX6CNT(cp_vat_reg_c) \
    ((((unsigned long)(cp_vat_reg_c)) & CP_VAT_REG_C_TEX6CNT_MASK) >> CP_VAT_REG_C_TEX6CNT_SHIFT)
#define CP_VAT_REG_C_SET_TEX6CNT(cp_vat_reg_c, tex6Cnt) { \
    FDL_ASSERT(!((tex6Cnt) & ~((1 << CP_VAT_REG_C_TEX6CNT_SIZE)-1))); \
    cp_vat_reg_c = (((unsigned long)(cp_vat_reg_c)) & ~CP_VAT_REG_C_TEX6CNT_MASK) | (((unsigned long)(tex6Cnt)) << CP_VAT_REG_C_TEX6CNT_SHIFT);\
}
#define CP_VAT_REG_C_TEX6FMT_SIZE   3
#define CP_VAT_REG_C_TEX6FMT_SHIFT  15
#define CP_VAT_REG_C_TEX6FMT_MASK   0x00038000
#define CP_VAT_REG_C_GET_TEX6FMT(cp_vat_reg_c) \
    ((((unsigned long)(cp_vat_reg_c)) & CP_VAT_REG_C_TEX6FMT_MASK) >> CP_VAT_REG_C_TEX6FMT_SHIFT)
#define CP_VAT_REG_C_SET_TEX6FMT(cp_vat_reg_c, tex6Fmt) { \
    FDL_ASSERT(!((tex6Fmt) & ~((1 << CP_VAT_REG_C_TEX6FMT_SIZE)-1))); \
    cp_vat_reg_c = (((unsigned long)(cp_vat_reg_c)) & ~CP_VAT_REG_C_TEX6FMT_MASK) | (((unsigned long)(tex6Fmt)) << CP_VAT_REG_C_TEX6FMT_SHIFT);\
}
#define CP_VAT_REG_C_TEX6SHFT_SIZE  5
#define CP_VAT_REG_C_TEX6SHFT_SHIFT 18
#define CP_VAT_REG_C_TEX6SHFT_MASK  0x007c0000
#define CP_VAT_REG_C_GET_TEX6SHFT(cp_vat_reg_c) \
    ((((unsigned long)(cp_vat_reg_c)) & CP_VAT_REG_C_TEX6SHFT_MASK) >> CP_VAT_REG_C_TEX6SHFT_SHIFT)
#define CP_VAT_REG_C_SET_TEX6SHFT(cp_vat_reg_c, tex6Shft) { \
    FDL_ASSERT(!((tex6Shft) & ~((1 << CP_VAT_REG_C_TEX6SHFT_SIZE)-1))); \
    cp_vat_reg_c = (((unsigned long)(cp_vat_reg_c)) & ~CP_VAT_REG_C_TEX6SHFT_MASK) | (((unsigned long)(tex6Shft)) << CP_VAT_REG_C_TEX6SHFT_SHIFT);\
}
#define CP_VAT_REG_C_TEX7CNT_SIZE   1
#define CP_VAT_REG_C_TEX7CNT_SHIFT  23
#define CP_VAT_REG_C_TEX7CNT_MASK   0x00800000
#define CP_VAT_REG_C_GET_TEX7CNT(cp_vat_reg_c) \
    ((((unsigned long)(cp_vat_reg_c)) & CP_VAT_REG_C_TEX7CNT_MASK) >> CP_VAT_REG_C_TEX7CNT_SHIFT)
#define CP_VAT_REG_C_SET_TEX7CNT(cp_vat_reg_c, tex7Cnt) { \
    FDL_ASSERT(!((tex7Cnt) & ~((1 << CP_VAT_REG_C_TEX7CNT_SIZE)-1))); \
    cp_vat_reg_c = (((unsigned long)(cp_vat_reg_c)) & ~CP_VAT_REG_C_TEX7CNT_MASK) | (((unsigned long)(tex7Cnt)) << CP_VAT_REG_C_TEX7CNT_SHIFT);\
}
#define CP_VAT_REG_C_TEX7FMT_SIZE   3
#define CP_VAT_REG_C_TEX7FMT_SHIFT  24
#define CP_VAT_REG_C_TEX7FMT_MASK   0x07000000
#define CP_VAT_REG_C_GET_TEX7FMT(cp_vat_reg_c) \
    ((((unsigned long)(cp_vat_reg_c)) & CP_VAT_REG_C_TEX7FMT_MASK) >> CP_VAT_REG_C_TEX7FMT_SHIFT)
#define CP_VAT_REG_C_SET_TEX7FMT(cp_vat_reg_c, tex7Fmt) { \
    FDL_ASSERT(!((tex7Fmt) & ~((1 << CP_VAT_REG_C_TEX7FMT_SIZE)-1))); \
    cp_vat_reg_c = (((unsigned long)(cp_vat_reg_c)) & ~CP_VAT_REG_C_TEX7FMT_MASK) | (((unsigned long)(tex7Fmt)) << CP_VAT_REG_C_TEX7FMT_SHIFT);\
}
#define CP_VAT_REG_C_TEX7SHFT_SIZE  5
#define CP_VAT_REG_C_TEX7SHFT_SHIFT 27
#define CP_VAT_REG_C_TEX7SHFT_MASK  0xf8000000
#define CP_VAT_REG_C_GET_TEX7SHFT(cp_vat_reg_c) \
    ((((unsigned long)(cp_vat_reg_c)) & CP_VAT_REG_C_TEX7SHFT_MASK) >> CP_VAT_REG_C_TEX7SHFT_SHIFT)
#define CP_VAT_REG_C_SET_TEX7SHFT(cp_vat_reg_c, tex7Shft) { \
    FDL_ASSERT(!((tex7Shft) & ~((1 << CP_VAT_REG_C_TEX7SHFT_SIZE)-1))); \
    cp_vat_reg_c = (((unsigned long)(cp_vat_reg_c)) & ~CP_VAT_REG_C_TEX7SHFT_MASK) | (((unsigned long)(tex7Shft)) << CP_VAT_REG_C_TEX7SHFT_SHIFT);\
}
#define CP_VAT_REG_C_TOTAL_SIZE 32
#define CP_VAT_REG_C(tex4Shft, tex5Cnt, tex5Fmt, tex5Shft, tex6Cnt, tex6Fmt, tex6Shft, tex7Cnt, tex7Fmt, tex7Shft) \
    ((((unsigned long)(tex4Shft)) << CP_VAT_REG_C_TEX4SHFT_SHIFT) | \
    (((unsigned long)(tex5Cnt)) << CP_VAT_REG_C_TEX5CNT_SHIFT) | \
    (((unsigned long)(tex5Fmt)) << CP_VAT_REG_C_TEX5FMT_SHIFT) | \
    (((unsigned long)(tex5Shft)) << CP_VAT_REG_C_TEX5SHFT_SHIFT) | \
    (((unsigned long)(tex6Cnt)) << CP_VAT_REG_C_TEX6CNT_SHIFT) | \
    (((unsigned long)(tex6Fmt)) << CP_VAT_REG_C_TEX6FMT_SHIFT) | \
    (((unsigned long)(tex6Shft)) << CP_VAT_REG_C_TEX6SHFT_SHIFT) | \
    (((unsigned long)(tex7Cnt)) << CP_VAT_REG_C_TEX7CNT_SHIFT) | \
    (((unsigned long)(tex7Fmt)) << CP_VAT_REG_C_TEX7FMT_SHIFT) | \
    (((unsigned long)(tex7Shft)) << CP_VAT_REG_C_TEX7SHFT_SHIFT))

/*
*  cp_matidx_reg_a struct
*/
#define CP_MATIDX_REG_A_POSIDX_SIZE 6
#define CP_MATIDX_REG_A_POSIDX_SHIFT    0
#define CP_MATIDX_REG_A_POSIDX_MASK 0x0000003f
#define CP_MATIDX_REG_A_GET_POSIDX(cp_matidx_reg_a) \
    ((((unsigned long)(cp_matidx_reg_a)) & CP_MATIDX_REG_A_POSIDX_MASK) >> CP_MATIDX_REG_A_POSIDX_SHIFT)
#define CP_MATIDX_REG_A_SET_POSIDX(cp_matidx_reg_a, posIdx) { \
    FDL_ASSERT(!((posIdx) & ~((1 << CP_MATIDX_REG_A_POSIDX_SIZE)-1))); \
    cp_matidx_reg_a = (((unsigned long)(cp_matidx_reg_a)) & ~CP_MATIDX_REG_A_POSIDX_MASK) | (((unsigned long)(posIdx)) << CP_MATIDX_REG_A_POSIDX_SHIFT);\
}
#define CP_MATIDX_REG_A_TEX0IDX_SIZE    6
#define CP_MATIDX_REG_A_TEX0IDX_SHIFT   6
#define CP_MATIDX_REG_A_TEX0IDX_MASK    0x00000fc0
#define CP_MATIDX_REG_A_GET_TEX0IDX(cp_matidx_reg_a) \
    ((((unsigned long)(cp_matidx_reg_a)) & CP_MATIDX_REG_A_TEX0IDX_MASK) >> CP_MATIDX_REG_A_TEX0IDX_SHIFT)
#define CP_MATIDX_REG_A_SET_TEX0IDX(cp_matidx_reg_a, tex0Idx) { \
    FDL_ASSERT(!((tex0Idx) & ~((1 << CP_MATIDX_REG_A_TEX0IDX_SIZE)-1))); \
    cp_matidx_reg_a = (((unsigned long)(cp_matidx_reg_a)) & ~CP_MATIDX_REG_A_TEX0IDX_MASK) | (((unsigned long)(tex0Idx)) << CP_MATIDX_REG_A_TEX0IDX_SHIFT);\
}
#define CP_MATIDX_REG_A_TEX1IDX_SIZE    6
#define CP_MATIDX_REG_A_TEX1IDX_SHIFT   12
#define CP_MATIDX_REG_A_TEX1IDX_MASK    0x0003f000
#define CP_MATIDX_REG_A_GET_TEX1IDX(cp_matidx_reg_a) \
    ((((unsigned long)(cp_matidx_reg_a)) & CP_MATIDX_REG_A_TEX1IDX_MASK) >> CP_MATIDX_REG_A_TEX1IDX_SHIFT)
#define CP_MATIDX_REG_A_SET_TEX1IDX(cp_matidx_reg_a, tex1Idx) { \
    FDL_ASSERT(!((tex1Idx) & ~((1 << CP_MATIDX_REG_A_TEX1IDX_SIZE)-1))); \
    cp_matidx_reg_a = (((unsigned long)(cp_matidx_reg_a)) & ~CP_MATIDX_REG_A_TEX1IDX_MASK) | (((unsigned long)(tex1Idx)) << CP_MATIDX_REG_A_TEX1IDX_SHIFT);\
}
#define CP_MATIDX_REG_A_TEX2IDX_SIZE    6
#define CP_MATIDX_REG_A_TEX2IDX_SHIFT   18
#define CP_MATIDX_REG_A_TEX2IDX_MASK    0x00fc0000
#define CP_MATIDX_REG_A_GET_TEX2IDX(cp_matidx_reg_a) \
    ((((unsigned long)(cp_matidx_reg_a)) & CP_MATIDX_REG_A_TEX2IDX_MASK) >> CP_MATIDX_REG_A_TEX2IDX_SHIFT)
#define CP_MATIDX_REG_A_SET_TEX2IDX(cp_matidx_reg_a, tex2Idx) { \
    FDL_ASSERT(!((tex2Idx) & ~((1 << CP_MATIDX_REG_A_TEX2IDX_SIZE)-1))); \
    cp_matidx_reg_a = (((unsigned long)(cp_matidx_reg_a)) & ~CP_MATIDX_REG_A_TEX2IDX_MASK) | (((unsigned long)(tex2Idx)) << CP_MATIDX_REG_A_TEX2IDX_SHIFT);\
}
#define CP_MATIDX_REG_A_TEX3IDX_SIZE    6
#define CP_MATIDX_REG_A_TEX3IDX_SHIFT   24
#define CP_MATIDX_REG_A_TEX3IDX_MASK    0x3f000000
#define CP_MATIDX_REG_A_GET_TEX3IDX(cp_matidx_reg_a) \
    ((((unsigned long)(cp_matidx_reg_a)) & CP_MATIDX_REG_A_TEX3IDX_MASK) >> CP_MATIDX_REG_A_TEX3IDX_SHIFT)
#define CP_MATIDX_REG_A_SET_TEX3IDX(cp_matidx_reg_a, tex3Idx) { \
    FDL_ASSERT(!((tex3Idx) & ~((1 << CP_MATIDX_REG_A_TEX3IDX_SIZE)-1))); \
    cp_matidx_reg_a = (((unsigned long)(cp_matidx_reg_a)) & ~CP_MATIDX_REG_A_TEX3IDX_MASK) | (((unsigned long)(tex3Idx)) << CP_MATIDX_REG_A_TEX3IDX_SHIFT);\
}
#define CP_MATIDX_REG_A_TOTAL_SIZE  30
#define CP_MATIDX_REG_A(posIdx, tex0Idx, tex1Idx, tex2Idx, tex3Idx) \
    ((((unsigned long)(posIdx)) << CP_MATIDX_REG_A_POSIDX_SHIFT) | \
    (((unsigned long)(tex0Idx)) << CP_MATIDX_REG_A_TEX0IDX_SHIFT) | \
    (((unsigned long)(tex1Idx)) << CP_MATIDX_REG_A_TEX1IDX_SHIFT) | \
    (((unsigned long)(tex2Idx)) << CP_MATIDX_REG_A_TEX2IDX_SHIFT) | \
    (((unsigned long)(tex3Idx)) << CP_MATIDX_REG_A_TEX3IDX_SHIFT))

/*
*  cp_matidx_reg_b struct
*/
#define CP_MATIDX_REG_B_TEX4IDX_SIZE    6
#define CP_MATIDX_REG_B_TEX4IDX_SHIFT   0
#define CP_MATIDX_REG_B_TEX4IDX_MASK    0x0000003f
#define CP_MATIDX_REG_B_GET_TEX4IDX(cp_matidx_reg_b) \
    ((((unsigned long)(cp_matidx_reg_b)) & CP_MATIDX_REG_B_TEX4IDX_MASK) >> CP_MATIDX_REG_B_TEX4IDX_SHIFT)
#define CP_MATIDX_REG_B_SET_TEX4IDX(cp_matidx_reg_b, tex4Idx) { \
    FDL_ASSERT(!((tex4Idx) & ~((1 << CP_MATIDX_REG_B_TEX4IDX_SIZE)-1))); \
    cp_matidx_reg_b = (((unsigned long)(cp_matidx_reg_b)) & ~CP_MATIDX_REG_B_TEX4IDX_MASK) | (((unsigned long)(tex4Idx)) << CP_MATIDX_REG_B_TEX4IDX_SHIFT);\
}
#define CP_MATIDX_REG_B_TEX5IDX_SIZE    6
#define CP_MATIDX_REG_B_TEX5IDX_SHIFT   6
#define CP_MATIDX_REG_B_TEX5IDX_MASK    0x00000fc0
#define CP_MATIDX_REG_B_GET_TEX5IDX(cp_matidx_reg_b) \
    ((((unsigned long)(cp_matidx_reg_b)) & CP_MATIDX_REG_B_TEX5IDX_MASK) >> CP_MATIDX_REG_B_TEX5IDX_SHIFT)
#define CP_MATIDX_REG_B_SET_TEX5IDX(cp_matidx_reg_b, tex5Idx) { \
    FDL_ASSERT(!((tex5Idx) & ~((1 << CP_MATIDX_REG_B_TEX5IDX_SIZE)-1))); \
    cp_matidx_reg_b = (((unsigned long)(cp_matidx_reg_b)) & ~CP_MATIDX_REG_B_TEX5IDX_MASK) | (((unsigned long)(tex5Idx)) << CP_MATIDX_REG_B_TEX5IDX_SHIFT);\
}
#define CP_MATIDX_REG_B_TEX6IDX_SIZE    6
#define CP_MATIDX_REG_B_TEX6IDX_SHIFT   12
#define CP_MATIDX_REG_B_TEX6IDX_MASK    0x0003f000
#define CP_MATIDX_REG_B_GET_TEX6IDX(cp_matidx_reg_b) \
    ((((unsigned long)(cp_matidx_reg_b)) & CP_MATIDX_REG_B_TEX6IDX_MASK) >> CP_MATIDX_REG_B_TEX6IDX_SHIFT)
#define CP_MATIDX_REG_B_SET_TEX6IDX(cp_matidx_reg_b, tex6Idx) { \
    FDL_ASSERT(!((tex6Idx) & ~((1 << CP_MATIDX_REG_B_TEX6IDX_SIZE)-1))); \
    cp_matidx_reg_b = (((unsigned long)(cp_matidx_reg_b)) & ~CP_MATIDX_REG_B_TEX6IDX_MASK) | (((unsigned long)(tex6Idx)) << CP_MATIDX_REG_B_TEX6IDX_SHIFT);\
}
#define CP_MATIDX_REG_B_TEX7IDX_SIZE    6
#define CP_MATIDX_REG_B_TEX7IDX_SHIFT   18
#define CP_MATIDX_REG_B_TEX7IDX_MASK    0x00fc0000
#define CP_MATIDX_REG_B_GET_TEX7IDX(cp_matidx_reg_b) \
    ((((unsigned long)(cp_matidx_reg_b)) & CP_MATIDX_REG_B_TEX7IDX_MASK) >> CP_MATIDX_REG_B_TEX7IDX_SHIFT)
#define CP_MATIDX_REG_B_SET_TEX7IDX(cp_matidx_reg_b, tex7Idx) { \
    FDL_ASSERT(!((tex7Idx) & ~((1 << CP_MATIDX_REG_B_TEX7IDX_SIZE)-1))); \
    cp_matidx_reg_b = (((unsigned long)(cp_matidx_reg_b)) & ~CP_MATIDX_REG_B_TEX7IDX_MASK) | (((unsigned long)(tex7Idx)) << CP_MATIDX_REG_B_TEX7IDX_SHIFT);\
}
#define CP_MATIDX_REG_B_TOTAL_SIZE  24
#define CP_MATIDX_REG_B(tex4Idx, tex5Idx, tex6Idx, tex7Idx) \
    ((((unsigned long)(tex4Idx)) << CP_MATIDX_REG_B_TEX4IDX_SHIFT) | \
    (((unsigned long)(tex5Idx)) << CP_MATIDX_REG_B_TEX5IDX_SHIFT) | \
    (((unsigned long)(tex6Idx)) << CP_MATIDX_REG_B_TEX6IDX_SHIFT) | \
    (((unsigned long)(tex7Idx)) << CP_MATIDX_REG_B_TEX7IDX_SHIFT))

/*
*  cp_array_base_reg struct
*/
#define CP_ARRAY_BASE_REG_BASE_SIZE 29
#define CP_ARRAY_BASE_REG_BASE_SHIFT    0
#define CP_ARRAY_BASE_REG_BASE_MASK 0x1fffffff
#define CP_ARRAY_BASE_REG_GET_BASE(cp_array_base_reg) \
    ((((unsigned long)(cp_array_base_reg)) & CP_ARRAY_BASE_REG_BASE_MASK) >> CP_ARRAY_BASE_REG_BASE_SHIFT)
#define CP_ARRAY_BASE_REG_SET_BASE(cp_array_base_reg, base) { \
    FDL_ASSERT(!((base) & ~((1 << CP_ARRAY_BASE_REG_BASE_SIZE)-1))); \
    cp_array_base_reg = (((unsigned long)(cp_array_base_reg)) & ~CP_ARRAY_BASE_REG_BASE_MASK) | (((unsigned long)(base)) << CP_ARRAY_BASE_REG_BASE_SHIFT);\
}
#define CP_ARRAY_BASE_REG_PAD0_SIZE 3
#define CP_ARRAY_BASE_REG_PAD0_SHIFT    29
#define CP_ARRAY_BASE_REG_PAD0_MASK 0xe0000000
#define CP_ARRAY_BASE_REG_GET_PAD0(cp_array_base_reg) \
    ((((unsigned long)(cp_array_base_reg)) & CP_ARRAY_BASE_REG_PAD0_MASK) >> CP_ARRAY_BASE_REG_PAD0_SHIFT)
#define CP_ARRAY_BASE_REG_SET_PAD0(cp_array_base_reg, pad0) { \
    FDL_ASSERT(!((pad0) & ~((1 << CP_ARRAY_BASE_REG_PAD0_SIZE)-1))); \
    cp_array_base_reg = (((unsigned long)(cp_array_base_reg)) & ~CP_ARRAY_BASE_REG_PAD0_MASK) | (((unsigned long)(pad0)) << CP_ARRAY_BASE_REG_PAD0_SHIFT);\
}
#define CP_ARRAY_BASE_REG_TOTAL_SIZE    32
#define CP_ARRAY_BASE_REG(base) \
    ((((unsigned long)(base)) << CP_ARRAY_BASE_REG_BASE_SHIFT))

/*
*  cp_array_stride_reg struct
*/
#define CP_ARRAY_STRIDE_REG_STRIDE_SIZE 8
#define CP_ARRAY_STRIDE_REG_STRIDE_SHIFT    0
#define CP_ARRAY_STRIDE_REG_STRIDE_MASK 0x000000ff
#define CP_ARRAY_STRIDE_REG_GET_STRIDE(cp_array_stride_reg) \
    ((((unsigned long)(cp_array_stride_reg)) & CP_ARRAY_STRIDE_REG_STRIDE_MASK) >> CP_ARRAY_STRIDE_REG_STRIDE_SHIFT)
#define CP_ARRAY_STRIDE_REG_SET_STRIDE(cp_array_stride_reg, stride) { \
    FDL_ASSERT(!((stride) & ~((1 << CP_ARRAY_STRIDE_REG_STRIDE_SIZE)-1))); \
    cp_array_stride_reg = (((unsigned long)(cp_array_stride_reg)) & ~CP_ARRAY_STRIDE_REG_STRIDE_MASK) | (((unsigned long)(stride)) << CP_ARRAY_STRIDE_REG_STRIDE_SHIFT);\
}
#define CP_ARRAY_STRIDE_REG_PAD0_SIZE   24
#define CP_ARRAY_STRIDE_REG_PAD0_SHIFT  8
#define CP_ARRAY_STRIDE_REG_PAD0_MASK   0xffffff00
#define CP_ARRAY_STRIDE_REG_GET_PAD0(cp_array_stride_reg) \
    ((((unsigned long)(cp_array_stride_reg)) & CP_ARRAY_STRIDE_REG_PAD0_MASK) >> CP_ARRAY_STRIDE_REG_PAD0_SHIFT)
#define CP_ARRAY_STRIDE_REG_SET_PAD0(cp_array_stride_reg, pad0) { \
    FDL_ASSERT(!((pad0) & ~((1 << CP_ARRAY_STRIDE_REG_PAD0_SIZE)-1))); \
    cp_array_stride_reg = (((unsigned long)(cp_array_stride_reg)) & ~CP_ARRAY_STRIDE_REG_PAD0_MASK) | (((unsigned long)(pad0)) << CP_ARRAY_STRIDE_REG_PAD0_SHIFT);\
}
#define CP_ARRAY_STRIDE_REG_TOTAL_SIZE  32
#define CP_ARRAY_STRIDE_REG(stride) \
    ((((unsigned long)(stride)) << CP_ARRAY_STRIDE_REG_STRIDE_SHIFT))

/*
*  cp_stat_enable_reg struct
*/
#define CP_STAT_ENABLE_REG_VC_STAT_SIZE 1
#define CP_STAT_ENABLE_REG_VC_STAT_SHIFT    0
#define CP_STAT_ENABLE_REG_VC_STAT_MASK 0x00000001
#define CP_STAT_ENABLE_REG_GET_VC_STAT(cp_stat_enable_reg) \
    ((((unsigned long)(cp_stat_enable_reg)) & CP_STAT_ENABLE_REG_VC_STAT_MASK) >> CP_STAT_ENABLE_REG_VC_STAT_SHIFT)
#define CP_STAT_ENABLE_REG_SET_VC_STAT(cp_stat_enable_reg, vc_stat) { \
    FDL_ASSERT(!((vc_stat) & ~((1 << CP_STAT_ENABLE_REG_VC_STAT_SIZE)-1))); \
    cp_stat_enable_reg = (((unsigned long)(cp_stat_enable_reg)) & ~CP_STAT_ENABLE_REG_VC_STAT_MASK) | (((unsigned long)(vc_stat)) << CP_STAT_ENABLE_REG_VC_STAT_SHIFT);\
}
#define CP_STAT_ENABLE_REG_PAD0_SIZE    1
#define CP_STAT_ENABLE_REG_PAD0_SHIFT   1
#define CP_STAT_ENABLE_REG_PAD0_MASK    0x00000002
#define CP_STAT_ENABLE_REG_GET_PAD0(cp_stat_enable_reg) \
    ((((unsigned long)(cp_stat_enable_reg)) & CP_STAT_ENABLE_REG_PAD0_MASK) >> CP_STAT_ENABLE_REG_PAD0_SHIFT)
#define CP_STAT_ENABLE_REG_SET_PAD0(cp_stat_enable_reg, pad0) { \
    FDL_ASSERT(!((pad0) & ~((1 << CP_STAT_ENABLE_REG_PAD0_SIZE)-1))); \
    cp_stat_enable_reg = (((unsigned long)(cp_stat_enable_reg)) & ~CP_STAT_ENABLE_REG_PAD0_MASK) | (((unsigned long)(pad0)) << CP_STAT_ENABLE_REG_PAD0_SHIFT);\
}
#define CP_STAT_ENABLE_REG_FRCLK_SIZE   1
#define CP_STAT_ENABLE_REG_FRCLK_SHIFT  2
#define CP_STAT_ENABLE_REG_FRCLK_MASK   0x00000004
#define CP_STAT_ENABLE_REG_GET_FRCLK(cp_stat_enable_reg) \
    ((((unsigned long)(cp_stat_enable_reg)) & CP_STAT_ENABLE_REG_FRCLK_MASK) >> CP_STAT_ENABLE_REG_FRCLK_SHIFT)
#define CP_STAT_ENABLE_REG_SET_FRCLK(cp_stat_enable_reg, frclk) { \
    FDL_ASSERT(!((frclk) & ~((1 << CP_STAT_ENABLE_REG_FRCLK_SIZE)-1))); \
    cp_stat_enable_reg = (((unsigned long)(cp_stat_enable_reg)) & ~CP_STAT_ENABLE_REG_FRCLK_MASK) | (((unsigned long)(frclk)) << CP_STAT_ENABLE_REG_FRCLK_SHIFT);\
}
#define CP_STAT_ENABLE_REG_TOTAL_SIZE   3
#define CP_STAT_ENABLE_REG(vc_stat, frclk) \
    ((((unsigned long)(vc_stat)) << CP_STAT_ENABLE_REG_VC_STAT_SHIFT) | \
    (((unsigned long)(frclk)) << CP_STAT_ENABLE_REG_FRCLK_SHIFT))

/*
*  cp_stat_sel_reg struct
*/
#define CP_STAT_SEL_REG_ATTR_SEL_SIZE   4
#define CP_STAT_SEL_REG_ATTR_SEL_SHIFT  0
#define CP_STAT_SEL_REG_ATTR_SEL_MASK   0x0000000f
#define CP_STAT_SEL_REG_GET_ATTR_SEL(cp_stat_sel_reg) \
    ((((unsigned long)(cp_stat_sel_reg)) & CP_STAT_SEL_REG_ATTR_SEL_MASK) >> CP_STAT_SEL_REG_ATTR_SEL_SHIFT)
#define CP_STAT_SEL_REG_SET_ATTR_SEL(cp_stat_sel_reg, attr_sel) { \
    FDL_ASSERT(!((attr_sel) & ~((1 << CP_STAT_SEL_REG_ATTR_SEL_SIZE)-1))); \
    cp_stat_sel_reg = (((unsigned long)(cp_stat_sel_reg)) & ~CP_STAT_SEL_REG_ATTR_SEL_MASK) | (((unsigned long)(attr_sel)) << CP_STAT_SEL_REG_ATTR_SEL_SHIFT);\
}
#define CP_STAT_SEL_REG_STALLPERF_SEL_SIZE  4
#define CP_STAT_SEL_REG_STALLPERF_SEL_SHIFT 4
#define CP_STAT_SEL_REG_STALLPERF_SEL_MASK  0x000000f0
#define CP_STAT_SEL_REG_GET_STALLPERF_SEL(cp_stat_sel_reg) \
    ((((unsigned long)(cp_stat_sel_reg)) & CP_STAT_SEL_REG_STALLPERF_SEL_MASK) >> CP_STAT_SEL_REG_STALLPERF_SEL_SHIFT)
#define CP_STAT_SEL_REG_SET_STALLPERF_SEL(cp_stat_sel_reg, stallperf_sel) { \
    FDL_ASSERT(!((stallperf_sel) & ~((1 << CP_STAT_SEL_REG_STALLPERF_SEL_SIZE)-1))); \
    cp_stat_sel_reg = (((unsigned long)(cp_stat_sel_reg)) & ~CP_STAT_SEL_REG_STALLPERF_SEL_MASK) | (((unsigned long)(stallperf_sel)) << CP_STAT_SEL_REG_STALLPERF_SEL_SHIFT);\
}
#define CP_STAT_SEL_REG_TOTAL_SIZE  8
#define CP_STAT_SEL_REG(attr_sel, stallperf_sel) \
    ((((unsigned long)(attr_sel)) << CP_STAT_SEL_REG_ATTR_SEL_SHIFT) | \
    (((unsigned long)(stallperf_sel)) << CP_STAT_SEL_REG_STALLPERF_SEL_SHIFT))

/*
*  cp_stallperf_sel enum
*/
#define STALLPERF_ZERO  0x00000000
#define STALLPERF_ONE   0x00000001
#define ELEMQ_FULL  0x00000002
#define MISSQ_FULL  0x00000003
#define MEMREQ_FULL 0x00000004
#define VC_STATCNT7 0x00000005
#define VC_MISS_REP_FULL    0x00000006
#define VC_STALL_STMBUFLOW  0x00000007
#define VTX_CNT 0x00000008
#define ALL_STALL   0x00000009
#define CP_STALLPERF_SEL_UNUSED_10  0x0000000a
#define CP_STALLPERF_SEL_UNUSED_11  0x0000000b
#define CP_STALLPERF_SEL_UNUSED_12  0x0000000c
#define CP_STALLPERF_SEL_UNUSED_13  0x0000000d
#define CP_STALLPERF_SEL_UNUSED_14  0x0000000e
#define CP_STALLPERF_SEL_UNUSED_15  0x0000000f

/*
*  cp_array_addr value
*/
#define ATTR_ARRAY_POS  0x0
#define ATTR_ARRAY_NRM  0x1
#define ATTR_ARRAY_COL0 0x2
#define ATTR_ARRAY_COL1 0x3
#define ATTR_ARRAY_TEX0 0x4
#define ATTR_ARRAY_TEX1 0x5
#define ATTR_ARRAY_TEX2 0x6
#define ATTR_ARRAY_TEX3 0x7
#define ATTR_ARRAY_TEX4 0x8
#define ATTR_ARRAY_TEX5 0x9
#define ATTR_ARRAY_TEX6 0xa
#define ATTR_ARRAY_TEX7 0xb
#define ATTR_ARRAY_XFINDREGA    0xc
#define ATTR_ARRAY_XFINDREGB    0xd
#define ATTR_ARRAY_XFINDREGC    0xe
#define ATTR_ARRAY_XFINDREGD    0xf

/*
*  cp_xf_loadregs struct
*/
#define CP_XF_LOADREGS_ADDR_SIZE    16
#define CP_XF_LOADREGS_ADDR_SHIFT   0
#define CP_XF_LOADREGS_ADDR_MASK    0x0000ffff
#define CP_XF_LOADREGS_GET_ADDR(cp_xf_loadregs) \
    ((((unsigned long)(cp_xf_loadregs)) & CP_XF_LOADREGS_ADDR_MASK) >> CP_XF_LOADREGS_ADDR_SHIFT)
#define CP_XF_LOADREGS_SET_ADDR(cp_xf_loadregs, addr) { \
    FDL_ASSERT(!((addr) & ~((1 << CP_XF_LOADREGS_ADDR_SIZE)-1))); \
    cp_xf_loadregs = (((unsigned long)(cp_xf_loadregs)) & ~CP_XF_LOADREGS_ADDR_MASK) | (((unsigned long)(addr)) << CP_XF_LOADREGS_ADDR_SHIFT);\
}
#define CP_XF_LOADREGS_CNT_SIZE 4
#define CP_XF_LOADREGS_CNT_SHIFT    16
#define CP_XF_LOADREGS_CNT_MASK 0x000f0000
#define CP_XF_LOADREGS_GET_CNT(cp_xf_loadregs) \
    ((((unsigned long)(cp_xf_loadregs)) & CP_XF_LOADREGS_CNT_MASK) >> CP_XF_LOADREGS_CNT_SHIFT)
#define CP_XF_LOADREGS_SET_CNT(cp_xf_loadregs, cnt) { \
    FDL_ASSERT(!((cnt) & ~((1 << CP_XF_LOADREGS_CNT_SIZE)-1))); \
    cp_xf_loadregs = (((unsigned long)(cp_xf_loadregs)) & ~CP_XF_LOADREGS_CNT_MASK) | (((unsigned long)(cnt)) << CP_XF_LOADREGS_CNT_SHIFT);\
}
#define CP_XF_LOADREGS_TOTAL_SIZE   20
#define CP_XF_LOADREGS_UNUSED_SIZE  12

#define CP_XF_LOADREGS(addr, cnt) \
    ((((unsigned long)(addr)) << CP_XF_LOADREGS_ADDR_SHIFT) | \
    (((unsigned long)(cnt)) << CP_XF_LOADREGS_CNT_SHIFT))

/*
*  cp_xf_loadindex struct
*/
#define CP_XF_LOADINDEX_ADDR_SIZE   12
#define CP_XF_LOADINDEX_ADDR_SHIFT  0
#define CP_XF_LOADINDEX_ADDR_MASK   0x00000fff
#define CP_XF_LOADINDEX_GET_ADDR(cp_xf_loadindex) \
    ((((unsigned long)(cp_xf_loadindex)) & CP_XF_LOADINDEX_ADDR_MASK) >> CP_XF_LOADINDEX_ADDR_SHIFT)
#define CP_XF_LOADINDEX_SET_ADDR(cp_xf_loadindex, addr) { \
    FDL_ASSERT(!((addr) & ~((1 << CP_XF_LOADINDEX_ADDR_SIZE)-1))); \
    cp_xf_loadindex = (((unsigned long)(cp_xf_loadindex)) & ~CP_XF_LOADINDEX_ADDR_MASK) | (((unsigned long)(addr)) << CP_XF_LOADINDEX_ADDR_SHIFT);\
}
#define CP_XF_LOADINDEX_CNT_SIZE    4
#define CP_XF_LOADINDEX_CNT_SHIFT   12
#define CP_XF_LOADINDEX_CNT_MASK    0x0000f000
#define CP_XF_LOADINDEX_GET_CNT(cp_xf_loadindex) \
    ((((unsigned long)(cp_xf_loadindex)) & CP_XF_LOADINDEX_CNT_MASK) >> CP_XF_LOADINDEX_CNT_SHIFT)
#define CP_XF_LOADINDEX_SET_CNT(cp_xf_loadindex, cnt) { \
    FDL_ASSERT(!((cnt) & ~((1 << CP_XF_LOADINDEX_CNT_SIZE)-1))); \
    cp_xf_loadindex = (((unsigned long)(cp_xf_loadindex)) & ~CP_XF_LOADINDEX_CNT_MASK) | (((unsigned long)(cnt)) << CP_XF_LOADINDEX_CNT_SHIFT);\
}
#define CP_XF_LOADINDEX_IDX_SIZE    16
#define CP_XF_LOADINDEX_IDX_SHIFT   16
#define CP_XF_LOADINDEX_IDX_MASK    0xffff0000
#define CP_XF_LOADINDEX_GET_IDX(cp_xf_loadindex) \
    ((((unsigned long)(cp_xf_loadindex)) & CP_XF_LOADINDEX_IDX_MASK) >> CP_XF_LOADINDEX_IDX_SHIFT)
#define CP_XF_LOADINDEX_SET_IDX(cp_xf_loadindex, idx) { \
    FDL_ASSERT(!((idx) & ~((1 << CP_XF_LOADINDEX_IDX_SIZE)-1))); \
    cp_xf_loadindex = (((unsigned long)(cp_xf_loadindex)) & ~CP_XF_LOADINDEX_IDX_MASK) | (((unsigned long)(idx)) << CP_XF_LOADINDEX_IDX_SHIFT);\
}
#define CP_XF_LOADINDEX_TOTAL_SIZE  32
#define CP_XF_LOADINDEX(addr, cnt, idx) \
    ((((unsigned long)(addr)) << CP_XF_LOADINDEX_ADDR_SHIFT) | \
    (((unsigned long)(cnt)) << CP_XF_LOADINDEX_CNT_SHIFT) | \
    (((unsigned long)(idx)) << CP_XF_LOADINDEX_IDX_SHIFT))

/*
*  cp_callobj_d1 struct
*/
#define CP_CALLOBJ_D1_PAD0_SIZE 5
#define CP_CALLOBJ_D1_PAD0_SHIFT    0
#define CP_CALLOBJ_D1_PAD0_MASK 0x0000001f
#define CP_CALLOBJ_D1_GET_PAD0(cp_callobj_d1) \
    ((((unsigned long)(cp_callobj_d1)) & CP_CALLOBJ_D1_PAD0_MASK) >> CP_CALLOBJ_D1_PAD0_SHIFT)
#define CP_CALLOBJ_D1_SET_PAD0(cp_callobj_d1, pad0) { \
    FDL_ASSERT(!((pad0) & ~((1 << CP_CALLOBJ_D1_PAD0_SIZE)-1))); \
    cp_callobj_d1 = (((unsigned long)(cp_callobj_d1)) & ~CP_CALLOBJ_D1_PAD0_MASK) | (((unsigned long)(pad0)) << CP_CALLOBJ_D1_PAD0_SHIFT);\
}
#define CP_CALLOBJ_D1_ADDR_SIZE 24
#define CP_CALLOBJ_D1_ADDR_SHIFT    5
#define CP_CALLOBJ_D1_ADDR_MASK 0x1fffffe0
#define CP_CALLOBJ_D1_GET_ADDR(cp_callobj_d1) \
    ((((unsigned long)(cp_callobj_d1)) & CP_CALLOBJ_D1_ADDR_MASK) >> CP_CALLOBJ_D1_ADDR_SHIFT)
#define CP_CALLOBJ_D1_SET_ADDR(cp_callobj_d1, addr) { \
    FDL_ASSERT(!((addr) & ~((1 << CP_CALLOBJ_D1_ADDR_SIZE)-1))); \
    cp_callobj_d1 = (((unsigned long)(cp_callobj_d1)) & ~CP_CALLOBJ_D1_ADDR_MASK) | (((unsigned long)(addr)) << CP_CALLOBJ_D1_ADDR_SHIFT);\
}
#define CP_CALLOBJ_D1_TOTAL_SIZE    29
#define CP_CALLOBJ_D1_UNUSED_SIZE   3

#define CP_CALLOBJ_D1(addr) \
    ((((unsigned long)(addr)) << CP_CALLOBJ_D1_ADDR_SHIFT))

/*
*  cp_callobj_d2 struct
*/
#define CP_CALLOBJ_D2_PAD0_SIZE 5
#define CP_CALLOBJ_D2_PAD0_SHIFT    0
#define CP_CALLOBJ_D2_PAD0_MASK 0x0000001f
#define CP_CALLOBJ_D2_GET_PAD0(cp_callobj_d2) \
    ((((unsigned long)(cp_callobj_d2)) & CP_CALLOBJ_D2_PAD0_MASK) >> CP_CALLOBJ_D2_PAD0_SHIFT)
#define CP_CALLOBJ_D2_SET_PAD0(cp_callobj_d2, pad0) { \
    FDL_ASSERT(!((pad0) & ~((1 << CP_CALLOBJ_D2_PAD0_SIZE)-1))); \
    cp_callobj_d2 = (((unsigned long)(cp_callobj_d2)) & ~CP_CALLOBJ_D2_PAD0_MASK) | (((unsigned long)(pad0)) << CP_CALLOBJ_D2_PAD0_SHIFT);\
}
#define CP_CALLOBJ_D2_CNT_SIZE  24
#define CP_CALLOBJ_D2_CNT_SHIFT 5
#define CP_CALLOBJ_D2_CNT_MASK  0x1fffffe0
#define CP_CALLOBJ_D2_GET_CNT(cp_callobj_d2) \
    ((((unsigned long)(cp_callobj_d2)) & CP_CALLOBJ_D2_CNT_MASK) >> CP_CALLOBJ_D2_CNT_SHIFT)
#define CP_CALLOBJ_D2_SET_CNT(cp_callobj_d2, cnt) { \
    FDL_ASSERT(!((cnt) & ~((1 << CP_CALLOBJ_D2_CNT_SIZE)-1))); \
    cp_callobj_d2 = (((unsigned long)(cp_callobj_d2)) & ~CP_CALLOBJ_D2_CNT_MASK) | (((unsigned long)(cnt)) << CP_CALLOBJ_D2_CNT_SHIFT);\
}
#define CP_CALLOBJ_D2_TOTAL_SIZE    29
#define CP_CALLOBJ_D2_UNUSED_SIZE   3

#define CP_CALLOBJ_D2(cnt) \
    ((((unsigned long)(cnt)) << CP_CALLOBJ_D2_CNT_SHIFT))



#define SC_CP_OPCODE_SET_INDEX(line, cp_opcode,index) \
	FAST_GPFLAGSET(line, cp_opcode,index,CP_OPCODE_INDEX)

#define SC_CP_OPCODE_SET_CMD(line, cp_opcode,cmd) \
	FAST_GPFLAGSET(line, cp_opcode,cmd,CP_OPCODE_CMD)

#define SC_CP_STREAM_REG_SET_INDEX(line, cp_stream_reg,index) \
	FAST_GPFLAGSET(line, cp_stream_reg,index,CP_STREAM_REG_INDEX)

#define SC_CP_STREAM_REG_SET_ADDR(line, cp_stream_reg,addr) \
	FAST_GPFLAGSET(line, cp_stream_reg,addr,CP_STREAM_REG_ADDR)

#define SC_CP_REG_STATUS_SET_OVFL(line, cp_reg_status,ovfl) \
	FAST_GPFLAGSET(line, cp_reg_status,ovfl,CP_REG_STATUS_OVFL)

#define SC_CP_REG_STATUS_SET_UNFL(line, cp_reg_status,unfl) \
	FAST_GPFLAGSET(line, cp_reg_status,unfl,CP_REG_STATUS_UNFL)

#define SC_CP_REG_STATUS_SET_FIFO_RDIDLE(line, cp_reg_status,fifo_rdidle) \
	FAST_GPFLAGSET(line, cp_reg_status,fifo_rdidle,CP_REG_STATUS_FIFO_RDIDLE)

#define SC_CP_REG_STATUS_SET_CPIDLE(line, cp_reg_status,cpidle) \
	FAST_GPFLAGSET(line, cp_reg_status,cpidle,CP_REG_STATUS_CPIDLE)

#define SC_CP_REG_STATUS_SET_FIFOBRK(line, cp_reg_status,fifobrk) \
	FAST_GPFLAGSET(line, cp_reg_status,fifobrk,CP_REG_STATUS_FIFOBRK)

#define SC_CP_REG_ENABLE_SET_FIFORD(line, cp_reg_enable,fiford) \
	FAST_GPFLAGSET(line, cp_reg_enable,fiford,CP_REG_ENABLE_FIFORD)

#define SC_CP_REG_ENABLE_SET_FIFOBRK(line, cp_reg_enable,fifobrk) \
	FAST_GPFLAGSET(line, cp_reg_enable,fifobrk,CP_REG_ENABLE_FIFOBRK)

#define SC_CP_REG_ENABLE_SET_OVFLINT(line, cp_reg_enable,ovflint) \
	FAST_GPFLAGSET(line, cp_reg_enable,ovflint,CP_REG_ENABLE_OVFLINT)

#define SC_CP_REG_ENABLE_SET_UNFLINT(line, cp_reg_enable,unflint) \
	FAST_GPFLAGSET(line, cp_reg_enable,unflint,CP_REG_ENABLE_UNFLINT)

#define SC_CP_REG_ENABLE_SET_WRPTRINC(line, cp_reg_enable,wrptrinc) \
	FAST_GPFLAGSET(line, cp_reg_enable,wrptrinc,CP_REG_ENABLE_WRPTRINC)

#define SC_CP_REG_ENABLE_SET_FIFOBRKINT(line, cp_reg_enable,fifobrkint) \
	FAST_GPFLAGSET(line, cp_reg_enable,fifobrkint,CP_REG_ENABLE_FIFOBRKINT)

#define SC_CP_REG_CLR_SET_OVFLINT(line, cp_reg_clr,ovflint) \
	FAST_GPFLAGSET(line, cp_reg_clr,ovflint,CP_REG_CLR_OVFLINT)

#define SC_CP_REG_CLR_SET_UNFLINT(line, cp_reg_clr,unflint) \
	FAST_GPFLAGSET(line, cp_reg_clr,unflint,CP_REG_CLR_UNFLINT)

#define SC_CP_REG_CLR_SET_PERFCNT(line, cp_reg_clr,perfcnt) \
	FAST_GPFLAGSET(line, cp_reg_clr,perfcnt,CP_REG_CLR_PERFCNT)

#define SC_CP_REG_MEMPERFSEL_SET_PERFSEL(line, cp_reg_memperfsel,perfsel) \
	FAST_GPFLAGSET(line, cp_reg_memperfsel,perfsel,CP_REG_MEMPERFSEL_PERFSEL)

#define SC_CP_REG_FIFO_BASEL_SET_PAD0(line, cp_reg_fifo_basel,pad0) \
	FAST_GPFLAGSET(line, cp_reg_fifo_basel,pad0,CP_REG_FIFO_BASEL_PAD0)

#define SC_CP_REG_FIFO_BASEL_SET_ADDR(line, cp_reg_fifo_basel,addr) \
	FAST_GPFLAGSET(line, cp_reg_fifo_basel,addr,CP_REG_FIFO_BASEL_ADDR)

#define SC_CP_REG_FIFO_BASEH_SET_ADDR(line, cp_reg_fifo_baseh,addr) \
	FAST_GPFLAGSET(line, cp_reg_fifo_baseh,addr,CP_REG_FIFO_BASEH_ADDR)

#define SC_CP_REG_FIFO_TOPL_SET_PAD0(line, cp_reg_fifo_topl,pad0) \
	FAST_GPFLAGSET(line, cp_reg_fifo_topl,pad0,CP_REG_FIFO_TOPL_PAD0)

#define SC_CP_REG_FIFO_TOPL_SET_ADDR(line, cp_reg_fifo_topl,addr) \
	FAST_GPFLAGSET(line, cp_reg_fifo_topl,addr,CP_REG_FIFO_TOPL_ADDR)

#define SC_CP_REG_FIFO_TOPH_SET_ADDR(line, cp_reg_fifo_toph,addr) \
	FAST_GPFLAGSET(line, cp_reg_fifo_toph,addr,CP_REG_FIFO_TOPH_ADDR)

#define SC_CP_REG_FIFO_HICNTL_SET_PAD0(line, cp_reg_fifo_hicntl,pad0) \
	FAST_GPFLAGSET(line, cp_reg_fifo_hicntl,pad0,CP_REG_FIFO_HICNTL_PAD0)

#define SC_CP_REG_FIFO_HICNTL_SET_ADDR(line, cp_reg_fifo_hicntl,addr) \
	FAST_GPFLAGSET(line, cp_reg_fifo_hicntl,addr,CP_REG_FIFO_HICNTL_ADDR)

#define SC_CP_REG_FIFO_HICNTH_SET_ADDR(line, cp_reg_fifo_hicnth,addr) \
	FAST_GPFLAGSET(line, cp_reg_fifo_hicnth,addr,CP_REG_FIFO_HICNTH_ADDR)

#define SC_CP_REG_FIFO_LOCNTL_SET_PAD0(line, cp_reg_fifo_locntl,pad0) \
	FAST_GPFLAGSET(line, cp_reg_fifo_locntl,pad0,CP_REG_FIFO_LOCNTL_PAD0)

#define SC_CP_REG_FIFO_LOCNTL_SET_ADDR(line, cp_reg_fifo_locntl,addr) \
	FAST_GPFLAGSET(line, cp_reg_fifo_locntl,addr,CP_REG_FIFO_LOCNTL_ADDR)

#define SC_CP_REG_FIFO_LOCNTH_SET_ADDR(line, cp_reg_fifo_locnth,addr) \
	FAST_GPFLAGSET(line, cp_reg_fifo_locnth,addr,CP_REG_FIFO_LOCNTH_ADDR)

#define SC_CP_REG_FIFO_COUNTL_SET_PAD0(line, cp_reg_fifo_countl,pad0) \
	FAST_GPFLAGSET(line, cp_reg_fifo_countl,pad0,CP_REG_FIFO_COUNTL_PAD0)

#define SC_CP_REG_FIFO_COUNTL_SET_ADDR(line, cp_reg_fifo_countl,addr) \
	FAST_GPFLAGSET(line, cp_reg_fifo_countl,addr,CP_REG_FIFO_COUNTL_ADDR)

#define SC_CP_REG_FIFO_COUNTH_SET_ADDR(line, cp_reg_fifo_counth,addr) \
	FAST_GPFLAGSET(line, cp_reg_fifo_counth,addr,CP_REG_FIFO_COUNTH_ADDR)

#define SC_CP_REG_FIFO_WPTRL_SET_PAD0(line, cp_reg_fifo_wptrl,pad0) \
	FAST_GPFLAGSET(line, cp_reg_fifo_wptrl,pad0,CP_REG_FIFO_WPTRL_PAD0)

#define SC_CP_REG_FIFO_WPTRL_SET_ADDR(line, cp_reg_fifo_wptrl,addr) \
	FAST_GPFLAGSET(line, cp_reg_fifo_wptrl,addr,CP_REG_FIFO_WPTRL_ADDR)

#define SC_CP_REG_FIFO_WPTRH_SET_ADDR(line, cp_reg_fifo_wptrh,addr) \
	FAST_GPFLAGSET(line, cp_reg_fifo_wptrh,addr,CP_REG_FIFO_WPTRH_ADDR)

#define SC_CP_REG_FIFO_RPTRL_SET_PAD0(line, cp_reg_fifo_rptrl,pad0) \
	FAST_GPFLAGSET(line, cp_reg_fifo_rptrl,pad0,CP_REG_FIFO_RPTRL_PAD0)

#define SC_CP_REG_FIFO_RPTRL_SET_ADDR(line, cp_reg_fifo_rptrl,addr) \
	FAST_GPFLAGSET(line, cp_reg_fifo_rptrl,addr,CP_REG_FIFO_RPTRL_ADDR)

#define SC_CP_REG_FIFO_RPTRH_SET_ADDR(line, cp_reg_fifo_rptrh,addr) \
	FAST_GPFLAGSET(line, cp_reg_fifo_rptrh,addr,CP_REG_FIFO_RPTRH_ADDR)

#define SC_CP_REG_FIFO_BRKL_SET_PAD0(line, cp_reg_fifo_brkl,pad0) \
	FAST_GPFLAGSET(line, cp_reg_fifo_brkl,pad0,CP_REG_FIFO_BRKL_PAD0)

#define SC_CP_REG_FIFO_BRKL_SET_ADDR(line, cp_reg_fifo_brkl,addr) \
	FAST_GPFLAGSET(line, cp_reg_fifo_brkl,addr,CP_REG_FIFO_BRKL_ADDR)

#define SC_CP_REG_FIFO_BRKH_SET_ADDR(line, cp_reg_fifo_brkh,addr) \
	FAST_GPFLAGSET(line, cp_reg_fifo_brkh,addr,CP_REG_FIFO_BRKH_ADDR)

#define SC_CP_VCD_REG_LO_SET_PMIDX(line, cp_vcd_reg_lo,pmidx) \
	FAST_GPFLAGSET(line, cp_vcd_reg_lo,pmidx,CP_VCD_REG_LO_PMIDX)

#define SC_CP_VCD_REG_LO_SET_T0MIDX(line, cp_vcd_reg_lo,t0midx) \
	FAST_GPFLAGSET(line, cp_vcd_reg_lo,t0midx,CP_VCD_REG_LO_T0MIDX)

#define SC_CP_VCD_REG_LO_SET_T1MIDX(line, cp_vcd_reg_lo,t1midx) \
	FAST_GPFLAGSET(line, cp_vcd_reg_lo,t1midx,CP_VCD_REG_LO_T1MIDX)

#define SC_CP_VCD_REG_LO_SET_T2MIDX(line, cp_vcd_reg_lo,t2midx) \
	FAST_GPFLAGSET(line, cp_vcd_reg_lo,t2midx,CP_VCD_REG_LO_T2MIDX)

#define SC_CP_VCD_REG_LO_SET_T3MIDX(line, cp_vcd_reg_lo,t3midx) \
	FAST_GPFLAGSET(line, cp_vcd_reg_lo,t3midx,CP_VCD_REG_LO_T3MIDX)

#define SC_CP_VCD_REG_LO_SET_T4MIDX(line, cp_vcd_reg_lo,t4midx) \
	FAST_GPFLAGSET(line, cp_vcd_reg_lo,t4midx,CP_VCD_REG_LO_T4MIDX)

#define SC_CP_VCD_REG_LO_SET_T5MIDX(line, cp_vcd_reg_lo,t5midx) \
	FAST_GPFLAGSET(line, cp_vcd_reg_lo,t5midx,CP_VCD_REG_LO_T5MIDX)

#define SC_CP_VCD_REG_LO_SET_T6MIDX(line, cp_vcd_reg_lo,t6midx) \
	FAST_GPFLAGSET(line, cp_vcd_reg_lo,t6midx,CP_VCD_REG_LO_T6MIDX)

#define SC_CP_VCD_REG_LO_SET_T7MIDX(line, cp_vcd_reg_lo,t7midx) \
	FAST_GPFLAGSET(line, cp_vcd_reg_lo,t7midx,CP_VCD_REG_LO_T7MIDX)

#define SC_CP_VCD_REG_LO_SET_POS(line, cp_vcd_reg_lo,pos) \
	FAST_GPFLAGSET(line, cp_vcd_reg_lo,pos,CP_VCD_REG_LO_POS)

#define SC_CP_VCD_REG_LO_SET_NRM(line, cp_vcd_reg_lo,nrm) \
	FAST_GPFLAGSET(line, cp_vcd_reg_lo,nrm,CP_VCD_REG_LO_NRM)

#define SC_CP_VCD_REG_LO_SET_COL0(line, cp_vcd_reg_lo,col0) \
	FAST_GPFLAGSET(line, cp_vcd_reg_lo,col0,CP_VCD_REG_LO_COL0)

#define SC_CP_VCD_REG_LO_SET_COL1(line, cp_vcd_reg_lo,col1) \
	FAST_GPFLAGSET(line, cp_vcd_reg_lo,col1,CP_VCD_REG_LO_COL1)

#define SC_CP_VCD_REG_HI_SET_TEX0(line, cp_vcd_reg_hi,tex0) \
	FAST_GPFLAGSET(line, cp_vcd_reg_hi,tex0,CP_VCD_REG_HI_TEX0)

#define SC_CP_VCD_REG_HI_SET_TEX1(line, cp_vcd_reg_hi,tex1) \
	FAST_GPFLAGSET(line, cp_vcd_reg_hi,tex1,CP_VCD_REG_HI_TEX1)

#define SC_CP_VCD_REG_HI_SET_TEX2(line, cp_vcd_reg_hi,tex2) \
	FAST_GPFLAGSET(line, cp_vcd_reg_hi,tex2,CP_VCD_REG_HI_TEX2)

#define SC_CP_VCD_REG_HI_SET_TEX3(line, cp_vcd_reg_hi,tex3) \
	FAST_GPFLAGSET(line, cp_vcd_reg_hi,tex3,CP_VCD_REG_HI_TEX3)

#define SC_CP_VCD_REG_HI_SET_TEX4(line, cp_vcd_reg_hi,tex4) \
	FAST_GPFLAGSET(line, cp_vcd_reg_hi,tex4,CP_VCD_REG_HI_TEX4)

#define SC_CP_VCD_REG_HI_SET_TEX5(line, cp_vcd_reg_hi,tex5) \
	FAST_GPFLAGSET(line, cp_vcd_reg_hi,tex5,CP_VCD_REG_HI_TEX5)

#define SC_CP_VCD_REG_HI_SET_TEX6(line, cp_vcd_reg_hi,tex6) \
	FAST_GPFLAGSET(line, cp_vcd_reg_hi,tex6,CP_VCD_REG_HI_TEX6)

#define SC_CP_VCD_REG_HI_SET_TEX7(line, cp_vcd_reg_hi,tex7) \
	FAST_GPFLAGSET(line, cp_vcd_reg_hi,tex7,CP_VCD_REG_HI_TEX7)

#define SC_CP_VAT_REG_A_SET_POSCNT(line, cp_vat_reg_a,posCnt) \
	FAST_GPFLAGSET(line, cp_vat_reg_a,posCnt,CP_VAT_REG_A_POSCNT)

#define SC_CP_VAT_REG_A_SET_POSFMT(line, cp_vat_reg_a,posFmt) \
	FAST_GPFLAGSET(line, cp_vat_reg_a,posFmt,CP_VAT_REG_A_POSFMT)

#define SC_CP_VAT_REG_A_SET_POSSHFT(line, cp_vat_reg_a,posShft) \
	FAST_GPFLAGSET(line, cp_vat_reg_a,posShft,CP_VAT_REG_A_POSSHFT)

#define SC_CP_VAT_REG_A_SET_NRMCNT(line, cp_vat_reg_a,nrmCnt) \
	FAST_GPFLAGSET(line, cp_vat_reg_a,nrmCnt,CP_VAT_REG_A_NRMCNT)

#define SC_CP_VAT_REG_A_SET_NRMFMT(line, cp_vat_reg_a,nrmFmt) \
	FAST_GPFLAGSET(line, cp_vat_reg_a,nrmFmt,CP_VAT_REG_A_NRMFMT)

#define SC_CP_VAT_REG_A_SET_COL0CNT(line, cp_vat_reg_a,Col0Cnt) \
	FAST_GPFLAGSET(line, cp_vat_reg_a,Col0Cnt,CP_VAT_REG_A_COL0CNT)

#define SC_CP_VAT_REG_A_SET_COL0FMT(line, cp_vat_reg_a,Col0Fmt) \
	FAST_GPFLAGSET(line, cp_vat_reg_a,Col0Fmt,CP_VAT_REG_A_COL0FMT)

#define SC_CP_VAT_REG_A_SET_COL1CNT(line, cp_vat_reg_a,Col1Cnt) \
	FAST_GPFLAGSET(line, cp_vat_reg_a,Col1Cnt,CP_VAT_REG_A_COL1CNT)

#define SC_CP_VAT_REG_A_SET_COL1FMT(line, cp_vat_reg_a,Col1Fmt) \
	FAST_GPFLAGSET(line, cp_vat_reg_a,Col1Fmt,CP_VAT_REG_A_COL1FMT)

#define SC_CP_VAT_REG_A_SET_TEX0CNT(line, cp_vat_reg_a,tex0Cnt) \
	FAST_GPFLAGSET(line, cp_vat_reg_a,tex0Cnt,CP_VAT_REG_A_TEX0CNT)

#define SC_CP_VAT_REG_A_SET_TEX0FMT(line, cp_vat_reg_a,tex0Fmt) \
	FAST_GPFLAGSET(line, cp_vat_reg_a,tex0Fmt,CP_VAT_REG_A_TEX0FMT)

#define SC_CP_VAT_REG_A_SET_TEX0SHFT(line, cp_vat_reg_a,tex0Shft) \
	FAST_GPFLAGSET(line, cp_vat_reg_a,tex0Shft,CP_VAT_REG_A_TEX0SHFT)

#define SC_CP_VAT_REG_A_SET_BYTEDEQUANT(line, cp_vat_reg_a,byteDequant) \
	FAST_GPFLAGSET(line, cp_vat_reg_a,byteDequant,CP_VAT_REG_A_BYTEDEQUANT)

#define SC_CP_VAT_REG_A_SET_NORMALINDEX3(line, cp_vat_reg_a,normalIndex3) \
	FAST_GPFLAGSET(line, cp_vat_reg_a,normalIndex3,CP_VAT_REG_A_NORMALINDEX3)

#define SC_CP_VAT_REG_B_SET_TEX1CNT(line, cp_vat_reg_b,tex1Cnt) \
	FAST_GPFLAGSET(line, cp_vat_reg_b,tex1Cnt,CP_VAT_REG_B_TEX1CNT)

#define SC_CP_VAT_REG_B_SET_TEX1FMT(line, cp_vat_reg_b,tex1Fmt) \
	FAST_GPFLAGSET(line, cp_vat_reg_b,tex1Fmt,CP_VAT_REG_B_TEX1FMT)

#define SC_CP_VAT_REG_B_SET_TEX1SHFT(line, cp_vat_reg_b,tex1Shft) \
	FAST_GPFLAGSET(line, cp_vat_reg_b,tex1Shft,CP_VAT_REG_B_TEX1SHFT)

#define SC_CP_VAT_REG_B_SET_TEX2CNT(line, cp_vat_reg_b,tex2Cnt) \
	FAST_GPFLAGSET(line, cp_vat_reg_b,tex2Cnt,CP_VAT_REG_B_TEX2CNT)

#define SC_CP_VAT_REG_B_SET_TEX2FMT(line, cp_vat_reg_b,tex2Fmt) \
	FAST_GPFLAGSET(line, cp_vat_reg_b,tex2Fmt,CP_VAT_REG_B_TEX2FMT)

#define SC_CP_VAT_REG_B_SET_TEX2SHFT(line, cp_vat_reg_b,tex2Shft) \
	FAST_GPFLAGSET(line, cp_vat_reg_b,tex2Shft,CP_VAT_REG_B_TEX2SHFT)

#define SC_CP_VAT_REG_B_SET_TEX3CNT(line, cp_vat_reg_b,tex3Cnt) \
	FAST_GPFLAGSET(line, cp_vat_reg_b,tex3Cnt,CP_VAT_REG_B_TEX3CNT)

#define SC_CP_VAT_REG_B_SET_TEX3FMT(line, cp_vat_reg_b,tex3Fmt) \
	FAST_GPFLAGSET(line, cp_vat_reg_b,tex3Fmt,CP_VAT_REG_B_TEX3FMT)

#define SC_CP_VAT_REG_B_SET_TEX3SHFT(line, cp_vat_reg_b,tex3Shft) \
	FAST_GPFLAGSET(line, cp_vat_reg_b,tex3Shft,CP_VAT_REG_B_TEX3SHFT)

#define SC_CP_VAT_REG_B_SET_TEX4CNT(line, cp_vat_reg_b,tex4Cnt) \
	FAST_GPFLAGSET(line, cp_vat_reg_b,tex4Cnt,CP_VAT_REG_B_TEX4CNT)

#define SC_CP_VAT_REG_B_SET_TEX4FMT(line, cp_vat_reg_b,tex4Fmt) \
	FAST_GPFLAGSET(line, cp_vat_reg_b,tex4Fmt,CP_VAT_REG_B_TEX4FMT)

#define SC_CP_VAT_REG_B_SET_VCACHE_ENHANCE(line, cp_vat_reg_b,vcache_enhance) \
	FAST_GPFLAGSET(line, cp_vat_reg_b,vcache_enhance,CP_VAT_REG_B_VCACHE_ENHANCE)

#define SC_CP_VAT_REG_C_SET_TEX4SHFT(line, cp_vat_reg_c,tex4Shft) \
	FAST_GPFLAGSET(line, cp_vat_reg_c,tex4Shft,CP_VAT_REG_C_TEX4SHFT)

#define SC_CP_VAT_REG_C_SET_TEX5CNT(line, cp_vat_reg_c,tex5Cnt) \
	FAST_GPFLAGSET(line, cp_vat_reg_c,tex5Cnt,CP_VAT_REG_C_TEX5CNT)

#define SC_CP_VAT_REG_C_SET_TEX5FMT(line, cp_vat_reg_c,tex5Fmt) \
	FAST_GPFLAGSET(line, cp_vat_reg_c,tex5Fmt,CP_VAT_REG_C_TEX5FMT)

#define SC_CP_VAT_REG_C_SET_TEX5SHFT(line, cp_vat_reg_c,tex5Shft) \
	FAST_GPFLAGSET(line, cp_vat_reg_c,tex5Shft,CP_VAT_REG_C_TEX5SHFT)

#define SC_CP_VAT_REG_C_SET_TEX6CNT(line, cp_vat_reg_c,tex6Cnt) \
	FAST_GPFLAGSET(line, cp_vat_reg_c,tex6Cnt,CP_VAT_REG_C_TEX6CNT)

#define SC_CP_VAT_REG_C_SET_TEX6FMT(line, cp_vat_reg_c,tex6Fmt) \
	FAST_GPFLAGSET(line, cp_vat_reg_c,tex6Fmt,CP_VAT_REG_C_TEX6FMT)

#define SC_CP_VAT_REG_C_SET_TEX6SHFT(line, cp_vat_reg_c,tex6Shft) \
	FAST_GPFLAGSET(line, cp_vat_reg_c,tex6Shft,CP_VAT_REG_C_TEX6SHFT)

#define SC_CP_VAT_REG_C_SET_TEX7CNT(line, cp_vat_reg_c,tex7Cnt) \
	FAST_GPFLAGSET(line, cp_vat_reg_c,tex7Cnt,CP_VAT_REG_C_TEX7CNT)

#define SC_CP_VAT_REG_C_SET_TEX7FMT(line, cp_vat_reg_c,tex7Fmt) \
	FAST_GPFLAGSET(line, cp_vat_reg_c,tex7Fmt,CP_VAT_REG_C_TEX7FMT)

#define SC_CP_VAT_REG_C_SET_TEX7SHFT(line, cp_vat_reg_c,tex7Shft) \
	FAST_GPFLAGSET(line, cp_vat_reg_c,tex7Shft,CP_VAT_REG_C_TEX7SHFT)

#define SC_CP_MATIDX_REG_A_SET_POSIDX(line, cp_matidx_reg_a,posIdx) \
	FAST_GPFLAGSET(line, cp_matidx_reg_a,posIdx,CP_MATIDX_REG_A_POSIDX)

#define SC_CP_MATIDX_REG_A_SET_TEX0IDX(line, cp_matidx_reg_a,tex0Idx) \
	FAST_GPFLAGSET(line, cp_matidx_reg_a,tex0Idx,CP_MATIDX_REG_A_TEX0IDX)

#define SC_CP_MATIDX_REG_A_SET_TEX1IDX(line, cp_matidx_reg_a,tex1Idx) \
	FAST_GPFLAGSET(line, cp_matidx_reg_a,tex1Idx,CP_MATIDX_REG_A_TEX1IDX)

#define SC_CP_MATIDX_REG_A_SET_TEX2IDX(line, cp_matidx_reg_a,tex2Idx) \
	FAST_GPFLAGSET(line, cp_matidx_reg_a,tex2Idx,CP_MATIDX_REG_A_TEX2IDX)

#define SC_CP_MATIDX_REG_A_SET_TEX3IDX(line, cp_matidx_reg_a,tex3Idx) \
	FAST_GPFLAGSET(line, cp_matidx_reg_a,tex3Idx,CP_MATIDX_REG_A_TEX3IDX)

#define SC_CP_MATIDX_REG_B_SET_TEX4IDX(line, cp_matidx_reg_b,tex4Idx) \
	FAST_GPFLAGSET(line, cp_matidx_reg_b,tex4Idx,CP_MATIDX_REG_B_TEX4IDX)

#define SC_CP_MATIDX_REG_B_SET_TEX5IDX(line, cp_matidx_reg_b,tex5Idx) \
	FAST_GPFLAGSET(line, cp_matidx_reg_b,tex5Idx,CP_MATIDX_REG_B_TEX5IDX)

#define SC_CP_MATIDX_REG_B_SET_TEX6IDX(line, cp_matidx_reg_b,tex6Idx) \
	FAST_GPFLAGSET(line, cp_matidx_reg_b,tex6Idx,CP_MATIDX_REG_B_TEX6IDX)

#define SC_CP_MATIDX_REG_B_SET_TEX7IDX(line, cp_matidx_reg_b,tex7Idx) \
	FAST_GPFLAGSET(line, cp_matidx_reg_b,tex7Idx,CP_MATIDX_REG_B_TEX7IDX)

#define SC_CP_ARRAY_BASE_REG_SET_BASE(line, cp_array_base_reg,base) \
	FAST_GPFLAGSET(line, cp_array_base_reg,base,CP_ARRAY_BASE_REG_BASE)

#define SC_CP_ARRAY_BASE_REG_SET_PAD0(line, cp_array_base_reg,pad0) \
	FAST_GPFLAGSET(line, cp_array_base_reg,pad0,CP_ARRAY_BASE_REG_PAD0)

#define SC_CP_ARRAY_STRIDE_REG_SET_STRIDE(line, cp_array_stride_reg,stride) \
	FAST_GPFLAGSET(line, cp_array_stride_reg,stride,CP_ARRAY_STRIDE_REG_STRIDE)

#define SC_CP_ARRAY_STRIDE_REG_SET_PAD0(line, cp_array_stride_reg,pad0) \
	FAST_GPFLAGSET(line, cp_array_stride_reg,pad0,CP_ARRAY_STRIDE_REG_PAD0)

#define SC_CP_STAT_ENABLE_REG_SET_VC_STAT(line, cp_stat_enable_reg,vc_stat) \
	FAST_GPFLAGSET(line, cp_stat_enable_reg,vc_stat,CP_STAT_ENABLE_REG_VC_STAT)

#define SC_CP_STAT_ENABLE_REG_SET_PAD0(line, cp_stat_enable_reg,pad0) \
	FAST_GPFLAGSET(line, cp_stat_enable_reg,pad0,CP_STAT_ENABLE_REG_PAD0)

#define SC_CP_STAT_ENABLE_REG_SET_FRCLK(line, cp_stat_enable_reg,frclk) \
	FAST_GPFLAGSET(line, cp_stat_enable_reg,frclk,CP_STAT_ENABLE_REG_FRCLK)

#define SC_CP_STAT_SEL_REG_SET_ATTR_SEL(line, cp_stat_sel_reg,attr_sel) \
	FAST_GPFLAGSET(line, cp_stat_sel_reg,attr_sel,CP_STAT_SEL_REG_ATTR_SEL)

#define SC_CP_STAT_SEL_REG_SET_STALLPERF_SEL(line, cp_stat_sel_reg,stallperf_sel) \
	FAST_GPFLAGSET(line, cp_stat_sel_reg,stallperf_sel,CP_STAT_SEL_REG_STALLPERF_SEL)

#define SC_CP_XF_LOADREGS_SET_ADDR(line, cp_xf_loadregs,addr) \
	FAST_GPFLAGSET(line, cp_xf_loadregs,addr,CP_XF_LOADREGS_ADDR)

#define SC_CP_XF_LOADREGS_SET_CNT(line, cp_xf_loadregs,cnt) \
	FAST_GPFLAGSET(line, cp_xf_loadregs,cnt,CP_XF_LOADREGS_CNT)

#define SC_CP_XF_LOADINDEX_SET_ADDR(line, cp_xf_loadindex,addr) \
	FAST_GPFLAGSET(line, cp_xf_loadindex,addr,CP_XF_LOADINDEX_ADDR)

#define SC_CP_XF_LOADINDEX_SET_CNT(line, cp_xf_loadindex,cnt) \
	FAST_GPFLAGSET(line, cp_xf_loadindex,cnt,CP_XF_LOADINDEX_CNT)

#define SC_CP_XF_LOADINDEX_SET_IDX(line, cp_xf_loadindex,idx) \
	FAST_GPFLAGSET(line, cp_xf_loadindex,idx,CP_XF_LOADINDEX_IDX)

#define SC_CP_CALLOBJ_D1_SET_PAD0(line, cp_callobj_d1,pad0) \
	FAST_GPFLAGSET(line, cp_callobj_d1,pad0,CP_CALLOBJ_D1_PAD0)

#define SC_CP_CALLOBJ_D1_SET_ADDR(line, cp_callobj_d1,addr) \
	FAST_GPFLAGSET(line, cp_callobj_d1,addr,CP_CALLOBJ_D1_ADDR)

#define SC_CP_CALLOBJ_D2_SET_PAD0(line, cp_callobj_d2,pad0) \
	FAST_GPFLAGSET(line, cp_callobj_d2,pad0,CP_CALLOBJ_D2_PAD0)

#define SC_CP_CALLOBJ_D2_SET_CNT(line, cp_callobj_d2,cnt) \
	FAST_GPFLAGSET(line, cp_callobj_d2,cnt,CP_CALLOBJ_D2_CNT)

#endif /* __FDL_CP_REG_H__ */
