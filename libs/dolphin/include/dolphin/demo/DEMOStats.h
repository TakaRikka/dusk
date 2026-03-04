#ifndef _DOLPHIN_DEMOSTATS_H_
#define _DOLPHIN_DEMOSTATS_H_

#include <dolphin/gx.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum DEMO_STAT_TYPE {
    DEMO_STAT_GP0 = 0,
    DEMO_STAT_GP1 = 1,
    DEMO_STAT_MEM = 2,
    DEMO_STAT_PIX = 3,
    DEMO_STAT_VC = 4,
    DEMO_STAT_FR = 5,
    DEMO_STAT_TBW = 6,
    DEMO_STAT_TBP = 7,
    DEMO_STAT_MYC = 8,
    DEMO_STAT_MYR = 9,
} DEMO_STAT_TYPE;

typedef struct DemoStatData {
    char text[50];
    DEMO_STAT_TYPE stat_type;
    u32 stat;
    u32 count;
} DemoStatData;

typedef enum {
    DEMO_STAT_TL = 0,
    DEMO_STAT_BL = 1,
    DEMO_STAT_TLD = 2,
    DEMO_STAT_BLD = 3,
    DEMO_STAT_IO = 4,
} DEMO_STAT_DISP;

extern u8 DemoStatEnable;

void DEMOSetStats(DemoStatData* stat, u32 nstats, DEMO_STAT_DISP disp);
void DEMOUpdateStats(u8 inc);
void DEMOPrintStats(void);

#ifdef __cplusplus
}
#endif

#endif // _DOLPHIN_DEMOSTATS_H_
