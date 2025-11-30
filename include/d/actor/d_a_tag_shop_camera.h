#ifndef D_A_TAG_SHOP_CAMERA_H
#define D_A_TAG_SHOP_CAMERA_H

#include "f_op/f_op_actor_mng.h"

class daTag_ShopCamera_c : public fopAc_ac_c {
public:
    int create();
    int Delete();
    int Execute();
    int Draw();
    void initialize();
    virtual ~daTag_ShopCamera_c() {}
};

#endif /* D_A_TAG_SHOP_CAMERA_H */
