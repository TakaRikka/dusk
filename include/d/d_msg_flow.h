#ifndef D_MSG_D_MSG_FLOW_H
#define D_MSG_D_MSG_FLOW_H

#include "dolphin/types.h"

enum {
    NODETYPE_MESSAGE_e = 1,
    NODETYPE_BRANCH_e = 2,
    NODETYPE_EVENT_e = 3,
};

enum {
    SELTYPE_NONE_e = 0,
    SELTYPE_VERTICAL_e = 1,
    SELTYPE_HORIZONTAL_e = 2,
};

class fopAc_ac_c;
class msg_class;

// all mesg_flow_node structs members might be wrong
struct mesg_flow_node {
    /* 0x00 */ u8 type;
    /* 0x01 */ u8 field_0x1;
    /* 0x02 */ u16 msg_index;
    /* 0x04 */ u16 next_node_idx;
    /* 0x06 */ u16 unk_0x6;
};  // Size: 0x8

struct mesg_flow_node_branch {
    /* 0x00 */ u8 type;
    /* 0x01 */ u8 field_0x1;
    /* 0x02 */ u16 query_idx;
    /* 0x04 */ u16 param;
    /* 0x06 */ u16 next_node_idx;
};

struct mesg_flow_node_event {
    /* 0x00 */ u8 type;
    /* 0x01 */ u8 event_idx;
    /* 0x02 */ u16 next_node_idx;
    /* 0x04 */ u8 params[4];
};  // Size: 0x8

class dMsgFlow_c;
typedef u16 (dMsgFlow_c::*queryFunc)(mesg_flow_node_branch*, fopAc_ac_c*, int);
typedef int (dMsgFlow_c::*eventFunc)(mesg_flow_node_event*, fopAc_ac_c*);

class dMsgFlow_c {
public:
    dMsgFlow_c();
    virtual ~dMsgFlow_c();

    void init(fopAc_ac_c* i_partner, int i_flowID, int, fopAc_ac_c**);
    int checkOpenDoor(fopAc_ac_c*, int*);
    int doFlow(fopAc_ac_c*, fopAc_ac_c**, int i_flow);
    int checkEventRender(int*, int*, int*, int*);
    void remove();
    u16 getEventId(int*);
    u32 getMsgNo();
    u32 getNowMsgNo();
    msg_class* getMsg();
    void setInitValue(int);
    void setInitValueGroupChange(int, fopAc_ac_c**);
    u8* getMsgDataBlock(char const*);
    u16 getInitNodeIndex(u16);
    void setNodeIndex(u16, fopAc_ac_c**);
    int setSelectMsg(mesg_flow_node*, mesg_flow_node*, fopAc_ac_c*);
    int setNormalMsg(mesg_flow_node*, fopAc_ac_c*);
    int messageNodeProc(fopAc_ac_c*, fopAc_ac_c**);
    int branchNodeProc(fopAc_ac_c*, fopAc_ac_c**);
    int eventNodeProc(fopAc_ac_c*, fopAc_ac_c**);
    int nodeProc(fopAc_ac_c*, fopAc_ac_c**);
    int getParam(u16*, u16*, u8*);
    int getParam(u8*, u8*);
    int getParam(u8*);
    u16 query001(mesg_flow_node_branch*, fopAc_ac_c*, int);
    u16 query002(mesg_flow_node_branch*, fopAc_ac_c*, int);
    u16 query003(mesg_flow_node_branch*, fopAc_ac_c*, int);
    u16 query004(mesg_flow_node_branch*, fopAc_ac_c*, int);
    u16 query005(mesg_flow_node_branch*, fopAc_ac_c*, int);
    u16 query006(mesg_flow_node_branch*, fopAc_ac_c*, int);
    u16 query007(mesg_flow_node_branch*, fopAc_ac_c*, int);
    u16 query008(mesg_flow_node_branch*, fopAc_ac_c*, int);
    u16 query009(mesg_flow_node_branch*, fopAc_ac_c*, int);
    u16 query010(mesg_flow_node_branch*, fopAc_ac_c*, int);
    u16 query011(mesg_flow_node_branch*, fopAc_ac_c*, int);
    u16 query012(mesg_flow_node_branch*, fopAc_ac_c*, int);
    u16 query013(mesg_flow_node_branch*, fopAc_ac_c*, int);
    u16 query014(mesg_flow_node_branch*, fopAc_ac_c*, int);
    u16 query015(mesg_flow_node_branch*, fopAc_ac_c*, int);
    u16 query016(mesg_flow_node_branch*, fopAc_ac_c*, int);
    u16 query017(mesg_flow_node_branch*, fopAc_ac_c*, int);
    u16 query018(mesg_flow_node_branch*, fopAc_ac_c*, int);
    u16 query019(mesg_flow_node_branch*, fopAc_ac_c*, int);
    u16 query020(mesg_flow_node_branch*, fopAc_ac_c*, int);
    u16 query021(mesg_flow_node_branch*, fopAc_ac_c*, int);
    u16 query022(mesg_flow_node_branch*, fopAc_ac_c*, int);
    u16 query023(mesg_flow_node_branch*, fopAc_ac_c*, int);
    u16 query024(mesg_flow_node_branch*, fopAc_ac_c*, int);
    u16 query025(mesg_flow_node_branch*, fopAc_ac_c*, int);
    u16 query026(mesg_flow_node_branch*, fopAc_ac_c*, int);
    u16 query027(mesg_flow_node_branch*, fopAc_ac_c*, int);
    u16 query028(mesg_flow_node_branch*, fopAc_ac_c*, int);
    u16 query029(mesg_flow_node_branch*, fopAc_ac_c*, int);
    u16 query030(mesg_flow_node_branch*, fopAc_ac_c*, int);
    u16 query031(mesg_flow_node_branch*, fopAc_ac_c*, int);
    u16 query032(mesg_flow_node_branch*, fopAc_ac_c*, int);
    u16 query033(mesg_flow_node_branch*, fopAc_ac_c*, int);
    u16 query034(mesg_flow_node_branch*, fopAc_ac_c*, int);
    u16 query035(mesg_flow_node_branch*, fopAc_ac_c*, int);
    u16 query036(mesg_flow_node_branch*, fopAc_ac_c*, int);
    u16 query037(mesg_flow_node_branch*, fopAc_ac_c*, int);
    u16 query038(mesg_flow_node_branch*, fopAc_ac_c*, int);
    u16 query039(mesg_flow_node_branch*, fopAc_ac_c*, int);
    u16 query040(mesg_flow_node_branch*, fopAc_ac_c*, int);
    u16 query041(mesg_flow_node_branch*, fopAc_ac_c*, int);
    u16 query042(mesg_flow_node_branch*, fopAc_ac_c*, int);
    u16 query043(mesg_flow_node_branch*, fopAc_ac_c*, int);
    u16 query044(mesg_flow_node_branch*, fopAc_ac_c*, int);
    u16 query045(mesg_flow_node_branch*, fopAc_ac_c*, int);
    u16 query046(mesg_flow_node_branch*, fopAc_ac_c*, int);
    u16 query047(mesg_flow_node_branch*, fopAc_ac_c*, int);
    u16 query048(mesg_flow_node_branch*, fopAc_ac_c*, int);
    u16 query049(mesg_flow_node_branch*, fopAc_ac_c*, int);
    u16 query050(mesg_flow_node_branch*, fopAc_ac_c*, int);
    u16 query051(mesg_flow_node_branch*, fopAc_ac_c*, int);
    u16 query052(mesg_flow_node_branch*, fopAc_ac_c*, int);
    u16 query053(mesg_flow_node_branch*, fopAc_ac_c*, int);
    int event000(mesg_flow_node_event*, fopAc_ac_c*);
    int event001(mesg_flow_node_event*, fopAc_ac_c*);
    int event002(mesg_flow_node_event*, fopAc_ac_c*);
    int event003(mesg_flow_node_event*, fopAc_ac_c*);
    int event004(mesg_flow_node_event*, fopAc_ac_c*);
    int event005(mesg_flow_node_event*, fopAc_ac_c*);
    int event006(mesg_flow_node_event*, fopAc_ac_c*);
    int event007(mesg_flow_node_event*, fopAc_ac_c*);
    int event008(mesg_flow_node_event*, fopAc_ac_c*);
    int event009(mesg_flow_node_event*, fopAc_ac_c*);
    int event010(mesg_flow_node_event*, fopAc_ac_c*);
    int event011(mesg_flow_node_event*, fopAc_ac_c*);
    int event012(mesg_flow_node_event*, fopAc_ac_c*);
    int event013(mesg_flow_node_event*, fopAc_ac_c*);
    int event014(mesg_flow_node_event*, fopAc_ac_c*);
    int event015(mesg_flow_node_event*, fopAc_ac_c*);
    int event016(mesg_flow_node_event*, fopAc_ac_c*);
    int event017(mesg_flow_node_event*, fopAc_ac_c*);
    int event018(mesg_flow_node_event*, fopAc_ac_c*);
    int event019(mesg_flow_node_event*, fopAc_ac_c*);
    int event020(mesg_flow_node_event*, fopAc_ac_c*);
    int event021(mesg_flow_node_event*, fopAc_ac_c*);
    int event022(mesg_flow_node_event*, fopAc_ac_c*);
    int event023(mesg_flow_node_event*, fopAc_ac_c*);
    int event024(mesg_flow_node_event*, fopAc_ac_c*);
    int event025(mesg_flow_node_event*, fopAc_ac_c*);
    int event026(mesg_flow_node_event*, fopAc_ac_c*);
    int event027(mesg_flow_node_event*, fopAc_ac_c*);
    int event028(mesg_flow_node_event*, fopAc_ac_c*);
    int event029(mesg_flow_node_event*, fopAc_ac_c*);
    int event030(mesg_flow_node_event*, fopAc_ac_c*);
    int event031(mesg_flow_node_event*, fopAc_ac_c*);
    int event032(mesg_flow_node_event*, fopAc_ac_c*);
    int event033(mesg_flow_node_event*, fopAc_ac_c*);
    int event034(mesg_flow_node_event*, fopAc_ac_c*);
    int event035(mesg_flow_node_event*, fopAc_ac_c*);
    int event036(mesg_flow_node_event*, fopAc_ac_c*);
    int event037(mesg_flow_node_event*, fopAc_ac_c*);
    int event038(mesg_flow_node_event*, fopAc_ac_c*);
    int event039(mesg_flow_node_event*, fopAc_ac_c*);
    int event040(mesg_flow_node_event*, fopAc_ac_c*);
    int event041(mesg_flow_node_event*, fopAc_ac_c*);
    int event042(mesg_flow_node_event*, fopAc_ac_c*);

    void initWord(fopAc_ac_c*, const char*, u8, int, fopAc_ac_c**);

#if DEBUG
    void dbgPrint();
#endif

    u8 isSelectMessage() { return mSelectMessage; }
    void onNonStopJunpFlowFlag() { mNonStopJunpFlowFlag = 1; }
    u16 getSelectNum() { return query005(NULL, NULL, 0); }
    int getChoiceNo() { return (s16)mChoiceNo; }
    void setMsg(u32 msg) { mMsg = msg; }
    bool checkEndFlow() { return (u32)field_0x26 == 1; }

    static queryFunc mQueryList[53];
    static eventFunc mEventList[43];

private:
    /* 0x04 */ u8* mFlow_p;
    /* 0x08 */ u8* mLabelInfo_p;
    /* 0x0C */ union mesg_flow {
        mesg_flow_node message;
        mesg_flow_node_branch branch;
        mesg_flow_node_event event;
    }* mFlowNodeTBL;
    /* 0x10 */ u16 mNodeIdx;
    /* 0x12 */ u16 field_0x12;
    /* 0x14 */ u16* mFlowIdxTBL;
    /* 0x18 */ u16* field_0x18;
    /* 0x1C */ u16 mFlow;
    /* 0x20 */ u32 mMsg;
    /* 0x24 */ u8 mSelectMessage;
    /* 0x25 */ u8 field_0x25;
    /* 0x26 */ u8 field_0x26;
    /* 0x27 */ u8 field_0x27;
    /* 0x28 */ u32 mMsgNo;
    /* 0x2C */ u32 mNowMsgNo;
    /* 0x30 */ u16 field_0x30;
    /* 0x32 */ u16 mEventId;
    /* 0x34 */ int field_0x34;
    /* 0x38 */ u16 field_0x38;
    /* 0x3A */ u16 mChoiceNo;
    /* 0x3C */ s32 mTimer;
    /* 0x40 */ u8 field_0x40;
    /* 0x41 */ u8 field_0x41;
    /* 0x42 */ u8 field_0x42;
    /* 0x43 */ u8 field_0x43;
    /* 0x44 */ u8 field_0x44;
    /* 0x45 */ u8 field_0x45;
    /* 0x46 */ u8 mSelType;
    /* 0x47 */ u8 field_0x47;
    /* 0x48 */ u8 mNonStopJunpFlowFlag;
}; // size 0x4c

#endif /* D_MSG_D_MSG_FLOW_H */
