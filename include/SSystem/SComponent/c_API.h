#ifndef C_API_H
#define C_API_H

typedef void (*cAPIGph_Mthd)(void);

struct cAPI_Interface {
    /* 0x00 */ cAPIGph_Mthd createMtd;
    /* 0x04 */ cAPIGph_Mthd beforeOfDrawMtd;
    /* 0x08 */ cAPIGph_Mthd afterOfDrawMtd;
    /* 0x0C */ cAPIGph_Mthd painterMtd;
    /* 0x10 */ cAPIGph_Mthd blankingOnMtd;
    /* 0x14 */ cAPIGph_Mthd blankingOffMtd;
};

DUSK_GAME_EXTERN cAPI_Interface g_cAPI_Interface;

#endif /* C_API_H */
