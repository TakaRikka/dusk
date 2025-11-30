#ifndef D_MENU_D_MENU_MAP_COMMON_H
#define D_MENU_D_MENU_MAP_COMMON_H

#include "dolphin/types.h"

class J2DPicture;
class JKRArchive;
class dSelect_cursor_c;

enum dMenuMapIcon_e {
    ICON_UNK_0_e,
    ICON_UNK_1_e,
    ICON_DUNGEON_WARP_e,
    ICON_BOSS_GANON_e,
    ICON_BOSS_e,
    ICON_LIGHT_DROP_e,
    ICON_LIGHT_BALL_e,
    ICON_CANNON_BALL_e,
    ICON_COPY_STATUE_e,
    ICON_YETO_e,
    ICON_YETA_e,
    ICON_GOLD_WOLF_e,
    ICON_MONKEY_e,
    ICON_COACH_e,
    ICON_KEY_e,
    ICON_OOCCOO_e,
    ICON_OOCCOO_JR_e,
    ICON_LINK_e,
    ICON_DESTINATION_e,
    ICON_TREASURE_CHEST_e,
    ICON_DUNGEON_ENTER_e,
    ICON_LINK_ENTER_e,
    ICON_LV8_WARP_e,

    ICON_MAX_e,
};

class dMenuMapCommon_c {
public:
    struct IconInfo_s {
        /* 0x00 */ f32 pos_x;
        /* 0x04 */ f32 pos_y;
        /* 0x08 */ f32 scale;
        /* 0x0C */ f32 rotation;
        /* 0x10 */ f32 alpha_rate;
        /* 0x14 */ u8 icon_no;
        /* 0x15 */ u8 _15;
    };

    dMenuMapCommon_c();
    virtual ~dMenuMapCommon_c();
    void initiate(JKRArchive*);
    void drawIcon(f32, f32, f32, f32);
    void iconScale(int, f32, f32, f32);
    bool setIconInfo(u8, f32, f32, f32, f32, f32, u8);
    void clearIconInfo();
    void setBlendRatio(u8, f32, f32);
    void blinkMove(s16);
    void moveLightDropAnime();
    f32 getIconSizeX(u8 index);
    f32 getIconSizeY(u8 index);
    void debugIcon();

    void setCenterPosX(f32 center_pos, u8 param_2) {
        mCenterPosX = center_pos;
        _c90 = param_2;
    }

    struct Stage_c {
        // Incomplete class

        struct data {
            // Unknown name

            /* 0x00 */ char mName[8];
            /* 0x08 */ u8 mRoomNo;
            /* 0x09 */ u8 mRegionNo;
            /* 0x0A */ u16 mAreaName;
            /* 0x0C */ f32 mOffsetX;
            /* 0x10 */ f32 mOffsetZ;
        };

        /* 0x0 */ u8 mCount;
        /* 0x4 */ data mData[0];
    };

    class RoomData_c {
    public:
        struct data {
            /* 0x0 */ u8 mSize;
            /* 0x1 */ u8 field_0x1[3];
            /* 0x4 */ u8 mData[0];
        };

        data* getRoomData() { return mpRoomData; }
        RoomData_c* getNextData() { return mpNextData; }
        void setRoomData(void* i_data) { mpRoomData = (data*)i_data; }
        void setNextData(RoomData_c* i_nextData) { mpNextData = i_nextData; }

        /* 0x0 */ data* mpRoomData;
        /* 0x4 */ RoomData_c* mpNextData;
    };

    /* 0x004 */ J2DPicture* mPictures[23];
    /* 0x060 */ J2DPicture* mLightDropPic;
    /* 0x064 */ dSelect_cursor_c* mpDrawCursor;
    /* 0x068 */ dSelect_cursor_c* mpPortalIcon;
    /* 0x06C */ u32 _6c;
    /* 0x070 */ IconInfo_s mIconInfo[128];
    /* 0xC70 */ u16 mIconNum;
    /* 0xC72 */ s16 mBlinkTimer;
    /* 0xC74 */ s16 mLightDropFlashTimer;
    /* 0xC76 */ u16 _c76;
    /* 0xC78 */ f32 mBlinkAlpha;
    /* 0xC7C */ f32 _c7c;
    /* 0xC80 */ f32 _c80;
    /* 0xC84 */ f32 _c84;
    /* 0xC88 */ f32 _c88;
    /* 0xC8C */ f32 mCenterPosX;
    /* 0xC90 */ u8 _c90;
};

#endif /* D_MENU_D_MENU_MAP_COMMON_H */
