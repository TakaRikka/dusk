#ifndef D_CURSOR_MNG_H
#define D_CURSOR_MNG_H

#include "JSystem/J2DGraph/J2DScreen.h"
#include "m_Do/m_Do_graphic.h"
#include "m_Re/m_Re_controller_pad.h"

// TODO: putting this here until a more appropriate place is found
extern u8 data_8053a730;

class dCsr_mng_c {
    struct csr_c;

    struct node_c : mDoGph_gInf_c::csr_c {
        BOOL set(u8 i_priority, u8, u16 i_mask);

        /* 0x04 */ u8 m_priority;
        /* 0x05 */ u8 field_0x05;
        /* 0x06 */ u16 m_mask;
        /* 0x08 */ node_c* m_prev;
        /* 0x0C */ node_c* m_next;
        /* 0x10 */ int m_actor_id;
        /* 0x14 */ csr_c* m_csr;
        /* 0x18 */ node_c* m_pointed_obj;
    };

    struct obj_c : node_c {
    };

    struct list_c {
        list_c() : m_root(NULL) {
        }

        virtual ~list_c(void) {}
        void entry(node_c* i_node);
        node_c* release(node_c* i_node);
        node_c* release(u16 i_mask);
        BOOL isEntry(const node_c* i_node) const;

        /* 0x04 */ node_c* m_root;
    };

    struct bloObj_c {
        struct paneObj_c {
            paneObj_c(void) {
                m_handle = NULL;
                m_min_x = 0;
                m_min_y = 0;
                m_max_x = 0;
                m_max_y = 0;
            }
            virtual ~paneObj_c(void) {}

            /* 0x04 */ J2DPane* m_handle;
            /* 0x08 */ s16 m_min_x;
            /* 0x0A */ s16 m_min_y;
            /* 0x0C */ s16 m_max_x;
            /* 0x0E */ s16 m_max_y;
        };

        bloObj_c() {
            m_panes = new paneObj_c[1];
            m_screen = 0;
        }

        virtual ~bloObj_c(void);
        virtual u32 signature(void) const {
            return 0x626c6f20;
        }
        virtual BOOL isInside(s16 i_x, s16 i_y);
        BOOL create(J2DScreen* i_screen, u16 i_mask, u8 i_priority, u8);
        void calc(void);
        void calcPaneObjNum(J2DPane* i_pane);
        void createPaneObj(paneObj_c** i_panes, J2DPane* i_pane);

        /* 0x04 */ u8 field_0x04[0x10 - 0x4];
        /* 0x10 */ J2DScreen* m_screen;
        /* 0x14 */ paneObj_c* m_panes;
        /* 0x18 */ u16 m_pane_num;
        /* 0x1A */ u8 m_is_calc;
        /* 0x1C */ J2DPane* m_pane_handle;
    };

    struct ccObj_c : node_c {
        virtual ~ccObj_c(void);
        virtual int signature(void) const {
            return 'cc  ';
        }
        virtual BOOL isInside(s16, s16);
        void set(u16, u8, u8);
    };

    struct csr_c : node_c {
        virtual ~csr_c(void);
        BOOL set(mDoGph_gInf_c::csr_c* i_csr, u16, u8, u8);
    };

private:
    dCsr_mng_c() : is_csr_on(true) {
    }

    void update_(void);
    void releaseCsr_(csr_c* i_csr);
    void insideObjReleaseCheck_(void);

public:
    static void create(void);
    static void update(void);
    static void entryCsr(csr_c* i_csr);
    static void releaseCsr(csr_c* i_csr);
    static void entryObj(obj_c* i_obj);
    static void releaseObj(obj_c* i_obj);
    static void releaseObj(u16 i_mask);
    static void onCsr(void);
    static void offCsr(void);

    static dCsr_mng_c* m_myObj;

private:
    /* 0x00 */ list_c m_csr_list;
    /* 0x08 */ list_c m_obj_list;
    /* 0x10 */ bool is_csr_on;
};

#endif /* D_CURSOR_MNG_H */
