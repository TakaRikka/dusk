#include <dolphin/dolphin.h>
#include <dolphin/os.h>
#include <dolphin/dvd.h>

#include "os/__os.h"
#include "__dvd.h"

static u32 ErrorTable[18] = {
    0x00000000,
    0x00023A00,
    0x00062800,
    0x00030200,
    0x00031100,
    0x00052000,
    0x00052001,
    0x00052100,
    0x00052400,
    0x00052401,
    0x00052402,
    0x000B5A01,
    0x00056300,
    0x00020401,
    0x00020400,
    0x00040800,
    0x00100007,
    0x00000000,
};

#define DIDNT_MATCH 29

static u8 ErrorCode2Num(u32 errorCode) {
	u32 i;

	for (i = 0; i < 18; i++) {
		if (errorCode == ErrorTable[i]) {
            ASSERTLINE(73, i < DIDNT_MATCH);
			return i;
		}
	}

	if (errorCode >= 0x100000 && errorCode <= 0x100008) {
		return 17;
	}

	return DIDNT_MATCH;
}

static u8 Convert(u32 error) {
    u32 statusCode;
    u32 errorCode;
    u8 errorNum;

    if (error == 0x01234567) {
        return -1;
    } else if (error == 0x01234568) {
        return -2;
    }

    statusCode = (error >> 24) & 0xFF;
    errorCode = error & 0x00FFFFFF;
    errorNum = ErrorCode2Num(errorCode);
    if (statusCode >= 6) {
        statusCode = 6;
    }

    return statusCode * 30 + errorNum;
}

void __DVDStoreErrorCode(u32 error) {
	OSSramEx* sram;
    u8 num;

    num = Convert(error);
	sram = __OSLockSramEx();
	sram->dvdErrorCode = num;
	__OSUnlockSramEx(TRUE);
}
