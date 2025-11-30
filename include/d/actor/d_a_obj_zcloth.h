#ifndef D_A_OBJ_ZCLOTH_H
#define D_A_OBJ_ZCLOTH_H

#include "d/actor/d_a_itembase.h"

/**
 * @ingroup actors-objects
 * @class daObjZCloth_c
 * @brief Zora Armor?
 *
 * @details
 *
 */
class daObjZCloth_c : public daItemBase_c {
public:
    void initBaseMtx();
    void setBaseMtx();
    int Create();
    int create();
    int execute();
    int draw();
    int _delete();
};

STATIC_ASSERT(sizeof(daObjZCloth_c) == 0x92C);

#endif /* D_A_OBJ_ZCLOTH_H */
