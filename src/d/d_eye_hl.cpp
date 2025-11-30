/**
 * d_eye_hl.cpp
 * dolzel2 - Eye Highlight Management
 */

#include "d/dolzel.h" // IWYU pragma: keep

#include "d/d_eye_hl.h"
#include "JSystem/J3DGraphBase/J3DTexture.h"
#include "d/d_com_inf_game.h"
#include "d/d_s_play.h"

void dEyeHL_c::entry(J3DModelData* p_mData, char const* name) {
    J3DTexture* tex = p_mData->getTexture();
    JUTNameTab* tex_nametab = p_mData->getTextureName();

    if (tex != NULL && tex_nametab != NULL) {
        for (u16 i = 0; i < tex->getNum(); i++) {
            const char* tex_name = tex_nametab->getName(i);

            if (tex_name != NULL && !strcmp(tex_name, name)) {
                m_timg = tex->getResTIMG(i);
                m_lodBias = m_timg->LODBias;
                dEyeHL_mng_c::entry(this);
                return;
            }
        }
    }
    // "Eye Highlight Texture doesn't exist!\n"
    OSReport_Error("目のハイライトテクスチャーがありません！\n");
}

void dEyeHL_c::remove() {
    dEyeHL_mng_c::remove(this);
}

dEyeHL_c* dEyeHL_mng_c::m_obj;

void dEyeHL_mng_c::update() {
    dEyeHL_c* obj = m_obj;

    if (g_envHIO.mOther.mAdjustLODBias == 0 || !dComIfGp_event_runCheck()) {
        for (; obj != NULL; obj = obj->m_next) {
            obj->m_timg->LODBias = obj->m_lodBias;
        }
    } else {
        s16 tmp = 2.0f * (dComIfGd_getView()->fovy - 45.0f);
        for (; obj != NULL; obj = obj->m_next) {
            ResTIMG* timg = obj->m_timg;
            timg->LODBias = obj->m_lodBias + tmp;
            timg->LODBias = cLib_minMaxLimit<s16>((s16)timg->LODBias, -400, 399);
        }
    }
}

void dEyeHL_mng_c::entry(dEyeHL_c* i_obj) {
    if (m_obj != NULL) {
        m_obj->m_pre = i_obj;
    }
    i_obj->m_next = m_obj;
    m_obj = i_obj;
}

void dEyeHL_mng_c::remove(dEyeHL_c* i_obj) {
    if (i_obj->m_timg != NULL) {
        dEyeHL_c* prev = i_obj->m_pre;
        dEyeHL_c* next = i_obj->m_next;

        if (next != NULL) {
            next->m_pre = prev;
        }

        if (prev == NULL) {
            m_obj = next;
        } else {
            prev->m_next = next;
            next = m_obj;
        }

        i_obj->m_timg->LODBias = i_obj->m_lodBias;
        i_obj->m_timg = NULL;
        i_obj->m_pre = NULL;
        i_obj->m_next = NULL;
    }
}
