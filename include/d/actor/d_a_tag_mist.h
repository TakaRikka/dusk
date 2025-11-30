#ifndef D_A_TAG_MIST_H
#define D_A_TAG_MIST_H

#include "f_op/f_op_actor_mng.h"

class daTagMist_c : public fopAc_ac_c {
public:
    void offSw();
    void onMySw();
    BOOL isMySw();
    int Create();
    int create();
    int execute();
    int _delete();

    static u8 getPlayerNo();
    static void setPlayerNo(u8 i_playerNo) { mPlayerNo = i_playerNo; }

    static u8 mPlayerNo;

    /* 0x568 */ u8 unused_0x568[0x570 - 0x568];
    /* 0x570 */ cXyz mVertices[4];
    /* 0x5A0 */ u8 mPlayerTouched;
    /* 0x5A1 */ u8 mInitParams;
    /* 0x5A2 */ u16 field_0x5a2;
    /* 0x5A4 */ u16 field_0x5a4;
};  // Size: 0x5A8

namespace daTagMist_prm {
inline u8 getSwNoNum(daTagMist_c* i_this) {
    return fopAcM_GetParam(i_this) >> 0x10;
}
inline u8 getMySwNo(daTagMist_c* i_this) {
    return fopAcM_GetParam(i_this) >> 0x8;
}
inline u8 getTopSwNo(daTagMist_c* i_this) {
    return fopAcM_GetParam(i_this);
}
inline u8 getExitNo(daTagMist_c* i_this) {
    return (fopAcM_GetParam(i_this) >> 0x18) & 0x3F;
}
inline int getSwNo2(daTagMist_c* i_this) {
    return i_this->field_0x5a2 & 0xFF;
}
}  // namespace daTagMist_prm

#endif /* D_A_TAG_MIST_H */
