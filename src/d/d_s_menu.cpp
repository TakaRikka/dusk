/**
 * d_s_menu.cpp
 * Debug Level Select Menu
 */

#include "d/dolzel.h" // IWYU pragma: keep

#include "d/d_s_menu.h"
#include "d/d_com_inf_game.h"
#include "d/d_com_static.h"
#include "d/d_meter2_info.h"
#include "d/d_s_play.h"
#include "f_ap/f_ap_game.h"
#include "f_op/f_op_msg_mng.h"
#include "m_Do/m_Do_controller_pad.h"
#include "m_Do/m_Do_graphic.h"
#include "m_Do/m_Do_main.h"
#include "JSystem/JFramework/JFWSystem.h"
#include "JSystem/JUtility/JUTDbPrint.h"
#include "JSystem/JUtility/JUTReport.h"

#include <strtoul.h>
 
#if DEBUG
extern "C" int atoi(const char* str);

void dScnMenu_setItem(int i_slotNo, u8 i_itemNo);
void dScnMenu_setPlayerDebugMode();
void dScnMenu_setPlayerItemReset();
bool dScnMenu_setPlayerDebugModeDungeon();

#define ERR_MAP 1
#define ERR_ROOM 2
#define ERR_LAYER 4
#define ERR_POINT 8

int g_playerKind = 0;
int g_debugHpMode = 0;
int g_horsePosInit = 0;

char dScnMenu_c::cursolStageName[8];
int dScnMenu_c::cursolRoomNo = -1;
int dScnMenu_c::cursolLayer = -1;
int dScnMenu_c::cursolPoint = -1;

u8 dScnMenu_c::m_error_flags;
bool dScnMenu_c::m_autoSelect;
int dScnMenu_c::m_autoCursolID;
int dScnMenu_c::m_autoGroupPoint;

s32 l_cursolID;
s8* l_groupPoint;
s32 l_startID;
s32 l_timepat;
s16 l_weekpat;
s16 l_moonpat;

int S_line_cnt;
int S_antei_cnt;
u8 l_languageType;

int dSm_read_get_number(u8* i_data, int* i_pos) {
    BOOL var_r28;
    int pos;
    int number;

    number = 0;
    pos = *i_pos;
    var_r28 = 0;
    if (pos > 0) {
        pos--;
    }

    while (true) {
        if (++pos >= 10000) {
            OS_REPORT("\npreset text error!");
            number = -9;
            break;
        }

        if (i_data[pos] >= 0x30 && i_data[pos] <= 0x39) {
            var_r28 = 1;
            number *= 10;
            number += (i_data[pos] & 0xF);
        } else if (var_r28) {
            if (i_data[pos] == 10) {
                S_line_cnt++;
            }
            break;
        } else if (i_data[pos] == 10) {
            S_line_cnt++;
            number = -1;
            break;
        }
    }

    *i_pos = pos;
    return number;
}

int dSm_read_comp_keyword(int i_pos, u8* i_data, char* i_keyword, int i_size) {
    BOOL diff;
    BOOL var_r29;

    var_r29 = FALSE;
    diff = TRUE;

    if (i_pos == 0 && (&i_data[i_pos])[i_size] < 0x30) {
        var_r29 = TRUE;
    } else if (i_pos != 0 && i_data[i_pos - 1] < 0x30 && (&i_data[i_pos])[i_size] < 0x30) {
        var_r29 = TRUE;
    }

    if (var_r29 && memcmp(&i_data[i_pos], i_keyword, i_size) == 0) {
        diff = FALSE;
    }

    return diff;
}

int dSm_read_comp_spel(int i_pos, u8* i_data) {
    BOOL diff = TRUE;

    if (i_pos != 0) {
        if (i_data[i_pos - 1] == 10 && i_data[i_pos] >= 0x30) {
            diff = FALSE;
        }
    } else if (i_data[i_pos] >= 0x30) {
        diff = FALSE;
    }

    return diff;
}

u8 dSm_read_presettxt(u8* i_data) {
    u8 error = 0;

    OS_REPORT("\n\n\n===========================================");
    // "\n= Preset File Read START             ="
    OS_REPORT("\n= 状況ファイル読み込み  START             =");
    OS_REPORT("\n===========================================\n");

    S_line_cnt = 0;

    for (int pos = 0; pos < 10000; pos++) {
        if (i_data[pos] == (u8)'\n') {
            S_line_cnt++;
        }

        int value;
        if (dSm_read_comp_spel(pos, i_data) == 0) {
            if (memcmp(&i_data[pos], "#", 1) == 0) {
            loop_1:
                if (i_data[pos] != 10 && i_data[pos] != 0) {
                    pos++;
                    goto loop_1;
                }
            } else if (dSm_read_comp_keyword(pos, i_data, "initialize", 10) == 0) {
                value = dSm_read_get_number(i_data, &pos);
                if (value < 0) {
                    OS_REPORT("\ninitialize error!!!!!\n");
                    pos = 10000;
                    error = 1;
                } else if (value == 0) {
                    // "\n Items initialize once"
                    OS_REPORT("\nアイテム類　一旦初期化", value);
                    dComIfG_playerStatusD_pre_clear();
                } else {
                    // "\n Item Debug Settings"
                    OS_REPORT("\nアイテム類　デバッグ設定", value);
                    dComIfG_playerStatusD();
                }
            } else if (dSm_read_comp_keyword(pos, i_data, "hp", 2) == 0) {
                value = dSm_read_get_number(i_data, &pos);
                if (value < 0) {
                    OS_REPORT("\nhp error!!!!!\n");
                    pos = 10000;
                    error = 1;
                } else {
                    OS_REPORT("\nhp max[%d]", value);
                    dComIfGs_setMaxLife(value);
                }

                value = dSm_read_get_number(i_data, &pos);
                if (value < 0) {
                    OS_REPORT("\nhp max to onaji ni [%d]", dComIfGs_getMaxLife());
                    dComIfGs_setLife(dComIfGs_getMaxLife());
                } else {
                    OS_REPORT("\nhp [%d]", value);
                    dComIfGs_setLife((u16)value);
                }
            } else if (dSm_read_comp_keyword(pos, i_data, "mp", 2) == 0) {
                value = dSm_read_get_number(i_data, &pos);
                if (value < 0) {
                    OS_REPORT("\nmp error!!!!!");
                    pos = 10000;
                    error = 1;
                } else {
                    OS_REPORT("\nmp max[%d]", value);
                    dComIfGs_setMaxMagic(value);
                }

                value = dSm_read_get_number(i_data, &pos);
                if (value < 0) {
                    OS_REPORT("\nmp max to onaji ni [%d]", dComIfGs_getMaxMagic());
                    dComIfGs_setMagic(dComIfGs_getMaxMagic());
                } else {
                    OS_REPORT("\nmp [%d]", value);
                    dComIfGs_setMagic(value);
                }
            } else if (dSm_read_comp_keyword(pos, i_data, "sword", 5) == 0) {
                value = dSm_read_get_number(i_data, &pos);
                if (value < 0) {
                    OS_REPORT("\nsword error!!!!!");
                    pos = 10000;
                    error = 1;
                } else {
                    OS_REPORT("\nsword[%d]", value);
                    switch (value) {
                    case 0:
                        dMeter2Info_setSword(fpcNm_ITEM_NONE, false);
                        break;
                    case 1:
                        dMeter2Info_setSword(fpcNm_ITEM_SWORD, false);
                        break;
                    case 2:
                        dMeter2Info_setSword(fpcNm_ITEM_MASTER_SWORD, false);
                        break;
                    case 3:
                        dMeter2Info_setSword(fpcNm_ITEM_WOOD_STICK, false);
                        break;
                    case 4:
                        dMeter2Info_setSword(fpcNm_ITEM_LIGHT_SWORD, false);
                        break;
                    }
                }
            } else if (dSm_read_comp_keyword(pos, i_data, "shield", 6) == 0) {
                value = dSm_read_get_number(i_data, &pos);
                if (value < 0) {
                    OS_REPORT("\nshield error!!!!!\n");
                    pos = 10000;
                    error = 1;
                } else {
                    OS_REPORT("\nshield[%d]", value);
                    switch (value) {
                    case 0:
                        dMeter2Info_setShield(fpcNm_ITEM_NONE, false);
                        break;
                    case 1:
                        dMeter2Info_setShield(fpcNm_ITEM_WOOD_SHIELD, false);
                        break;
                    case 2:
                        dMeter2Info_setShield(fpcNm_ITEM_SHIELD, false);
                        break;
                    case 3:
                        dMeter2Info_setShield(fpcNm_ITEM_HYLIA_SHIELD, false);
                        break;
                    }
                }
            } else if (dSm_read_comp_keyword(pos, i_data, "boomerang", 9) == 0) {
                value = dSm_read_get_number(i_data, &pos);
                if (value < 0) {
                    OS_REPORT("\nboomerang error!!!!!\n");
                    pos = 10000;
                    error = 1;
                } else {
                    OS_REPORT("\nboomerang[%d]", value);
                    switch (value) {
                    case 0:
                        dScnMenu_setItem(SLOT_0, fpcNm_ITEM_NONE);
                        break;
                    case 1:
                        dScnMenu_setItem(SLOT_0, fpcNm_ITEM_BOOMERANG);
                        break;
                    }
                }
            } else if (dSm_read_comp_keyword(pos, i_data, "bow", 3) == 0) {
                value = dSm_read_get_number(i_data, &pos);
                if (value < 0) {
                    OS_REPORT("\nbow error!!!!!\n");
                    pos = 10000;
                    error = 1;
                } else {
                    OS_REPORT("\nbow max[%d]", value);
                    if (value == 0) {
                        dScnMenu_setItem(SLOT_4, fpcNm_ITEM_NONE);
                    } else if (value <= 10) {
                        dScnMenu_setItem(SLOT_4, fpcNm_ITEM_ARROW_10);
                    } else if (value <= 20) {
                        dScnMenu_setItem(SLOT_4, fpcNm_ITEM_ARROW_20);
                    } else {
                        dScnMenu_setItem(SLOT_4, fpcNm_ITEM_ARROW_30);
                    }
                    dComIfGs_setArrowMax(value);
                }

                value = dSm_read_get_number(i_data, &pos);
                if (value < 0) {
                    OS_REPORT("\nbow max to onaji ni [%d]", dComIfGs_getArrowMax());
                    dComIfGs_setArrowNum(dComIfGs_getArrowMax());
                } else {
                    OS_REPORT("\nbow [%d]", value);
                    dComIfGs_setArrowNum(value);
                }
            } else if (dSm_read_comp_keyword(pos, i_data, "arrow-fire", 10) == 0) {
                value = dSm_read_get_number(i_data, &pos);
                if (value < 0) {
                    OS_REPORT("\narror-fire error!!!!!\n");
                    pos = 10000;
                    error = 1;
                } else if (value == 1) {
                    OS_REPORT("\narror-fire[%d]", value);
                    dScnMenu_setItem(SLOT_4, fpcNm_ITEM_LIGHT_ARROW);
                }
            } else if (dSm_read_comp_keyword(pos, i_data, "arrow-light", 11) == 0) {
                value = dSm_read_get_number(i_data, &pos);
                if (value < 0) {
                    OS_REPORT("\narror-light error!!!!!\n");
                    pos = 10000;
                    error = 1;
                } else if (value == 1) {
                    OS_REPORT("\narror-light[%d]", value);
                    dScnMenu_setItem(SLOT_4, fpcNm_ITEM_LIGHT_ARROW);
                }
            } else if (dSm_read_comp_keyword(pos, i_data, "bomb", 4) == 0) {
                value = dSm_read_get_number(i_data, &pos);
                if (value < 0) {
                    OS_REPORT("\nbomb error!!!!!\n");
                    pos = 10000;
                    error = 1;
                } else {
                    OS_REPORT("\nbomb max [%d]", value);
                    if (value == 0) {
                        dScnMenu_setItem(SLOT_15, fpcNm_ITEM_NONE);
                        dComIfGs_setBombMax(value);
                    } else {
                        dScnMenu_setItem(SLOT_15, fpcNm_ITEM_BOMB_BAG_LV1);
                        dComIfGs_setBombMax(value);
                    }
                }

                value = dSm_read_get_number(i_data, &pos);
                if (value < 0) {
                    OS_REPORT("\nbomb max to onaji ni [%d]", dComIfGp_getItemMaxBombNumCount());
                    dComIfGs_setBombNum(dComIfGs_getBombMax());
                } else {
                    OS_REPORT("\nbomb [%d]", value);
                    dComIfGs_setBombNum(value);
                }
            } else if (dSm_read_comp_keyword(pos, i_data, "bottle", 6) == 0) {
                value = dSm_read_get_number(i_data, &pos);
                if (value < 0) {
                    OS_REPORT("\nbottle error!!!!!\n");
                    pos = 10000;
                    error = 1;
                } else {
                    OS_REPORT("\nbottle [%d]", value);
                    if (value <= 4) {
                        for (int i = 0; i < value; i++) {
                            dScnMenu_setItem((u8)(i + SLOT_11), fpcNm_ITEM_EMPTY_BOTTLE);
                        }
                    }
                }
            } else if ((dSm_read_comp_keyword(pos, i_data, "equip-x", 7) == 0) ||
                    (dSm_read_comp_keyword(pos, i_data, "equip-y", 7) == 0) ||
                    (dSm_read_comp_keyword(pos, i_data, "equip-z", 7) == 0))
            {
                int i;
                int equip_btn = 0;
                if (dSm_read_comp_keyword(pos, i_data, "equip-x", 7) == 0) {
                    equip_btn = 0;
                } else if (dSm_read_comp_keyword(pos, i_data, "equip-y", 7) == 0) {
                    equip_btn = 1;
                } else if (dSm_read_comp_keyword(pos, i_data, "equip-z", 7) == 0) {
                    equip_btn = 2;
                }

                for (i = 0; i < 20; i++) {
                    if (dSm_read_comp_keyword(pos + i, i_data, "boomerang", 9) == 0) {
                        dComIfGs_setSelectItemIndex(equip_btn, SLOT_0);
                        break;
                    } else if (dSm_read_comp_keyword(pos + i, i_data, "bomb", 4) == 0) {
                        dComIfGs_setSelectItemIndex(equip_btn, SLOT_15);
                        break;
                    } else if (dSm_read_comp_keyword(pos + i, i_data, "bow", 3) == 0) {
                        dComIfGs_setSelectItemIndex(equip_btn, SLOT_4);
                        break;
                    } else if ((&i_data[pos])[i] == 10) {
                        switch (equip_btn) {
                        case 0:
                            // "\nerror! equip-x is NO ITEM"
                            OS_REPORT("\nerror! equip-x 該当アイテム無し");
                            break;
                        case 1:
                            // "\nerror! equip-y is NO ITEM"
                            OS_REPORT("\nerror! equip-y 該当アイテム無し");
                            break;
                        case 2:
                        default:
                            // "\nerror! equip-zx is NO ITEM"
                            OS_REPORT("\nerror! equip-zx 該当アイテム無し");
                            break;
                        }
                        pos = (10000 - i);
                        error = 1;
                        break;
                    }
                }

                pos += i;
            } else if (dSm_read_comp_keyword(pos, i_data, "end", 3) == 0) {
                // "\nend command!"
                OS_REPORT("\nend指令！");
                break;
            } else if (dSm_read_comp_keyword(pos, i_data, "stage", 5) != 0) {
                // "\nCommand Wrong! [Line%d]=[%-10.10s]"
                OS_REPORT("\nコマンドまちがい！[%d行目]=[%-10.10s]", S_line_cnt + 1, &i_data[pos]);
                pos = 10000;
                error = 1;
                break;
            }
        }

        if (i_data[pos] == 0x1A || i_data[pos] == 0) {
            break;
        }
    }

    OS_REPORT("\n===========================================");
    // "\n= Preset File Read end               ="
    OS_REPORT("\n= 状況ファイル読み込み  end               =");
    OS_REPORT("\n===========================================\n\n\n");
    return error;
}

u8 dSm_read_stageset(u8* i_data) {
    u8 sp8 = 0;
    u8 buffer[10];
    BOOL char_copied = FALSE;

    for (int pos = 0; pos < 10000; pos++) {
        int value;

        if (memcmp(&i_data[pos], "#", 1) == 0) {
        loop_2:
            if (i_data[pos] == 10 || i_data[pos] == 0) {
                S_line_cnt++;
                OS_REPORT("\n6[%d][%x]", pos, i_data[pos]);
            } else {
                pos++;
                goto loop_2;
            }
        }

        if (dSm_read_comp_keyword(pos, i_data, "stage", 5) == 0) {
            pos += 6;
            memset(buffer, 0, 10);

            int buffer_pos = 0;
            for (; buffer_pos < 8; buffer_pos++) {
                if ((&i_data[buffer_pos])[pos] >= 0x30) {
                    buffer[buffer_pos] = (&i_data[buffer_pos])[pos];
                    char_copied = TRUE;
                } else if (char_copied) {
                    buffer[buffer_pos] = 0;
                    break;
                }
            }

            sp8 = 1;
            memcpy(dScnMenu_c::cursolStageName, buffer, 8);
            dScnMenu_c::m_error_flags |= (u8)ERR_MAP;
            pos += buffer_pos;

            value = dSm_read_get_number(i_data, &pos);
            if (value < 0) {
                dScnMenu_c::cursolRoomNo = -1;
                dScnMenu_c::cursolLayer = -1;
                dScnMenu_c::cursolPoint = -1;
                break;
            }

            dScnMenu_c::cursolRoomNo = value;
            dScnMenu_c::m_error_flags |= (u8)ERR_ROOM;
            value = dSm_read_get_number(i_data, &pos);
            if (value < 0) {
                dScnMenu_c::cursolLayer = -1;
                dScnMenu_c::cursolPoint = -1;
                break;
            }

            dScnMenu_c::cursolLayer = value;
            dScnMenu_c::m_error_flags |= (u8)ERR_LAYER;
            value = dSm_read_get_number(i_data, &pos);
            if (value < 0) {
                dScnMenu_c::cursolPoint = -1;
                break;
            }

            dScnMenu_c::cursolPoint = value;
            dScnMenu_c::m_error_flags |= (u8)ERR_POINT;
            break;
        }

        if (i_data[pos] == 0x1A || i_data[pos] == 0) {
            break;
        }
    }

    return sp8;
}

// DEBUG NONMATCHING - regalloc / stack
int dScnMenu_Draw(dScnMenu_c* i_this) {
    static dDlst_2DQuad_c effectQuad;
    static dDlst_2DQuad_c effectQuad2[40];
    static dDlst_2DQuad_c effectQuad3;

    GXColor category_color = {0xFF, 0x00, 0x00, 0x3C};
    effectQuad3.init(50, 55, 280, 38, category_color);
    dComIfGd_set2DOpa(&effectQuad3);

    if (i_this->current_category != 0) {
        JUTReport(50, 55, "[%s]", i_this->category_info->data[i_this->current_category - 1].name);
    } else {
        // "[Uncategorized]"
        JUTReport(50, 55, "[未分類]");
    }

    // "Analog Up/Down - Category Selection"
    JUTReport(340, 39, "アナログ 上下-カテゴリ選択");
    // "     Left/Right - Expand Sub-menu"
    JUTReport(340, 55, "　　　　 左右-サブ拡大");
    // "After Change"
    JUTReport(540, 55, "変更後");

    if (dScnMenu_c::m_error_flags & ERR_MAP) {
        // " (Map Selection Error) "
        JUTReport(150, 55, "（マップ指定の誤り）");
    } else if (dScnMenu_c::m_error_flags & ERR_ROOM) {
        // " (Room Selection Error) "
        JUTReport(150, 55, "（部屋指定の誤り）");
    } else if (dScnMenu_c::m_error_flags & ERR_LAYER) {
        // " (Layer Selection Error) "
        JUTReport(150, 55, "（レイヤー指定の誤り）");
    } else if (dScnMenu_c::m_error_flags & ERR_POINT) {
        // " (Link No Selection Error) "
        JUTReport(150, 55, "（リンクＮｏ指定の誤り）");
    }

    if (i_this->select_point != 0) {
        JUTReport(400, 50, "<%d>", i_this->select_point - 1);
    }

    menu_info_class* menu_info = i_this->info;
    int line_y_pos = 70;
    int dist_from_startID = (l_cursolID - l_startID);
    int line_num = menu_info->num < 20 ? menu_info->num : 20;

    if (dist_from_startID < 5) {
        l_startID += dist_from_startID - 5;
        if (l_startID < 0) {
            l_startID = 0;
        }
    } else if (dist_from_startID > 15) {
        int sp34 = (menu_info->num - 20);
        if (sp34 < 0) {
            sp34 = 0;
        }
        l_startID += dist_from_startID - 15;
        if (l_startID > sp34) {
            l_startID = sp34;
        }
    }

    int line_no = l_startID;
    static int effectQuad2_cnt = 0;

    for (int i = 0; i < line_num; i++) {
        if (line_no < menu_info->num) {
            char map_name[100];
            memset(map_name, 0, 100);
            memset(map_name, 32, 99);
            memcpy(map_name, &menu_info->stage_data[line_no], 30);

            char room_desc[100];
            memset(room_desc, 0, 100);
            memset(room_desc, 32, 99);
            memcpy(room_desc, &menu_info->stage_data[line_no].data[l_groupPoint[line_no]], 27);

            char sub_room_desc[65];
            memset(sub_room_desc, 0, 65);
            memset(sub_room_desc, 32, 64);
            memcpy(sub_room_desc, &menu_info->stage_data[line_no].data[l_groupPoint[line_no]], 60);

            if (i_this->current_category == menu_info->stage_data[line_no].field_0x43) {
                GXColor sp2C = {0x00, 0x00, 0xFF, 0x37};
                effectQuad2[effectQuad2_cnt].init(30, line_y_pos - 16, 540, line_y_pos, sp2C);
                dComIfGd_set2DOpa(&effectQuad2[effectQuad2_cnt]);
                effectQuad2_cnt++;
            }

            if (l_cursolID == line_no) {
                if (fabsf(mDoCPd_c::getStickX(PAD_1)) < 0.5f) {
                    JUTReport(20, line_y_pos, "%c %2d %s", l_cursolID == line_no ? 'O' : ' ',
                            line_no, map_name);
                    JUTReport(300, line_y_pos, "%2d:%s", l_groupPoint[line_no], room_desc);
                } else {
                    JUTReport(20, line_y_pos, " %2d:%s", l_groupPoint[line_no], sub_room_desc);
                }

                GXColor sp28 = {0x00, 0x00, 0x78, 0xFF};
                effectQuad.init(30, line_y_pos - 18, 540, line_y_pos - 2, sp28);
                dComIfGd_set2DOpa(&effectQuad);
            } else {
                if (fabsf(mDoCPd_c::getStickX(PAD_1)) < 0.5f) {
                    JUTReport(20, line_y_pos, "%c %2d %s", l_cursolID == line_no ? 'O' : ' ',
                            line_no, map_name);
                    JUTReport(300, line_y_pos, "%2d:%s", l_groupPoint[line_no], room_desc);
                } else {
                    JUTReport(20, line_y_pos, " %2d:%s", l_groupPoint[line_no], sub_room_desc);
                }
            }

            if (menu_info->stage_data[line_no].field_0x42 != 0xFF) {
                JUTReport(540, line_y_pos, "+%02d", menu_info->stage_data[line_no].field_0x42);
                if (menu_info->stage_data[line_no].field_0x42 <= 5) {
                    u8 alpha = (menu_info->stage_data[line_no].field_0x42 / 5.0f) * 80.0f;
                    GXColor sp24 = {0x14, 0x78, 0x14, 0xDC - alpha};

                    effectQuad2[effectQuad2_cnt].init(540, line_y_pos - 17, 578, line_y_pos - 2,
                                                    sp24);
                    dComIfGd_set2DOpa(&effectQuad2[effectQuad2_cnt]);
                    effectQuad2_cnt++;
                }
            } else {
                JUTReport(540, line_y_pos, "***");
            }
            line_no++;
            line_y_pos += 16;
        } else {
            break;
        }
    }

    char* time_setting[] = {
        "つうじょう(8:00)",  // Normal (8:00)
        "じこくひきつぎ",    // Time Passing
        "あさ固定",          // Fixed Morning
        "ひる固定",          // Fixed Afternoon
        "ゆうがた固定",      // Fixed Dusk
        "よる固定",          // Fixed Night
        "時に固定",          // Time Fixed
    };

    char* season_setting[] = {
        "はる",  // Spring
        "なつ",  // Summer
        "あき",  // Fall
        "ふゆ",  // Winter
        "無視",  // Ignore
    };

    char* moon_setting[] = {
        "1", "2", "3", "4", "5", "6", "7",
    };

    char* light_setting[] = {
        "つうじょう",  // Normal
        "１灯",        // 1 Light
        "２灯",        // 2 Light
        "３灯",        // 3 Light
        "４灯",        // 4 Light
        "５灯",        // 5 Light
        "６灯",        // 6 Light
        "７灯",        // 7 Light
    };

    if (l_timepat >= 6) {
        int sp20 = l_timepat - 6;
        // "(Y or X) Time:%d%s"
        JUTReport(120, 420, "(ＹorＸ）じこく：%d%s", sp20, time_setting[6]);
    } else {
        // "(Y or X) Time:%s"
        JUTReport(120, 420, "(ＹorＸ）じこく：%s", time_setting[l_timepat]);
    }

    // "(3B) Light:%s"
    JUTReport(290, 400, "(３Ｂ)ライト:%s", light_setting[g_env_light.light_mask_type & 0xF]);
    // "(3X) Moon:%s"
    JUTReport(470, 400, "(３Ｘ)月:%s", moon_setting[l_moonpat]);
    // "%d Clears"
    JUTReport(40, 400, "%d周目", dComIfGs_getClearCount());

    static char* language[] = {
        "ENGLISH", "GERMAN", "FRENCH", "SPANISH", "ITALIAN",
    };

    JUTReport(40, 440, "%s", language[dComIfGs_getPalLanguage()]);

    if (g_debugHpMode == 0) {
        // "(3Y) Normal"
        JUTReport(100, 400, "(３Ｙ)通常");
    } else {
        // "(3Y) Invincible"
        JUTReport(100, 400, "(３Ｙ)無敵");
    }

    if (g_playerKind == 0) {
        // "Z: Normal"
        JUTReport(400, 420, "Ｚ：通常");
    } else if (g_playerKind == 1) {
        // "Z: Fixed Wolf"
        JUTReport(400, 420, "Ｚ：狼固定");
    } else if (g_playerKind == 2) {
        // "Z: Ranch Fixed"
        JUTReport(400, 420, "Ｚ：牧童固定");
    } else if (g_playerKind == 3) {
        // "Z: Kokiri Fixed"
        JUTReport(400, 420, "Ｚ：コキリ固定");
    } else if (g_playerKind == 4) {
        // "Z: Zora Fixed"
        JUTReport(400, 420, "Ｚ：ゾーラ固定");
    } else {
        // "Z: Armor Fixed"
        JUTReport(400, 420, "Ｚ：鎧固定");
    }

        /* Main Event - Midna riding / not riding (ON == riding) */
    if (dComIfGs_isEventBit(dSv_event_flag_c::M_067)) {
        // "Midna ON"
        JUTReport(500, 420, "ミドナＯＮ");
    } else {
        // "Midna OFF"
        JUTReport(500, 420, "ミドナＯＦＦ");
    }

    int header_x = 36;
    int sp18 = 40;
    sp18 += 12;
    int header_y = sp18 - 12;

    JUTReport(header_x, header_y, "_DEBUG %s %s", mDoMain::COPYDATE_STRING, "Authorized User");

    if (g_presetHIO.mPresetData[0] != 0) {
        // "Preset File  In Use"
        JUTReport(450, 380, "状況ファイル　使用中");
    }

    return 1;
}

int toGameScene(dScnMenu_c* i_this) {
    int rt;

    if ((strcmp(dComIfGp_getNextStageName(), "F_SP127") == 0 ||
        strcmp(dComIfGp_getNextStageName(), "R_SP127") == 0) &&
        dComIfGp_getNextStageLayer() <= 3)
    {
        g_env_light.fishing_hole_season = dComIfGp_getNextStageLayer() + 1;
    }

    if (strcmp(dComIfGp_getNextStageName(), "OPENING") == 0) {
        /* Main Event - when OFF, wolf carries sword and shield on back */
        dComIfGs_offEventBit(dSv_event_flag_c::M_068);
        /* Main Event - Get shadow crystal (can now transform) */
        dComIfGs_offEventBit(dSv_event_flag_c::M_077);
        /* dSv_event_flag_c::F_0550 - Main Event - Gain ability to use sense */
        dComIfGs_offEventBit((u16)dSv_event_flag_c::saveBitLabels[550]);
        /* Secret techniques - Obtained 1 secret techinques - Shield attack */
        dComIfGs_offEventBit(dSv_event_flag_c::F_0338);
        rt = fopScnM_ChangeReq(i_this, PROC_PLAY_SCENE, 0, 5);
        dComIfGs_init();
        dComIfGp_offEnableNextStage();
        daNpcMsg_setEvtNum(7);
        dComIfGp_setNextStage("F_SP108", 21, 1, 13);
        dComIfGs_resetDan();
        dComIfGs_setRestartRoomParam(0);
    } else {
        if (strcmp(dComIfGp_getNextStageName(), "D_MN54") == 0 ||
            strcmp(dComIfGp_getNextStageName(), "D_MN04A") == 0 ||
            strcmp(dComIfGp_getNextStageName(), "F_SP127") == 0)
        {
            /* Main Event - Get shadow crystal (can now transform) */
            dComIfGs_offEventBit(dSv_event_flag_c::M_077);
        }

        if (g_env_light.light_mask_type != 0) {
            g_env_light.light_mask_type = (u8)(g_env_light.light_mask_type | 0xF0);
        }

        rt = fopScnM_ChangeReq(i_this, PROC_PLAY_SCENE, 0, 5);
        dComIfGs_setRestartRoomParam(0);
    }

    return rt;
}

// Matching ?
void setEnvData(menu_data_class* i_data) {
    int i;
    int time = 99;
    dComIfGs_setTime(120.0f);

    for (i = 0; i < 65; i++) {
        if (memcmp(&i_data->env_data[i], "TIME", 4) == 0) {
            time = (i_data->env_data[i + 4] & 0xF) * 10;
            time += i_data->env_data[i + 5] & 0xF;
            break;
        }
    }

    if (time != 99) {
        dComIfGs_setTime(15.0f * (f32)time);
        g_env_light.daytime = 15.0f * (f32)time;
    }

    for (int i = 0; i < 65; i++) {
        if (memcmp(&i_data->env_data[i], "(光０)", 6) == 0) {
            // "(Light 0)"
            dComIfGs_onDarkClearLV(0);
            dComIfGs_onDarkClearLV(1);
            dComIfGs_onDarkClearLV(2);
            dComIfGs_onDarkClearLV(3);
            dComIfGs_onDarkClearLV(4);
            dComIfGs_onDarkClearLV(5);
        } else if (memcmp(&i_data->env_data[i], "(光１)", 6) == 0) {
            // "(Light 1)"
            dComIfGs_onDarkClearLV(0);
        } else if (memcmp(&i_data->env_data[i], "(光２)", 6) == 0) {
            // "(Light 2)"
            dComIfGs_onDarkClearLV(1);
        } else if (memcmp(&i_data->env_data[i], "(光３)", 6) == 0) {
            // "(Light 3)"
            dComIfGs_onDarkClearLV(2);
        } else if (memcmp(&i_data->env_data[i], "(光４)", 6) == 0) {
            // "(Light 4)"
            dComIfGs_onDarkClearLV(3);
        } else if (memcmp(&i_data->env_data[i], "(光５)", 6) == 0) {
            // "(Light 5)"
            dComIfGs_onDarkClearLV(4);
        } else if (memcmp(&i_data->env_data[i], "剣無", 4) == 0) {
            // "No Sword"
            dMeter2Info_setSword(fpcNm_ITEM_NONE, false);
            dComIfGs_offItemFirstBit(fpcNm_ITEM_WOOD_STICK);
            dComIfGs_offItemFirstBit(fpcNm_ITEM_SWORD);
            dComIfGs_offItemFirstBit(fpcNm_ITEM_MASTER_SWORD);
            dComIfGs_offItemFirstBit(fpcNm_ITEM_LIGHT_SWORD);
            dMeter2Info_setShield(fpcNm_ITEM_NONE, false);
            dComIfGs_offItemFirstBit(fpcNm_ITEM_SHIELD);
            dComIfGs_offItemFirstBit(fpcNm_ITEM_WOOD_SHIELD);
            dComIfGs_offItemFirstBit(fpcNm_ITEM_HYLIA_SHIELD);
        } else if (memcmp(&i_data->env_data[i], "神剣", 4) == 0) {
            // "Master Sword"
            dMeter2Info_setSword(fpcNm_ITEM_MASTER_SWORD, false);
            dComIfGs_onItemFirstBit(fpcNm_ITEM_WOOD_STICK);
            dComIfGs_onItemFirstBit(fpcNm_ITEM_SWORD);
            dComIfGs_onItemFirstBit(fpcNm_ITEM_MASTER_SWORD);
            dComIfGs_onItemFirstBit(fpcNm_ITEM_LIGHT_SWORD);
        } else if (memcmp(&i_data->env_data[i], "木刀", 4) == 0) {
            // "Wooden Sword"
            dMeter2Info_setSword(fpcNm_ITEM_WOOD_STICK, false);
            dComIfGs_onItemFirstBit(fpcNm_ITEM_WOOD_STICK);
            dMeter2Info_setShield(fpcNm_ITEM_NONE, false);
            dComIfGs_offItemFirstBit(fpcNm_ITEM_SHIELD);
            dComIfGs_offItemFirstBit(fpcNm_ITEM_WOOD_SHIELD);
            dComIfGs_offItemFirstBit(fpcNm_ITEM_HYLIA_SHIELD);
        } else if (memcmp(&i_data->env_data[i], "童", 2) == 0) {
            // "Child"
            if (g_playerKind == 0) {
                g_playerKind = 2;
            }
        } else if (memcmp(&i_data->env_data[i], "狼", 2) == 0) {
            // "Wolf"
            if (g_playerKind == 0) {
                g_playerKind = 1;
            }
        } else if ((dScnMenu_c::isAutoSelect() && i == 0) ||
                memcmp(&i_data->env_data[i], "緑服", 4) == 0)
        {
            // "Green Tunic"
            if (g_playerKind == 0) {
                g_playerKind = 3;
                dMeter2Info_setSword(fpcNm_ITEM_SWORD, false);
                dMeter2Info_setShield(fpcNm_ITEM_HYLIA_SHIELD, false);
            }
        } else if (memcmp(&i_data->env_data[i], "SET", 3) == 0 && i_data->env_data[i + 3] >= '0' &&
                i_data->env_data[i + 3] <= '9')
        {
            i += 3;

            while (true) {
                char string[4];
                int label_no;
                int j = 0;

                memset(string, 0, 4);

                while (true) {
                    if (i_data->env_data[i] >= '0' && i_data->env_data[i] <= '9' && j < 3) {
                        string[j] = i_data->env_data[i];

                        j++;
                        i++;
                    } else {
                        break;
                    }
                }

                label_no = atoi(string);
                if (label_no >= 0 && label_no <= 999) {
                    dComIfGs_onEventBit((u16)dSv_event_flag_c::saveBitLabels[label_no]);
                }

                if (i_data->env_data[i] != '+') {
                    break;
                }
                i++;
            }

            i--;
        } else if (memcmp(&i_data->env_data[i], "RESET", 5) == 0 &&
                i_data->env_data[i + 5] >= '0' && i_data->env_data[i + 5] <= '9')
        {
            i += 5;

            while (true) {
                char string[4];
                int label_no;
                int j = 0;
                
                memset(string, 0, 4);

                while (true) {
                    if (i_data->env_data[i] >= '0' && i_data->env_data[i] <= '9' && j < 3) {
                        string[j] = i_data->env_data[i];

                        j++;
                        i++;
                    } else {
                        break;
                    }
                }

                label_no = atoi(string);
                if (label_no >= 0 && label_no <= 999) {
                    dComIfGs_offEventBit((u16)dSv_event_flag_c::saveBitLabels[label_no]);
                }

                if (i_data->env_data[i] != '+') {
                    break;
                }
                i++;
            }

            i--;
        } else if (memcmp(&i_data->env_data[i], "SETT", 4) == 0) {
            i += 4;

            while (true) {
                char string[4];
                int label_no;
                int j = 0;
                
                memset(string, 0, 4);

                while (true) {
                    if (i_data->env_data[i] >= '0' && i_data->env_data[i] <= '9' && j < 3) {
                        string[j] = i_data->env_data[i];

                        j++;
                        i++;
                    } else {
                        break;
                    }
                }

                label_no = atoi(string);
                if (label_no >= 0 && label_no <= 999) {
                    dComIfGs_onTmpBit((u16)dSv_event_tmp_flag_c::tempBitLabels[label_no]);
                    OS_WARNING("onTmpBit[%d]\n", label_no);
                }

                if (i_data->env_data[i] != '+') {
                    break;
                }
                i++;
            }

            i--;
        } else if (memcmp(&i_data->env_data[i], "RESETT", 6) == 0) {
            i += 6;

            while (true) {
                char string[4];
                int label_no;
                int j = 0;
                
                memset(string, 0, 4);

                while (true) {
                    if (i_data->env_data[i] >= '0' && i_data->env_data[i] <= '9' && j < 3) {
                        string[j] = i_data->env_data[i];

                        j++;
                        i++;
                    } else {
                        break;
                    }
                }

                label_no = atoi(string);
                if (label_no >= 0 && label_no <= 999) {
                    dComIfGs_offTmpBit((u16)dSv_event_tmp_flag_c::tempBitLabels[label_no]);
                    OS_WARNING("offTmpBit[%d]\n", label_no);
                }

                if (i_data->env_data[i] != '+') {
                    break;
                }
                i++;
            }

            i--;
        } else if (memcmp(&i_data->env_data[i], "鉄靴", 4) == 0) {
            // "Iron Boots"
            dScnMenu_setItem(SLOT_3, fpcNm_ITEM_HVY_BOOTS);
            dComIfGs_setSelectItemIndex(SELECT_ITEM_Y, SLOT_3);
        } else if (memcmp(&i_data->env_data[i], "水服", 4) == 0) {
            // "Water Tunic"
            dMeter2Info_setCloth(fpcNm_ITEM_WEAR_ZORA, false);
        } else if (memcmp(&i_data->env_data[i], "(初)", 4) == 0 ||
                memcmp(&i_data->env_data[i], "（初）", 6) == 0)
        {
            // "(First)"
            dComIfGs_offGetMagicUseFlag();
            dComIfGs_offItemFirstBit(fpcNm_ITEM_BOW);
        } else if (memcmp(&i_data->env_data[i], "ミドナ瀕死", 10) == 0) {
            // "Midna Dying"
            dComIfGs_onDarkClearLV(0);
            dComIfGs_onDarkClearLV(1);
            dComIfGs_onDarkClearLV(2);
            dComIfGs_offDarkClearLV(3);
            dComIfGs_offDarkClearLV(4);
            dComIfGs_onTransformLV(0);
            dComIfGs_onTransformLV(1);
            dComIfGs_onTransformLV(2);
            dComIfGs_onTransformLV(3);
        } else if (memcmp(&i_data->env_data[i], "変身自由", 8) == 0) {
            // "Transform Unlocked"
            dComIfGs_onDarkClearLV(0);
            dComIfGs_onDarkClearLV(1);
            dComIfGs_onDarkClearLV(2);
            dComIfGs_onDarkClearLV(3);
            dComIfGs_onTransformLV(0);
            dComIfGs_onTransformLV(1);
            dComIfGs_onTransformLV(2);
            dComIfGs_onTransformLV(3);
            /* Cutscene - [cutscene: 21] reunion with Zelda / Midna revived (Hyrule Castle barrier appears) */
            dComIfGs_onEventBit(dSv_event_flag_c::F_0250);
        } else if (memcmp(&i_data->env_data[i], "奥１", 4) == 0) {
            // Hidden Skill 1
            /* Secret techniques - Obtained 2 secret techinques */
            dComIfGs_onEventBit(dSv_event_flag_c::F_0339);
        } else if (memcmp(&i_data->env_data[i], "奥２", 4) == 0) {
            // Hidden Skill 2
            /* Secret techniques - Obtained 1 secret techinques - Shield attack */
            dComIfGs_onEventBit(dSv_event_flag_c::F_0338);
            /* Secret techniques - Obtained 2 secret techinques */
            dComIfGs_onEventBit(dSv_event_flag_c::F_0339);
        } else if (memcmp(&i_data->env_data[i], "奥３", 4) == 0) {
            // Hidden Skill 3
            /* Secret techniques - Obtained 1 secret techinques - Shield attack */
            dComIfGs_onEventBit(dSv_event_flag_c::F_0338);
            /* Secret techniques - Obtained 2 secret techinques */
            dComIfGs_onEventBit(dSv_event_flag_c::F_0339);
            /* Secret techniques - Obtained 3 secret techinques */
            dComIfGs_onEventBit(dSv_event_flag_c::F_0340);
        } else if (memcmp(&i_data->env_data[i], "奥４", 4) == 0) {
            // Hidden Skill 4
            /* Secret techniques - Obtained 1 secret techinques - Shield attack */
            dComIfGs_onEventBit(dSv_event_flag_c::F_0338);
            /* Secret techniques - Obtained 2 secret techinques */
            dComIfGs_onEventBit(dSv_event_flag_c::F_0339);
            /* Secret techniques - Obtained 3 secret techinques */
            dComIfGs_onEventBit(dSv_event_flag_c::F_0340);
            /* Secret techniques - Obtained 4 secret techinques */
            dComIfGs_onEventBit(dSv_event_flag_c::F_0341);
        } else if (memcmp(&i_data->env_data[i], "奥５", 4) == 0) {
            // Hidden Skill 5
            /* Secret techniques - Obtained 1 secret techinques - Shield attack */
            dComIfGs_onEventBit(dSv_event_flag_c::F_0338);
            /* Secret techniques - Obtained 2 secret techinques */
            dComIfGs_onEventBit(dSv_event_flag_c::F_0339);
            /* Secret techniques - Obtained 3 secret techinques */
            dComIfGs_onEventBit(dSv_event_flag_c::F_0340);
            /* Secret techniques - Obtained 4 secret techinques */
            dComIfGs_onEventBit(dSv_event_flag_c::F_0341);
            /* Secret techniques - Obtained 5 secret techinques */
            dComIfGs_onEventBit(dSv_event_flag_c::F_0342);
        } else if (memcmp(&i_data->env_data[i], "奥６", 4) == 0) {
            // Hidden Skill 6
            /* Secret techniques - Obtained 1 secret techinques - Shield attack */
            dComIfGs_onEventBit(dSv_event_flag_c::F_0338);
            /* Secret techniques - Obtained 2 secret techinques */
            dComIfGs_onEventBit(dSv_event_flag_c::F_0339);
            /* Secret techniques - Obtained 3 secret techinques */
            dComIfGs_onEventBit(dSv_event_flag_c::F_0340);
            /* Secret techniques - Obtained 4 secret techinques */
            dComIfGs_onEventBit(dSv_event_flag_c::F_0341);
            /* Secret techniques - Obtained 5 secret techinques */
            dComIfGs_onEventBit(dSv_event_flag_c::F_0342);
            /* Secret techniques - Obtained 6 secret techinques */
            dComIfGs_onEventBit(dSv_event_flag_c::F_0343);
        } else if (memcmp(&i_data->env_data[i], "奥７", 4) == 0) {
            // Hidden Skill 7
            /* Secret techniques - Obtained 1 secret techinques - Shield attack */
            dComIfGs_onEventBit(dSv_event_flag_c::F_0338);
            /* Secret techniques - Obtained 2 secret techinques */
            dComIfGs_onEventBit(dSv_event_flag_c::F_0339);
            /* Secret techniques - Obtained 3 secret techinques */
            dComIfGs_onEventBit(dSv_event_flag_c::F_0340);
            /* Secret techniques - Obtained 4 secret techinques */
            dComIfGs_onEventBit(dSv_event_flag_c::F_0341);
            /* Secret techniques - Obtained 5 secret techinques */
            dComIfGs_onEventBit(dSv_event_flag_c::F_0342);
            /* Secret techniques - Obtained 6 secret techinques */
            dComIfGs_onEventBit(dSv_event_flag_c::F_0343);
            /* Secret techniques - Obtained 7 secret techinques */
            dComIfGs_onEventBit(dSv_event_flag_c::F_0344);
        }
    }
}

// DEBUG NONMATCHING - some stack issues
int dScnMenu_Execute(dScnMenu_c* i_this) {
    static const u8 l_autoCancel[2][2] = {
        {99, 99},
        {0, 0},
    };

    menu_info_class* menu_info = i_this->info;

    if (!mDoCPd_c::getHoldL(PAD_1) && !mDoCPd_c::getHoldR(PAD_1) &&
        mDoCPd_c::getAnalogL(PAD_1) > 0.8f && mDoCPd_c::getAnalogR(PAD_1) > 0.8f &&
        mDoCPd_c::getTrigA(PAD_1))
    {
        dScnMenu_c::m_autoSelect = true;
        dScnMenu_c::m_autoCursolID = l_cursolID;
        dScnMenu_c::m_autoGroupPoint = l_groupPoint[l_cursolID];
    }

    if (dScnMenu_c::m_autoSelect) {
        if (dScnMenu_c::m_autoCursolID > menu_info->num) {
            dScnMenu_c::m_autoSelect = false;
            goto block_24;
        } else if (i_this->field_0x1ee == 0) {
            menu_data_class* menu_data = &menu_info->stage_data[dScnMenu_c::m_autoCursolID]
                                            .data[dScnMenu_c::m_autoGroupPoint];
            dComIfGp_offEnableNextStage();
            dComIfGp_setNextStage(menu_data->stage_name, menu_data->point, menu_data->room_no,
                                menu_data->layer);
            g_playerKind = 0;
            setEnvData(menu_data);

            if (toGameScene(i_this)) {
                OS_REPORT("\x1b[45;30m自動メニュー選択（<%s>ステージ<%s> レイヤー<%d> 部屋Ｎｏ<%d> "
                        "プレイヤーＮｏ<%d> ---<%d,%d>）\n\x1b[40;37m",
                        menu_data->env_data, menu_data->stage_name, menu_data->layer,
                        menu_data->room_no, menu_data->point, dScnMenu_c::m_autoCursolID,
                        dScnMenu_c::m_autoGroupPoint);

                u8 sp8;
                do {
                    if (++dScnMenu_c::m_autoGroupPoint >=
                        menu_info->stage_data[dScnMenu_c::m_autoCursolID].field_0x41)
                    {
                        dScnMenu_c::m_autoCursolID++;
                        dScnMenu_c::m_autoGroupPoint = 0;
                    }

                    sp8 = 0;

                    const u8* sp34 = l_autoCancel[0];
                    for (u32 i = 0; i < 1; i++) {
                        if (dScnMenu_c::m_autoCursolID >= 82 ||
                            (dScnMenu_c::m_autoCursolID == sp34[0] &&
                            dScnMenu_c::m_autoGroupPoint == sp34[1]))
                        {
                            sp8 = 1;
                            break;
                        }

                        sp34 += 2;
                    }
                } while (sp8 != 0);

                i_this->field_0x1ee = 1;
            }
        }
        return 1;
    }

block_24:
    if (mDoCPd_c::getHoldUp(PAD_1) || mDoCPd_c::getHoldDown(PAD_1)) {
        BOOL move_cursor = TRUE;

        if (mDoCPd_c::getTrigUp(PAD_1) || mDoCPd_c::getTrigDown(PAD_1)) {
            i_this->cursor_move_timer = 20;
        } else {
            if (cLib_calcTimer<u8>(&i_this->cursor_move_timer) == 0) {
                i_this->cursor_move_timer = 4;
            } else {
                move_cursor = FALSE;
            }
        }

        if (move_cursor) {
            if (mDoCPd_c::getHoldUp(PAD_1)) {
                if (--l_cursolID < 0) {
                    l_cursolID = menu_info->num - 1;
                }
            } else {
                if (++l_cursolID >= menu_info->num) {
                    l_cursolID = 0;
                }
            }

            i_this->current_category = menu_info->stage_data[l_cursolID].field_0x43;
        }
    }

    f32 stick_x = mDoCPd_c::getStickX(PAD_1);
    f32 stick_y = mDoCPd_c::getStickY(PAD_1);

    if (fabsf(stick_y) > 0.5f) {
        BOOL move_cursor = TRUE;
        if (i_this->category_move_timer == 20) {
            i_this->category_move_timer--;
        } else {
            if (cLib_calcTimer<u8>(&i_this->category_move_timer) == 0) {
                i_this->category_move_timer = 19;
            } else {
                move_cursor = FALSE;
            }
        }

        if (move_cursor) {
            if (stick_y > 0.0f) {
                if (i_this->current_category != 0) {
                    i_this->current_category--;
                } else {
                    i_this->current_category = i_this->category_info->num;
                }

                for (int i = 0; i < menu_info->num; i++) {
                    if (i_this->current_category == menu_info->stage_data[i].field_0x43) {
                        l_cursolID = i;
                        break;
                    }
                }
            } else {
                if (i_this->current_category < i_this->category_info->num) {
                    i_this->current_category++;
                } else {
                    i_this->current_category = 0;
                }

                for (int i = 0; i < menu_info->num; i++) {
                    if (i_this->current_category == menu_info->stage_data[i].field_0x43) {
                        l_cursolID = i;
                        break;
                    }
                }
            }

            l_startID = l_cursolID;
        }
    } else {
        i_this->category_move_timer = 20;
    }

    if (mDoCPd_c::getHoldB(PAD_1) || mDoCPd_c::getHoldA(PAD_1)) {
        BOOL change_point = TRUE;
        if (mDoCPd_c::getTrigB(PAD_1) || mDoCPd_c::getTrigA(PAD_1)) {
            i_this->group_move_timer = 20;
        } else {
            if (cLib_calcTimer<u8>(&i_this->group_move_timer) == 0) {
                i_this->group_move_timer = 4;
            } else {
                change_point = FALSE;
            }
        }

        if (change_point) {
            if (mDoCPd_c::getHoldB(PAD_1)) {
                if (--l_groupPoint[l_cursolID] < 0) {
                    l_groupPoint[l_cursolID] = menu_info->stage_data[l_cursolID].field_0x41 - 1;
                }
            } else {
                if (++l_groupPoint[l_cursolID] >= menu_info->stage_data[l_cursolID].field_0x41) {
                    l_groupPoint[l_cursolID] = 0;
                }
            }
        }
    }

    if (mDoCPd_c::getTrigR(PAD_1)) {
        if (++i_this->select_point == 0) {
            i_this->select_point++;
        }
    } else if (mDoCPd_c::getTrigL(PAD_1)) {
        if (--i_this->select_point == 0) {
            i_this->select_point--;
        }
    }

    if (mDoCPd_c::getTrigStart(PAD_1) || g_presetHIO.field_0x2717 == 2) {
        menu_data_class* data = &menu_info->stage_data[l_cursolID].data[l_groupPoint[l_cursolID]];
        dComIfGp_offEnableNextStage();

        int point;
        if (i_this->select_point != 0) {
            point = i_this->select_point - 1;
        } else {
            point = data->point;
        }

        dComIfGp_setNextStage(data->stage_name, point, data->room_no, data->layer);
        setEnvData(data);

        if (g_presetHIO.field_0x2717 == 2) {
            g_presetHIO.field_0x2717 = 0;
            dSm_read_stageset(g_presetHIO.mPresetData);
            fopScnM_ChangeReq(i_this, PROC_MENU_SCENE, 0, 5);
        } else {
            dScnMenu_c::cursolStageName[0] = 0;
            toGameScene(i_this);
        }

        dScnMenu_setPlayerDebugMode();
    }

    if (mDoCPd_c::getTrigY(PAD_1)) {
        if (--l_timepat < 0) {
            l_timepat = 29;
        }
    } else if (mDoCPd_c::getTrigX(PAD_1)) {
        if (++l_timepat > 29) {
            l_timepat = 0;
        }
    }

    f32 substick_x = mDoCPd_c::getSubStickX(PAD_1);
    if (fabsf(substick_x) > 0.5f) {
        if (i_this->week_move_timer == 0) {
            if (substick_x < 0.0f) {
                if (--l_weekpat < 0) {
                    l_weekpat = 4;
                }
            } else {
                if (++l_weekpat >= 5) {
                    l_weekpat = 0;
                }
            }
            i_this->week_move_timer = 99;
        }
    } else {
        i_this->week_move_timer = 0;
    }

    f32 temp_f27 = mDoCPd_c::getSubStickX(PAD_1);
    if (mDoCPd_c::getTrigX(PAD_3)) {
        if (++l_moonpat >= 7) {
            l_moonpat = 0;
        }
    }

    dComIfGs_setDate(l_moonpat);

    if (mDoCPd_c::getTrigB(PAD_3)) {
        g_env_light.light_mask_type++;
        if (g_env_light.light_mask_type > 7) {
            g_env_light.light_mask_type = 0;
        }
    }

    if (mDoCPd_c::getTrigZ(PAD_1)) {
        g_playerKind++;
        if (g_playerKind == 6) {
            g_playerKind = 0;
        }
    }

    g_horsePosInit = 1;

    if (mDoCPd_c::getTrigY(PAD_3)) {
        g_debugHpMode ^= 1;
    }

    if (mDoCPd_c::getTrigA(PAD_3)) {
            /* Main Event - Midna riding / not riding (ON == riding) */
        if (dComIfGs_isEventBit(dSv_event_flag_c::M_067)) {
            /* Main Event - Midna riding / not riding (ON == riding) */
            dComIfGs_offEventBit(dSv_event_flag_c::M_067);
            /* Inside Hyrule Castle - Midna removes wolf's chains in prison */
            dComIfGs_offEventBit(dSv_event_flag_c::M_011);
            dComIfGs_offGetMagicUseFlag();

            for (int i = 0; i < 4; i++) {
                dComIfGs_offLightDropGetFlag(i);
            }
        } else {
            /* Main Event - Midna riding / not riding (ON == riding) */
            dComIfGs_onEventBit(dSv_event_flag_c::M_067);
            /* Inside Hyrule Castle - Midna removes wolf's chains in prison */
            dComIfGs_onEventBit(dSv_event_flag_c::M_011);

            for (int i = 0; i < 4; i++) {
                dComIfGs_onLightDropGetFlag(i);
            }
        }
    }

    g_env_light.time_change_rate = 0.0f;

    if (l_timepat >= 6) {
        f32 time = 15.0f * (f32)(l_timepat - 6);
        dComIfGs_setTime(time);
        g_env_light.time_change_rate = 1000.0f;
    } else {
        switch (l_timepat) {
        case 0:
        case 1:
            break;
        case 2:
            dComIfGs_setTime(90.0f);
            g_env_light.daytime = 90.0f;
            g_env_light.time_change_rate = 1000.0f;
            break;
        case 3:
            dComIfGs_setTime(195.0f);
            g_env_light.daytime = 195.0f;
            g_env_light.time_change_rate = 1001.0f;
            break;
        case 4:
            dComIfGs_setTime(255.0f);
            g_env_light.daytime = 255.0f;
            g_env_light.time_change_rate = 1002.0f;
            break;
        case 5:
            dComIfGs_setTime(315.0f);
            g_env_light.daytime = 315.0f;
            g_env_light.time_change_rate = 1003.0f;
            break;
        }
    }

    if (S_antei_cnt > 30) {
        g_presetHIO.exePreset();
    } else {
        S_antei_cnt++;
    }

    return 1;
}

int dScnMenu_IsDelete(dScnMenu_c* i_this) {
    "Is Delete -> Menu scene\n";
    return 1;
}

int dScnMenu_Delete(dScnMenu_c* i_this) {
    "dScnMenu_Delete():\n";

    JUTDbPrint::getManager()->changeFont(JFWSystem::getSystemFont());
    delete i_this->font;

    JKRFree(i_this->info);
    JKRFree(i_this->fontRes);
    fapGmHIO_offMenu();

    if (g_presetHIO.mPresetData[0] != 0) {
        g_presetHIO.field_0x2716 = dSm_read_presettxt(g_presetHIO.mPresetData);
        if (g_presetHIO.field_0x2716 != 0) {
            dComIfG_playerStatusD();
        }
    } else {
        dComIfGp_itemDataInit();
    }

    fapGmHIO_offMenu();
    return 1;
}

int phase_1(dScnMenu_c* i_this) {
    if (!dStage_roomControl_c::resetArchiveBank(0)) {
        return cPhs_INIT_e;
    }

    JKRHeap* heap = mDoExt_getZeldaHeap();
    s32 total_free = heap->getTotalFreeSize();
    "dScnMenu_Create(): TotalFreeSize=%d\n";
    OS_REPORT("Menu Create !!\n");

    i_this->command = mDoDvdThd_toMainRam_c::create("/res/Menu/Menu1.dat", 0, NULL);
    JUT_ASSERT(3083, i_this->command != NULL);

    i_this->fontCommand = mDoDvdThd_toMainRam_c::create("/res/Menu/kanfont_fix16.bfn", 0, NULL);
    JUT_ASSERT(3086, i_this->fontCommand != NULL);

    dComIfG_playerStatusD();
    dComIfGs_offDarkClearLV(0);
    dComIfGs_offDarkClearLV(1);
    dComIfGs_offDarkClearLV(2);
    dComIfGs_offDarkClearLV(3);
    dComIfGs_offDarkClearLV(4);
    dComIfGs_offDarkClearLV(5);
    dComIfGs_offTransformLV(0);
    dComIfGs_offTransformLV(1);
    dComIfGs_offTransformLV(2);
    dComIfGs_offTransformLV(3);
    g_env_light.light_mask_type = 0;
    return cPhs_NEXT_e;
}

u8 search(menu_info_class* i_info) {
    int index;
    u8 rt = 0;
    if (dScnMenu_c::cursolStageName[0] != 0) {
        index = i_info->num - 1;
        while (index >= 0) {
            menu_data_class* data = i_info->stage_data[index].data;

            for (s8 i = 0; i < i_info->stage_data[index].field_0x41; i++) {
                if (strcmp(data->stage_name, dScnMenu_c::cursolStageName) == 0) {
                    l_cursolID = index;
                    rt = 1;
                    l_groupPoint[l_cursolID] = i;

                    dScnMenu_c::m_error_flags &= (u8)~ERR_MAP;
                    if (data->room_no == dScnMenu_c::cursolRoomNo) {
                        dScnMenu_c::m_error_flags &= (u8)~ERR_ROOM;
                        if (data->layer == dScnMenu_c::cursolLayer) {
                            dScnMenu_c::m_error_flags &= (u8)~ERR_LAYER;
                            if (data->point == dScnMenu_c::cursolPoint) {
                                dScnMenu_c::m_error_flags &= (u8)~ERR_POINT;
                                return rt;
                            }

                            if (dScnMenu_c::cursolPoint == -1) {
                                return rt;
                            }
                        } else if (dScnMenu_c::cursolLayer == -1) {
                            return rt;
                        }
                    }
                }
                data++;
            }
            index--;
        }

        dScnMenu_c::cursolStageName[0] = 0;
    }

    return rt;
}

int phase_2(dScnMenu_c* i_this) {
    if (!i_this->command->sync() || !i_this->fontCommand->sync()) {
        return cPhs_INIT_e;
    }

    i_this->info = (menu_info_class*)i_this->command->getMemAddress();
    JUT_ASSERT(3237, i_this->info != NULL);

    menu_info_class* menu_info = i_this->info;
    menu_info->stage_data = (menu_stage_class*)((uintptr_t)menu_info->stage_data + (uintptr_t)menu_info);

    for (int i = 0; i < menu_info->num; i++) {
        menu_info->stage_data[i].data =
            (menu_data_class*)((uintptr_t)menu_info->stage_data[i].data + (uintptr_t)menu_info);
    }

    if (l_groupPoint == NULL) {
        l_groupPoint = new s8[menu_info->num];
        JUT_ASSERT(3252, l_groupPoint != NULL);

        if (mDoExt_getSafeZeldaHeapSize() >= 0) {
            mDoExt_addSafeZeldaHeapSize(-(mDoExt_getZeldaHeap()->getSize(l_groupPoint) + 0x10));
        }

        for (int i = 0; i < menu_info->num; i++) {
            l_groupPoint[i] = 0;
        }
    }

    menu_category_class* category_info;

    u32 category_offset = 8;
    category_offset += menu_info->num * sizeof(menu_stage_class);

    for (int i = 0; i < menu_info->num; i++) {
        category_offset += menu_info->stage_data[i].field_0x41 * sizeof(menu_data_class);
    }

    i_this->category_info = (menu_category_class*)(&((u8*)i_this->command->getMemAddress())[category_offset]);
    JUT_ASSERT(3282, i_this->category_info != NULL);

    category_info = i_this->category_info;
    category_info->data = (menu_category_data_class*)((uintptr_t)category_info->data + (uintptr_t)menu_info);
    i_this->current_category = menu_info->stage_data[l_cursolID].field_0x43;

    delete i_this->command;

    u8 sp8 = search(menu_info);

    i_this->fontRes = (ResFONT*)i_this->fontCommand->getMemAddress();
    delete i_this->fontCommand;

    if (i_this->fontRes != NULL) {
        i_this->font = new myFontClass(i_this->fontRes, NULL);
        if (i_this->font != NULL) {
            JUTDbPrint::getManager()->changeFont(i_this->font);
        }
    }

    mDoGph_gInf_c::setTickRate(OS_TIMER_CLOCK / 60);
    mDoGph_gInf_c::setBackColor(g_clearColor);
    fapGmHIO_onMenu();
    i_this->current_category = menu_info->stage_data[l_cursolID].field_0x43;

    /* Main Event - Get shadow crystal (can now transform) */
    dComIfGs_onEventBit(dSv_event_flag_c::M_077);
    /* Main Event - when OFF, wolf carries sword and shield on back */
    dComIfGs_onEventBit(dSv_event_flag_c::M_068);
    /* dSv_event_flag_c::F_0550 - Main Event - Gain ability to use sense */
    dComIfGs_onEventBit((u16)dSv_event_flag_c::saveBitLabels[550]);
    return cPhs_COMPLEATE_e;
}

int dScnMenu_Create(scene_class* i_this) {
    typedef int (*method)(void*);
    static method l_method[] = {
        (method)phase_1,
        (method)phase_2,
    };

    dScnMenu_c* a_this = (dScnMenu_c*)i_this;

    l_languageType = dComIfGs_getPalLanguage();
    return dComLbG_PhaseHandler(&a_this->phase, l_method, a_this);
}

void dScnMenu_setItem(int i_slotNo, u8 i_itemNo) {
    dComIfGs_setItem(i_slotNo, i_itemNo);

    if (i_itemNo != fpcNm_ITEM_NONE) {
        dComIfGs_onItemFirstBit(i_itemNo);
    }
}

void dScnMenu_setPlayerDebugMode() {
    if (mDoCPd_c::isConnect(PAD_3) || !dScnMenu_setPlayerDebugModeDungeon()) {
        if (strcmp(dComIfGp_getNextStageName(), "D_MN05") == 0) {
            dScnMenu_setItem(SLOT_11, fpcNm_ITEM_EMPTY_BOTTLE);
            dScnMenu_setItem(SLOT_12, fpcNm_ITEM_RED_BOTTLE);
            dComIfGs_setSelectItemIndex(SELECT_ITEM_X, SLOT_11);
            dComIfGs_setSelectItemIndex(SELECT_ITEM_Y, SLOT_12);
            dComIfGs_setRupee(0);
            dComIfGs_setMaxLife(25);
            dComIfGs_setLife(20);
        } else if (strcmp(dComIfGp_getNextStageName(), "D_MN54") == 0 ||
                strcmp(dComIfGp_getNextStageName(), "D_MN04A") == 0)
        {
            for (int i = 0x17; i >= 0; i--) {
                dScnMenu_setItem(i, fpcNm_ITEM_NONE);
            }

            for (int i = 0; i < 0x100; i++) {
                dComIfGs_offItemFirstBit(i);
            }

            dComIfGs_onItemFirstBit(fpcNm_ITEM_L2_KEY_PIECES1);
            dComIfGs_onItemFirstBit(fpcNm_ITEM_L2_KEY_PIECES2);
            dComIfGs_onItemFirstBit(fpcNm_ITEM_L2_KEY_PIECES3);
            dComIfGs_onItemFirstBit(fpcNm_ITEM_LV2_BOSS_KEY);
            dComIfGs_onItemFirstBit(fpcNm_ITEM_BOW);
            dComIfGs_onItemFirstBit(fpcNm_ITEM_HOOKSHOT);
            dComIfGs_onItemFirstBit(fpcNm_ITEM_BOOMERANG);
            dComIfGs_onItemFirstBit(fpcNm_ITEM_RED_BOTTLE);
            dComIfGs_onItemFirstBit(fpcNm_ITEM_OIL_BOTTLE);
            dComIfGs_onItemFirstBit(fpcNm_ITEM_FAIRY);
            dComIfGs_onItemFirstBit(fpcNm_ITEM_KANTERA);
            dComIfGs_onItemFirstBit(fpcNm_ITEM_PACHINKO);
            dComIfGs_onItemFirstBit(fpcNm_ITEM_HVY_BOOTS);

            dScnMenu_setItem(SLOT_4, fpcNm_ITEM_BOW);
            dScnMenu_setItem(SLOT_9, fpcNm_ITEM_HOOKSHOT);
            dScnMenu_setItem(SLOT_0, fpcNm_ITEM_BOOMERANG);
            dScnMenu_setItem(SLOT_13, fpcNm_ITEM_FAIRY);
            dScnMenu_setItem(SLOT_12, fpcNm_ITEM_OIL_BOTTLE);
            dScnMenu_setItem(SLOT_11, fpcNm_ITEM_RED_BOTTLE);
            dScnMenu_setItem(SLOT_1, fpcNm_ITEM_KANTERA);
            dScnMenu_setItem(SLOT_23, fpcNm_ITEM_PACHINKO);
            dScnMenu_setItem(SLOT_3, fpcNm_ITEM_HVY_BOOTS);

            dComIfGs_setMixItemIndex(SELECT_ITEM_X, 0xFF);
            dComIfGs_setMixItemIndex(SELECT_ITEM_Y, 0xFF);
            dComIfGs_setMixItemIndex(2, 0xFF);
            dComIfGs_setMixItemIndex(3, 0xFF);
            dComIfGs_setSelectItemIndex(2, SLOT_4);
            dComIfGs_setSelectItemIndex(3, 0xFF);
            dComIfGs_setSelectItemIndex(SELECT_ITEM_Y, SLOT_0);
            dComIfGs_setSelectItemIndex(SELECT_ITEM_X, SLOT_3);

            dComIfGs_setRupee(0);
            dComIfGs_setMaxLife(25);
            dComIfGs_setLife(20);
            dComIfGs_setArrowNum(60);
            dComIfGs_setArrowMax(60);
            dComIfGs_setMaxOil(21600);
            dComIfGs_setOil(21600);
            dComIfGp_setMaxOxygen(600);
            dComIfGp_setOxygen(600);
            g_dComIfG_gameInfo.info.getSavedata().getSave(17).getBit().onDungeonItemBossKey();
            /* Main Event - Midna riding / not riding (ON == riding) */
            dComIfGs_onEventBit(dSv_event_flag_c::M_067);

            if (strcmp(dComIfGp_getNextStageName(), "D_MN54") == 0) {
                dMeter2Info_onTempBit(0);
            }
        } else if (strcmp(dComIfGp_getNextStageName(), "R_SP107") == 0) {
            dMeter2Info_setSword(fpcNm_ITEM_NONE, false);
            dMeter2Info_setShield(fpcNm_ITEM_NONE, false);
        } else if (dMeter2Info_isNextStage("F_SP102", 0, 0, 0) == 0) {
            if (strcmp(dComIfGp_getNextStageName(), "D_MN01A") == 0) {
                dScnMenu_setItem(SLOT_9, fpcNm_ITEM_HOOKSHOT);
                dScnMenu_setItem(SLOT_3, fpcNm_ITEM_HVY_BOOTS);
                dComIfGs_setSelectItemIndex(SELECT_ITEM_X, SLOT_9);
                dComIfGs_setSelectItemIndex(SELECT_ITEM_Y, SLOT_3);
                dMeter2Info_setCloth(fpcNm_ITEM_WEAR_ZORA, false);
            }
        }
    }
}

void dScnMenu_setPlayerItemReset() {
    for (int i = 23; i >= 0; i--) {
        dScnMenu_setItem(i, fpcNm_ITEM_NONE);
    }

    for (int i = 0; i < 0x100; i++) {
        dComIfGs_offItemFirstBit(i);
    }

    dComIfGs_onItemFirstBit(fpcNm_ITEM_SWORD);
    dComIfGs_onItemFirstBit(fpcNm_ITEM_MASTER_SWORD);
    dComIfGs_onItemFirstBit(fpcNm_ITEM_WOOD_STICK);
    dComIfGs_offItemFirstBit(fpcNm_ITEM_LIGHT_SWORD);
    dComIfGs_onItemFirstBit(fpcNm_ITEM_SHIELD);
    dComIfGs_onItemFirstBit(fpcNm_ITEM_HYLIA_SHIELD);
    dComIfGs_onItemFirstBit(fpcNm_ITEM_WEAR_CASUAL);
    dComIfGs_onItemFirstBit(fpcNm_ITEM_WEAR_KOKIRI);
    dComIfGs_onItemFirstBit(fpcNm_ITEM_ARMOR);
    dComIfGs_onItemFirstBit(fpcNm_ITEM_WEAR_ZORA);

    dComIfGs_setMixItemIndex(SELECT_ITEM_X, 0xFF);
    dComIfGs_setSelectItemIndex(SELECT_ITEM_X, 0xFF);
    dComIfGs_setMixItemIndex(SELECT_ITEM_Y, 0xFF);
    dComIfGs_setSelectItemIndex(SELECT_ITEM_Y, 0xFF);
    dComIfGs_setMixItemIndex(2, 0xFF);
    dComIfGs_setSelectItemIndex(2, 0xFF);
    dComIfGs_setMixItemIndex(3, 0xFF);
    dComIfGs_setSelectItemIndex(3, 0xFF);
}

bool dScnMenu_setPlayerDebugModeDungeon() {
    if (strcmp(dComIfGp_getNextStageName(), "D_MN05") == 0) {
        dScnMenu_setPlayerItemReset();
        dScnMenu_setItem(SLOT_1, fpcNm_ITEM_KANTERA);
        dScnMenu_setItem(SLOT_23, fpcNm_ITEM_PACHINKO);
        dScnMenu_setItem(SLOT_11, fpcNm_ITEM_OIL_BOTTLE);
        dScnMenu_setItem(SLOT_12, fpcNm_ITEM_RED_BOTTLE);
        dComIfGs_setSelectItemIndex(SELECT_ITEM_X, SLOT_11);
        dComIfGs_setSelectItemIndex(SELECT_ITEM_Y, SLOT_12);
        dComIfGs_setMaxLife(20);
        dComIfGs_setLife(16);
        dComIfGs_setRupee(0);
        dComIfGs_setArrowNum(0);
    } else if (strcmp(dComIfGp_getNextStageName(), "D_MN04") == 0 ||
            strcmp(dComIfGp_getNextStageName(), "D_MN04A") == 0)
    {
        dScnMenu_setPlayerItemReset();
        dScnMenu_setItem(SLOT_0, fpcNm_ITEM_BOOMERANG);
        dScnMenu_setItem(SLOT_1, fpcNm_ITEM_KANTERA);
        dScnMenu_setItem(SLOT_23, fpcNm_ITEM_PACHINKO);
        dScnMenu_setItem(SLOT_3, fpcNm_ITEM_HVY_BOOTS);
        dScnMenu_setItem(SLOT_4, fpcNm_ITEM_BOW);
        dScnMenu_setItem(SLOT_15, fpcNm_ITEM_NORMAL_BOMB);
        dScnMenu_setItem(SLOT_11, fpcNm_ITEM_OIL_BOTTLE);
        dScnMenu_setItem(SLOT_12, fpcNm_ITEM_RED_BOTTLE);
        dComIfGs_setSelectItemIndex(SELECT_ITEM_X, 0);
        dComIfGs_setSelectItemIndex(SELECT_ITEM_Y, 3);
        dComIfGs_setMaxLife(25);
        dComIfGs_setLife(20);
        dComIfGs_setRupee(0);
        dComIfGs_setArrowNum(30);
        dComIfGs_setArrowMax(30);
        dComIfGs_setBombNum(0, 10);
    } else if (strcmp(dComIfGp_getNextStageName(), "D_MN01") == 0 ||
            strcmp(dComIfGp_getNextStageName(), "D_MN01A") == 0 ||
            strcmp(dComIfGp_getNextStageName(), "D_MN01B") == 0)
    {
        dScnMenu_setPlayerItemReset();
        dScnMenu_setItem(SLOT_0, fpcNm_ITEM_BOOMERANG);
        dScnMenu_setItem(SLOT_1, fpcNm_ITEM_KANTERA);
        dScnMenu_setItem(SLOT_23, fpcNm_ITEM_PACHINKO);
        dScnMenu_setItem(SLOT_3, fpcNm_ITEM_HVY_BOOTS);
        dScnMenu_setItem(SLOT_4, fpcNm_ITEM_BOW);
        dScnMenu_setItem(SLOT_15, fpcNm_ITEM_NORMAL_BOMB);
        dScnMenu_setItem(SLOT_16, fpcNm_ITEM_WATER_BOMB);
        dScnMenu_setItem(SLOT_11, fpcNm_ITEM_OIL_BOTTLE);
        dScnMenu_setItem(SLOT_12, fpcNm_ITEM_RED_BOTTLE);
        dComIfGs_setSelectItemIndex(SELECT_ITEM_X, SLOT_16);
        dComIfGs_setSelectItemIndex(SELECT_ITEM_Y, SLOT_3);
        dComIfGs_setMaxLife(30);
        dComIfGs_setLife(24);
        dComIfGs_setRupee(0);
        dComIfGs_setArrowNum(30);
        dComIfGs_setArrowMax(30);
        dComIfGs_setBombNum(0, 10);
        dComIfGs_setBombNum(1, 15);
    } else if (strcmp(dComIfGp_getNextStageName(), "D_MN10") == 0 ||
            strcmp(dComIfGp_getNextStageName(), "D_MN10A") == 0 ||
            strcmp(dComIfGp_getNextStageName(), "D_MN10B") == 0)
    {
        dScnMenu_setPlayerItemReset();
        dScnMenu_setItem(SLOT_0, fpcNm_ITEM_BOOMERANG);
        dScnMenu_setItem(SLOT_1, fpcNm_ITEM_KANTERA);
        dScnMenu_setItem(SLOT_23, fpcNm_ITEM_PACHINKO);
        dScnMenu_setItem(SLOT_3, fpcNm_ITEM_HVY_BOOTS);
        dScnMenu_setItem(SLOT_4, fpcNm_ITEM_BOW);
        dScnMenu_setItem(SLOT_9, fpcNm_ITEM_HOOKSHOT);
        dScnMenu_setItem(SLOT_15, fpcNm_ITEM_NORMAL_BOMB);
        dScnMenu_setItem(SLOT_16, fpcNm_ITEM_WATER_BOMB);
        dScnMenu_setItem(SLOT_11, fpcNm_ITEM_GREEN_BOTTLE);
        dScnMenu_setItem(SLOT_12, fpcNm_ITEM_RED_BOTTLE);
        dComIfGs_setSelectItemIndex(SELECT_ITEM_X, SLOT_9);
        dComIfGs_setSelectItemIndex(SELECT_ITEM_Y, SLOT_4);
        dComIfGs_setMaxLife(40);
        dComIfGs_setLife(32);
        dComIfGs_setRupee(0);
        dComIfGs_setArrowNum(60);
        dComIfGs_setArrowMax(60);
        dComIfGs_setBombNum(0, 20);
        dComIfGs_setBombNum(1, 15);
    } else if (strcmp(dComIfGp_getNextStageName(), "D_MN11") == 0 ||
            strcmp(dComIfGp_getNextStageName(), "D_MN11A") == 0 ||
            strcmp(dComIfGp_getNextStageName(), "D_MN11B") == 0)
    {
        dScnMenu_setPlayerItemReset();
        dScnMenu_setItem(SLOT_0, fpcNm_ITEM_BOOMERANG);
        dScnMenu_setItem(SLOT_1, fpcNm_ITEM_KANTERA);
        dScnMenu_setItem(SLOT_23, fpcNm_ITEM_PACHINKO);
        dScnMenu_setItem(SLOT_2, fpcNm_ITEM_SPINNER);
        dScnMenu_setItem(SLOT_3, fpcNm_ITEM_HVY_BOOTS);
        dScnMenu_setItem(SLOT_4, fpcNm_ITEM_BOW);
        dScnMenu_setItem(SLOT_9, fpcNm_ITEM_HOOKSHOT);
        dScnMenu_setItem(SLOT_15, fpcNm_ITEM_NORMAL_BOMB);
        dScnMenu_setItem(SLOT_16, fpcNm_ITEM_WATER_BOMB);
        dScnMenu_setItem(SLOT_11, fpcNm_ITEM_GREEN_BOTTLE);
        dScnMenu_setItem(SLOT_12, fpcNm_ITEM_RED_BOTTLE);
        dComIfGs_setSelectItemIndex(SELECT_ITEM_X, SLOT_2);
        dComIfGs_setSelectItemIndex(SELECT_ITEM_Y, SLOT_12);
        dComIfGs_setMaxLife(45);
        dComIfGs_setLife(36);
        dComIfGs_setRupee(0);
        dComIfGs_setArrowNum(60);
        dComIfGs_setArrowMax(60);
        dComIfGs_setBombNum(0, 20);
        dComIfGs_setBombNum(1, 15);
    } else if (strcmp(dComIfGp_getNextStageName(), "D_MN06") == 0 ||
            strcmp(dComIfGp_getNextStageName(), "T_MN06") == 0)
    {
        dScnMenu_setPlayerItemReset();
        dScnMenu_setItem(SLOT_0, fpcNm_ITEM_BOOMERANG);
        dScnMenu_setItem(SLOT_1, fpcNm_ITEM_KANTERA);
        dScnMenu_setItem(SLOT_23, fpcNm_ITEM_PACHINKO);
        dScnMenu_setItem(SLOT_2, fpcNm_ITEM_SPINNER);
        dScnMenu_setItem(SLOT_3, fpcNm_ITEM_HVY_BOOTS);
        dScnMenu_setItem(SLOT_4, fpcNm_ITEM_BOW);
        dScnMenu_setItem(SLOT_9, fpcNm_ITEM_HOOKSHOT);
        dScnMenu_setItem(SLOT_6, fpcNm_ITEM_IRONBALL);
        dScnMenu_setItem(SLOT_15, fpcNm_ITEM_NORMAL_BOMB);
        dScnMenu_setItem(SLOT_16, fpcNm_ITEM_WATER_BOMB);
        dScnMenu_setItem(SLOT_11, fpcNm_ITEM_FAIRY);
        dScnMenu_setItem(SLOT_12, fpcNm_ITEM_RED_BOTTLE);
        dComIfGs_setSelectItemIndex(SELECT_ITEM_X, SLOT_6);
        dComIfGs_setSelectItemIndex(SELECT_ITEM_Y, SLOT_12);
        dComIfGs_setMaxLife(50);
        dComIfGs_setLife(40);
        dComIfGs_setRupee(0);
        dComIfGs_setArrowNum(60);
        dComIfGs_setArrowMax(60);
        dComIfGs_setBombNum(0, 20);
        dComIfGs_setBombNum(1, 15);
    } else if (strcmp(dComIfGp_getNextStageName(), "D_MN07") == 0 ||
            strcmp(dComIfGp_getNextStageName(), "T_MN07") == 0)
    {
        dScnMenu_setPlayerItemReset();
        dScnMenu_setItem(SLOT_0, fpcNm_ITEM_BOOMERANG);
        dScnMenu_setItem(SLOT_1, fpcNm_ITEM_KANTERA);
        dScnMenu_setItem(SLOT_23, fpcNm_ITEM_PACHINKO);
        dScnMenu_setItem(SLOT_2, fpcNm_ITEM_SPINNER);
        dScnMenu_setItem(SLOT_3, fpcNm_ITEM_HVY_BOOTS);
        dScnMenu_setItem(SLOT_4, fpcNm_ITEM_BOW);
        dScnMenu_setItem(SLOT_9, fpcNm_ITEM_HOOKSHOT);
        dScnMenu_setItem(SLOT_6, fpcNm_ITEM_IRONBALL);
        dScnMenu_setItem(SLOT_8, fpcNm_ITEM_COPY_ROD);
        dScnMenu_setItem(SLOT_15, fpcNm_ITEM_NORMAL_BOMB);
        dScnMenu_setItem(SLOT_16, fpcNm_ITEM_WATER_BOMB);
        dScnMenu_setItem(SLOT_11, fpcNm_ITEM_GREEN_BOTTLE);
        dScnMenu_setItem(SLOT_12, fpcNm_ITEM_RED_BOTTLE);
        dComIfGs_setSelectItemIndex(SELECT_ITEM_X, SLOT_8);
        dComIfGs_setSelectItemIndex(SELECT_ITEM_Y, SLOT_12);
        dComIfGs_setMaxLife(55);
        dComIfGs_setLife(44);
        dComIfGs_setRupee(0);
        dComIfGs_setArrowNum(60);
        dComIfGs_setArrowMax(60);
        dComIfGs_setBombNum(0, 20);
        dComIfGs_setBombNum(1, 15);
    } else if (strcmp(dComIfGp_getNextStageName(), "D_MN08") == 0) {
        dScnMenu_setPlayerItemReset();
        dScnMenu_setItem(SLOT_0, fpcNm_ITEM_BOOMERANG);
        dScnMenu_setItem(SLOT_1, fpcNm_ITEM_KANTERA);
        dScnMenu_setItem(SLOT_23, fpcNm_ITEM_PACHINKO);
        dScnMenu_setItem(SLOT_2, fpcNm_ITEM_SPINNER);
        dScnMenu_setItem(SLOT_3, fpcNm_ITEM_HVY_BOOTS);
        dScnMenu_setItem(SLOT_4, fpcNm_ITEM_BOW);
        dScnMenu_setItem(SLOT_9, fpcNm_ITEM_HOOKSHOT);
        dScnMenu_setItem(SLOT_6, fpcNm_ITEM_IRONBALL);
        dScnMenu_setItem(SLOT_8, fpcNm_ITEM_COPY_ROD);
        dScnMenu_setItem(SLOT_10, fpcNm_ITEM_W_HOOKSHOT);
        dScnMenu_setItem(SLOT_15, fpcNm_ITEM_NORMAL_BOMB);
        dScnMenu_setItem(SLOT_16, fpcNm_ITEM_WATER_BOMB);
        dScnMenu_setItem(SLOT_11, fpcNm_ITEM_BLUE_BOTTLE);
        dScnMenu_setItem(SLOT_12, fpcNm_ITEM_RED_BOTTLE);
        dComIfGs_setSelectItemIndex(SELECT_ITEM_X, SLOT_10);
        dComIfGs_setSelectItemIndex(SELECT_ITEM_Y, SLOT_11);
        dComIfGs_setMaxLife(65);
        dComIfGs_setLife(52);
        dComIfGs_setRupee(0);
        dComIfGs_setArrowNum(60);
        dComIfGs_setArrowMax(60);
        dComIfGs_setBombNum(0, 30);
        dComIfGs_setBombNum(1, 15);
    } else if (strcmp(dComIfGp_getNextStageName(), "D_MN09") == 0) {
        dScnMenu_setPlayerItemReset();
        dScnMenu_setItem(SLOT_0, fpcNm_ITEM_BOOMERANG);
        dScnMenu_setItem(SLOT_1, fpcNm_ITEM_KANTERA);
        dScnMenu_setItem(SLOT_23, fpcNm_ITEM_PACHINKO);
        dScnMenu_setItem(SLOT_2, fpcNm_ITEM_SPINNER);
        dScnMenu_setItem(SLOT_3, fpcNm_ITEM_HVY_BOOTS);
        dScnMenu_setItem(SLOT_4, fpcNm_ITEM_BOW);
        dScnMenu_setItem(SLOT_9, fpcNm_ITEM_HOOKSHOT);
        dScnMenu_setItem(SLOT_6, fpcNm_ITEM_IRONBALL);
        dScnMenu_setItem(SLOT_8, fpcNm_ITEM_COPY_ROD);
        dScnMenu_setItem(SLOT_10, fpcNm_ITEM_W_HOOKSHOT);
        dScnMenu_setItem(SLOT_15, fpcNm_ITEM_NORMAL_BOMB);
        dScnMenu_setItem(SLOT_16, fpcNm_ITEM_WATER_BOMB);
        dScnMenu_setItem(SLOT_11, fpcNm_ITEM_BLUE_BOTTLE);
        dScnMenu_setItem(SLOT_12, fpcNm_ITEM_RED_BOTTLE);
        dComIfGs_setSelectItemIndex(SELECT_ITEM_X, SLOT_4);
        dComIfGs_setSelectItemIndex(SELECT_ITEM_Y, SLOT_12);
        dComIfGs_setMaxLife(75);
        dComIfGs_setLife(60);
        dComIfGs_setRupee(0);
        dComIfGs_setArrowNum(60);
        dComIfGs_setArrowMax(60);
        dComIfGs_setBombNum(0, 30);
        dComIfGs_setBombNum(1, 15);
    }

    return 0;
}

myFontClass::~myFontClass() {}

f32 myFontClass::drawChar_scale(f32 param_0, f32 param_1, f32 param_2, f32 param_3, int param_4, bool param_5) {
    param_2;

    if (param_4 < 0xFF) {
        return field_0x70.drawChar_scale(param_0, param_1, 8.75f, param_3, param_4, param_5);
    } else {
        return JUTResFont::drawChar_scale(param_0, param_1, 13.5f, param_3, param_4, param_5);
    }
}

static leafdraw_method_class l_dScnMenu_Method = {
    (process_method_func)dScnMenu_Create,  (process_method_func)dScnMenu_Delete,
    (process_method_func)dScnMenu_Execute, (process_method_func)dScnMenu_IsDelete,
    (process_method_func)dScnMenu_Draw,
};

extern scene_process_profile_definition g_profile_MENU_SCENE = {
    fpcLy_ROOT_e,                               // mLayerID
    1,                                          // mListID
    fpcPi_CURRENT_e,                            // mListPrio
    PROC_MENU_SCENE,                            // mProcName
    &g_fpcNd_Method.base,                      // sub_method
    sizeof(dScnMenu_c),                         // mSize
    0,                                          // mSizeOther
    0,                                          // mParameters
    &g_fopScn_Method.base,                     // sub_method
    (process_method_class*)&l_dScnMenu_Method,  // mpMtd
};
#else
extern scene_process_profile_definition g_profile_MENU_SCENE = {
    fpcLy_ROOT_e,           // mLayerID
    1,                      // mListID
    fpcPi_CURRENT_e,        // mListPrio
    PROC_MENU_SCENE,        // mProcName
    &g_fpcNd_Method.base,  // sub_method
    0,                      // mSize
    0,                      // mSizeOther      
    0,                      // mParameters      
    &g_fopScn_Method.base, // sub_method
    NULL,                   // mpMtd
};
#endif
