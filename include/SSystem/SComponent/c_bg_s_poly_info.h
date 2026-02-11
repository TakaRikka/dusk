#ifndef C_BG_S_POLY_INFO_H
#define C_BG_S_POLY_INFO_H

#include <dolphin/types.h>
#include "f_pc/f_pc_manager.h"

class cBgS_PolyInfo {
private:
    /* 0x00 */ u16 mPolyIndex;
    /* 0x02 */ u16 mBgIndex;
    /* 0x04 */ void* unk_0x04;
    /* 0x08 */ fpc_ProcID mActorId;

public:
    cBgS_PolyInfo();
    bool ChkSetInfo() const;
    void ClearPi();
    void SetPolyInfo(const cBgS_PolyInfo&);
    void SetActorInfo(int, void*, fpc_ProcID);
    bool ChkSafe(const void*, fpc_ProcID) const;
    void SetPolyIndex(int);
    bool ChkBgIndex() const;

    virtual ~cBgS_PolyInfo();

    int GetPolyIndex() const { return mPolyIndex; }
    int GetBgIndex() const { return mBgIndex; }
};  // Size: 0x10

#endif /* C_BG_S_POLY_INFO_H */
