#ifndef _DOLPHIN_GD_GEOMETRY_H_
#define _DOLPHIN_GD_GEOMETRY_H_

#include <dolphin/gx/GXStruct.h>

#ifdef __cplusplus
extern "C" {
#endif

// Command processor register IDs
#define CP_REG_MTXIDXA_ID     0x30  // Matrix index A
#define CP_REG_MTXIDXB_ID     0x40  // Matrix index B
#define CP_REG_VCD_LO_ID      0x50  // Vertex descriptor (lo)
#define CP_REG_VCD_HI_ID      0x60  // Vertex descriptor (hi)
#define CP_REG_VAT_GRP0_ID    0x70  // Vertex attribute table (group 0)
#define CP_REG_VAT_GRP1_ID    0x80  // Vertex attribute table (group 1)
#define CP_REG_VAT_GRP2_ID    0x90  // Vertex attribute table (group 2)
#define CP_REG_ARRAYBASE_ID   0xA0  // Vertex array start/base
#define CP_REG_ARRAYSTRIDE_ID 0xB0  // Vertex array stride

// XF locators for textures
// Projection type [30-30]
#define GX_XF_TEX_PROJTYPE_ST 30
#define GX_XF_TEX_PROJTYPE_END 30

// Input format [29-29]
#define GX_XF_TEX_INPUTFORM_ST 29
#define GX_XF_TEX_INPUTFORM_END 29

// Texture gen type [25-27]
#define GX_XF_TEX_TEXGENTYPE_ST 25
#define GX_XF_TEX_TEXGENTYPE_END 27

// Source row [20-24]
#define GX_XF_TEX_SRCROW_ST 20
#define GX_XF_TEX_SRCROW_END 24

// Bump source texture [17-19]
#define GX_XF_TEX_BUMPSRCTEX_ST 17
#define GX_XF_TEX_BUMPSRCTEX_END 19

// Bump source light [14-16]
#define GX_XF_TEX_BUMPSRCLIGHT_ST 14
#define GX_XF_TEX_BUMPSRCLIGHT_END 16

// Blitting processor registers.
#define GX_BP_REG_GENMODE 0x0  // gen mode

// display copy filters
#define GX_BP_REG_DISPCOPYFILTER0 0x1  // display copy filter 0
#define GX_BP_REG_DISPCOPYFILTER1 0x2  // display copy filter 1
#define GX_BP_REG_DISPCOPYFILTER2 0x3  // display copy filter 2
#define GX_BP_REG_DISPCOPYFILTER3 0x4  // display copy filter 3

// indirect matrices
#define GX_BP_REG_INDMTX0A 0x6  // indirect matrix 0A
#define GX_BP_REG_INDMTX0B 0x7  // indirect matrix 0B
#define GX_BP_REG_INDMTX0C 0x8  // indirect matrix 0C
#define GX_BP_REG_INDMTX1A 0x9  // indirect matrix 1A
#define GX_BP_REG_INDMTX1B 0xA  // indirect matrix 1B
#define GX_BP_REG_INDMTX1C 0xB  // indirect matrix 1C
#define GX_BP_REG_INDMTX2A 0xC  // indirect matrix 2A
#define GX_BP_REG_INDMTX2B 0xD  // indirect matrix 2B
#define GX_BP_REG_INDMTX2C 0xE  // indirect matrix 2C
#define GX_BP_REG_INDIMASK 0xF  // indirect mask

// indirect TEV stages
#define GX_BP_REG_INDTEVSTAGE0 0x10   // indirect TEV stage 0
#define GX_BP_REG_INDTEVSTAGE1 0x11   // indirect TEV stage 1
#define GX_BP_REG_INDTEVSTAGE2 0x12   // indirect TEV stage 2
#define GX_BP_REG_INDTEVSTAGE3 0x13   // indirect TEV stage 3
#define GX_BP_REG_INDTEVSTAGE4 0x14   // indirect TEV stage 4
#define GX_BP_REG_INDTEVSTAGE5 0x15   // indirect TEV stage 5
#define GX_BP_REG_INDTEVSTAGE6 0x16   // indirect TEV stage 6
#define GX_BP_REG_INDTEVSTAGE7 0x17   // indirect TEV stage 7
#define GX_BP_REG_INDTEVSTAGE8 0x18   // indirect TEV stage 8
#define GX_BP_REG_INDTEVSTAGE9 0x19   // indirect TEV stage 9
#define GX_BP_REG_INDTEVSTAGE10 0x1A  // indirect TEV stage 10
#define GX_BP_REG_INDTEVSTAGE11 0x1B  // indirect TEV stage 11
#define GX_BP_REG_INDTEVSTAGE12 0x1C  // indirect TEV stage 12
#define GX_BP_REG_INDTEVSTAGE13 0x1D  // indirect TEV stage 13
#define GX_BP_REG_INDTEVSTAGE14 0x1E  // indirect TEV stage 14
#define GX_BP_REG_INDTEVSTAGE15 0x1F  // indirect TEV stage 15

// performance manips
#define GX_BP_REG_SCISSORTL 0x20    // scissor top left
#define GX_BP_REG_SCISSORBR 0x21    // scissor bottom right
#define GX_BP_REG_LINEPTWIDTH 0x22  // line point width
#define GX_BP_REG_PERF0TRI 0x23     // performance 0 (triangle)
#define GX_BP_REG_PERF0QUAD 0x24    // performance 0 (quad)

// rasters
#define GX_BP_REG_RAS1_SS0 0x25
#define GX_BP_REG_RAS1_SS1 0x26
#define GX_BP_REG_RAS1_IREF 0x27
#define GX_BP_REG_RAS1_TREF0 0x28
#define GX_BP_REG_RAS1_TREF1 0x29
#define GX_BP_REG_RAS1_TREF2 0x2A
#define GX_BP_REG_RAS1_TREF3 0x2B
#define GX_BP_REG_RAS1_TREF4 0x2C
#define GX_BP_REG_RAS1_TREF5 0x2D
#define GX_BP_REG_RAS1_TREF6 0x2E
#define GX_BP_REG_RAS1_TREF7 0x2F

// setup sizes
#define GX_BP_REG_SU_SSIZE0 0x30
#define GX_BP_REG_SU_TSIZE0 0x31
#define GX_BP_REG_SU_SSIZE1 0x32
#define GX_BP_REG_SU_TSIZE1 0x33
#define GX_BP_REG_SU_SSIZE2 0x34
#define GX_BP_REG_SU_TSIZE2 0x35
#define GX_BP_REG_SU_SSIZE3 0x36
#define GX_BP_REG_SU_TSIZE3 0x37
#define GX_BP_REG_SU_SSIZE4 0x38
#define GX_BP_REG_SU_TSIZE4 0x39
#define GX_BP_REG_SU_SSIZE5 0x3A
#define GX_BP_REG_SU_TSIZE5 0x3B
#define GX_BP_REG_SU_SSIZE6 0x3C
#define GX_BP_REG_SU_TSIZE6 0x3D
#define GX_BP_REG_SU_SSIZE7 0x3E
#define GX_BP_REG_SU_TSIZE7 0x3F

// Z and blend controls
#define GX_BP_REG_ZMODE 0x40
#define GX_BP_REG_BLENDMODE 0x41
#define GX_BP_REG_DSTALPHA 0x42
#define GX_BP_REG_ZCONTROL 0x43
#define GX_BP_REG_FIELDMASK 0x44
#define GX_BP_REG_DRAWDONE 0x45
#define GX_BP_REG_PETOKEN 0x47
#define GX_BP_REG_PETOKENINT 0x48

// copying
#define GX_BP_REG_TEXCOPYSRCXY 0x49
#define GX_BP_REG_TEXCOPYSRCWH 0x4A
#define GX_BP_REG_TEXCOPYDST 0x4B
#define GX_BP_REG_DISPCOPYSTRIDE 0x4D
#define GX_BP_REG_DISPCOPYSCALEY 0x4E
#define GX_BP_REG_COPYCLEARAR 0x4F
#define GX_BP_REG_COPYCLEARGB 0x50
#define GX_BP_REG_COPYCLEARZ 0x51
#define GX_BP_REG_COPYFILTER0 0x53
#define GX_BP_REG_COPYFILTER1 0x54

//
#define GX_BP_REG_BOUNDINGBOX0 0x55
#define GX_BP_REG_BOUNDINGBOX1 0x56

#define GX_BP_REG_SCISSOROFFSET 0x59

// texture memory
#define GX_BP_REG_TMEMPRELOADADDR 0x60
#define GX_BP_REG_TMEMPRELOADEVEN 0x61
#define GX_BP_REG_TMEMPRELOADODD 0x62
#define GX_BP_REG_TMEMPRELOADMODE 0x63
#define GX_BP_REG_TMEMTLUTSRC 0x64
#define GX_BP_REG_TMEMTLUTDST 0x65
#define GX_BP_REG_TMEMTEXINVALIDATE 0x66

// performance 1
#define GX_BP_REG_PERF1 0x67
#define GX_BP_REG_FIELDMODE 0x68

// set modes
#define GX_BP_REG_SETMODE0_TEX0 0x80
#define GX_BP_REG_SETMODE0_TEX1 0x81
#define GX_BP_REG_SETMODE0_TEX2 0x82
#define GX_BP_REG_SETMODE0_TEX3 0x83
#define GX_BP_REG_SETMODE1_TEX0 0x84
#define GX_BP_REG_SETMODE1_TEX1 0x85
#define GX_BP_REG_SETMODE1_TEX2 0x86
#define GX_BP_REG_SETMODE1_TEX3 0x87

// set images
#define GX_BP_REG_SETIMAGE0_TEX0 0x88
#define GX_BP_REG_SETIMAGE0_TEX1 0x89
#define GX_BP_REG_SETIMAGE0_TEX2 0x8A
#define GX_BP_REG_SETIMAGE0_TEX3 0x8B
#define GX_BP_REG_SETIMAGE1_TEX0 0x8C
#define GX_BP_REG_SETIMAGE1_TEX1 0x8D
#define GX_BP_REG_SETIMAGE1_TEX2 0x8E
#define GX_BP_REG_SETIMAGE1_TEX3 0x8F
#define GX_BP_REG_SETIMAGE2_TEX0 0x90
#define GX_BP_REG_SETIMAGE2_TEX1 0x91
#define GX_BP_REG_SETIMAGE2_TEX2 0x92
#define GX_BP_REG_SETIMAGE2_TEX3 0x93
#define GX_BP_REG_SETIMAGE3_TEX0 0x94
#define GX_BP_REG_SETIMAGE3_TEX1 0x95
#define GX_BP_REG_SETIMAGE3_TEX2 0x96
#define GX_BP_REG_SETIMAGE3_TEX3 0x97

// set texture lookups
#define GX_BP_REG_SETTLUT_TEX0 0x98
#define GX_BP_REG_SETTLUT_TEX1 0x99
#define GX_BP_REG_SETTLUT_TEX2 0x9A
#define GX_BP_REG_SETTLUT_TEX3 0x9B

// set modes continued
#define GX_BP_REG_SETMODE0_TEX4 0xA0
#define GX_BP_REG_SETMODE0_TEX5 0xA1
#define GX_BP_REG_SETMODE0_TEX6 0xA2
#define GX_BP_REG_SETMODE0_TEX7 0xA3
#define GX_BP_REG_SETMODE1_TEX4 0xA4
#define GX_BP_REG_SETMODE1_TEX5 0xA5
#define GX_BP_REG_SETMODE1_TEX6 0xA6
#define GX_BP_REG_SETMODE1_TEX7 0xA7

// set images continued
#define GX_BP_REG_SETIMAGE0_TEX4 0xA8
#define GX_BP_REG_SETIMAGE0_TEX5 0xA9
#define GX_BP_REG_SETIMAGE0_TEX6 0xAA
#define GX_BP_REG_SETIMAGE0_TEX7 0xAB
#define GX_BP_REG_SETIMAGE1_TEX4 0xAC
#define GX_BP_REG_SETIMAGE1_TEX5 0xAD
#define GX_BP_REG_SETIMAGE1_TEX6 0xAE
#define GX_BP_REG_SETIMAGE1_TEX7 0xAF
#define GX_BP_REG_SETIMAGE2_TEX4 0xB0
#define GX_BP_REG_SETIMAGE2_TEX5 0xB1
#define GX_BP_REG_SETIMAGE2_TEX6 0xB2
#define GX_BP_REG_SETIMAGE2_TEX7 0xB3
#define GX_BP_REG_SETIMAGE3_TEX4 0xB4
#define GX_BP_REG_SETIMAGE3_TEX5 0xB5
#define GX_BP_REG_SETIMAGE3_TEX6 0xB6
#define GX_BP_REG_SETIMAGE3_TEX7 0xB7

// set texture lookups continued
#define GX_BP_REG_SETTLUT_TEX4 0xB8
#define GX_BP_REG_SETTLUT_TEX5 0xB9
#define GX_BP_REG_SETTLUT_TEX6 0xBA
#define GX_BP_REG_SETTLUT_TEX7 0xBB

// TEV color manips
#define GX_BP_REG_TEVCOLORCOMBINER0 0xC0
#define GX_BP_REG_TEVALPHACOMBINER0 0xC1
#define GX_BP_REG_TEVCOLORCOMBINER1 0xC2
#define GX_BP_REG_TEVALPHACOMBINER1 0xC3
#define GX_BP_REG_TEVCOLORCOMBINER2 0xC4
#define GX_BP_REG_TEVALPHACOMBINER2 0xC5
#define GX_BP_REG_TEVCOLORCOMBINER3 0xC6
#define GX_BP_REG_TEVALPHACOMBINER3 0xC7
#define GX_BP_REG_TEVCOLORCOMBINER4 0xC8
#define GX_BP_REG_TEVALPHACOMBINER4 0xC9
#define GX_BP_REG_TEVCOLORCOMBINER5 0xCA
#define GX_BP_REG_TEVALPHACOMBINER5 0xCB
#define GX_BP_REG_TEVCOLORCOMBINER6 0xCC
#define GX_BP_REG_TEVALPHACOMBINER6 0xCD
#define GX_BP_REG_TEVCOLORCOMBINER7 0xCE
#define GX_BP_REG_TEVALPHACOMBINER7 0xCF
#define GX_BP_REG_TEVCOLORCOMBINER8 0xD0
#define GX_BP_REG_TEVALPHACOMBINER8 0xD1
#define GX_BP_REG_TEVCOLORCOMBINER9 0xD2
#define GX_BP_REG_TEVALPHACOMBINER9 0xD3
#define GX_BP_REG_TEVCOLORCOMBINER10 0xD4
#define GX_BP_REG_TEVALPHACOMBINER10 0xD5
#define GX_BP_REG_TEVCOLORCOMBINER11 0xD6
#define GX_BP_REG_TEVALPHACOMBINER11 0xD7
#define GX_BP_REG_TEVCOLORCOMBINER12 0xD8
#define GX_BP_REG_TEVALPHACOMBINER12 0xD9
#define GX_BP_REG_TEVCOLORCOMBINER13 0xDA
#define GX_BP_REG_TEVALPHACOMBINER13 0xDB
#define GX_BP_REG_TEVCOLORCOMBINER14 0xDC
#define GX_BP_REG_TEVALPHACOMBINER14 0xDD
#define GX_BP_REG_TEVCOLORCOMBINER15 0xDE
#define GX_BP_REG_TEVALPHACOMBINER15 0xDF

// TEV registers
#define GX_BP_REG_TEVREG0LO 0xE0
#define GX_BP_REG_TEVREG0HI 0xE1
#define GX_BP_REG_TEVREG1LO 0xE2
#define GX_BP_REG_TEVREG1HI 0xE3
#define GX_BP_REG_TEVREG2LO 0xE4
#define GX_BP_REG_TEVREG2HI 0xE5
#define GX_BP_REG_TEVREG3LO 0xE6
#define GX_BP_REG_TEVREG3HI 0xE7

// fog registers
#define GX_BP_REG_FOGRANGE 0xE8
#define GX_BP_REG_FOGRANGEK0 0xE9
#define GX_BP_REG_FOGRANGEK1 0xEA
#define GX_BP_REG_FOGRANGEK2 0xEB
#define GX_BP_REG_FOGRANGEK3 0xEC
#define GX_BP_REG_FOGRANGEK4 0xED
#define GX_BP_REG_FOGPARAM0 0xEE
#define GX_BP_REG_FOGPARAM1 0xEF
#define GX_BP_REG_FOGPARAM2 0xF0
#define GX_BP_REG_FOGPARAM3 0xF1
#define GX_BP_REG_FOGCOLOR 0xF2

// performance manip registers
#define GX_BP_REG_ALPHACOMPARE 0xF3
#define GX_BP_REG_ZTEXTURE0 0xF4
#define GX_BP_REG_ZTEXTURE1 0xF5

// TEV K selectors
#define GX_BP_REG_TEVKSEL0 0xF6
#define GX_BP_REG_TEVKSEL1 0xF7
#define GX_BP_REG_TEVKSEL2 0xF8
#define GX_BP_REG_TEVKSEL3 0xF9
#define GX_BP_REG_TEVKSEL4 0xFA
#define GX_BP_REG_TEVKSEL5 0xFB
#define GX_BP_REG_TEVKSEL6 0xFC
#define GX_BP_REG_TEVKSEL7 0xFD

// SS mask
#define GX_BP_REG_SSMASK 0xFE

// Transform Unit Registers
#define GX_XF_REG_ERROR 0x1000
#define GX_XF_REG_DIAGNOSTICS 0x1001
#define GX_XF_REG_STATE0 0x1002
#define GX_XF_REG_STATE1 0x1003
#define GX_XF_REG_CLOCK 0x1004
#define GX_XF_REG_CLIPDISABLE 0x1005
#define GX_XF_REG_PERF0 0x1006
#define GX_XF_REG_PERF1 0x1007
#define GX_XF_REG_INVERTEXSPEC 0x1008
#define GX_XF_REG_NUMCOLORS 0x1009
#define GX_XF_REG_AMBIENT0 0x100A
#define GX_XF_REG_AMBIENT1 0x100B
#define GX_XF_REG_MATERIAL0 0x100C
#define GX_XF_REG_MATERIAL1 0x100D
#define GX_XF_REG_COLOR0CNTRL 0x100E
#define GX_XF_REG_COLOR1CNTRL 0x100F
#define GX_XF_REG_ALPHA0CNTRL 0x1010
#define GX_XF_REG_ALPHA1CNTRL 0x1011
#define GX_XF_REG_DUALTEXTRAN 0x1012
#define GX_XF_REG_MATRIXINDEX0 0x1018
#define GX_XF_REG_MATRIXINDEX1 0x1019
#define GX_XF_REG_SCALEX 0x101A
#define GX_XF_REG_SCALEY 0x101B
#define GX_XF_REG_SCALEZ 0x101C
#define GX_XF_REG_OFFSETX 0x101D
#define GX_XF_REG_OFFSETY 0x101E
#define GX_XF_REG_OFFSETZ 0x101F
#define GX_XF_REG_PROJECTIONA 0x1020
#define GX_XF_REG_PROJECTIONB 0x1021
#define GX_XF_REG_PROJECTIONC 0x1022
#define GX_XF_REG_PROJECTIOND 0x1023
#define GX_XF_REG_PROJECTIONE 0x1024
#define GX_XF_REG_PROJECTIONF 0x1025
#define GX_XF_REG_PROJECTORTHO 0x1026
#define GX_XF_REG_NUMTEX 0x103F
#define GX_XF_REG_TEX0 0x1040
#define GX_XF_REG_TEX1 0x1041
#define GX_XF_REG_TEX2 0x1042
#define GX_XF_REG_TEX3 0x1043
#define GX_XF_REG_TEX4 0x1044
#define GX_XF_REG_TEX5 0x1045
#define GX_XF_REG_TEX6 0x1046
#define GX_XF_REG_TEX7 0x1047
#define GX_XF_REG_DUALTEX0 0x1050
#define GX_XF_REG_DUALTEX1 0x1051
#define GX_XF_REG_DUALTEX2 0x1052
#define GX_XF_REG_DUALTEX3 0x1053
#define GX_XF_REG_DUALTEX4 0x1054
#define GX_XF_REG_DUALTEX5 0x1055
#define GX_XF_REG_DUALTEX6 0x1056
#define GX_XF_REG_DUALTEX7 0x1057

#define CP_REG_VCD_LO(pnMtxIdx, txMtxIdxMask, posn, norm, col0, col1) \
    ( \
        (pnMtxIdx) << 0  | \
        (txMtxIdxMask) << 1  | \
        (posn) << 9  | \
        (norm) << 11 | \
        (col0) << 13 | \
        (col1) << 15   \
    )

#define CP_REG_VCD_HI(tex0, tex1, tex2, tex3, tex4, tex5, tex6, tex7) \
    ( \
        (tex0) << 0  | \
        (tex1) << 2  | \
        (tex2) << 4  | \
        (tex3) << 6  | \
        (tex4) << 8  | \
        (tex5) << 10 | \
        (tex6) << 12 | \
        (tex7) << 14   \
    )

#define CP_REG_VAT_GRP0(posCnt, posType, posFrac, nrmCnt, nrmType, c0Cnt, c0Type, c1Cnt, c1Type, tx0Cnt, tx0Type, tx0Frac, p12, nrmIdx3) \
    ( \
        (posCnt) << 0 | \
        (posType) << 1 | \
        (posFrac) << 4 | \
        (nrmCnt) << 9 | \
        (nrmType) << 10 | \
        (c0Cnt) << 13 | \
        (c0Type) << 14 | \
        (c1Cnt) << 17 | \
        (c1Type) << 18 | \
        (tx0Cnt) << 21 | \
        (tx0Type) << 22 | \
        (tx0Frac) << 25 | \
        (p12) << 30 | \
        (nrmIdx3) << 31 \
    )

#define CP_REG_VAT_GRP1(tx1Cnt, tx1Type, tx1Frac, tx2Cnt, tx2Type, tx2Frac, tx3Cnt, tx3Type, tx3Frac, tx4Cnt, tx4Type, p11) \
    ( \
        (tx1Cnt) << 0 | \
        (tx1Type) << 1 | \
        (tx1Frac) << 4 | \
        (tx2Cnt) << 9 | \
        (tx2Type) << 10 | \
        (tx2Frac) << 13 | \
        (tx3Cnt) << 18 | \
        (tx3Type) << 19 | \
        (tx3Frac) << 22 | \
        (tx4Cnt) << 27 | \
        (tx4Type) << 28 | \
        p11 << 31 \
    )

#define CP_REG_VAT_GRP2(tx4Frac, tx5Cnt, tx5Type, tx5Frac, tx6Cnt, tx6Type, tx6Frac, tx7Cnt, tx7Type, tx7Frac) \
    ( \
        (tx4Frac) << 0 | \
        (tx5Cnt) << 5 | \
        (tx5Type) << 6 | \
        (tx5Frac) << 9 | \
        (tx6Cnt) << 14 | \
        (tx6Type) << 15 | \
        (tx6Frac) << 18 | \
        (tx7Cnt) << 23 | \
        (tx7Type) << 24 | \
        (tx7Frac) << 27 \
    )

// Transform unit register IDs
#define XF_REG_ERROR_ID        0x1000
#define XF_REG_DIAGNOSTICS_ID  0x1001
#define XF_REG_STATE0_ID       0x1002
#define XF_REG_STATE1_ID       0x1003
#define XF_REG_CLOCK_ID        0x1004
#define XF_REG_CLIPDISABLE_ID  0x1005
#define XF_REG_PERF0_ID        0x1006
#define XF_REG_PERF1_ID        0x1007
#define XF_REG_INVERTEXSPEC_ID 0x1008
#define XF_REG_NUMCOLORS_ID    0x1009
#define XF_REG_DUALTEXTRAN_ID  0x1012
#define XF_REG_SCALEX_ID       0x101A
#define XF_REG_SCALEY_ID       0x101B
#define XF_REG_SCALEZ_ID       0x101C
#define XF_REG_OFFSETX_ID      0x101D
#define XF_REG_OFFSETY_ID      0x101E
#define XF_REG_OFFSETZ_ID      0x101F
#define XF_REG_NUMTEX_ID       0x103F
#define XF_REG_TEX0_ID         0x1040
#define XF_REG_TEX1_ID         0x1041
#define XF_REG_TEX2_ID         0x1042
#define XF_REG_TEX3_ID         0x1043
#define XF_REG_TEX4_ID         0x1044
#define XF_REG_TEX5_ID         0x1045
#define XF_REG_TEX6_ID         0x1046
#define XF_REG_TEX7_ID         0x1047
#define XF_REG_DUALTEX0_ID     0x1050
#define XF_REG_DUALTEX1_ID     0x1051
#define XF_REG_DUALTEX2_ID     0x1052
#define XF_REG_DUALTEX3_ID     0x1053
#define XF_REG_DUALTEX4_ID     0x1054
#define XF_REG_DUALTEX5_ID     0x1055
#define XF_REG_DUALTEX6_ID     0x1056
#define XF_REG_DUALTEX7_ID     0x1057

#define XF_REG_INVTXSPEC(ncols, nnorms, ntexs) \
    ( \
        (ncols) << 0 | \
        (nnorms) << 2 | \
        (ntexs) << 4 \
    )

#define XF_REG_TEX(proj, form, tgType, row, embossRow, embossLit) \
    ( \
        (proj) << 1 | \
        (form) << 2 | \
        (tgType) << 4 | \
        (row) << 7 | \
        (embossRow) << 12 | \
        (embossLit) << 15 \
    )

#define XF_REG_DUALTEX(mtx, normalize) \
    ( \
        (mtx) << 0 | \
        (normalize) << 8 \
    )

#define BP_GEN_MODE(nTexGens, nChans, nTevs, p4, nInds) \
    ( \
        (u32)(nTexGens) << 0 | \
        (u32)(nChans) << 4 | \
        (u32)(nTevs) << 10 | \
        (u32)(p4) << 14 | \
        (u32)(nInds) << 16 \
    )

#define BP_LP_SIZE(lineWidth, pointSize, lineOffset, pointOffset, lineHalfAspect, p5) \
    ( \
        (u32)(lineWidth) << 0 | \
        (u32)(pointSize) << 8 | \
        (u32)(lineOffset) << 16 | \
        (u32)(pointOffset) << 19 | \
        (u32)(lineHalfAspect) << 22 | \
        (u32)(p5) << 24 \
    )

void GDSetVtxDescv(const GXVtxDescList* attrPtr);
void GDSetVtxAttrFmtv(GXVtxFmt vtxfmt, const GXVtxAttrFmtList* list);
void GDSetArray(GXAttr attr, void* base_ptr, u8 stride);
void GDSetArrayRaw(GXAttr attr, u32 base_ptr_raw, u8 stride);
void GDPatchArrayPtr(void* base_ptr);
void GDSetTexCoordGen(GXTexCoordID dst_coord, GXTexGenType func, GXTexGenSrc src_param, u8 normalize, u32 postmtx);
void GDSetCullMode(GXCullMode mode);
void GDSetGenMode(u8 nTexGens, u8 nChans, u8 nTevs);
void GDSetGenMode2(u8 nTexGens, u8 nChans, u8 nTevs, u8 nInds, GXCullMode cm);
void GDSetLPSize(u8 lineWidth, u8 pointSize, GXTexOffset lineOffset, GXTexOffset pointOffset, u8 lineHalfAspect);
void GDSetCoPlanar(u8 enable);

#ifdef __cplusplus
}
#endif

#endif
