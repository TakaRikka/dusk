#ifndef GXREGS_H
#define GXREGS_H

#include <revolution/gx.h>

#ifdef __cplusplus
extern "C" {
#endif

#include <revolution/private/bp_reg.h>
#include <revolution/private/cp_reg.h>
#include <revolution/private/gen_reg.h>
#include <revolution/private/pe_reg.h>
#include <revolution/private/pi_reg.h>
#include <revolution/private/ras_reg.h>
#include <revolution/private/su_reg.h>
#include <revolution/private/tev_reg.h>
#include <revolution/private/tx_reg.h>
#include <revolution/private/xf_mem.h>

extern volatile void* __piReg;
extern volatile void* __cpReg;
extern volatile void* __peReg;
extern volatile void* __memReg;

#define MEM_PE_REQCOUNTH_IDX 0x27
#define MEM_PE_REQCOUNTL_IDX 0x28

/* GX fifo write helpers */

#define GX_WRITE_U8(ub)     \
    GXWGFifo.u8 = (u8)(ub)

#define GX_WRITE_U16(us)   \
   GXWGFifo.u16 = (u16)(us)

#define GX_WRITE_U32(ui)   \
   GXWGFifo.u32 = (u32)(ui)

#define GX_WRITE_F32(f)     \
   GXWGFifo.f32 = (f32)(f);


#define GX_PI_REG_WRITE_U32(a, d) *(vu32*)((vu8*)__piReg + (a)) = (u32)(d)

#define GX_PI_REG_READ_U32(a) *(vu32*)((vu8*)__piReg + (a))

#define GX_CP_REG_WRITE_U16(a, d) *(vu16*)((vu16*)__cpReg + (a)) = (u16)(d)

#define GX_CP_REG_READ_U16(a) *(vu16*)((vu16*)__cpReg + (a))

#define GX_CP_REG_WRITE_U32(a, d) *(vu32*)((vu16*)__cpReg + (a)) = (u32)(d)

#define GX_CP_REG_READ_U32(a) *(vu32*)((vu16*)__cpReg + (a))

#define GX_MEM_REG_WRITE_U16(a, d) *(vu16*)((vu16*)__memReg + (a)) = (u16)(d)

#define GX_MEM_REG_READ_U16(a) *(vu16*)((vu16*)__memReg + (a))

#define GX_PE_REG_WRITE_U16(a, d) *(vu16*)((vu16*)__peReg + (a)) = (u16)(d)

#define GX_PE_REG_READ_U16(a) *(vu16*)((vu16*)__peReg + (a))

#define GX_CP_IDLE_REG_READ_U16(a) GX_CP_REG_READ_U16(a)

#define GX_PI_REG_WRITE_U32(a, d) *(vu32*)((vu8*)__piReg + (a)) = (u32)(d)

#define GX_PI_REG_READ_U32(a) *(vu32*)((vu8*)__piReg + (a))

#define GX_CP_REG_WRITE_U16(a, d) *(vu16*)((vu16*)__cpReg + (a)) = (u16)(d)

#define GX_CP_REG_READ_U16(a) *(vu16*)((vu16*)__cpReg + (a))

#define GX_CP_REG_WRITE_U32(a, d) *(vu32*)((vu16*)__cpReg + (a)) = (u32)(d)

#define GX_CP_REG_READ_U32(a) *(vu32*)((vu16*)__cpReg + (a))

#define GX_MEM_REG_WRITE_U16(a, d) *(vu16*)((vu16*)__memReg + (a)) = (u16)(d)

#define GX_MEM_REG_READ_U16(a) *(vu16*)((vu16*)__memReg + (a))

#define GX_PE_REG_WRITE_U16(a, d) *(vu16*)((vu16*)__peReg + (a)) = (u16)(d)

#define GX_PE_REG_READ_U16(a) *(vu16*)((vu16*)__peReg + (a))

#define GX_CP_IDLE_REG_READ_U16(a) GX_CP_REG_READ_U16(a)

#define GX_WRITE_CP_STRM_REG(addr, vtxfmt, data)                                                                                                     \
    {                                                                                                                                                \
        GX_WRITE_U8(CP_OPCODE(0, 1));                                                                                                                \
        GX_WRITE_U8(CP_STREAM_REG((vtxfmt), (addr)));                                                                                                \
        GX_WRITE_U32((data));                                                                                                                        \
    }

// needed to match some places
#define GX_WRITE_CP_STRM_REG_alt(addr, vtxfmt, data, rAddr)                                                                                                     \
    {                                                                                                                                                \
        s32 regAddr; \
        GX_WRITE_U8(CP_OPCODE(0, 1));                                                                                                                \
        GX_WRITE_U8(CP_STREAM_REG((vtxfmt), (addr)));                                                                                                \
        GX_WRITE_U32((data));                                                                                                                        \
        regAddr = rAddr; \
    }

// needed to match some places
#if DEBUG
#define GX_WRITE_CP_STRM_REG_alt2(addr, vtxfmt, data, rAddr)                                                                                                     \
    {                                                                                                                                                \
        s32 regAddr; \
        GX_WRITE_U8(CP_OPCODE(0, 1));                                                                                                                \
        GX_WRITE_U8(CP_STREAM_REG((vtxfmt), (addr)));                                                                                                \
        GX_WRITE_U32((data));                                                                                                                        \
        regAddr = rAddr; \
        if (regAddr >= 0 && regAddr < 4) { \
            __GXData->indexBase[regAddr] = data; \
        } \
    }
#else
#define GX_WRITE_CP_STRM_REG_alt2(addr, vtxfmt, data, rAddr) GX_WRITE_CP_STRM_REG(addr, vtxfmt, data)
#endif

// needed to match some places
#if DEBUG
#define GX_WRITE_CP_STRM_REG_alt3(addr, vtxfmt, data, rAddr)                                                                                                     \
    {                                                                                                                                                \
        s32 regAddr; \
        GX_WRITE_U8(CP_OPCODE(0, 1));                                                                                                                \
        GX_WRITE_U8(CP_STREAM_REG((vtxfmt), (addr)));                                                                                                \
        GX_WRITE_U32((data));                                                                                                                        \
        regAddr = rAddr; \
        if (regAddr >= 0 && regAddr < 4) { \
            __GXData->indexStride[regAddr] = data; \
        } \
    }
#else
#define GX_WRITE_CP_STRM_REG_alt3(addr, vtxfmt, data, rAddr) GX_WRITE_CP_STRM_REG(addr, vtxfmt, data)
#endif

#define GX_WRITE_XF_REG(addr, data, cnt)                                                                                                             \
    {                                                                                                                                                \
        GX_WRITE_U8(CP_OPCODE(0, 0x2));                                                                                                              \
        GX_WRITE_U32(CP_XF_LOADREGS((addr), (cnt)));                                                                                                 \
        GX_WRITE_U32((data));                                                                                                                        \
        VERIF_XF_REG(addr, data);                                                                                                                   \
    }

#define GX_WRITE_XF_MEM_U32(addr, data) GX_WRITE_U32(data)
#define GX_WRITE_XF_MEM_F32(addr, data) GX_WRITE_F32(data)

#define GX_WRITE_RA_REG(reg)                                                                                                                         \
    {                                                                                                                                                \
        GX_WRITE_U8(CP_OPCODE(1, 0xC));                                                                                                              \
        GX_WRITE_U32((reg));                                                                                                                         \
        VERIF_RAS_REG(reg); \
    }

#ifdef __MWERKS__
#define GX_DEFINE_GX_READ_COUNTER(unit)                                                                                                              \
    inline u32 __GXRead##unit##CounterU32(u32 regAddrL, u32 regAddrH) {                                                                              \
        u32 ctrH0, ctrH1, ctrL;                                                                                                                      \
        ctrH0 = GX_##unit##_REG_READ_U16(regAddrH);                                                                                                  \
        do {                                                                                                                                         \
            ctrH1 = ctrH0;                                                                                                                           \
            ctrL = GX_##unit##_REG_READ_U16(regAddrL);                                                                                               \
            ctrH0 = GX_##unit##_REG_READ_U16(regAddrH);                                                                                              \
        } while (ctrH0 != ctrH1);                                                                                                                    \
        return ((ctrH0 << 16) | ctrL);                                                                                                               \
    }
#else
#define GX_DEFINE_GX_READ_COUNTER(unit)
#endif

#ifdef __MWERKS__
#define FAST_FLAG_SET(regOrg, newFlag, shift, size)                                                                                                  \
    do {                                                                                                                                             \
        (regOrg) = (u32)__rlwimi((int)(regOrg), (int)(newFlag), (shift), (32 - (shift) - (size)), (31 - (shift)));                                   \
    } while (0);
#else
#define FAST_FLAG_SET(regOrg, newFlag, shift, size)
#endif

#ifdef __cplusplus
}
#endif

#endif  // GXREGS_H
