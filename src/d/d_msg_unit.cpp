#include "d/dolzel.h" // IWYU pragma: keep

#include "d/d_msg_unit.h"
#include "d/d_com_inf_game.h"
#include "stdio.h"
#include "d/d_kankyo.h"
#include "d/d_meter2_info.h"

// temporary until a better solution is found
typedef struct dMsgUnit_inf1_entry {
    u32 dat1EntryOffset;
#if REGION_JPN
    u16 field_0x04;
    u16 field_0x06;
    u16 field_0x08;
    u16 field_0x0a;
    u16 field_0x0c;
    u16 field_0x0e;
    u16 field_0x10;
    u16 field_0x12;
    u16 field_0x14;
    u16 field_0x16;
    u16 field_0x18;
#else
    u16 startFrame;
    u16 endFrame;
#endif
} dMsgUnit_inf1_entry;

typedef struct dMsgUnit_inf1_section_t {
    /* 0x00 */ u32 msgType;   // sectionType
    /* 0x04 */ u32 size;    // total size of the section
    /* 0x08 */ u16 entryCount;
    /* 0x0A */ u16 entryLength;
    /* 0x0C */ u16 msgArchiveId;
    /* 0x0E */ dMsgUnit_inf1_entry entries[0];
} dMsgUnit_inf1_section_t;

dMsgUnit_c::dMsgUnit_c() {}

dMsgUnit_c::~dMsgUnit_c() {}

// NONMATCHING - regalloc
#if REGION_JPN
void dMsgUnit_c::setTag(int i_type, int i_value, char* o_buffer, bool param_4) {
    *o_buffer = 0;
    bool stack9 = false;
    bool stack8 = false;
    int value = i_value;

    if (i_type == 0x10000) {
        sprintf(o_buffer, "%d", i_value);
        return;
    }

    if (i_type == 0x10001) {
        int tens_digit = i_value / 10;
        int ones_digit = i_value % 10;
        sprintf(o_buffer, "%d-%d", tens_digit, ones_digit);
        return;
    }

    if ((i_type == 4 && param_4 == true) || (i_type == 5 && param_4 == false)) {
        int seconds = i_value / 1000;
        int minutes = seconds / 60;
        seconds -= minutes * 60;
        if (minutes > 99) {
            minutes = 99;
            seconds = 59;
        }

        if (minutes == 0 && seconds == 0) {
            if (strcmp(dComIfGp_getStartStageName(), "F_SP00") == 0) {
                return;
            }
        }

        if (i_type == 4) {
            i_value = minutes;
            if (minutes == 0) {
                stack9 = true;
            }
        }

        if (i_type == 5) {
            if (seconds == 0 && minutes != 0) {
                return;
            }
            i_value = seconds;
        }
    }

    if ((i_type == 3 && param_4 == true) || i_type == 4 && param_4 == false) {
        f32 dayTime = g_env_light.getDaytime();
        f32 hour = dayTime / 15.0f;

        f32 iVar8b = ((s32)(1000000.0f * dayTime) % 15000000) / 1000000.0f;
        f32 minute = 60.0f * (iVar8b / 15.0f);
        // not sure why this affects codegen, in theory it should be optimized out
        f32 minute2 = 60.0f * (iVar8b / 15.0f);
        if (i_type == 3) {
            i_value = (s32)hour;
        }

        if (i_type == 4) {
            i_value = (s32)minute;
        }
    }

    if (i_type == 9 && param_4 == true) {
        sprintf(o_buffer, "%d", i_value);
        stack8 = true;
    }

    if (!stack9) {
        bmg_header_t* pHeader = (bmg_header_t*)dMeter2Info_getMsgUnitResource();
        dMsgUnit_inf1_section_t* pInfoBlock = NULL;
        const void* pMsgDataBlock = NULL;
        str1_section_t* pStrAttributeBlock = NULL;
        int filepos = sizeof(bmg_header_t);
        u32 filesize = pHeader->size;
        bmg_section_t* pSection = (bmg_section_t*)(((u8*)pHeader) + filepos);

        for (; filepos < filesize; filepos += pSection->size) {
            switch(pSection->magic) {
                case 'FLW1':
                    break;
                case 'FLI1':
                    break;
                case 'INF1':
                    pInfoBlock = (dMsgUnit_inf1_section_t*)pSection;
                    break;
                case 'DAT1':
                    pMsgDataBlock = pSection;
                    break;
                case 'STR1':
                    pStrAttributeBlock = (str1_section_t*)pSection;
                    break;
            }
            pSection = (bmg_section_t*)((u8*)pSection + pSection->size);
        }

        u16 vals[12];
        vals[0] = pInfoBlock->entries[i_type].field_0x04;
        vals[1] = pInfoBlock->entries[i_type].field_0x06;
        vals[2] = pInfoBlock->entries[i_type].field_0x08;
        vals[3] = pInfoBlock->entries[i_type].field_0x0a;
        vals[4] = pInfoBlock->entries[i_type].field_0x0c;
        vals[5] = pInfoBlock->entries[i_type].field_0x0e;
        vals[6] = pInfoBlock->entries[i_type].field_0x10;
        vals[7] = pInfoBlock->entries[i_type].field_0x12;
        vals[8] = pInfoBlock->entries[i_type].field_0x14;
        vals[9] = pInfoBlock->entries[i_type].field_0x16;
        vals[10] = pInfoBlock->entries[i_type].field_0x18;
        int entryOff = pInfoBlock->entries[i_type].dat1EntryOffset;

        char* value2 = (char*)((uintptr_t)pMsgDataBlock + entryOff + 8);

        const char* uVar5;
        if (i_value == 0) {
            uVar5 = pStrAttributeBlock->entries->str + vals[0];
        } else {
            if ((i_value % 10) == 0) {
                uVar5 = pStrAttributeBlock->entries->str + (vals[10]);
            } else {
                uVar5 = pStrAttributeBlock->entries->str + vals[i_value % 10];
            }
        }

        int uVar5Len = strlen(uVar5);
        if (uVar5Len == 0) {
            if (stack8) {
                strcat(o_buffer, value2);
            } else {
                sprintf(o_buffer, "%d%s", i_value, value2);
            }
        } else {
            char unkCharArr[7];
            unkCharArr[0] = 26;
            unkCharArr[1] = uVar5Len + 6;
            unkCharArr[2] = -1;
            unkCharArr[3] = -1;
            unkCharArr[4] = 2;
            unkCharArr[5] = strlen(value2) / 2;
            unkCharArr[6] = 0;

            if (stack8) {
                strcat(o_buffer, unkCharArr);
                strcat(o_buffer, uVar5);
                strcat(o_buffer, value2);
            } else {
                sprintf(o_buffer, "%d%s%s%s", i_value, unkCharArr, uVar5, value2);
            }
        }
    }

    if (i_type == 3 && param_4 == true) {
        char buffer[20];
        setTag(4, 0, buffer, false);
        strcat(o_buffer, buffer);
    }

    if (i_type == 4 && param_4 == true) {
        char buffer[20];
        setTag(5, value, buffer, false);
        strcat(o_buffer, buffer);
    }
}
#else
void dMsgUnit_c::setTag(int i_type, int i_value, char* o_buffer, bool param_4) {
    *o_buffer = 0;
    bool stack9 = false;
    bool stack8 = false;
    int param_2b = i_value; // sp40
    int tens_digit; // sp3C
    int ones_digit; // sp38
    int seconds; // r27
    int minutes; // sp34

    if (i_type == 0x10000) {
        sprintf(o_buffer, "%d", i_value);
        return;
    }

    if (i_type == 0x10001) {
        tens_digit = i_value / 10;
        ones_digit = i_value % 10;
        sprintf(o_buffer, "%d-%d", tens_digit, ones_digit);
        return;
    }

    if (i_type == 4 && param_4 == true) {
        seconds = i_value / 1000;
        minutes = seconds / 60;
        seconds -= minutes * 60;
        if (minutes > 99) {
            minutes = 99;
            seconds = 59;
        }
        if (minutes != 0 || seconds != 0) {
            sprintf(o_buffer, "%d:%02d", minutes, seconds);
        }

        return;
    }

    if (i_type == 3 && param_4 == true) {
        (void)seconds; // dummy use to force into register instead of stack
        f32 iVar8b;
        f32 dayTime = g_env_light.getDaytime();
        f32 hour = dayTime / 15.0f;

        iVar8b = ((s32)(1000000.0f * dayTime) % 15000000) / 1000000.0f;
        f32 minute = 60.0f * (iVar8b / 15.0f);

        iVar8b = ((s32)(1000000.0f * dayTime) % 250000) / 1000000.0f;
        f32 iVar9 = 60.0f * (iVar8b / 0.25f);
        sprintf(o_buffer, "%d:%02d", (s32)hour, (s32)minute);
    } else {
        if (i_type == 9 && param_4 == true) {
            int value = i_value;
            sprintf(o_buffer, "%d", value);
            stack8 = true;
        }

        if (!stack9) {
            bmg_header_t* pHeader = (bmg_header_t*)dMeter2Info_getMsgUnitResource();
            dMsgUnit_inf1_section_t* pInfoBlock = NULL;
            const void* pMsgDataBlock = NULL;
            str1_section_t* pStrAttributeBlock = NULL;
            int filepos = sizeof(bmg_header_t);
            u32 filesize = pHeader->size;
            bmg_section_t* pSection = (bmg_section_t*)(((u8*)pHeader) + filepos);

            for (; filepos < filesize; filepos += pSection->size) {
                switch (pSection->magic) {
                case 'FLW1':
                    break;
                case 'FLI1':
                    break;
                case 'INF1':
                    pInfoBlock = (dMsgUnit_inf1_section_t*)pSection;
                    break;
                case 'DAT1':
                    pMsgDataBlock = pSection;
                    break;
                case 'STR1':
                    pStrAttributeBlock = (str1_section_t*)pSection;
                    break;
                }
                pSection = (bmg_section_t*)((u8*)pSection + pSection->size);
            }

            // This section is weird. The debug seems like entriesStr is outside the condition
            // but the normal build doesn't really work with that. Same for pInfoBlock->entries.

#if DEBUG
            dMsgUnit_inf1_entry* entriesInf = &pInfoBlock->entries[i_type];
            u32 dat1EntryOffset = entriesInf->dat1EntryOffset;
            const char* uVar5;
            u16 vals[14];
            vals[0] = entriesInf->startFrame;
            vals[1] = entriesInf->endFrame;
            (void)entriesInf;  // dummy use to force into register instead of stack
            str1_entry_t* entriesStr = pStrAttributeBlock->entries;
#else
            u32 dat1EntryOffset = pInfoBlock->entries[i_type].dat1EntryOffset;
            const char* uVar5;
            u16 vals[14];
            vals[0] = pInfoBlock->entries[i_type].startFrame;
            vals[1] = pInfoBlock->entries[i_type].endFrame;
#endif

#if REGION_PAL
            if (i_value == 1 ||
                (dComIfGs_getPalLanguage() == dSv_player_config_c::LANGAUGE_FRENCH &&
                    i_value == 0)) {
#elif !REGION_USA
            if (i_value == 1 ||
                (dComIfGs_getPalLanguage() == dSv_player_config_c::LANGAUGE_SPANISH &&
                    i_value == 0)) {
#else
            if (i_value == 1) {
#endif
#if DEBUG
                uVar5 = entriesStr->str + vals[0];
#else
                uVar5 = pStrAttributeBlock->entries->str + vals[0];
#endif
            } else {
#if DEBUG
                uVar5 = entriesStr->str + vals[1];
#else
                uVar5 = pStrAttributeBlock->entries->str + vals[1];
#endif
            }

            if (strcmp(uVar5, "") == 0) {
                sprintf(o_buffer, "%d%s", i_value, uVar5);
            } else {
                sprintf(o_buffer, "%d %s", i_value, uVar5);
            }
        }
        if (i_type == 3 && param_4 == true) {
            char buffer[20];
            setTag(4, 0, buffer, false);
            strcat(o_buffer, buffer);
        }

        if (i_type == 4 && param_4 == true) {
            char buffer[20];
            setTag(5, param_2b, buffer, false);
            strcat(o_buffer, buffer);
        }
    }
}
#endif

dMsgUnit_c g_msg_unit;
