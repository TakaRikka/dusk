#include <revolution/wpad.h>
#include <revolution/wpad/__wpad.h>
#include <cstring>

extern void DEBUGPrint(const char*, ...);

void __a1_20_status_report(u8 chan, u8* data);
void __a1_21_user_data(u8 chan, u8* data);
void __a1_22_ack(u8 chan, u8* data);
void __a1_30_data_type(u8 chan, u8* data);
void __a1_31_data_type(u8 chan, u8* data);
void __a1_32_data_type(u8 chan, u8* data);
void __a1_33_data_type(u8 chan, u8* data);
void __a1_34_data_type(u8 chan, u8* data);
void __a1_35_data_type(u8 chan, u8* data);
void __a1_36_data_type(u8 chan, u8* data);
void __a1_37_data_type(u8 chan, u8* data);
void __a1_3d_data_type(u8 chan, u8* data);
void __a1_3e_data_type(u8 chan, u8* data);
void __a1_3f_data_type(u8 chan, u8* data);
void __a1_unused_report(u8 chan, u8* data);

#define HID_WPAD_BUTTON_MASK (u16)(0x9F1F)
#define HID_WPAD_BUTTON_CLEAR (u16)(0x60E0)
#define HID_NEAREMPTY_MASK (u8)(0x80)
#define HID_DPD_MASK (u8)(0x8)
#define HID_SPEAKER_MASK (u8)(0x4)
#define HID_ATTACH_MASK (u8)(0x2)
#define HID_LOWBAT_MASK (u8)(0x1)
#define HID_LED_MASK (u8)(0xF0)
#define HID_FW_MASK (u8)(0xF0)
#define HID_PRT_MASK (u8)(0xF)

#define HID_IDX_BYTE7 1
#define HID_IDX_BYTE8 2
#define HID_IDX_BYTE9 3
#define HID_IDX_BYTE10 4
#define HID_IDX_BYTE11 5
#define HID_IDX_BYTE12 6
#define HID_IDX_BYTE13 7
#define HID_IDX_BYTE14 8
#define HID_IDX_BYTE15 9
#define HID_IDX_BYTE16 10
#define HID_IDX_BYTE17 11
#define HID_IDX_BYTE18 12
#define HID_IDX_BYTE19 13
#define HID_IDX_BYTE20 14
#define HID_IDX_BYTE21 15
#define HID_IDX_BYTE22 16
#define HID_IDX_BYTE23 17
#define HID_IDX_BYTE24 18
#define HID_IDX_BYTE25 19
#define HID_IDX_BYTE26 20
#define HID_IDX_BYTE27 21

void (*__a1_input_reports_array[])(u8 chan, u8* data) = {
    __a1_20_status_report,  __a1_21_user_data,  __a1_22_ack,        __a1_unused_report,
    __a1_unused_report,     __a1_unused_report, __a1_unused_report, __a1_unused_report,
    __a1_unused_report,     __a1_unused_report, __a1_unused_report, __a1_unused_report,
    __a1_unused_report,     __a1_unused_report, __a1_unused_report, __a1_unused_report,
    __a1_30_data_type,      __a1_31_data_type,  __a1_32_data_type,  __a1_33_data_type,
    __a1_34_data_type,      __a1_35_data_type,  __a1_36_data_type,  __a1_37_data_type,
    __a1_unused_report,     __a1_unused_report, __a1_unused_report, __a1_unused_report,
    __a1_unused_report,     __a1_3d_data_type,  __a1_3e_data_type,  __a1_3f_data_type
};

static u8 checkInvalidData[21] = {
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff,
};
static u8 checkBuffer[21];

WPADExtensionCallback initExtension(s32 chan, WPADControlBlock*);

void abortInitExtension(s32 chan, s32 result) {
    WPADControlBlock* p_wpd = _wpdcb[chan];
    u32 type;

    if (result != WPAD_ERR_OK) {
        WPADiClearQueue(&p_wpd->extCmdQueue);

        if (result == WPAD_ERR_NO_CONTROLLER) {
            type = WPAD_DEV_NONE;
        } else if (p_wpd->info.attach) {
            type = WPAD_DEV_NOT_SUPPORTED;
        } else {
            type = WPAD_DEV_CORE;
        }

        p_wpd->devType = (u8)type;
        if (p_wpd->extensionCallback) {
            p_wpd->extensionCallback(chan, (s32)type);
        }
    }
}

#define PI 3.141592f

// Copied from Petari - this is likely close to the correct implementation,
// but loop unrolling is not happening so it ends up completely mismatching.
// Not sure if it's supposed to be manually unrolled in code? But then the
// bit-twiddling instructions end up mismatching.
static inline void __parse_dpd_data(s32 chan, WPADStatus** p_status, u8 fmt, u8* p_data, u8 len) {
    u8 i;
    u8 x;
    u8 y;
    u8 xy;

    if (fmt == WPAD_DPD_STANDARD) {
        for (i = 0; i < WPAD_DPD_MAX_OBJECTS; ++i) {
            if (i * 3 + 2 < len) {
                x = *(p_data + i * 3);
                y = *(p_data + i * 3 + 1);
                xy = *(p_data + i * 3 + 2);

                (*p_status)->obj[i].x = (s16)((s16)((s16)(((u16)x) & 0xFF) | (u16)(((u16)xy & 0x30) << 4)));
                (*p_status)->obj[i].y = (s16)(WPAD_DPD_IMG_RESO_WY - 1 - (s16)((s16)(((u16)y) & 0xFF) | (u16)(((u16)xy & 0xC0) << 2)));
                (*p_status)->obj[i].size = (u16)(((xy & 0xF) & 0xFF));
                (*p_status)->obj[i].size = (u8)((f32)((*p_status)->obj[i].size) * (f32)((*p_status)->obj[i].size) * PI);
                if ((*p_status)->obj[i].size == 0 || (*p_status)->obj[i].x == (s16)(WPAD_DPD_IMG_RESO_WX - 1) ||
                    (*p_status)->obj[i].y == (s16)(WPAD_DPD_IMG_RESO_WY - 1)) {
                    (*p_status)->obj[i].x = (s16)0;
                    (*p_status)->obj[i].y = (s16)(WPAD_DPD_IMG_RESO_WY - 1);
                    (*p_status)->obj[i].size = 0;
                }
                (*p_status)->obj[i].traceId = i;
            } else {
                (*p_status)->obj[i].x = (s16)0;
                (*p_status)->obj[i].y = (s16)(WPAD_DPD_IMG_RESO_WY - 1);
                (*p_status)->obj[i].size = 0;
                (*p_status)->obj[i].traceId = i;
            }
        }
    } else if (fmt == WPAD_DPD_BASIC) {
        for (i = 0; i < WPAD_DPD_MAX_OBJECTS; ++i) {
            x = *(p_data + i * 3 - (i / 2));
            y = *(p_data + i * 3 - (i / 2) + 1);
            xy = *(p_data + (i / 2) * 5 + 2);

            if ((i % 2) == 0) {
                (*p_status)->obj[i].x = (s16)((s16)((s16)(((u16)x) & 0xFF) | (u16)(((u16)(xy & 0x30)) << 4)));
                (*p_status)->obj[i].y = (s16)(WPAD_DPD_IMG_RESO_WY - 1 - (s16)((s16)(((u16)y) & 0xFF) | (u16)(((u16)(xy & 0xC0)) << 2)));
            } else {
                (*p_status)->obj[i].x = (s16)((s16)((s16)(((u16)x) & 0xFF) | (u16)(((u16)(xy & (u8)(0x30 >> 4))) << 8)));
                (*p_status)->obj[i].y = (s16)(WPAD_DPD_IMG_RESO_WY - 1 - (s16)((s16)(((u16)y) & 0xFF) | (u16)(((u16)(xy & (u8)(0xC0 >> 4))) << 6)));
            }
            if ((*p_status)->obj[i].x == (s16)(WPAD_DPD_IMG_RESO_WX - 1) || (*p_status)->obj[i].y == (s16)(WPAD_DPD_IMG_RESO_WY - 1)) {
                (*p_status)->obj[i].x = (s16)0;
                (*p_status)->obj[i].y = (s16)(WPAD_DPD_IMG_RESO_WY - 1);
                (*p_status)->obj[i].size = 0;
            } else {
                (*p_status)->obj[i].size = 12;
            }
            (*p_status)->obj[i].traceId = i;
        }
    }
}

void getDevConfig(s32 chan, s32 result, WPADStatus* p_status, u8* data) {
    WPADControlBlock* p_wpd = _wpdcb[chan];
    u8* p_buf = p_wpd->wmReadDataPtr;
    u8 checkSum;
    u8 i;
    int j;
    int index;
    DPDObject obj[WPAD_DPD_MAX_OBJECTS];
    const s16 dummyObjX[] = {0x7F, 0x7F, 0x380, 0x380};
    const s16 dummyObjY[] = {0x5D, 0x2A2, 0x2A2, 0x5D};

    for (i = 0; i < WPAD_DPD_MAX_OBJECTS; i++) {
        p_wpd->devConf.obj[i].x = dummyObjX[i];
        p_wpd->devConf.obj[i].y = dummyObjY[i];
        p_wpd->devConf.obj[i].size = (u16)p_wpd->dpdDummyObjSize;
        p_wpd->devConf.obj[i].traceId = i;
    }

    p_wpd->devConf.acc_0g.x = 530;
    p_wpd->devConf.acc_0g.y = 530;
    p_wpd->devConf.acc_0g.z = 530;
    p_wpd->devConf.acc_1g.x = 636;
    p_wpd->devConf.acc_1g.y = 636;
    p_wpd->devConf.acc_1g.z = 636;
    p_wpd->devConf.volume = 0;
    p_wpd->devConf.motor = 0;

    if (result != WPAD_ERR_OK) {
        return;
    }

    if (!p_wpd->oldFw) {
        for (i = 0; i < 2; i++) {
            checkSum = 0;
            index = i * (11);
            for (j = index; j < index + 10; j++) {
                checkSum += p_buf[j];
            }
            checkSum = (u8)(checkSum + 0x55);

            if (checkSum == p_buf[index + 10]) {
                DEBUGPrint("Dpd Setting is ok.\n");

                __parse_dpd_data(chan, &p_status, WPAD_DPD_BASIC, p_buf, index + 11);

                obj[0].x = (s16)((s16)((u16)p_buf[index] & 0xFF) |
                                 (u16)((u16)(p_buf[index + 2] & 0x30) << 4));
                obj[0].y =
                    (s16)(WPAD_DPD_IMG_RESO_WY - 1 - (s16)((s16)(((u16)(p_buf[index + 1])) & 0xFF) | (u16)(((u16)(p_buf[index + 2] & 0xC0)) << 2)));
                obj[0].size = p_wpd->dpdDummyObjSize;
                obj[0].traceId = 0;

                obj[1].x = (s16)((s16)((u16)p_buf[index + 3] & 0xFF) |
                                 (u16)((u16)(p_buf[index + 2] & (u8)(0x30 >> 4)) << 8));
                obj[1].y = (s16)(WPAD_DPD_IMG_RESO_WY - 1 -
                                 (s16)((s16)(((u16)(p_buf[index + 4])) & 0xFF) | (u16)(((u16)(p_buf[index + 2] & (u8)(0xC0 >> 4))) << 6)));
                obj[1].size = p_wpd->dpdDummyObjSize;
                obj[1].traceId = 1;

                obj[2].x = (s16)((s16)((u16)p_buf[index + 5] & 0xFF) |
                                 (u16)((u16)(p_buf[index + 7] & 0x30) << 4));
                obj[2].y =
                    (s16)(WPAD_DPD_IMG_RESO_WY - 1 - (s16)((s16)(((u16)(p_buf[index + 6])) & 0xFF) | (u16)(((u16)(p_buf[index + 7] & 0xC0)) << 2)));
                obj[2].size = p_wpd->dpdDummyObjSize;
                obj[2].traceId = 2;

                obj[3].x = (s16)((s16)((u16)p_buf[index + 8] & 0xFF) |
                                 (u16)((u16)(p_buf[index + 7] & (u8)(0x30 >> 4)) << 8));
                obj[3].y = (s16)(WPAD_DPD_IMG_RESO_WY - 1 -
                                 (s16)((s16)(((u16)(p_buf[index + 9])) & 0xFF) | (u16)(((u16)(p_buf[index + 7] & (u8)(0xC0 >> 4))) << 6)));
                obj[3].size = p_wpd->dpdDummyObjSize;
                obj[3].traceId = 3;

                break;
            } else {
                DEBUGPrint("Dpd Setting is broken.\n");
            }
        }
    } else {
        DEBUGPrint("Old Firmware uses default values because it has no Dpd Setting.\n");
    }

    for (i = 0; i < 2; i++) {
        checkSum = 0;

        index = i * (10);
        if (!p_wpd->oldFw) {
            index += 22;
        }

        for (j = index; j < index + 9; j++) {
            checkSum += p_buf[j];
        }
        checkSum = (u8)(checkSum + 0x55);
        if (checkSum == p_buf[index + 9]) {
            DEBUGPrint("Acc is ok.\n");
            p_wpd->devConf.acc_0g.x = (s16)((u16)(((u16)(p_buf[index + 0]) << 2) & 0xFFFC) | (u16)(((u16)(p_buf[index + 3]) >> 4) & 3));
            p_wpd->devConf.acc_0g.y = (s16)((u16)(((u16)(p_buf[index + 1]) << 2) & 0xFFFC) | (u16)(((u16)(p_buf[index + 3]) >> 2) & 3));
            p_wpd->devConf.acc_0g.z = (s16)((u16)(((u16)(p_buf[index + 2]) << 2) & 0xFFFC) | (u16)(((u16)(p_buf[index + 3])) & 3));
            p_wpd->devConf.acc_1g.x = (s16)((u16)(((u16)(p_buf[index + 4]) << 2) & 0xFFFC) | (u16)(((u16)(p_buf[index + 7]) >> 4) & 3));
            p_wpd->devConf.acc_1g.y = (s16)((u16)(((u16)(p_buf[index + 5]) << 2) & 0xFFFC) | (u16)(((u16)(p_buf[index + 7]) >> 2) & 3));
            p_wpd->devConf.acc_1g.z = (s16)((u16)(((u16)(p_buf[index + 6]) << 2) & 0xFFFC) | (u16)(((u16)(p_buf[index + 7])) & 3));
            p_wpd->devConf.volume = (u8)(p_buf[index + 8] & (u8)0x7F);
            p_wpd->devConf.motor = (u8)(p_buf[index + 8] & (u8)0x80);
            break;
        } else {
            DEBUGPrint("Acc is broken.\n");
        }
    }
    DEBUGPrint("0G:  accX = %d,  accY = %d,  accZ = %d\n", p_wpd->devConf.acc_0g.x, p_wpd->devConf.acc_0g.y, p_wpd->devConf.acc_0g.z);
    DEBUGPrint("1G:  accX = %d,  accY = %d,  accZ = %d\n", p_wpd->devConf.acc_1g.x, p_wpd->devConf.acc_1g.y, p_wpd->devConf.acc_1g.z);
    DEBUGPrint("Volume: %d,   Motor: %d\n", p_wpd->devConf.volume, p_wpd->devConf.motor);
}

void getExtConfig(s32 chan, s32 result) {
    WPADControlBlock* p_wpd = _wpdcb[chan];
    u8* p_buf = p_wpd->wmReadDataPtr;
    int i, j, k;
    int index = -1;
    int checksum;

    if (result != WPAD_ERR_OK) {
        p_wpd->devType = WPAD_DEV_NOT_SUPPORTED;
    } else if (p_wpd->devType == WPAD_DEV_FS || p_wpd->devType == WPAD_DEV_CLASSIC) {
        WPADiDecode(chan, p_buf, 0x20, 0x20);

        for (i = 0; i < 2; i++) {
            checksum = 0;
            k = i * 16;
            for (j = k; j < k + 14; j++) {
                checksum += p_buf[j];
            }

            if (((u8)(checksum + 0x55) == p_buf[k + 14]) && ((u8)(checksum + 0xaa) == p_buf[k + 15])) {
                index = k;
            }
        }

        switch (p_wpd->devType) {
        case WPAD_DEV_FS:
            if (index < 0) {
                DEBUGPrint("check sum error.\n");
                p_wpd->extConf.fs.acc_0g.x = 512;
                p_wpd->extConf.fs.acc_0g.y = 512;
                p_wpd->extConf.fs.acc_0g.z = 512;
                p_wpd->extConf.fs.acc_1g.x = 716;
                p_wpd->extConf.fs.acc_1g.y = 716;
                p_wpd->extConf.fs.acc_1g.z = 716;
            } else {
                p_wpd->extConf.fs.acc_0g.x = (s16)((u16)(((u16)(p_buf[index + 0]) << 2) & 0xFFFC) | (u16)(((u16)(p_buf[index + 3]) >> 4) & 3));
                p_wpd->extConf.fs.acc_0g.y = (s16)((u16)(((u16)(p_buf[index + 1]) << 2) & 0xFFFC) | (u16)(((u16)(p_buf[index + 3]) >> 2) & 3));
                p_wpd->extConf.fs.acc_0g.z = (s16)((u16)(((u16)(p_buf[index + 2]) << 2) & 0xFFFC) | (u16)(((u16)(p_buf[index + 3])) & 3));
                p_wpd->extConf.fs.acc_1g.x = (s16)((u16)(((u16)(p_buf[index + 4]) << 2) & 0xFFFC) | (u16)(((u16)(p_buf[index + 7]) >> 4) & 3));
                p_wpd->extConf.fs.acc_1g.y = (s16)((u16)(((u16)(p_buf[index + 5]) << 2) & 0xFFFC) | (u16)(((u16)(p_buf[index + 7]) >> 2) & 3));
                p_wpd->extConf.fs.acc_1g.z = (s16)((u16)(((u16)(p_buf[index + 6]) << 2) & 0xFFFC) | (u16)(((u16)(p_buf[index + 7])) & 3));
                p_wpd->extConf.fs.stick.x_max = (s8)p_buf[index + 8];
                p_wpd->extConf.fs.stick.x_min = (s8)p_buf[index + 9];
                p_wpd->extConf.fs.stick.x = (s8)p_buf[index + 10];
                p_wpd->extConf.fs.stick.y_max = (s8)p_buf[index + 11];
                p_wpd->extConf.fs.stick.y_min = (s8)p_buf[index + 12];
                p_wpd->extConf.fs.stick.y = (s8)p_buf[index + 13];

                DEBUGPrint("0G:  fsaccX = %d,  fsaccY = %d,  fsaccZ = %d\n", p_wpd->extConf.fs.acc_0g.x, p_wpd->extConf.fs.acc_0g.y,
                           p_wpd->extConf.fs.acc_0g.z);
                DEBUGPrint("1G:  fsaccX = %d,  fsaccY = %d,  fsaccZ = %d\n", p_wpd->extConf.fs.acc_1g.x, p_wpd->extConf.fs.acc_1g.y,
                           p_wpd->extConf.fs.acc_1g.z);
                DEBUGPrint("FS:  X = %d,  X max = %d,  X min = %d\n", p_wpd->extConf.fs.stick.x, p_wpd->extConf.fs.stick.x_max,
                           p_wpd->extConf.fs.stick.x_min);
                DEBUGPrint("FS:  Y = %d,  Y max = %d,  Y min = %d\n", p_wpd->extConf.fs.stick.y, p_wpd->extConf.fs.stick.y_max,
                           p_wpd->extConf.fs.stick.y_min);
            }
            break;

        case WPAD_DEV_CLASSIC:
            p_wpd->extConf.cl.lstk.x_max = (s8)p_buf[index + 0];
            p_wpd->extConf.cl.lstk.x_min = (s8)p_buf[index + 1];
            p_wpd->extConf.cl.lstk.x = (s8)p_buf[index + 2];
            p_wpd->extConf.cl.lstk.y_max = (s8)p_buf[index + 3];
            p_wpd->extConf.cl.lstk.y_min = (s8)p_buf[index + 4];
            p_wpd->extConf.cl.lstk.y = (s8)p_buf[index + 5];
            p_wpd->extConf.cl.rstk.x_max = (s8)p_buf[index + 6];
            p_wpd->extConf.cl.rstk.x_min = (s8)p_buf[index + 7];
            p_wpd->extConf.cl.rstk.x = (s8)p_buf[index + 8];
            p_wpd->extConf.cl.rstk.y_max = (s8)p_buf[index + 9];
            p_wpd->extConf.cl.rstk.y_min = (s8)p_buf[index + 10];
            p_wpd->extConf.cl.rstk.y = (s8)p_buf[index + 11];
            p_wpd->extConf.cl.triggerL = p_buf[index + 12];
            p_wpd->extConf.cl.triggerR = p_buf[index + 13];

            DEBUGPrint("CL:  X = %d,  X max = %d,  X min = %d\n", p_wpd->extConf.cl.lstk.x, p_wpd->extConf.cl.lstk.x_max,
                       p_wpd->extConf.cl.lstk.x_min);
            DEBUGPrint("CL:  Y = %d,  Y max = %d,  Y min = %d\n", p_wpd->extConf.cl.lstk.y, p_wpd->extConf.cl.lstk.y_max,
                       p_wpd->extConf.cl.lstk.y_min);
            DEBUGPrint("CR:  X = %d,  X max = %d,  X min = %d\n", p_wpd->extConf.cl.rstk.x, p_wpd->extConf.cl.rstk.x_max,
                       p_wpd->extConf.cl.rstk.x_min);
            DEBUGPrint("CR:  Y = %d,  Y max = %d,  Y min = %d\n", p_wpd->extConf.cl.rstk.y, p_wpd->extConf.cl.rstk.y_max,
                       p_wpd->extConf.cl.rstk.y_min);
            DEBUGPrint("LR:  L = %d,  R = %d\n", p_wpd->extConf.cl.triggerL, p_wpd->extConf.cl.triggerR);
            break;
        }
        p_wpd->calibrated = 0;
    }

    if (p_wpd->cmdBlkCallback) {
        p_wpd->cmdBlkCallback(chan, p_wpd->devType);
        p_wpd->cmdBlkCallback = NULL;
    }
}

static BOOL isExtEncMain(u8 type) {
    if (type == WPAD_DEV_FS || type == WPAD_DEV_CLASSIC) {
        return TRUE;
    } else {
        return FALSE;
    }
}

extern BOOL __OSInIPL;

void getExtType(s32 chan, s32 result) {
    WPADControlBlock* p_wpd = _wpdcb[chan];
    WPADCmdQueue* cmdq = &p_wpd->extCmdQueue;
    u8* p_buf = (u8*)p_wpd->wmReadDataPtr;

    if (result == WPAD_ERR_OK) {
        WPADiDecode(chan, p_buf, 2, 0xfe);

        p_wpd->devMode = p_buf[0];

        switch (p_buf[1]) {
        case 0x00:
            p_wpd->devType = WPAD_DEV_FS;
            break;
        case 0x01:
            p_wpd->devType = WPAD_DEV_CLASSIC;
            break;
        default:
            p_wpd->devType = WPAD_DEV_FUTURE;
            break;
        }

        //_devCmpt[chan] = 0;
        //_devMode[chan] = p_buf[4];
        if (p_wpd->devType == WPAD_DEV_CLASSIC) {
            if (p_wpd->devMode < 1 || p_wpd->devMode > 3) {
                p_wpd->devType = WPAD_DEV_NOT_SUPPORTED;
            }
        }

        DEBUGPrint("type : %d\n", p_wpd->devType);
        DEBUGPrint("mode : %d\n", p_wpd->devMode);
    } else {
        p_wpd->devType = WPAD_DEV_NOT_SUPPORTED;
        p_wpd->devMode = 0;
    }
}

void getGameInfo(s32 chan, s32 result, u8 index) {
    WPADControlBlock* p_wpd = _wpdcb[chan];
    WPADMEMGameInfo* p_info = p_wpd->wmReadDataPtr;
    u8* p_buf = p_wpd->wmReadDataPtr;
    int i;
    u8 sum = 0;

    if (result == WPAD_ERR_OK) {
        for (i = 0; i < 47; i++) {
            sum += p_buf[i];
        }
        sum += 0x55;

        if (p_info->checkSum == sum) {
            memcpy(&p_wpd->gameInfo, p_buf, sizeof(WPADMEMGameInfo));
            p_wpd->gameInfoErr[index] = WPAD_ERR_OK;
        } else {
            p_wpd->gameInfoErr[index] = WPAD_ERR_INVALID;
        }
    }
}

WPADExtensionCallback initExtension(s32 chan, WPADControlBlock* p_wpd) {
    WPADCmdQueue* cmdQueue = &p_wpd->extCmdQueue;
    WPADExtensionCallback extCallback = p_wpd->extensionCallback;

    WPADiSendSetReportType(cmdQueue, p_wpd->dataFormat, NULL);
    WPADiSendWriteDataCmd(cmdQueue, 0xAA, WM_REG_EXTENSION_F0, abortInitExtension);

    WPADiCreateKey(chan);
    WPADiSendWriteData(cmdQueue, p_wpd->key, 6, WM_REG_EXTENSION_40, abortInitExtension);
    WPADiSendWriteData(cmdQueue, p_wpd->key + 6, 6, WM_REG_EXTENSION_46, abortInitExtension);
    WPADiSendWriteData(cmdQueue, p_wpd->key + 12, 4, WM_REG_EXTENSION_4C, abortInitExtension);

    WPADiSendReadData(cmdQueue, p_wpd->wmReadDataBuf, 2, WM_REG_EXTENSION_DEV_MODE, abortInitExtension);
    WPADiSendReadData(cmdQueue, p_wpd->wmReadDataBuf, 0x20, WM_REG_EXTENSION_CONFIG, extCallback);

    return extCallback;
}

s32 WPADiHIDParser(u8 chan, u8* data) {
    int enable;
    s32 ret = 0;

    if ((data[0] >= 0x20) && (data[0] <= 0x3F)) {
        enable = OSDisableInterrupts();
        (*(__a1_input_reports_array[data[0] - 0x20]))(chan, data);
        OSRestoreInterrupts(enable);
        WPADiExcludeButton(chan);
        WPADiCopyOut(chan);
    } else {
        ret = -1;
    }

    return ret;
}

void __a1_20_status_report(u8 chan, u8* data) {
    WPADControlBlock* p_wpd;
    BOOL enable;
    BOOL attach;
    WPADStatus* p_status;
    WPADExtensionCallback extcb;
    u8* p_clear;
    int i;
    WPADCmdQueue* cmdQueue;

    DEBUGPrint("Received report 20\n");

    enable = OSDisableInterrupts();
    p_wpd = _wpdcb[chan];

    if (!p_wpd->handshakeFinished) {
        OSRestoreInterrupts(enable);
        return;
    }

    p_status = (WPADStatus*)&p_wpd->rxBufs[p_wpd->rxBufIndex];
    p_clear = (u8*)p_status;
    for (i = 0; i < WPAD_RX_DATASIZE; i++) {
        p_clear[i] = 0;
    }

    attach = p_wpd->info.attach;

    p_status->button =
        (u16)((u16)(((u16)((u16)(data[HID_IDX_BYTE7]) & 0xFF) | (u16)((u16)(data[HID_IDX_BYTE8] << 8) & 0xFF00))) &
                             HID_WPAD_BUTTON_MASK);
    p_wpd->info.nearempty = (u8)((data[HID_IDX_BYTE7] & HID_NEAREMPTY_MASK) >> 7);
    p_wpd->info.dpd = (data[HID_IDX_BYTE9] & HID_DPD_MASK) >> 3;
    p_wpd->info.speaker = (data[HID_IDX_BYTE9] & HID_SPEAKER_MASK) >> 2;

    p_wpd->info.attach = (data[HID_IDX_BYTE9] & HID_ATTACH_MASK) >> 1;
    p_wpd->info.lowBat = data[HID_IDX_BYTE9] & HID_LOWBAT_MASK;
    p_wpd->info.led = (u8)((data[HID_IDX_BYTE9] & HID_LED_MASK) >> 4);
    p_wpd->info.protocol = (u8)((data[HID_IDX_BYTE11] & HID_PRT_MASK) >> 4);
    p_wpd->info.firmware = (u8)(data[HID_IDX_BYTE11] & HID_FW_MASK);

    if (data[HID_IDX_BYTE12] >= 0x55) {
        p_wpd->info.battery = WPAD_BATTERY_LEVEL_MAX;
    } else if (data[HID_IDX_BYTE12] >= 0x44) {
        p_wpd->info.battery = WPAD_BATTERY_LEVEL_HIGH;
    } else if (data[HID_IDX_BYTE12] >= 0x33) {
        p_wpd->info.battery = WPAD_BATTERY_LEVEL_MEDIUM;
    } else if (data[HID_IDX_BYTE12] >= 0x3) {
        p_wpd->info.battery = WPAD_BATTERY_LEVEL_LOW;
    } else {
        p_wpd->info.battery = WPAD_BATTERY_LEVEL_CRITICAL;
    }

    if (p_wpd->info.attach) {
        if (!attach) {
            DEBUGPrint("initialize attachment\n");

            extcb = initExtension(chan, p_wpd);

            cmdQueue = &p_wpd->extCmdQueue;

            p_wpd->calibrated = 0;
            p_wpd->devType = WPAD_DEV_INITIALIZING;
            p_wpd->devMode = 0;

            if (extcb) {
                extcb(chan, WPAD_DEV_INITIALIZING);
            }
        } else {
            DEBUGPrint("already initialized\n");
        }
    } else {
        p_wpd->devType = WPAD_DEV_CORE;
        p_wpd->devMode = 0;

        if (attach) {
            WPADiClearQueue(&p_wpd->extCmdQueue);
            WPADiSendSetReportType(&p_wpd->stdCmdQueue, p_wpd->dataFormat, NULL);

            if (p_wpd->extensionCallback) {
                p_wpd->extensionCallback(chan, WPAD_DEV_CORE);
            }
        } else {
            DEBUGPrint("already disconnected\n");
        }
    }

    if (p_wpd->infoOut) {
        memcpy(p_wpd->infoOut, &p_wpd->info, sizeof(WPADInfo));
        p_wpd->infoOut = NULL;
    }

    if (p_wpd->cmdBlkCallback && p_wpd->statusReqBusy) {
        p_wpd->cmdBlkCallback(chan, WPAD_ERR_OK);
        p_wpd->cmdBlkCallback = NULL;
    }

    p_status->err = WPAD_ERR_BUSY;
    p_status->dev = p_wpd->devType;
    p_wpd->statusReqBusy = 0;
    p_wpd->rxBufIndex = (u8)((p_wpd->rxBufIndex) ? 0 : 1);

    OSRestoreInterrupts(enable);
}

void __a1_21_user_data(u8 chan, u8* data) {
    WPADControlBlock* p_wpd;
    BOOL enable;
    WPADStatus* p_status;
    u8* p_clear;
    u16 addr;
    u16 base;
    u16 i2c;
    u8 err;
    u8 len;
    s16 offset;
    int i;
    s32 status;

    enable = OSDisableInterrupts();
    p_wpd = _wpdcb[chan];
    p_status = (WPADStatus*)(p_wpd->rxBufs + p_wpd->rxBufIndex);
    p_clear = (u8*)(p_status);
    for (i = 0; i < WPAD_RX_DATASIZE; i++) {
        p_clear[i] = 0;
    }

    p_status->button =
        (u16)((u16)(((u16)((u16)(data[HID_IDX_BYTE7]) & 0xFF) | (u16)((u16)(data[HID_IDX_BYTE8] << 8) & 0xFF00))) &
                             HID_WPAD_BUTTON_MASK);

    err = (u8)(data[HID_IDX_BYTE9] & 0x0f);
    len = (u8)(((u8)(data[HID_IDX_BYTE9] >> 4) & 0x0f) + 1);
    addr = (u16)((u16)((u16)((u16)(data[HID_IDX_BYTE10]) << 8) & (u16)0xFF00) | (u16)((u16)(data[HID_IDX_BYTE11]) & (u16)0xFF));

    if (err != 0) {
        DEBUGPrint("read error happens!\n");
        p_wpd->wmReadErr = -1;

        if (p_wpd->cmdBlkCallback) {
            p_wpd->cmdBlkCallback(chan, WPAD_ERR_TRANSFER);
            p_wpd->cmdBlkCallback = NULL;
        }
        p_wpd->status = WPAD_ERR_OK;
    }

    base = (u16)(p_wpd->wmReadAddr & 0xFFFF);
    i2c = (u16)((p_wpd->wmReadAddr & 0xFFFF0000) >> 16);
    offset = (s16)(addr - base);

    if (addr >= base && addr <= base + p_wpd->wmReadLength) {
        memcpy((u8*)p_wpd->wmReadDataPtr + offset, data + HID_IDX_BYTE12, len);

        if (base + p_wpd->wmReadLength == addr + len) {
            DEBUGPrint("base addr: %08x\n", p_wpd->wmReadAddr);
            DEBUGPrint("length   : %d\n", p_wpd->wmReadLength);
            status = p_wpd->wmReadErr < 0 ? WPAD_CETRANSFER : WPAD_CESUCCESS;

            if ((p_wpd->wmReadAddr == 0 && p_wpd->oldFw == FALSE) ||
                (p_wpd->wmReadAddr == WM_ADDR_MEM_176C && p_wpd->oldFw == TRUE)) {
                getDevConfig(chan, status, p_status, data);
            }
            if (p_wpd->wmReadAddr == WM_REG_EXTENSION_CONFIG) {
                getExtConfig(chan, status);
            }
            if (p_wpd->wmReadAddr == WM_REG_EXTENSION_DEV_MODE) {
                getExtType(chan, status);
            }
            if (p_wpd->wmReadAddr == 0x2A) {
                getGameInfo(chan, status, 0);
            }
            if (p_wpd->wmReadAddr == 0x2A + sizeof(WPADMEMGameInfo)) {
                getGameInfo(chan, status, 1);
            }

            if (p_wpd->cmdBlkCallback) {
                p_wpd->cmdBlkCallback(chan, status);
                p_wpd->cmdBlkCallback = NULL;
            }
            p_wpd->status = WPAD_ERR_OK;
        }
    } else {
        DEBUGPrint("received data is out of range!\n");
    }

    p_status->err = WPAD_ERR_BUSY;
    p_status->dev = p_wpd->devType;

    p_wpd->rxBufIndex = (u8)((p_wpd->rxBufIndex) ? 0 : 1);

    OSRestoreInterrupts(enable);
}

void __a1_22_ack(u8 chan, u8* data) {
    WPADControlBlock* p_wpd = _wpdcb[chan];
    u8 type;
    u8 err;
    BOOL enable;
    WPADStatus* p_status;
    u8* p_clear;
    s32 status;
    int i;

    enable = OSDisableInterrupts();

    DEBUGPrint("Received ack!\n");

    p_wpd = _wpdcb[chan];
    p_status = (WPADStatus*)(p_wpd->rxBufs + p_wpd->rxBufIndex);
    p_clear = (u8*)(p_status);
    for (i = 0; i < WPAD_RX_DATASIZE; i++) {
        p_clear[i] = 0;
    }

    p_status->button =
        (u16)((u16)(((u16)((u16)(data[HID_IDX_BYTE7]) & 0xFF) | (u16)((u16)(data[HID_IDX_BYTE8] << 8) & 0xFF00))) & (u16)HID_WPAD_BUTTON_MASK);

    type = data[HID_IDX_BYTE9];
    err = data[HID_IDX_BYTE10];

    DEBUGPrint("ack --> report ID = %02x, error code = %d\n", type, err);

    status = err == 0 ? WPAD_CESUCCESS : WPAD_CETRANSFER;
    p_status->err = WPAD_ERR_BUSY;
    p_status->dev = p_wpd->devType;

    if (err != 0) {
        DEBUGPrint("ack error --> report ID = %d, error code = %d\n", type, err);
    }

    if (p_wpd->lastReportId == type) {
        if (p_wpd->cmdBlkCallback) {
            p_wpd->cmdBlkCallback(chan, status);
            p_wpd->cmdBlkCallback = NULL;
        }
        p_wpd->status = WPAD_ERR_OK;
    } else {
        DEBUGPrint("invalid ack!\n");
    }
    p_wpd->rxBufIndex = (u8)((p_wpd->rxBufIndex) ? 0 : 1);

    OSRestoreInterrupts(enable);
}

void __a1_30_data_type(u8 chan, u8* data) {
    WPADControlBlock* p_cb;
    WPADStatus* p_status;
    u8* p_clear;

    int old;
    int i;

    old = OSDisableInterrupts();
    p_cb = _wpdcb[chan];
    p_status = (WPADStatus*)&p_cb->rxBufs[p_cb->rxBufIndex];
    p_clear = (u8*)(p_status);
    for (i = 0; i < WPAD_RX_DATASIZE; i++) {
        p_clear[i] = 0;
    }

    p_status->button =
        (u16)((u16)(((u16)((u16)data[HID_IDX_BYTE7] & 0xFF) | (u16)((u16)(data[HID_IDX_BYTE8] << 8) & 0xFF00))) &
                             HID_WPAD_BUTTON_MASK);

    if (p_cb->dataFormat == WPAD_FMT_CORE_BTN) {
        p_status->err = p_cb->status;
    } else if (p_cb->status == 0) {
        p_status->err = WPAD_ERR_INVALID;
    }

    p_status->dev = p_cb->devType;
    p_cb->info.nearempty = (u8)((data[HID_IDX_BYTE7] & HID_NEAREMPTY_MASK) >> 7);
    p_cb->rxBufIndex = (u8)((p_cb->rxBufIndex) ? 0 : 1);
    OSRestoreInterrupts(old);
}

void __a1_31_data_type(u8 chan, u8* data) {
    WPADControlBlock* p_cb;
    WPADStatus* p_status;
    u8* p_clear;
    int old, i;

    p_cb = _wpdcb[chan];
    p_status = (WPADStatus*)&p_cb->rxBufs[p_cb->rxBufIndex];

    p_clear = (u8*)(p_status);
    for (i = 0; i < WPAD_RX_DATASIZE; i++) {
        p_clear[i] = 0;
    }

    p_status->button =
        (u16)((u16)(((u16)((u16)data[HID_IDX_BYTE7] & 0xFF) | (u16)((u16)(data[HID_IDX_BYTE8] << 8) & 0xFF00))) &
                             HID_WPAD_BUTTON_MASK);

    if (p_cb->dataFormat == WPAD_FMT_CORE_BTN || p_cb->dataFormat == WPAD_FMT_CORE_BTN_ACC) {
        p_status->err = p_cb->status;
    } else if (p_cb->status == 0) {
        p_status->err = WPAD_ERR_INVALID;
    }

    p_status->dev = p_cb->devType;
    p_cb->info.nearempty = (u8)((data[HID_IDX_BYTE7] & HID_NEAREMPTY_MASK) >> 7);
    p_status->accX = (s16)((s16)((s16)((s16)((s16)data[HID_IDX_BYTE9] << 2) & (s16)0xFFFC) |
                                 (s16)((s16)(u16)(data[HID_IDX_BYTE7] >> 5) & (s16)0x0003)) -
                           p_cb->devConf.acc_0g.x);
    p_status->accY = (s16)((s16)((s16)((s16)((s16)data[HID_IDX_BYTE10] << 2) & (s16)0xFFFC) |
                                 (s16)((s16)(u16)(data[HID_IDX_BYTE8] >> 4) & (s16)0x0002)) -
                           p_cb->devConf.acc_0g.y);
    p_status->accZ = (s16)((s16)((s16)((s16)((s16)data[HID_IDX_BYTE11] << 2) & (s16)0xFFFC) |
                                 (s16)((s16)(u16)(data[HID_IDX_BYTE8] >> 5) & (s16)0x0002)) -
                           p_cb->devConf.acc_0g.z);
    old = OSDisableInterrupts();
    p_cb->rxBufIndex = (u8)((p_cb->rxBufIndex) ? 0 : 1);
    OSRestoreInterrupts(old);
}

void __a1_32_data_type(u8 chan, u8* data) {
    WPADControlBlock* p_cb;
    WPADStatus* p_status;
    WPADFSStatus* p_fsStat;
    WPADCLStatus* p_clStat;
    u8* p_clear;
    int old, i;
    s16 x, y;
    old = OSDisableInterrupts();
    p_cb = _wpdcb[chan];
    p_status = (WPADStatus*)&p_cb->rxBufs[p_cb->rxBufIndex];

    p_clear = (u8*)(p_status);
    for (i = 0; i < WPAD_RX_DATASIZE; i++) {
        p_clear[i] = 0;
    }

    p_status->button =
        (u16)((u16)(((u16)((u16)(data[HID_IDX_BYTE7]) & 0xFF) | (u16)((u16)(data[HID_IDX_BYTE8] << 8) & 0xFF00))) & (u16)HID_WPAD_BUTTON_MASK);

    if (p_cb->dataFormat == WPAD_FMT_CORE_BTN || p_cb->dataFormat == WPAD_FMT_FS_BTN || p_cb->dataFormat == WPAD_FMT_CLASSIC_BTN) {
        p_status->err = p_cb->status;
    } else if (p_cb->status == 0) {
        p_status->err = WPAD_ERR_INVALID;
    }

    p_status->dev = p_cb->devType;
    p_cb->info.nearempty = (u8)((data[HID_IDX_BYTE7] & HID_NEAREMPTY_MASK) >> 7);

    memcpy(checkBuffer, data + HID_IDX_BYTE9, 8);
    WPADiDecode(chan, data + HID_IDX_BYTE9, 8, 0);

    if (p_cb->info.attach) {
        if (p_cb->devType == WPAD_DEV_FS) {
            p_fsStat = (WPADFSStatus*)&p_cb->rxBufs[p_cb->rxBufIndex];

            p_fsStat->fsStickX = (s8)(data[HID_IDX_BYTE9]);
            p_fsStat->fsStickY = (s8)(data[HID_IDX_BYTE10]);
            p_fsStat->fsAccX =
                (s16)((s16)((s16)((s16)((s16)(data[HID_IDX_BYTE11]) << 2) & (s16)0xFFFC) | (s16)((s16)(data[HID_IDX_BYTE14] >> 2) & (s16)(0x0003))) -
                      p_cb->extConf.fs.acc_0g.x);
            p_fsStat->fsAccY =
                (s16)((s16)((s16)((s16)((s16)(data[HID_IDX_BYTE12]) << 2) & (s16)0xFFFC) | (s16)((s16)(data[HID_IDX_BYTE14] >> 4) & (s16)(0x0003))) -
                      p_cb->extConf.fs.acc_0g.y);
            p_fsStat->fsAccZ =
                (s16)((s16)((s16)((s16)((s16)(data[HID_IDX_BYTE13]) << 2) & (s16)0xFFFC) | (s16)((s16)(data[HID_IDX_BYTE14] >> 6) & (s16)(0x0003))) -
                      p_cb->extConf.fs.acc_0g.z);

            p_fsStat->base.button |= (u16)(((u16)(~data[HID_IDX_BYTE14]) << 13) & (u16)0x6000);

            if (!p_cb->calibrated) {
                p_cb->calibrated = 1;
                p_cb->extConf.fs.stick.x = p_fsStat->fsStickX;
                p_cb->extConf.fs.stick.y = p_fsStat->fsStickY;
            }

            x = (s16)((u8)p_fsStat->fsStickX - (u8)p_cb->extConf.fs.stick.x);
            y = (s16)((u8)p_fsStat->fsStickY - (u8)p_cb->extConf.fs.stick.y);

            if (x < -128) {
                p_fsStat->fsStickX = -128;
            } else if (x > 127) {
                p_fsStat->fsStickX = 127;
            } else {
                p_fsStat->fsStickX = (s8)x;
            }

            if (y < -128) {
                p_fsStat->fsStickY = -128;
            } else if (y > 127) {
                p_fsStat->fsStickY = 127;
            } else {
                p_fsStat->fsStickY = (s8)y;
            }
        } else if (p_cb->devType == WPAD_DEV_CLASSIC) {
            switch (p_cb->devMode) {
            case 1:
                p_clStat = (WPADCLStatus*)&p_cb->rxBufs[p_cb->rxBufIndex];

                p_clStat->clLStickX = (s16)((s16)((s16)((s16)(data[HID_IDX_BYTE9]) & (s16)0x003F) << 4));
                p_clStat->clLStickY = (s16)((s16)((s16)((s16)(data[HID_IDX_BYTE10]) & (s16)0x003F) << 4));
                p_clStat->clRStickX = (s16)((s16)((s16)((s16)((s16)((s16)(data[HID_IDX_BYTE9]) >> 3) & (s16)0x0018) |
                                                        (s16)((s16)((s16)(data[HID_IDX_BYTE10]) >> 5) & (s16)0x0006) |
                                                        (s16)((s16)((s16)(data[HID_IDX_BYTE11]) >> 7) & (s16)0x0001))
                                                  << 5) -
                                            (s16)(1024 / 2));
                p_clStat->clRStickY = (s16)((s16)((s16)((s16)(data[HID_IDX_BYTE11]) & (s16)0x001F) << 5) - (s16)(1024 / 2));
                p_clStat->clTriggerL =
                    (u8)((u8)((u8)((u8)(data[HID_IDX_BYTE11] >> 2) & (u8)0x18) | (u8)((u8)(data[HID_IDX_BYTE12] >> 5) & (u8)0x07)) << 3);
                p_clStat->clTriggerR = (u8)((u8)(data[HID_IDX_BYTE12] & (u8)0x1F) << 3);

                p_clStat->clButton =
                    (u16)((u16)((u16)((u16)(data[HID_IDX_BYTE13]) << 8) & (u16)0xFF00 | (u16)(data[HID_IDX_BYTE14]) & (u16)0xFF) ^ 0xFFFF);
                break;

            case 2:
                p_clStat = (WPADCLStatus*)&p_cb->rxBufs[p_cb->rxBufIndex];
                p_clStat->clLStickX = (s16)((s16)((s16)((s16)((s16)(data[HID_IDX_BYTE9]) << 2) & (s16)0xFFFC) |
                                                  (s16)((s16)((s16)(data[HID_IDX_BYTE13])) & (s16)0x0003)) -
                                            (s16)(1024 / 2));
                p_clStat->clRStickX = (s16)((s16)((s16)((s16)((s16)(data[HID_IDX_BYTE10]) << 2) & (s16)0xFFFC) |
                                                  (s16)((s16)((s16)(data[HID_IDX_BYTE13]) >> 2) & (s16)0x0003)) -
                                            (s16)(1024 / 2));
                p_clStat->clLStickY = (s16)((s16)((s16)((s16)((s16)(data[HID_IDX_BYTE11]) << 2) & (s16)0xFFFC) |
                                                  (s16)((s16)((s16)(data[HID_IDX_BYTE13]) >> 4) & (s16)0x0003)) -
                                            (s16)(1024 / 2));
                p_clStat->clRStickY = (s16)((s16)((s16)((s16)((s16)(data[HID_IDX_BYTE12]) << 2) & (s16)0xFFFC) |
                                                  (s16)((s16)((s16)(data[HID_IDX_BYTE13]) >> 6) & (s16)0x0003)) -
                                            (s16)(1024 / 2));
                p_clStat->clTriggerL = (u8)(data[HID_IDX_BYTE14]);
                p_clStat->clTriggerR = (u8)(data[HID_IDX_BYTE15]);

                p_clStat->clButton =
                    (u16)((u16)((u16)((u16)(data[HID_IDX_BYTE16]) << 8) & (u16)0xFF00 | (u16)(data[HID_IDX_BYTE17]) & (u16)0xFF) ^ 0xFFFF);
                break;

            case 3:
                p_clStat = (WPADCLStatus*)&p_cb->rxBufs[p_cb->rxBufIndex];

                p_clStat->clLStickX = (s16)((s16)((s16)((s16)(data[HID_IDX_BYTE9]) & (s16)0xFF) << 2) - (s16)(1024 / 2));
                p_clStat->clRStickX = (s16)((s16)((s16)((s16)(data[HID_IDX_BYTE10]) & (s16)0xFF) << 2) - (s16)(1024 / 2));
                p_clStat->clLStickY = (s16)((s16)((s16)((s16)(data[HID_IDX_BYTE11]) & (s16)0xFF) << 2) - (s16)(1024 / 2));
                p_clStat->clRStickY = (s16)((s16)((s16)((s16)(data[HID_IDX_BYTE12]) & (s16)0xFF) << 2) - (s16)(1024 / 2));
                p_clStat->clTriggerL = (u8)(data[HID_IDX_BYTE13]);
                p_clStat->clTriggerR = (u8)(data[HID_IDX_BYTE14]);

                p_clStat->clButton =
                    (u16)((u16)((u16)((u16)(data[HID_IDX_BYTE15]) << 8) & (u16)0xFF00 | (u16)(data[HID_IDX_BYTE16]) & (u16)0xFF) ^ 0xFFFF);
                break;
            }

            if (!p_cb->calibrated) {
                p_cb->calibrated = 1;
                p_cb->extConf.cl.lstk.x = p_clStat->clLStickX;
                p_cb->extConf.cl.lstk.y = p_clStat->clLStickY;
                p_cb->extConf.cl.rstk.x = p_clStat->clRStickX;
                p_cb->extConf.cl.rstk.y = p_clStat->clRStickY;
                p_cb->extConf.cl.triggerL = p_clStat->clTriggerL;
                p_cb->extConf.cl.triggerR = p_clStat->clTriggerR;
            }

            x = (s16)(p_clStat->clLStickX - p_cb->extConf.cl.lstk.x);
            y = (s16)(p_clStat->clLStickY - p_cb->extConf.cl.lstk.y);

            if (x < -0x400) {
                p_clStat->clLStickX = -0x400;
            } else if (x > 0x3FF) {
                p_clStat->clLStickX = 0x3FF;
            } else {
                p_clStat->clLStickX = x;
            }

            if (y < -0x400) {
                p_clStat->clLStickY = -0x400;
            } else if (y > 0x3FF) {
                p_clStat->clLStickY = 0x3FF;
            } else {
                p_clStat->clLStickY = y;
            }

            x = (s16)(p_clStat->clRStickX - p_cb->extConf.cl.rstk.x);
            y = (s16)(p_clStat->clRStickY - p_cb->extConf.cl.rstk.y);

            if (x < -0x400) {
                p_clStat->clRStickX = -0x400;
            } else if (x > 0x3FF) {
                p_clStat->clRStickX = 0x3FF;
            } else {
                p_clStat->clRStickX = x;
            }

            if (y < -0x400) {
                p_clStat->clRStickY = -0x400;
            } else if (y > 0x3FF) {
                p_clStat->clRStickY = 0x3FF;
            } else {
                p_clStat->clRStickY = y;
            }

            x = (s16)((u8)p_clStat->clTriggerL - (u8)p_cb->extConf.cl.triggerL);
            y = (s16)((u8)p_clStat->clTriggerR - (u8)p_cb->extConf.cl.triggerR);

            if (x < 0) {
                p_clStat->clTriggerL = 0;
            } else {
                p_clStat->clTriggerL = p_clStat->clTriggerL = (u8)x;
            }

            if (y < 0) {
                p_clStat->clTriggerR = 0;
            } else {
                p_clStat->clTriggerR = p_clStat->clTriggerR = (u8)y;
            }
        }

        if (!memcmp(checkBuffer, checkInvalidData, 7)) {
            if (p_status->err == WPAD_ERR_OK) {
                p_status->err = WPAD_ERR_CORRUPTED;
            }
        }
    }

    p_cb->rxBufIndex = (u8)((p_cb->rxBufIndex) ? 0 : 1);
    OSRestoreInterrupts(old);
}

void __a1_33_data_type(u8 chan, u8* data) {
    WPADControlBlock* p_cb;
    WPADStatus* p_status;
    u8* p_clear;
    int old, i;

    p_cb = _wpdcb[chan];
    p_status = (WPADStatus*)&p_cb->rxBufs[p_cb->rxBufIndex];
    p_clear = (u8*)(p_status);

    for (i = 0; i < WPAD_RX_DATASIZE; i++) {
        p_clear[i] = 0;
    }

    p_status->button =
        (u16)((u16)(((u16)((u16)(data[HID_IDX_BYTE7]) & 0xFF) | (u16)((u16)(data[HID_IDX_BYTE8] << 8) & 0xFF00))) & (u16)HID_WPAD_BUTTON_MASK);

    if (p_cb->dataFormat == WPAD_FMT_CORE_BTN || p_cb->dataFormat == WPAD_FMT_CORE_BTN_ACC || p_cb->dataFormat == WPAD_FMT_CORE_BTN_ACC_DPD) {
        p_status->err = WPAD_ERR_OK;
    } else {
        p_status->err = WPAD_ERR_INVALID;
    }

    p_status->dev = p_cb->devType;
    p_cb->info.nearempty = (u8)((data[HID_IDX_BYTE7] & HID_NEAREMPTY_MASK) >> 7);
    p_status->accX = (s16)((s16)((s16)((s16)((s16)((s16)(data[HID_IDX_BYTE9]) << 2) & (s16)0xFFFC) |
                                       (s16)((s16)((u16)(data[HID_IDX_BYTE7] >> 5)) & (s16)0x0003))) -
                           (s16)(p_cb->devConf.acc_0g.x));
    p_status->accY = (s16)((s16)((s16)((s16)((s16)((s16)(data[HID_IDX_BYTE10]) << 2) & (s16)0xFFFC) |
                                       (s16)((s16)((u16)(data[HID_IDX_BYTE8] >> 4)) & (s16)0x0002))) -
                           (s16)(p_cb->devConf.acc_0g.y));
    p_status->accZ = (s16)((s16)((s16)((s16)((s16)((s16)(data[HID_IDX_BYTE11]) << 2) & (s16)0xFFFC) |
                                       (s16)((s16)((u16)(data[HID_IDX_BYTE8] >> 5)) & (s16)0x0002))) -
                           (s16)(p_cb->devConf.acc_0g.z));

    __parse_dpd_data(chan, &p_status, WPAD_DPD_STANDARD, data, 0);

    old = OSDisableInterrupts();
    p_cb->rxBufIndex = (u8)((p_cb->rxBufIndex) ? 0 : 1);
    OSRestoreInterrupts(old);
}

void __a1_34_data_type(u8 chan, u8* data) {
    // empty function
}

void __a1_35_data_type(u8 chan, u8* data) {
    WPADControlBlock* p_cb;
    WPADStatus* p_status;
    WPADFSStatus* p_fsStat;
    WPADCLStatus* p_clStat;
    u8* p_clear;
    int old, i;
    s16 x, y;
    p_cb = _wpdcb[chan];
    p_status = (WPADStatus*)&p_cb->rxBufs[p_cb->rxBufIndex];

    p_clear = (u8*)(p_status);
    for (i = 0; i < WPAD_RX_DATASIZE; i++) {
        p_clear[i] = 0;
    }

    p_status->button =
        (u16)((u16)(((u16)((u16)(data[HID_IDX_BYTE7]) & 0xFF) | (u16)((u16)(data[HID_IDX_BYTE8] << 8) & 0xFF00))) & (u16)HID_WPAD_BUTTON_MASK);

    if (p_cb->dataFormat == WPAD_FMT_CORE_BTN || p_cb->dataFormat == WPAD_FMT_CORE_BTN_ACC || p_cb->dataFormat == WPAD_FMT_FS_BTN ||
        p_cb->dataFormat == WPAD_FMT_FS_BTN_ACC || p_cb->dataFormat == WPAD_FMT_CLASSIC_BTN || p_cb->dataFormat == WPAD_FMT_CLASSIC_BTN_ACC) {
        p_status->err = p_cb->status;
    } else if (p_cb->status != 0) {
        p_status->err = WPAD_ERR_INVALID;
    }

    p_status->dev = p_cb->devType;
    p_cb->info.nearempty = (u8)((data[HID_IDX_BYTE7] & HID_NEAREMPTY_MASK) >> 7);
    p_status->accX = (s16)((s16)((s16)((s16)((s16)((s16)(data[HID_IDX_BYTE9]) << 2) & (s16)0xFFFC) |
                                       (s16)((s16)((u16)(data[HID_IDX_BYTE7] >> 5)) & (s16)0x0003))) -
                           (s16)(p_cb->devConf.acc_0g.x));
    p_status->accY = (s16)((s16)((s16)((s16)((s16)((s16)(data[HID_IDX_BYTE10]) << 2) & (s16)0xFFFC) |
                                       (s16)((s16)((u16)(data[HID_IDX_BYTE8] >> 4)) & (s16)0x0002))) -
                           (s16)(p_cb->devConf.acc_0g.y));
    p_status->accZ = (s16)((s16)((s16)((s16)((s16)((s16)(data[HID_IDX_BYTE11]) << 2) & (s16)0xFFFC) |
                                       (s16)((s16)((u16)(data[HID_IDX_BYTE8] >> 5)) & (s16)0x0002))) -
                           (s16)(p_cb->devConf.acc_0g.z));

    memcpy(checkBuffer, data + HID_IDX_BYTE12, 16);
    WPADiDecode(chan, data + HID_IDX_BYTE12, 16, 0);

    if (p_cb->info.attach) {
        if (p_cb->devType == WPAD_DEV_FS) {
            p_fsStat = (WPADFSStatus*)&p_cb->rxBufs[p_cb->rxBufIndex];

            p_fsStat->fsStickX = (s8)(data[HID_IDX_BYTE12]);
            p_fsStat->fsStickY = (s8)(data[HID_IDX_BYTE13]);
            p_fsStat->fsAccX =
                (s16)((s16)((s16)((s16)((s16)(data[HID_IDX_BYTE14]) << 2) & (s16)0xFFFC) | (s16)((s16)(data[HID_IDX_BYTE17] >> 2) & (s16)(0x0003))) -
                      (s16)(p_cb->extConf.fs.acc_0g.x));
            p_fsStat->fsAccY =
                (s16)((s16)((s16)((s16)((s16)(data[HID_IDX_BYTE15]) << 2) & (s16)0xFFFC) | (s16)((s16)(data[HID_IDX_BYTE17] >> 4) & (s16)(0x0003))) -
                      (s16)(p_cb->extConf.fs.acc_0g.y));
            p_fsStat->fsAccZ =
                (s16)((s16)((s16)((s16)((s16)(data[HID_IDX_BYTE16]) << 2) & (s16)0xFFFC) | (s16)((s16)(data[HID_IDX_BYTE17] >> 6) & (s16)(0x0003))) -
                      (s16)(p_cb->extConf.fs.acc_0g.z));

            p_fsStat->base.button |= (u16)(((u16)(~data[HID_IDX_BYTE17]) << 13) & (u16)0x6000);

            if (!p_cb->calibrated) {
                p_cb->calibrated = 1;
                p_cb->extConf.fs.stick.x = p_fsStat->fsStickX;
                p_cb->extConf.fs.stick.y = p_fsStat->fsStickY;
            }

            x = (s16)((u8)p_fsStat->fsStickX - (u8)p_cb->extConf.fs.stick.x);
            y = (s16)((u8)p_fsStat->fsStickY - (u8)p_cb->extConf.fs.stick.y);

            if (x < -128) {
                p_fsStat->fsStickX = -128;
            } else if (x > 127) {
                p_fsStat->fsStickX = 127;
            } else {
                p_fsStat->fsStickX = (s8)x;
            }

            if (y < -128) {
                p_fsStat->fsStickY = -128;
            } else if (y > 127) {
                p_fsStat->fsStickY = 127;
            } else {
                p_fsStat->fsStickY = (s8)y;
            }
        } else if (p_cb->devType == WPAD_DEV_CLASSIC) {
            switch (p_cb->devMode) {
            case 1:
                p_clStat = (WPADCLStatus*)&p_cb->rxBufs[p_cb->rxBufIndex];
                p_clStat->clLStickX = (s16)((s16)((s16)((s16)(data[HID_IDX_BYTE12]) & (s16)0x003F) << 4) - (s16)(1024 / 2));
                p_clStat->clLStickY = (s16)((s16)((s16)((s16)(data[HID_IDX_BYTE13]) & (s16)0x003F) << 4) - (s16)(1024 / 2));
                p_clStat->clRStickX = (s16)((s16)((s16)((s16)((s16)((s16)(data[HID_IDX_BYTE12]) >> 3) & (s16)0x0018) |
                                                        (s16)((s16)((s16)(data[HID_IDX_BYTE13]) >> 5) & (s16)0x0006) |
                                                        (s16)((s16)((s16)(data[HID_IDX_BYTE14]) >> 7) & (s16)0x0001))
                                                  << 5) -
                                            (s16)(1024 / 2));
                p_clStat->clRStickY = (s16)((s16)((s16)((s16)(data[HID_IDX_BYTE14]) & (s16)0x001F) << 5) - (s16)(1024 / 2));
                p_clStat->clTriggerL =
                    (u8)((u8)((u8)((u8)(data[HID_IDX_BYTE14] >> 2) & (u8)0x18) | (u8)((u8)(data[HID_IDX_BYTE15] >> 5) & (u8)0x07)) << 3);
                p_clStat->clTriggerR = (u8)((u8)((u8)(data[HID_IDX_BYTE15]) & (u8)0x1F) << 3);

                p_clStat->clButton =
                    (u16)((u16)((u16)((u16)(data[HID_IDX_BYTE16]) << 8) & (u16)0xFF00 | (u16)(data[HID_IDX_BYTE17]) & (u16)0xFF) ^ 0xFFFF);
                break;

            case 2:
                p_clStat = (WPADCLStatus*)&p_cb->rxBufs[p_cb->rxBufIndex];
                p_clStat->clLStickX = (s16)((s16)((s16)((s16)((s16)(data[HID_IDX_BYTE12]) << 2) & (s16)0xFFFC) |
                                                  (s16)((s16)((s16)(data[HID_IDX_BYTE16])) & (s16)0x0003)) -
                                            (s16)(1024 / 2));
                p_clStat->clRStickX = (s16)((s16)((s16)((s16)((s16)(data[HID_IDX_BYTE13]) << 2) & (s16)0xFFFC) |
                                                  (s16)((s16)((s16)(data[HID_IDX_BYTE16]) >> 2) & (s16)0x0003)) -
                                            (s16)(1024 / 2));
                p_clStat->clLStickY = (s16)((s16)((s16)((s16)((s16)(data[HID_IDX_BYTE14]) << 2) & (s16)0xFFFC) |
                                                  (s16)((s16)((s16)(data[HID_IDX_BYTE16]) >> 4) & (s16)0x0003)) -
                                            (s16)(1024 / 2));
                p_clStat->clRStickY = (s16)((s16)((s16)((s16)((s16)(data[HID_IDX_BYTE15]) << 2) & (s16)0xFFFC) |
                                                  (s16)((s16)((s16)(data[HID_IDX_BYTE16]) >> 6) & (s16)0x0003)) -
                                            (s16)(1024 / 2));
                p_clStat->clTriggerL = (u8)(data[HID_IDX_BYTE17]);
                p_clStat->clTriggerR = (u8)(data[HID_IDX_BYTE18]);

                p_clStat->clButton =
                    (u16)((u16)((u16)((u16)(data[HID_IDX_BYTE19]) << 8) & (u16)0xFF00 | (u16)(data[HID_IDX_BYTE20]) & (u16)0xFF) ^ 0xFFFF);
                break;

            case 3:
                p_clStat = (WPADCLStatus*)&p_cb->rxBufs[p_cb->rxBufIndex];

                p_clStat->clLStickX = (s16)((s16)((s16)((s16)(data[HID_IDX_BYTE12]) & (s16)0xFF) << 2) - (s16)(1024 / 2));
                p_clStat->clRStickX = (s16)((s16)((s16)((s16)(data[HID_IDX_BYTE13]) & (s16)0xFF) << 2) - (s16)(1024 / 2));
                p_clStat->clLStickY = (s16)((s16)((s16)((s16)(data[HID_IDX_BYTE14]) & (s16)0xFF) << 2) - (s16)(1024 / 2));
                p_clStat->clRStickY = (s16)((s16)((s16)((s16)(data[HID_IDX_BYTE15]) & (s16)0xFF) << 2) - (s16)(1024 / 2));
                p_clStat->clTriggerL = (u8)(data[HID_IDX_BYTE16]);
                p_clStat->clTriggerR = (u8)(data[HID_IDX_BYTE17]);

                p_clStat->clButton =
                    (u16)((u16)((u16)((u16)(data[HID_IDX_BYTE18]) << 8) & (u16)0xFF00 | (u16)(data[HID_IDX_BYTE19]) & (u16)0xFF) ^ 0xFFFF);
                break;
            }

            if (!p_cb->calibrated) {
                p_cb->calibrated = 1;
                p_cb->extConf.cl.lstk.x = p_clStat->clLStickX;
                p_cb->extConf.cl.lstk.y = p_clStat->clLStickY;
                p_cb->extConf.cl.rstk.x = p_clStat->clRStickX;
                p_cb->extConf.cl.rstk.y = p_clStat->clRStickY;
                p_cb->extConf.cl.triggerL = p_clStat->clTriggerL;
                p_cb->extConf.cl.triggerR = p_clStat->clTriggerR;
            }

            x = (s16)(p_clStat->clLStickX - p_cb->extConf.cl.lstk.x);
            y = (s16)(p_clStat->clLStickY - p_cb->extConf.cl.lstk.y);

            if (x < -0x400) {
                p_clStat->clLStickX = -0x400;
            } else if (x > 0x3FF) {
                p_clStat->clLStickX = 0x3FF;
            } else {
                p_clStat->clLStickX = x;
            }

            if (y < -0x400) {
                p_clStat->clLStickY = -0x400;
            } else if (y > 0x3FF) {
                p_clStat->clLStickY = 0x3FF;
            } else {
                p_clStat->clLStickY = y;
            }

            x = (s16)(p_clStat->clRStickX - p_cb->extConf.cl.rstk.x);
            y = (s16)(p_clStat->clRStickY - p_cb->extConf.cl.rstk.y);

            if (x < -0x400) {
                p_clStat->clRStickX = -0x400;
            } else if (x > 0x3FF) {
                p_clStat->clRStickX = 0x3FF;
            } else {
                p_clStat->clRStickX = x;
            }

            if (y < -0x400) {
                p_clStat->clRStickY = -0x400;
            } else if (y > 0x3FF) {
                p_clStat->clRStickY = 0x3FF;
            } else {
                p_clStat->clRStickY = y;
            }

            x = (s16)(p_clStat->clTriggerL - p_cb->extConf.cl.triggerL);
            y = (s16)(p_clStat->clTriggerR - p_cb->extConf.cl.triggerR);

            if (x < 0) {
                p_clStat->clTriggerL = 0;
            } else {
                p_clStat->clTriggerL = p_clStat->clTriggerL = (u8)x;
            }

            if (y < 0) {
                p_clStat->clTriggerR = 0;
            } else {
                p_clStat->clTriggerR = p_clStat->clTriggerR = (u8)y;
            }
        }

        if (!memcmp(checkBuffer, checkInvalidData, 16)) {
            if (p_status->err == WPAD_ERR_OK) {
                p_status->err = WPAD_ERR_CORRUPTED;
            }
        }
    }

    old = OSDisableInterrupts();
    p_cb->rxBufIndex = (u8)((p_cb->rxBufIndex) ? 0 : 1);
    OSRestoreInterrupts(old);
}

void __a1_36_data_type(u8 chan, u8* data) {
    // empty function
}

void __a1_37_data_type(u8 chan, u8* data) {
    WPADControlBlock* p_cb;
    WPADStatus* p_status;
    WPADFSStatus* p_fsStat;
    WPADCLStatus* p_clStat;
    u8* p_clear;
    int old, i;
    s16 x, y;
    s16 temp_r0;
    p_cb = _wpdcb[chan];
    p_status = (WPADStatus*)&p_cb->rxBufs[p_cb->rxBufIndex];
    p_clear = (u8*)(p_status);

    for (i = 0; i < WPAD_RX_DATASIZE; i++) {
        p_clear[i] = 0;
    }

    p_status->button =
        (u16)((u16)(((u16)((u16)(data[HID_IDX_BYTE7]) & 0xFF) | (u16)((u16)(data[HID_IDX_BYTE8] << 8) & 0xFF00))) & (u16)HID_WPAD_BUTTON_MASK);

    if (p_cb->dataFormat == WPAD_FMT_CORE_BTN || p_cb->dataFormat == WPAD_FMT_CORE_BTN_ACC || p_cb->dataFormat == WPAD_FMT_CORE_BTN_ACC_DPD ||
        p_cb->dataFormat == WPAD_FMT_FS_BTN || p_cb->dataFormat == WPAD_FMT_FS_BTN_ACC || p_cb->dataFormat == WPAD_FMT_FS_BTN_ACC_DPD ||
        p_cb->dataFormat == WPAD_FMT_CLASSIC_BTN || p_cb->dataFormat == WPAD_FMT_CLASSIC_BTN_ACC || p_cb->dataFormat == WPAD_FMT_CLASSIC_BTN_ACC_DPD) {
        p_status->err = p_cb->status;
    } else if (p_cb->status == 0) {
        p_status->err = WPAD_ERR_INVALID;
    }

    p_status->dev = p_cb->devType;
    p_cb->info.nearempty = (u8)((data[HID_IDX_BYTE7] & HID_NEAREMPTY_MASK) >> 7);
    p_status->accX = (s16)((s16)((s16)((s16)((s16)((s16)(data[HID_IDX_BYTE9]) << 2) & (s16)0xFFFC) |
                                       (s16)((s16)((u16)(data[HID_IDX_BYTE7] >> 5)) & (s16)0x0003))) -
                           p_cb->devConf.acc_0g.x);
    p_status->accY = (s16)((s16)((s16)((s16)((s16)data[HID_IDX_BYTE10] << 2) & (s16)0xFFFC) |
                                 (s16)((s16)(u16)(data[HID_IDX_BYTE8] >> 4) & (s16)0x0002)) -
                           p_cb->devConf.acc_0g.y);
    p_status->accZ = (s16)((s16)((s16)((s16)((s16)data[HID_IDX_BYTE11] << 2) & (s16)0xFFFC) |
                                 (s16)((s16)(u16)(data[HID_IDX_BYTE8] >> 5) & (s16)0x0002)) -
                           p_cb->devConf.acc_0g.z);

    __parse_dpd_data(chan, &p_status, WPAD_DPD_STANDARD, data, 0);

    memcpy(checkBuffer, data + HID_IDX_BYTE22, 6);
    WPADiDecode(chan, data + HID_IDX_BYTE22, 6, 0);

    if (p_cb->info.attach) {
        if (p_cb->devType == WPAD_DEV_FS) {
            p_fsStat = (WPADFSStatus*)&p_cb->rxBufs[p_cb->rxBufIndex];

            p_fsStat->fsStickX = (s8)(data[HID_IDX_BYTE22]);
            p_fsStat->fsStickY = (s8)(data[HID_IDX_BYTE23]);
            p_fsStat->fsAccX =
                (s16)((s16)((s16)((s16)((s16)(data[HID_IDX_BYTE24]) << 2) & (s16)0xFFFC) | (s16)((s16)(data[HID_IDX_BYTE27] >> 2) & (s16)(0x0003))) -
                      p_cb->extConf.fs.acc_0g.x);
            p_fsStat->fsAccY =
                (s16)((s16)((s16)((s16)((s16)(data[HID_IDX_BYTE25]) << 2) & (s16)0xFFFC) | (s16)((s16)(data[HID_IDX_BYTE27] >> 4) & (s16)(0x0003))) -
                      p_cb->extConf.fs.acc_0g.y);
            p_fsStat->fsAccZ =
                (s16)((s16)((s16)((s16)((s16)(data[HID_IDX_BYTE26]) << 2) & (s16)0xFFFC) | (s16)((s16)(data[HID_IDX_BYTE27] >> 6) & (s16)(0x0003))) -
                      p_cb->extConf.fs.acc_0g.z);

            p_fsStat->base.button |= (u16)(((u16)(~data[HID_IDX_BYTE27]) << 13) & (u16)0x6000);

            if (!p_cb->calibrated) {
                p_cb->calibrated = 1;
                p_cb->extConf.fs.stick.x = p_fsStat->fsStickX;
                p_cb->extConf.fs.stick.y = p_fsStat->fsStickY;
            }

            x = (s16)((u8)p_fsStat->fsStickX - (u8)p_cb->extConf.fs.stick.x);
            y = (s16)((u8)p_fsStat->fsStickY - (u8)p_cb->extConf.fs.stick.y);

            if (x < -128) {
                p_fsStat->fsStickX = -128;
            } else if (x > 127) {
                p_fsStat->fsStickX = 127;
            } else {
                p_fsStat->fsStickX = (s8)x;
            }

            if (y < -128) {
                p_fsStat->fsStickY = -128;
            } else if (y > 127) {
                p_fsStat->fsStickY = 127;
            } else {
                p_fsStat->fsStickY = (s8)y;
            }
        } else if (p_cb->devType == WPAD_DEV_CLASSIC) {
            switch (p_cb->devMode) {
            case 1:
                p_clStat = (WPADCLStatus*)&p_cb->rxBufs[p_cb->rxBufIndex];
                p_clStat->clLStickX = (s16)((s16)((s16)((s16)(data[HID_IDX_BYTE22]) & (s16)0x003F) << 4) - (s16)(1024 / 2));
                p_clStat->clLStickY = (s16)((s16)((s16)((s16)(data[HID_IDX_BYTE23]) & (s16)0x003F) << 4) - (s16)(1024 / 2));
                p_clStat->clRStickX = (s16)((s16)((s16)((s16)((s16)((s16)(data[HID_IDX_BYTE22]) >> 3) & (s16)0x0018) |
                                                        (s16)((s16)((s16)(data[HID_IDX_BYTE23]) >> 5) & (s16)0x0006) |
                                                        (s16)((s16)((s16)(data[HID_IDX_BYTE24]) >> 7) & (s16)0x0001))
                                                  << 5) -
                                            (s16)(1024 / 2));
                p_clStat->clRStickY = (s16)((s16)((s16)((s16)(data[HID_IDX_BYTE24]) & (s16)0x001F) << 5) - (s16)(1024 / 2));
                p_clStat->clTriggerL =
                    (u8)((u8)((u8)((u8)(data[HID_IDX_BYTE24] >> 2) & (u8)0x18) | (u8)((u8)(data[HID_IDX_BYTE25] >> 5) & (u8)0x07)) << 3);
                p_clStat->clTriggerR = (u8)((u8)((u8)(data[HID_IDX_BYTE25]) & (u8)0x1F) << 3);

                p_clStat->clButton =
                    (u16)((u16)((u16)((u16)(data[HID_IDX_BYTE26]) << 8) & (u16)0xFF00 | (u16)(data[HID_IDX_BYTE27]) & (u16)0xFF) ^ 0xFFFF);
                break;

            case 2:
                p_clStat = (WPADCLStatus*)&p_cb->rxBufs[p_cb->rxBufIndex];
                p_clStat->clLStickX = (s16)((s16)((s16)((s16)((s16)(data[HID_IDX_BYTE22]) << 2) & (s16)0xFFFC) |
                                                  (s16)((s16)((s16)(data[HID_IDX_BYTE26])) & (s16)0x0003)) -
                                            (s16)(1024 / 2));
                p_clStat->clRStickX = (s16)((s16)((s16)((s16)((s16)(data[HID_IDX_BYTE23]) << 2) & (s16)0xFFFC) |
                                                  (s16)((s16)((s16)(data[HID_IDX_BYTE26]) >> 2) & (s16)0x0003)) -
                                            (s16)(1024 / 2));
                p_clStat->clLStickY = (s16)((s16)((s16)((s16)((s16)(data[HID_IDX_BYTE24]) << 2) & (s16)0xFFFC) |
                                                  (s16)((s16)((s16)(data[HID_IDX_BYTE26]) >> 4) & (s16)0x0003)) -
                                            (s16)(1024 / 2));
                p_clStat->clRStickY = (s16)((s16)((s16)((s16)((s16)(data[HID_IDX_BYTE25]) << 2) & (s16)0xFFFC) |
                                                  (s16)((s16)((s16)(data[HID_IDX_BYTE26]) >> 6) & (s16)0x0003)) -
                                            (s16)(1024 / 2));
                p_clStat->clTriggerL = (u8)(data[HID_IDX_BYTE27]);
                p_clStat->clTriggerR = (u8)0;

                p_clStat->clButton = (u16)0;
                break;

            case 3:
                p_clStat = (WPADCLStatus*)&p_cb->rxBufs[p_cb->rxBufIndex];

                p_clStat->clLStickX = (s16)((s16)((s16)((s16)(data[HID_IDX_BYTE22]) & (s16)0xFF) << 2) - (s16)(1024 / 2));
                p_clStat->clRStickX = (s16)((s16)((s16)((s16)(data[HID_IDX_BYTE23]) & (s16)0xFF) << 2) - (s16)(1024 / 2));
                p_clStat->clLStickY = (s16)((s16)((s16)((s16)(data[HID_IDX_BYTE24]) & (s16)0xFF) << 2) - (s16)(1024 / 2));
                p_clStat->clRStickY = (s16)((s16)((s16)((s16)(data[HID_IDX_BYTE25]) & (s16)0xFF) << 2) - (s16)(1024 / 2));
                p_clStat->clTriggerL = (u8)(data[HID_IDX_BYTE26]);
                p_clStat->clTriggerR = (u8)(data[HID_IDX_BYTE27]);

                p_clStat->clButton = (u16)0;
                break;
            }

            if (!p_cb->calibrated) {
                p_cb->calibrated = 1;
                p_cb->extConf.cl.lstk.x = p_clStat->clLStickX;
                p_cb->extConf.cl.lstk.y = p_clStat->clLStickY;
                p_cb->extConf.cl.rstk.x = p_clStat->clRStickX;
                p_cb->extConf.cl.rstk.y = p_clStat->clRStickY;
                p_cb->extConf.cl.triggerL = p_clStat->clTriggerL;
                p_cb->extConf.cl.triggerR = p_clStat->clTriggerR;
            }

            x = (s16)(p_clStat->clLStickX - p_cb->extConf.cl.lstk.x);
            y = (s16)(p_clStat->clLStickY - p_cb->extConf.cl.lstk.y);

            if (x < -0x400) {
                p_clStat->clLStickX = -0x400;
            } else if (x > 0x3FF) {
                p_clStat->clLStickX = 0x3FF;
            } else {
                p_clStat->clLStickX = x;
            }

            if (y < -0x400) {
                p_clStat->clLStickY = -0x400;
            } else if (y > 0x3FF) {
                p_clStat->clLStickY = 0x3FF;
            } else {
                p_clStat->clLStickY = y;
            }

            x = (s16)(p_clStat->clRStickX - p_cb->extConf.cl.rstk.x);
            y = (s16)(p_clStat->clRStickY - p_cb->extConf.cl.rstk.y);

            if (x < -0x400) {
                p_clStat->clRStickX = -0x400;
            } else if (x > 0x3FF) {
                p_clStat->clRStickX = 0x3FF;
            } else {
                p_clStat->clRStickX = x;
            }

            if (y < -0x400) {
                p_clStat->clRStickY = -0x400;
            } else if (y > 0x3FF) {
                p_clStat->clRStickY = 0x3FF;
            } else {
                p_clStat->clRStickY = y;
            }

            x = (s16)(p_clStat->clTriggerL - p_cb->extConf.cl.triggerL);
            y = (s16)(p_clStat->clTriggerR - p_cb->extConf.cl.triggerR);

            if (x < 0) {
                p_clStat->clTriggerL = 0;
            } else {
                p_clStat->clTriggerL = p_clStat->clTriggerL = (u8)x;
            }

            if (y < 0) {
                p_clStat->clTriggerR = 0;
            } else {
                p_clStat->clTriggerR = p_clStat->clTriggerR = (u8)y;
            }
        }

        if (!memcmp(checkBuffer, checkInvalidData, 6)) {
            if (p_status->err == WPAD_ERR_OK) {
                p_status->err = WPAD_ERR_CORRUPTED;
            }
        }
    }

    old = OSDisableInterrupts();
    p_cb->rxBufIndex = (u8)((p_cb->rxBufIndex) ? 0 : 1);
    OSRestoreInterrupts(old);
}
void __a1_3d_data_type(u8 chan, u8* data) {}

u8 _recv_3e[WPAD_MAX_CONTROLLERS] = {0, 0, 0, 0};
u8 _recv_3f[WPAD_MAX_CONTROLLERS] = {0, 0, 0, 0};

void __a1_3e_data_type(u8 chan, u8* data) {
    WPADControlBlock* p_cb;
    WPADStatusEx* p_status;
    u8* p_clear;
    int old, i;
    p_cb = _wpdcb[chan];
    p_status = (WPADStatusEx*)&p_cb->rxBufs[p_cb->rxBufIndex];

    if (_recv_3e[chan] == 0 && _recv_3f[chan] == 0) {
        p_clear = (u8*)(p_status);
        for (i = 0; i < WPAD_RX_DATASIZE; i++) {
            p_clear[i] = 0;
        }
    }

    p_status->base.button =
        (u16)((u16)(((u16)((u16)(data[HID_IDX_BYTE7]) & 0xFF) | (u16)((u16)(data[HID_IDX_BYTE8] << 8) & 0xFF00))) & (u16)HID_WPAD_BUTTON_MASK);

    if (p_cb->dataFormat == WPAD_FMT_CORE_BTN ||
        p_cb->dataFormat == WPAD_FMT_CORE_BTN_ACC ||
        p_cb->dataFormat == WPAD_FMT_BTN_ACC_DPD_EXTENDED) {
        p_status->base.err = p_cb->status;
    } else if (p_cb->status == 0) {
        p_status->base.err = WPAD_ERR_INVALID;
    }
    p_status->base.dev = p_cb->devType;
    p_cb->info.nearempty = 0;
    p_status->base.accX = (s16)((s16)((s16)((s16)((s16)data[HID_IDX_BYTE9] << 2) & (s16)0xFFFC) |
                                      (s16)((s16)(u16)(data[HID_IDX_BYTE7] >> 6) & (s16)0x0002)) -
                           p_cb->devConf.acc_0g.x);
    p_status->base.accZ |= (s16)((s16)((s16)((s16)data[HID_IDX_BYTE8] << 3) & (s16)0xFF00) |
                                 (s16)((s16)(u16)(data[HID_IDX_BYTE7] << 1) & (s16)0x00C0));

    p_status->base.obj[0].x = (s16)((s16)((u16)data[HID_IDX_BYTE10] & 0xFF) |
                                    (u16)((u16)(data[HID_IDX_BYTE12] & 0x30) << 4));
    p_status->base.obj[0].y =
        (s16)(WPAD_DPD_IMG_RESO_WY - 1 - (s16)((s16)(((u16)(data[HID_IDX_BYTE11])) & 0xFF) | (u16)(((u16)(data[HID_IDX_BYTE12] & 0xC0)) << 2)));
    p_status->exp[0].pixel = (u16)(((s16)((s16)(((u16)(data[HID_IDX_BYTE17]) << 8) & 0xFF00) | (u16)(data[HID_IDX_BYTE18] & 0xFF))) * 64);
    p_status->exp[0].radius = (s8)((data[HID_IDX_BYTE12] & 0x0f));
    p_status->exp[0].range_x1 = (s16)(((s8)(data[HID_IDX_BYTE13]) == -1) ? 0 : data[HID_IDX_BYTE13]);
    p_status->exp[0].range_y1 = (s16)(((s8)(data[HID_IDX_BYTE14]) == -1) ? 0 : data[HID_IDX_BYTE14]);
    p_status->exp[0].range_x2 = (s16)(((s8)(data[HID_IDX_BYTE15]) == -1) ? 0 : data[HID_IDX_BYTE15]);
    p_status->exp[0].range_y2 = (s16)(((s8)(data[HID_IDX_BYTE16]) == -1) ? 0 : data[HID_IDX_BYTE16]);
    p_status->exp[0].range_x1 = (s16)(p_status->exp[0].range_x1 * 8);
    p_status->exp[0].range_y1 = (s16)((s16)(WPAD_DPD_IMG_RESO_WY - 1) - (s16)(p_status->exp[0].range_y1 * 8));
    p_status->exp[0].range_x2 = (s16)(p_status->exp[0].range_x2 * 8);
    p_status->exp[0].range_y2 = (s16)((s16)(WPAD_DPD_IMG_RESO_WY - 1) - (s16)(p_status->exp[0].range_y2 * 8));
    p_status->base.obj[0].size = (u16)((f32)p_status->exp[0].radius * (f32)p_status->exp[0].radius * PI);
    if (p_status->base.obj[0].size == 0 || p_status->base.obj[0].x == (s16)(WPAD_DPD_IMG_RESO_WX - 1) ||
        p_status->base.obj[0].y == (s16)(WPAD_DPD_IMG_RESO_WY - 1) || p_status->exp[0].radius == 0xf) {
        p_status->base.obj[0].x = (s16)0;
        p_status->base.obj[0].y = (s16)(WPAD_DPD_IMG_RESO_WY - 1);
        p_status->base.obj[0].size = 0;
        p_status->exp[0].pixel = 0;
        p_status->exp[0].radius = 0;
    }
    p_status->base.obj[0].traceId = 0;

    p_status->base.obj[1].x = (s16)((s16)((u16)data[HID_IDX_BYTE19] & 0xFF) |
                               (u16)((u16)(data[HID_IDX_BYTE21] & 0x30) << 4));
    p_status->base.obj[1].y =
        (s16)(WPAD_DPD_IMG_RESO_WY - 1 - (s16)((s16)(((u16)(data[HID_IDX_BYTE20])) & 0xFF) | (u16)(((u16)(data[HID_IDX_BYTE21] & 0xC0)) << 2)));
    p_status->exp[1].pixel = (u16)(((s16)((s16)(((u16)(data[HID_IDX_BYTE26]) << 8) & 0xFF00) | (u16)(data[HID_IDX_BYTE27] & 0xFF))) * 64);
    p_status->exp[1].radius = (s8)((data[HID_IDX_BYTE21] & 0x0f));
    p_status->exp[1].range_x1 = (s16)(((s8)(data[HID_IDX_BYTE22]) == -1) ? 0 : data[HID_IDX_BYTE22]);
    p_status->exp[1].range_y1 = (s16)(((s8)(data[HID_IDX_BYTE23]) == -1) ? 0 : data[HID_IDX_BYTE23]);
    p_status->exp[1].range_x2 = (s16)(((s8)(data[HID_IDX_BYTE24]) == -1) ? 0 : data[HID_IDX_BYTE24]);
    p_status->exp[1].range_y2 = (s16)(((s8)(data[HID_IDX_BYTE25]) == -1) ? 0 : data[HID_IDX_BYTE25]);
    p_status->exp[1].range_x1 = (s16)(p_status->exp[1].range_x1 * 8);
    p_status->exp[1].range_y1 = (s16)((s16)(WPAD_DPD_IMG_RESO_WY - 1) - (s16)(p_status->exp[1].range_y1 * 8));
    p_status->exp[1].range_x2 = (s16)(p_status->exp[1].range_x2 * 8);
    p_status->exp[1].range_y2 = (s16)((s16)(WPAD_DPD_IMG_RESO_WY - 1) - (s16)(p_status->exp[1].range_y2 * 8));
    p_status->base.obj[1].size = (u16)((f32)p_status->exp[1].radius * (f32)p_status->exp[1].radius * PI);
    if (p_status->base.obj[1].size == 0 || p_status->base.obj[1].x == (s16)(WPAD_DPD_IMG_RESO_WX - 1) ||
        p_status->base.obj[1].y == (s16)(WPAD_DPD_IMG_RESO_WY - 1) || p_status->exp[1].radius == 0xf) {
        p_status->base.obj[1].x = (s16)0;
        p_status->base.obj[1].y = (s16)(WPAD_DPD_IMG_RESO_WY - 1);
        p_status->base.obj[1].size = 0;
        p_status->exp[1].pixel = 0;
        p_status->exp[1].radius = 0;
    }
    p_status->base.obj[1].traceId = 1;

    old = OSDisableInterrupts();
    _recv_3e[chan] = 1;
    if (_recv_3e[chan] && _recv_3f[chan]) {
        p_status->base.accZ -= (s16)(p_cb->devConf.acc_0g.z);
        p_cb->rxBufIndex = (u8)((p_cb->rxBufIndex) ? 0 : 1);
        _recv_3e[chan] = _recv_3f[chan] = 0;
    }
    OSRestoreInterrupts(old);
}

void __a1_3f_data_type(u8 chan, u8* data) {
    WPADControlBlock* p_cb;
    WPADStatusEx* p_status;
    u8* p_clear;
    int old, i;
    p_cb = _wpdcb[chan];
    p_status = (WPADStatusEx*)&p_cb->rxBufs[p_cb->rxBufIndex];

    if (_recv_3e[chan] == 0 && _recv_3f[chan] == 0) {
        p_clear = (u8*)(p_status);
        for (i = 0; i < WPAD_RX_DATASIZE; i++) {
            p_clear[i] = 0;
        }
    }

    p_status->base.button =
        (u16)((u16)(((u16)((u16)(data[HID_IDX_BYTE7]) & 0xFF) | (u16)((u16)(data[HID_IDX_BYTE8] << 8) & 0xFF00))) &
                             HID_WPAD_BUTTON_MASK);

    if (p_cb->dataFormat == WPAD_FMT_CORE_BTN || p_cb->dataFormat == WPAD_FMT_CORE_BTN_ACC || p_cb->dataFormat == WPAD_FMT_BTN_ACC_DPD_EXTENDED) {
        p_status->base.err = p_cb->status;
    } else if (p_cb->status == 0) {
        p_status->base.err = WPAD_ERR_INVALID;
    }
    p_status->base.dev = p_cb->devType;
    p_cb->info.nearempty = 0;
    p_status->base.accY = (s16)((s16)((s16)((s16)((s16)data[HID_IDX_BYTE9] << 2) & (s16)0xFFFC) |
                                 (s16)((s16)(u16)(data[HID_IDX_BYTE7] >> 6) & (s16)0x0002)) -
                           p_cb->devConf.acc_0g.y);
    p_status->base.accZ |= (s16)((s16)((s16)((s16)data[HID_IDX_BYTE8] >> 1) & (s16)0x0030) |
                            (s16)((s16)(u16)(data[HID_IDX_BYTE7] >> 3) & (s16)0x000C));

    p_status->base.obj[2].x = (s16)((s16)((u16)data[HID_IDX_BYTE10] & 0xFF) |
                               (u16)((u16)(data[HID_IDX_BYTE12] & 0x30) << 4));
    p_status->base.obj[2].y =
        (s16)(WPAD_DPD_IMG_RESO_WY - 1 - (s16)((s16)(((u16)(data[HID_IDX_BYTE11])) & 0xFF) | (u16)(((u16)(data[HID_IDX_BYTE12] & 0xC0)) << 2)));
    p_status->exp[2].pixel = (u16)(((s16)((s16)(((u16)(data[HID_IDX_BYTE17]) << 8) & 0xFF00) | (u16)(data[HID_IDX_BYTE18] & 0xFF))) * 64);
    p_status->exp[2].radius = (s8)((data[HID_IDX_BYTE12] & 0x0f));
    p_status->exp[2].range_x1 = (s16)(((s8)(data[HID_IDX_BYTE13]) == -1) ? 0 : data[HID_IDX_BYTE13]);
    p_status->exp[2].range_y1 = (s16)(((s8)(data[HID_IDX_BYTE14]) == -1) ? 0 : data[HID_IDX_BYTE14]);
    p_status->exp[2].range_x2 = (s16)(((s8)(data[HID_IDX_BYTE15]) == -1) ? 0 : data[HID_IDX_BYTE15]);
    p_status->exp[2].range_y2 = (s16)(((s8)(data[HID_IDX_BYTE16]) == -1) ? 0 : data[HID_IDX_BYTE16]);
    p_status->exp[2].range_x1 = (s16)(p_status->exp[2].range_x1 * 8);
    p_status->exp[2].range_y1 = (s16)((s16)(WPAD_DPD_IMG_RESO_WY - 1) - (s16)(p_status->exp[2].range_y1 * 8));
    p_status->exp[2].range_x2 = (s16)(p_status->exp[2].range_x2 * 8);
    p_status->exp[2].range_y2 = (s16)((s16)(WPAD_DPD_IMG_RESO_WY - 1) - (s16)(p_status->exp[2].range_y2 * 8));
    p_status->base.obj[2].size = (u16)((f32)p_status->exp[2].radius * (f32)p_status->exp[2].radius * PI);
    if (p_status->base.obj[2].size == 0 || p_status->base.obj[2].x == (s16)(WPAD_DPD_IMG_RESO_WX - 1) ||
        p_status->base.obj[2].y == (s16)(WPAD_DPD_IMG_RESO_WY - 1) || p_status->exp[2].radius == 0xf) {
        p_status->base.obj[2].x = (s16)0;
        p_status->base.obj[2].y = (s16)(WPAD_DPD_IMG_RESO_WY - 1);
        p_status->base.obj[2].size = 0;
        p_status->exp[2].pixel = 0;
        p_status->exp[2].radius = 0;
    }
    p_status->base.obj[2].traceId = 2;

    p_status->base.obj[3].x = (s16)((s16)((u16)data[HID_IDX_BYTE19] & 0xFF) |
                                    (u16)((u16)(data[HID_IDX_BYTE21] & 0x30) << 4));
    p_status->base.obj[3].y =
        (s16)(WPAD_DPD_IMG_RESO_WY - 1 - (s16)((s16)(((u16)(data[HID_IDX_BYTE20])) & 0xFF) | (u16)(((u16)(data[HID_IDX_BYTE21] & 0xC0)) << 2)));
    p_status->exp[3].pixel = (u16)(((s16)((s16)(((u16)(data[HID_IDX_BYTE26]) << 8) & 0xFF00) | (u16)(data[HID_IDX_BYTE27] & 0xFF))) * 64);
    p_status->exp[3].radius = (s8)((data[HID_IDX_BYTE21] & 0x0f));
    p_status->exp[3].range_x1 = (s16)(((s8)(data[HID_IDX_BYTE22]) == -1) ? 0 : data[HID_IDX_BYTE22]);
    p_status->exp[3].range_y1 = (s16)(((s8)(data[HID_IDX_BYTE23]) == -1) ? 0 : data[HID_IDX_BYTE23]);
    p_status->exp[3].range_x2 = (s16)(((s8)(data[HID_IDX_BYTE24]) == -1) ? 0 : data[HID_IDX_BYTE24]);
    p_status->exp[3].range_y2 = (s16)(((s8)(data[HID_IDX_BYTE25]) == -1) ? 0 : data[HID_IDX_BYTE25]);
    p_status->exp[3].range_x1 = (s16)(p_status->exp[3].range_x1 * 8);
    p_status->exp[3].range_y1 = (s16)((s16)(WPAD_DPD_IMG_RESO_WY - 1) - (s16)(p_status->exp[3].range_y1 * 8));
    p_status->exp[3].range_x2 = (s16)(p_status->exp[3].range_x2 * 8);
    p_status->exp[3].range_y2 = (s16)((s16)(WPAD_DPD_IMG_RESO_WY - 1) - (s16)(p_status->exp[3].range_y2 * 8));
    p_status->base.obj[3].size = (u16)((f32)p_status->exp[3].radius * (f32)p_status->exp[3].radius * PI);
    if (p_status->base.obj[3].size == 0 || p_status->base.obj[3].x == (s16)(WPAD_DPD_IMG_RESO_WX - 1) ||
        p_status->base.obj[3].y == (s16)(WPAD_DPD_IMG_RESO_WY - 1) || p_status->exp[3].radius == 0xf) {
        p_status->base.obj[3].x = (s16)0;
        p_status->base.obj[3].y = (s16)(WPAD_DPD_IMG_RESO_WY - 1);
        p_status->base.obj[3].size = 0;
        p_status->exp[3].pixel = 0;
        p_status->exp[3].radius = 0;
    }

    p_status->base.obj[3].traceId = 3;
    old = OSDisableInterrupts();
    _recv_3f[chan] = 1;
    if (_recv_3e[chan] && _recv_3f[chan]) {
        p_status->base.accZ -= p_cb->devConf.acc_0g.z;
        p_cb->rxBufIndex = (u8)((p_cb->rxBufIndex) ? 0 : 1);
        _recv_3e[chan] = _recv_3f[chan] = 0;
    }
    OSRestoreInterrupts(old);
}

void __a1_unused_report(u8 chan, u8* data) {}
