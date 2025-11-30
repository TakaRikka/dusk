#ifndef D_A_TAG_SPINNER_H
#define D_A_TAG_SPINNER_H

#include "d/actor/d_a_spinner.h"

class daTagSpinner_c : public fopAc_ac_c {
public:
    /* 0x568 */ f32 mRadius;
    /* 0x56c */ f32 field_0x56c;

    void initBaseMtx();
    void setBaseMtx();
    int Create();
    int CreateHeap();
    int create();
    int execute();
    int _delete();
    f32 GetR() { return mRadius; }
};

namespace daTagSpinner_prm {
inline u8 getSwNo(daTagSpinner_c* pActor) {
    return fopAcM_GetParam(pActor);
}
}  // namespace daTagSpinner_prm

#endif /* D_A_TAG_SPINNER_H */
