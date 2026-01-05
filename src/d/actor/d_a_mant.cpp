/**
 * @file d_a_mant.cpp
 *
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_mant.h"
#include "JSystem/J3DGraphBase/J3DDrawBuffer.h"
#include "d/actor/d_a_b_gnd.h"
#include "d/d_com_inf_game.h"

#include "assets/l_Egnd_mantTEX.h"

#include "assets/l_Egnd_mantTEX_U.h"

#include "assets/l_Egnd_mantPAL.h"
#include "d/d_s_play.h"

static u32 l_pos[507] = {
    0x42480000, 0x3F5CFC93, 0xC365BD9C, 0x4226AAAA,
    0x3F5CFC93, 0xC365BD9C, 0x42055556, 0x3F5CFC93,
    0xC365BD9C, 0x41C80000, 0x3F5CFC93, 0xC365BD9C,
    0x41855556, 0x3F5CFC93, 0xC365BD9C, 0x41055556,
    0x3F5CFC93, 0xC365BD9C, 0x358637BD, 0x3F5CFC93,
    0xC365BD9C, 0xC1055554, 0x3F5CFC93, 0xC365BD9C,
    0xC1855554, 0x3F5CFC93, 0xC365BD9C, 0xC1C7FFFF,
    0x3F5CFC93, 0xC365BD9C, 0xC2055554, 0x3F5CFC93,
    0xC365BD9C, 0xC226AAAA, 0x3F5CFC93, 0xC365BD9C,
    0xC2480000, 0x3F5CFC93, 0xC365BD9C, 0x42480000,
    0x3F5CFC93, 0xC35292F0, 0x4226AAAA, 0x3F5CFC93,
    0xC35292F0, 0x42055556, 0x3F5CFC93, 0xC35292F0,
    0x41C80000, 0x3F5CFC93, 0xC35292F0, 0x41855556,
    0x3F5CFC93, 0xC35292F0, 0x41055556, 0x3F5CFC93,
    0xC35292F0, 0x358637BD, 0x3F5CFC93, 0xC35292F0,
    0xC1055554, 0x3F5CFC93, 0xC35292F0, 0xC1855554,
    0x3F5CFC93, 0xC35292F0, 0xC1C7FFFF, 0x3F5CFC93,
    0xC35292F0, 0xC2055554, 0x3F5CFC93, 0xC35292F0,
    0xC226AAAA, 0x3F5CFC93, 0xC35292F0, 0xC2480000,
    0x3F5CFC93, 0xC35292F0, 0x42480000, 0x3F5CFC93,
    0xC33F6846, 0x4226AAAA, 0x3F5CFC93, 0xC33F6846,
    0x42055556, 0x3F5CFC93, 0xC33F6846, 0x41C80000,
    0x3F5CFC93, 0xC33F6846, 0x41855556, 0x3F5CFC93,
    0xC33F6846, 0x41055556, 0x3F5CFC93, 0xC33F6846,
    0x358637BD, 0x3F5CFC93, 0xC33F6846, 0xC1055554,
    0x3F5CFC93, 0xC33F6846, 0xC1855554, 0x3F5CFC93,
    0xC33F6846, 0xC1C7FFFF, 0x3F5CFC93, 0xC33F6846,
    0xC2055554, 0x3F5CFC93, 0xC33F6846, 0xC226AAAA,
    0x3F5CFC93, 0xC33F6846, 0xC2480000, 0x3F5CFC93,
    0xC33F6846, 0x42480000, 0x3F5CFC93, 0xC32C3D9C,
    0x4226AAAA, 0x3F5CFC93, 0xC32C3D9C, 0x42055556,
    0x3F5CFC93, 0xC32C3D9C, 0x41C80000, 0x3F5CFC93,
    0xC32C3D9C, 0x41855556, 0x3F5CFC93, 0xC32C3D9C,
    0x41055556, 0x3F5CFC93, 0xC32C3D9C, 0x358637BD,
    0x3F5CFC93, 0xC32C3D9C, 0xC1055554, 0x3F5CFC93,
    0xC32C3D9C, 0xC1855554, 0x3F5CFC93, 0xC32C3D9C,
    0xC1C7FFFF, 0x3F5CFC93, 0xC32C3D9C, 0xC2055554,
    0x3F5CFC93, 0xC32C3D9C, 0xC226AAAA, 0x3F5CFC93,
    0xC32C3D9C, 0xC2480000, 0x3F5CFC93, 0xC32C3D9C,
    0x42480000, 0x3F5CFC93, 0xC31912F1, 0x4226AAAA,
    0x3F5CFC93, 0xC31912F1, 0x42055556, 0x3F5CFC93,
    0xC31912F1, 0x41C80000, 0x3F5CFC93, 0xC31912F1,
    0x41855556, 0x3F5CFC93, 0xC31912F1, 0x41055556,
    0x3F5CFC93, 0xC31912F1, 0x358637BD, 0x3F5CFC93,
    0xC31912F1, 0xC1055554, 0x3F5CFC93, 0xC31912F1,
    0xC1855554, 0x3F5CFC93, 0xC31912F1, 0xC1C7FFFF,
    0x3F5CFC93, 0xC31912F1, 0xC2055554, 0x3F5CFC93,
    0xC31912F1, 0xC226AAAA, 0x3F5CFC93, 0xC31912F1,
    0xC2480000, 0x3F5CFC93, 0xC31912F1, 0x42480000,
    0x3F5CFC93, 0xC305E846, 0x4226AAAA, 0x3F5CFC93,
    0xC305E846, 0x42055556, 0x3F5CFC93, 0xC305E846,
    0x41C80000, 0x3F5CFC93, 0xC305E846, 0x41855556,
    0x3F5CFC93, 0xC305E846, 0x41055556, 0x3F5CFC93,
    0xC305E846, 0x358637BD, 0x3F5CFC93, 0xC305E846,
    0xC1055554, 0x3F5CFC93, 0xC305E846, 0xC1855554,
    0x3F5CFC93, 0xC305E846, 0xC1C7FFFF, 0x3F5CFC93,
    0xC305E846, 0xC2055554, 0x3F5CFC93, 0xC305E846,
    0xC226AAAA, 0x3F5CFC93, 0xC305E846, 0xC2480000,
    0x3F5CFC93, 0xC305E846, 0x42480000, 0x3F5CFC93,
    0xC2E57B38, 0x4226AAAA, 0x3F5CFC93, 0xC2E57B38,
    0x42055556, 0x3F5CFC93, 0xC2E57B38, 0x41C80000,
    0x3F5CFC93, 0xC2E57B38, 0x41855556, 0x3F5CFC93,
    0xC2E57B38, 0x41055556, 0x3F5CFC93, 0xC2E57B38,
    0x358637BD, 0x3F5CFC93, 0xC2E57B38, 0xC1055554,
    0x3F5CFC93, 0xC2E57B38, 0xC1855554, 0x3F5CFC93,
    0xC2E57B38, 0xC1C7FFFF, 0x3F5CFC93, 0xC2E57B38,
    0xC2055554, 0x3F5CFC93, 0xC2E57B38, 0xC226AAAA,
    0x3F5CFC93, 0xC2E57B38, 0xC2480000, 0x3F5CFC93,
    0xC2E57B38, 0x42480000, 0x3F5CFC93, 0xC2BF25E2,
    0x4226AAAA, 0x3F5CFC93, 0xC2BF25E2, 0x42055556,
    0x3F5CFC93, 0xC2BF25E2, 0x41C80000, 0x3F5CFC93,
    0xC2BF25E2, 0x41855556, 0x3F5CFC93, 0xC2BF25E2,
    0x41055556, 0x3F5CFC93, 0xC2BF25E2, 0x358637BD,
    0x3F5CFC93, 0xC2BF25E2, 0xC1055554, 0x3F5CFC93,
    0xC2BF25E2, 0xC1855554, 0x3F5CFC93, 0xC2BF25E2,
    0xC1C7FFFF, 0x3F5CFC93, 0xC2BF25E2, 0xC2055554,
    0x3F5CFC93, 0xC2BF25E2, 0xC226AAAA, 0x3F5CFC93,
    0xC2BF25E2, 0xC2480000, 0x3F5CFC93, 0xC2BF25E2,
    0x42480000, 0x3F5CFC93, 0xC298D08D, 0x4226AAAA,
    0x3F5CFC93, 0xC298D08D, 0x42055556, 0x3F5CFC93,
    0xC298D08D, 0x41C80000, 0x3F5CFC93, 0xC298D08D,
    0x41855556, 0x3F5CFC93, 0xC298D08D, 0x41055556,
    0x3F5CFC93, 0xC298D08D, 0x358637BD, 0x3F5CFC93,
    0xC298D08D, 0xC1055554, 0x3F5CFC93, 0xC298D08D,
    0xC1855554, 0x3F5CFC93, 0xC298D08D, 0xC1C7FFFF,
    0x3F5CFC93, 0xC298D08D, 0xC2055554, 0x3F5CFC93,
    0xC298D08D, 0xC226AAAA, 0x3F5CFC93, 0xC298D08D,
    0xC2480000, 0x3F5CFC93, 0xC298D08D, 0x42480000,
    0x3F5CFC93, 0xC264F66F, 0x4226AAAA, 0x3F5CFC93,
    0xC264F66F, 0x42055556, 0x3F5CFC93, 0xC264F66F,
    0x41C80000, 0x3F5CFC93, 0xC264F66F, 0x41855556,
    0x3F5CFC93, 0xC264F66F, 0x41055556, 0x3F5CFC93,
    0xC264F66F, 0x358637BD, 0x3F5CFC93, 0xC264F66F,
    0xC1055554, 0x3F5CFC93, 0xC264F66F, 0xC1855554,
    0x3F5CFC93, 0xC264F66F, 0xC1C7FFFF, 0x3F5CFC93,
    0xC264F66F, 0xC2055554, 0x3F5CFC93, 0xC264F66F,
    0xC226AAAA, 0x3F5CFC93, 0xC264F66F, 0xC2480000,
    0x3F5CFC93, 0xC264F66F, 0x42480000, 0x3F5CFC93,
    0xC2184BC4, 0x4226AAAA, 0x3F5CFC93, 0xC2184BC4,
    0x42055556, 0x3F5CFC93, 0xC2184BC4, 0x41C80000,
    0x3F5CFC93, 0xC2184BC4, 0x41855556, 0x3F5CFC93,
    0xC2184BC4, 0x41055556, 0x3F5CFC93, 0xC2184BC4,
    0x358637BD, 0x3F5CFC93, 0xC2184BC4, 0xC1055554,
    0x3F5CFC93, 0xC2184BC4, 0xC1855554, 0x3F5CFC93,
    0xC2184BC4, 0xC1C7FFFF, 0x3F5CFC93, 0xC2184BC4,
    0xC2055554, 0x3F5CFC93, 0xC2184BC4, 0xC226AAAA,
    0x3F5CFC93, 0xC2184BC4, 0xC2480000, 0x3F5CFC93,
    0xC2184BC4, 0x42480000, 0x3F5CFC93, 0xC1974231,
    0x4226AAAA, 0x3F5CFC93, 0xC1974231, 0x42055556,
    0x3F5CFC93, 0xC1974231, 0x41C80000, 0x3F5CFC93,
    0xC1974231, 0x41855556, 0x3F5CFC93, 0xC1974231,
    0x41055556, 0x3F5CFC93, 0xC1974231, 0x358637BD,
    0x3F5CFC93, 0xC1974231, 0xC1055554, 0x3F5CFC93,
    0xC1974231, 0xC1855554, 0x3F5CFC93, 0xC1974231,
    0xC1C7FFFF, 0x3F5CFC93, 0xC1974231, 0xC2055554,
    0x3F5CFC93, 0xC1974231, 0xC226AAAA, 0x3F5CFC93,
    0xC1974231, 0xC2480000, 0x3F5CFC93, 0xC1974231,
    0x42480000, 0x3F5CFC93, 0x3E84C964, 0x4226AAAA,
    0x3F5CFC93, 0x3E84C964, 0x42055556, 0x3F5CFC93,
    0x3E84C964, 0x41C80000, 0x3F5CFC93, 0x3E84C964,
    0x41855556, 0x3F5CFC93, 0x3E84C964, 0x41055556,
    0x3F5CFC93, 0x3E84C964, 0x358637BD, 0x3F5CFC93,
    0x3E84C964, 0xC1055554, 0x3F5CFC93, 0x3E84C964,
    0xC1855554, 0x3F5CFC93, 0x3E84C964, 0xC1C7FFFF,
    0x3F5CFC93, 0x3E84C964, 0xC2055554, 0x3F5CFC93,
    0x3E84C964, 0xC226AAAA, 0x3F5CFC93, 0x3E84C964,
    0xC2480000, 0x3F5CFC93, 0x3E84C964,
};

static u32 l_normal[3] = {
    0x00000000, 0x3F800000, 0x00000000,
};

static u32 l_texCoord[338] = {
    0x00000000, 0x3F6AAAB0, 0x3DAAAA7E, 0x3F6AAAB0,
    0x3DAAAA7E, 0x3F800000, 0x00000000, 0x3F800000,
    0x3E2AAAC1, 0x3F6AAAB0, 0x3E2AAAC1, 0x3F800000,
    0x3E800000, 0x3F6AAAB0, 0x3E800000, 0x3F800000,
    0x3EAAAA9F, 0x3F6AAAB0, 0x3EAAAA9F, 0x3F800000,
    0x3ED55561, 0x3F6AAAB0, 0x3ED55561, 0x3F800000,
    0x3F000000, 0x3F6AAAB0, 0x3F000000, 0x3F800000,
    0x3F155550, 0x3F6AAAB0, 0x3F155550, 0x3F800000,
    0x3F2AAAB0, 0x3F6AAAB0, 0x3F2AAAB0, 0x3F800000,
    0x3F400000, 0x3F6AAAB0, 0x3F400000, 0x3F800000,
    0x3F555550, 0x3F6AAAB0, 0x3F555550, 0x3F800000,
    0x3F6AAAB0, 0x3F6AAAB0, 0x3F6AAAB0, 0x3F800000,
    0x3F800000, 0x3F6AAAB0, 0x3F800000, 0x3F800000,
    0x00000000, 0x3F555550, 0x3DAAAA7E, 0x3F555550,
    0x3E2AAAC1, 0x3F555550, 0x3E800000, 0x3F555550,
    0x3EAAAA9F, 0x3F555550, 0x3ED55561, 0x3F555550,
    0x3F000000, 0x3F555550, 0x3F155550, 0x3F555550,
    0x3F2AAAB0, 0x3F555550, 0x3F400000, 0x3F555550,
    0x3F555550, 0x3F555550, 0x3F6AAAB0, 0x3F555550,
    0x3F800000, 0x3F555550, 0x00000000, 0x3F400000,
    0x3DAAAA7E, 0x3F400000, 0x3E2AAAC1, 0x3F400000,
    0x3E800000, 0x3F400000, 0x3EAAAA9F, 0x3F400000,
    0x3ED55561, 0x3F400000, 0x3F000000, 0x3F400000,
    0x3F155550, 0x3F400000, 0x3F2AAAB0, 0x3F400000,
    0x3F400000, 0x3F400000, 0x3F555550, 0x3F400000,
    0x3F6AAAB0, 0x3F400000, 0x3F800000, 0x3F400000,
    0x00000000, 0x3F2AAAB0, 0x3DAAAA7E, 0x3F2AAAB0,
    0x3E2AAAC1, 0x3F2AAAB0, 0x3E800000, 0x3F2AAAB0,
    0x3EAAAA9F, 0x3F2AAAB0, 0x3ED55561, 0x3F2AAAB0,
    0x3F000000, 0x3F2AAAB0, 0x3F155550, 0x3F2AAAB0,
    0x3F2AAAB0, 0x3F2AAAB0, 0x3F400000, 0x3F2AAAB0,
    0x3F555550, 0x3F2AAAB0, 0x3F6AAAB0, 0x3F2AAAB0,
    0x3F800000, 0x3F2AAAB0, 0x00000000, 0x3F155550,
    0x3DAAAA7E, 0x3F155550, 0x3E2AAAC1, 0x3F155550,
    0x3E800000, 0x3F155550, 0x3EAAAA9F, 0x3F155550,
    0x3ED55561, 0x3F155550, 0x3F000000, 0x3F155550,
    0x3F155550, 0x3F155550, 0x3F2AAAB0, 0x3F155550,
    0x3F400000, 0x3F155550, 0x3F555550, 0x3F155550,
    0x3F6AAAB0, 0x3F155550, 0x3F800000, 0x3F155550,
    0x00000000, 0x3F000000, 0x3DAAAA7E, 0x3F000000,
    0x3E2AAAC1, 0x3F000000, 0x3E800000, 0x3F000000,
    0x3EAAAA9F, 0x3F000000, 0x3ED55561, 0x3F000000,
    0x3F000000, 0x3F000000, 0x3F155550, 0x3F000000,
    0x3F2AAAB0, 0x3F000000, 0x3F400000, 0x3F000000,
    0x3F555550, 0x3F000000, 0x3F6AAAB0, 0x3F000000,
    0x3F800000, 0x3F000000, 0x00000000, 0x3ED55561,
    0x3DAAAA7E, 0x3ED55561, 0x3E2AAAC1, 0x3ED55561,
    0x3E800000, 0x3ED55561, 0x3EAAAA9F, 0x3ED55561,
    0x3ED55561, 0x3ED55561, 0x3F000000, 0x3ED55561,
    0x3F155550, 0x3ED55561, 0x3F2AAAB0, 0x3ED55561,
    0x3F400000, 0x3ED55561, 0x3F555550, 0x3ED55561,
    0x3F6AAAB0, 0x3ED55561, 0x3F800000, 0x3ED55561,
    0x00000000, 0x3EAAAA9F, 0x3DAAAA7E, 0x3EAAAA9F,
    0x3E2AAAC1, 0x3EAAAA9F, 0x3E800000, 0x3EAAAA9F,
    0x3EAAAA9F, 0x3EAAAA9F, 0x3ED55561, 0x3EAAAA9F,
    0x3F000000, 0x3EAAAA9F, 0x3F155550, 0x3EAAAA9F,
    0x3F2AAAB0, 0x3EAAAA9F, 0x3F400000, 0x3EAAAA9F,
    0x3F555550, 0x3EAAAA9F, 0x3F6AAAB0, 0x3EAAAA9F,
    0x3F800000, 0x3EAAAA9F, 0x00000000, 0x3E800000,
    0x3DAAAA7E, 0x3E800000, 0x3E2AAAC1, 0x3E800000,
    0x3E800000, 0x3E800000, 0x3EAAAA9F, 0x3E800000,
    0x3ED55561, 0x3E800000, 0x3F000000, 0x3E800000,
    0x3F155550, 0x3E800000, 0x3F2AAAB0, 0x3E800000,
    0x3F400000, 0x3E800000, 0x3F555550, 0x3E800000,
    0x3F6AAAB0, 0x3E800000, 0x3F800000, 0x3E800000,
    0x00000000, 0x3E2AAAC1, 0x3DAAAA7E, 0x3E2AAAC1,
    0x3E2AAAC1, 0x3E2AAAC1, 0x3E800000, 0x3E2AAAC1,
    0x3EAAAA9F, 0x3E2AAAC1, 0x3ED55561, 0x3E2AAAC1,
    0x3F000000, 0x3E2AAAC1, 0x3F155550, 0x3E2AAAC1,
    0x3F2AAAB0, 0x3E2AAAC1, 0x3F400000, 0x3E2AAAC1,
    0x3F555550, 0x3E2AAAC1, 0x3F6AAAB0, 0x3E2AAAC1,
    0x3F800000, 0x3E2AAAC1, 0x00000000, 0x3DAAAA7E,
    0x3DAAAA7E, 0x3DAAAA7E, 0x3E2AAAC1, 0x3DAAAA7E,
    0x3E800000, 0x3DAAAA7E, 0x3EAAAA9F, 0x3DAAAA7E,
    0x3ED55561, 0x3DAAAA7E, 0x3F000000, 0x3DAAAA7E,
    0x3F155550, 0x3DAAAA7E, 0x3F2AAAB0, 0x3DAAAA7E,
    0x3F400000, 0x3DAAAA7E, 0x3F555550, 0x3DAAAA7E,
    0x3F6AAAB0, 0x3DAAAA7E, 0x3F800000, 0x3DAAAA7E,
    0x00000000, 0x00000000, 0x3DAAAA7E, 0x00000000,
    0x3E2AAAC1, 0x00000000, 0x3E800000, 0x00000000,
    0x3EAAAA9F, 0x00000000, 0x3ED55561, 0x00000000,
    0x3F000000, 0x00000000, 0x3F155550, 0x00000000,
    0x3F2AAAB0, 0x00000000, 0x3F400000, 0x00000000,
    0x3F555550, 0x00000000, 0x3F6AAAB0, 0x00000000,
    0x3F800000, 0x00000000,
};

#include "assets/l_Egnd_mantDL.h"

static void* pal_d = (void*)&l_Egnd_mantPAL;

static void* tex_d[2] = {
    (void*)&l_Egnd_mantTEX,
    (void*)&l_Egnd_mantTEX_U,
};

static char lbl_277_bss_0;

void daMant_packet_c::draw() {
    void* image = tex_d[0];
    void* lut = pal_d;

    j3dSys.reinitGX();
    GXSetNumIndStages(0);
    dKy_setLight_again();
    dKy_GxFog_tevstr_set(this->mTevStr);
    GXClearVtxDesc();

    GXSetVtxDesc(GX_VA_POS, GX_INDEX8);
    GXSetVtxDesc(GX_VA_NRM, GX_INDEX8);
    GXSetVtxDesc(GX_VA_TEX0, GX_INDEX8);

    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_CLR_RGBA, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_NRM, GX_CLR_RGB, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_CLR_RGBA, GX_F32, 0);

    GXSetArray(GX_VA_POS, this->getPos(), 12);
    GXSetArray(GX_VA_NRM, this->getNrm(), 12);
    GXSetArray(GX_VA_TEX0, &l_texCoord, 8);

    GXSetZCompLoc(0);
    GXSetZMode(GX_ENABLE, GX_LEQUAL, GX_ENABLE);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0, GX_ENABLE, GX_SRC_REG, GX_SRC_REG, 0xff, GX_DF_CLAMP, GX_AF_SPOT);
    GXSetNumTexGens(1);
    GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x3c);
    GXSetNumTevStages(1);
    GXSetTevSwapMode(GX_TEVSTAGE0, GX_TEV_SWAP0, GX_TEV_SWAP0);

    dKy_Global_amb_set(this->mTevStr);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);

    GXSetTevColor(GX_TEVREG0, (GXColor){1, 0, 0, 0});
    GXSetTevKColor(GX_KCOLOR0, (GXColor){1, 0, 0, 0});

    GXSetTevKColorSel(GX_TEVSTAGE0, GX_TEV_KCSEL_K0);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_KONST, GX_CC_TEXC, GX_CC_RASC, GX_CC_C0);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_4, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_KONST, GX_CA_TEXA, GX_CA_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevKAlphaSel(GX_TEVSTAGE0, GX_TEV_KASEL_K3_A);
    GXSetAlphaCompare(GX_GREATER, 0, GX_AOP_OR, GX_GREATER, 0);

    GXTlutObj GStack_80;
    GXInitTlutObj(&GStack_80, lut, GX_TL_RGB5A3, 0x100);

    GXTexObj GStack_74;
    GXInitTexObjCI(&GStack_74, image, 0x80, 0x80, GX_TF_C8, GX_CLAMP, GX_CLAMP, 0, 0);
    GXInitTexObjLOD(&GStack_74, GX_LINEAR, GX_LINEAR, 0.0, 0.0, 0.0, 0, 0, GX_ANISO_1);

    GXLoadTlut(&GStack_80, 0);
    GXLoadTexObj(&GStack_74, GX_TEXMAP0);

    GXSetCullMode(GX_CULL_BACK);

    GXLoadPosMtxImm(this->mMtx, GX_PNMTX0);
    Mtx MStack_54;
    cMtx_inverseTranspose(this->mMtx, MStack_54);

    GXLoadNrmMtxImm(MStack_54, GX_PNMTX0);
    GXCallDisplayList(l_Egnd_mantDL, 0x3e0);

    GXInitTexObjCI(&GStack_74, l_Egnd_mantTEX_U, 0x80, 0x80, GX_TF_C8, GX_CLAMP, GX_CLAMP, 0, 0);
    GXInitTexObjLOD(&GStack_74, GX_LINEAR, GX_LINEAR, 0.0, 0.0, 0.0, 0, 0, GX_ANISO_1);
    GXLoadTexObj(&GStack_74, GX_TEXMAP0);

    GXSetTevColor(GX_TEVREG0, (GXColor){0, 0, 0, 0});
    GXSetTevKColor(GX_KCOLOR0, (GXColor){0, 0, 0, 0});

    GXSetCullMode(GX_CULL_FRONT);
    GXLoadPosMtxImm(this->mMtx2, GX_PNMTX0);
    cMtx_inverseTranspose(this->mMtx2, MStack_54);

    GXLoadNrmMtxImm(MStack_54, GX_PNMTX0);
    GXCallDisplayList(l_Egnd_mantDL, 0x3e0);

    this->field_0x74 = lbl_277_bss_0 & 1;
    J3DShape::resetVcdVatCache();
}

static int daMant_Draw(mant_class* i_this) {
    g_env_light.settingTevStruct(0, &i_this->current.pos, &i_this->tevStr);

    MtxTrans(0.0f, 0.0f, 0.0f, 0.0f);

    cMtx_concat(j3dSys.getViewMtx(), *calc_mtx, i_this->field_0x0570.getMtx());

    cMtx_concat(j3dSys.getViewMtx(), *calc_mtx, i_this->field_0x0570.getMtx2());

    i_this->field_0x0570.setTevStr(&i_this->tevStr);

    j3dSys.getDrawBuffer(0)->entryImm(&i_this->field_0x0570, 0);

    return 1;
}

static void joint_control(mant_class* i_this, mant_j_s* param_2, int param_3, f32 param_4, f32 param_5) {
    static f32 d_p[12] = {
        1.4000001f, 0.6f, 0.35f, 0.3f, 0.3f, 0.3f, 0.25f, 0.2f, 0.2f, 0.2f, 0.15f, 0.1f
    };

    mant_class* mant_sp38 = i_this;

    cXyz spFC;
    cXyz spF0;
    cXyz spE4;

    int sp34;
    f32 sp30;
    f32 sp2C;
    f32 sp28;
    cXyz* var_r30;
    cXyz* sp24;
    BOOL sp20 = FALSE;

    cXyz spD8;
    Vec spCC;

    b_gnd_class* gndActor = (b_gnd_class*)fopAcM_SearchByID(mant_sp38->parentActorID);

    f32 var_f31;
    f32 var_f30;
    f32 var_f29;
    f32 var_f28;
    f32 var_f27;
    f32 var_f26;

    f32 sp18;
    f32 sp14;

    if (gndActor->mDrawHorse != 0) {
        sp20 = TRUE;
        spD8 = gndActor->field_0x1fb8;
    } else if (i_this->field_0x3966 != 0) {
        spD8 = i_this->field_0x3928[0] + ((i_this->field_0x3928[1] - i_this->field_0x3928[0]) * 0.5f);
        spD8.y += -60.0f + KREG_F(11);
    }

    var_r30 = param_2->field_0x0;
    sp24 = param_2->field_0x9c;
    dBgS_GndChk(sp108);
    spCC = param_2->field_0x0[0];
    spCC.y += 50.0f;

    sp108.SetPos(&spCC);
    var_f27 = dComIfG_Bgsp().GroundCross(&sp108) + 3.0f;

    if (var_f27 - var_r30[0].y > 50.0f) {
        var_f27 = var_r30[0].y;
    }

    cXyz spC0;
    cXyz spB4;
    cXyz spA8(0.0f, 0.0f, 0.0f);
    cXyz sp9C(0.0f, 0.0f, 0.0f);
    cXyz sp90(0.0f, 0.0f, 0.0f);

    cMtx_YrotS(*calc_mtx, param_2->field_0x013a);
    spFC.x = 0.0f;
    spFC.y = 0.0f;
    spFC.z = i_this->field_0x3954 * (cM_ssin(param_3 * 23000) * 0.05f + 1.0f);
    MtxPosition(&spFC, &spC0);

    cXyz sp84;

    s16 sp0C;
    s16 sp0A;

    s16 sp08 = param_3 + -6;
    if (sp08 < 0) {
        sp08 *= -1;
    }

    sp08 *= (s16)(-4000 + VREG_S(5));
    spFC.x = 0.0f;
    spFC.y = 0.0f;
    spFC.z = i_this->field_0x394c;
    spFC.z *= i_this->scale.y;

    for (sp34 = 0; sp34 < 13; sp34++, var_r30++, sp24++) {
        if (0 < sp34) {
            sp14 = i_this->field_0x3950;

            spB4 = spC0 * (d_p[sp34 - 1] + NREG_F(sp34));

            sp18 = i_this->field_0x3958;
            sp18 *= 1.0f  + VREG_F(0) - sp34 * (0.07f + VREG_F(1));

            sp84.zero();

            // (1.0f / 100.0f)
            if (param_4 > 0.01f) {
                sp14 = 0.0f + VREG_F(15);
                var_f26 = param_4 * (sp34 * (0.2f + VREG_F(16)) + 1.0f);
                cMtx_YrotS(*calc_mtx, param_2->field_0x013a);
                cMtx_XrotM(*calc_mtx, param_2->field_0x0138);

                spF0.x = ((2.0f + VREG_F(17)) * var_f26) *
                    cM_ssin(i_this->field_0x25a0 * (0x1000 + JREG_S(0)) +
                        (sp34 * (-7500 + JREG_S(1))) + sp08);
                spF0.y = ((5.0f + VREG_F(18)) * var_f26) *
                    cM_ssin(i_this->field_0x25a0 * (0x1800 + JREG_S(2)) +
                        (sp34 * (-7000 + JREG_S(3))) + sp08);
                spF0.z = -15.0f + VREG_F(19);
                MtxPosition(&spF0, &sp84);
            }

            if (param_5 > 0.01f) {
                var_f26 = param_5 * (sp34 * (0.2f + VREG_F(16)) + 1.0f);
                cMtx_YrotS(*calc_mtx, param_2->field_0x013a + -6000);
                cMtx_XrotM(*calc_mtx, -5000);

                spF0.x = ((2.0f + VREG_F(17)) * var_f26) *
                    cM_ssin(i_this->field_0x25a0 * (0x448 + JREG_S(0)) +
                    (sp34 * (-7000 + JREG_S(1))) + sp08);
                spF0.y = ((6.0f + VREG_F(18)) * var_f26) *
                    cM_ssin(i_this->field_0x25a0 * (0xc48 + JREG_S(2)) +
                    (sp34 * (-7500 + JREG_S(3))) + sp08);
                spF0.z = (-15.0f + VREG_F(19)) * param_5;
                MtxPosition(&spF0, &spE4);
                sp84 += spE4;
            }

            if (i_this->field_0x3960 > 0.1f) {
                sp84.y = i_this->field_0x3960 *
                    cM_ssin(i_this->field_0x25a0 * (0x1100 + JREG_S(2)) +
                        (sp34 * (-7000 + JREG_S(3))) + sp08);
            }

            sp30 = (var_r30->x - var_r30[-1].x) + sp24->x + spB4.x + sp84.x;
            sp28 = (var_r30->z - var_r30[-1].z) + sp24->z + spB4.z + sp84.z;
            var_f31 = var_r30->y + sp24->y + sp18 + sp84.y;

            if (sp20) {
                var_f30 = var_f27;
                spE4 = spD8 - *var_r30;
                var_f29 = JMAFastSqrt(spE4.x * spE4.x + spE4.z * spE4.z);
                var_f28 = 85.0f + KREG_F(12);
                if (var_f29 < var_f28) {
                    var_f30 = spD8.y + 1.0f * JMAFastSqrt(var_f28 * var_f28 - var_f29 * var_f29) *
                        (1.0f + KREG_F(13));
                }

                if (var_f31 < var_f30) {
                    var_f31 = var_f30;
                }
            } else if (i_this->field_0x3966 != 0) {
                var_f30 = var_f27;
                spE4 = spD8 - *var_r30;
                var_f29 = JMAFastSqrt(spE4.x * spE4.x + spE4.z * spE4.z);
                var_f28 = 85.0f + KREG_F(12);
                if (var_f29 < var_f28) {
                    var_f30 = spD8.y + JMAFastSqrt(var_f28 * var_f28 - var_f29 * var_f29) *
                        (1.0f + KREG_F(13));
                }

                if (var_f31 < var_f30) {
                    var_f31 = var_f30;
                }
            } else {
                if (var_f31 < var_f27) {
                    var_f31 = var_f27;
                }
            }

            sp2C = var_f31 - var_r30[-1].y;
            sp0C = -cM_atan2s(sp2C, sp28);
            sp0A = (s16)cM_atan2s(sp30, JMAFastSqrt(sp2C * sp2C + sp28 * sp28));

            cMtx_XrotS(*calc_mtx, sp0C);
            cMtx_YrotM(*calc_mtx, sp0A);
            MtxPosition(&spFC, &spE4);

            *sp24 = *var_r30;

            var_r30->x = var_r30[-1].x + spE4.x;
            var_r30->y = var_r30[-1].y + spE4.y;
            var_r30->z = var_r30[-1].z + spE4.z;

            sp24->x = sp14 * (var_r30->x - sp24->x);
            sp24->y = sp14 * (var_r30->y - sp24->y);
            sp24->z = sp14 * (var_r30->z - sp24->z);
        }
    }
}

static void mant_v_calc(mant_class* i_this) {
    cXyz local_e4, cStack_f0, local_fc, local_108;
    f32 dVar16, dVar15, dVar14, uVar15;
    csXyz local_134(0, 0, 0);
    mant_j_s* mantJS;

    local_fc = i_this->field_0x3928[0] - i_this->field_0x3928[1];
    local_134.y = cM_atan2s(local_fc.x, local_fc.z) + 0x4000;

    mantJS = i_this->field_0x25a8;

    local_e4.x = 0.0f;

    dVar16 = local_fc.x / 12.0f;
    dVar15 = local_fc.y / 12.0f;
    dVar14 = local_fc.z / 12.0f;

    local_108 = (i_this->current.pos - i_this->field_0x3940) * 0.9f;

    if (local_108.abs() < 10.0f) {
        uVar15 = 0.0f;
    } else {
        local_134.y = cM_atan2s(local_108.x, local_108.z);
        local_134.x = -cM_atan2s(local_108.y, JMAFastSqrt(local_108.x * local_108.x + local_108.z * local_108.z));

        if (i_this->field_0x3964 != 0) {
            uVar15 = 4.0f;
            i_this->field_0x3964 = 0;
        } else {
            uVar15 = 1.0f;
        }
    }

    f32 uVar14 = 0.0f;
    if (i_this->field_0x3965 == 0) {
        if (i_this->field_0x3969 == 1) {
            uVar14 = (1.0f / 5.0f);
        } else if (i_this->field_0x3969 == 2) {
            uVar14 = 0.6f;
        } else if (i_this->field_0x3969 == 3) {
            uVar14 = (7.0f / 100.0f);
        }
    }

    for (int i = 0; i < 13; i++, mantJS++) {
        i_this->field_0x25a8[i].field_0x0[0].x = i_this->field_0x3928[1].x + (dVar16 * i);
        i_this->field_0x25a8[i].field_0x0[0].y = i_this->field_0x3928[1].y + (dVar15 * i);
        i_this->field_0x25a8[i].field_0x0[0].z = i_this->field_0x3928[1].z + (dVar14 * i);

        cMtx_YrotS(*calc_mtx, local_134.y);

        f32 temp = cM_fsin(i * 0.2617994f);
        local_e4.y = temp * -10.0f;
        local_e4.z = temp * -20.0f;

        MtxPosition(&local_e4, &cStack_f0);

        i_this->field_0x25a8[i].field_0x0[0] += cStack_f0;

        i_this->field_0x25a8[i].field_0x0138 = local_134.x;
        i_this->field_0x25a8[i].field_0x013a = local_134.y + (i + -6) * 0x5dc;

        for (int j = 1; j < 13; j++) {
            i_this->field_0x25a8[i].field_0x0[j].x += local_108.x;
            i_this->field_0x25a8[i].field_0x0[j].z += local_108.z;
        }

        joint_control(i_this, mantJS, i, uVar15, uVar14);
    }
}

static void mant_move(mant_class* i_this) {
    u8 uVar1 = i_this->field_0x0570.field_0x74;
    cXyz* pcVar5 = i_this->field_0x0570.getPos();
    mant_v_calc(i_this);
    for (int i = 0; i < 13; i++) {
        for (int j = 0; j < 13; j++) {
            pcVar5[i + j * 13] = i_this->field_0x25a8[i].field_0x0[12 - j];
        }
    }

    DCStoreRangeNoSync(i_this->field_0x0570.getPos(), 0x7ec);
}

static int mant_cut_type;

static int daMant_Execute(mant_class* i_this) {
    f32 var_f31, var_f30;
    int iVar8;
    s16 unaff_r29;
    int iVar2, uVar1, uVar4;

    fopAc_ac_c* mant_actor = (fopAc_ac_c*)i_this;

    fopAc_ac_c* unusedPlayerActor = dComIfGp_getPlayer(0);
    daPy_py_c* unusedPlayer = (daPy_py_c*)unusedPlayerActor;

    i_this->field_0x25a0++;
    lbl_277_bss_0++;

    if (i_this->field_0x399e != 0) {
        i_this->field_0x399e--;
    }

    b_gnd_class* gndActor = (b_gnd_class*)fopAcM_SearchByID(mant_actor->parentActorID);

    if (gndActor && gndActor->mDrawHorse != 0) {
        i_this->field_0x394c = 21.0f;
        i_this->field_0x3950 = 0.75f;
        i_this->field_0x3958 = -5.0f;
        i_this->field_0x3954 = -3.0f;
    } else {
        i_this->field_0x394c = 25.0f;
        i_this->field_0x3950 = 0.55f + i_this->field_0x395c * 0.2f;
        i_this->field_0x3958 = -20.0f + i_this->field_0x395c * 25.0f;
        i_this->field_0x3954 = -13.0f - i_this->field_0x395c * 5.0f;
        cLib_addCalc0(&i_this->field_0x395c, 1.0f, 0.05f);
        cLib_addCalc0(&i_this->field_0x3960, 1.0f, 0.3f);
    }

    if (i_this->field_0x3965 != 0) {
        i_this->field_0x3954 = 0.0f;
        i_this->field_0x3958 = -10.0f;
    }

    mant_move(i_this);

    i_this->field_0x3965 = 0;
    i_this->field_0x3966 = 0;

    i_this->field_0x3940 = mant_actor->current.pos;

    iVar8 = 0;

    if (i_this->field_0x3967 != 0) {
        mant_cut_type = i_this->field_0x3967;

        if (i_this->field_0x3968 < 15) {
            i_this->field_0x3968++;
            if (mant_cut_type == 0) {
                iVar8 = 40;
            } else if (mant_cut_type == 1) {
                iVar8 = 30;
            } else {
                iVar8 = 20;
            }

            unaff_r29 = cM_rndF(65536.0f);
            var_f31 = cM_rndFX(32.0f);
            var_f30 = cM_rndFX(32.0f);
        }

        i_this->field_0x3967 = 0;
    }

    for (int i = 0; i < iVar8; i++) {
        var_f31 += cM_ssin(unaff_r29);
        var_f30 += -cM_scos(unaff_r29);

        uVar4 = (int)(var_f31 + 64.0f) | (int)(var_f30 + 64.0f) << 7;

        if (mant_cut_type == 0) {
            if (i <= 3 || 36 <= i) {
                iVar2 = 1;
            } else if (i >= 12 && 28 >= i) {
                iVar2 = 9;
            } else {
                iVar2 = 4;
            }
        } else if (mant_cut_type == 1) {
            if (i <= 3 || 26 <= i) {
                iVar2 = 1;
            } else if (i >= 12 && 18 >= i) {
                iVar2 = 9;
            } else {
                iVar2 = 4;
            }
        } else if (i <= 3 || 16 <= i) {
            iVar2 = 1;
        } else {
            iVar2 = 4;
        }

        for (int j = 0; j < iVar2; j++) {
            if (j == 0) {
                uVar1 = uVar4;
            } else if (j == 1) {
                uVar1 = uVar4 + 1;
            } else if (j == 2) {
                uVar1 = uVar4 + 0x80;
            } else if (j == 3) {
                uVar1 = uVar4 + 0x81;
            } else if (j == 3) {
                uVar1 = uVar4 + 0x81;
            } else if (j == 4) {
                uVar1 = uVar4 + 2;
            } else if (j == 5) {
                uVar1 = uVar4 + 0x82;
            } else if (j == 6) {
                uVar1 = uVar4 + 0x102;
            } else if (j == 7) {
                uVar1 = uVar4 + 0x101;
            } else if (j == 8) {
                uVar1 = uVar4 + 0x100;
            }

            if (0 <= uVar1 && uVar1 < 0x4000) {
                int iVar5 = (uVar1 & 7) + (uVar1 & 0x78) * 4 + (uVar1 >> 4 & 0x18) + (uVar1 & 0x3e00);
                l_Egnd_mantTEX[iVar5] = l_Egnd_mantTEX_U[iVar5] = 0;
            }
        }
    }

    return 1;
}

static bool daMant_IsDelete(mant_class* i_this) {
    return true;
}

static int daMant_Delete(mant_class* i_this) {
    return 1;
}

static int daMant_Create(fopAc_ac_c* i_this) {
    mant_class* m_this = (mant_class*)i_this;

    fpc_ProcID unusedId = fopAcM_GetID(m_this);

    fopAcM_ct(m_this, mant_class);
    //m_this->field_0x0570.field_0x74 = 0;
    m_this->field_0x259c = fopAcM_GetParam(i_this);

    fopAcM_SetMin(i_this, -2000.0f, -2000.0f, -2000.0f);
    fopAcM_SetMax(i_this, 2000.0f, 2000.0f, 2000.0f);

    m_this->field_0x0570.mArg0 = m_this->field_0x259c;
    m_this->field_0x394c = 30.0f;
    m_this->field_0x3950 = 7.0f / 10.0f;
    m_this->field_0x3958 = -10.0f;
    m_this->scale.set(1.0f, 1.0f, 1.0f);

    for (int i = 0; i < 0x4000; i++) {
        l_Egnd_mantTEX_U[i] = 6;
    }

    lbl_277_bss_0 = 0;
    daMant_Execute(m_this);
    return 4;
}

mant_j_s::~mant_j_s() {}

mant_j_s::mant_j_s() {}

daMant_packet_c::~daMant_packet_c() {}

// cXyz::cXyz() {
extern "C" void __ct__4cXyzFv() {
    /* empty function */
}

static actor_method_class l_daMant_Method = {
    (process_method_func)daMant_Create,
    (process_method_func)daMant_Delete,
    (process_method_func)daMant_Execute,
    (process_method_func)daMant_IsDelete,
    (process_method_func)daMant_Draw,
};

actor_process_profile_definition g_profile_MANT = {
  fpcLy_CURRENT_e,         // mLayerID
  8,                       // mListID
  fpcPi_CURRENT_e,         // mListPrio
  PROC_MANT,               // mProcName
  &g_fpcLf_Method.base,   // sub_method
  sizeof(mant_class),      // mSize
  0,                       // mSizeOther
  0,                       // mParameters
  &g_fopAc_Method.base,    // sub_method
  234,                     // mPriority
  &l_daMant_Method,        // sub_method
  0x00044000,              // mStatus
  fopAc_ACTOR_e,           // mActorType
  fopAc_CULLBOX_CUSTOM_e,  // cullType
};
