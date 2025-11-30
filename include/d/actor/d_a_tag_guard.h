#ifndef D_A_TAG_GUARD_H
#define D_A_TAG_GUARD_H

#include "f_op/f_op_actor_mng.h"
#include "d/d_path.h"

class daTagGuard_c : public fopAc_ac_c {
public:
    int getAppearPoint(Vec*);
    void createGuard(u32);

    u8 getPathID() { return fopAcM_GetParam(this) >> 8; }
    inline int create();
    inline void create_init();

    /* 0x568 */ dPath* mPath;
};  // Size: 0x56C

#endif /* D_A_TAG_GUARD_H */
