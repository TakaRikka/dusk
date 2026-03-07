/**
 * d_bg_s_movebg_actor.cpp
 * Non-static BG actor base
 */

#include "d/dolzel.h" // IWYU pragma: keep

#include "d/d_bg_s_movebg_actor.h"
#include "d/d_bg_w.h"
#include "d/d_com_inf_game.h"
#include "m_Do/m_Do_mtx.h"

dBgS_MoveBgActor::dBgS_MoveBgActor() {
    mpBgW = NULL;
}

static int CheckCreateHeap(fopAc_ac_c* p_actor) {
    return static_cast<dBgS_MoveBgActor*>(p_actor)->MoveBGCreateHeap();
}

int dBgS_MoveBgActor::CreateHeap() {
    return 1;
}

int dBgS_MoveBgActor::Create() {
    return 1;
}

int dBgS_MoveBgActor::Execute(Mtx**) {
    return 1;
}

int dBgS_MoveBgActor::Draw() {
    return 1;
}

int dBgS_MoveBgActor::Delete() {
    return 1;
}

int dBgS_MoveBgActor::IsDelete() {
    return 1;
}

int dBgS_MoveBgActor::ToFore() {
    return 1;
}

int dBgS_MoveBgActor::ToBack() {
    return 1;
}

const char* dBgS_MoveBgActor::m_name;

int dBgS_MoveBgActor::m_dzb_id;

MoveBGActor_SetFunc dBgS_MoveBgActor::m_set_func;

int dBgS_MoveBgActor::MoveBGCreateHeap() {
    if (!CreateHeap()) {
        return 0;
    }

    if (m_dzb_id == -1) {
        mpBgW = NULL;
        return 1;
    }

    mpBgW = new dBgW();
    if (mpBgW != NULL) {
        cBgD_t* res = (cBgD_t*)dComIfG_getObjectRes(m_name, m_dzb_id);
        if (!mpBgW->Set(res, cBgW::MOVE_BG_e, &mBgMtx)) {
            if (m_set_func != NULL) {
                mpBgW->SetCrrFunc(m_set_func);
            }
        } else {
            goto RET;  // probably fake match, clean up later
        }
    } else {
    RET:
        mpBgW = NULL;
        return 0;
    }

    return 1;
}

int dBgS_MoveBgActor::MoveBGCreate(char const* i_arcName, int i_dzb_id,
                                   MoveBGActor_SetFunc i_setFunc, u32 i_heapSize, Mtx* i_bgMtx) {
    if (i_bgMtx == NULL) {
        mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
        mDoMtx_stack_c::YrotM(shape_angle.y);
        mDoMtx_stack_c::scaleM(scale.x, scale.y, scale.z);
        MTXCopy(mDoMtx_stack_c::get(), mBgMtx);
    } else {
        MTXCopy(*i_bgMtx, mBgMtx);
    }

    m_name = i_arcName;
    m_dzb_id = i_dzb_id;
    m_set_func = i_setFunc;

    if (!fopAcM_entrySolidHeap(this, CheckCreateHeap, i_heapSize)) {
        return cPhs_ERROR_e;
    }

    if (mpBgW != NULL && dComIfG_Bgsp().Regist(mpBgW, this)) {
        return cPhs_ERROR_e;
    }

    // return Create() ? cPhs_COMPLEATE_e : cPhs_ERROR_e;  // using enums here changes code gen
    return Create() ? 4 : 5;
}

int dBgS_MoveBgActor::MoveBGDelete() {
    int ret = Delete();

    if (mpBgW != NULL && mpBgW->ChkUsed()) {
        dComIfG_Bgsp().Release(mpBgW);
    }
    return ret;
}

int dBgS_MoveBgActor::MoveBGExecute() {
    Mtx* new_mtx = NULL;

    int ret = Execute(&new_mtx);
    if (new_mtx == NULL) {
        mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
        mDoMtx_stack_c::YrotM(shape_angle.y);
        mDoMtx_stack_c::scaleM(scale.x, scale.y, scale.z);
        MTXCopy(mDoMtx_stack_c::get(), mBgMtx);
    } else {
        MTXCopy(*new_mtx, mBgMtx);
    }

    if (mpBgW != NULL) {
        mpBgW->Move();
    }

    return ret;
}
