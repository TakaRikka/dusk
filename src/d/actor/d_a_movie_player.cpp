/**
 * @file d_a_movie_player.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

// This TU seems to disable inlining entirely, as there are several weak functions that get inlined
// in other TUs, but not here.
#pragma dont_inline on

// Unsure about the optimization level here. Shield looks like it definitely uses O4,p for this TU,
// but gamecube has functions that imply O3,p and others that match better on O4,p.
#pragma optimization_level 4
#pragma optimize_for_size off

#include "JSystem/JKernel/JKRExpHeap.h"
#include "JSystem/JAudio2/JASAiCtrl.h"
#include "JSystem/JAudio2/JASDriverIF.h"
#include "d/actor/d_a_movie_player.h"
#include "Z2AudioLib/Z2Instances.h"
#include "f_op/f_op_overlap_mng.h"

inline s32 daMP_NEXT_READ_SIZE(daMP_THPReadBuffer* readBuf) {
    return *(s32*)readBuf->ptr;
}

#ifdef __cplusplus
extern "C" {
#endif

static u32 THPAudioDecode(s16* audioBuffer, u8* audioFrame, s32 flag) {
    THPAudioRecordHeader* header;
    THPAudioDecodeInfo decInfo;
    u8 *left, *right;
    s16 *decLeftPtr, *decRightPtr;
    s16 yn1, yn2;
    s32 i;
    s32 step;
    s32 sample;
    s64 yn;

    if (audioBuffer == NULL || audioFrame == NULL) {
        return 0;
    }

    header = (THPAudioRecordHeader*)audioFrame;
    left = audioFrame + sizeof(THPAudioRecordHeader);
    right = left + header->offsetNextChannel;

    if (flag == 1) {
        decRightPtr = audioBuffer;
        decLeftPtr = audioBuffer + header->sampleSize;
        step = 1;
    } else {
        decRightPtr = audioBuffer;
        decLeftPtr = audioBuffer + 1;
        step = 2;
    }

    if (header->offsetNextChannel == 0) {
        __THPAudioInitialize(&decInfo, left);

        yn1 = header->lYn1;
        yn2 = header->lYn2;

        for (i = 0; i < header->sampleSize; i++) {
            sample = __THPAudioGetNewSample(&decInfo);
            yn = header->lCoef[decInfo.predictor][1] * yn2;
            yn += header->lCoef[decInfo.predictor][0] * yn1;
            yn += (sample << decInfo.scale) << 11;
            yn <<= 5;

            u16 temp = yn & 0xffff;
            if (temp > 0x8000) {
                yn += 0x10000;
            } else if (temp == 0x8000 && (yn & 0x10000)) {
                yn += 0x10000;
            }

            if (yn > 2147483647LL) {
                yn = 2147483647LL;
            }

            if (yn < -2147483648LL) {
                yn = -2147483648LL;
            }

            *decLeftPtr = (s16)(yn >> 16);
            decLeftPtr += step;
            *decRightPtr = (s16)(yn >> 16);
            decRightPtr += step;
            yn2 = yn1;
            yn1 = (s16)(yn >> 16);
        }
    } else {
        __THPAudioInitialize(&decInfo, left);

        yn1 = header->lYn1;
        yn2 = header->lYn2;

        for (i = 0; i < header->sampleSize; i++) {
            sample = __THPAudioGetNewSample(&decInfo);
            yn = header->lCoef[decInfo.predictor][1] * yn2;
            yn += header->lCoef[decInfo.predictor][0] * yn1;
            yn += (sample << decInfo.scale) << 11;
            yn <<= 5;
            
            u16 temp = yn & 0xffff;
            if (temp > 0x8000) {
                yn += 0x10000;
            } else if (temp == 0x8000 && (yn & 0x10000)) {
                yn += 0x10000;
            }

            if (yn > 2147483647LL) {
                yn = 2147483647LL;
            }

            if (yn < -2147483648LL) {
                yn = -2147483648LL;
            }

            *decLeftPtr = (s16)(yn >> 16);
            decLeftPtr += step;
            yn2 = yn1;
            yn1 = (s16)(yn >> 16);
        }

        __THPAudioInitialize(&decInfo, right);

        yn1 = header->rYn1;
        yn2 = header->rYn2;

        for (i = 0; i < header->sampleSize; i++) {
            sample = __THPAudioGetNewSample(&decInfo);
            yn = header->rCoef[decInfo.predictor][1] * yn2;
            yn += header->rCoef[decInfo.predictor][0] * yn1;
            yn += (sample << decInfo.scale) << 11;
            yn <<= 5;

            u16 temp = yn & 0xffff;
            if (temp > 0x8000) {
                yn += 0x10000;
            } else if (temp == 0x8000 && (yn & 0x10000)) {
                yn += 0x10000;
            }

            if (yn > 2147483647LL) {
                yn = 2147483647LL;
            }

            if (yn < -2147483648LL) {
                yn = -2147483648LL;
            }

            *decRightPtr = (s16)(yn >> 16);
            decRightPtr += step;
            yn2 = yn1;
            yn1 = (s16)(yn >> 16);
        }
    }

    return header->sampleSize;
}

static s32 __THPAudioGetNewSample(THPAudioDecodeInfo* info) {
    s32 sample;

    if (!(info->offsetNibbles & 0x0f)) {
        info->predictor = (u8)((*(info->encodeData) & 0x70) >> 4);
        info->scale = (u8)((*(info->encodeData) & 0xF));
        info->encodeData++;
        info->offsetNibbles += 2;
    }

    if (info->offsetNibbles & 0x1) {
        sample = (s32)((*(info->encodeData) & 0xF) << 28) >> 28;
        info->encodeData++;
    } else {
        sample = (s32)((*(info->encodeData) & 0xF0) << 24) >> 28;
    }

    info->offsetNibbles++;
    return sample;
}

static void __THPAudioInitialize(THPAudioDecodeInfo* info, u8* ptr) {
    info->encodeData = ptr;
    info->offsetNibbles = 2;
    info->predictor = (u8)((*(info->encodeData) & 0x70) >> 4);
    info->scale = (u8)((*(info->encodeData) & 0xF));
    info->encodeData++;
}

static u8 THPStatistics[1120] ATTRIBUTE_ALIGN(32);

static THPHuffmanTab* Ydchuff ATTRIBUTE_ALIGN(32);

static THPHuffmanTab* Udchuff ATTRIBUTE_ALIGN(32);

static THPHuffmanTab* Vdchuff ATTRIBUTE_ALIGN(32);

static THPHuffmanTab* Yachuff ATTRIBUTE_ALIGN(32);

static THPHuffmanTab* Uachuff ATTRIBUTE_ALIGN(32);

static THPHuffmanTab* Vachuff ATTRIBUTE_ALIGN(32);

static f32 __THPIDCTWorkspace[64] ATTRIBUTE_ALIGN(32);

static u8* __THPHuffmanBits;

static u8* __THPHuffmanSizeTab;

static u16* __THPHuffmanCodeTab;

static THPSample* Gbase ATTRIBUTE_ALIGN(32);

static u32 Gwid ATTRIBUTE_ALIGN(32);

static f32* Gq ATTRIBUTE_ALIGN(32);

static u8* __THPLCWork512[3];

static u8* __THPLCWork640[3];

static u32 __THPOldGQR5;

static u32 __THPOldGQR6;

static u8* __THPWorkArea;

static THPCoeff* __THPMCUBuffer[6];

static THPFileInfo* __THPInfo;

static BOOL __THPInitFlag;

static s32 THPVideoDecode(void* file, void* tileY, void* tileU, void* tileV, void* work) {
    u8 all_done, status;
    s32 errorCode;

    if (!file) {
        goto _err_no_input;
    }

    if (tileY == NULL || tileU == NULL || tileV == NULL) {
        goto _err_no_output;
    }

    if (!work) {
        goto _err_no_work;
    }

    if (!(PPCMfhid2() & 0x10000000)) {
        goto _err_lc_not_enabled;
    }

    if (__THPInitFlag == FALSE) {
        goto _err_not_initialized;
    }

    __THPWorkArea = (u8*)work;
    __THPInfo = (THPFileInfo*)OSRoundUp32B(__THPWorkArea);
    __THPWorkArea = (u8*)OSRoundUp32B(__THPWorkArea) + sizeof(THPFileInfo);
    DCZeroRange(__THPInfo, sizeof(THPFileInfo));
    __THPInfo->cnt = 33;
    __THPInfo->decompressedY = 0;
    __THPInfo->c = (u8*)file;
    all_done = FALSE;

    for (;;) {
        if ((*(__THPInfo->c)++) != 255) {
            goto _err_bad_syntax;
        }

        while (*__THPInfo->c == 255) {
            ((__THPInfo->c)++);
        }

        status = (*(__THPInfo->c)++);

        if (status <= 0xD7) {
            if (status == 196) {
                status = __THPReadHuffmanTableSpecification();
                if (status != 0) {
                    goto _err_bad_status;
                }
            }

            else if (status == 192) {
                status = __THPReadFrameHeader();
                if (status != 0) {
                    goto _err_bad_status;
                }
            }

            else {
                goto _err_unsupported_marker;
            }
        }

        else if (0xD8 <= status && status <= 0xDF) {
            if (status == 221) {
                __THPRestartDefinition();
            }

            else if (status == 219) {
                status = __THPReadQuantizationTable();
                if (status != 0) {
                    goto _err_bad_status;
                }
            }

            else if (status == 218) {
                status = __THPReadScaneHeader();
                if (status != 0) {
                    goto _err_bad_status;
                }

                all_done = TRUE;
            } else if (status == 216) {
                // empty but required for match
            } else {
                goto _err_unsupported_marker;
            }
        }

        else if (0xE0 <= status) {
            if ((224 <= status && status <= 239) || status == 254) {
                __THPInfo->c += (__THPInfo->c)[0] << 8 | (__THPInfo->c)[1];
            } else {
                goto _err_unsupported_marker;
            }
        }

        if (all_done) {
            break;
        }
    }

    __THPSetupBuffers();
    __THPDecompressYUV(tileY, tileU, tileV);
    return 0;

_err_no_input:
    errorCode = 25;
    goto _err_exit;

_err_no_output:
    errorCode = 27;
    goto _err_exit;

_err_no_work:
    errorCode = 26;
    goto _err_exit;

_err_unsupported_marker:
    errorCode = 11;
    goto _err_exit;

_err_bad_resource:
    errorCode = 1;
    goto _err_exit;

_err_no_mem:
    errorCode = 6;
    goto _err_exit;

_err_bad_syntax:
    errorCode = 3;
    goto _err_exit;

_err_bad_status:
    errorCode = status;
    goto _err_exit;

_err_lc_not_enabled:
    errorCode = 28;
    goto _err_exit;

_err_not_initialized:
    errorCode = 29;
    goto _err_exit;

_err_exit:
    return errorCode;
}

static void __THPSetupBuffers() {
    u8 i;
    THPCoeff* buffer;

    buffer = (THPCoeff*)OSRoundUp32B(__THPWorkArea);

    for (i = 0; i < 6; i++) {
        __THPMCUBuffer[i] = &buffer[i * 64];
    }
}

static u8 __THPReadFrameHeader() {
    u8 i, utmp8;

    __THPInfo->c += 2;

    utmp8 = (*(__THPInfo->c)++);

    if (utmp8 != 8) {
        return 10;
    }

    __THPInfo->yPixelSize = (u16)((__THPInfo->c)[0] << 8 | (__THPInfo->c)[1]);
    __THPInfo->c += 2;
    __THPInfo->xPixelSize = (u16)((__THPInfo->c)[0] << 8 | (__THPInfo->c)[1]);
    __THPInfo->c += 2;

    utmp8 = (*(__THPInfo->c)++);
    if (utmp8 != 3) {
        return 12;
    }

    for (i = 0; i < 3; i++) {
        utmp8 = (*(__THPInfo->c)++);
        utmp8 = (*(__THPInfo->c)++);
        if ((i == 0 && utmp8 != 0x22) || (i > 0 && utmp8 != 0x11)) {
            return 19;
        }

        __THPInfo->components[i].quantizationTableSelector = (*(__THPInfo->c)++);
    }

    return 0;
}

#define THPROUNDUP(a, b) ((((s32)(a)) + ((s32)(b)-1L)) / ((s32)(b)))

static u8 __THPReadScaneHeader() {
    u8 i, utmp8;
    __THPInfo->c += 2;

    utmp8 = (*(__THPInfo->c)++);

    if (utmp8 != 3) {
        return 12;
    }

    for (i = 0; i < 3; i++) {
        utmp8 = (*(__THPInfo->c)++);

        utmp8 = (*(__THPInfo->c)++);
        __THPInfo->components[i].DCTableSelector = (u8)(utmp8 >> 4);
        __THPInfo->components[i].ACTableSelector = (u8)(utmp8 & 15);

        if ((__THPInfo->validHuffmanTabs & (1 << ((utmp8 >> 4)))) == 0) {
            return 15;
        }

        if ((__THPInfo->validHuffmanTabs & (1 << ((utmp8 & 15) + 1))) == 0) {
            return 15;
        }
    }

    __THPInfo->c += 3;
    __THPInfo->MCUsPerRow = (u16)THPROUNDUP(__THPInfo->xPixelSize, 16);
    __THPInfo->components[0].predDC = 0;
    __THPInfo->components[1].predDC = 0;
    __THPInfo->components[2].predDC = 0;
    return 0;
}

static const u8 __THPJpegNaturalOrder[80] = {
    0,  1,  8,  16, 9,  2,  3,  10, 17, 24, 32, 25, 18, 11, 4,  5,  12, 19, 26, 33,
    40, 48, 41, 34, 27, 20, 13, 6,  7,  14, 21, 28, 35, 42, 49, 56, 57, 50, 43, 36,
    29, 22, 15, 23, 30, 37, 44, 51, 58, 59, 52, 45, 38, 31, 39, 46, 53, 60, 61, 54,
    47, 55, 62, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63,
};

static const f64 __THPAANScaleFactor[8] = {
    1.0f, 1.387039845f, 1.306562965f, 1.175875602f, 1.0f, 0.785694958f, 0.541196100f, 0.275899379f,
};

static u8 __THPReadQuantizationTable() {
    int length;
    u16 id, i, row, col;
	f32 q_temp[64];

	length = ((__THPInfo->c)[0] << 8 | (__THPInfo->c)[1]) & 0xffff;
	__THPInfo->c += 2;
	length -= 2;

	for (;;) {
		id = (*(__THPInfo->c)++);

		for (i = 0; i < 64; i++) {
			q_temp[__THPJpegNaturalOrder[i]] = (f32)(*(__THPInfo->c)++);
		}

		i = 0;
		for (row = 0; row < 8; row++) {
			for (col = 0; col < 8; col++) {
				__THPInfo->quantTabs[id][i]
				    = (f32)((f64)q_temp[i] * __THPAANScaleFactor[row]
				            * __THPAANScaleFactor[col]);
				i++;
			}
		}

		length -= 65;
		if ((u16)length == 0) {
			break;
		}
	}

	return 0;
}

static u8 __THPReadHuffmanTableSpecification() {
    u8 t_class, id, i, tab_index;
    u16 length, num_Vij;

    __THPHuffmanSizeTab = __THPWorkArea;
    __THPHuffmanCodeTab = (u16*)((u32)__THPWorkArea + 256 + 1);
    length = (u16)((__THPInfo->c)[0] << 8 | (__THPInfo->c)[1]);
    __THPInfo->c += 2;
    length -= 2;

    for (;;) {
        i = (*(__THPInfo->c)++);
        id = (u8)(i & 15);
        t_class = (u8)(i >> 4);
        __THPHuffmanBits = __THPInfo->c;
        tab_index = (u8)((id << 1) + t_class);
        num_Vij = 0;

        for (i = 0; i < 16; i++) {
            num_Vij += (*(__THPInfo->c)++);
        }

        __THPInfo->huffmanTabs[tab_index].Vij = __THPInfo->c;
        __THPInfo->c += num_Vij;
        __THPHuffGenerateSizeTable();
        __THPHuffGenerateCodeTable();
        __THPHuffGenerateDecoderTables(tab_index);
        __THPInfo->validHuffmanTabs |= 1 << tab_index;
        length -= 17 + num_Vij;

        if (length == 0) {
            break;
        }
    }

    return 0;
}

static void __THPHuffGenerateSizeTable() {
    s32 p, l, i;
    p = 0;

    for (l = 1; l <= 16; l++) {
        i = (s32)__THPHuffmanBits[l - 1];
        while (i--) {
            __THPHuffmanSizeTab[p++] = (u8)l;
        }
    }

    __THPHuffmanSizeTab[p] = 0;
}

static void __THPHuffGenerateCodeTable() {
    u8 si;
    u16 p, code;

    p = 0;
    code = 0;
    si = __THPHuffmanSizeTab[0];

    while (__THPHuffmanSizeTab[p]) {
        while (__THPHuffmanSizeTab[p] == si) {
            __THPHuffmanCodeTab[p++] = code;
            code++;
        }

        code <<= 1;
        si++;
    }
}

static void __THPHuffGenerateDecoderTables(u8 tabIndex) {
    s32 p, l;
    THPHuffmanTab* h;

    p = 0;
    h = &__THPInfo->huffmanTabs[tabIndex];
    for (l = 1; l <= 16; l++) {
        if (__THPHuffmanBits[l - 1]) {
            h->valPtr[l] = p - __THPHuffmanCodeTab[p];
            p += __THPHuffmanBits[l - 1];
            h->maxCode[l] = __THPHuffmanCodeTab[p - 1];
        } else {
            h->maxCode[l] = -1;
            h->valPtr[l]  = -1;
        }
    }

    h->maxCode[17] = 0xfffffL;
}

static void __THPRestartDefinition() {
    __THPInfo->RST = TRUE;
    __THPInfo->c += 2;
    __THPInfo->nMCU = (u16)((__THPInfo->c)[0] << 8 | (__THPInfo->c)[1]);
    __THPInfo->c += 2;
    __THPInfo->currMCU = __THPInfo->nMCU;
}

static void __THPPrepBitStream() {
    u32* ptr;
    u32 offset, i, j, k;

    ptr = (u32*)((u32)__THPInfo->c & 0xFFFFFFFC);
    offset = (u32)__THPInfo->c & 3;
    ASSERTLINE(3799, __THPInfo->cnt <= 33);

    if (__THPInfo->cnt != 33) {
        __THPInfo->cnt -= (3 - offset) * 8;
    } else {
        __THPInfo->cnt = (offset * 8) + 1;
    }

    ASSERTLINE(3810, __THPInfo->cnt <= 33);

    __THPInfo->c = (u8*)ptr;
    __THPInfo->currByte = *ptr;

    for (i = 0; i < 4; i++) {
        if (__THPInfo->validHuffmanTabs & (1 << i)) {
            for (j = 0; j < 32; j++) {
                __THPInfo->huffmanTabs[i].quick[j] = 0xFF;

                for (k = 0; k < 5; k++) {
                    s32 code = (s32)(j >> (5 - k - 1));

                    if (code <= __THPInfo->huffmanTabs[i].maxCode[k + 1]) {
                        __THPInfo->huffmanTabs[i].quick[j]
                            = __THPInfo->huffmanTabs[i].Vij[(s32)(code + __THPInfo->huffmanTabs[i].valPtr[k + 1])];
                        __THPInfo->huffmanTabs[i].increment[j] = (u8)(k + 1);
                        k                                      = 99;
                    } else {
                    }
                }
            }
        }
    }

    {
        s32 YdcTab, UdcTab, VdcTab, YacTab, UacTab, VacTab;

        YdcTab = (__THPInfo->components[0].DCTableSelector << 1);
        UdcTab = (__THPInfo->components[1].DCTableSelector << 1);
        VdcTab = (__THPInfo->components[2].DCTableSelector << 1);

        YacTab = (__THPInfo->components[0].ACTableSelector << 1) + 1;
        UacTab = (__THPInfo->components[1].ACTableSelector << 1) + 1;
        VacTab = (__THPInfo->components[2].ACTableSelector << 1) + 1;

        Ydchuff = &__THPInfo->huffmanTabs[YdcTab];
        Udchuff = &__THPInfo->huffmanTabs[UdcTab];
        Vdchuff = &__THPInfo->huffmanTabs[VdcTab];

        Yachuff = &__THPInfo->huffmanTabs[YacTab];
        Uachuff = &__THPInfo->huffmanTabs[UacTab];
        Vachuff = &__THPInfo->huffmanTabs[VacTab];
    }
}

static void __THPDecompressYUV(void* tileY, void* tileU, void* tileV) {
    u16 currentY, targetY;
    __THPInfo->dLC[0] = (u8*)tileY;
    __THPInfo->dLC[1] = (u8*)tileU;
    __THPInfo->dLC[2] = (u8*)tileV;

    currentY = __THPInfo->decompressedY;
    targetY = __THPInfo->yPixelSize;

    __THPGQRSetup();
    __THPPrepBitStream();

    if (__THPInfo->xPixelSize == 512 && targetY == 448) {
        while (currentY < targetY) {
            __THPDecompressiMCURow512x448();
            currentY += (u16)16;
        }
    } else if (__THPInfo->xPixelSize == 640 && targetY == 480) {
        while (currentY < targetY) {
            __THPDecompressiMCURow640x480();
            currentY += (u16)16;
        }
    } else {
        while (currentY < targetY) {
            __THPDecompressiMCURowNxN();
            currentY += (u16)16;
        }
    }

    __THPGQRRestore();
}

static void __THPGQRRestore() {
#ifdef __MWERKS__
    register u32 tmp1, tmp2;
    tmp1 = __THPOldGQR5;
    tmp2 = __THPOldGQR6;

    // clang-format off
    asm {
        mtspr   GQR5, tmp1;
        mtspr   GQR6, tmp2;
    }
#endif
    // clang-format on
}

static void __THPGQRSetup() {
#ifdef __MWERKS__
    register u32 tmp1, tmp2;

    // clang-format off
    asm {
        mfspr   tmp1, GQR5;
        mfspr   tmp2, GQR6;
    }

    __THPOldGQR5 = tmp1;
    __THPOldGQR6 = tmp2;

    asm {
        li      r3, 0x0007
        oris    r3, r3, 0x0007
        mtspr   GQR5, r3
        li      r3, 0x3D04
        oris    r3, r3, 0x3D04
        mtspr   GQR6, r3
    }
#endif
    // clang-format on
}

static void __THPDecompressiMCURow512x448() {
    u8 cl_num;
    u32 x_pos;
    THPComponent* comp;

    LCQueueWait(3);

    for (cl_num = 0; cl_num < __THPInfo->MCUsPerRow; cl_num++) {
        __THPHuffDecodeDCTCompY(__THPInfo, __THPMCUBuffer[0]);
        __THPHuffDecodeDCTCompY(__THPInfo, __THPMCUBuffer[1]);
        __THPHuffDecodeDCTCompY(__THPInfo, __THPMCUBuffer[2]);
        __THPHuffDecodeDCTCompY(__THPInfo, __THPMCUBuffer[3]);
        __THPHuffDecodeDCTCompU(__THPInfo, __THPMCUBuffer[4]);
        __THPHuffDecodeDCTCompV(__THPInfo, __THPMCUBuffer[5]);

        comp = &__THPInfo->components[0];
        Gbase = __THPLCWork512[0];
        Gwid = 512;
        Gq = __THPInfo->quantTabs[comp->quantizationTableSelector];
        x_pos = (u32)(cl_num * 16);
        __THPInverseDCTNoYPos(__THPMCUBuffer[0], x_pos);
        __THPInverseDCTNoYPos(__THPMCUBuffer[1], x_pos + 8);
        __THPInverseDCTY8(__THPMCUBuffer[2], x_pos);
        __THPInverseDCTY8(__THPMCUBuffer[3], x_pos + 8);

        comp = &__THPInfo->components[1];
        Gbase = __THPLCWork512[1];
        Gwid = 256;
        Gq = __THPInfo->quantTabs[comp->quantizationTableSelector];
        x_pos /= 2;
        __THPInverseDCTNoYPos(__THPMCUBuffer[4], x_pos);
        comp = &__THPInfo->components[2];
        Gbase = __THPLCWork512[2];
        Gq = __THPInfo->quantTabs[comp->quantizationTableSelector];
        __THPInverseDCTNoYPos(__THPMCUBuffer[5], x_pos);

        if (__THPInfo->RST != 0) {
            if ((--__THPInfo->currMCU) == 0) {
                __THPInfo->currMCU = __THPInfo->nMCU;
                __THPInfo->cnt = 1 + ((__THPInfo->cnt + 6) & 0xFFFFFFF8);

                if (__THPInfo->cnt > 33) {
                    __THPInfo->cnt = 33;
                }

                __THPInfo->components[0].predDC = 0;
                __THPInfo->components[1].predDC = 0;
                __THPInfo->components[2].predDC = 0;
            }
        }
    }

    LCStoreData(__THPInfo->dLC[0], __THPLCWork512[0], 0x2000);
    LCStoreData(__THPInfo->dLC[1], __THPLCWork512[1], 0x800);
    LCStoreData(__THPInfo->dLC[2], __THPLCWork512[2], 0x800);

    __THPInfo->dLC[0] += 0x2000;
    __THPInfo->dLC[1] += 0x800;
    __THPInfo->dLC[2] += 0x800;
}

static void __THPInverseDCTY8(__REGISTER THPCoeff* in, __REGISTER u32 xPos) {
#ifdef __MWERKS__
    register f32 *q, *ws;
    register f32 tmp0, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7, tmp8, tmp9;
    register f32 tmp10, tmp11, tmp12, tmp13;
    register f32 tmp20, tmp21, tmp22, tmp23;
    register f32 cc4 = 1.414213562F;
    register f32 cc2 = 1.847759065F;
    register f32 cc2c6s = 1.082392200F;
    register f32 cc2c6a = -2.613125930F;
    register f32 bias = 1024.0F;

    q = Gq;
    ws = &__THPIDCTWorkspace[0] - 2;

    {
        register u32 itmp0, itmp1, itmp2, itmp3;

        // clang-format off
        asm {
            li          itmp2, 8
            mtctr       itmp2

        _loopHead0:
            psq_l       tmp10, 0(in), 0, 5
            psq_l       tmp11, 0(q), 0, 0
            lwz         itmp0, 12(in)
            lwz         itmp3, 8(in)
            ps_mul      tmp10, tmp10, tmp11
            lwz         itmp1, 4(in)
            lhz         itmp2, 2(in)
            or          itmp0, itmp0, itmp3

        _loopHead1:
            cmpwi       itmp0, 0
            bne         _regularIDCT
            ps_merge00  tmp0, tmp10, tmp10
            cmpwi       itmp1, 0
            psq_st      tmp0, 8(ws), 0, 0
            bne         _halfIDCT
            psq_st      tmp0, 16(ws), 0, 0
            cmpwi       itmp2, 0
            psq_st      tmp0, 24(ws), 0, 0
            bne         _quarterIDCT
            addi        q, q, 8*sizeof(f32)
            psq_stu     tmp0, 32(ws), 0, 0
            addi        in, in, 8*sizeof(THPCoeff)
            bdnz        _loopHead0
            b           _loopEnd

        _quarterIDCT:
            ps_msub     tmp2, tmp10, cc2, tmp10
            addi        in, in, 8*sizeof(THPCoeff)
            ps_merge00  tmp9, tmp10, tmp10
            addi        q, q, 8*sizeof(f32)
            ps_sub      tmp1, cc2, cc2c6s
            lwz         itmp1, 4(in)
            ps_msub     tmp3, tmp10, cc4, tmp2
            lhz         itmp2, 2(in)
            ps_merge11  tmp5, tmp10, tmp2
            psq_l       tmp11, 0(q), 0, 0
            ps_nmsub    tmp4, tmp10, tmp1, tmp3
            ps_add      tmp7, tmp9, tmp5
            psq_l       tmp10, 0(in), 0, 5
            ps_merge11  tmp6, tmp3, tmp4
            ps_sub      tmp5, tmp9, tmp5
            lwz         itmp0, 12(in)
            ps_add      tmp8, tmp9, tmp6
            lwz         itmp3, 8(in)
            ps_sub      tmp6, tmp9, tmp6
            psq_stu     tmp7, 8(ws), 0, 0
            ps_merge10  tmp6, tmp6, tmp6
            psq_stu     tmp8, 8(ws), 0, 0
            ps_merge10  tmp5, tmp5, tmp5
            or          itmp0, itmp0, itmp3
            psq_stu     tmp6, 8(ws), 0, 0
            ps_mul      tmp10, tmp10, tmp11
            psq_stu     tmp5, 8(ws), 0, 0
            bdnz        _loopHead1
            b           _loopEnd

        _halfIDCT:
            psq_l       tmp1, 4(in), 0, 5
            psq_l       tmp9, 8(q), 0, 0
            addi        in, in, 8*sizeof(THPCoeff)
            ps_mul      tmp1, tmp1, tmp9
            addi        q, q, 8*sizeof(f32)
            ps_sub      tmp3, tmp10, tmp1
            ps_add      tmp2, tmp10, tmp1
            lwz         itmp0, 12(in)
            ps_madd     tmp4, tmp1, cc4, tmp3
            ps_nmsub    tmp5, tmp1, cc4, tmp2
            ps_mul      tmp8, tmp3, cc2
            ps_merge00  tmp4, tmp2, tmp4
            lwz         itmp3, 8(in)
            ps_nmsub    tmp6, tmp1, cc2c6a, tmp8
            ps_merge00  tmp5, tmp5, tmp3
            lwz         itmp1, 4(in)
            ps_sub      tmp6, tmp6, tmp2
            ps_nmsub    tmp7, tmp10, cc2c6s, tmp8
            lhz         itmp2, 2(in)
            ps_merge11  tmp2, tmp2, tmp6
            ps_msub     tmp8, tmp3, cc4, tmp6
            psq_l       tmp10, 0(in), 0, 5
            ps_add      tmp9, tmp4, tmp2
            ps_sub      tmp7, tmp7, tmp8
            psq_l       tmp11, 0(q), 0, 0
            ps_merge11  tmp3, tmp8, tmp7
            ps_sub      tmp4, tmp4, tmp2
            psq_stu     tmp9, 8(ws), 0, 0
            ps_add      tmp0, tmp5, tmp3
            ps_sub      tmp1, tmp5, tmp3
            or          itmp0, itmp0, itmp3
            psq_stu     tmp0, 8(ws), 0, 0
            ps_merge10  tmp1, tmp1, tmp1
            ps_merge10  tmp4, tmp4, tmp4
            psq_stu     tmp1, 8(ws), 0, 0
            ps_mul      tmp10, tmp10, tmp11
            psq_stu     tmp4, 8(ws), 0, 0
            bdnz        _loopHead1
            b           _loopEnd

        _regularIDCT:
            psq_l       tmp9, 4(in), 0, 5
            psq_l       tmp5, 8(q), 0, 0
            ps_mul      tmp9, tmp9, tmp5
            psq_l       tmp2, 8(in), 0, 5
            psq_l       tmp6, 16(q), 0, 0
            ps_merge01  tmp0, tmp10, tmp9
            psq_l       tmp3, 12(in), 0, 5
            ps_merge01  tmp1, tmp9, tmp10
            psq_l       tmp7, 24(q), 0, 0
            addi        in, in, 8*sizeof(THPCoeff)
            ps_madd     tmp4, tmp2, tmp6, tmp0
            ps_nmsub    tmp5, tmp2, tmp6, tmp0
            ps_madd     tmp6, tmp3, tmp7, tmp1
            ps_nmsub    tmp7, tmp3, tmp7, tmp1
            addi        q, q, 8*sizeof(f32)
            ps_add      tmp0, tmp4, tmp6
            ps_sub      tmp3, tmp4, tmp6
            ps_msub     tmp2, tmp7, cc4, tmp6
            lwz         itmp0, 12(in)
            ps_sub      tmp8, tmp7, tmp5
            ps_add      tmp1, tmp5, tmp2
            ps_sub      tmp2, tmp5, tmp2
            ps_mul      tmp8, tmp8, cc2
            lwz         itmp3, 8(in)
            ps_merge00  tmp1, tmp0, tmp1
            ps_nmsub    tmp6, tmp5, cc2c6a, tmp8
            ps_msub     tmp4, tmp7, cc2c6s, tmp8
            lwz         itmp1, 4(in)
            ps_sub      tmp6, tmp6, tmp0
            ps_merge00  tmp2, tmp2, tmp3
            lhz         itmp2, 2(in)
            ps_madd     tmp5, tmp3, cc4, tmp6
            ps_merge11  tmp7, tmp0, tmp6
            psq_l       tmp10, 0(in), 0, 5
            ps_sub      tmp4, tmp4, tmp5
            ps_add      tmp3, tmp1, tmp7
            psq_l       tmp11, 0(q), 0, 0
            ps_merge11  tmp4, tmp5, tmp4
            ps_sub      tmp0, tmp1, tmp7
            ps_mul      tmp10, tmp10, tmp11
            ps_add      tmp5, tmp2, tmp4
            ps_sub      tmp6, tmp2, tmp4
            ps_merge10  tmp5, tmp5, tmp5
            psq_stu     tmp3, 8(ws), 0, 0
            ps_merge10  tmp0, tmp0, tmp0
            psq_stu     tmp6, 8(ws), 0, 0
            psq_stu     tmp5, 8(ws), 0, 0
            or          itmp0, itmp0, itmp3
            psq_stu     tmp0, 8(ws), 0, 0
            bdnz        _loopHead1

        _loopEnd:

        }
        // clang-format on
    }

    ws = &__THPIDCTWorkspace[0];

    {
        register THPSample* obase = Gbase;
        register u32 wid = Gwid;

        register u32 itmp0, off0, off1;
        register THPSample *out0, *out1;

        // clang-format off
        asm {
            psq_l       tmp10, 8*0*sizeof(f32)(ws), 0, 0
            slwi off0, wid, 3;
            psq_l       tmp11, 8*4*sizeof(f32)(ws), 0, 0
            slwi        xPos, xPos, 2
            psq_l       tmp12, 8*2*sizeof(f32)(ws), 0, 0
            slwi        off1, wid, 2
            ps_add      tmp6, tmp10, tmp11
            add         off0, off0, xPos
            psq_l       tmp13, 8*6*sizeof(f32)(ws), 0, 0
            ps_sub      tmp8, tmp10, tmp11
            add         off1, off0, off1
            ps_add      tmp6, tmp6, bias
            li          itmp0, 3
            ps_add      tmp7, tmp12, tmp13
            add         out0, obase, off0
            ps_sub      tmp9, tmp12, tmp13
            ps_add      tmp0, tmp6, tmp7
            add         out1, obase, off1
            ps_add      tmp8, tmp8, bias
            mtctr       itmp0

        _loopHead10:
            psq_l       tmp4, 8*1*sizeof(f32)(ws), 0, 0
            ps_msub     tmp9, tmp9, cc4, tmp7
            psq_l       tmp5, 8*3*sizeof(f32)(ws), 0, 0
            ps_sub      tmp3, tmp6, tmp7
            ps_add      tmp1, tmp8, tmp9
            psq_l       tmp6, 8*5*sizeof(f32)(ws), 0, 0
            ps_sub      tmp2, tmp8, tmp9
            psq_l       tmp7, 8*7*sizeof(f32)(ws), 0, 0
            ps_add      tmp8, tmp6, tmp5
            ps_sub      tmp6, tmp6, tmp5
            addi        ws, ws, 2*sizeof(f32)
            ps_add      tmp9, tmp4, tmp7
            ps_sub      tmp4, tmp4, tmp7
            psq_l       tmp10, 8*0*sizeof(f32)(ws), 0, 0
            ps_add      tmp7, tmp9, tmp8
            ps_sub      tmp5, tmp9, tmp8
            ps_add      tmp8, tmp6, tmp4
            psq_l       tmp11, 8*4*sizeof(f32)(ws), 0, 0
            ps_add      tmp9, tmp0, tmp7
            ps_mul      tmp8, tmp8, cc2
            psq_l       tmp12, 8*2*sizeof(f32)(ws), 0, 0
            ps_sub      tmp23, tmp0, tmp7
            ps_madd     tmp6, tmp6, cc2c6a, tmp8
            psq_l       tmp13, 8*6*sizeof(f32)(ws), 0, 0
            ps_sub      tmp6, tmp6, tmp7
            addi        off0, off0, 2*sizeof(THPSample)
            psq_st      tmp9, 0(out0), 0, 6
            ps_msub     tmp4, tmp4, cc2c6s, tmp8
            ps_add      tmp9, tmp1, tmp6
            ps_msub     tmp5, tmp5, cc4, tmp6
            ps_sub      tmp22, tmp1, tmp6
            psq_st      tmp9, 8(out0), 0, 6
            ps_add      tmp8, tmp2, tmp5
            ps_add      tmp4, tmp4, tmp5
            psq_st      tmp8, 16(out0), 0, 6
            addi        off1, off1, 2*sizeof(THPSample)
            ps_sub      tmp9, tmp3, tmp4
            ps_add      tmp20, tmp3, tmp4
            psq_st      tmp9, 24(out0), 0, 6
            ps_sub      tmp21, tmp2, tmp5
            ps_add      tmp6, tmp10, tmp11
            psq_st      tmp20, 0(out1), 0, 6
            ps_sub      tmp8, tmp10, tmp11
            ps_add      tmp6, tmp6, bias
            psq_st      tmp21, 8(out1), 0, 6
            ps_add      tmp7, tmp12, tmp13
            ps_sub      tmp9, tmp12, tmp13
            psq_st      tmp22, 16(out1), 0, 6
            add         out0, obase, off0
            ps_add      tmp0, tmp6, tmp7
            psq_st      tmp23, 24(out1), 0, 6
            ps_add      tmp8, tmp8, bias
            add         out1, obase, off1

            bdnz        _loopHead10
            psq_l       tmp4, 8*1*sizeof(f32)(ws), 0, 0
            ps_msub     tmp9, tmp9, cc4, tmp7
            psq_l       tmp5, 8*3*sizeof(f32)(ws), 0, 0
            ps_sub      tmp3, tmp6, tmp7
            ps_add      tmp1, tmp8, tmp9
            psq_l       tmp6, 8*5*sizeof(f32)(ws), 0, 0
            ps_sub      tmp2, tmp8, tmp9
            psq_l       tmp7, 8*7*sizeof(f32)(ws), 0, 0
            ps_add      tmp8, tmp6, tmp5
            ps_sub      tmp6, tmp6, tmp5
            ps_add      tmp9, tmp4, tmp7
            ps_sub      tmp4, tmp4, tmp7
            ps_add      tmp7, tmp9, tmp8
            ps_sub      tmp5, tmp9, tmp8
            ps_add      tmp8, tmp6, tmp4
            ps_add      tmp9, tmp0, tmp7
            ps_mul      tmp8, tmp8, cc2
            ps_sub      tmp23, tmp0, tmp7
            ps_madd     tmp6, tmp6, cc2c6a, tmp8
            psq_st      tmp9, 0(out0), 0, 6
            ps_sub      tmp6, tmp6, tmp7
            ps_msub     tmp4, tmp4, cc2c6s, tmp8
            psq_st      tmp23, 24(out1), 0, 6
            ps_add      tmp9, tmp1, tmp6
            ps_msub     tmp5, tmp5, cc4, tmp6
            ps_sub      tmp22, tmp1, tmp6
            psq_st      tmp9, 8(out0), 0, 6
            ps_add      tmp8, tmp2, tmp5
            ps_add      tmp4, tmp4, tmp5
            psq_st      tmp8, 16(out0), 0, 6
            ps_sub      tmp9, tmp3, tmp4
            psq_st      tmp22, 16(out1), 0, 6
            ps_add      tmp20, tmp3, tmp4
            psq_st      tmp9, 24(out0), 0, 6
            ps_sub      tmp21, tmp2, tmp5
            psq_st      tmp20, 0(out1), 0, 6
            psq_st      tmp21, 8(out1), 0, 6

        }
        // clang-format on
    }
#endif
}

static void __THPInverseDCTNoYPos(__REGISTER THPCoeff* in, __REGISTER u32 xPos) {
#ifdef __MWERKS__
    register f32 *q, *ws;
    register f32 tmp0, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7, tmp8, tmp9;
    register f32 tmp10, tmp11, tmp12, tmp13;
    register f32 tmp20, tmp21, tmp22, tmp23;
    register f32 cc4 = 1.414213562F;
    register f32 cc2 = 1.847759065F;
    register f32 cc2c6s = 1.082392200F;
    register f32 cc2c6a = -2.613125930F;
    register f32 bias = 1024.0F;
    q = Gq;
    ws = &__THPIDCTWorkspace[0] - 2;

    {
        register u32 itmp0, itmp1, itmp2, itmp3;
        // clang-format off
        asm {
            li          itmp2, 8
            mtctr       itmp2

        _loopHead0:
            psq_l       tmp10, 0(in), 0, 5
            psq_l       tmp11, 0(q), 0, 0
            lwz         itmp0, 12(in)
            lwz         itmp3, 8(in)
            ps_mul      tmp10, tmp10, tmp11
            lwz         itmp1, 4(in)
            lhz         itmp2, 2(in)
            or.         itmp0, itmp0, itmp3

        _loopHead1:
            cmpwi       itmp0, 0
            bne         _regularIDCT
            ps_merge00  tmp0, tmp10, tmp10
            cmpwi       itmp1, 0
            psq_st      tmp0, 8(ws), 0, 0
            bne         _halfIDCT
            psq_st      tmp0, 16(ws), 0, 0
            cmpwi       itmp2, 0
            psq_st      tmp0, 24(ws), 0, 0
            bne         _quarterIDCT
            addi        q, q, 8*sizeof(f32)
            psq_stu     tmp0, 32(ws), 0, 0
            addi        in, in, 8*sizeof(THPCoeff)
            bdnz        _loopHead0
            b           _loopEnd

        _quarterIDCT:
            addi        in, in, 8*sizeof(THPCoeff)
            ps_msub     tmp2, tmp10, cc2, tmp10
            addi        q, q, 8*sizeof(f32)
            ps_merge00  tmp9, tmp10, tmp10
            lwz         itmp1, 4(in)
            ps_sub      tmp1, cc2, cc2c6s
            ps_msub     tmp3, tmp10, cc4, tmp2
            lhz         itmp2, 2(in)
            ps_merge11  tmp5, tmp10, tmp2
            psq_l       tmp11, 0(q), 0, 0
            ps_nmsub    tmp4, tmp10, tmp1, tmp3
            ps_add      tmp7, tmp9, tmp5
            psq_l       tmp10, 0(in), 0, 5
            ps_merge11  tmp6, tmp3, tmp4
            ps_sub      tmp5, tmp9, tmp5
            lwz         itmp0, 12(in)
            ps_add      tmp8, tmp9, tmp6
            lwz         itmp3, 8(in)
            ps_sub      tmp6, tmp9, tmp6
            psq_stu     tmp7, 8(ws), 0, 0
            ps_merge10  tmp6, tmp6, tmp6
            psq_stu     tmp8, 8(ws), 0, 0
            ps_merge10  tmp5, tmp5, tmp5
            or          itmp0, itmp0, itmp3
            psq_stu     tmp6, 8(ws), 0, 0
            ps_mul      tmp10, tmp10, tmp11
            psq_stu     tmp5, 8(ws), 0, 0
            bdnz        _loopHead1
            b           _loopEnd

        _halfIDCT:
            psq_l       tmp1, 4(in), 0, 5
            psq_l       tmp9, 8(q), 0, 0
            addi        in, in, 8*sizeof(THPCoeff)
            ps_mul      tmp1, tmp1, tmp9
            addi        q, q, 8*sizeof(f32)
            ps_sub      tmp3, tmp10, tmp1
            ps_add      tmp2, tmp10, tmp1
            lwz         itmp0, 12(in)
            ps_madd     tmp4, tmp1, cc4, tmp3
            ps_nmsub    tmp5, tmp1, cc4, tmp2
            ps_mul      tmp8, tmp3, cc2
            ps_merge00  tmp4, tmp2, tmp4
            lwz         itmp3, 8(in)
            ps_nmsub    tmp6, tmp1, cc2c6a, tmp8
            ps_merge00  tmp5, tmp5, tmp3
            lwz         itmp1, 4(in)
            ps_sub      tmp6, tmp6, tmp2
            ps_nmsub    tmp7, tmp10, cc2c6s, tmp8
            lhz         itmp2, 2(in)
            ps_merge11  tmp2, tmp2, tmp6
            ps_msub     tmp8, tmp3, cc4, tmp6
            psq_l       tmp10, 0(in), 0, 5
            ps_add      tmp9, tmp4, tmp2
            ps_sub      tmp7, tmp7, tmp8
            psq_l       tmp11, 0(q), 0, 0
            ps_merge11  tmp3, tmp8, tmp7
            ps_sub      tmp4, tmp4, tmp2
            psq_stu     tmp9, 8(ws), 0, 0
            ps_add      tmp0, tmp5, tmp3
            ps_sub      tmp1, tmp5, tmp3
            or          itmp0, itmp0, itmp3
            psq_stu     tmp0, 8(ws), 0, 0
            ps_merge10  tmp1, tmp1, tmp1
            ps_merge10  tmp4, tmp4, tmp4
            psq_stu     tmp1, 8(ws), 0, 0
            ps_mul      tmp10, tmp10, tmp11
            psq_stu     tmp4, 8(ws), 0, 0
            bdnz        _loopHead1
            b           _loopEnd

        _regularIDCT:
            psq_l       tmp9, 4(in), 0, 5
            psq_l       tmp5, 8(q), 0, 0
            ps_mul      tmp9, tmp9, tmp5
            psq_l       tmp2, 8(in), 0, 5
            psq_l       tmp6, 16(q), 0, 0
            ps_merge01  tmp0, tmp10, tmp9
            psq_l       tmp3, 12(in), 0, 5
            ps_merge01  tmp1, tmp9, tmp10
            psq_l       tmp7, 24(q), 0, 0
            addi        in, in, 8*sizeof(THPCoeff)
            ps_madd     tmp4, tmp2, tmp6, tmp0
            ps_nmsub    tmp5, tmp2, tmp6, tmp0
            ps_madd     tmp6, tmp3, tmp7, tmp1
            ps_nmsub    tmp7, tmp3, tmp7, tmp1
            addi        q, q, 8*sizeof(f32)
            ps_add      tmp0, tmp4, tmp6
            ps_sub      tmp3, tmp4, tmp6
            ps_msub     tmp2, tmp7, cc4, tmp6
            lwz         itmp0, 12(in)
            ps_sub      tmp8, tmp7, tmp5
            ps_add      tmp1, tmp5, tmp2
            ps_sub      tmp2, tmp5, tmp2
            ps_mul      tmp8, tmp8, cc2
            lwz         itmp3, 8(in)
            ps_merge00  tmp1, tmp0, tmp1
            ps_nmsub    tmp6, tmp5, cc2c6a, tmp8
            ps_msub     tmp4, tmp7, cc2c6s, tmp8
            lwz         itmp1, 4(in)
            ps_sub      tmp6, tmp6, tmp0
            ps_merge00  tmp2, tmp2, tmp3
            lhz         itmp2, 2(in)
            ps_madd     tmp5, tmp3, cc4, tmp6
            ps_merge11  tmp7, tmp0, tmp6
            psq_l       tmp10, 0(in), 0, 5
            ps_sub      tmp4, tmp4, tmp5
            ps_add      tmp3, tmp1, tmp7
            psq_l       tmp11, 0(q), 0, 0
            ps_merge11  tmp4, tmp5, tmp4
            ps_sub      tmp0, tmp1, tmp7
            ps_mul      tmp10, tmp10, tmp11
            ps_add      tmp5, tmp2, tmp4
            ps_sub      tmp6, tmp2, tmp4
            ps_merge10  tmp5, tmp5, tmp5
            psq_stu     tmp3, 8(ws), 0, 0
            ps_merge10  tmp0, tmp0, tmp0
            psq_stu     tmp6, 8(ws), 0, 0
            psq_stu     tmp5, 8(ws), 0, 0
            or          itmp0, itmp0, itmp3
            psq_stu     tmp0, 8(ws), 0, 0
            bdnz        _loopHead1

        _loopEnd:

        }
        // clang-format on
    }

    ws = &__THPIDCTWorkspace[0];

    {
        register THPSample* obase = Gbase;
        register u32 wid = Gwid;

        register u32 itmp0, off0, off1;
        register THPSample *out0, *out1;

        // clang-format off
        asm {
            psq_l       tmp10, 8*0*sizeof(f32)(ws), 0, 0
            slwi        xPos, xPos, 2
            psq_l       tmp11, 8*4*sizeof(f32)(ws), 0, 0
            slwi        off1, wid, 2
            psq_l       tmp12, 8*2*sizeof(f32)(ws), 0, 0
            mr         off0, xPos
            ps_add      tmp6, tmp10, tmp11
            psq_l       tmp13, 8*6*sizeof(f32)(ws), 0, 0
            ps_sub      tmp8, tmp10, tmp11
            add         off1, off0, off1
            ps_add      tmp6, tmp6, bias
            li      itmp0, 3
            ps_add      tmp7, tmp12, tmp13
            add         out0, obase, off0
            ps_sub      tmp9, tmp12, tmp13
            ps_add      tmp0, tmp6, tmp7
            add         out1, obase, off1
            ps_add      tmp8, tmp8, bias
            mtctr   itmp0

        _loopHead10:
            psq_l       tmp4, 8*1*sizeof(f32)(ws), 0, 0
            ps_msub     tmp9, tmp9, cc4, tmp7
            psq_l       tmp5, 8*3*sizeof(f32)(ws), 0, 0
            ps_sub      tmp3, tmp6, tmp7
            ps_add      tmp1, tmp8, tmp9
            psq_l       tmp6, 8*5*sizeof(f32)(ws), 0, 0
            ps_sub      tmp2, tmp8, tmp9
            psq_l       tmp7, 8*7*sizeof(f32)(ws), 0, 0
            ps_add      tmp8, tmp6, tmp5
            ps_sub      tmp6, tmp6, tmp5
            addi        ws, ws, 2*sizeof(f32)
            ps_add      tmp9, tmp4, tmp7
            ps_sub      tmp4, tmp4, tmp7
            psq_l       tmp10, 8*0*sizeof(f32)(ws), 0, 0
            ps_add      tmp7, tmp9, tmp8
            ps_sub      tmp5, tmp9, tmp8
            ps_add      tmp8, tmp6, tmp4
            psq_l       tmp11, 8*4*sizeof(f32)(ws), 0, 0
            ps_add      tmp9, tmp0, tmp7
            ps_mul      tmp8, tmp8, cc2
            psq_l       tmp12, 8*2*sizeof(f32)(ws), 0, 0
            ps_sub      tmp23, tmp0, tmp7
            ps_madd     tmp6, tmp6, cc2c6a, tmp8
            psq_l       tmp13, 8*6*sizeof(f32)(ws), 0, 0
            ps_sub      tmp6, tmp6, tmp7
            addi        off0, off0, 2*sizeof(THPSample)
            psq_st      tmp9, 0(out0), 0, 6
            ps_msub     tmp4, tmp4, cc2c6s, tmp8
            ps_add      tmp9, tmp1, tmp6
            ps_msub     tmp5, tmp5, cc4, tmp6
            ps_sub      tmp22, tmp1, tmp6
            psq_st      tmp9, 8(out0), 0, 6
            ps_add      tmp8, tmp2, tmp5
            ps_add      tmp4, tmp4, tmp5
            psq_st      tmp8, 16(out0), 0, 6
            addi        off1, off1, 2*sizeof(THPSample)
            ps_sub      tmp9, tmp3, tmp4
            ps_add      tmp20, tmp3, tmp4
            psq_st      tmp9, 24(out0), 0, 6
            ps_sub      tmp21, tmp2, tmp5
            ps_add      tmp6, tmp10, tmp11
            psq_st      tmp20, 0(out1), 0, 6
            ps_sub      tmp8, tmp10, tmp11
            ps_add      tmp6, tmp6, bias
            psq_st      tmp21, 8(out1), 0, 6
            ps_add      tmp7, tmp12, tmp13
            ps_sub      tmp9, tmp12, tmp13
            psq_st      tmp22, 16(out1), 0, 6
            add         out0, obase, off0
            ps_add      tmp0, tmp6, tmp7
            psq_st      tmp23, 24(out1), 0, 6
            ps_add      tmp8, tmp8, bias
            add         out1, obase, off1
            bdnz        _loopHead10
            psq_l       tmp4, 8*1*sizeof(f32)(ws), 0, 0
            ps_msub     tmp9, tmp9, cc4, tmp7
            psq_l       tmp5, 8*3*sizeof(f32)(ws), 0, 0
            ps_sub      tmp3, tmp6, tmp7
            ps_add      tmp1, tmp8, tmp9
            psq_l       tmp6, 8*5*sizeof(f32)(ws), 0, 0
            ps_sub      tmp2, tmp8, tmp9
            psq_l       tmp7, 8*7*sizeof(f32)(ws), 0, 0
            ps_add      tmp8, tmp6, tmp5
            ps_sub      tmp6, tmp6, tmp5
            ps_add      tmp9, tmp4, tmp7
            ps_sub      tmp4, tmp4, tmp7
            ps_add      tmp7, tmp9, tmp8
            ps_sub      tmp5, tmp9, tmp8
            ps_add      tmp8, tmp6, tmp4
            ps_add      tmp9, tmp0, tmp7
            ps_mul      tmp8, tmp8, cc2
            ps_sub      tmp23, tmp0, tmp7
            ps_madd     tmp6, tmp6, cc2c6a, tmp8
            psq_st      tmp9, 0(out0), 0, 6
            ps_sub      tmp6, tmp6, tmp7
            ps_msub     tmp4, tmp4, cc2c6s, tmp8
            psq_st      tmp23, 24(out1), 0, 6
            ps_add      tmp9, tmp1, tmp6
            ps_msub     tmp5, tmp5, cc4, tmp6
            ps_sub      tmp22, tmp1, tmp6
            psq_st      tmp9, 8(out0), 0, 6
            ps_add      tmp8, tmp2, tmp5
            ps_add      tmp4, tmp4, tmp5
            psq_st      tmp22, 16(out1), 0, 6
            psq_st      tmp8, 16(out0), 0, 6
            ps_sub      tmp9, tmp3, tmp4
            ps_add      tmp20, tmp3, tmp4
            psq_st      tmp9, 24(out0), 0, 6
            ps_sub      tmp21, tmp2, tmp5
            psq_st      tmp20, 0(out1), 0, 6
            psq_st      tmp21, 8(out1), 0, 6
        }
        // clang-format on
    }
#endif
}

static void __THPDecompressiMCURow640x480() {
    u8 cl_num;
    u32 x_pos;
    THPComponent* comp;

    LCQueueWait(3);

    {
        for (cl_num = 0; cl_num < __THPInfo->MCUsPerRow; cl_num++) {
            THPFileInfo* um = __THPInfo;
            __THPHuffDecodeDCTCompY(um, __THPMCUBuffer[0]);
            __THPHuffDecodeDCTCompY(__THPInfo, __THPMCUBuffer[1]);
            __THPHuffDecodeDCTCompY(__THPInfo, __THPMCUBuffer[2]);
            __THPHuffDecodeDCTCompY(__THPInfo, __THPMCUBuffer[3]);
            __THPHuffDecodeDCTCompU(__THPInfo, __THPMCUBuffer[4]);
            __THPHuffDecodeDCTCompV(__THPInfo, __THPMCUBuffer[5]);

            comp = &__THPInfo->components[0];
            Gbase = __THPLCWork640[0];
            Gwid = 640;
            Gq = __THPInfo->quantTabs[comp->quantizationTableSelector];
            x_pos = (u32)(cl_num * 16);
            __THPInverseDCTNoYPos(__THPMCUBuffer[0], x_pos);
            __THPInverseDCTNoYPos(__THPMCUBuffer[1], x_pos + 8);
            __THPInverseDCTY8(__THPMCUBuffer[2], x_pos);
            __THPInverseDCTY8(__THPMCUBuffer[3], x_pos + 8);

            comp = &__THPInfo->components[1];
            Gbase = __THPLCWork640[1];
            Gwid = 320;
            Gq = __THPInfo->quantTabs[comp->quantizationTableSelector];
            x_pos /= 2;
            __THPInverseDCTNoYPos(__THPMCUBuffer[4], x_pos);

            comp = &__THPInfo->components[2];
            Gbase = __THPLCWork640[2];
            Gq = __THPInfo->quantTabs[comp->quantizationTableSelector];
            __THPInverseDCTNoYPos(__THPMCUBuffer[5], x_pos);

            if (__THPInfo->RST != 0) {
                __THPInfo->currMCU--;
                if (__THPInfo->currMCU == 0) {
                    __THPInfo->currMCU = __THPInfo->nMCU;

                    __THPInfo->cnt = 1 + ((__THPInfo->cnt + 6) & 0xFFFFFFF8);

                    if (__THPInfo->cnt > 32) {
                        __THPInfo->cnt = 33;
                    }

                    __THPInfo->components[0].predDC = 0;
                    __THPInfo->components[1].predDC = 0;
                    __THPInfo->components[2].predDC = 0;
                }
            }
        }
    }

    LCStoreData(__THPInfo->dLC[0], __THPLCWork640[0], 0x2800);
    LCStoreData(__THPInfo->dLC[1], __THPLCWork640[1], 0xA00);
    LCStoreData(__THPInfo->dLC[2], __THPLCWork640[2], 0xA00);

    __THPInfo->dLC[0] += 0x2800;
    __THPInfo->dLC[1] += 0xA00;
    __THPInfo->dLC[2] += 0xA00;
}

static void __THPDecompressiMCURowNxN() {
    u8 cl_num;
    u32 x_pos, x;
    THPComponent* comp;

    x = __THPInfo->xPixelSize;

    LCQueueWait(3);

    for (cl_num = 0; cl_num < __THPInfo->MCUsPerRow; cl_num++) {
        __THPHuffDecodeDCTCompY(__THPInfo, __THPMCUBuffer[0]);
        __THPHuffDecodeDCTCompY(__THPInfo, __THPMCUBuffer[1]);
        __THPHuffDecodeDCTCompY(__THPInfo, __THPMCUBuffer[2]);
        __THPHuffDecodeDCTCompY(__THPInfo, __THPMCUBuffer[3]);
        __THPHuffDecodeDCTCompU(__THPInfo, __THPMCUBuffer[4]);
        __THPHuffDecodeDCTCompV(__THPInfo, __THPMCUBuffer[5]);

        comp = &__THPInfo->components[0];
        Gbase = __THPLCWork640[0];
        Gwid = x;
        Gq = __THPInfo->quantTabs[comp->quantizationTableSelector];
        x_pos = (u32)(cl_num * 16);
        __THPInverseDCTNoYPos(__THPMCUBuffer[0], x_pos);
        __THPInverseDCTNoYPos(__THPMCUBuffer[1], x_pos + 8);
        __THPInverseDCTY8(__THPMCUBuffer[2], x_pos);
        __THPInverseDCTY8(__THPMCUBuffer[3], x_pos + 8);

        comp = &__THPInfo->components[1];
        Gbase = __THPLCWork640[1];
        Gwid = x / 2;
        Gq = __THPInfo->quantTabs[comp->quantizationTableSelector];
        x_pos /= 2;
        __THPInverseDCTNoYPos(__THPMCUBuffer[4], x_pos);

        comp  = &__THPInfo->components[2];
        Gbase = __THPLCWork640[2];
        Gq    = __THPInfo->quantTabs[comp->quantizationTableSelector];
        __THPInverseDCTNoYPos(__THPMCUBuffer[5], x_pos);

        if (__THPInfo->RST != 0) {
            __THPInfo->currMCU--;
            if (__THPInfo->currMCU == 0) {
                __THPInfo->currMCU = __THPInfo->nMCU;
                __THPInfo->cnt = 1 + ((__THPInfo->cnt + 6) & 0xFFFFFFF8);

                if (__THPInfo->cnt > 32) {
                    __THPInfo->cnt = 33;
                }

                __THPInfo->components[0].predDC = 0;
                __THPInfo->components[1].predDC = 0;
                __THPInfo->components[2].predDC = 0;
            }
        }
    }

    LCStoreData(__THPInfo->dLC[0], __THPLCWork640[0], ((4 * sizeof(u8) * 64) * (x / 16)));
    LCStoreData(__THPInfo->dLC[1], __THPLCWork640[1], ((sizeof(u8) * 64) * (x / 16)));
    LCStoreData(__THPInfo->dLC[2], __THPLCWork640[2], ((sizeof(u8) * 64) * (x / 16)));
    __THPInfo->dLC[0] += ((4 * sizeof(u8) * 64) * (x / 16));
    __THPInfo->dLC[1] += ((sizeof(u8) * 64) * (x / 16));
    __THPInfo->dLC[2] += ((sizeof(u8) * 64) * (x / 16));
}

static void __THPHuffDecodeDCTCompY(__REGISTER THPFileInfo* info, THPCoeff* block) {
    {
        __REGISTER s32 t;
        THPCoeff dc;
        __REGISTER THPCoeff diff;

        __dcbz((void*)block, 0);
        t = __THPHuffDecodeTab(info, Ydchuff);
        __dcbz((void*)block, 32);
        diff = 0;
        __dcbz((void*)block, 64);

        if (t) {
            {
                __REGISTER s32 v;
                __REGISTER u32 cb;
                __REGISTER u32 cnt;
                __REGISTER u32 code;
                __REGISTER u32 tmp;
                __REGISTER u32 cnt1;
                __REGISTER u32 tmp1;
                // clang-format off
#ifdef __MWERKS__
                asm {
                        lwz      cnt,info->cnt;
                        subfic   code,cnt,33;
                        lwz      cb,info->currByte;

                        subfc. tmp, code, t;
                        subi     cnt1,cnt,1;

                        bgt      _notEnoughBitsDIFF;
                        add      v,cnt,t;

                        slw      cnt,cb,cnt1;
                        stw      v,info->cnt;
                        subfic   v,t,32;
                        srw      diff,cnt,v;
                }
#endif
                // clang-format on

                ASSERTLINE(4309, info->cnt <=33);

                // clang-format off
#ifdef __MWERKS__
                asm
                {
                    b _DoneDIFF;
                _notEnoughBitsDIFF:
                    lwz tmp1, info->c;
                    slw v, cb, cnt1;
                    lwzu cb, 4(tmp1);
                    addi tmp, tmp, 1;
                    stw cb, info->currByte;
                    srw cb, cb, code;
                    stw tmp1, info->c;
                    add v, cb, v;
                    stw tmp, info->cnt;
                    subfic tmp, t, 32;
                    srw diff, v, tmp;
                _DoneDIFF:
                }
#endif
                // clang-format on
                ASSERTLINE(4336, info->cnt <=33);
            }

            if (__cntlzw((u32)diff) > 32 - t) {
                diff += ((0xFFFFFFFF << t) + 1);
            }
        };

        __dcbz((void*)block, 96);
        dc = (s16)(info->components[0].predDC + diff);
        block[0] = info->components[0].predDC = dc;
    }

    {
        __REGISTER s32 k;
        __REGISTER s32 code;
        __REGISTER u32 cnt;
        __REGISTER u32 cb;
        __REGISTER u32 increment;
        __REGISTER s32 tmp;
        __REGISTER THPHuffmanTab* h = Yachuff;

        // clang-format off
#ifdef __MWERKS__
        asm
        {
            lwz     cnt, info->cnt;
            addi    increment, h, 32;
            lwz     cb, info->currByte;
        }
#endif
        // clang-format on

        for (k = 1; k < 64; k++)
        {
            __REGISTER s32 ssss;
            __REGISTER s32 rrrr;

            // clang-format off
#ifdef __MWERKS__
            asm {
                addi    code, cnt, 4;
                cmpwi   cnt, 28;
                rlwnm   tmp, cb, code, 27, 31;
                bgt     _notEnoughBits;

                lbzx    ssss, h, tmp;
                lbzx    code, increment, tmp;
                cmpwi   ssss, 0xFF;

                beq     _FailedCheckEnoughBits;
                add     cnt, cnt, code;
                b       _DoneDecodeTab;
            }
#endif
            // clang-format on

            {
                __REGISTER u32 maxcodebase;
                __REGISTER u32 tmp2;

            _FailedCheckEnoughBits:
                cnt += 5;
                maxcodebase = (u32) & (h->maxCode);
                // clang-format off
#ifdef __MWERKS__
                asm {
                    li          tmp2, sizeof(s32)*(5);
                    li          code, 5;
                    add         maxcodebase, maxcodebase, tmp2;
                  __WHILE_START:
                    cmpwi       cnt, 33;
                    slwi        tmp, tmp, 1

                    beq         _FCEB_faster;
                    rlwnm       ssss, cb, cnt, 31, 31;
                    lwzu        tmp2, 4(maxcodebase);
                    or          tmp, tmp, ssss
                    addi        cnt, cnt, 1;
                    b __WHILE_CHECK;

                  _FCEB_faster:
                    lwz     ssss, info->c;
                    li      cnt, 1;
                    lwzu    cb, 4(ssss);

                    lwzu    tmp2, 4(maxcodebase);

                    stw     ssss, info->c;
                    rlwimi  tmp, cb, 1,31,31;
                    b __FL_WHILE_CHECK;

                  __FL_WHILE_START:
                    slwi    tmp, tmp, 1;

                    rlwnm   ssss, cb, cnt, 31, 31;
                    lwzu    tmp2, 4(maxcodebase);
                    or      tmp, tmp, ssss;

                  __FL_WHILE_CHECK:
                    cmpw    tmp,tmp2
                    addi    cnt, cnt, 1;
                    addi    code, code, 1
                    bgt     __FL_WHILE_START;
                    b _FCEB_Done;

                  __WHILE_CHECK:
                    cmpw    tmp,tmp2
                    addi    code, code, 1
                    bgt     __WHILE_START;
                }
#endif
                // clang-format on
            }
        _FCEB_Done:
            ssss = (h->Vij[(s32)(tmp + h->valPtr[code])]);
            goto _DoneDecodeTab;

        _notEnoughBits:
            // clang-format off
#ifdef __MWERKS__
            asm
            {
                cmpwi   cnt, 33;
                lwz     tmp, info->c;
                beq     _getfullword;

                cmpwi   cnt, 32;
                rlwnm   code, cb, code, 27, 31
                beq     _1bitleft;

                lbzx    ssss, h, code;
                lbzx    rrrr, increment, code;
                cmpwi   ssss, 0xFF;
                add     code, cnt, rrrr;
                beq _FailedCheckNoBits0;

                cmpwi   code, 33;
                bgt     _FailedCheckNoBits1;
            }
#endif
            // clang-format on
            cnt = (u32)code;
            goto _DoneDecodeTab;

        _getfullword : {
            // clang-format off
#ifdef __MWERKS__
            asm
            {
                    lwzu    cb, 4(tmp);
                    rlwinm  code, cb, 5, 27, 31
                    stw     tmp, info->c;
                    lbzx    ssss, h, code;
                    lbzx    tmp, increment, code;
                    cmpwi   ssss, 0xFF
                    addi    cnt, tmp, 1
                    beq     _FailedCheckEnoughbits_Updated;
            }
#endif
            // clang-format on
        }
            goto _DoneDecodeTab;

        _FailedCheckEnoughbits_Updated:
            ssss = 5;
            do {
                // clang-format off
#ifdef __MWERKS__
                asm
                {
                    subfic  tmp, ssss, 31;
                    addi    ssss, ssss, 1;
                    srw     code, cb, tmp;
                }
#endif
                // clang-format on
            } while (code > h->maxCode[ssss]);

            cnt = (u32)(ssss + 1);
            ssss = (h->Vij[(s32)(code + h->valPtr[ssss])]);

            goto _DoneDecodeTab;

        _1bitleft:
            // clang-format off
#ifdef __MWERKS__
            asm {
                lwzu    cb, 4(tmp);

                stw     tmp, info->c;
                rlwimi  code, cb, 4, 28, 31;
                lbzx    ssss, h, code;
                lbzx    cnt, increment, code
                cmpwi   ssss, 0xFF
                beq     _Read4;

            }
#endif
            // clang-format on

            goto _DoneDecodeTab;

        _Read4 : {
            __REGISTER u32 maxcodebase = (u32) & (h->maxCode);
            __REGISTER u32 tmp2;

            // clang-format off
#ifdef __MWERKS__
            asm {
                    li  cnt, sizeof(s32)*5;
                    add     maxcodebase, maxcodebase, cnt;

                    slwi    tmp, code, 32-5;
                    li      cnt,5;
                    rlwimi  tmp, cb, 32-1, 1,31;

                  __DR4_WHILE_START:

                    subfic  ssss, cnt, 31;
                    lwzu    tmp2, 4(maxcodebase);
                    srw     code, tmp, ssss;
                  __DR4_WHILE_CHECK:
                    cmpw    code, tmp2
                    addi    cnt, cnt, 1
                    bgt     __DR4_WHILE_START;

            }
#endif
            // clang-format on
        }
            ssss = (h->Vij[(s32)(code + h->valPtr[cnt])]);
            goto _DoneDecodeTab;

        _FailedCheckNoBits0:
        _FailedCheckNoBits1:
        _REALFAILEDCHECKNOBITS : {
            __REGISTER u32 mask = 0xFFFFFFFF << (33 - cnt);
            __REGISTER u32 tmp2;
            __REGISTER u32 tmp3;
            code = (s32)(cb & (~mask));
            mask = (u32) & (h->maxCode);

            // clang-format off
#ifdef __MWERKS__
            asm {
                    lwz     tmp, info->c;
                    subfic  tmp2, cnt, 33;
                    addi    tmp3, tmp2, 1;
                    slwi    tmp2, tmp2, 2;
                    lwzu    cb, 4(tmp);
                    add     mask,mask, tmp2;
                    stw     tmp, info->c;
                    slwi    code, code, 1;
                    rlwimi  code, cb, 1, 31, 31;
                    lwzu    tmp2, 4(mask);
                    li      cnt, 2;
                    b       __FCNB1_WHILE_CHECK;

                  __FCNB1_WHILE_START:
                    slwi    code, code, 1;

                    addi    tmp3, tmp3, 1;
                    lwzu    tmp2, 4(mask);
                    add     code, code, rrrr;
                    addi    cnt, cnt, 1;

                  __FCNB1_WHILE_CHECK:
                    cmpw    code, tmp2;
                    rlwnm   rrrr, cb, cnt, 31, 31;
                    bgt     __FCNB1_WHILE_START;

            }
#endif
            // clang-format on
            ssss = (h->Vij[(s32)(code + h->valPtr[tmp3])]);
        }

            goto _DoneDecodeTab;

        _DoneDecodeTab:
            // clang-format off
#ifdef __MWERKS__
            asm {
                andi.   rrrr, ssss, 15;
                srawi   ssss, ssss, 4;
                beq     _RECV_SSSS_ZERO;
            }
#endif
            // clang-format on

            {
                k += ssss;
                {
                    __REGISTER s32 v;
                    __REGISTER u32 cnt1;
                    __REGISTER u32 tmp1;
                    // clang-format off
#ifdef __MWERKS__
                    asm
                    {
                        subfic   code,cnt,33;
                        subfc. tmp, code, rrrr;
                        subi     cnt1,cnt,1;
                        bgt      _RECVnotEnoughBits;
                        add      cnt,cnt,rrrr;
                        slw      tmp1,cb,cnt1;
                        subfic   v,rrrr,32;
                        srw      ssss,tmp1,v;
                    }
#endif
                    // clang-format on
                    // clang-format off
#ifdef __MWERKS__
                    asm
                    {
                        b _RECVDone;
                    _RECVnotEnoughBits:
                        lwz tmp1, info->c;
                        slw v, cb, cnt1;
                        lwzu cb, 4(tmp1);
                        addi cnt, tmp, 1;
                        stw tmp1, info->c;
                        srw tmp1, cb, code;

                        add v, tmp1, v;
                        subfic tmp, rrrr, 32;
                        srw ssss, v, tmp;
                    _RECVDone:
                    }
#endif
                    // clang-format on
                }

                if (__cntlzw((u32)ssss) > 32 - rrrr) {
                    ssss += ((0xFFFFFFFF << rrrr) + 1);
                }

                block[__THPJpegNaturalOrder[k]] = (s16)ssss;
#ifdef __MWERKS__
                goto _RECV_END;
#else
                continue;
#endif
            }

            {
            _RECV_SSSS_ZERO:
                if (ssss != 15) {
                    break;
                }

                k += 15;
            };

            // clang-format off
#ifdef __MWERKS__
            asm
            {
              _RECV_END:
            }
#endif
            // clang-format on
        }
        info->cnt      = cnt;
        info->currByte = cb;
    }
}

static s32 __THPHuffDecodeTab(__REGISTER THPFileInfo* info, __REGISTER THPHuffmanTab* h) {
    __REGISTER s32 code;
    __REGISTER u32 cnt;
    __REGISTER s32 cb;
    __REGISTER u32 increment;
    __REGISTER s32 tmp;

    // clang-format off
#ifdef __MWERKS__
    asm
    {
        lwz     cnt, info->cnt;
        addi    increment, h, 32;
        lwz     cb, info->currByte;
        addi    code, cnt, 4;
        cmpwi   cnt, 28;
        rlwnm   tmp, cb, code, 27, 31;
        bgt     _notEnoughBits;
        lbzx    code, h, tmp;
        lbzx    increment, increment, tmp;
        cmpwi   code, 0xFF;
        beq     _FailedCheckEnoughBits;
        add     cnt, cnt, increment;
        stw     cnt, info->cnt;
    }
#endif
    // clang-format on
    ASSERTLINE(1164, info->cnt <=33);
_done:
    return code;

    {
        __REGISTER u32 maxcodebase;
        __REGISTER u32 tmp2;

    _FailedCheckEnoughBits:
        maxcodebase = (u32) & (h->maxCode);
        cnt += 5;

        // clang-format off
#ifdef __MWERKS__
        asm {
            li          tmp2, sizeof(s32)*(5);
            li          code, 5;
            add         maxcodebase, maxcodebase, tmp2;
          __WHILE_START:
            cmpwi       cnt, 33;
            slwi        tmp, tmp, 1

            beq         _FCEB_faster;
            rlwnm       increment, cb, cnt, 31, 31;
            lwzu        tmp2, 4(maxcodebase);
            or          tmp, tmp, increment
            addi        cnt, cnt, 1;
            b __WHILE_CHECK;

          _FCEB_faster:
            lwz     increment, info->c;
            li      cnt, 1;
            lwzu    cb, 4(increment);
            lwzu    tmp2, 4(maxcodebase);

            stw     increment, info->c;
            rlwimi  tmp, cb, 1,31,31;
            stw     cb, info->currByte;
            b __FL_WHILE_CHECK;

          __FL_WHILE_START:
            slwi    tmp, tmp, 1;
            rlwnm   increment, cb, cnt, 31, 31;
            lwzu    tmp2, 4(maxcodebase);
            or      tmp, tmp, increment;

          __FL_WHILE_CHECK:
            cmpw    tmp,tmp2
            addi    cnt, cnt, 1;
            addi        code, code, 1
            bgt     __FL_WHILE_START;
            b _FCEB_Done;

          __WHILE_CHECK:
            cmpw    tmp,tmp2
            addi        code, code, 1
            bgt     __WHILE_START;
        }
#endif
        // clang-format on
    }
_FCEB_Done:
    info->cnt = cnt;
    return (h->Vij[(s32)(tmp + h->valPtr[code])]);

    // clang-format off
#ifdef __MWERKS__
    asm
    {
      _notEnoughBits:
        cmpwi   cnt, 33;
        lwz     tmp, info->c;
        beq     _getfullword; 

        cmpwi   cnt, 32;
        rlwnm   code, cb, code, 27, 31
        beq     _1bitleft;

        lbzx    tmp, h, code;
        lbzx    increment, increment, code;
        cmpwi   tmp, 0xFF;
        add     code, cnt, increment;
        beq _FailedCheckNoBits0;

        cmpwi   code, 33;
        stw     code, info->cnt;
        bgt     _FailedCheckNoBits1;
    }
#endif
    // clang-format on
    ASSERTLINE(1259, info->cnt <=33);
    return tmp;

    // clang-format off
#ifdef __MWERKS__
    asm    {
      _1bitleft:
        lwzu    cb, 4(tmp);

        stw     tmp, info->c;
        rlwimi  code, cb, 4, 28, 31;
        lbzx    tmp, h, code;
        lbzx    increment, increment, code
        stw     cb, info->currByte;
        cmpwi   tmp, 0xFF
        stw     increment, info->cnt;
        beq     _Read4;

    }
#endif
    // clang-format on
    ASSERTLINE(1278, info->cnt <=33);
    return tmp;

_Read4 : {
    __REGISTER u32 maxcodebase = (u32) & (h->maxCode);
    __REGISTER u32 tmp2;

    // clang-format off
#ifdef __MWERKS__
    asm
    {
            li      cnt, sizeof(s32)*5;
            add     maxcodebase, maxcodebase, cnt;

            slwi    tmp, code, 32-5;
            li      cnt,5;
            rlwimi  tmp, cb, 32-1, 1,31;

          __DR4_WHILE_START:

            subfic  cb, cnt, 31;
            lwzu    tmp2, 4(maxcodebase);
            srw     code, tmp, cb;
          __DR4_WHILE_CHECK:
            cmpw    code, tmp2
            addi    cnt, cnt, 1
            bgt     __DR4_WHILE_START;

    }
#endif
    // clang-format on
}

info->cnt = cnt;
ASSERTLINE(1311, info->cnt <=33);
__CODE_PLUS_VP_CNT:
    return (h->Vij[(s32)(code + h->valPtr[cnt])]);

_getfullword:
    // clang-format off
#ifdef __MWERKS__
    asm
    {
        lwzu    cb, 4(tmp);

        rlwinm  code, cb, 5, 27, 31
        stw     tmp, info->c;
        lbzx    cnt, h, code;
        lbzx    increment, increment, code;
        cmpwi   cnt, 0xFF
        stw     cb, info->currByte;
        addi    increment, increment, 1
        beq     _FailedCheckEnoughbits_Updated;

        stw     increment, info->cnt;
    }
#endif
    // clang-format on
    ASSERTLINE(1339, info->cnt <=33);
    return (s32)cnt;

_FailedCheckEnoughbits_Updated:

    cnt = 5;
    do {
        // clang-format off
#ifdef __MWERKS__
        asm
        {
            subfic  tmp, cnt, 31;
            addi    cnt, cnt, 1;
            srw     code, cb, tmp;
        }
#endif
        // clang-format on
    } while (code > h->maxCode[cnt]);

    info->cnt = cnt + 1;
    ASSERTLINE(1357, info->cnt <=33);
    goto __CODE_PLUS_VP_CNT;

_FailedCheckNoBits0:
_FailedCheckNoBits1:

{
    __REGISTER u32 mask = 0xFFFFFFFF << (33 - cnt);
    __REGISTER u32 tmp2;

    code = (s32)(cb & (~mask));
    mask = (u32) & (h->maxCode);

    // clang-format off
#ifdef __MWERKS__
    asm
    {
            lwz     tmp, info->c;
            subfic  tmp2, cnt, 33;
            addi    cnt, tmp2, 1;
            slwi    tmp2, tmp2, 2;
            lwzu    cb, 4(tmp);
            add     mask,mask, tmp2;
            stw     tmp, info->c;
            slwi    code, code, 1;
            stw     cb, info->currByte;
            rlwimi  code, cb, 1, 31, 31;
            lwzu    tmp2, 4(mask);
            li      tmp, 2;
            b       __FCNB1_WHILE_CHECK;

          __FCNB1_WHILE_START:
            slwi    code, code, 1;

            addi    cnt, cnt, 1;
            lwzu    tmp2, 4(mask);
            add     code, code, increment;
            addi    tmp, tmp, 1;

          __FCNB1_WHILE_CHECK:
            cmpw    code, tmp2;
            rlwnm   increment, cb, tmp, 31, 31;
            bgt     __FCNB1_WHILE_START;

    }
#endif
    // clang-format on
}

    info->cnt = (u32)tmp;
    ASSERTLINE(1419, info->cnt <=33);
    return (h->Vij[(s32)(code + h->valPtr[cnt])]);
}

static void __THPHuffDecodeDCTCompU(__REGISTER THPFileInfo* info, THPCoeff* block) {
    __REGISTER s32 t;
    __REGISTER THPCoeff diff;
    THPCoeff dc;
    __REGISTER s32 v;
    __REGISTER u32 cb;
    __REGISTER u32 cnt;
    __REGISTER u32 cnt33;
    __REGISTER u32 tmp;
    __REGISTER u32 cnt1;
    __REGISTER u32 tmp1;
    __REGISTER s32 k;
    __REGISTER s32 ssss;
    __REGISTER s32 rrrr;

    __dcbz((void*)block, 0);
    t = __THPHuffDecodeTab(info, Udchuff);
    __dcbz((void*)block, 32);
    diff = 0;
    __dcbz((void*)block, 64);
    
    if (t) {
        ASSERTLINE(5023, info->cnt <=33);
        // clang-format off
#ifdef __MWERKS__
        asm
        {
            lwz      cnt,info->cnt;
            subfic   cnt33,cnt,33;
            lwz      cb,info->currByte;
            subfc. tmp, cnt33, t;
            subi     cnt1,cnt,1;
            bgt      _notEnoughBitsDIFF;
            add      v,cnt,t;
            slw      cnt,cb,cnt1;
            stw      v,info->cnt;
            subfic   v,t,32;
            srw      diff,cnt,v;
        }
#endif
        // clang-format on

        ASSERTLINE(5043, info->cnt <=33);

        // clang-format off
#ifdef __MWERKS__
        asm
        {
            b _DoneDIFF;
        _notEnoughBitsDIFF:
            lwz tmp1, info->c;
            slw v, cb, cnt1;
            lwzu cb, 4(tmp1);
            addi tmp, tmp, 1;
            stw cb, info->currByte;
            srw cb, cb, cnt33;
            stw tmp1, info->c;
            add v, cb, v;
            stw tmp, info->cnt;
            subfic tmp, t, 32;
            srw diff, v, tmp;
        _DoneDIFF:
        }
#endif
        // clang-format on

        ASSERTLINE(5070, info->cnt <=33);

        if (__cntlzw((u32)diff) > 32 - t) {
            diff += ((0xFFFFFFFF << t) + 1);
        }
    }

    __dcbz((void*)block, 96);
    dc = (s16)(info->components[1].predDC + diff);
    block[0] = info->components[1].predDC = dc;

    for (k = 1; k < 64; k++) {
        ssss = __THPHuffDecodeTab(info, Uachuff);
        rrrr = ssss >> 4;
        ssss &= 15;

        if (ssss) {
            k += rrrr;
            // clang-format off
#ifdef __MWERKS__
            asm
            {
                lwz      cnt,info->cnt;
                subfic   cnt33,cnt,33;
                lwz      cb,info->currByte;
                subf. tmp, cnt33, ssss;
                subi     cnt1,cnt,1;
                bgt      _notEnoughBits;
                add      v,cnt,ssss;
                slw      cnt,cb,cnt1;
                stw      v,info->cnt;
                subfic   v,ssss,32;
                srw      rrrr,cnt,v;
            }
#endif
            // clang-format on

            ASSERTLINE(5110, info->cnt <=33);

            // clang-format off
#ifdef __MWERKS__
            asm
            {
                b _Done;
            _notEnoughBits:
                lwz tmp1, info->c;
                slw v, cb, cnt1;
                lwzu cb, 4(tmp1);
                addi tmp, tmp, 1;
                stw cb, info->currByte;
                srw cb, cb, cnt33;
                stw tmp1, info->c;
                add v, cb, v;
                stw tmp, info->cnt;
                subfic tmp, ssss, 32;
                srw rrrr, v, tmp;
            _Done:
            }
#endif
            // clang-format on

            ASSERTLINE(5137, info->cnt <=33);

            if (__cntlzw((u32)rrrr) > 32 - ssss) {
                rrrr += ((0xFFFFFFFF << ssss) + 1);
            }

            block[__THPJpegNaturalOrder[k]] = (s16)rrrr;
        }

        else {
            if (rrrr != 15)
                break;
            k += 15;
        }
    }
}

static void __THPHuffDecodeDCTCompV(__REGISTER THPFileInfo* info, THPCoeff* block) {
    __REGISTER s32 t;
    __REGISTER THPCoeff diff;
    THPCoeff dc;
    __REGISTER s32 v;
    __REGISTER u32 cb;
    __REGISTER u32 cnt;
    __REGISTER u32 cnt33;
    __REGISTER u32 tmp;
    __REGISTER u32 cnt1;
    __REGISTER u32 tmp1;
    __REGISTER s32 k;
    __REGISTER s32 ssss;
    __REGISTER s32 rrrr;

    __dcbz((void*)block, 0);
    t = __THPHuffDecodeTab(info, Vdchuff);
    __dcbz((void*)block, 32);
    diff = 0;
    __dcbz((void*)block, 64);

    if (t) {
        ASSERTLINE(5208, info->cnt <=33);
        // clang-format off
#ifdef __MWERKS__
        asm
        {
            lwz      cnt,info->cnt;
            subfic   cnt33,cnt,33;
            lwz      cb,info->currByte;
            subf. tmp, cnt33, t;
            subi     cnt1,cnt,1;
            bgt      _notEnoughBitsDIFF;
            add      v,cnt,t;
            slw      cnt,cb,cnt1;
            stw      v,info->cnt;
            subfic   v,t,32;
            srw      diff,cnt,v;
        }
#endif
        // clang-format on

        ASSERTLINE(5228, info->cnt <=33);

        // clang-format off
#ifdef __MWERKS__
        asm
        {
            b _DoneDIFF;
        _notEnoughBitsDIFF:
            lwz tmp1, info->c;
            slw v, cb, cnt1;
            lwzu cb, 4(tmp1);
            addi tmp, tmp, 1;
            stw cb, info->currByte;
            srw cb, cb, cnt33;
            stw tmp1, info->c;
            add v, cb, v;
            stw tmp, info->cnt;
            subfic tmp, t, 32;
            srw diff, v, tmp;
        _DoneDIFF:
        }
#endif
        // clang-format on

        ASSERTLINE(5255, info->cnt <=33);

        if (__cntlzw((u32)diff) > 32 - t) {
            diff += ((0xFFFFFFFF << t) + 1);
        }
    }

    __dcbz((void*)block, 96);

    dc = (s16)(info->components[2].predDC + diff);
    block[0] = info->components[2].predDC = dc;

    for (k = 1; k < 64; k++) {
        ssss = __THPHuffDecodeTab(info, Vachuff);
        rrrr = ssss >> 4;
        ssss &= 15;

        if (ssss) {
            k += rrrr;

            // clang-format off
#ifdef __MWERKS__
            asm
            {
                lwz      cnt,info->cnt;
                subfic   cnt33,cnt,33;
                lwz      cb,info->currByte;

                subf. tmp, cnt33, ssss;
                subi     cnt1,cnt,1;

                bgt      _notEnoughBits;
                add      v,cnt,ssss;

                slw      cnt,cb,cnt1;
                stw      v,info->cnt;
                subfic   v,ssss,32;
                srw      rrrr,cnt,v;
            }
#endif
            // clang-format on

            ASSERTLINE(5296, info->cnt <=33);

            // clang-format off
#ifdef __MWERKS__
            asm
            {
                b _Done;
            _notEnoughBits:
                lwz tmp1, info->c;
                slw v, cb, cnt1;
                lwzu cb, 4(tmp1);
                addi tmp, tmp, 1;
                stw cb, info->currByte;
                srw cb, cb, cnt33;
                stw tmp1, info->c;
                add v, cb, v;
                stw tmp, info->cnt;
                subfic tmp, ssss, 32;
                srw rrrr, v, tmp;
            _Done:
            }
#endif
            // clang-format on

            ASSERTLINE(5323, info->cnt <=33);

            if (__cntlzw((u32)rrrr) > 32 - ssss) {
                rrrr += ((0xFFFFFFFF << ssss) + 1);
            }

            block[__THPJpegNaturalOrder[k]] = (s16)rrrr;
        } else {
            if (rrrr != 15)
                break;
            k += 15;
        }
    }
}

static BOOL THPInit() {
    u8* base;
    base = (u8*)(0xE000 << 16);

    __THPLCWork512[0] = base;
    base += 0x2000;
    __THPLCWork512[1] = base;
    base += 0x800;
    __THPLCWork512[2] = base;
    base += 0x200;

    base = (u8*)(0xE000 << 16);
    __THPLCWork640[0] = base;
    base += 0x2800;
    __THPLCWork640[1] = base;
    base += 0xA00;
    __THPLCWork640[2] = base;
    base += 0xA00;

    OSInitFastCast();

    __THPInitFlag = TRUE;
    return TRUE;
}

#ifdef __cplusplus
}
#endif

static daMP_THPPlayer daMP_ActivePlayer;

static BOOL daMP_ReadThreadCreated;

static OSMessageQueue daMP_FreeReadBufferQueue;

static OSMessageQueue daMP_ReadedBufferQueue;

void* daMP_PopReadedBuffer() {
    OSMessage buffer;
    OSReceiveMessage(&daMP_ReadedBufferQueue, &buffer, 1);
    return buffer;
}

void daMP_PushReadedBuffer(void* buffer) {
    OSSendMessage(&daMP_ReadedBufferQueue, buffer, 1);
}

void* daMP_PopFreeReadBuffer() {
    OSMessage buffer;
    OSReceiveMessage(&daMP_FreeReadBufferQueue, &buffer, 1);
    return buffer;
}

void daMP_PushFreeReadBuffer(void* buffer) {
    OSSendMessage(&daMP_FreeReadBufferQueue, buffer, 1);
}

static OSMessageQueue daMP_ReadedBufferQueue2;

void* daMP_PopReadedBuffer2() {
    OSMessage buffer;
    OSReceiveMessage(&daMP_ReadedBufferQueue2, &buffer, 1);
    return buffer;
}

void daMP_PushReadedBuffer2(void* buffer) {
    OSSendMessage(&daMP_ReadedBufferQueue2, buffer, 1);
}

static OSMessage daMP_FreeReadBufferMessage[10];

static OSMessage daMP_ReadedBufferMessage[10];

static OSMessage daMP_ReadedBufferMessage2[10];

static OSThread daMP_ReadThread;

void daMP_ReadThreadStart() {
    if (daMP_ReadThreadCreated) {
        OSResumeThread(&daMP_ReadThread);
    }
}

void daMP_ReadThreadCancel() {
    if (daMP_ReadThreadCreated) {
        OSCancelThread(&daMP_ReadThread);
        daMP_ReadThreadCreated = FALSE;
    }
}

void* daMP_Reader(void*) {
    daMP_THPReadBuffer* buf;
	s32 curFrame;
	s32 status;
	s32 offset;
	s32 initReadSize;
	s32 frame = 0;

    offset = daMP_ActivePlayer.initOffset;
	initReadSize = daMP_ActivePlayer.initReadSize;

	while (TRUE) {
		buf = (daMP_THPReadBuffer*)daMP_PopFreeReadBuffer();
		status = DVDReadPrio(&daMP_ActivePlayer.fileInfo, buf->ptr, initReadSize, offset, 2);
		if (status != initReadSize) {
			if (status == -1)
				daMP_ActivePlayer.dvdError = -1;
			if (frame == 0)
				daMP_PrepareReady(FALSE);

			OSSuspendThread(&daMP_ReadThread);
		}

		buf->frameNumber = frame;
		daMP_PushReadedBuffer(buf);
		offset += initReadSize;
		initReadSize = daMP_NEXT_READ_SIZE(buf);

        u32 numFrames = daMP_ActivePlayer.header.numFrames;
		curFrame = (frame + daMP_ActivePlayer.initReadFrame) % numFrames;
		if (curFrame == daMP_ActivePlayer.header.numFrames - 1) {
			if (daMP_ActivePlayer.playFlag & 1)
				offset = daMP_ActivePlayer.header.movieDataOffsets;
			else
				OSSuspendThread(&daMP_ReadThread);
		}

		frame++;
	}
}

static u8 daMP_ReadThreadStack[0x2000];

static BOOL daMP_VideoDecodeThreadCreated;

static BOOL daMP_CreateReadThread(s32 param_0) {
    if (!OSCreateThread(&daMP_ReadThread, daMP_Reader, 0, daMP_ReadThreadStack + sizeof(daMP_ReadThreadStack), sizeof(daMP_ReadThreadStack), param_0, 1)) {
        OSReport("Can't create read thread\n");
        return FALSE;
    }

    OSInitMessageQueue(&daMP_FreeReadBufferQueue, daMP_FreeReadBufferMessage, 10);
    OSInitMessageQueue(&daMP_ReadedBufferQueue, daMP_ReadedBufferMessage, 10);
    OSInitMessageQueue(&daMP_ReadedBufferQueue2, daMP_ReadedBufferMessage2, 10);
    daMP_ReadThreadCreated = TRUE;
    return TRUE;
}

static OSThread daMP_VideoDecodeThread;

static u8 daMP_VideoDecodeThreadStack[0x64000];

static OSMessageQueue daMP_FreeTextureSetQueue;

void* daMP_PopFreeTextureSet() {
    OSMessage tex;
    OSReceiveMessage(&daMP_FreeTextureSetQueue, &tex, 1);
    return tex;
}

void daMP_PushFreeTextureSet(void* tex) {
    OSSendMessage(&daMP_FreeTextureSetQueue, tex, 0);
}

static OSMessageQueue daMP_DecodedTextureSetQueue;

void* daMP_PopDecodedTextureSet(s32 flags) {
    OSMessage tex;
    if (OSReceiveMessage(&daMP_DecodedTextureSetQueue, &tex, flags) == TRUE) {
        return tex;
    } else {
        return NULL;
    }
}

void daMP_PushDecodedTextureSet(void* tex) {
    OSSendMessage(&daMP_DecodedTextureSetQueue, tex, 1);
}

static OSMessage daMP_FreeTextureSetMessage[3];

static OSMessage daMP_DecodedTextureSetMessage[3];

static BOOL daMP_First;

static void daMP_VideoDecode(daMP_THPReadBuffer* readBuffer) {
    THPTextureSet* textureSet;
	s32 i;
	u32* tileOffsets;
	u8* tile;

    tileOffsets = (u32*)(readBuffer->ptr + 8);
	tile = &readBuffer->ptr[daMP_ActivePlayer.compInfo.numComponents * 4] + 8;
	textureSet = (THPTextureSet*)daMP_PopFreeTextureSet();

    for (i = 0; i < daMP_ActivePlayer.compInfo.numComponents; i++) {
        switch (daMP_ActivePlayer.compInfo.frameComp[i]) {
		case 0: {
			if ((daMP_ActivePlayer.videoError = THPVideoDecode(
			         tile, textureSet->ytexture, textureSet->utexture,
			         textureSet->vtexture, daMP_ActivePlayer.thpWork))) {
				if (daMP_First) {
					daMP_PrepareReady(FALSE);
					daMP_First = FALSE;
				}
				OSSuspendThread(&daMP_VideoDecodeThread);
			}
			textureSet->frameNumber = readBuffer->frameNumber;
			daMP_PushDecodedTextureSet(textureSet);
			BOOL enable = OSDisableInterrupts();
			daMP_ActivePlayer.videoDecodeCount++;
			OSRestoreInterrupts(enable);
		}
		}

        tile += *tileOffsets;
		tileOffsets++;
    }

    if (daMP_First) {
        daMP_PrepareReady(1);
        daMP_First = 0;
    }
}

static void* daMP_VideoDecoder(void* param_0) {
    daMP_THPReadBuffer* thpBuffer;

	while (TRUE) {
		if (daMP_ActivePlayer.audioExist) {
			for (; daMP_ActivePlayer.videoDecodeCount < 0;) {
				thpBuffer = (daMP_THPReadBuffer*)daMP_PopReadedBuffer2();
				s32 remaining
				    = ((thpBuffer->frameNumber + daMP_ActivePlayer.initReadFrame)
				       % daMP_ActivePlayer.header.numFrames);
				if (remaining == (daMP_ActivePlayer.header.numFrames - 1)
				    && (daMP_ActivePlayer.playFlag & 1) == 0)
					daMP_VideoDecode(thpBuffer);

				daMP_PushFreeReadBuffer(thpBuffer);
				BOOL enable = OSDisableInterrupts();
				daMP_ActivePlayer.videoDecodeCount++;
				OSRestoreInterrupts(enable);
			}
		}

		if (daMP_ActivePlayer.audioExist)
			thpBuffer = (daMP_THPReadBuffer*)daMP_PopReadedBuffer2();
		else
			thpBuffer = (daMP_THPReadBuffer*)daMP_PopReadedBuffer();

		daMP_VideoDecode(thpBuffer);
		daMP_PushFreeReadBuffer(thpBuffer);
	}
}

static void* daMP_VideoDecoderForOnMemory(void* param_0) {
    daMP_THPReadBuffer readBuffer;
	s32 readSize;
	s32 frame;
	s32 remaining;

	readSize = daMP_ActivePlayer.initReadSize;
	readBuffer.ptr = (u8*)param_0;
    frame = 0;

	while (TRUE) {
		if (daMP_ActivePlayer.audioExist) {
			while (daMP_ActivePlayer.videoDecodeCount < 0) {
				BOOL enable = OSDisableInterrupts();
				daMP_ActivePlayer.videoDecodeCount++;
				OSRestoreInterrupts(enable);
				remaining = (frame + daMP_ActivePlayer.initReadFrame)
				            % daMP_ActivePlayer.header.numFrames;
				if (remaining == daMP_ActivePlayer.header.numFrames - 1) {
					if ((daMP_ActivePlayer.playFlag & 1) == 0)
						break;

					readSize       = *(s32*)readBuffer.ptr;
					readBuffer.ptr = daMP_ActivePlayer.movieData;
				} else {
					s32 size = *(s32*)readBuffer.ptr;
					readBuffer.ptr += readSize;
					readSize = size;
				}
				frame++;
			}
		}

		readBuffer.frameNumber = frame;

		daMP_VideoDecode(&readBuffer);

		remaining = (frame + daMP_ActivePlayer.initReadFrame)
		            % daMP_ActivePlayer.header.numFrames;
		if (remaining == daMP_ActivePlayer.header.numFrames - 1) {
			if ((daMP_ActivePlayer.playFlag & 1)) {
				readSize = *(s32*)readBuffer.ptr;
				readBuffer.ptr = daMP_ActivePlayer.movieData;
			} else {
				OSSuspendThread(&daMP_VideoDecodeThread);
			}
		} else {
			s32 size = *(s32*)readBuffer.ptr;
			readBuffer.ptr += readSize;
			readSize = size;
		}

		frame++;
	}
}

static BOOL daMP_CreateVideoDecodeThread(OSPriority prio, u8* param_1) {
    if (param_1 != NULL) {
        if (!OSCreateThread(&daMP_VideoDecodeThread, daMP_VideoDecoderForOnMemory, param_1, daMP_VideoDecodeThreadStack + sizeof(daMP_VideoDecodeThreadStack), sizeof(daMP_VideoDecodeThreadStack), prio, 1)) {
            OSReport("Can't create video decode thread\n");
            return FALSE;
        }
    } else {
        if (!OSCreateThread(&daMP_VideoDecodeThread, daMP_VideoDecoder, NULL, daMP_VideoDecodeThreadStack + sizeof(daMP_VideoDecodeThreadStack), sizeof(daMP_VideoDecodeThreadStack), prio, 1)) {
            OSReport("Can't create video decode thread\n");
            return FALSE;
        }
    }

    OSInitMessageQueue(&daMP_FreeTextureSetQueue, daMP_FreeTextureSetMessage, 3);
    OSInitMessageQueue(&daMP_DecodedTextureSetQueue, daMP_DecodedTextureSetMessage, 3);

    daMP_First = daMP_VideoDecodeThreadCreated = TRUE;
    return TRUE;
}

static void daMP_VideoDecodeThreadStart() {
    if (daMP_VideoDecodeThreadCreated) {
        OSResumeThread(&daMP_VideoDecodeThread);
    }
}

void daMP_VideoDecodeThreadCancel() {
    if (daMP_VideoDecodeThreadCreated) {
        OSCancelThread(&daMP_VideoDecodeThread);
        daMP_VideoDecodeThreadCreated = FALSE;
    }
}

static BOOL daMP_AudioDecodeThreadCreated;

static OSThread daMP_AudioDecodeThread;

static u8 daMP_AudioDecodeThreadStack[0x64000];

static OSMessageQueue daMP_FreeAudioBufferQueue;

static void* daMP_PopFreeAudioBuffer() {
    OSMessage buffer;
    OSReceiveMessage(&daMP_FreeAudioBufferQueue, &buffer, OS_MESSAGE_BLOCK);
    return buffer;
}

static void daMP_PushFreeAudioBuffer(void* buffer) {
    OSSendMessage(&daMP_FreeAudioBufferQueue, buffer, OS_MESSAGE_NOBLOCK);
}

static OSMessageQueue daMP_DecodedAudioBufferQueue;

static void* daMP_PopDecodedAudioBuffer(s32 flags) {
    OSMessage buffer;
    if (OSReceiveMessage(&daMP_DecodedAudioBufferQueue, &buffer, flags) == 1) {
        return buffer;
    }

    return NULL;
}

static void daMP_PushDecodedAudioBuffer(void* buffer) {
    OSSendMessage(&daMP_DecodedAudioBufferQueue, buffer, OS_MESSAGE_BLOCK);
}

static void daMP_AudioDecode(daMP_THPReadBuffer* readBuffer) {
    THPAudioBuffer* audioBuf;
	s32 i;
	u32* offsets;
	u8* audioData;

	offsets = (u32*)(readBuffer->ptr + 8);
	audioData = &readBuffer->ptr[daMP_ActivePlayer.compInfo.numComponents * 4] + 8;
	audioBuf = (THPAudioBuffer*)daMP_PopFreeAudioBuffer();

	for (i = 0; i < daMP_ActivePlayer.compInfo.numComponents; i++) {
		switch (daMP_ActivePlayer.compInfo.frameComp[i]) {
		case 1: {
			audioBuf->validSample = THPAudioDecode(
			    audioBuf->buffer,
			    (audioData + *offsets * daMP_ActivePlayer.curAudioTrack), 0);
			audioBuf->curPtr = audioBuf->buffer;
			daMP_PushDecodedAudioBuffer(audioBuf);
			return;
		}
		}

		audioData += *offsets;
		offsets++;
	}
}

static void* daMP_AudioDecoder(void* param_0) {
    daMP_THPReadBuffer* buf;

    while (TRUE) {
        buf = (daMP_THPReadBuffer*)daMP_PopReadedBuffer();
        daMP_AudioDecode(buf);
        daMP_PushReadedBuffer2(buf);
    }
}

static void* daMP_AudioDecoderForOnMemory(void* param_0) {
    s32 size;
	s32 readSize;
	daMP_THPReadBuffer readBuffer;
    s32 frame;
    s32 remaining;

	readSize = daMP_ActivePlayer.initReadSize;
	readBuffer.ptr = (u8*)param_0;
    frame = 0;

	while (TRUE) {
		readBuffer.frameNumber = frame;
		daMP_AudioDecode(&readBuffer);

		remaining = (frame + daMP_ActivePlayer.initReadFrame) % daMP_ActivePlayer.header.numFrames;
		if (remaining == daMP_ActivePlayer.header.numFrames - 1) {
			if ((daMP_ActivePlayer.playFlag & 1)) {
				readSize = *(s32*)readBuffer.ptr;
				readBuffer.ptr = daMP_ActivePlayer.movieData;
			} else {
				OSSuspendThread(&daMP_AudioDecodeThread);
			}
		} else {
			size = *(s32*)readBuffer.ptr;
			readBuffer.ptr += readSize;
			readSize = size;
		}
		frame++;
	}
}

static OSMessage daMP_FreeAudioBufferMessage[3];

static OSMessage daMP_DecodedAudioBufferMessage[3];

static BOOL daMP_CreateAudioDecodeThread(OSPriority prio, u8* param_1) {
    if (param_1 != NULL) {
        if (!OSCreateThread(&daMP_AudioDecodeThread, daMP_AudioDecoderForOnMemory, param_1, daMP_AudioDecodeThreadStack + sizeof(daMP_AudioDecodeThreadStack), sizeof(daMP_AudioDecodeThreadStack), prio, 1)) {
            OS_REPORT("Can't create audio decode thread\n");
            return FALSE;
        }
    } else {
        if (!OSCreateThread(&daMP_AudioDecodeThread, daMP_AudioDecoder, NULL, daMP_AudioDecodeThreadStack + sizeof(daMP_AudioDecodeThreadStack), sizeof(daMP_AudioDecodeThreadStack), prio, 1)) {
            OSReport("Can't create audio decode thread\n");
            return FALSE;
        }
    }

    OSInitMessageQueue(&daMP_FreeAudioBufferQueue, daMP_FreeAudioBufferMessage, 3);
    OSInitMessageQueue(&daMP_DecodedAudioBufferQueue, daMP_DecodedAudioBufferMessage, 3);

    daMP_AudioDecodeThreadCreated = TRUE;
    return TRUE;
}

void daMP_AudioDecodeThreadStart() {
    if (daMP_AudioDecodeThreadCreated) {
        OSResumeThread(&daMP_AudioDecodeThread);
    }
}

void daMP_AudioDecodeThreadCancel() {
    if (daMP_AudioDecodeThreadCreated) {
        OSCancelThread(&daMP_AudioDecodeThread);
        daMP_AudioDecodeThreadCreated = FALSE;
    }
}

static void daMP_THPGXRestore() {
    GXSetZMode(GX_ENABLE, GX_ALWAYS, GX_DISABLE);
    GXSetBlendMode(GX_BM_NONE, GX_BL_ONE, GX_BL_ZERO, GX_LO_SET);
    GXSetNumTexGens(1);
    GXSetNumChans(0);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevOp(GX_TEVSTAGE0, GX_REPLACE);
    GXSetTevSwapMode(GX_TEVSTAGE0, GX_TEV_SWAP0, GX_TEV_SWAP0);
    GXSetTevSwapMode(GX_TEVSTAGE1, GX_TEV_SWAP0, GX_TEV_SWAP0);
    GXSetTevSwapMode(GX_TEVSTAGE2, GX_TEV_SWAP0, GX_TEV_SWAP0);
    GXSetTevSwapMode(GX_TEVSTAGE3, GX_TEV_SWAP0, GX_TEV_SWAP0);
    GXSetTevSwapModeTable(GX_TEV_SWAP0, GX_CH_RED, GX_CH_GREEN, GX_CH_BLUE, GX_CH_ALPHA);
    GXSetTevSwapModeTable(GX_TEV_SWAP1, GX_CH_RED, GX_CH_RED, GX_CH_RED, GX_CH_ALPHA);
    GXSetTevSwapModeTable(GX_TEV_SWAP2, GX_CH_GREEN, GX_CH_GREEN, GX_CH_GREEN, GX_CH_ALPHA);
    GXSetTevSwapModeTable(GX_TEV_SWAP3, GX_CH_BLUE, GX_CH_BLUE, GX_CH_BLUE, GX_CH_ALPHA);
}

static f32 dummyLiteral() {
    f32 temp = 100.0f;
    temp += 60.0f;
    return temp;
}

static void daMP_THPGXYuv2RgbSetup(const GXRenderModeObj* rmode) {
    s32 w, h;
    f32 var_f31;
	Mtx44 m;
	Mtx e_m;

    w = rmode->fbWidth;
    h = rmode->efbHeight;
    var_f31 = 0.0f;

    #if WIDESCREEN_SUPPORT
    if (!mDoGph_gInf_c::isWide()) {
        var_f31 = (rmode->efbHeight - (h * 808.0f) / 608.0f) * 0.5f;
    }
    #endif

    GXSetPixelFmt(GX_PF_RGB8_Z24, GX_ZC_LINEAR);
    C_MTXOrtho(m, var_f31, h - var_f31, 0.0f, w, 0.0f, -1.0f);
    GXSetProjection(m, GX_ORTHOGRAPHIC);
    GXSetViewport(0.0f, 0.0f, w, h, 0.0f, 1.0f);
    GXSetScissor(0, 0, w, h);
    MTXIdentity(e_m);
    GXLoadPosMtxImm(e_m, GX_PNMTX0);
    GXSetCurrentMtx(0);
    GXSetZCompLoc(GX_TRUE);
    GXSetZMode(GX_ENABLE, GX_ALWAYS, GX_DISABLE);
    GXSetBlendMode(GX_BM_NONE, GX_BL_ONE, GX_BL_ZERO, GX_LO_CLEAR);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_OR, GX_ALWAYS, 0);
    GXSetFog(GX_FOG_NONE, 0.0f, 0.0f, 0.0f, 0.0f, g_clearColor);
    GXSetFogRangeAdj(GX_DISABLE, 0, NULL);
    GXSetCullMode(GX_CULL_NONE);
    GXSetDither(GX_ENABLE);
    GXSetColorUpdate(GX_ENABLE);
    GXSetAlphaUpdate(GX_DISABLE);
    GXSetNumIndStages(0);
    GXSetNumChans(0);
    GXSetNumTexGens(2);
    GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 60);
    GXSetTexCoordGen(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX0, 60);
    GXInvalidateTexAll();
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT7, GX_VA_POS, GX_CLR_RGBA, GX_RGBA4, 0);
    GXSetVtxAttrFmt(GX_VTXFMT7, GX_VA_TEX0, GX_CLR_RGBA, GX_RGBX8, 0);
    GXSetNumTevStages(4);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD1, GX_TEXMAP1, GX_COLOR_NULL);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_TEXC, GX_CC_KONST, GX_CC_C0);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_FALSE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_TEXA, GX_CA_KONST, GX_CA_A0);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_SUB, GX_TB_ZERO, GX_CS_SCALE_1, GX_FALSE, GX_TEVPREV);
    GXSetTevKColorSel(GX_TEVSTAGE0, GX_TEV_KCSEL_K0);
    GXSetTevKAlphaSel(GX_TEVSTAGE0, GX_TEV_KASEL_K0_A);
    GXSetTevSwapMode(GX_TEVSTAGE0, GX_TEV_SWAP0, GX_TEV_SWAP0);
    GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD1, GX_TEXMAP2, GX_COLOR_NULL);
    GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_ZERO, GX_CC_TEXC, GX_CC_KONST, GX_CC_CPREV);
    GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_2, GX_FALSE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_TEXA, GX_CA_KONST, GX_CA_APREV);
    GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_SUB, GX_TB_ZERO, GX_CS_SCALE_1, GX_FALSE, GX_TEVPREV);
    GXSetTevKColorSel(GX_TEVSTAGE1, GX_TEV_KCSEL_K1);
    GXSetTevKAlphaSel(GX_TEVSTAGE1, GX_TEV_KASEL_K1_A);
    GXSetTevSwapMode(GX_TEVSTAGE1, GX_TEV_SWAP0, GX_TEV_SWAP0);
    GXSetTevOrder(GX_TEVSTAGE2, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorIn(GX_TEVSTAGE2, GX_CC_ZERO, GX_CC_TEXC, GX_CC_ONE, GX_CC_CPREV);
    GXSetTevColorOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE2, GX_CA_TEXA, GX_CA_ZERO, GX_CA_ZERO, GX_CA_APREV);
    GXSetTevAlphaOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevSwapMode(GX_TEVSTAGE2, GX_TEV_SWAP0, GX_TEV_SWAP0);
    GXSetTevOrder(GX_TEVSTAGE3, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
    GXSetTevColorIn(GX_TEVSTAGE3, GX_CC_APREV, GX_CC_CPREV, GX_CC_KONST, GX_CC_ZERO);
    GXSetTevColorOp(GX_TEVSTAGE3, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE3, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE3, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevSwapMode(GX_TEVSTAGE3, GX_TEV_SWAP0, GX_TEV_SWAP0);
    GXSetTevKColorSel(GX_TEVSTAGE3, GX_TEV_KCSEL_K2);

    GXColorS10 spA8 = {-0x5A, 0x00, -0x72, 0x87};
    GXSetTevColorS10(GX_TEVREG0, spA8);

    GXColor spB0 = {0x00, 0x00, 0xE2, 0x58};
    GXSetTevKColor(GX_KCOLOR0, spB0);

    GXColor spB4 = {0xB3, 0x00, 0x00, 0xB6};
    GXSetTevKColor(GX_KCOLOR1, spB4);

    GXColor spB8 = {0xFF, 0x00, 0xFF, 0x80};
    GXSetTevKColor(GX_KCOLOR2, spB8);

    GXSetTevSwapModeTable(GX_TEV_SWAP0, GX_CH_RED, GX_CH_GREEN, GX_CH_BLUE, GX_CH_ALPHA);
}

static void daMP_THPGXYuv2RgbDraw(u8* y_data, u8* u_data, u8* v_data, s16 x,
                                  s16 y, s16 textureWidth, s16 textureHeight, s16 polygonWidth,
                                  s16 polygonHeight) {
    GXTexObj tobj0;
    GXTexObj tobj1;
    GXTexObj tobj2;

    GXInitTexObj(&tobj0, y_data, textureWidth, textureHeight, GX_TF_I8, GX_CLAMP, GX_CLAMP, GX_FALSE);
    GXInitTexObjLOD(&tobj0, GX_NEAR, GX_NEAR, 0.0f, 0.0f, 0.0f, 0, 0, GX_ANISO_1);
    GXLoadTexObj(&tobj0, GX_TEXMAP0);

    GXInitTexObj(&tobj1, u_data, textureWidth >> 1, textureHeight >> 1, GX_TF_I8, GX_CLAMP, GX_CLAMP, GX_FALSE);
    GXInitTexObjLOD(&tobj1, GX_NEAR, GX_NEAR, 0.0f, 0.0f, 0.0f, 0, 0, GX_ANISO_1);
    GXLoadTexObj(&tobj1, GX_TEXMAP1);

    GXInitTexObj(&tobj2, v_data, textureWidth >> 1, textureHeight >> 1, GX_TF_I8, GX_CLAMP, GX_CLAMP, GX_FALSE);
    GXInitTexObjLOD(&tobj2, GX_NEAR, GX_NEAR, 0.0f, 0.0f, 0.0f, 0, 0, GX_ANISO_1);
    GXLoadTexObj(&tobj2, GX_TEXMAP2);

    GXBegin(GX_QUADS, GX_VTXFMT7, 4);
    GXPosition3s16(x, y, 0);
    GXTexCoord2u16(0, 0);
    GXPosition3s16(x + polygonWidth, y, 0);
    GXTexCoord2u16(1, 0);
    GXPosition3s16(x + polygonWidth, y + polygonHeight, 0);
    GXTexCoord2u16(1, 1);
    GXPosition3s16(x, y + polygonHeight, 0);
    GXTexCoord2u16(0, 1);
    GXEnd();
}

static u16 daMP_VolumeTable[] = {
    0x0000, 0x0002, 0x0008, 0x0012, 0x0020, 0x0032, 0x0049, 0x0063,
    0x0082, 0x00A4, 0x00CB, 0x00F5, 0x0124, 0x0157, 0x018E, 0x01C9,
    0x0208, 0x024B, 0x0292, 0x02DD, 0x032C, 0x037F, 0x03D7, 0x0432,
    0x0492, 0x04F5, 0x055D, 0x05C9, 0x0638, 0x06AC, 0x0724, 0x07A0,
    0x0820, 0x08A4, 0x092C, 0x09B8, 0x0A48, 0x0ADD, 0x0B75, 0x0C12,
    0x0CB2, 0x0D57, 0x0DFF, 0x0EAC, 0x0F5D, 0x1012, 0x10CA, 0x1187,
    0x1248, 0x130D, 0x13D7, 0x14A4, 0x1575, 0x164A, 0x1724, 0x1801,
    0x18E3, 0x19C8, 0x1AB2, 0x1BA0, 0x1C91, 0x1D87, 0x1E81, 0x1F7F,
    0x2081, 0x2187, 0x2291, 0x239F, 0x24B2, 0x25C8, 0x26E2, 0x2801,
    0x2923, 0x2A4A, 0x2B75, 0x2CA3, 0x2DD6, 0x2F0D, 0x3048, 0x3187,
    0x32CA, 0x3411, 0x355C, 0x36AB, 0x37FF, 0x3956, 0x3AB1, 0x3C11,
    0x3D74, 0x3EDC, 0x4048, 0x41B7, 0x432B, 0x44A3, 0x461F, 0x479F,
    0x4923, 0x4AAB, 0x4C37, 0x4DC7, 0x4F5C, 0x50F4, 0x5290, 0x5431,
    0x55D6, 0x577E, 0x592B, 0x5ADC, 0x5C90, 0x5E49, 0x6006, 0x61C7,
    0x638C, 0x6555, 0x6722, 0x68F4, 0x6AC9, 0x6CA2, 0x6E80, 0x7061,
    0x7247, 0x7430, 0x761E, 0x7810, 0x7A06, 0x7C00, 0x7DFE, 0x8000,
};

#pragma push
#pragma optimization_level 3
static void daMP_MixAudio(s16* destination, s16*, u32 sample) {
    if (daMP_ActivePlayer.open && daMP_ActivePlayer.internalState == 2 && daMP_ActivePlayer.audioExist) {
		u32 sampleNum;
		u32 requestSample;
		s32 i;
		s16* dst;
		s16* curPtr;
		s32 l_mix, r_mix;
		u16 attenuation;

		requestSample = sample;
		dst = destination;

        BOOL loop = TRUE;
		do {
			do {
				if (daMP_ActivePlayer.playAudioBuffer == (THPAudioBuffer*)NULL) {
					if (!(daMP_ActivePlayer.playAudioBuffer = (THPAudioBuffer*)daMP_PopDecodedAudioBuffer(0))) {
						memset(dst, 0, requestSample * 4);
						return;
					}
					daMP_ActivePlayer.curAudioNumber++;
				}
			} while ((sampleNum = daMP_ActivePlayer.playAudioBuffer->validSample) == 0);

            if (sampleNum >= requestSample) {
                sampleNum = requestSample;
            }

			curPtr = daMP_ActivePlayer.playAudioBuffer->curPtr;

			for (i = 0; i < sampleNum; i++) {
				if (daMP_ActivePlayer.rampCount != 0) {
					daMP_ActivePlayer.rampCount--;
					daMP_ActivePlayer.curVolume += daMP_ActivePlayer.deltaVolume;
				} else {
					daMP_ActivePlayer.curVolume = daMP_ActivePlayer.targetVolume;
				}

				attenuation = daMP_VolumeTable[(s32)daMP_ActivePlayer.curVolume];

				l_mix = 0.7f * (attenuation * curPtr[0] >> 15);
				// clamp volume
				if (l_mix < -32768)
					l_mix = -32768;
				if (l_mix > 32767)
					l_mix = 32767;

				r_mix = 0.7f * (attenuation * curPtr[1] >> 15);
				if (r_mix < -32768)
					r_mix = -32768;
				if (r_mix > 32767)
					r_mix = 32767;

                if (JASDriver::getOutputMode() == 0) {
                    l_mix = r_mix = ((r_mix >> 1) + (l_mix >> 1));
                    r_mix = (s16)r_mix;
                    l_mix = (s16)l_mix;
                }

                dst[0] = l_mix;
				dst[1] = r_mix;

				dst += 2;
				curPtr += 2;
			}

			requestSample -= sampleNum;
			daMP_ActivePlayer.playAudioBuffer->validSample -= sampleNum;
			daMP_ActivePlayer.playAudioBuffer->curPtr = curPtr;

			if ((daMP_ActivePlayer.playAudioBuffer)->validSample == 0) {
				daMP_PushFreeAudioBuffer(daMP_ActivePlayer.playAudioBuffer);
				daMP_ActivePlayer.playAudioBuffer = (THPAudioBuffer*)NULL;
			}

			if (requestSample == 0) {
				break;
			}

		} while (loop);
	} else {
		memset(destination, 0, sample * 4);
	}
}
#pragma pop

static BOOL daMP_Initialized;

static u32 daMP_WorkBuffer[16] ATTRIBUTE_ALIGN(32);

static OSMessageQueue daMP_PrepareReadyQueue;

static OSMessageQueue daMP_UsedTextureSetQueue;

static OSMessage daMP_PrepareReadyMessage;

static OSMessage daMP_UsedTextureSetMessage[3];

static VIRetraceCallback daMP_OldVIPostCallback;

static u32 daMP_SoundBufferIndex;

static u32 daMP_OldAIDCallback;

static void* daMP_LastAudioBuffer;

static void* daMP_CurAudioBuffer;

static s32 daMP_AudioSystem;

static u8 daMP_SoundBuffer[2][0x8C0] ATTRIBUTE_ALIGN(32);

static s16* daMP_audioCallbackWithMSound(s32 sample) {
    if (daMP_ActivePlayer.open == 0 || daMP_ActivePlayer.internalState != 2 || daMP_ActivePlayer.audioExist == 0) {
        return NULL;
    }

    BOOL enable = OSEnableInterrupts();
    daMP_SoundBufferIndex ^= 1;
    daMP_MixAudio((s16*)daMP_SoundBuffer[daMP_SoundBufferIndex], NULL, sample);
    OSRestoreInterrupts(enable);
    return (s16*)daMP_SoundBuffer[daMP_SoundBufferIndex];
}

static void daMP_audioInitWithMSound() {
    JASDriver::registerMixCallback(daMP_audioCallbackWithMSound, MIX_MODE_INTERLEAVE);
}

static void daMP_audioQuitWithMSound() {
    JASDriver::registerMixCallback(NULL, MIX_MODE_MONO);
}

static void daMP_PushUsedTextureSet(void* tex) {
    OSSendMessage(&daMP_UsedTextureSetQueue, tex, OS_MESSAGE_NOBLOCK);
}

static void* daMP_PopUsedTextureSet() {
    OSMessage tex;
    if (OSReceiveMessage(&daMP_UsedTextureSetQueue, &tex, OS_MESSAGE_NOBLOCK) == 1) {
        return tex;
    }

    return NULL;
}

static int daMP_THPPlayerInit(s32 param_0) {
    BOOL enable;

    ASSERTMSGLINE(7593, param_0 >= 0 && param_0 <= 2, "audioSystem flag is invalid\n");

    memset(&daMP_ActivePlayer, 0, sizeof(daMP_ActivePlayer));
    LCEnable();

    OSInitMessageQueue(&daMP_UsedTextureSetQueue, daMP_UsedTextureSetMessage, 3);

    if (!THPInit()) {
        return 0;
    }

    enable = OSDisableInterrupts();
    daMP_AudioSystem = param_0;
    daMP_SoundBufferIndex = 0;
    daMP_LastAudioBuffer = NULL;
    daMP_CurAudioBuffer = NULL;
    daMP_audioInitWithMSound();
    OSRestoreInterrupts(enable);

    if (daMP_AudioSystem == 0) {
        memset(daMP_SoundBuffer, 0, sizeof(daMP_SoundBuffer));
        DCStoreRange(daMP_SoundBuffer, sizeof(daMP_SoundBuffer));
    }

    daMP_Initialized = TRUE;
    return 1;
}

static void daMP_THPPlayerQuit() {
    LCDisable();
    daMP_audioQuitWithMSound();
    daMP_Initialized = FALSE;
    daMP_ActivePlayer.dvdError = 0;
    daMP_ActivePlayer.videoError = 0;
}

static BOOL daMP_THPPlayerOpen(char const* filename, BOOL onMemory) {
    s32 offset;
	s32 i;

    if (!daMP_Initialized) {
        OSReport("You must call daMP_THPPlayerInit before you call this function\n");
        return 0;
    }

    if (daMP_ActivePlayer.open) {
        OSReport("Can't open %s. Because thp file have already opened.\n");
        return 0;
    }

    memset(&daMP_ActivePlayer.videoInfo, 0, sizeof(THPVideoInfo));
    memset(&daMP_ActivePlayer.audioInfo, 0, sizeof(THPAudioInfo));

    if (!DVDOpen(filename, &daMP_ActivePlayer.fileInfo)) {
        OSReport("Can't open %s.\n", filename);
        return 0;
    }

    if (DVDReadPrio(&daMP_ActivePlayer.fileInfo, daMP_WorkBuffer, sizeof(daMP_WorkBuffer), 0, 2) < 0) {
        OSReport("Fail to read the header from THP file.\n");
        DVDClose(&daMP_ActivePlayer.fileInfo);
        return 0;
    }

    memcpy(&daMP_ActivePlayer.header, daMP_WorkBuffer, sizeof(THPHeader));

    if (strcmp(daMP_ActivePlayer.header.magic, "THP") != 0) {
        OSReport("This file is not THP file.\n");
        DVDClose(&daMP_ActivePlayer.fileInfo);
        return 0;
    }

    if (daMP_ActivePlayer.header.version != 0x11000) {
        OSReport("invalid version.\n");
        DVDClose(&daMP_ActivePlayer.fileInfo);
        return 0;
    }

    offset = daMP_ActivePlayer.header.compInfoDataOffsets;

    if (DVDReadPrio(&daMP_ActivePlayer.fileInfo, daMP_WorkBuffer, 0x20, offset, 2) < 0) {
        OSReport("Fail to read the frame component infomation from THP file.\n");
        DVDClose(&daMP_ActivePlayer.fileInfo);
        return 0;
    }

    memcpy(&daMP_ActivePlayer.compInfo, daMP_WorkBuffer, sizeof(THPFrameCompInfo));
    offset += sizeof(THPFrameCompInfo);

    daMP_ActivePlayer.audioExist = 0;

    for (i = 0; i < daMP_ActivePlayer.compInfo.numComponents; i++) {
        switch (daMP_ActivePlayer.compInfo.frameComp[i]) {
        case 0:
            if (DVDReadPrio(&daMP_ActivePlayer.fileInfo, daMP_WorkBuffer, 0x20, offset, 2) < 0) {
                OSReport("Fail to read the video infomation from THP file.\n");
                DVDClose(&daMP_ActivePlayer.fileInfo);
                return 0;
            }

            memcpy(&daMP_ActivePlayer.videoInfo, daMP_WorkBuffer, sizeof(THPVideoInfo));
            offset += sizeof(THPVideoInfo);
            break;
        case 1:
            if (DVDReadPrio(&daMP_ActivePlayer.fileInfo, daMP_WorkBuffer, 0x20, offset, 2) < 0) {
                OSReport("Fail to read the video infomation from THP file.\n");
                DVDClose(&daMP_ActivePlayer.fileInfo);
                return 0;
            }

            memcpy(&daMP_ActivePlayer.audioInfo, daMP_WorkBuffer, sizeof(THPAudioInfo));
            daMP_ActivePlayer.audioExist = 1;
            offset += sizeof(THPAudioInfo);
            break;
        default:
            OSReport("Unknow frame components.\n");
            return 0;
        }
    }

    daMP_ActivePlayer.internalState = 0;
    daMP_ActivePlayer.state = 0;
    daMP_ActivePlayer.playFlag = 0;
    daMP_ActivePlayer.onMemory = onMemory;
    daMP_ActivePlayer.open = 1;
    daMP_ActivePlayer.curVolume = 127.0f;
    daMP_ActivePlayer.targetVolume = 127.0f;
    daMP_ActivePlayer.rampCount = 0;
    return 1;
}

static BOOL daMP_THPPlayerClose() {
    if (daMP_ActivePlayer.open && daMP_ActivePlayer.state == 0) {
        daMP_ActivePlayer.open = 0;
        DVDClose(&daMP_ActivePlayer.fileInfo);
        return TRUE;
    }

    return FALSE;
}

static u32 daMP_THPPlayerCalcNeedMemory() {
    if (daMP_ActivePlayer.open) {
		u32 size = daMP_ActivePlayer.onMemory
		               ? ALIGN_NEXT(daMP_ActivePlayer.header.movieDataSize, 32)
		               : ALIGN_NEXT(daMP_ActivePlayer.header.bufsize, 32) * 10;

		size += ALIGN_NEXT(daMP_ActivePlayer.videoInfo.xSize * daMP_ActivePlayer.videoInfo.ySize, 32) * 3;
		size += ALIGN_NEXT(daMP_ActivePlayer.videoInfo.xSize * daMP_ActivePlayer.videoInfo.ySize / 4, 32) * 3;
		size += ALIGN_NEXT(daMP_ActivePlayer.videoInfo.xSize * daMP_ActivePlayer.videoInfo.ySize / 4, 32) * 3;

		if (daMP_ActivePlayer.audioExist) {
			size += ALIGN_NEXT(daMP_ActivePlayer.header.audioMaxSamples * 4, 32) * THP_AUDIO_BUFFER_COUNT;
		}
    
		return size + 0x1000;
	}

	return 0;
}

static BOOL daMP_THPPlayerSetBuffer(u8* buffer) {
    u32 i;
	u8* ptr;
	u32 ysize;
	u32 uvsize;

    ASSERTMSGLINE(7939, buffer != NULL, "buffer is NULL");

	if (daMP_ActivePlayer.open && daMP_ActivePlayer.state == 0) {
		ptr = buffer;
		if (daMP_ActivePlayer.onMemory) {
			daMP_ActivePlayer.movieData = buffer;
			ptr += daMP_ActivePlayer.header.movieDataSize;
		} else {
			for (i = 0; i < ARRAY_SIZE(daMP_ActivePlayer.readBuffer); i++) {
				daMP_ActivePlayer.readBuffer[i].ptr = ptr;
				ptr += ALIGN_NEXT(daMP_ActivePlayer.header.bufsize, 32);
			}
		}

		ysize = ALIGN_NEXT(daMP_ActivePlayer.videoInfo.xSize * daMP_ActivePlayer.videoInfo.ySize, 32);
		uvsize = ALIGN_NEXT(daMP_ActivePlayer.videoInfo.xSize * daMP_ActivePlayer.videoInfo.ySize / 4, 32);

		for (i = 0; i < ARRAY_SIZE(daMP_ActivePlayer.textureSet); i++) {
			daMP_ActivePlayer.textureSet[i].ytexture = ptr;

			DCInvalidateRange(ptr, ysize);
			ptr += ysize;

			daMP_ActivePlayer.textureSet[i].utexture = ptr;
			DCInvalidateRange(ptr, uvsize);
			ptr += uvsize;

			daMP_ActivePlayer.textureSet[i].vtexture = ptr;
			DCInvalidateRange(ptr, uvsize);
			ptr += uvsize;
		}

		if (daMP_ActivePlayer.audioExist) {
			for (i = 0; i < ARRAY_SIZE(daMP_ActivePlayer.audioBuffer); i++) {
				daMP_ActivePlayer.audioBuffer[i].buffer = (s16*)ptr;
				daMP_ActivePlayer.audioBuffer[i].curPtr = (s16*)ptr;
				daMP_ActivePlayer.audioBuffer[i].validSample = 0;
				ptr += ALIGN_NEXT(daMP_ActivePlayer.header.audioMaxSamples * 4, 32);
			}
		}

		daMP_ActivePlayer.thpWork = ptr;
		return TRUE;
	}

	return FALSE;
}

static void daMP_InitAllMessageQueue() {
    int i;
	if (daMP_ActivePlayer.onMemory == FALSE) {
		for (i = 0; i < THP_READ_BUFFER_COUNT; i++) {
			daMP_PushFreeReadBuffer(&daMP_ActivePlayer.readBuffer[i]);
		}
	}

	for (i = 0; i < THP_TEXTURE_SET_COUNT; i++) {
		daMP_PushFreeTextureSet(&daMP_ActivePlayer.textureSet[i]);
	}

	if (daMP_ActivePlayer.audioExist) {
		for (i = 0; i < THP_AUDIO_BUFFER_COUNT; i++) {
			daMP_PushFreeAudioBuffer(&daMP_ActivePlayer.audioBuffer[i]);
		}
	}

	OSInitMessageQueue(&daMP_PrepareReadyQueue, &daMP_PrepareReadyMessage, 1);
}

static BOOL daMP_ProperTimingForStart() {
    if (daMP_ActivePlayer.videoInfo.videoType & 1) {
		if (VIGetNextField() == 0)
			return TRUE;
	} else if (daMP_ActivePlayer.videoInfo.videoType & 2) {
		if (VIGetNextField() == 1)
			return TRUE;
	} else
		return TRUE;

	return FALSE;
}

static BOOL daMP_ProperTimingForGettingNextFrame() {
    if ((daMP_ActivePlayer.videoInfo.videoType & 1)) {
		if (VIGetNextField() == 0) {
			return TRUE;
		}
	} else if ((daMP_ActivePlayer.videoInfo.videoType & 2)) {
		if (VIGetNextField() == 1) {
			return TRUE;
		}
	} else {
		s32 frameRate = daMP_ActivePlayer.header.frameRate * 100.0f;
		if (VIGetTvFormat() == VI_PAL) {
			daMP_ActivePlayer.curCount = daMP_ActivePlayer.retaceCount * frameRate / 5000;
		} else {
			daMP_ActivePlayer.curCount = daMP_ActivePlayer.retaceCount * frameRate / 5994;
		}

		if (daMP_ActivePlayer.prevCount != daMP_ActivePlayer.curCount) {
			daMP_ActivePlayer.prevCount = daMP_ActivePlayer.curCount;
			return TRUE;
		}
	}

	return FALSE;
}

static void daMP_PlayControl(u32 retraceCnt) {
    THPTextureSet* decodedTexture;

	if (daMP_OldVIPostCallback != NULL)
		daMP_OldVIPostCallback(retraceCnt);

	decodedTexture = (THPTextureSet*)-1;
	if (daMP_ActivePlayer.open && daMP_ActivePlayer.state == 2) {
		if (daMP_ActivePlayer.dvdError || daMP_ActivePlayer.videoError) {
			daMP_ActivePlayer.internalState = 5;
			daMP_ActivePlayer.state = 5;
			return;
		}

		++daMP_ActivePlayer.retaceCount;

		if (daMP_ActivePlayer.retaceCount == 0) {
			if (daMP_ProperTimingForStart()) {
				if (daMP_ActivePlayer.audioExist) {
					if (daMP_ActivePlayer.curVideoNumber - daMP_ActivePlayer.curAudioNumber <= 1) {
						decodedTexture = (THPTextureSet*)daMP_PopDecodedTextureSet(0);
						daMP_ActivePlayer.videoDecodeCount--;
						daMP_ActivePlayer.curVideoNumber++;
					} else {
						daMP_ActivePlayer.internalState = 2;
					}
				} else {
					decodedTexture = (THPTextureSet*)daMP_PopDecodedTextureSet(0);
				}
			} else {
				daMP_ActivePlayer.retaceCount = -1;
			}
		} else {
			if (daMP_ActivePlayer.retaceCount == 1) {
				daMP_ActivePlayer.internalState = 2;
			}

			if (daMP_ProperTimingForGettingNextFrame()) {
				if (daMP_ActivePlayer.audioExist) {
					if (daMP_ActivePlayer.curVideoNumber - daMP_ActivePlayer.curAudioNumber <= 1) {
						decodedTexture = (THPTextureSet*)daMP_PopDecodedTextureSet(0);
						daMP_ActivePlayer.videoDecodeCount--;
						daMP_ActivePlayer.curVideoNumber++;
					}
				} else {
					decodedTexture = (THPTextureSet*)daMP_PopDecodedTextureSet(0);
				}
			}
		}

		if (decodedTexture != NULL && decodedTexture != (THPTextureSet*)-1) {
			if (daMP_ActivePlayer.dispTextureSet != NULL)
				daMP_PushUsedTextureSet(daMP_ActivePlayer.dispTextureSet);
			daMP_ActivePlayer.dispTextureSet = decodedTexture;
		}

		if ((daMP_ActivePlayer.playFlag & 1) == 0) {
			if (daMP_ActivePlayer.audioExist) {
				s32 audioFrame = daMP_ActivePlayer.curAudioNumber + daMP_ActivePlayer.initReadFrame;
				if (audioFrame == daMP_ActivePlayer.header.numFrames && daMP_ActivePlayer.playAudioBuffer == NULL) {
					daMP_ActivePlayer.internalState = 3;
					daMP_ActivePlayer.state = 3;
				}
			} else {
				s32 curFrame;
				if (daMP_ActivePlayer.dispTextureSet != NULL)
					curFrame = daMP_ActivePlayer.dispTextureSet->frameNumber + daMP_ActivePlayer.initReadFrame;
				else
					curFrame = daMP_ActivePlayer.initReadFrame - 1;

				if (curFrame == daMP_ActivePlayer.header.numFrames - 1 && decodedTexture == NULL) {
					daMP_ActivePlayer.internalState = 3;
					daMP_ActivePlayer.state = 3;
				}
			}
		}
	}
}

BOOL daMP_WaitUntilPrepare() {
    OSMessage msg;
    OSReceiveMessage(&daMP_PrepareReadyQueue, &msg, 1);
    
    if ((intptr_t)msg) {
		return TRUE;
	} else {
		return FALSE;
	}
}

void daMP_PrepareReady(BOOL msg) {
    OSSendMessage(&daMP_PrepareReadyQueue, (OSMessage)msg, 1);
}

static BOOL daMP_THPPlayerPrepare(s32 frame, s32 flag, s32 audioTrack) {
    u8* threadData;
	if (daMP_ActivePlayer.open && daMP_ActivePlayer.state == 0) {
		if (frame > 0) {
			if (daMP_ActivePlayer.header.offsetDataOffsets == 0) {
                OSReport("This thp file doesn't have the offset data\n");
                return FALSE;
            }

			if (daMP_ActivePlayer.header.numFrames > frame) {
                int offset = daMP_ActivePlayer.header.offsetDataOffsets + (frame - 1) * 4;
				if (DVDReadPrio(&daMP_ActivePlayer.fileInfo, daMP_WorkBuffer, 0x20, offset, 2) < 0) {
					OSReport("Fail to read the offset data from THP file.\n");
                    return FALSE;
                }

				daMP_ActivePlayer.initOffset = daMP_ActivePlayer.header.movieDataOffsets + daMP_WorkBuffer[0];
				daMP_ActivePlayer.initReadFrame = frame;
				daMP_ActivePlayer.initReadSize = daMP_WorkBuffer[1] - daMP_WorkBuffer[0];
			} else {
                OSReport("Specified frame number is over total frame number\n");
				return FALSE;
			}
		} else {
			daMP_ActivePlayer.initOffset = daMP_ActivePlayer.header.movieDataOffsets;
			daMP_ActivePlayer.initReadSize = daMP_ActivePlayer.header.firstFrameSize;
			daMP_ActivePlayer.initReadFrame = frame;
		}

		if (daMP_ActivePlayer.audioExist) {
			if (audioTrack < 0 || audioTrack >= daMP_ActivePlayer.audioInfo.sndNumTracks) {
                OSReport("Specified audio track number is invalid\n");
                return FALSE;
            }
			daMP_ActivePlayer.curAudioTrack = audioTrack;
		}

        flag &= 1;
		daMP_ActivePlayer.playFlag = flag;
		daMP_ActivePlayer.videoDecodeCount = 0;

		if (daMP_ActivePlayer.onMemory) {
			if (DVDReadPrio(&daMP_ActivePlayer.fileInfo, daMP_ActivePlayer.movieData, daMP_ActivePlayer.header.movieDataSize, daMP_ActivePlayer.header.movieDataOffsets, 2) < 0) {
				OSReport("Fail to read all movie data from THP file\n");
                return FALSE;
			}

			threadData = daMP_ActivePlayer.movieData + daMP_ActivePlayer.initOffset - daMP_ActivePlayer.header.movieDataOffsets;
			daMP_CreateVideoDecodeThread(20, threadData);

			if (daMP_ActivePlayer.audioExist)
				daMP_CreateAudioDecodeThread(12, threadData);
		} else {
			daMP_CreateVideoDecodeThread(20, 0);
			if (daMP_ActivePlayer.audioExist)
				daMP_CreateAudioDecodeThread(12, NULL);
			daMP_CreateReadThread(8);
		}

		daMP_InitAllMessageQueue();
		daMP_VideoDecodeThreadStart();

		if (daMP_ActivePlayer.audioExist)
			daMP_AudioDecodeThreadStart();
		if (daMP_ActivePlayer.onMemory == 0)
			daMP_ReadThreadStart();

		if (!daMP_WaitUntilPrepare())
			return FALSE;

		daMP_ActivePlayer.state = 1;
		daMP_ActivePlayer.internalState = 0;
		daMP_ActivePlayer.dispTextureSet = (THPTextureSet*)NULL;
		daMP_ActivePlayer.playAudioBuffer = (THPAudioBuffer*)NULL;
		daMP_ActivePlayer.curVideoNumber = 0;
		daMP_ActivePlayer.curAudioNumber = 0;

		daMP_OldVIPostCallback = VISetPostRetraceCallback(daMP_PlayControl);

		return TRUE;
	}

	return FALSE;
}

static void daMP_THPPlayerDrawDone() {
    GXDrawDone();

    if (daMP_Initialized) {
        while (TRUE) {
            void* tex = daMP_PopUsedTextureSet();
            if (tex == NULL) {
                break;
            }
            daMP_PushFreeTextureSet(tex);
        }
    }
}

static BOOL daMP_THPPlayerPlay() {
    if (daMP_ActivePlayer.open != 0 && (daMP_ActivePlayer.state == 1 || daMP_ActivePlayer.state == 4)) {
        daMP_ActivePlayer.state = 2;
        daMP_ActivePlayer.prevCount = 0;
        daMP_ActivePlayer.curCount = 0;
        daMP_ActivePlayer.retaceCount = -1;
        return TRUE;
    }

    return FALSE;
}

static void daMP_THPPlayerStop() {
    if (daMP_ActivePlayer.open != 0 && daMP_ActivePlayer.state != 0) {
        daMP_ActivePlayer.internalState = 0;
        daMP_ActivePlayer.state = 0;

        VISetPostRetraceCallback(daMP_OldVIPostCallback);

        if (daMP_ActivePlayer.onMemory == 0) {
            DVDCancel(&daMP_ActivePlayer.fileInfo.cb);
            daMP_ReadThreadCancel();
        }

        daMP_VideoDecodeThreadCancel();

        if (daMP_ActivePlayer.audioExist != 0) {
            daMP_AudioDecodeThreadCancel();
            daMP_audioQuitWithMSound();
        }

        while (daMP_PopUsedTextureSet() != NULL) {}

        daMP_ActivePlayer.curVolume = daMP_ActivePlayer.targetVolume;
        daMP_ActivePlayer.rampCount = 0.0f;
    }
}

static int daMP_THPPlayerPause() {
    if (daMP_ActivePlayer.open != 0 && daMP_ActivePlayer.state == 2) {
        daMP_ActivePlayer.internalState = 4;
        daMP_ActivePlayer.state = 4;
        return 1;
    }

    return 0;
}

static int daMP_THPPlayerDrawCurrentFrame(const GXRenderModeObj* rmode, u32 x,
                                          u32 y, u32 polygonW, u32 polygonH) {
    s32 frame;
	if (daMP_ActivePlayer.open && daMP_ActivePlayer.state != 0 && daMP_ActivePlayer.dispTextureSet != NULL) {
		daMP_THPGXYuv2RgbSetup(rmode);
		daMP_THPGXYuv2RgbDraw(daMP_ActivePlayer.dispTextureSet->ytexture,
		                 daMP_ActivePlayer.dispTextureSet->utexture,
		                 daMP_ActivePlayer.dispTextureSet->vtexture, x, y,
		                 daMP_ActivePlayer.videoInfo.xSize,
		                 daMP_ActivePlayer.videoInfo.ySize, polygonW, polygonH);
		daMP_THPGXRestore();
		frame = (daMP_ActivePlayer.dispTextureSet->frameNumber + daMP_ActivePlayer.initReadFrame) % daMP_ActivePlayer.header.numFrames;
        
        if (mDoGph_gInf_c::isFade()) {
            mDoGph_gInf_c::fadeIn(1.0f);
        }

        return frame;
    }

	return -1;
}

static int daMP_THPPlayerGetVideoInfo(THPVideoInfo* info) {
    if (daMP_ActivePlayer.open != 0) {
        memcpy(info, &daMP_ActivePlayer.videoInfo, sizeof(THPVideoInfo));
        return 1;
    }

    return 0;
}

static int daMP_THPPlayerGetAudioInfo(THPAudioInfo* info) {
    if (daMP_ActivePlayer.open != 0) {
        memcpy(info, &daMP_ActivePlayer.audioInfo, sizeof(THPAudioInfo));
        return 1;
    }

    return 0;
}

static u32 daMP_THPPlayerGetTotalFrame() {
    if (daMP_ActivePlayer.open != 0) {
        return daMP_ActivePlayer.header.numFrames;
    }

    return 0;
}

static int daMP_THPPlayerGetState() {
    return daMP_ActivePlayer.state;
}

static BOOL daMP_THPPlayerSetVolume(s32 vol, s32 duration) {
    u32 numSamples;
	BOOL interrupt;

	if (daMP_ActivePlayer.open && daMP_ActivePlayer.audioExist) {
		numSamples = AIGetDSPSampleRate() == 0 ? 32 : 48;

		// clamp volume
		if (vol > 127)
			vol = 127;
		if (vol < 0)
			vol = 0;

		// clamp duration
		if (duration > 60000)
			duration = 60000;
		if (duration < 0)
			duration = 0;

		interrupt = OSDisableInterrupts();

		daMP_ActivePlayer.targetVolume = vol;
		if (duration != 0) {
			daMP_ActivePlayer.rampCount = numSamples * duration;
			daMP_ActivePlayer.deltaVolume = (daMP_ActivePlayer.targetVolume - daMP_ActivePlayer.curVolume) / daMP_ActivePlayer.rampCount;
		} else {
			daMP_ActivePlayer.rampCount = 0;
			daMP_ActivePlayer.curVolume = daMP_ActivePlayer.targetVolume;
		}
		OSRestoreInterrupts(interrupt);

		return TRUE;
	}

	return FALSE;
}

static THPVideoInfo daMP_videoInfo;

static THPAudioInfo daMP_audioInfo;

static u32 daMP_DrawPosX;

static u32 daMP_DrawPosY;

static void* daMP_buffer;

static BOOL daMP_ActivePlayer_Init(char const* moviePath) {
    daMP_THPPlayerInit(0);
    
    if (!daMP_THPPlayerOpen(moviePath, 0)) {
        OSReport("Fail to open the thp file\n");
        #if DEBUG
        JUT_ASSERT(9135, FALSE);
        #else
        return 0;
        #endif
    }

    daMP_THPPlayerGetVideoInfo(&daMP_videoInfo);
    daMP_THPPlayerGetAudioInfo(&daMP_audioInfo);

    u16 width = JUTVideo::getManager()->getRenderMode()->fbWidth;
    u16 height = JUTVideo::getManager()->getRenderMode()->efbHeight;

    daMP_DrawPosX = (width - daMP_videoInfo.xSize) >> 1;
    daMP_DrawPosY = (height - daMP_videoInfo.ySize) >> 1;

    // "The memory needed for this THP movie is %d bytes\n"
    OS_REPORT("このＴＨＰムービーが必要なメモリは%dバイトです\n", daMP_THPPlayerCalcNeedMemory());

    daMP_buffer = mDoExt_getArchiveHeap()->alloc(daMP_THPPlayerCalcNeedMemory(), 0x20);
    if (daMP_buffer == NULL) {
        OSReport("Can't allocate the memory");
        #if DEBUG
        JUT_ASSERT(9162, FALSE);
        #else
        return 0;
        #endif
    }

    daMP_THPPlayerSetBuffer((u8*)daMP_buffer);

    if (!daMP_THPPlayerPrepare(0, 0, daMP_audioInfo.sndNumTracks != 1 ? OSGetTick() % daMP_audioInfo.sndNumTracks : 0)) {
        OSReport("Fail to prepare\n");
        #if DEBUG
        JUT_ASSERT(9190, FALSE);
        #else
        return 0;
        #endif
    }

    return 1;
}

static void daMP_ActivePlayer_Finish() {
    daMP_THPPlayerStop();
    daMP_THPPlayerClose();
    daMP_THPPlayerQuit();

    if (daMP_buffer != NULL) {
        JKRFree(daMP_buffer);
    }
}

static void daMP_ActivePlayer_Main() {
    if (daMP_THPPlayerGetState() == 5) {
        daMP_THPPlayerStop();
        daMP_THPPlayerClose();

        if (daMP_buffer != NULL) {
            JKRFree(daMP_buffer);
        }

        OSReport("Error happen");
    }
}

static void daMP_ActivePlayer_Draw() {
    int frame = daMP_THPPlayerDrawCurrentFrame(JUTVideo::getManager()->getRenderMode(), daMP_DrawPosX, daMP_DrawPosY, daMP_videoInfo.xSize, daMP_videoInfo.ySize);
    daMP_THPPlayerDrawDone();

    if (!fopOvlpM_IsPeek() && frame > 0 && (cAPICPad_ANY_BUTTON(0) || !daMP_c::daMP_c_Get_MovieRestFrame())) {
        dComIfGp_event_reset();
        daMP_c::daMP_c_Set_PercentMovieVolume(0.0f);
    }
}

static BOOL daMP_Fail_alloc;

static u32 daMP_Get_MovieRestFrame() {
    int temp_r31;
    if (daMP_Fail_alloc != 0 || daMP_THPPlayerGetState() == 5) {
        return 0;
    }

    if (daMP_ActivePlayer.open && daMP_ActivePlayer.dispTextureSet != NULL) {
        temp_r31 = (daMP_ActivePlayer.dispTextureSet->frameNumber + daMP_ActivePlayer.initReadFrame) % daMP_ActivePlayer.header.numFrames;
    } else {
        return -1;
    }

    int temp_r3 = daMP_THPPlayerGetTotalFrame();
    if ((u32)temp_r3 == 0) {
        return 0;
    }

    if ((u32)temp_r3 <= 1) {
        return 0;
    }

    if ((u32)temp_r3 - 1 <= temp_r31) {
        return 0;
    }

    return (temp_r3 - 1) - temp_r31;
}

static void daMP_Set_PercentMovieVolume(f32 volume) {
    if (!daMP_Fail_alloc) {
        s32 player_vol;
        if (volume >= 1.0f) {
            player_vol = 127;
        } else if (volume <= 0.0f) {
            player_vol = 0;
        } else {
            player_vol = volume / 127.0f;
        }

        daMP_THPPlayerSetVolume(player_vol, 1000);
    }
}

int daMP_c::daMP_c_Get_arg_demoNo() {
    return ((u32)(fopAcM_GetParam(this) >> 7)) & 0x7F;
}

int daMP_c::daMP_c_Get_arg_movieNo() {
    return fopAcM_GetParam(this) & 0x7F;
}

int daMP_c::daMP_c_Init() {
    JUT_ASSERT(9469, m_myObj == NULL);

    mDoGph_gInf_c::setFrameRate(1);
    daMP_Fail_alloc = FALSE;

    int demoNo = daMP_c_Get_arg_demoNo();
    JUT_ASSERT(9505, 0 <= demoNo && demoNo <= 99);

    int movieNo = daMP_c_Get_arg_movieNo();
    JUT_ASSERT(9507, 0 <= movieNo && movieNo <= 99);

    char path[32];
    sprintf(path, "/Movie/demo_movie%02d_%02d.thp", demoNo, movieNo);

    if (!daMP_ActivePlayer_Init(path)) {
        daMP_Fail_alloc = TRUE;
    }

    mpGetMovieRestFrame = daMP_Get_MovieRestFrame;
    mpSetPercentMovieVol = daMP_Set_PercentMovieVolume;
    mpTHPGetTotalFrame = daMP_THPPlayerGetTotalFrame;
    mpTHPPlay = daMP_THPPlayerPlay;
    mpTHPStop = daMP_THPPlayerStop;
    mpTHPPause = daMP_THPPlayerPause;

    m_myObj = this;
    return cPhs_COMPLEATE_e;
}

int daMP_c::daMP_c_Finish() {
    daMP_ActivePlayer_Finish();
    m_myObj = NULL;
    return 1;
}

int daMP_c::daMP_c_Main() {
    daMP_ActivePlayer_Main();
    return 1;
}

void daMP_Dlst_base_c::draw() {
    daMP_ActivePlayer_Draw();
}

static daMP_Dlst_base_c daMP_c_Dlst_base;

int daMP_c::daMP_c_Draw() {
    dComIfGd_set2DOpa(&daMP_c_Dlst_base);
    return 1;
}

int daMP_c::daMP_c_Callback_Init(fopAc_ac_c* i_this) {
    fopAcM_ct(i_this, daMP_c);
    return ((daMP_c*)i_this)->daMP_c_Init();
}

int daMP_c::daMP_c_Callback_Finish(daMP_c* i_this) {
    return i_this->daMP_c_Finish();
}

int daMP_c::daMP_c_Callback_Main(daMP_c* i_this) {
    #if PLATFORM_WII || PLATFORM_SHIELD
    mDoGph_gInf_c::resetDimming();
    #endif

    if (daMP_Fail_alloc) {
        return 1;
    }

    return i_this->daMP_c_Main();
}

int daMP_c::daMP_c_Callback_Draw(daMP_c* i_this) {
    if (daMP_Fail_alloc) {
        return 1;
    }

    return i_this->daMP_c_Draw();
}

static int daMP_Callback_Dummy(daMP_c* i_this) {
    return 1;
}

static actor_method_class daMP_METHODS = {
    (process_method_func)daMP_c::daMP_c_Callback_Init,
    (process_method_func)daMP_c::daMP_c_Callback_Finish,
    (process_method_func)daMP_c::daMP_c_Callback_Main,
    (process_method_func)daMP_Callback_Dummy,
    (process_method_func)daMP_c::daMP_c_Callback_Draw,
};

actor_process_profile_definition g_profile_MOVIE_PLAYER = {
  fpcLy_CURRENT_e,        // mLayerID
  7,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_MOVIE_PLAYER,      // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(daMP_c),         // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  9,                      // mPriority
  &daMP_METHODS,          // sub_method
  0x0006C000,             // mStatus
  fopAc_ACTOR_e,          // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};

AUDIO_INSTANCES;
