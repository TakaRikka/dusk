#ifndef D_A_DMIDNA_H
#define D_A_DMIDNA_H

#include "d/d_com_inf_game.h"

/**
 * @ingroup actors-unsorted
 * @class daDmidna_c
 * @brief Dying Midna (White Midna)
 *
 * @details
 *
 */
class daDmidna_c : public fopAc_ac_c {
public:
    int createHeap();
    int create();
    ~daDmidna_c();
    void setMatrix();
    int execute();
    int draw();

    /* 0x568 */ request_of_phase_process_class mPhase;
    /* 0x570 */ u8 unk_0x570[4];
    /* 0x574 */ mDoExt_McaMorfSO* mpModelMorf;
};

#endif /* D_A_DMIDNA_H */
