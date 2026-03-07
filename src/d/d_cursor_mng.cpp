#include "d/dolzel.h" // IWYU pragma: keep

#include "d/d_cursor_mng.h"

#include "d/d_com_inf_game.h"

dCsr_mng_c* dCsr_mng_c::m_myObj;

// TODO: putting this here until a more appropriate place is found
u8 data_8053a730;

void dCsr_mng_c::update_(void) {
    BOOL is_valid = FALSE;

    if (dComIfGs_getOptPointer()) {
        mReCPd::enableDpd(0);
        is_valid = mReCPd::chkDpdValid(NULL);
    } else {
        mReCPd::disableDpd(0);
    }

    if (is_csr_on == 0) {
        mDoGph_gInf_c::entryCsr(NULL);
        node_c* cur_node = m_csr_list.m_root;
        while (cur_node != NULL) {
            cur_node->m_pointed_obj = 0;
            cur_node = cur_node->m_next;
        }

        return;
    }

    mDoGph_gInf_c::releaseCsr();
    Vec2* pos = &mReCPd::getDpd2DPos(0);
    int x = pos->x;
    int y = pos->y;

    node_c* cur_node = m_csr_list.m_root;
    mDoGph_gInf_c::csr_c* csr = NULL;
    u16 last_mask = 1;
    while (cur_node != NULL) {
        cur_node->m_pointed_obj = NULL;

        u16 cur_mask = cur_node->m_mask;
        if (!g_dComIfG_gameInfo.play.mItemInfo.mPauseFlag || (cur_node->m_mask & 0x200) == 0) {
            if (csr == NULL) {
                csr = cur_node->m_csr;
            }

            if (is_valid && last_mask != 0) {
                last_mask = cur_mask;
                if (last_mask != 0) {
                    node_c* cur_node_2 = m_obj_list.m_root;
                    while (cur_node_2 != NULL) {
                        if ((cur_mask & cur_node_2->m_mask) != 0 &&
                            ((bloObj_c*)cur_node_2)->isInside(x, y))
                        {
                            cur_node->m_pointed_obj = cur_node_2;
                        }

                        cur_node_2 = cur_node_2->m_next;
                    }
                }
            }
        }

        cur_node = cur_node->m_next;
    }

    if (!dComIfGs_getOptPointer()) {
        if (csr != NULL) {
            if (csr->isPointer()) {
                csr = NULL;
            }
        }
    } else if (csr == NULL) {
        return;
    }

    mDoGph_gInf_c::entryCsr(csr);
}

void dCsr_mng_c::releaseCsr_(csr_c* i_csr) {
    mDoGph_gInf_c::csr_c* temp_r5 = i_csr->m_csr;
    i_csr->m_pointed_obj = 0;
    if (temp_r5 != NULL && temp_r5 == mDoGph_gInf_c::m_csr) {
        mDoGph_gInf_c::entryCsr(NULL);
    }
    m_csr_list.release(i_csr);
}

void dCsr_mng_c::insideObjReleaseCheck_(void) {
    node_c* cur_node = m_csr_list.m_root;
    while (cur_node != NULL) {
        if (!m_obj_list.isEntry(cur_node->m_pointed_obj)) {
            cur_node->m_pointed_obj = NULL;
        }
        cur_node = cur_node->m_next;
    }
}

void dCsr_mng_c::create(void) {
    dCsr_mng_c* mng = new dCsr_mng_c();
    m_myObj = mng;
}

void dCsr_mng_c::update(void) {
    m_myObj->update_();
}

void dCsr_mng_c::entryCsr(csr_c* i_csr) {
    dCsr_mng_c* mng = m_myObj;
    mng->m_csr_list.entry(i_csr);
    mng->is_csr_on = true;
}

void dCsr_mng_c::releaseCsr(csr_c* i_csr) {
    m_myObj->releaseCsr_(i_csr);
}

void dCsr_mng_c::entryObj(obj_c* i_obj) {
    m_myObj->m_obj_list.entry(i_obj);
}

void dCsr_mng_c::releaseObj(obj_c* i_obj) {
    m_myObj->m_obj_list.release(i_obj);
    m_myObj->insideObjReleaseCheck_();
}

void dCsr_mng_c::releaseObj(u16 param_0) {
    m_myObj->m_obj_list.release(param_0);
    m_myObj->insideObjReleaseCheck_();
}

void dCsr_mng_c::onCsr(void) {
    m_myObj->is_csr_on = true;
}

void dCsr_mng_c::offCsr(void) {
    m_myObj->is_csr_on = false;
}

BOOL dCsr_mng_c::node_c::set(u8 i_priority, u8 param_1, u16 i_mask) {
    BOOL has_relatives = FALSE;
    if (m_prev != NULL || m_next != NULL) {
        has_relatives = TRUE;
    }
    if (has_relatives) {
        return FALSE;
    }

    m_priority = i_priority;
    field_0x05 = param_1;
    m_mask = i_mask;

    return TRUE;
}

void dCsr_mng_c::list_c::entry(node_c* i_node) {
    BOOL has_relatives = FALSE;
    if (i_node->m_prev != NULL || i_node->m_next != NULL) {
        has_relatives = TRUE;
    }
    if (has_relatives) {
        return;
    }

    node_c* cur_node = m_root;
    if (cur_node == NULL) {
        m_root = i_node;
        return;
    }

    while (i_node->m_priority > cur_node->m_priority) {
        node_c* next = cur_node->m_next;
        if (next == NULL) {
            cur_node->m_next = i_node;
            i_node->m_prev = cur_node;
            i_node->m_next = NULL;
            return;
        }
        cur_node = next;
    }

    node_c* prev = cur_node->m_prev;
    if (prev == NULL) {
        m_root = i_node;
    } else {
        i_node->m_prev = prev;
        prev->m_next = i_node;
    }
    i_node->m_next = cur_node;
    cur_node->m_prev = i_node;
}

dCsr_mng_c::node_c* dCsr_mng_c::list_c::release(node_c* i_node) {
    node_c *next;
    node_c *prev;

    if (!isEntry(i_node)) {
        return NULL;
    }

    prev = i_node->m_prev;
    next = i_node->m_next;
    if (prev != NULL) {
        prev->m_next = next;
        i_node->m_prev = NULL;
    } else {
        m_root = next;
    }

    if (next != NULL) {
        next->m_prev = prev;
        i_node->m_next = NULL;
    }

    return next;
}

dCsr_mng_c::node_c* dCsr_mng_c::list_c::release(u16 i_mask) {
    node_c* cur_node = m_root;
    while (cur_node != NULL) {
        if (cur_node->m_mask & i_mask) {
            cur_node = release(cur_node);
        } else {
            cur_node = cur_node->m_next;
        }
    }
}

BOOL dCsr_mng_c::list_c::isEntry(const node_c* i_node) const {
    node_c* cur_node = m_root;
    while (cur_node != NULL) {
        if (cur_node == i_node) {
            return TRUE;
        }
        cur_node = cur_node->m_next;
    }
    return FALSE;
}

dCsr_mng_c::bloObj_c::~bloObj_c() {
    if (m_panes != NULL) {
        delete[] m_panes;
        m_panes = NULL;
    }
    m_screen = NULL;
}

BOOL dCsr_mng_c::bloObj_c::isInside(s16 i_x, s16 i_y) {
    if (mReCPd::chkSwingBlock(0) == 0) {
        paneObj_c* pane = m_panes;
        for (u16 i = 0; i < m_pane_num; pane++, i++) {
            if (pane->m_min_x <= i_x && i_x <= pane->m_max_x &&
                pane->m_min_y <= i_y && i_y <= pane->m_max_y) {
                J2DPane* pane_handle = pane->m_handle;
                m_pane_handle = pane_handle;
                if (pane_handle->mAlpha != 0 && pane_handle->mVisible) {
                    return 1;
                }
            }
        }
    }

    m_pane_handle = NULL;
    return 0;
}

BOOL dCsr_mng_c::bloObj_c::create(J2DScreen* i_screen, u16 i_mask, u8 i_priority, u8 param_3) {
    if (!((node_c*)this)->set(i_priority, param_3, i_mask)) {
        return FALSE;
    }
    m_screen = i_screen;
    m_pane_num = 0;
    calcPaneObjNum(i_screen);
    m_panes = new paneObj_c[m_pane_num];
    paneObj_c* sp08 = m_panes + m_pane_num;
    createPaneObj(&sp08, m_screen);
    return TRUE;
}

void dCsr_mng_c::bloObj_c::calc(void) {
    if (m_is_calc == 0) {
        paneObj_c* pane_obj = m_panes;
        u32 var_r30 = 0;
        while ((u16)var_r30 < m_pane_num) {
            J2DPane* pane = pane_obj->m_handle;
            Vec sp20 = pane->getGlbVtx(0);
            pane_obj->m_min_x = sp20.x;
            pane_obj->m_min_y = sp20.y;
            sp20 = pane->getGlbVtx(3);
            var_r30++;
            pane_obj->m_max_x = sp20.x;
            pane_obj->m_max_y = sp20.y;
            pane_obj++;
        }
        m_is_calc = 1;
    }
}

char* nullSkip(char* buf) {
    char* cursor = buf;
    while (*cursor == 0) {
        cursor += 1;
    }
    return cursor;
}

void dCsr_mng_c::bloObj_c::calcPaneObjNum(J2DPane* i_pane) {
    u64 info_tag = i_pane->mInfoTag;
    char* info_start = nullSkip((char*)&info_tag);
    if (info_start[0] == 0x4E && info_start[1] == 0x5F) {
        m_pane_num += 1;
    }

    JSUTreeIterator<J2DPane> iter = i_pane->getPaneTree()->getFirstChild();
    while (iter != NULL) {
        calcPaneObjNum(*iter);
        ++iter;
    }
}

void dCsr_mng_c::bloObj_c::createPaneObj(paneObj_c** i_panes, J2DPane* i_pane) {
    u64 info_tag = i_pane->mInfoTag;
    char* info_start = nullSkip((char*)&info_tag);
    if (info_start[0] == 0x4E && info_start[1] == 0x5F) {
        paneObj_c* pane = *i_panes;
        *i_panes -= 1;
        pane->m_handle = i_pane;
    }

    JSUTreeIterator<J2DPane> iter = i_pane->getPaneTree()->getFirstChild();
    while (iter != NULL) {
        createPaneObj(i_panes, *iter);
        ++iter;
    }
}

BOOL dCsr_mng_c::ccObj_c::isInside(s16 param_0, s16 param_1) {
    if (dComIfGd_getView() == NULL) {
        return 0;
    }

    f32 x = (param_0 - mDoGph_gInf_c::getMinXF()) / mDoGph_gInf_c::getWidthF() * 2.0f - 1.0f;
    f32 y = (param_1 - mDoGph_gInf_c::getMinYF()) / mDoGph_gInf_c::getHeightF() * 2.0f - 1.0f;
    cXyz sp18;
    mDoLib_2Dto3D(x, y, 1000.0f, &sp18);
    cXyz sp0C;
    PSMTXMultVec(dComIfGd_getInvViewMtx(), &cXyz::Zero, &sp0C);
    dBgS_LinChk linChk;
    linChk.Set(&sp0C, &sp18, NULL);
    if (dComIfG_Bgsp().LineCross(&linChk)) {
        sp18 = linChk.GetCross();
    }

    s32 actor_id;
    fopAc_ac_c* actor_p = NULL;
    dComIfG_Ccsp()->ChkLine(sp0C, sp18, 15.0f, &actor_p);
    if (actor_p != NULL) {
        actor_id = actor_p->base.base.id;
    } else {
        actor_id = fpcM_ERROR_PROCESS_ID_e;
    }
    m_actor_id = actor_id;
    return actor_p != 0;
}

void dCsr_mng_c::ccObj_c::set(u16 param_0, u8 param_1, u8 param_2) {
    node_c::set(param_1, param_2, param_0);
}

dCsr_mng_c::csr_c::~csr_c() {
    if (m_csr == mDoGph_gInf_c::m_csr) {
        mDoGph_gInf_c::releaseCsr();
    }
}

BOOL dCsr_mng_c::csr_c::set(mDoGph_gInf_c::csr_c* i_csr, u16 param_1, u8 param_2, u8 param_3) {
    if (!node_c::set(param_2, param_3, param_1)) {
        return FALSE;
    }
    m_csr = i_csr;
    return TRUE;
}

dCsr_mng_c::ccObj_c::~ccObj_c() {
    // empty function
}
