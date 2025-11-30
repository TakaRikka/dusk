#ifndef D_A_TAG_LV5SOUP_H
#define D_A_TAG_LV5SOUP_H

#include "f_op/f_op_actor_mng.h"

class daTag_Lv5Soup_c : public fopAc_ac_c {
public:
    daTag_Lv5Soup_c() {}

    int create();
    int Delete();
    int Execute();
    int Draw();
    bool isAreaCheck();

    virtual ~daTag_Lv5Soup_c();
};

#endif /* D_A_TAG_LV5SOUP_H */
