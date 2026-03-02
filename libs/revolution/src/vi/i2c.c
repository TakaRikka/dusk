#include <revolution/vi.h>
#include <revolution/os.h>

#include "__vi.h"

static volatile u32 __i2c_ident_flag  = 1;
static volatile u32 __i2c_ident_first = 0;

#define busRd32(addr) (*(volatile u32 *)(addr))
#define busWrt32(addr, val) (*(volatile u32 *)(addr)) = (val)

void WaitMicroTime(s32 usec) {
    OSTime t = __OSGetSystemTime();

    while (((__OSGetSystemTime() - t) * 8) / 486 < usec);
}

static void VICheckI2C(void) {
    __i2c_ident_flag = 1;
}

static BOOL __VISetSCL(u32 value) {
    u32 reg;
    value &= 1;

    reg = busRd32(0xCD8000C0);
    reg &= ~(1 << 14);
    reg = (u32)(reg | (value << 14));
    busWrt32(0xCD8000C0, reg);
    return TRUE;
}

static BOOL __VISetSDA(u32 value) {
    u32 reg;
    value &= 1;
    reg = busRd32(0xCD8000C0);
    reg &= ~(1 << 15);
    reg = (u32)(reg | (value << 15));
    busWrt32(0xCD8000C0, reg);
    return TRUE;
}

static u32 __VIGetSDA(void) {
    u32 reg = busRd32(0xCD8000C8);
    return (u32)((reg >> 15) & 1);
}

static void __VIOpenI2C(u32 dir) {
    u32 reg = busRd32(0xCD8000C4);
    reg = (u32)(reg & ~(1 << 15));
    reg = (u32)(reg | (1 << 14) | (dir << 15));
    busWrt32(0xCD8000C4, reg);
}

static int wait4ClkHigh(void) {
    WaitMicroTime(2);
    return 1;
}

static int sendSlaveAddr(u8 slaveAddr) {
    int i;

    if (0 == __i2c_ident_flag) {
        __VISetSDA(1); 
    } else {
        __VISetSDA(0);  
    }

    WaitMicroTime(2);
    __VISetSCL(0);

    for (i = 0; i < 8; i++) {
        if (slaveAddr & 0x80) {
            if (0 == __i2c_ident_flag) {
                __VISetSDA(0);
            } else {
                __VISetSDA(1);
            }
        } else {
            if (0 == __i2c_ident_flag) {
                __VISetSDA(1); 
            } else {
                __VISetSDA(0); 
            }
        }

        WaitMicroTime(2);
        __VISetSCL(1);

        if (wait4ClkHigh() == 0) {
            return 0;
        }

        __VISetSCL(0);
        slaveAddr <<= 1;
    }

    __VIOpenI2C(0);
    WaitMicroTime(2);
    __VISetSCL(1);

    if (wait4ClkHigh() == 0) {
        return 0;
    }

    if (1 == __i2c_ident_flag) {
        if (__VIGetSDA() != 0) {
            return 0;
        }
    }
    
    if (0 == __i2c_ident_flag) {
        __VISetSDA(1);
    } else {
        __VISetSDA(0);
    }

    __VIOpenI2C(1);
    __VISetSCL(0);
    return 1;
}

int __VISendI2CData(u8 slaveAddr, u8* pData, int nBytes) {
    s32 i;
    u8 data;
    BOOL enabled;

    if (__i2c_ident_first == 0) {
        VICheckI2C();
        __i2c_ident_first = 1;
    }

    enabled = OSDisableInterrupts();
    
    __VIOpenI2C(1);
    __VISetSCL(1);

    if (0 == __i2c_ident_flag) {
        __VISetSDA(0); 
    } else {
        __VISetSDA(1);  
    }

    WaitMicroTime(2);
    WaitMicroTime(2);

    if (sendSlaveAddr(slaveAddr) == 0) {
        OSRestoreInterrupts(enabled);
        return 0;
    }

    __VIOpenI2C(1);

    while (nBytes != 0) {
        data = *pData++;
        for (i = 0; i < 8; i++) {
            if (data & 0x80) {
                if (0 == __i2c_ident_flag) {
                    __VISetSDA(0); 
                } else {
                    __VISetSDA(1);  
                }
            } else {
                if (0 == __i2c_ident_flag) {
                    __VISetSDA(1); 
                } else {
                    __VISetSDA(0);  
                }
            }

            WaitMicroTime(2);
            __VISetSCL(1);

            if (wait4ClkHigh() == 0) {
                OSRestoreInterrupts(enabled);
                return 0;
            }

            __VISetSCL(0);
            data <<= 1;
        }

        __VIOpenI2C(0);
        WaitMicroTime(2);
        __VISetSCL(1);

        if (wait4ClkHigh() == 0) {
            OSRestoreInterrupts(enabled);
            return 0;
        }

         if (1 == __i2c_ident_flag) {
            if (__VIGetSDA() != 0) {
                OSRestoreInterrupts(enabled);
                return 0;
            }
        }

        if (0 == __i2c_ident_flag) {
            __VISetSDA(1);
        } else {
            __VISetSDA(0);
        }

        __VIOpenI2C(1);
        __VISetSCL(0);
        nBytes--;
    }

    __VIOpenI2C(1);

    if (0 == __i2c_ident_flag) {
        __VISetSDA(1);
    } else {
        __VISetSDA(0);
    }

    WaitMicroTime(2);
    __VISetSCL(1);
    WaitMicroTime(2);

    if (0 == __i2c_ident_flag) {
        __VISetSDA(0);
    } else {
        __VISetSDA(1);
    }

    OSRestoreInterrupts(enabled);
    return 1;
}
