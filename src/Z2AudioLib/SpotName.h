#ifndef SPOTNAME_H
#define SPOTNAME_H

// This symbol has polluted multiple TUs in the lib, hence,
// it probably was defined in the header as static by accident
static const char* sSpotName[] = {
    "F_SP00",  "F_SP103", "R_SP01",  "F_SP104", "R_SP107", "F_SP108", "R_SP108", "F_SP117",
    "F_SP109", "R_SP109", "R_SP209", "F_SP110", "R_SP110", "F_SP111", "F_SP128", "R_SP128",
    "F_SP115", "F_SP112", "F_SP126", "F_SP127", "R_SP127", "F_SP113", "F_SP116", "R_SP116",
    "R_SP160", "R_SP161", "F_SP114", "F_SP118", "F_SP124", "F_SP125", "F_SP121", "F_SP122",
    "F_SP123", "F_SP200", "F_SP102", "",        "R_SP300", "R_SP301", "T_ENEMY", "D_MN54",
    "D_MN05",  "D_MN05B", "D_MN05A", "D_MN04",  "D_MN04B", "D_MN04A", "D_MN01",  "D_MN01B",
    "D_MN01A", "D_MN10",  "D_MN10B", "D_MN10A", "D_MN11",  "D_MN11B", "D_MN11A", "D_MN06",
    "D_MN06B", "D_MN06A", "D_MN07",  "D_MN07B", "D_MN07A", "D_MN08",  "D_MN08B", "D_MN08C",
    "D_MN08A", "D_MN08D", "D_MN09",  "D_MN09A", "D_MN09B", "D_MN09C", "D_SB00",  "D_SB01",
    "D_SB02",  "D_SB03",  "D_SB04",  "D_SB05",  "D_SB06",  "D_SB07",  "D_SB08",  "D_SB09",
    "D_SB10",
};

#endif /* SPOTNAME_H */
