#ifndef D_S_D_S_MENU_H
#define D_S_D_S_MENU_H

#include "JSystem/JUtility/JUTResFont.h"
#include "f_op/f_op_scene_mng.h"

class mDoDvdThd_toMainRam_c;

#if DEBUG
class myFontClass : public JUTResFont {
public:
    myFontClass(const ResFONT* i_font, JKRHeap* i_heap)
        : JUTResFont(i_font, i_heap),
          field_0x70((ResFONT*)JUTResFONT_Ascfont_fix12, i_heap)
    {}

    virtual ~myFontClass();
    virtual f32 drawChar_scale(f32 param_0, f32 param_1, f32 param_2, f32 param_3, int param_4, bool param_5);

    /* 0x70 */ JUTResFont field_0x70;
};

struct menu_category_data_class {
    u8 name[68];
};

struct menu_category_class {
    /* 0x0 */ u8 num;
    /* 0x4 */ menu_category_data_class* data;
};

struct menu_data_class {
    /* 0x00 */ char env_data[65];
    /* 0x41 */ char stage_name[8];
    /* 0x49 */ s8 room_no;
    /* 0x4A */ u8 point;
    /* 0x4B */ s8 layer;
};

struct menu_stage_class {
    /* 0x00 */ u8 field_0x00[0x41 - 0x00];
    /* 0x41 */ u8 field_0x41;
    /* 0x42 */ u8 field_0x42;
    /* 0x43 */ u8 field_0x43;
    /* 0x44 */ menu_data_class* data;
};

struct menu_info_class {
    /* 0x0 */ u8 num;
    /* 0x4 */ menu_stage_class* stage_data;
};

class dScnMenu_c : public scene_class {
public:
    static bool isAutoSelect() { return m_autoSelect; }

    static char cursolStageName[8];
    static int cursolRoomNo;
    static int cursolLayer;
    static int cursolPoint;

    static u8 m_error_flags;
    static bool m_autoSelect;
    static int m_autoCursolID;
    static int m_autoGroupPoint;

    /* 0x1C8 */ request_of_phase_process_class phase;
    /* 0x1D0 */ mDoDvdThd_toMainRam_c* command;
    /* 0x1D4 */ mDoDvdThd_toMainRam_c* fontCommand;
    /* 0x1D8 */ menu_info_class* info;
    /* 0x1DC */ menu_category_class* category_info;
    /* 0x1E0 */ ResFONT* fontRes;
    /* 0x1E4 */ myFontClass* font;
    /* 0x1E8 */ u8 select_point;
    /* 0x1E9 */ u8 cursor_move_timer;
    /* 0x1EA */ u8 group_move_timer;
    /* 0x1EB */ u8 category_move_timer;
    /* 0x1EC */ u8 week_move_timer;
    /* 0x1ED */ u8 current_category;
    /* 0x1EE */ u8 field_0x1ee;
};

extern int g_playerKind;
extern int g_debugHpMode;
extern int g_horsePosInit;
#else
class dScnMenu_c {};
#endif

#endif /* D_S_D_S_MENU_H */
