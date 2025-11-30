#ifndef _REVOLUTION_HW_REGS_H_
#define _REVOLUTION_HW_REGS_H_

#include <revolution/types.h>

#ifdef __MWERKS__
volatile u16 __VIRegs[59]     AT_ADDRESS(0xCC002000);
volatile u32 __PIRegs[12]     AT_ADDRESS(0xCC003000);
volatile u16 __MEMRegs[64]    AT_ADDRESS(0xCC004000);
volatile u16 __DSPRegs[]      AT_ADDRESS(0xCC005000);
volatile u32 __DIRegs[]       AT_ADDRESS(0xCD006000);
volatile u32 __SIRegs[0x100]  AT_ADDRESS(0xCC006400);
volatile u32 __EXIRegs[0x40]  AT_ADDRESS(0xCD006800);
volatile u32 __AIRegs[8]      AT_ADDRESS(0xCD006C00);
volatile u32 __IPCRegs[4]     AT_ADDRESS(0xCD000000);
#else
#define __VIRegs         ((volatile u16 *)0xCC002000)
#define __PIRegs         ((volatile u32 *)0xCC003000)
#define __MEMRegs        ((volatile u16 *)0xCC004000)
#define __DSPRegs        ((volatile u16 *)0xCC005000)
#define __DIRegs         ((volatile u32 *)0xCD006000)
#define __SIRegs         ((volatile u32 *)0xCC006400)
#define __EXIRegs        ((volatile u32 *)0xCD006800)
#define __AIRegs         ((volatile u32 *)0xCD006C00)
#define __IPCRegs        ((volatile u32 *)0xCD000000)
#endif

// Offsets for __VIRegs

// offsets for __VIRegs[i]
#define VI_VERT_TIMING        (0)
#define VI_DISP_CONFIG        (1)
#define VI_HORIZ_TIMING_0L    (2)
#define VI_HORIZ_TIMING_0U    (3)
#define VI_HORIZ_TIMING_1L    (4)
#define VI_HORIZ_TIMING_1U    (5)
#define VI_VERT_TIMING_ODD    (6)
#define VI_VERT_TIMING_ODD_U  (7)
#define VI_VERT_TIMING_EVEN   (8)
#define VI_VERT_TIMING_EVEN_U (9)

#define VI_BBI_ODD    (10) // burst blanking interval
#define VI_BBI_ODD_U  (11) // burst blanking interval
#define VI_BBI_EVEN   (12) // burst blanking interval
#define VI_BBI_EVEN_U (13) // burst blanking interval

#define VI_TOP_FIELD_BASE_LEFT   (14) // top in 2d, top of left pic in 3d
#define VI_TOP_FIELD_BASE_LEFT_U (15) // top in 2d, top of left pic in 3d

#define VI_TOP_FIELD_BASE_RIGHT   (16) // top of right pic in 3d
#define VI_TOP_FIELD_BASE_RIGHT_U (17) // top of right pic in 3d

#define VI_BTTM_FIELD_BASE_LEFT   (18) // bottom in 2d, bottom of left pic in 3d
#define VI_BTTM_FIELD_BASE_LEFT_U (19) // bottom in 2d, bottom of left pic in 3d

#define VI_BTTM_FIELD_BASE_RIGHT   (20) // bottom of right pic in 3d
#define VI_BTTM_FIELD_BASE_RIGHT_U (21) // bottom of right pic in 3d

#define VI_VERT_COUNT  (22) // vertical display position
#define VI_HORIZ_COUNT (23) // horizontal display position

#define VI_DISP_INT_0  (24) // display interrupt 0L
#define VI_DISP_INT_0U (25) // display interrupt 0U
#define VI_DISP_INT_1  (26) // display interrupt 1L
#define VI_DISP_INT_1U (27) // display interrupt 1U
#define VI_DISP_INT_2  (28) // display interrupt 2L
#define VI_DISP_INT_2U (29) // display interrupt 2U
#define VI_DISP_INT_3  (30) // display interrupt 3L
#define VI_DISP_INT_3U (31) // display interrupt 3U

#define VI_HSW (36) // horizontal scaling width
#define VI_HSR (37) // horizontal scaling register

#define VI_FCT_0  (38) // filter coefficient table 0L
#define VI_FCT_0U (39) // filter coefficient table 0U
#define VI_FCT_1  (40) // filter coefficient table 1L
#define VI_FCT_1U (41) // filter coefficient table 1U
#define VI_FCT_2  (42) // filter coefficient table 2L
#define VI_FCT_2U (43) // filter coefficient table 2U
#define VI_FCT_3  (44) // filter coefficient table 3L
#define VI_FCT_3U (45) // filter coefficient table 3U
#define VI_FCT_4  (46) // filter coefficient table 4L
#define VI_FCT_4U (47) // filter coefficient table 4U
#define VI_FCT_5  (48) // filter coefficient table 5L
#define VI_FCT_5U (49) // filter coefficient table 5U
#define VI_FCT_6  (50) // filter coefficient table 6L
#define VI_FCT_6U (51) // filter coefficient table 6U

#define VI_CLOCK_SEL (54) // clock select
#define VI_DTV_STAT  (55) // DTV status

#define VI_WIDTH (56)

// offsets for __DSPRegs[i]
#define DSP_MAILBOX_IN_HI  (0)
#define DSP_MAILBOX_IN_LO  (1)
#define DSP_MAILBOX_OUT_HI (2)
#define DSP_MAILBOX_OUT_LO (3)
#define DSP_CONTROL_STATUS (5)

#define DSP_ARAM_SIZE        (9)
#define DSP_ARAM_MODE        (11)
#define DSP_ARAM_REFRESH     (13)
#define DSP_ARAM_DMA_MM_HI   (16) // Main mem address
#define DSP_ARAM_DMA_MM_LO   (17)
#define DSP_ARAM_DMA_ARAM_HI (18) // ARAM address
#define DSP_ARAM_DMA_ARAM_LO (19)
#define DSP_ARAM_DMA_SIZE_HI (20) // DMA buffer size
#define DSP_ARAM_DMA_SIZE_LO (21)

#define DSP_DMA_START_HI    (24) // DMA start address
#define DSP_DMA_START_LO    (25)
#define DSP_DMA_CONTROL_LEN (27)
#define DSP_DMA_BYTES_LEFT  (29)

#define DSP_DMA_START_FLAG (0x8000) // set to start DSP

#endif
