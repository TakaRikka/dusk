#include <dolphin/dolphin.h>
#include <dolphin/gx.h>
#include <dolphin/demo.h>


static u32 __DEMOWIN_PAD_repeat_threshold = 0x0000000F;
static u32 __DEMOWIN_PAD_repeat_rate = 0x00000002;

DEMOWindow* __first_node;
DEMOWindow* __last_node;
DEMOWindow* __curr_node;
GXRenderModeObj* __rmp;

s32 fontShift = 0;

// functions
static void __DEMOWin_add_node(DEMOWindow* handle);
static void __DEMOWin_delete_node(DEMOWindow* handle);
static void __DEMOWin_puts_n(s16 x, s16 y, s16 z, u16 n, char* string);
static void __DEMOWinMenu_refesh_menu(DEMOWindow* w);
static u16 __DEMOWinMenu_get_user_input(DEMOWinPadInfo* p);
static void __DEMOWinList_refresh_list(DEMOWindow* w);

void DEMOWinInit() {
    __first_node = NULL;
    __last_node = NULL;
    __curr_node = NULL;
    __rmp = DEMOGetRenderModeObj();
    GXSetCopyClear((GXColor){0, 0, 0, 0}, 0xFFFFFF);
}

DEMOWindow* DEMOWinCreateWindow(s32 x1, s32 y1, s32 x2, s32 y2, char* caption, u16 scroll, void* func) {
    DEMOWindow* handle;
    ASSERTMSGLINE(188, x1 < x2, "DEMOWIN: Illegal X coords for window\n");
    ASSERTMSGLINE(189, y1 < y2, "DEMOWIN: Illegal y coords for window\n");

    handle = (void*)OSAlloc(sizeof(DEMOWindow));
    ASSERTMSGLINE(193, handle, "DEMOWIN: FAILED TO ALLOCATE WINDOW!\n");

    handle->x1 = x1;
    handle->y1 = y1;
    handle->x2 = x2;
    handle->y2 = y2;
    handle->pixel_width = (x2 - x1) + 1;
    handle->pixel_height = (y2 - y1) + 1;
    handle->caption = caption;
    handle->char_width = (handle->pixel_width / 8) - 1;
    handle->char_height = (handle->pixel_height / 8) - 2;
    handle->x_cal = (((handle->pixel_width - (handle->char_width * 8)) + 1) / 2);
    handle->y_cal = (((handle->pixel_height - 7) - (handle->char_height * 8)) / 2);
    handle->num_scroll_lines = scroll;
    handle->total_lines = handle->char_height + handle->num_scroll_lines;
    handle->curr_output_line = 0;
    handle->curr_output_col = 0;
    handle->curr_view_line = 0;
    handle->refresh = func;
    handle->flags = 0;
    handle->priority = 0;

    handle->buffer = (void*)OSAlloc(handle->total_lines * handle->char_width);
    ASSERTMSGLINE(249, handle->buffer, "DEMOWinCreateWindow(): Unable to allocation buffer!\n");
    memset(handle->buffer, ' ', handle->total_lines * handle->char_width); // set to all empty spaces
    
    DEMOWinSetWindowColor(handle, DEMOWIN_ITEM_DEFAULT, 0, 0, 0, 0);
    handle->cursor_line = -1;
    handle->parent = 0;
    __DEMOWin_add_node(handle);
    return handle;
}

void DEMOWinDestroyWindow(DEMOWindow* handle) {
    BOOL old;
    ASSERTMSGLINE(287, handle, "DEMOWinDestroyWindow(): NULL handle!\n");
    old = OSDisableInterrupts();
    __DEMOWin_delete_node(handle);
    OSFree(handle->buffer);
    OSFree(handle);
    OSRestoreInterrupts(old);
}

void DEMOWinOpenWindow(DEMOWindow* handle) {
    ASSERTMSGLINE(321, handle, "DEMOWinOpenWindow(): NULL handle!\n");
    handle->flags |= DEMOWIN_FLAGS_OPENED;
}

void DEMOWinCloseWindow(DEMOWindow* handle) {
    ASSERTMSGLINE(337, handle, "DEMOWinCloseWindow(): NULL handle!\n");
    handle->flags &= ~(DEMOWIN_FLAGS_OPENED);
}

void DEMOWinSetWindowColor(DEMOWindow* handle, u32 item, u8 r, u8 g, u8 b, u8 a) {
    ASSERTMSGLINE(355, handle, "DEMOWinSetWinColor(): NULL window handle\n");

    switch(item) {
    case DEMOWIN_ITEM_CAP: // set cap
        handle->cap.r = r;
        handle->cap.g = g;
        handle->cap.b = b;
        handle->cap.a = a;
        return;
    case DEMOWIN_ITEM_BORDER: // set border
        handle->border.r = r;
        handle->border.g = g;
        handle->border.b = b;
        handle->border.a = a;
        return;
    case DEMOWIN_ITEM_BKGND: // set background
        handle->bkgnd.r = r;
        handle->bkgnd.g = g;
        handle->bkgnd.b = b;
        handle->bkgnd.a = a;
        return;
    case DEMOWIN_ITEM_DEFAULT: // default window colors
        // RGB 26, 31, 33; Cinder (grey)
        handle->bkgnd.r = 26;
        handle->bkgnd.g = 31;
        handle->bkgnd.b = 33;
        handle->bkgnd.a = 255;
        // RGB 85, 31, 31; Burnt Crimson (red)
        handle->cap.r = 85;
        handle->cap.g = 31;
        handle->cap.b = 31;
        handle->cap.a = 255;
        // RGB 69, 37, 37; Bulgarian Rose (red)
        handle->border.r = 69;
        handle->border.g = 37;
        handle->border.b = 37;
        handle->border.a = 255;
        return;
    default:
        ASSERTMSGLINE(398, FALSE, "DEMOWinSetWinColor(): Unknown item\n");
        return;
    }
}

void DEMOWinLogPrintf(DEMOWindow* handle, char* fmt, ...) {
    va_list vlist;
    char buffer[128];
    u16 len;
    u16 i;
    BOOL old;
    u16 index;

    va_start(vlist, fmt);
    vsprintf(buffer, fmt, vlist);
    
    old = OSDisableInterrupts();
    len = strlen(buffer);
    for (i = 0; i < len; i++) {
        if(buffer[i] == 0xA) {
            handle->curr_output_line = (handle->curr_output_line + 1) % handle->total_lines;
            handle->curr_view_line = (handle->curr_view_line + 1) % handle->total_lines;
            handle->curr_output_col = 0;
            index = handle->curr_output_col + (handle->curr_output_line* handle->char_width);
            memset(&handle->buffer[index], ' ', handle->char_width);
        } else {
            index = handle->curr_output_col + (handle->curr_output_line* handle->char_width);
            handle->buffer[index] = buffer[i];
            handle->curr_output_col++;
        }

        if (handle->curr_output_col >= handle->char_width) {
            handle->curr_output_col = 0;
            handle->curr_output_line = (handle->curr_output_line + 1) % handle->total_lines;
            handle->curr_view_line = (handle->curr_view_line + 1) % handle->total_lines;
            index = handle->curr_output_col + (handle->curr_output_line* handle->char_width);
            memset(&handle->buffer[index], ' ', handle->char_width);
        }
    }

    OSRestoreInterrupts(old);
    va_end(vlist);
}

void DEMOWinPrintfXY(DEMOWindow* handle, u16 col, u16 row, char* fmt, ...) {
    BOOL old;
    va_list vlist;
    char string[128];
    u16 buffer_row;
    u16 i;
    u16 index;

    if (row >= handle->char_height || col >= handle->char_width) {
        return;
    }

    old = OSDisableInterrupts();
    va_start(vlist, fmt);
    vsprintf(string, fmt, vlist);
    buffer_row = ((handle->curr_view_line + handle->total_lines) - (handle->char_height - 1)) % handle->total_lines; 
    buffer_row = (((buffer_row) + row) % handle->total_lines);
    string[handle->char_width - col] = 0;
    index = (col + buffer_row* handle->char_width);

    for (i = 0; i < strlen(string); i++) {
        handle->buffer[index + i] = string[i];
    }

    OSRestoreInterrupts(old);
}

void DEMOWinScrollWindow(DEMOWindow* handle, u32 dir) {
    BOOL old;
    u16 n;
    u16 v_start;

    ASSERTMSGLINE(550, handle, "DEMOWinScrollWindow(): NULL handle!\n");
    ASSERTMSGLINE(551, handle->num_scroll_lines, "DEMOWinScrollWindow(): No scrollback buffer!\n");

    switch(dir) {
    case 1:
        old = OSDisableInterrupts();
        n = (handle->curr_view_line + handle->total_lines - 1) % handle->total_lines;
        v_start = ((n + handle->total_lines) - handle->char_height + 1) % handle->total_lines;
        if (v_start != handle->curr_output_line) {
            handle->curr_view_line = n;
        }
        OSRestoreInterrupts(old);
        return;
    case 2:
        old = OSDisableInterrupts();
        if (handle->curr_view_line != handle->curr_output_line) {
            handle->curr_view_line = (handle->curr_view_line + 1) % handle->total_lines;
        }
        OSRestoreInterrupts(old);
        return;
    case 0:
        old = OSDisableInterrupts();
        handle->curr_view_line = handle->curr_output_line;
        OSRestoreInterrupts(old);
        return;
    default:
        ASSERTMSGLINE(586, FALSE, "DEMOWinScrollWindow(): Unknown token\n");
        return;
    }
}

void DEMOWinBringToFront(DEMOWindow* handle) {
    DEMOWindow* ptr;
    ASSERTMSGLINE(609, __first_node, "DEMOWinBringToFront(): Window list is empty!\n");
    ASSERTMSGLINE(610, handle, "DEMOWinBringToFront(): NULL handle!\n");

    if (handle->priority) {
        for(ptr = __first_node; ptr; ptr = ptr->next) {
            ptr->priority = 1;
        }
        handle->priority = 0;
    }
}

void DEMOWinSendToBack(DEMOWindow* handle) {
    ASSERTMSGLINE(645, handle, "DEMOWinSendToBack(): NULL handle!\n");
    handle->priority = 1;
} 

void DEMOWinClearRow(DEMOWindow* handle, u16 row) {
    u16 buffer_row;
    u16 index;
    u16 i;
    BOOL old;

    ASSERTMSGLINE(669, handle, "DEMOWinClearRow(): NULL handle!\n");

    if (row < handle->char_height) {
        old = OSDisableInterrupts(); 
        buffer_row = (((handle->curr_view_line + handle->total_lines) - (handle->char_height - 1)) % handle->total_lines);
        buffer_row = (((buffer_row + row) % handle->total_lines));
        index = (buffer_row* handle->char_width);

        for (i = 0; i < handle->char_width; i++) {
            handle->buffer[index + i] = ' ';
        }

        OSRestoreInterrupts(old);
    }
}

void DEMOWinClearWindow(DEMOWindow* handle) {
    u16 buffer_row;
    u16 index;
    u16 i;
    BOOL old;

    ASSERTMSGLINE(718, handle, "DEMOWinClearWindow(): NULL handle!\n");

    old = OSDisableInterrupts();
    buffer_row = ((handle->curr_view_line + handle->total_lines) - (handle->char_height - 1)) % handle->total_lines;

    for (i = 0; i < handle->char_height; i++) {
        index = buffer_row* handle->char_width;
        memset(&handle->buffer[index], ' ', handle->char_width);
        buffer_row = (buffer_row + 1) % handle->total_lines;
    }

    OSRestoreInterrupts(old);
}

void DEMOWinClearBuffer(DEMOWindow* handle) {
    BOOL old;

    ASSERTMSGLINE(752, handle, "DEMOWinClearBuffer(): NULL handle!\n");
    old = OSDisableInterrupts();
    memset(handle->buffer, ' ', handle->total_lines* handle->char_width);
    OSRestoreInterrupts(old);
}

void DEMOWinRefresh(void) {
    DEMOWindow* ptr;
    u16 i;
    u16 index;
    u16 n;
    u16 y;
    BOOL old;

    ASSERTMSGLINE(792, __first_node, "DEMOWinRefresh(): Windowlist is empty!\n");

    for (ptr = __first_node; ptr; ptr = ptr->next) {
        if (ptr->flags & DEMOWIN_FLAGS_OPENED) {
            GXSetZMode(GX_ENABLE, GX_LEQUAL, GX_ENABLE);
            GXSetBlendMode(GX_BM_BLEND, GX_BL_ONE, GX_BL_ZERO, GX_LO_CLEAR);
            GXClearVtxDesc();
            GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
            GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
            GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
            GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0); 
            GXSetNumChans(1);
            GXSetChanCtrl(GX_COLOR0A0, GX_DISABLE, GX_SRC_VTX, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
            GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
            GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
            GXSetNumTexGens(0); 
            GXSetNumTevStages(1);
            GXSetLineWidth(6, GX_TO_ZERO);

            GXBegin(GX_QUADS, GX_VTXFMT0, 4);
            GXPosition3f32(ptr->x1, ptr->y1, ptr->priority);
            GXColor4u8(ptr->bkgnd.r, ptr->bkgnd.g, ptr->bkgnd.b, ptr->bkgnd.a);
            GXPosition3f32(ptr->x2, ptr->y1, ptr->priority);
            GXColor4u8(ptr->bkgnd.r, ptr->bkgnd.g, ptr->bkgnd.b, ptr->bkgnd.a);
            GXPosition3f32(ptr->x2, ptr->y2, ptr->priority);
            GXColor4u8(ptr->bkgnd.r, ptr->bkgnd.g, ptr->bkgnd.b, ptr->bkgnd.a);
            GXPosition3f32(ptr->x1, ptr->y2, ptr->priority);
            GXColor4u8(ptr->bkgnd.r, ptr->bkgnd.g, ptr->bkgnd.b, ptr->bkgnd.a);
            GXEnd();

            GXSetBlendMode(GX_BM_BLEND, GX_BL_ONE, GX_BL_ONE, GX_LO_CLEAR);

            GXBegin(GX_QUADS, GX_VTXFMT0, 4);
            GXPosition3f32(ptr->x1, ptr->y1, ptr->priority);
            GXColor4u8(ptr->cap.r, ptr->cap.g, ptr->cap.b, 255);
            GXPosition3f32(ptr->x2, ptr->y1, ptr->priority);
            GXColor4u8(0, 0, 0, 0x40);
            GXPosition3f32(ptr->x2, ptr->y1 + 10, ptr->priority);
            GXColor4u8(0, 0, 0, 0x40);
            GXPosition3f32(ptr->x1, ptr->y1 + 10, ptr->priority);
            GXColor4u8(ptr->cap.r, ptr->cap.g, ptr->cap.b, 255);
            GXEnd();

            GXSetBlendMode(GX_BM_BLEND, GX_BL_ONE, GX_BL_ZERO, GX_LO_CLEAR);

            // macro?
            do {
                // would initialize on init, but DWARF order for 2nd macro suggests they didnt init on same declare.
                u8 r1;
                u8 g1;
                u8 b1;
                u8 r2;
                u8 g2;
                u8 b2;
                u8 a;
            
                r1 = 1.3 * (f32)ptr->border.r;
                g1 = 1.3 * (f32)ptr->border.g;
                b1 = 1.3 * (f32)ptr->border.b;
                r2 = 0.4 * (f32)ptr->border.r;
                g2 = 0.4 * (f32)ptr->border.g;
                b2 = 0.4 * (f32)ptr->border.b;
                a = 64;

                GXSetLineWidth(6, GX_TO_ZERO);

                GXBegin(GX_LINESTRIP, GX_VTXFMT0, 7);
                GXPosition3f32(ptr->x1, ptr->y1, ptr->priority);
                GXColor4u8(r1, g1, b1, a);
                GXPosition3f32(ptr->x2, ptr->y1, ptr->priority);
                GXColor4u8(r1, g1, b1, a);
                GXPosition3f32(ptr->x2, ptr->y1, ptr->priority);
                GXColor4u8(r2, g2, b2, a);
                GXPosition3f32(ptr->x2, ptr->y2, ptr->priority);
                GXColor4u8(r2, g2, b2, a);
                GXPosition3f32(ptr->x1, ptr->y2, ptr->priority);
                GXColor4u8(r2, g2, b2, a);
                GXPosition3f32(ptr->x1, ptr->y2, ptr->priority);
                GXColor4u8(r1, g1, b1, a);
                GXPosition3f32(ptr->x1, ptr->y1, ptr->priority);
                GXColor4u8(r1, g1, b1, a);
                GXEnd();
            } while(0);

            if (ptr->refresh) {
                ptr->refresh(ptr);
            }

            DEMOInitCaption(DM_FT_XLU, __rmp->fbWidth, __rmp->efbHeight);
            GXSetZMode(GX_ENABLE, GX_LEQUAL, GX_ENABLE);

            old = OSDisableInterrupts();
            y = (ptr->y2 - 8) - ptr->y_cal;
            n = ptr->curr_view_line;
            index = n* ptr->char_width;

            for (i = 0; i < ptr->char_height; i++) {
                __DEMOWin_puts_n(ptr->x1 + ptr->x_cal, y, ptr->priority, ptr->char_width, (void*)&ptr->buffer[index]);
                y = y - 8;
                n = (n + (ptr->total_lines) - 1) % ptr->total_lines;
                index = n* ptr->char_width;
            }

            DEMOPrintf(ptr->x1 + 2, ptr->y1, ptr->priority, "%s", ptr->caption);

            if (ptr->cursor_line >= 0) {
                GXSetLineWidth(6, GX_TO_ZERO);
                GXSetZMode(GX_ENABLE, GX_LEQUAL, GX_ENABLE);
                GXSetBlendMode(GX_BM_BLEND, GX_BL_ONE, GX_BL_ONE, GX_LO_CLEAR);
                GXClearVtxDesc();
                GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
                GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
                GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
                GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0); 
                GXSetNumChans(1);
                GXSetChanCtrl(GX_COLOR0A0, GX_DISABLE, GX_SRC_VTX, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
                GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
                GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
                GXSetNumTexGens(0); 
                GXSetNumTevStages(1);
                GXSetLineWidth(6, GX_TO_ZERO);
                
                // macro?
                do {
                    u8 r;
                    u8 g;
                    u8 b;
                    u8 a;
                    s32 curr_y;
                
                    curr_y = (ptr->y2 - 8) - ptr->y_cal;
                    curr_y -= ((ptr->char_height - 1) * 8) - ptr->cursor_line * 8;
                    r = 1.9 * (f32)ptr->bkgnd.r;
                    g = 1.9 * (f32)ptr->bkgnd.g;
                    b = 1.9 * (f32)ptr->bkgnd.b;

                    a = 100;
                    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
                    GXPosition3f32(ptr->x1, curr_y, ptr->priority);
                    GXColor4u8(r, g, b, a);
                    GXPosition3f32(ptr->x2, curr_y, ptr->priority);
                    GXColor4u8(r, g, b, a);
                    GXPosition3f32(ptr->x2, (f32)(curr_y + 8), ptr->priority);
                    GXColor4u8(r, g, b, a);
                    GXPosition3f32(ptr->x1, (f32)(curr_y + 8), ptr->priority);
                    GXColor4u8(r, g, b, a);
                    GXEnd();
                } while(0);
            }
            OSRestoreInterrupts(old);
        }
    }
}

static void __DEMOWin_add_node(DEMOWindow* handle) {
    ASSERTMSGLINE(1032, handle, "__add_node(): you're adding a NULL node!\n");

    // WHY. why it backwards. who writes like this?
    if (NULL == __last_node) {
        __curr_node = handle;
        __last_node = handle;
        __first_node = handle;
        handle->next = 0;
        handle->prev = 0;
        ASSERTMSGLINE(1042, __first_node, "  > __first_node: NULL HANDLE!\n");
    } else {
        __last_node->next = handle;
        handle->next = 0;
        handle->prev = __last_node;
        __last_node = handle;
    }

    handle->flags |= DEMOWIN_FLAGS_INIT;
}

static void __DEMOWin_delete_node(DEMOWindow* handle) {
    ASSERTMSGLINE(1071, handle, "__delete_node(): you're deleting a NULL node!\n");

    if (__first_node == handle) {
        if (handle->next) {
            __first_node = handle->next;
            handle->next->prev = NULL;
        } else {
            __first_node = __last_node = NULL;
        }
    } else if (__last_node == handle) {
        if (handle->prev) {
            __last_node = handle->prev;
            handle->prev->next = NULL;
        } else {
            __first_node = __last_node = NULL;
        }
    } else {
        handle->prev->next = handle->next;
        handle->next->prev = handle->prev;
    }

    handle->flags &= ~(DEMOWIN_FLAGS_INIT);
}

static void __DEMOWin_puts_n(s16 x, s16 y, s16 z, u16 n, char* string) {
    s32 s;
    s32 t;
    s32 w;
    s32 len;
    s32 i;

    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_S16, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_S16, 1);

    len = n;
    if (len > 0) {
        GXBegin(GX_QUADS, GX_VTXFMT0, len * 4);
        for (i = 0; i < len; i++) {
            w = string[i] - 0x20;
            s = fontShift + (((w) % 8) * 16);
            t = fontShift + (((w) / 8) * 16);
            GXPosition3s16(x + (i * 8), y, z);
            GXTexCoord2s16(s, t);
            GXPosition3s16(x + (i * 8) + 8, y, z);
            GXTexCoord2s16(s + 16, t);
            GXPosition3s16(x + (i * 8) + 8, y + 8, z); 
            GXTexCoord2s16(s + 16, t + 16);
            GXPosition3s16(x + (i * 8), y + 8, z);
            GXTexCoord2s16(s, t + 16);
        }
        GXEnd();
    }
}

DEMOWinMenu* DEMOWinCreateMenuWindow(DEMOWinMenu* menu, u16 x, u16 y) {
    DEMOWinMenuItem* ptr;

    ptr = menu->items;
    menu->max_str_len = strlen(menu->title);
    menu->num_items = 0;

    while(!(ptr->flags & 0x80000000)) {
        if (strlen(ptr->name) > menu->max_str_len) {
            menu->max_str_len = strlen(ptr->name);
        }
        menu->num_items++;
        ptr++;
    }

    if (menu->num_items > menu->max_display_items) {
        menu->num_display_items = menu->max_display_items;
    } else {
        menu->num_display_items = menu->num_items;
    }

    menu->handle = DEMOWinCreateWindow((s16)x, (s16)y, (s16)(((menu->max_str_len + 7) * 8) + 4 + x), (s16)(((menu->num_display_items + 2) * 8) + 4 + y), menu->title, 0, __DEMOWinMenu_refesh_menu);
    menu->handle->parent = menu;
    if (menu->num_items) {
        return menu;
    }

    return NULL;
}

void DEMOWinDestroyMenuWindow(DEMOWinMenu* menu) {
    if (menu->handle) {
        DEMOWinCloseWindow(menu->handle);
        DEMOWinDestroyWindow(menu->handle);
        menu->handle = NULL;
    }
}

u32 DEMOWinMenuChild(DEMOWinMenu* menu, int child_flag) {
    DEMOWinPadInfo* pad;
    DEMOWindow* handle;
    u16 user_input;
    int exit_flag;
    u32 result;

    exit_flag = 0;
    result = 0;
    handle = menu->handle;
    pad = &menu->handle->pad;

    DEMOWinOpenWindow(handle);
    DEMOWinBringToFront(handle);
    menu->curr_pos = 0;
    menu->display_pos = 0;

    if(menu->cb_open) {
       menu->cb_open(menu, menu->curr_pos);
    }

    DEMOWinPadInit(pad);
    DEMOBeforeRender();
    DEMOWinRefresh();
    DEMODoneRender();
    DEMOWinPadRead(pad);
    DEMOBeforeRender();
    DEMOWinRefresh();
    DEMODoneRender();

    while (!exit_flag) {
        user_input = __DEMOWinMenu_get_user_input(pad);
        switch(user_input) {
        case 1:
            menu->curr_pos = ((menu->curr_pos - 1 + menu->num_items) % menu->num_items) & 0xFFFF;
            while (menu->items[menu->curr_pos].flags & 9) {
                menu->curr_pos = ((menu->curr_pos - 1 + menu->num_items) % menu->num_items) & 0xFFFF;
            }
            if (menu->cb_move) {
                menu->cb_move(menu, menu->curr_pos);
            }
            break;
        case 2:
            menu->curr_pos = ((menu->curr_pos + 1) % menu->num_items) & 0xFFFF;
            while (menu->items[menu->curr_pos].flags & 9) {
                menu->curr_pos = ((menu->curr_pos + 1) % menu->num_items) & 0xFFFF;
            }
            if (menu->cb_move) {
                menu->cb_move(menu, menu->curr_pos);
            }
            break;
        case 3:
            if (child_flag == 1) {
                exit_flag = 1;
                if (menu->cb_cancel) {
                    menu->cb_cancel(menu, menu->curr_pos);
                }
            }
            break;
        case 4:
            if (menu->cb_move) {
                menu->cb_move(menu, menu->curr_pos);
            }
            if (menu->items[menu->curr_pos].link) {
                if (menu->items[menu->curr_pos].link->handle) {
                    menu->items[menu->curr_pos].link->handle->x1 = (handle->x1 + 20) & 0xFFFF;
                    menu->items[menu->curr_pos].link->handle->y1 = (handle->y1 + 20) & 0xFFFF;
                    result = DEMOWinMenuChild(menu->items[menu->curr_pos].link, 1);
                    if (menu->items[menu->curr_pos].link->flags & 1) {
                        exit_flag = 1;
                    }
                } else {
                    DEMOWinCreateMenuWindow(menu->items[menu->curr_pos].link, (handle->x1 + 20), (handle->y1 + 20));
                    result = DEMOWinMenuChild(menu->items[menu->curr_pos].link, 1);
                    if (menu->items[menu->curr_pos].link->flags & 1) {
                        exit_flag = 1;
                    }
                    DEMOWinDestroyMenuWindow(menu->items[menu->curr_pos].link);
                }
                VIWaitForRetrace();
                DEMOWinPadRead(pad);
            }
            break;
        case 5:
            if (menu->cb_select) {
                menu->cb_select(menu, menu->curr_pos);
            }
            if (menu->items[menu->curr_pos].link) {
                if (menu->items[menu->curr_pos].link->handle) {
                    menu->items[menu->curr_pos].link->handle->x1 = (handle->x1 + 20) & 0xFFFF;
                    menu->items[menu->curr_pos].link->handle->y1 = (handle->y1 + 20) & 0xFFFF;
                    result = DEMOWinMenuChild(menu->items[menu->curr_pos].link, 1);
                    if (menu->items[menu->curr_pos].link->flags & 1) {
                        exit_flag = 1;
                    }
                } else {
                    DEMOWinCreateMenuWindow(menu->items[menu->curr_pos].link, (handle->x1 + 20), (handle->y1 + 20));
                    result = DEMOWinMenuChild(menu->items[menu->curr_pos].link, 1);
                    if (menu->items[menu->curr_pos].link->flags & 1) {
                        exit_flag = 1;
                    }
                    DEMOWinDestroyMenuWindow(menu->items[menu->curr_pos].link);
                }
                VIWaitForRetrace();
                DEMOWinPadRead(pad);
            } else if (menu->items[menu->curr_pos].function) {
                menu->items[menu->curr_pos].function(menu, menu->curr_pos, &result);
                if (menu->items[menu->curr_pos].flags & 0x10) {
                    exit_flag = 1;
                }
                VIWaitForRetrace();
                DEMOWinPadRead(pad);
            }
            break;
        case 6:
            if (menu->cb_cancel) {
                menu->cb_cancel(menu, menu->curr_pos);
            }
            exit_flag = 1;
            break;
        }

        if (menu->curr_pos > (menu->display_pos + menu->num_display_items - 1)) {
            menu->display_pos = (menu->curr_pos - menu->num_display_items) + 1;
        } else if (menu->curr_pos < menu->display_pos) {
            menu->display_pos = menu->curr_pos;
        }

        if (menu->display_pos > menu->curr_pos) {
            handle->cursor_line = (menu->display_pos - menu->curr_pos);
        } else {
            handle->cursor_line = (menu->curr_pos - menu->display_pos);
        }

        DEMOBeforeRender();
        DEMOWinRefresh();
        DEMODoneRender();
    }

    DEMOWinCloseWindow(handle);
    DEMOBeforeRender();
    DEMOWinRefresh();
    DEMODoneRender();
    return result;
}

static void __DEMOWinMenu_refesh_menu(DEMOWindow* w) {
    DEMOWinMenu* m;
    s32 i;
    s32 j;
    char check;
    char para_start;
    char para_end;
    char link;

    DEMOWinClearWindow(w);
    m = w->parent;
    j = m->display_pos;

    for (i = 0; i < m->num_display_items; j++, i++) {
        if (m->items[j].flags & 8) {
            if (strlen(m->items[j & 0xFFFF].name)) {
                DEMOWinPrintfXY(w, 0, i, "   %s     ", m->items[j & 0xFFFF].name);
            }
        } else {
            check      = (s8)((m->items[j].flags & 4) ? 'X' : ' ');
            para_start = (s8)((m->items[j].flags & 1) ? '(' : ' ');
            para_end   = (s8)((m->items[j].flags & 1) ? ')' : ' ');
            link       = (s8)((NULL != m->items[j].link)      ? '>' : ' ');
            DEMOWinPrintfXY(w, 0, i, "%c %c%s%c %c", check, para_start, m->items[j & 0xFFFF].name, para_end, link);
        }
    }
}

void DEMOWinPadInit(DEMOWinPadInfo* p) {
    u16 i;

    for (i = 0; i < 4; i++) {
        p->old_button[i] = 0;
        p->changed_button[i] = 0;
        p->repeat_button[i] = 0;
        p->repeat_ctr[i] = 0;
    }
}

void DEMOWinPadRead(DEMOWinPadInfo* p) {
    PADStatus* pad;
    u16 index;
    u32 curr;
    u32 old;
    u32 repeat;

    PADRead(p->pads);

    for (index = 0; index < 4; index++) {
        old = p->old_button[index];
        pad = &p->pads[index];

        curr = ((pad->stickX       >   0x40 ? 0x00040000 : 0)
              | (pad->stickX       <  -0x40 ? 0x00080000 : 0)
              | (pad->stickY       >   0x40 ? 0x00010000 : 0)
              | (pad->stickY       <  -0x40 ? 0x00020000 : 0)
              | (pad->substickX    >  +0x40 ? 0x00400000 : 0)
              | (pad->substickX    <  -0x40 ? 0x00800000 : 0)
              | (pad->substickY    >  +0x40 ? 0x00100000 : 0)
              | (pad->substickY    <  -0x40 ? 0x00200000 : 0) 
              | (pad->triggerLeft  >  +0x80 ? 0x02000000 : 0)
              | (pad->triggerRight >  +0x80 ? 0x01000000 : 0) 
              | pad->button);

        p->changed_button[index] = (curr & (old ^ curr));
        if (curr) {
            if (old == curr) {
                p->repeat_ctr[index]++;
            } else {
                p->repeat_ctr[index] = 1;
            }
        } else {
            p->repeat_ctr[index] = 0;
        }

        repeat = p->repeat_ctr[index];

        if (repeat == 1) {
            p->repeat_button[index] = curr;
        } else if (repeat > __DEMOWIN_PAD_repeat_threshold) {
            if (((repeat - __DEMOWIN_PAD_repeat_threshold) % __DEMOWIN_PAD_repeat_rate) == 0) {
                p->repeat_button[index] = curr;
            } else {
                p->repeat_button[index] = 0;
            }
        } else {
            p->repeat_button[index] = 0;
        }

        p->old_button[index] = curr;
    }
}

static u16 __DEMOWinMenu_get_user_input(DEMOWinPadInfo* p) {
    u16 user_input;

    DEMOWinPadRead(p);
    if (p->repeat_button[0] & 0x00010008) {
        user_input = 1;
    } else if (p->repeat_button[0] & 0x00020004) {
        user_input = 2;
    } else if (p->repeat_button[0] & 0x00080001) {
        user_input = 3;
    } else if (p->repeat_button[0] & 0x00040002) {
        user_input = 4;
    } else if (p->changed_button[0] & 0x00000100) {
        user_input = 5;
    } else if (p->changed_button[0] & 0x00000200) {
        user_input = 6;
    } else {
        user_input = 0;
    }

    return user_input;
}

void DEMOWinSetRepeat(u32 threshold, u32 rate) {
    __DEMOWIN_PAD_repeat_rate = rate;
    __DEMOWIN_PAD_repeat_threshold = threshold;
}

void DEMOWinResetRepeat(void) {
    __DEMOWIN_PAD_repeat_threshold = 15;
    __DEMOWIN_PAD_repeat_rate = 2;
}

DEMOWinListbox* DEMOWinCreateListWindow(DEMOWinListbox* list, u16 x, u16 y) {
    DEMOWinListboxItem* ptr;
    ASSERTMSGLINE(1846, list, "DEMOWinCreateListWindow(): List is NULL!\n");

    ptr = list->items;
    list->max_str_len = strlen(list->title);
    list->num_items = 0;

    while (!(ptr->flags & 0x80000000)) {
        if (strlen(ptr->name) > list->max_str_len) {
            list->max_str_len = strlen(ptr->name); 
        }
        list->num_items++;
        ptr++;
    }

    if (list->num_items > list->max_display_items) {
        list->num_display_items = list->max_display_items;
    } else {
        list->num_display_items = list->num_items;
    }

    list->handle = DEMOWinCreateWindow((s16)x, (s16)y, (s16)((list->max_str_len + 7) * 8 + 4 + x), (s16)((list->num_display_items + 2) * 8 + 4 + y), list->title, 0, __DEMOWinList_refresh_list);
    list->handle->parent = list;
    if (list->num_items) {
        return list;
    }
    return NULL;
}

void DEMOWinDestroyListWindow(DEMOWinListbox* list) {
    if (list->handle) {
        DEMOWinCloseWindow(list->handle);
        DEMOWinDestroyWindow(list->handle);
        list->handle = NULL;
    }
}

static void __DEMOWinList_refresh_list(DEMOWindow* w) {
    DEMOWinListbox* l;
    s32 i;
    s32 j;

    l = w->parent;
    l->curr_pos = (l->curr_pos % l->num_items);
    if (l->curr_pos > (l->display_pos + l->num_display_items - 1)) {
        l->display_pos = (l->curr_pos - l->num_display_items + 1);
    } else if(l->curr_pos < l->display_pos) {
        l->display_pos = l->curr_pos;
    }

    if (l->cursor_state != 0) {
        if(l->display_pos > l->curr_pos) {
            w->cursor_line = (l->display_pos - l->curr_pos);
        } else {
            w->cursor_line = (l->curr_pos - l->display_pos);
        }
    } else {
        w->cursor_line = -1;
    }

    DEMOWinClearWindow(w);

    j = l->display_pos;
    for (i = 0; i < l->num_display_items; i++) {
        if (!(l->items[j].flags & 0x8)) {
            DEMOWinPrintfXY(w, 0, i, " %s ", l->items[j & 0xFFFF].name);
        }
        j++;
    }
}

void DEMOWinListSetCursor(DEMOWinListbox* list, int x) {
    list->cursor_state = x;
}

s32 DEMOWinListScrollList(DEMOWinListbox* list, u32 dir) {
    ASSERTMSGLINE(2032, list, "DEMOWinListScrollList(): NULL handle!\n");

    switch(dir) {
    case 1:
        if (list->display_pos) {
            list->display_pos = (u16)((list->display_pos - 1 + list->num_items) % list->num_items);
        }
        break;
    case 2:
        if (list->display_pos < (list->num_items - list->num_display_items)) {
            list->display_pos = (u16)((list->display_pos + 1) % list->num_items);
        }
        break;
    case 0:
        list->display_pos = 0;
        break;
    default:
        ASSERTMSGLINE(2057, FALSE, "DEMOWinListScrollList(): Invalid dimension!\n");
        break;
    }

    if (list->curr_pos > (list->display_pos + list->num_display_items - 1)) {
        list->curr_pos = (list->display_pos + list->num_display_items - 1);
    } else if (list->curr_pos < list->display_pos) {
        list->curr_pos = list->display_pos;
    }

    return list->display_pos;
}

s32 DEMOWinListMoveCursor(DEMOWinListbox* list, u32 dir) {
    ASSERTMSGLINE(2092, list, "DEMOWinListScrollList(): NULL handle!\n");

    switch (dir) {
    case 1:
        list->curr_pos = (list->curr_pos + list->num_items - 1) % list->num_items;
        break;
    case 2:
        list->curr_pos = (list->curr_pos + 1) % list->num_items;
        break;
    default:
        ASSERTMSGLINE(2105, FALSE, "DEMOWinListMoveCursor(): Invalid dimension!\n");
        break;
    }

    return list->curr_pos;
}
