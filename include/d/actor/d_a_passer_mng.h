#ifndef D_A_PASSER_MNG_H
#define D_A_PASSER_MNG_H

#include "SSystem/SComponent/c_lib.h"
#include "d/d_kankyo.h"
#include "f_op/f_op_actor_mng.h"

class dPath;

/**
 * @ingroup actors-unsorted
 * @class daPasserMng_c
 * @brief Hylian Passerby Manager?
 *
 * @details
 *
 */
class daPasserMng_c : public fopAc_ac_c {
public:
    int execute();
    int getPasserParam();
    int getLuggageParamHigh(u32);
    int create();
    void create_init();
    ~daPasserMng_c() { delete [] childProcIds; }

    u8 getDetailLevel() { return argument; }
    u8 getPathID() { return fopAcM_GetParam(this); }
    u8 getIntervalTime() { return fopAcM_GetParam(this) >> 24; }
    int getStartTime() { return (fopAcM_GetParam(this) >> 8) & 0xff; }
    u8 getEndTime() { return (fopAcM_GetParam(this) >> 16) & 0xff; }
    u8 getMaxNum() { return shape_angle.x & 0xFF; }
    u8 getGroupNo() { return (shape_angle.x >> 8) & 0xff; }

    int getTimeHour() {
        return (dKy_darkworld_check()) ? dKy_getDarktime_hour() : dKy_getdaytime_hour();
    }

    int getTimeMinute() {
        return (dKy_darkworld_check()) ? dKy_getDarktime_minute() : dKy_getdaytime_minute();
    }

    int getTime() {
        return getTimeHour() * 60 + getTimeMinute();
    }

    int getDayOfWeek() {
        return (dKy_darkworld_check()) ? dKy_getDarktime_week() : dKy_get_dayofweek();
    }

    int getChildNum() {
        int childNum = 0;
        fopAc_ac_c* pActor = NULL;
        for (int i = 0; i < getMaxNum(); i++) {
            fopAcM_SearchByID(childProcIds[i], &pActor);
            if (pActor != NULL) {
                childNum++;
            }
        }
        return childNum;
    }

    bool checkOverlapping(int param_1, int param_2) {
        int iVar1;
        if ((u32)param_2 <= 6) {
            iVar1 = (u8)param_2 - 1;
        } else {
            iVar1 = 6;
        }
        for (int i = 0; i < iVar1; i++) {
            if (field_0x58a[i] == param_1) {
                return true;
            }
        }
        return false;
    }

    int getLuggageParamLow(u32 param_1) {
        int paramLow;
  
        paramLow = 0;
        switch (param_1) {
        case 0:
        case 1:
            if (cLib_getRndValue(0, 2) != 0) {
                paramLow = 2;
            }
            break;
        case 2:
        case 8:
        case 9:
        case 10:
        case 15:
            switch(cLib_getRndValue(0, 3)) {
            case 0:
                paramLow = 0;
                break;
            case 1:
                paramLow = 1;
                break;
            case 2:
                paramLow = 4;
                break;
            }
            break;
        case 3:
        case 11:
            switch(cLib_getRndValue(0, 2)) {
            case 0:
                paramLow = 0;
                break;
            case 1:
                paramLow = 4;
                break;
            }
            break;
        case 4:
        case 5:
        case 12:
        case 13:
            switch(cLib_getRndValue(0, 3)) {
            case 0:
                paramLow = 0;
                break;
            case 1:
                paramLow = 1;
                break;
            case 2:
                paramLow = 4;
                break;
            }
            break;
        case 6:
        case 14:
            switch (cLib_getRndValue(0, 2)) {
            case 0:
                paramLow = 0;
                break;
            case 1:
                paramLow = 1;
                break;
            }
            break;
        case 7:
            switch (cLib_getRndValue(0, 2)) {
            case 0:
                paramLow = 0;
                break;
            case 1:
                paramLow = 4;
                break;
            }
            break;
        case 0x10:
        case 0x17:
            switch(cLib_getRndValue(0, 3)) {
            case 0:
                paramLow = 0;
                break;
            case 1:
                paramLow = 8;
                break;
            case 2:
                paramLow = 5;
                break;
            }
            break;
        case 0x11:
        case 0x18:
            switch(cLib_getRndValue(0, 3)) {
            case 0:
                paramLow = 0;
                break;
            case 1:
                paramLow = 8;
                break;
            case 2:
                paramLow = 5;
                break;
            }
            break;
        case 0x12:
        case 0x19:
            switch(cLib_getRndValue(0, 4)) {
            case 0:
                paramLow = 0;
                break;
            case 1:
                paramLow = 8;
                break;
            case 2:
                paramLow = 5;
                break;
            case 3:
                paramLow = 3;
                break;
            }
            break;
        case 0x13:
        case 0x1a:
            switch (cLib_getRndValue(0, 2)) {
            case 0:
                paramLow = 0;
                break;
            case 1:
                paramLow = 5;
                break;
            }
            break;
        case 0x14:
        case 0x1b:
            switch (cLib_getRndValue(0, 2)) {
            case 0:
                paramLow = 0;
                break;
            case 1:
                paramLow = 5;
                break;
            }
            break;
        case 0x15:
        case 0x1c:
            switch (cLib_getRndValue(0, 2)) {
            case 0:
                paramLow = 0;
                break;
            case 1:
                paramLow = 5;
                break;
            }
            break;
        case 0x16:
        case 0x1d:
            switch(cLib_getRndValue(0, 3)) {
            case 0:
                paramLow = 0;
                break;
            case 1:
                paramLow = 5;
                break;
            case 2:
                paramLow = 3;
                break;
            }
            break;
        default:
            OS_REPORT("%s: Line.%d arg=%d\n", "d_a_passer_mng.cpp", 1049, param_1);
            JUT_ASSERT(1050, FALSE);
            break;
        }
        return paramLow << 8;
    }

    struct Group {
        u8 field_0x00;
        int field_0x04[0];
    };

    static Group* mGroupTbl[4];

private:
    /* 0x568 */ fpc_ProcID* childProcIds;
    /* 0x56C */ dPath* mPath;
    /* 0x570 */ int startTime;
    /* 0x574 */ int endTime;
    /* 0x578 */ int intervalTime;
    /* 0x57C */ int mTime;
    /* 0x580 */ int dayOfWeek;
    /* 0x584 */ int currentChildIndex;
    /* 0x588 */ s16 npcId;
    /* 0x58A */ s16 field_0x58a[5];
    /* 0x594 */ s16 field_0x594;
    /* 0x596 */ u8 field_0x596;
    /* 0x597 */ u8 field_0x597;
    };

STATIC_ASSERT(sizeof(daPasserMng_c) == 0x598);


#endif /* D_A_PASSER_MNG_H */
