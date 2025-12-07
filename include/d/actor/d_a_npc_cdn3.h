#ifndef D_A_NPC_CDN3_H
#define D_A_NPC_CDN3_H

#include "d/actor/d_a_npc.h"
#include "d/actor/d_a_npc_cd2.h"
#include "d/actor/d_a_tag_schedule.h"

/**
 * @ingroup actors-npcs
 * @class daNpcCdn3_c
 * @brief Hylian Adult
 *
 * @details
 *
 */
class daNpcCdn3_c : public daNpcCd2_c {
public:
    typedef void (daNpcCdn3_c::*actionFunc)();
    typedef int (daNpcCdn3_c::*seqFunc)(void*);

    enum Mode_e {
        MODE_WAIT,
        MODE_PATH,
        MODE_TALK,
        MODE_ESCAPSE,
        MODE_FEAR,
        MODE_FEAR2,
        MODE_FIGHT,
        MODE_RELIEF,
        MODE_MAX_e,
    };

    void setAction(daNpcCdn3_c::Mode_e);
    void callInit();
    void callExecute();
    void initWait();
    void executeWait();
    void initPath();
    void executePath();
    void initTalk();
    void executeTalk();
    void initEscape();
    void executeEscape();
    void initFear();
    void executeFear();
    void initFear2();
    void executeFear2();
    void initFight();
    void executeFight();
    void initRelief();
    void executeRelief();
    void setAngle();
    void pathMoveF();
    int ctrlMsgAnm();
    bool searchNextScheduleTagSub(fopAc_ac_c*);
    bool searchFirstScheduleTagSub(fopAc_ac_c*);
    void checkSchedule();
    void setSchedule(daTagSchedule_c*);
    inline bool isInShop();
    bool isChairStyle();
    bool isNoTurnTalk();
    int waita(void*);
    int talka(void*);
    int waitwall(void*);
    int talkwall(void*);
    int talkbwall(void*);
    int talkb(void*);
    int talkc(void*);
    int waitb(void*);
    int sitwaita(void*);
    int sittalka(void*);
    int sittalkab(void*);
    int sitwaitb(void*);
    int sittalkb(void*);
    int sittalkbb(void*);
    int looka(void*);
    int lookb(void*);
    int playm(void*);
    int turnr(void*);
    int turnl(void*);
    int walka(void*);
    int walkb(void*);
    int runa(void*);
    int runb(void*);
    int browsea(void*);
    int browseb(void*);
    int dance(void*);
    int ladytalka(void*);
    int ladytalkb(void*);
    int normtalka(void*);
    int normtalkb(void*);
    int sellme(void*);
    int sing(void*);
    int create();
    void create_init();
    inline int createHeap();
    void setMtx();
    void lookat();
    virtual ~daNpcCdn3_c();
    int execute();
    void checkTimeSchedule();

    inline int draw();

    int getTimeHour() {
        int hour;
        if (mIsDarkWorld) {
            hour = dKy_getDarktime_hour();
        } else {
            hour = dKy_getdaytime_hour();
        }
        return hour;
    }

    int getTimeMinute() {
        int minute;
        if (mIsDarkWorld) {
            minute = dKy_getDarktime_minute();
        } else {
            minute = dKy_getdaytime_minute();
        }
        return minute;
    }

    int getTime() {
        return getTimeHour() * 60 + getTimeMinute();
    }

    int getDayOfWeek() {
        int day;
        if (mIsDarkWorld) {
            day = dKy_getDarktime_week();
        } else {
            day = dKy_get_dayofweek();
        }
        return day;
    }

    void setSpeed(f32 param_0, f32 param_1, f32* param_2, int param_3) {
        (void)param_3;
        f32 target = field_0xb5c * (param_1 * field_0xb5c);
        f32 step = field_0xb5c * (param_0 * field_0xb5c);
        if (param_1 < target) {
            target = param_1;
        }
        cLib_chaseF(param_2, target, step);
    }

    int getGroupID() { return fopAcM_GetParam(this) & 0xff; }
    u8 getPathID() { return fopAcM_GetParam(this) >> 24; }
    int isStop() { return TRUE; }
    int getType() { return argument & 0x7f; }
    int getSeqNum() { return shape_angle.x & 0x3f; }
    int getFlowNodeNum() { return shape_angle.z; }
    u8 getStartTime() { return (fopAcM_GetParam(this) >> 8) & 0xff; }
    u8 getEndTime() { return (fopAcM_GetParam(this) >> 16) & 0xff; }

    void initParamTime() {
        u16 x;
        u16 y;
        u8 startTime = getStartTime();
        u8 endTime = getEndTime();
        if (startTime != 0xff && endTime != 0xff) {
            x = startTime / 10;
            y = (startTime % 10) * 10;
            field_0xb8c = y + x * 60;
            x = endTime / 10;
            y = (endTime % 10) * 10;
            field_0xb8e = y + x * 60;
            field_0xb96 = 0;
        } else {
            field_0xb96 = 1;
        }
        field_0xb90 = 30;
    }

    void initTimeSchedule() {
        if (getGroupID() == 0xff) {
            initParamTime();
            if (field_0xb96 == 0) {
                int iVar1 = getTime();
                if (field_0xb8c > iVar1 || field_0xb8e <= iVar1)
                {
                    field_0xb95 = 1;
                }
            }
        } else {
            mTagSched = NULL;
            field_0xb88 = NULL;
        }
    }

    int getObjNum() {
        int uVar3 = (shape_angle.x >> 6) & 0xf;
        if (isM_()) {
            switch (uVar3) {
            case 0:
                uVar3 = 0;
                break;
            case 1:
                uVar3 = 1;
                break;
            case 2:
                uVar3 = 2;
                break;
            case 3:
                uVar3 = 4;
                break;
            case 4:
                uVar3 = 6;
                break;
            case 5:
                uVar3 = 0;
                break;
            case 6:
                uVar3 = 0;
                break;
            case 7:
                uVar3 = 9;
                break;
            default:
                uVar3 = 0;
                break;
            }
                /* dSv_event_flag_c::F_0281 - Shop - Malo Mart opens in Castle Town */
            if (dComIfGs_isEventBit((u16)dSv_event_flag_c::saveBitLabels[281]) && uVar3 == 9) {
                uVar3 = 0xb;
            }
        } else {
            switch (uVar3) {
            case 0:
                uVar3 = 0;
                break;
            case 1:
                uVar3 = 8;
                break;
            case 2:
                uVar3 = 3;
                break;
            case 3:
                uVar3 = 5;
                break;
            case 4:
                uVar3 = 7;
                break;
            case 5:
                uVar3 = 0;
                break;
            case 6:
                uVar3 = 0;
                break;
            case 7:
                uVar3 = 10;
                break;
            default:
                uVar3 = 0;
                break;
            }
                /* dSv_event_flag_c::F_0281 - Shop - Malo Mart opens in Castle Town */
            if (dComIfGs_isEventBit((u16)dSv_event_flag_c::saveBitLabels[281]) && uVar3 == 10) {
                uVar3 = 12;
            }
        }
        return uVar3;
    }

    void initCollision() {
        mStts.Init(0xd9, 0xff, this);
        mStts.SetRoomId(fopAcM_GetRoomNo(this));
        mCyl.SetStts(&mStts);
        mCyl.Set(m_cylDat);
        mCyl.SetR(Cd2_HIO_cylR(m_type));
        mCyl.SetH(Cd2_HIO_cylH(m_type));
    }

    void setCollision() {
        #if DEBUG
        mCyl.SetR(Cd2_HIO_cylR(m_type));
        mCyl.SetH(Cd2_HIO_cylH(m_type));
        #endif
        mCyl.SetC(current.pos);
        if (!fopAcM_CheckCondition(this, 4)) {
            dComIfG_Ccsp()->Set(&mCyl);
        }
    }

    bool isOrchestra() {
        if (strcmp(dComIfGp_getStartStageName(), "F_SP116") != 0) {
            return 0;
        } else {
            if (m_seqNum == 17 &&
                (getType() == 4 || getType() == 5 || getType() == 6 || getType() == 16))
            {
                return 1;
            } else if (m_seqNum == 37 && getType() == 7) {
                return 1;
            } else {
                return 0;
            }
        }
    }

    int chkEvent() {
        attention_info.flags = 0;
        if (mFlowNodeNum > 0) {
            attention_info.flags = fopAc_AttnFlag_SPEAK_e | fopAc_AttnFlag_TALK_e;
        }
        if (mIsDarkWorld) {
            if (daPy_py_c::checkNowWolfEyeUp()) {
                attention_info.flags |= (fopAc_AttnFlag_UNK_0x800000 | fopAc_AttnFlag_UNK_0x400000);
            } else {
                attention_info.flags = 0;
            }
        }
        if (dComIfGp_event_runCheck()) {
            if (eventInfo.checkCommandDemoAccrpt()) {
                return 1;
            }
            if (eventInfo.checkCommandTalk() && mMode != MODE_TALK) {
                setAction(MODE_TALK);
                return 1;
            }
        }
        return 0;
    }

    int orderEvent() {
#if PLATFORM_SHIELD
        if (mFlowNodeNum > 0) {
#else
        if ((!mIsDarkWorld || daPy_py_c::checkNowWolfEyeUp()) &&
            mFlowNodeNum > 0 &&
            ((attention_info.flags & fopAc_AttnFlag_SPEAK_e) ||
                (attention_info.flags & fopAc_AttnFlag_TALK_e))) {
#endif
            eventInfo.onCondition(dEvtCnd_CANTALK_e);
        }
        return 1;
    }

    inline bool searchFirstScheduleTag();
    inline bool searchNextScheduleTag();

    struct ActionPair {
        actionFunc mInitFn;
        actionFunc mExecFn;
    };

    static const ActionPair ActionTable[8];
    static seqFunc* m_funcTbl[44];
    static seqFunc m_seq00_funcTbl[2];
    static seqFunc m_seq01_funcTbl[2];
    static seqFunc m_seq02_funcTbl[2];
    static seqFunc m_seq03_funcTbl[2];
    static seqFunc m_seq04_funcTbl[2];
    static seqFunc m_seq05_funcTbl[4];
    static seqFunc m_seq06_funcTbl[4];
    static seqFunc m_seq07_funcTbl[2];
    static seqFunc m_seq08_funcTbl[7];
    static seqFunc m_seq09_funcTbl[2];
    static seqFunc m_seq10_funcTbl[2];
    static seqFunc m_seq11_funcTbl[6];
    static seqFunc m_seq12_funcTbl[2];
    static seqFunc m_seq13_funcTbl[6];
    static seqFunc m_seq14_funcTbl[2];
    static seqFunc m_seq15_funcTbl[2];
    static seqFunc m_seq16_funcTbl[7];
    static seqFunc m_seq17_funcTbl[2];
    static seqFunc m_seq18_funcTbl[2];
    static seqFunc m_seq19_funcTbl[7];
    static seqFunc m_seq20_funcTbl[2];
    static seqFunc m_seq21_funcTbl[2];
    static seqFunc m_seq22_funcTbl[4];
    static seqFunc m_seq23_funcTbl[7];
    static seqFunc m_seq24_funcTbl[5];
    static seqFunc m_seq25_funcTbl[7];
    static seqFunc m_seq26_funcTbl[3];
    static seqFunc m_seq27_funcTbl[2];
    static seqFunc m_seq28_funcTbl[3];
    static seqFunc m_seq29_funcTbl[3];
    static seqFunc m_seq30_funcTbl[6];
    static seqFunc m_seq31_funcTbl[6];
    static seqFunc m_seq32_funcTbl[7];
    static seqFunc m_seq33_funcTbl[7];
    static seqFunc m_seq34_funcTbl[9];
    static seqFunc m_seq35_funcTbl[2];
    static seqFunc m_seq36_funcTbl[4];
    static seqFunc m_seq37_funcTbl[2];
    static seqFunc m_seq38_funcTbl[2];
    static seqFunc m_seq39_funcTbl[2];
    static seqFunc m_seq40_funcTbl[3];
    static seqFunc m_seq41_funcTbl[2];
    static seqFunc m_seq42_funcTbl[2];
    static seqFunc m_seq43_funcTbl[3];

    /* 0xAC8 */ J3DModel* field_0xac8;
    /* 0xACC */ J3DModel* field_0xacc;
    /* 0xAD0 */ PathTrace_c m_path;
    /* 0xAE8 */ daNpcT_ActorMngr_c m_targetAct;
    /* 0xB00 */ dMsgFlow_c mMsgFlow;
    /* 0xB4C */ const actionFunc* mAction;
    /* 0xB50 */ Mode_e mMode;
    /* 0xB54 */ Mode_e mPrevMode;
    /* 0xB58 */ int mObjNum;
    /* 0xB5C */ f32 field_0xb5c;
    /* 0xB60 */ f32 field_0xb60;
    /* 0xB64 */ f32 field_0xb64;
    /* 0xB68 */ u32 field_0xb68;
    /* 0xB6C */ int mMsgIndex;
    /* 0xB70 */ int mFlowNodeNum;
    /* 0xB74 */ int field_0xb74;
    /* 0xB78 */ int m_seqNum;
    /* 0xB7C */ int field_0xb7c;
    /* 0xB80 */ J3DAnmTransform* field_0xb80;
    /* 0xB84 */ daTagSchedule_c* mTagSched;
    /* 0xB88 */ daTagSchedule_c* field_0xb88;
    /* 0xB8C */ u16 field_0xb8c;
    /* 0xB8C */ u16 field_0xb8e;
    /* 0xB90 */ u16 field_0xb90;
    /* 0xB92 */ s16 field_0xb92;
    /* 0xB94 */ u8 field_0xb94;
    /* 0xB95 */ u8 field_0xb95;
    /* 0xB96 */ u8 field_0xb96;
    /* 0xB97 */ u8 m_seqStep;
    /* 0xB98 */ u8 field_0xb98;
    /* 0xB99 */ u8 field_0xb99;
};

STATIC_ASSERT(sizeof(daNpcCdn3_c) == 0xb9c);


#endif /* D_A_NPC_CDN3_H */
