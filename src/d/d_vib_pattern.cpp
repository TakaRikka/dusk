#include "d/dolzel.h" // IWYU pragma: keep

#include "d/d_vibration.h"

// Motor Shock patterns
const vib_pattern dVibration_c::MS_patt[VIBMODE_S_MAX] = {
    /* VIBMODE_S_CUSTOM */ {0, 0, 0x00000000},
    /* VIBMODE_S_POWER1 */ {0, 3, 0xC0000000},
    /* VIBMODE_S_POWER2 */ {0, 4, 0xE0000000},
    /* VIBMODE_S_POWER3 */ {0, 6, 0xE8000000},
    /* VIBMODE_S_POWER4 */ {0, 7, 0xEC000000},
    /* VIBMODE_S_POWER5 */ {0, 8, 0xF6000000},
    /* VIBMODE_S_POWER6 */ {0, 9, 0xFB000000},
    /* VIBMODE_S_POWER7 */ {0, 10, 0xF6800000},
    /* VIBMODE_S_POWER8 */ {0, 11, 0xFB400000},
    /* VIBMODE_S_DOKUTT */ {0, 8, 0xC2000000},
    /* VIBMODE_S_FOR2D  */ {0, 2, 0xC0000000},
};

// Camera Shock patterns
const vib_pattern dVibration_c::CS_patt[VIBMODE_S_MAX] = {
    /* VIBMODE_S_CUSTOM */ {0, 0, 0x00000000},
    /* VIBMODE_S_POWER1 */ {0, 3, 0xC0000000},
    /* VIBMODE_S_POWER2 */ {0, 4, 0xE0000000},
    /* VIBMODE_S_POWER3 */ {0, 6, 0xE8000000},
    /* VIBMODE_S_POWER4 */ {0, 7, 0xEC000000},
    /* VIBMODE_S_POWER5 */ {0, 8, 0xF6000000},
    /* VIBMODE_S_POWER6 */ {0, 9, 0xFB000000},
    /* VIBMODE_S_POWER7 */ {0, 10, 0xF6800000},
    /* VIBMODE_S_POWER8 */ {0, 11, 0xFB400000},
    /* VIBMODE_S_DOKUTT */ {0, 8, 0xC2000000},
    /* VIBMODE_S_FOR2D  */ {0, 2, 0xC0000000},
};

// Motor Quake patterns
const vib_pattern dVibration_c::MQ_patt[VIBMODE_Q_MAX] = {
    /* VIBMODE_Q_CUSTOM */ {0, 0, 0x00000000},
    /* VIBMODE_Q_POWER1 */ {1, 32, 0x10202020},
    /* VIBMODE_Q_POWER2 */ {1, 32, 0x42104208},
    /* VIBMODE_Q_POWER3 */ {2, 32, 0x22222222},
    /* VIBMODE_Q_POWER4 */ {2, 32, 0x22492249},
    /* VIBMODE_Q_POWER5 */ {3, 32, 0x52525252},
    /* VIBMODE_Q_POWER6 */ {3, 32, 0x4A94AA95},
    /* VIBMODE_Q_POWER7 */ {4, 32, 0x36555655},
    /* VIBMODE_Q_POWER8 */ {4, 32, 0x6B6D6B6D},
    /* VIBMODE_Q_HORSE  */ {1, 27, 0x20201000},
};

// Camera Quake patterns
const vib_pattern dVibration_c::CQ_patt[VIBMODE_Q_MAX] = {
    /* VIBMODE_Q_CUSTOM */ {0, 0, 0x00000000},
    /* VIBMODE_Q_POWER1 */ {1, 32, 0x10202020},
    /* VIBMODE_Q_POWER2 */ {1, 32, 0x42104208},
    /* VIBMODE_Q_POWER3 */ {2, 32, 0x22222222},
    /* VIBMODE_Q_POWER4 */ {2, 32, 0x22492249},
    /* VIBMODE_Q_POWER5 */ {3, 32, 0x52525252},
    /* VIBMODE_Q_POWER6 */ {3, 32, 0x4A94AA95},
    /* VIBMODE_Q_POWER7 */ {4, 32, 0x36555655},
    /* VIBMODE_Q_POWER8 */ {4, 32, 0x6B6D6B6D},
    /* VIBMODE_Q_HORSE  */ {0, 27, 0x20201000},
};

const char* shock_names[VIBMODE_S_MAX] = {
    "VIBMODE_S_CUSTOM",
    "VIBMODE_S_POWER1",
    "VIBMODE_S_POWER2",
    "VIBMODE_S_POWER3",
    "VIBMODE_S_POWER4",
    "VIBMODE_S_POWER5",
    "VIBMODE_S_POWER6",
    "VIBMODE_S_POWER7",
    "VIBMODE_S_POWER8",
    "VIBMODE_S_DOKUTT",
    "VIBMODE_S_FOR2D",
};

const char* quake_names[VIBMODE_Q_MAX] = {
    "VIBMODE_Q_CUSTOM",
    "VIBMODE_Q_POWER1",
    "VIBMODE_Q_POWER2",
    "VIBMODE_Q_POWER3",
    "VIBMODE_Q_POWER4",
    "VIBMODE_Q_POWER5",
    "VIBMODE_Q_POWER6",
    "VIBMODE_Q_POWER7",
    "VIBMODE_Q_POWER8",
    "VIBMODE_Q_HORSE",
};
