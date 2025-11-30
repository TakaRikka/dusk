#ifndef D_D_TRESURE_H
#define D_D_TRESURE_H

#include "dolphin/mtx.h"

class dTres_c {
public:
    struct data_s {
        /* 0x00 */ u8 mNo;
        /* 0x01 */ s8 mRoomNo;
        /* 0x02 */ u8 mStatus;
        /* 0x03 */ u8 mArg1;
        /* 0x04 */ Vec mPos;
        /* 0x10 */ u8 mSwBit;
        /* 0x11 */ u8 mType;
        /* 0x12 */ u8 mArg2;
        /* 0x13 */ s8 mAngleY;
    };

    class typeGroupData_c : public data_s {
    public:
        data_s* getDataPointer() { return (data_s*)this; }
        const data_s* getConstDataPointer() const { return (data_s*)this; }
        typeGroupData_c* getNextDataPointer() const { return mNextData; }
        void setNextDataPointer(typeGroupData_c* data) { mNextData = data; }
        void setTypeGroupNo(u8 no) { mTypeGroupNo = no; }
        u8 getNo() const { return mNo; }
        u8 getStatus() const { return mStatus; }
        void setStatus(u8 status) { mStatus = status; }
        void setRoomNo(s8 roomNo) { mRoomNo = roomNo; }
        s8 getRoomNo() const { return mRoomNo; }
        void setPos(const Vec& pos) { mPos = pos; }
        u8 getSwBit() const { return mSwBit; }
        const Vec* getPos() const { return &mPos; }
        int getAngleY() const { return mAngleY; }

        /* 0x14 */ typeGroupData_c* mNextData;
        /* 0x18 */ u8 mTypeGroupNo;
    };  // Size: 0x1C

    struct list_class {
        /* 0x0 */ int field_0x0;
        /* 0x4 */ typeGroupData_c* field_0x4;
        /* 0x8 */ u8 mNumber;
    };

    struct type_group_list {
        /* 0x0 */ typeGroupData_c* field_0x0;
        /* 0x4 */ typeGroupData_c* field_0x4;
        /* 0x8 */ u8 mNumber;
    };

    static int createWork();
    static void create();
    static void remove();
    static void reset();
    static void addData(dTres_c::list_class*, s8);
    static void checkTreasureBox(dTres_c::data_s*);
    static void onStatus(u8, int, int);
    static void offStatus(u8, int, int);
    static bool getBossIconFloorNo(int*);
    static typeGroupData_c* getFirstData(u8);
    static typeGroupData_c* getNextData(dTres_c::typeGroupData_c*);
    static typeGroupData_c* getNextData(dTres_c::typeGroupData_c const*);
    static void setPosition(int, u8, Vec const*, int);
    static int getTypeGroupNoToType(u8);
    static u8 getTypeToTypeGroupNo(u8);

    static int getTypeGroupNumber(int index) {
        return mTypeGroupListAll[index].mNumber;
    }

    static void setPosition(int i_tboxNo, Vec const* i_pos) {
        setPosition(i_tboxNo, 0, i_pos, -1);
    }

    static void setLightDropPostion(int i_tboxNo, const Vec* i_pos) {
        setPosition(i_tboxNo, 4, i_pos, -1);
    }

    static void setIconPositionOfCarryLight(int i_tboxNo, const Vec* i_pos, int param_2) {
        setPosition(i_tboxNo, 12, i_pos, param_2);
    }

    static void setIconPositionOfCstatue(int i_tboxNo, const Vec* i_pos) {
        setPosition(i_tboxNo, 15, i_pos, -1);
    }

    static void setCarryIron(int i_tboxNo, const Vec* i_pos) {
        setPosition(i_tboxNo, 11, i_pos, -1);
    }

    static void offStatus(int param_0, int i_flag) {
        offStatus(0, param_0, i_flag);
    }

    static typeGroupData_c* getTypeGroupData() {
        return mTypeGroupData;
    }

    static u8 const typeToTypeGroup[17][2];
    static type_group_list mTypeGroupListAll[17];
    static typeGroupData_c* mTypeGroupData;
    static u16 mNum;

    static void setNpcYkmPosition(int param_1, Vec* param_2) {
        setPosition(param_1, 13, param_2, -1);
    }

    static void setNpcYkwPosition(int param_1, Vec* param_2) {
        setPosition(param_1, 14, param_2, -1);
    }

    static void setCoachPosition(int param_1, const Vec* param_2, int param_3) {
        setPosition(param_1, 10, param_2, param_3);
    }
};

#endif /* D_D_TRESURE_H */
