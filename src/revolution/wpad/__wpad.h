#ifndef _REVOLUTION_WPAD_PRIVATE_H_
#define _REVOLUTION_WPAD_PRIVATE_H_

#include <revolution/os.h>
#include <revolution/wpad.h>

#ifdef __cplusplus
extern "C" {
#endif

#define RPT_REPORT_ID					0
#define RPT_MAX_SIZE					21

// Limits
#define RPTID_OUT_LO					RPTID_SET_RUMBLE
#define RPTID_OUT_HI					RPTID_SET_DPD_CSB

#define RPTID_IN_LO						RPTID_STATUS
#define RPTID_IN_HI						RPTID_DATA_BTN_ACC_DPD18_2

// All outgoing reports
#define RPT_OUT_FLAGS					1

#define RPT_OUT_FLAG_ENABLE_FEATURE		2
#define RPT_OUT_FLAG_REQUEST_ACK_RPT	1
#define RPT_OUT_FLAG_RUMBLE				0

// All incoming reports with Button data (all except 0x3d)
#define RPT_BTN0						1
#define RPT_BTN1						2

// All incoming reports with Accelerometer (Standard) data (0x31, 0x33, 0x35, 0x37)
#define RPT_ACC_OFFSET					1
#define RPT_ACC_SIZE					3

/*
 * Outgoing reports (from POV of the host)
 */

// Report 0x10: Set rumble status
#define RPTID_SET_RUMBLE				0x10
#define RPT10_SIZE						1

#define RPT10_RUMBLE					0

// Report 0x11: Set LED lights
#define RPTID_SET_PORT					0x11 // internally called SetPort
#define	RPT11_SIZE						1

#define RPT11_LED						0

// Report 0x12: Set data reporting mode
#define RPTID_SET_DATA_REPORT_MODE		0x12
#define RPT12_SIZE						2

#define RPT12_CONT_REPORT				0
#define RPT12_DATA_REPORT_MODE			1

// Report 0x13: Enable IR camera
#define RPTID_ENABLE_DPD				0x13 // internally called the DPD
#define RPT13_SIZE						1

#define RPT13_DPD_ENABLE				0

// Report 0x14: Enable IR camera
#define RPTID_ENABLE_SPEAKER			0x14
#define RPT14_SIZE						1

#define RPT14_SPEAKER_ENABLE			0

// Report 0x15: Request status report
#define RPTID_REQUEST_STATUS			0x15
#define RPT15_SIZE						1

// Report 0x16: Write data
#define RPTID_WRITE_DATA				0x16
#define RPT16_SIZE						RPT_MAX_SIZE

#define RPT16_DATA_DST_ADDRESS			0
#define RPT16_DATA_LENGTH				4
#define RPT16_DATA						5

// Report 0x17: Read data
#define RPTID_READ_DATA					0x17
#define RPT17_SIZE						6

#define RPT17_DATA_SRC_ADDRESS			0
#define RPT17_DATA_LENGTH				4

// Report 0x18: Send speaker data
#define RPTID_SEND_SPEAKER_DATA			0x18
#define RPT18_SIZE						RPT_MAX_SIZE

#define RPT18_DATA_LENGTH				0
#define RPT18_DATA						1

// Report 0x19: Mute speaker
#define RPTID_MUTE_SPEAKER				0x19
#define RPT19_SIZE						1

#define RPT19_SPEAKER_MUTE				0

// Report 0x19: WPADiSendDPDCSB (?)
#define RPTID_SEND_DPD_CSB				0x1a
#define RPT1A_SIZE						1

#define RPT1A_DPD_CSB					0

/*
 * Incoming reports (from POV of the host)
 */

// Report 0x20: Status report
#define RPTID_STATUS					0x20
#define RPT20_FLAGS						3
#define RPT20_PROTO_FW					5
#define RPT20_BATTERY					6

// Report 0x21: Read Wiimote data
#define RPTID_DATA_READ					0x21
#define RPT21_SIZE_ERR					3
#define RPT21_ADDR0						4
#define RPT21_ADDR1						5
#define RPT21_DATA						6

// Report 0x22: Acknowledgement and request result
#define RPTID_ACK						0x22
#define RPT22_ACKED_RPT_ID				3
#define RPT22_ERR_CODE					4

// Reports 0x23-0x2f: Unused

// Report 0x30: Buttons
#define RPTID_DATA_BTN					0x30

// Report 0x31: Buttons, Accelerometer (Standard)
#define RPTID_DATA_BTN_ACC				0x31

// Report 0x32: Buttons, Extension (8 bytes)
#define RPTID_DATA_BTN_EXT8				0x32
#define RPT32_EXT_OFFSET				3
#define RPT32_EXT_LENGTH				8

/* Report 0x33: Buttons, Accelerometer (Standard), IR Camera (Standard, 12
 * bytes)
 */
#define RPTID_DATA_BTN_ACC_DPD12		0x33
#define RPT33_DPD_OFFSET				6
#define RPT33_DPD_LENGTH				12

// Report 0x34: Buttons, Extension (19 bytes)
#define RPTID_DATA_BTN_EXT19			0x34
#define RPT34_EXT_OFFSET				3
#define RPT34_EXT_LENGTH				19

// Report 0x35: Buttons, Accelerometer (Standard), Extension (16 bytes)
#define RPTID_DATA_BTN_ACC_EXT16		0x35
#define RPT35_EXT_OFFSET				6
#define RPT35_EXT_LENGTH				16

// Report 0x36: Buttons, IR Camera (Basic, 10 bytes), Extension (9 bytes)
#define RPTID_DATA_BTN_DPD10_EXT9		0x36
#define RPT36_DPD_OFFSET				3
#define RPT36_DPD_LENGTH				10
#define RPT36_EXT_OFFSET				13
#define RPT36_EXT_LENGTH				9

/* Report 0x37: Buttons, Accelerometer (Standard), IR Camera (Basic, 10
 * bytes), Extension (6 bytes)
 */
#define RPTID_DATA_BTN_ACC_DPD10_EXT9	0x37
#define RPT37_DPD_OFFSET				6
#define RPT37_DPD_LENGTH				10
#define RPT37_EXT_OFFSET				16
#define RPT37_EXT_LENGTH				6

// Report 0x38-0x3c: Unused

// Report 0x3d: Extension (21 bytes)
#define RPTID_DATA_EXT21				0x3d
#define RPT3D_EXT_OFFSET				1
#define RPT3D_EXT_LENGTH				21

/* Report 0x3e: Buttons, Accelerometer (Interleaved 1), IR Camera (Full 1,
 * 18 bytes)
 */
#define RPTID_DATA_BTN_ACC_DPD18_1		0x3e
#define RPT3E_DPD0						0
#define RPT3E_DPD0_OFFSET				4
#define RPT3E_DPD1						1
#define RPT3E_DPD1_OFFSET				13

/* Report 0x3f: Buttons, Accelerometer (Interleaved 2), IR Camera (Full 2,
 * 18 bytes)
 */
#define RPTID_DATA_BTN_ACC_DPD18_2		0x3f
#define RPT3F_DPD2						2
#define RPT3F_DPD2_OFFSET				4
#define RPT3F_DPD3						3
#define RPT3F_DPD3_OFFSET				13

#define WPAD_EXT_REG_SPEAKER      0xa2
#define WPAD_EXT_REG_EXTENSION    0xa4
#define WPAD_EXT_REG_MOTION_PLUS  0xa6
#define WPAD_EXT_REG_DPD          0xb0

#define RX_BUFFER_SIZE OSRoundUp32B(sizeof(WPADStatusEx))
#define LINT_NUM_MAX_LENGTH 64
#define LINT_NUM_MAX_BUFSIZ (1 + LINT_NUM_MAX_LENGTH + 1)

#define WM_MEM_ADDR(addr_) ((addr_) & 0xffff)
#define WM_EXT_REG_ADDR(type_, addr_)                                                              \
    (((addr_) & 0xffff) | ((WPAD_EXT_REG_##type_) << 16) | (1 << 26))

// https://wiibrew.org/wiki/Wiimote#EEPROM_Memory
#define WM_ADDR_MEM_DEV_CONFIG_0 WM_MEM_ADDR(0x0000)
#define WM_ADDR_MEM_GAME_INFO_0 WM_MEM_ADDR(0x002a)
#define WM_ADDR_MEM_GAME_INFO_1 WM_MEM_ADDR(0x0062)

// out of range??? wiibrew says the wiimote only has 0x1600 bytes of memory
#define WM_ADDR_MEM_176C WM_MEM_ADDR(0x176c)
#define WM_ADDR_MEM_1770 WM_MEM_ADDR(0x1770)

/*
 * Wiimote extension register addresses
 */

// Speaker register addresses
#define SPK_RST_REG     WM_EXT_REG_ADDR(SPEAKER, 0x01)
#define SPK_CTRL_REG    WM_EXT_REG_ADDR(SPEAKER, 0x08)
#define SPK_CLK_REG     WM_EXT_REG_ADDR(SPEAKER, 0x09)

// Extension register addresses
#define WM_REG_EXTENSION_CONFIG WM_EXT_REG_ADDR(EXTENSION, 0x20)
#define WM_REG_EXTENSION_40 WM_EXT_REG_ADDR(EXTENSION, 0x40)
#define WM_REG_EXTENSION_46 WM_EXT_REG_ADDR(EXTENSION, 0x46)
#define WM_REG_EXTENSION_4C WM_EXT_REG_ADDR(EXTENSION, 0x4c)
#define WM_REG_EXTENSION_CERT_PARAM WM_EXT_REG_ADDR(EXTENSION, 0x50)
#define WM_REG_EXTENSION_F0 WM_EXT_REG_ADDR(EXTENSION, 0xf0)
#define WM_REG_EXTENSION_CERT_CHALLENGE WM_EXT_REG_ADDR(EXTENSION, 0xf1)
#define WM_REG_EXTENSION_F2 WM_EXT_REG_ADDR(EXTENSION, 0xf2)
#define WM_REG_EXTENSION_F3 WM_EXT_REG_ADDR(EXTENSION, 0xf3)
#define WM_REG_EXTENSION_EXT_TYPE_2 WM_EXT_REG_ADDR(EXTENSION, 0xf6)
#define WM_REG_EXTENSION_CERT_PROBE WM_EXT_REG_ADDR(EXTENSION, 0xf7)
#define WM_REG_EXTENSION_FA WM_EXT_REG_ADDR(EXTENSION, 0xfa)
#define WM_REG_EXTENSION_FB WM_EXT_REG_ADDR(EXTENSION, 0xfb)
#define WM_REG_EXTENSION_DEV_MODE WM_EXT_REG_ADDR(EXTENSION, 0xfe)
#define WM_REG_EXTENSION_ID_BYTE WM_EXT_REG_ADDR(EXTENSION, 0xff)

#define WM_EXTENSION_CONFIG_SIZE 0x20      // 0xa40020 - 0xa4003f
#define WM_EXTENSION_CERT_PARAM_SIZE 0x40  // 0xa40050 - 0xa4008f

// Wiimote Motion Plus register addresses
#define WM_REG_MPLS_F0 WM_EXT_REG_ADDR(MOTION_PLUS, 0xf0)
#define WM_REG_MPLS_DEV_MODE WM_EXT_REG_ADDR(MOTION_PLUS, 0xfe)
#define WM_REG_MPLS_ID_BYTE WM_EXT_REG_ADDR(MOTION_PLUS, 0xff)

// IR camera register addresses
#define WM_REG_DPD_CONFIG_BLOCK_1 WM_EXT_REG_ADDR(DPD, 0x00)
#define WM_REG_DPD_CONFIG_BLOCK_2 WM_EXT_REG_ADDR(DPD, 0x1a)
#define WM_REG_DPD_30 WM_EXT_REG_ADDR(DPD, 0x30)
#define WM_REG_DPD_DATA_FORMAT WM_EXT_REG_ADDR(DPD, 0x33)


#define WPAD_NZFILTER_ACC   0
#define WPAD_NZFILTER_DPD   1
#define WPAD_NZFILTER_EXT   2
#define WPAD_NZFILTER_MPLS  3
#define WPAD_MAX_NZFILTERS  4

#define WPAD_BATTERY_LEVEL_CRITICAL 0
#define WPAD_BATTERY_LEVEL_LOW      1
#define WPAD_BATTERY_LEVEL_MEDIUM   2
#define WPAD_BATTERY_LEVEL_HIGH     3
#define WPAD_BATTERY_LEVEL_MAX      4

#define WPAD_RADIO_QUALITY_GOOD  0  // 80+
#define WPAD_RADIO_QUALITY_BAD   1  // 80-

#define WPAD_RX_DATASIZE 96
#define WPAD_COMMAND_CMD_MAX_LEN 24
#if PLATFORM_SHIELD
#define WPAD_COMMAND_EXT_MAX_LEN 12
#else
#define WPAD_COMMAND_EXT_MAX_LEN 8
#endif
#define WPAD_COMMAND_BUF_LEN 22

#define WPAD_I2CDEV_SEL_MASK    (u32)(0xFF000000)
#define WPAD_I2CDEV_DADR_MASK   (u32)(0x00FF0000)
#define WPAD_I2CDEV_UADR_MASK   (u32)(0x0000FF00)
#define WPAD_I2CDEV_LADR_MASK   (u32)(0x000000FF)
#define WPAD_WRITE_LEN_MASK     (u16)(0x001F)
#define WPAD_READ_ULEN_MASK     (u16)(0xFF00)
#define WPAD_READ_LLEN_MASK     (u16)(0x00FF)

#define SPK_CLK_OFF     0x00
#define SPK_CLK_ON      0x01
#define SPK_DERESET     0x01
#if SDK_AUG2010
#define SPK_RESET       0x80
#else
#define SPK_RESET       0x08
#endif
#define SPK_CTRL_PLAY   0x01

// Utility macros to help with SDK version differences

#if SDK_AUG2010
#define DIFF_COUNT_DPD(p_wpd) (p_wpd->filterDiff[WPAD_NZFILTER_DPD])
#define HYST_COUNT_DPD(p_wpd) (p_wpd->filterSame[WPAD_NZFILTER_DPD])
#define DIFF_COUNT_ACC(p_wpd) (p_wpd->filterDiff[WPAD_NZFILTER_ACC])
#define HYST_COUNT_ACC(p_wpd) (p_wpd->filterSame[WPAD_NZFILTER_ACC])
#define DIFF_COUNT_FS_ACC(p_wpd) (p_wpd->filterDiff[WPAD_NZFILTER_EXT])
#define HYST_COUNT_FS_ACC(p_wpd) (p_wpd->filterSame[WPAD_NZFILTER_EXT])
#else
#define DIFF_COUNT_DPD(p_wpd) (p_wpd->diffCountDpd)
#define HYST_COUNT_DPD(p_wpd) (p_wpd->hystCountDpd)
#define DIFF_COUNT_ACC(p_wpd) (p_wpd->diffCountAcc)
#define HYST_COUNT_ACC(p_wpd) (p_wpd->hystCountAcc)
#define DIFF_COUNT_FS_ACC(p_wpd) (p_wpd->diffCountfsAcc)
#define HYST_COUNT_FS_ACC(p_wpd) (p_wpd->hystCountfsAcc)
#endif

typedef struct WPADUnkStatus {
    /* 0x00 */ WPADStatus base;
    /* 0x2A */ u8 field_0x2a[0x60 - 0x2a];
} WPADUnkStatus;

typedef struct WPADMEMGameInfo {
    /* 0x00 */ OSTime calendarTimeTick;
    /* 0x08 */ u16 gameTitle[17];
    /* 0x2A */ char gameId[4];
    /* 0x2E */ u8 gameType;
    /* 0x2F */ u8 checkSum;
    /* 0x30 */ u8 reserved[8];
} WPADMEMGameInfo;

typedef struct WPADMemBlock {
    /* 0x00 */ BOOL busy;
    /* 0x04 */ const u8* data;
    /* 0x08 */ BOOL len;
    /* 0x0C */ BOOL addr;
    /* 0x10 */ WPADCallback* cb;
} WPADMemBlock;

typedef struct WPADCommand {
    /* 0x00 */ u32 command;
    /* 0x04 */ u8 data[22];
    /* 0x1A */ u16 len;
    /* 0x1C */ void* readBuf;
    /* 0x20 */ u16 readLen;
    /* 0x24 */ u32 readAddr;
    /* 0x28 */ WPADInfo* info;
    /* 0x2C */ WPADCallback callback;
} WPADCommand;

typedef struct WPADCmdQueue {
    /* 0x00 */ s8 head;
    /* 0x01 */ s8 tail;
    /* 0x04 */ WPADCommand* cmd;
    /* 0x08 */ u32 cmdlen;
} WPADCmdQueue;

typedef struct WPADDevConfig {
    /* 0x00 */ DPDObject obj[WPAD_DPD_MAX_OBJECTS];
    /* 0x20 */ WPADAcc acc_0g;
    /* 0x26 */ WPADAcc acc_1g;
    /* 0x2C */ u8 motor;
    /* 0x2D */ u8 volume;
#if SDK_AUG2010
    /* 0x2E */ u8 unk_0x2e[0x30 - 0x2e];
#endif
} WPADDevConfig;

typedef struct WPADStick {
    /* 0x00 */ s16 x;
    /* 0x02 */ s16 x_min;
    /* 0x04 */ s16 x_max;
    /* 0x06 */ s16 y;
    /* 0x08 */ s16 y_min;
    /* 0x0A */ s16 y_max;
} WPADStick;

typedef struct WPADMplsCalibration {
    /* 0x00 */ f32 pitchZero;
    /* 0x04 */ f32 pitchScale;
    /* 0x08 */ f32 yawZero;
    /* 0x0c */ f32 yawScale;
    /* 0x10 */ f32 rollZero;
    /* 0x14 */ f32 rollScale;
    /* 0x18 */ s32 degrees;
} WPADMplsCalibration;

typedef struct WPADFsConfig {
    /* 0x00 */ WPADStick stick;
    /* 0x0C */ WPADAcc acc_0g;
    /* 0x12 */ WPADAcc acc_1g;
} WPADFsConfig;

typedef struct WPADClConfig {
    /* 0x00 */ WPADStick lstk;
    /* 0x0C */ WPADStick rstk;
    /* 0x18 */ u8 triggerL;
    /* 0x19 */ u8 triggerR;
} WPADClConfig;

typedef struct WPADExtConfig {
    /* 0x00 */ union {
        WPADFsConfig fs;
        WPADClConfig cl;
#if SDK_AUG2010
        u8 bytes[0x1c];
#else
        u8 bytes[0x18];
#endif
    };
#if SDK_AUG2010
    struct WPADMplsConfig {
        /* 0x00 */ WPADMplsCalibration high;
        /* 0x1c */ WPADMplsCalibration low;
        /* 0x38 */ u32 calibCRC;
        /* 0x3c */ u16 calibID;
    } /* 0x1a */ mpls;
#endif
} WPADExtConfig;

typedef struct WPADControlBlock {
    /* 0x000 */ WPADMEMGameInfo gameInfo;
    /* 0x038 */ s32 gameInfoErr[2];
    /* 0x040 */ WPADUnkStatus prBuf[1];
    /* 0x0A0 */ WPADUnkStatus rxBufs[2];
    /* 0x160 */ WPADCmdQueue stdCmdQueue;
    /* 0x16C */ WPADCommand stdCmdQueueList[WPAD_COMMAND_CMD_MAX_LEN];
    /* 0x5EC */ WPADCmdQueue extCmdQueue;
    /* 0x5F8 */ WPADCommand extCmdQueueList[WPAD_COMMAND_EXT_MAX_LEN];
    /* 0x778 */ WPADInfo info;
    /* 0x790 */ WPADInfo* infoOut;
    /* 0x794 */ WPADDevConfig devConf;
    /* 0x7C4 */ WPADExtConfig extConf;
    /* 0x7DC */ WPADCallback cmdBlkCallback;
    /* 0x7E0 */ WPADExtensionCallback extensionCallback;
    /* 0x7E4 */ WPADConnectCallback connectCallback;
    /* 0x7E8 */ WPADSamplingCallback samplingCallback;
    /* 0x7EC */ void* samplingBufs_ptr;
    /* 0x7F0 */ u32 samplingBufIndex;
    /* 0x7F4 */ u32 samplingBufLength;
    /* 0x7F8 */ u32 dataFormat;
    /* 0x7FC */ s32 status;
    /* 0x800 */ u8 statusReqBusy;
    /* 0x801 */ u8 devType;
    /* 0x802 */ u8 devMode;
    /* 0x803 */ s8 devHandle;
#if SDK_AUG2010
    /* 0x908 */ int at_0x908; /* unknown */
    /* 0x90c */ u8 rxBufIndex;
    /* 0x90d */ s8 at_0x90d; /* unknown */
    /* 0x90e */ u8 dpdDummyObjSize; // maybe?
    /* 0x90f */ u8 currentDpdCommand;
    /* 0x910 */ u8 pendingDpdCommand;
#else
    /* 0x804 */ u16 dpdDummyObjSize;
    /* 0x806 */ u8 rxBufIndex;
    /* 0x807 */ u8 reqVolume;
    /* 0x808 */ WPADCallback reqVolCb;
#endif
    /* 0x80C */ u8 radioQuality;
    /* 0x80D */ u8 radioQualityOkMs; // see __wpadCalcRadioQuality
    /* 0x80E */ u8 audioFrames;
    /* 0x810 */ BOOL motorBusy;
    /* 0x814 */ BOOL motorRunning;
    /* 0x818 */ BOOL used;
    /* 0x81C */ BOOL handshakeFinished;
    /* 0x820 */ BOOL oldFw;
    /* 0x824 */ OSThreadQueue threadQueue;
#if SDK_AUG2010
    /* 0x930 */ WPADCallback vsmCallback;
    /* 0x934 */ u8 controlMplsBusy;
    /* 0x935 */ u8 mplsCBReadBuf[2];
    /* 0x937 */ u8 mplsCBCounter; // idk???
    /* 0x938 */ u8 pendingMplsCommand;
    /* 0x939 */ u8 noParseMplsCount;
    /* 0x93a */ u8 isInitingMpls; // maybe?
    /* 0x93b */ u8 hasReadExtType2; // maybe?
    /* 0x93c */ u8 at_0x93c; /* unknown */
    /* 0x93d */ u8 parseMPState;
    /* 0x93e */ u8 wmParamOffset;
    /* 0x93f */ u8 certWorkCounter; // idk???
    /* 0x940 */ u16 certWorkMs;
    /* 0x942 */ s16 certStateWorkMs;
    /* 0x944 */ s8 certChallengeRandomBit;
    /* 0x945 */ u8 certWorkBusy;
    /* 0x946 */ s8 certValidityStatus;
    /* 0x947 */ s8 certState;
    /* 0x948 */ u32* certParamPtr;
    /* 0x94c */ u32 certLintX[1 + 16 + 1];
    /* 0x994 */ u32 certLintY[1 + 16 + 1];
    /* 0x994 */ u32 certLintBig[LINT_NUM_MAX_BUFSIZ];
    /* 0xae4 */ int at_0xae4; /* unknown */
#endif
    /* 0x830 */ OSTime lastUpdateTime;
#if SDK_AUG2010
    /* 0xaf0 */ u16 filterDiff[WPAD_MAX_NZFILTERS];
    /* 0xaf8 */ u16 filterSame[WPAD_MAX_NZFILTERS];
#else
    /* 0x838 */ u16 diffCountDpd;
    /* 0x83A */ u16 hystCountDpd;
    /* 0x83C */ u16 diffCountAcc;
    /* 0x83E */ u16 hystCountAcc;
    /* 0x840 */ u16 diffCountfsAcc;
    /* 0x842 */ u16 hystCountfsAcc;
#endif
    /* 0x844 */ OSTime lastReportSendTime;
    /* 0x850 */ u8 cmdTimeoutAction;
    /* 0x851 */ u8 calibrated;
    /* 0x852 */ u16 recalibrateCount;
    /* 0x854 */ u8 key[16];
    /* 0x864 */ u8 ft[8];
    /* 0x86C */ u8 sb[8];
    /* 0x874 */ u8 wmReadDataBuf[64];
    /* 0x8B4 */ void* wmReadDataPtr;
    /* 0x8B8 */ u32 wmReadAddr;
    /* 0x8BC */ s32 wmReadErr;
    /* 0x8C0 */ u16 wmReadLength;
    /* 0x8C2 */ u8 keyIdx;
    /* 0x8C3 */ u8 radioSensitivity;
    /* 0x8C4 */ u16 packetCnt;
    /* 0x8C6 */ u8 disconnect;
    /* 0x8C7 */ u8 lastReportId;
#if SDK_AUG2010
    /* 0xB80 */ WPADCallback getInfoCB;
    /* 0xB84 */ u8 getInfoBusy;
    /* 0xB85 */ u8 extState;
    /* 0xB86 */ u8 savePower;
    /* 0xB87 */ u8 blcBattery;
    /* 0xB88 */ u8 savedDevType; // maybe?
    /* 0xB89 */ u8 extWasDisconnected;
    /* 0xB8A */ s16 reconnectExtMs;
    /* 0xB8C */ WPADMemBlock memBlock;
    /* 0xBA0 */ WPADCallback controlMplsCB;
    /* 0xBA4 */ u8 parseMPBuf;
    /* 0xBA5 */ u8 certProbeByte;
    /* 0xBA6 */ u8 dpdBusy;
    /* 0xBA7 */ u8 interleaveFlags;
    /* 0xBA8 */ u32 mplsCBReadAddress;
    /* 0xBAC */ u8 mplsCBState;
    /* 0xBAD */ u8 mplsUptimeMs;
    /* 0xBAE */ s8 certMayVerifyByCalibBlock;
    /* 0xBAF */ u8 unk_0xbaf[0xbb1 - 0xbaf]; /* unknown (can't be alignment) */
    /* 0xBB1 */ u8 certProbeStartingValue;
    /* 0xBB2 */ u16 lastMplsCalibID;
    /* 0xBB4 */ u32 lastMplsCalibCRC;
    /* 0xBB8 */ u8 noParseExtCount;
    /* 0xBB9 */ s8 extErr;
    /* 0xBBA */ u8 extDataLength;
    /* 0xBBB */ u8 extDevType;
    /* 0xBBC */ u8 currPwmDuty;
    /* 0xBBD */ u8 pendingPwmDuty;
    /* 0xBBE */ u8 unk_0xbbe[0xbc0 - 0xbbe]; /* unknown (can't be alignment) */
    /* 0xBC0 */ u8 extDataBuf[32];
#else
    /* 0x8C8 */ u8 extDataBuf[0x8e0 - 0x8c8];
#endif
} WPADControlBlock;

typedef struct WPADMEMControlBlock {
    /* 0x00 */ BOOL used;
    /* 0x04 */ u8* p_buf;
    /* 0x08 */ u16 len;
    /* 0x0C */ u32 addr;
    /* 0x10 */ WPADCallback callback;
} WPADMEMControlBlock;

struct WPADCmd {
    /* 0x00 */ u32 reportID;
    /* 0x04 */ u8 dataBuf[RPT_MAX_SIZE];
    /* 0x1a */ u16 dataLength;
    /* 0x1c */ void* dstBuf;
    /* 0x20 */ u16 readLength;
    /* 0x24 */ u32 readAddress;
    /* 0x28 */ WPADInfo* statusReportOut;
    /* 0x2c */ WPADCallback cmdCB;
};  // size 0x30

extern WPADMEMControlBlock _wmb[WPAD_MAX_CONTROLLERS];
#if SDK_AUG2010
extern WPADControlBlock* __rvl_p_wpadcb[WPAD_MAX_CONTROLLERS];
#define _wpdcb __rvl_p_wpadcb
#else
extern WPADControlBlock* _wpdcb[WPAD_MAX_CONTROLLERS];
#endif

/* WPAD.c */

void WPADiCreateKey(s32);
void WPADiCreateKeyFor3rd(s32);
s32 WPADiHIDParser(u8 chan, u8* p_rpt);

void WPADiClearQueue(WPADCmdQueue* queue);
BOOL WPADiSendSetReportType(WPADCmdQueue* queue, u32 fmt, WPADCallback callback);
BOOL WPADiSendWriteDataCmd(WPADCmdQueue* queue, u8 cmd, u32 addr, WPADCallback callback);
BOOL WPADiSendReadData(WPADCmdQueue* queue, void* p_buf, u16 len, u32 addr, WPADCallback callback);
BOOL WPADiSendWriteData(WPADCmdQueue* queue, void* p_buf, u16 len, u32 addr, WPADCallback callback);

void WPADiCopyOut(s32 chan);
void WPADiExcludeButton(s32 chan);
s32 WPADiGetStatus(s32 chan);

void WPADiShutdown();
void WPADiDisconnect(s32 chan, BOOL polite);

BOOL WPADiSendSetPort(WPADCmdQueue* queue, u8 pattern, WPADCallback callback);
BOOL WPADiSendDPDCSB(WPADCmdQueue* queue, BOOL enable, WPADCallback callback);
#if SDK_AUG2010
BOOL WPADiSendGetContStat(WPADCmdQueue* queue, WPADInfo* info, WPADCallback callback);
#else
BOOL WPADiSendGetContStat(WPADCmdQueue* queue, WPADInfo* info, u32 addr, WPADCallback callback);
#endif
void WPADiCheckContInputs(s32 chan);

s32 WPADiRetrieveChannel(u8 dev_handle);
s32 WPADiSendData(s32 chan, WPADCommand cmd);

BOOL WPADiGetCommand(WPADCmdQueue* queue, WPADCommand* cmd);
BOOL WPADiPopCommand(WPADCmdQueue* queue);
BOOL __wpadPushCommand(WPADCmdQueue* queue, WPADCommand cmd);
BOOL WPADiProcessCommand(s32 chan);
BOOL WPADiProcessExtCommand(s32 chan);
void WPADiRumbleMotor(s32 chan, BOOL sendCmd);
void WPADiRadioSensitivity(s32 chan);

void WPADiContMapTableUpdate(void);
void WPADiAfh(void);
void WPADiGetScSettings(void);

void WPADiConnCallback(u8 dev_handle, u8 open);
void WPADiRecvCallback(u8 dev_handle, u8* p_rpt, u16);

/* WPADEncrypt.c */
void WPADiCreateKey(s32 chan);
void WPADiDecode(s32 chan, void* p_buf, u16 len, u16 offset);

/* wpad_debug_msg.c */
void DEBUGPrint(const char* fmt, ...);

#ifdef __cplusplus
}
#endif

#endif  // _REVOLUTION_WPAD_PRIVATE_H_
