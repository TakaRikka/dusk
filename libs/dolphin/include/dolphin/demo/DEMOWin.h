#ifndef _DOLPHIN_DEMOWIN_H_
#define _DOLPHIN_DEMOWIN_H_

#include <dolphin/gx.h>
#include <dolphin/pad.h>

#ifdef __cplusplus
extern "C" {
#endif

#define DEMOWIN_ITEM_CAP     0
#define DEMOWIN_ITEM_BKGND   1
#define DEMOWIN_ITEM_BORDER  2
#define DEMOWIN_ITEM_DEFAULT 3

// flags
#define DEMOWIN_FLAGS_INIT   (1 << 0)
#define DEMOWIN_FLAGS_OPENED (1 << 1)

typedef struct {
    /* 0x00 */ PADStatus pads[4];
    /* 0x30 */ u32 button[4];
    /* 0x40 */ u32 old_button[4];
    /* 0x50 */ u32 changed_button[4];
    /* 0x60 */ u32 repeat_button[4];
    /* 0x70 */ u32 repeat_ctr[4];
} DEMOWinPadInfo;

typedef struct STRUCT_DEMOWIN DEMOWindow;
typedef struct STRUCT_DEMOWIN {
    /* 0x00 */ s32 x1;
    /* 0x04 */ s32 y1;
    /* 0x08 */ s32 x2;
    /* 0x0C */ s32 y2;
    /* 0x10 */ u32 priority;
    /* 0x14 */ u32 flags;
    /* 0x18 */ u16 x_cal;
    /* 0x1A */ u16 y_cal;
    /* 0x1C */ u16 pixel_width;
    /* 0x1E */ u16 pixel_height;
    /* 0x20 */ u16 char_width;
    /* 0x22 */ u16 char_height;
    /* 0x24 */ u16 num_scroll_lines;
    /* 0x26 */ u16 total_lines;
    /* 0x28 */ u16 curr_output_line;
    /* 0x2A */ u16 curr_output_col;
    /* 0x2C */ u16 curr_view_line;
    /* 0x2E */ s16 cursor_line;
    /* 0x30 */ char* caption;
    /* 0x34 */ u8* buffer;
    /* 0x38 */ GXColor bkgnd;
    /* 0x3C */ GXColor cap;
    /* 0x40 */ GXColor border;
    /* 0x44 */ void (*refresh)(DEMOWindow *);
    /* 0x48 */ DEMOWindow* next;
    /* 0x4C */ DEMOWindow* prev;
    /* 0x50 */ void* parent;
    /* 0x54 */ DEMOWinPadInfo pad;
} DEMOWindow;

typedef struct STRUCT_MENU DEMOWinMenu;
typedef struct STRUCT_MENU_ITEM {
    /* 0x00 */ char* name;
    /* 0x04 */ u32 flags;
    /* 0x08 */ void (*function)(DEMOWinMenu*, u32, u32*);
    /* 0x0C */ DEMOWinMenu* link;
} DEMOWinMenuItem;

typedef struct STRUCT_MENU {
    /* 0x00 */ char* title;
    /* 0x04 */ DEMOWindow* handle;
    /* 0x08 */ DEMOWinMenuItem* items;
    /* 0x0C */ s32 max_display_items;
    /* 0x10 */ u32 flags;
    /* 0x14 */ void (*cb_open)(DEMOWinMenu*, u32);
    /* 0x18 */ void (*cb_move)(DEMOWinMenu*, u32);
    /* 0x1C */ void (*cb_select)(DEMOWinMenu*, u32);
    /* 0x20 */ void (*cb_cancel)(DEMOWinMenu*, u32);
    /* 0x24 */ s32 num_display_items;
    /* 0x28 */ s32 num_items;
    /* 0x2C */ u32 max_str_len;
    /* 0x30 */ s32 curr_pos;
    /* 0x34 */ s32 display_pos;
} DEMOWinMenu;

typedef struct STRUCT_LISTBOX_ITEM {
    /* 0x00 */ char* name; // offset 0x0, size 0x4
    /* 0x04 */ u32 flags; // offset 0x4, size 0x4
} DEMOWinListboxItem;

typedef struct STRUCT_LISTBOX {
    /* 0x00 */ char* title; // offset 0x0, size 0x4
    /* 0x04 */ DEMOWindow* handle; // offset 0x4, size 0x4
    /* 0x08 */ DEMOWinListboxItem* items; // offset 0x8, size 0x4
    /* 0x0C */ s32 max_display_items; // offset 0xC, size 0x4
    /* 0x10 */ u32 flags; // offset 0x10, size 0x4
    /* 0x14 */ s32 num_display_items; // offset 0x14, size 0x4
    /* 0x18 */ s32 num_items; // offset 0x18, size 0x4
    /* 0x1C */ u32 max_str_len; // offset 0x1C, size 0x4
    /* 0x20 */ s32 curr_pos; // offset 0x20, size 0x4
    /* 0x24 */ s32 display_pos; // offset 0x24, size 0x4
    /* 0x28 */ int cursor_state; // offset 0x28, size 0x4
} DEMOWinListbox;

void DEMOWinInit(void);
DEMOWindow* DEMOWinCreateWindow(s32 x1, s32 y1, s32 x2, s32 y2, char* caption, u16 scroll, void* func);
void DEMOWinDestroyWindow(DEMOWindow* handle);
void DEMOWinOpenWindow(DEMOWindow* handle);
void DEMOWinCloseWindow(DEMOWindow* handle);
void DEMOWinSetWindowColor(DEMOWindow* handle, u32 item, u8 r, u8 g, u8 b, u8 a);
void DEMOWinLogPrintf(DEMOWindow* handle, char* fmt, ...);
void DEMOWinPrintfXY(DEMOWindow* handle, u16 col, u16 row, char* fmt, ...);
void DEMOWinScrollWindow(DEMOWindow* handle, u32 dir);
void DEMOWinBringToFront(DEMOWindow* handle);
void DEMOWinSendToBack(DEMOWindow* handle);
void DEMOWinClearRow(DEMOWindow* handle, u16 row);
void DEMOWinClearWindow(DEMOWindow* handle);
void DEMOWinClearBuffer(DEMOWindow* handle);
void DEMOWinRefresh(void);
DEMOWinMenu* DEMOWinCreateMenuWindow(DEMOWinMenu* menu, u16 x, u16 y);
void DEMOWinDestroyMenuWindow(DEMOWinMenu* menu);
u32 DEMOWinMenuChild(DEMOWinMenu* menu, int child_flag);
void DEMOWinPadInit(DEMOWinPadInfo* p);
void DEMOWinPadRead(DEMOWinPadInfo* p);
void DEMOWinSetRepeat(u32 threshold, u32 rate);
void DEMOWinResetRepeat(void);
DEMOWinListbox* DEMOWinCreateListWindow(DEMOWinListbox* list, u16 x, u16 y);
void DEMOWinDestroyListWindow(DEMOWinListbox* list);
void DEMOWinListSetCursor(DEMOWinListbox* list, int x);
s32 DEMOWinListScrollList(DEMOWinListbox* list, u32 dir);
s32 DEMOWinListMoveCursor(DEMOWinListbox* list, u32 dir);

#ifdef __cplusplus
}
#endif

#endif // _DOLPHIN_DEMOWIN_H_
