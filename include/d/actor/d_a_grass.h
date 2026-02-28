#ifndef D_A_GRASS_H
#define D_A_GRASS_H

#include "f_op/f_op_actor_mng.h"

class cCcD_Obj;
class dCcMassS_HitInf;
class dFlower_packet_c;
class dGrass_packet_c;

/**
 * @ingroup actors-unsorted
 * @class daGrass_c
 * @brief Grass
 *
 * @details
 *
 */
class daGrass_c : public fopAc_ac_c {
public:
    bool createGrass();
    void deleteGrass();
    void executeGrass();
    void drawGrass();
    void newGrassData(cXyz&, int, u8, u8, s16, u8);
    bool createFlower();
    void deleteFlower();
    void executeFlower();
    void drawFlower();
    void newFlowerData(s8, cXyz&, int, s8, s16);

    inline int create();

    inline int Delete();
    inline int execute();
    inline int draw();

    static dGrass_packet_c* getGrass() { return m_grass; }
    static dFlower_packet_c* getFlower() { return m_flower; }

    static void deleteRoomGrass(int);
    static void deleteRoomFlower(int);

    static daGrass_c* m_myObj;
    static dGrass_packet_c* m_grass;
    static dFlower_packet_c* m_flower;
};

namespace daGrass_prm {
    inline u8 getKind(daGrass_c* i_this) { return (fopAcM_GetParam(i_this) >> 4) & 3; }
    inline u8 getType(daGrass_c* i_this) { return fopAcM_GetParam(i_this) & 0xF; }
    inline u8 getItemNo(daGrass_c* i_this) { return (fopAcM_GetParam(i_this) >> 0x10) & 0xFF; }
    inline u8 getItemBitNo(daGrass_c* i_this) { return (fopAcM_GetParam(i_this) >> 8) & 0xFF; }
};

struct daGrass_offsetData {
    /* 0x0 */ u8 num;
    /* 0x4 */ csXyz* set_type;
};


#endif /* D_A_GRASS_H */
