#include <dolphin/exi.h>
#include <dolphin/os.h>
#include <dolphin/vi.h>

#include "__vi.h"

static u8 shadowGPIOOE;
static u8 shadowGPIOData;

// prototypes
static void initGpioExi(void);
static void setVideoReset(int value);
static void setI2CEnable(int value);
static int gpioOutput(u8 value);
static int gpioOE(u8 value);
static int gpioOut(u32 addr, u8 value);
static int gpioInput(u8* p);

void __VIInitI2C(void) {
    OSTime time;

    initGpioExi();
    setVideoReset(0);
    time = OSGetTime();
    while (OSGetTime() - time < OS_USEC_TO_TICKS(100)) {}
    setVideoReset(1);
    setI2CEnable(1);
}

static void initGpioExi(void) {
    shadowGPIOOE = 0;
    shadowGPIOData = 0;
    gpioOutput(shadowGPIOData);
    gpioOE(shadowGPIOOE);
}

void __VISetSCL(int value) {
    shadowGPIOOE &= ~2;
    if (value == 0) {
        shadowGPIOOE |= 2;
    }
    gpioOE(shadowGPIOOE);
}

int __VIGetSCL(void) {
    u8 value;

    gpioInput(&value);
    if (value & 2) {
        return 1;
    } else {
        return 0;
    }
}

void __VISetSDA(int value) {
    shadowGPIOOE &= ~1;
    if (value == 0) {
        shadowGPIOOE |= 1;
    }
    gpioOE(shadowGPIOOE);
}

int __VIGetSDA(void) {
    u8 value;

    gpioInput(&value);
    if (value & 1) {
        return 1;
    } else {
        return 0;
    }
}

static void setVideoReset(int value) {
    if (value != 0) {
        shadowGPIOData |= 4;
    } else {
        shadowGPIOData &= ~4;
    }
    shadowGPIOOE |= 4;
    gpioOutput(shadowGPIOData);
    gpioOE(shadowGPIOOE);
}

static void setI2CEnable(int value) {
    if (value != 0) {
        shadowGPIOData &= ~0x10;
    } else {
        shadowGPIOData |= 0x10;
    }
    shadowGPIOOE |= 0x10;
    gpioOutput(shadowGPIOData);
    gpioOE(shadowGPIOOE);
}

static int gpioOutput(u8 value) {
    return gpioOut(0x800404U, value);
}

static int gpioOE(u8 value) {
    return gpioOut(0x800408U, value);
}

static int gpioOut(u32 addr, u8 value) {
    u32 cmd;

    cmd = (addr | 0x02000000) << 6;
    if (EXILock(0, 1, 0) == 0) {
        return 0;
    }
    if (EXISelect(0, 1, 4) == 0) {
        EXIUnlock(0);
        return 0;
    }

    EXIImm(0, &cmd, 4, EXI_WRITE, 0);
    EXISync(0);
    cmd = value << 24;
    EXIImm(0, &cmd, 1, EXI_WRITE, 0);
    EXISync(0);
    EXIDeselect(0);
    EXIUnlock(0);
    return 1;
}

static int gpioInput(u8* p) {
    u32 cmd;

    if (EXILock(0, 1, 0) == 0) {
        return 0;
    }
    if (EXISelect(0, 1, 4) == 0) {
        EXIUnlock(0);
        return 0;
    }
    cmd = 0x20010100;
    EXIImm(0, &cmd, 4, EXI_WRITE, 0);
    EXISync(0);
    EXIImm(0, &cmd, 1, EXI_READ, 0);
    EXISync(0);
    EXIDeselect(0);
    EXIUnlock(0);
    *p = cmd >> 24;
    return 1;
}
