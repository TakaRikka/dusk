#ifndef C_BG_S_CHK_H
#define C_BG_S_CHK_H

#include <dolphin/mtx.h>
#include "f_pc/f_pc_base.h"
#include "SSystem/SComponent/c_bg_s_grp_pass_chk.h"
#include "SSystem/SComponent/c_bg_s_poly_pass_chk.h"

struct cBgD_Vtx_t : public Vec {};

class cBgS_Chk {
public:
    /* 0x0 */ cBgS_PolyPassChk* mPolyPassChk;
    /* 0x4 */ cBgS_GrpPassChk* mGrpPassChk;
    /* 0x8 */ fpc_ProcID mActorPid;
    /* 0xC */ bool mSameActorChk;
    /* 0x10 */  // __vtable__

public:
    cBgS_Chk();
    void SetExtChk(cBgS_Chk&);
    bool ChkSameActorPid(fpc_ProcID) const;

    void SetActorPid(fpc_ProcID pid) { mActorPid = pid; }
    fpc_ProcID GetActorPid() const { return mActorPid; }
    void SetPolyPassChk(cBgS_PolyPassChk* p_chk) { mPolyPassChk = p_chk; }
    void SetGrpPassChk(cBgS_GrpPassChk* p_chk) { mGrpPassChk = p_chk; }
    cBgS_PolyPassChk* GetPolyPassChk() const { return mPolyPassChk; }
    cBgS_GrpPassChk* GetGrpPassChk() const { return mGrpPassChk; }
    void OffSameActorChk() { mSameActorChk = false; }

    virtual ~cBgS_Chk();
};  // Size: 0x14

#endif /* C_BG_S_CHK_H */
