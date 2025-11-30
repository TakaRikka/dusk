#!/usr/bin/env python3

from argparse import ArgumentParser
import struct
import os
from enum import Enum
from binary_funcs import read_u16, read_u8


class GXBPRegs(Enum):
    GX_BP_REG_GENMODE = 0x0

    GX_BP_REG_DISPCOPYFILTER0 = 0x1
    GX_BP_REG_DISPCOPYFILTER1 = 0x2
    GX_BP_REG_DISPCOPYFILTER2 = 0x3
    GX_BP_REG_DISPCOPYFILTER3 = 0x4

    GX_BP_REG_INDMTX0A = 0x6
    GX_BP_REG_INDMTX0B = 0x7
    GX_BP_REG_INDMTX0C = 0x8
    GX_BP_REG_INDMTX1A = 0x9
    GX_BP_REG_INDMTX1B = 0xA
    GX_BP_REG_INDMTX1C = 0xB
    GX_BP_REG_INDMTX2A = 0xC
    GX_BP_REG_INDMTX2B = 0xD
    GX_BP_REG_INDMTX2C = 0xE
    GX_BP_REG_INDIMASK = 0xF

    GX_BP_REG_INDTEVSTAGE0 = 0x10
    GX_BP_REG_INDTEVSTAGE1 = 0x11
    GX_BP_REG_INDTEVSTAGE2 = 0x12
    GX_BP_REG_INDTEVSTAGE3 = 0x13
    GX_BP_REG_INDTEVSTAGE4 = 0x14
    GX_BP_REG_INDTEVSTAGE5 = 0x15
    GX_BP_REG_INDTEVSTAGE6 = 0x16
    GX_BP_REG_INDTEVSTAGE7 = 0x17
    GX_BP_REG_INDTEVSTAGE8 = 0x18
    GX_BP_REG_INDTEVSTAGE9 = 0x19
    GX_BP_REG_INDTEVSTAGE10 = 0x1A
    GX_BP_REG_INDTEVSTAGE11 = 0x1B
    GX_BP_REG_INDTEVSTAGE12 = 0x1C
    GX_BP_REG_INDTEVSTAGE13 = 0x1D
    GX_BP_REG_INDTEVSTAGE14 = 0x1E
    GX_BP_REG_INDTEVSTAGE15 = 0x1F

    GX_BP_REG_SCISSORTL = 0x20
    GX_BP_REG_SCISSORBR = 0x21
    GX_BP_REG_LINEPTWIDTH = 0x22
    GX_BP_REG_PERF0TRI = 0x23
    GX_BP_REG_PERF0QUAD = 0x24

    GX_BP_REG_RAS1_SS0 = 0x25
    GX_BP_REG_RAS1_SS1 = 0x26
    GX_BP_REG_RAS1_IREF = 0x27
    GX_BP_REG_RAS1_TREF0 = 0x28
    GX_BP_REG_RAS1_TREF1 = 0x29
    GX_BP_REG_RAS1_TREF2 = 0x2A
    GX_BP_REG_RAS1_TREF3 = 0x2B
    GX_BP_REG_RAS1_TREF4 = 0x2C
    GX_BP_REG_RAS1_TREF5 = 0x2D
    GX_BP_REG_RAS1_TREF6 = 0x2E
    GX_BP_REG_RAS1_TREF7 = 0x2F

    GX_BP_REG_SU_SSIZE0 = 0x30
    GX_BP_REG_SU_TSIZE0 = 0x31
    GX_BP_REG_SU_SSIZE1 = 0x32
    GX_BP_REG_SU_TSIZE1 = 0x33
    GX_BP_REG_SU_SSIZE2 = 0x34
    GX_BP_REG_SU_TSIZE2 = 0x35
    GX_BP_REG_SU_SSIZE3 = 0x36
    GX_BP_REG_SU_TSIZE3 = 0x37
    GX_BP_REG_SU_SSIZE4 = 0x38
    GX_BP_REG_SU_TSIZE4 = 0x39
    GX_BP_REG_SU_SSIZE5 = 0x3A
    GX_BP_REG_SU_TSIZE5 = 0x3B
    GX_BP_REG_SU_SSIZE6 = 0x3C
    GX_BP_REG_SU_TSIZE6 = 0x3D
    GX_BP_REG_SU_SSIZE7 = 0x3E
    GX_BP_REG_SU_TSIZE7 = 0x3F

    GX_BP_REG_ZMODE = 0x40
    GX_BP_REG_BLENDMODE = 0x41
    GX_BP_REG_DSTALPHA = 0x42
    GX_BP_REG_ZCONTROL = 0x43
    GX_BP_REG_FIELDMASK = 0x44
    GX_BP_REG_DRAWDONE = 0x45
    GX_BP_REG_PETOKEN = 0x47
    GX_BP_REG_PETOKENINT = 0x48

    GX_BP_REG_TEXCOPYSRCXY = 0x49
    GX_BP_REG_TEXCOPYSRCWH = 0x4A
    GX_BP_REG_TEXCOPYDST = 0x4B
    GX_BP_REG_DISPCOPYSTRIDE = 0x4D
    GX_BP_REG_DISPCOPYSCALEY = 0x4E
    GX_BP_REG_COPYCLEARAR = 0x4F
    GX_BP_REG_COPYCLEARGB = 0x50
    GX_BP_REG_COPYCLEARZ = 0x51
    GX_BP_REG_COPYFILTER0 = 0x53
    GX_BP_REG_COPYFILTER1 = 0x54

    GX_BP_REG_BOUNDINGBOX0 = 0x55
    GX_BP_REG_BOUNDINGBOX1 = 0x56

    GX_BP_REG_SCISSOROFFSET = 0x59

    GX_BP_REG_TMEMPRELOADADDR = 0x60
    GX_BP_REG_TMEMPRELOADEVEN = 0x61
    GX_BP_REG_TMEMPRELOADODD = 0x62
    GX_BP_REG_TMEMPRELOADMODE = 0x63
    GX_BP_REG_TMEMTLUTSRC = 0x64
    GX_BP_REG_TMEMTLUTDST = 0x65
    GX_BP_REG_TMEMTEXINVALIDATE = 0x66

    GX_BP_REG_PERF1 = 0x67
    GX_BP_REG_FIELDMODE = 0x68

    GX_BP_REG_SETMODE0_TEX0 = 0x80
    GX_BP_REG_SETMODE0_TEX1 = 0x81
    GX_BP_REG_SETMODE0_TEX2 = 0x82
    GX_BP_REG_SETMODE0_TEX3 = 0x83
    GX_BP_REG_SETMODE1_TEX0 = 0x84
    GX_BP_REG_SETMODE1_TEX1 = 0x85
    GX_BP_REG_SETMODE1_TEX2 = 0x86
    GX_BP_REG_SETMODE1_TEX3 = 0x87

    GX_BP_REG_SETIMAGE0_TEX0 = 0x88
    GX_BP_REG_SETIMAGE0_TEX1 = 0x89
    GX_BP_REG_SETIMAGE0_TEX2 = 0x8A
    GX_BP_REG_SETIMAGE0_TEX3 = 0x8B
    GX_BP_REG_SETIMAGE1_TEX0 = 0x8C
    GX_BP_REG_SETIMAGE1_TEX1 = 0x8D
    GX_BP_REG_SETIMAGE1_TEX2 = 0x8E
    GX_BP_REG_SETIMAGE1_TEX3 = 0x8F
    GX_BP_REG_SETIMAGE2_TEX0 = 0x90
    GX_BP_REG_SETIMAGE2_TEX1 = 0x91
    GX_BP_REG_SETIMAGE2_TEX2 = 0x92
    GX_BP_REG_SETIMAGE2_TEX3 = 0x93
    GX_BP_REG_SETIMAGE3_TEX0 = 0x94
    GX_BP_REG_SETIMAGE3_TEX1 = 0x95
    GX_BP_REG_SETIMAGE3_TEX2 = 0x96
    GX_BP_REG_SETIMAGE3_TEX3 = 0x97

    GX_BP_REG_SETTLUT_TEX0 = 0x98
    GX_BP_REG_SETTLUT_TEX1 = 0x99
    GX_BP_REG_SETTLUT_TEX2 = 0x9A
    GX_BP_REG_SETTLUT_TEX3 = 0x9B

    GX_BP_REG_SETMODE0_TEX4 = 0xA0
    GX_BP_REG_SETMODE0_TEX5 = 0xA1
    GX_BP_REG_SETMODE0_TEX6 = 0xA2
    GX_BP_REG_SETMODE0_TEX7 = 0xA3
    GX_BP_REG_SETMODE1_TEX4 = 0xA4
    GX_BP_REG_SETMODE1_TEX5 = 0xA5
    GX_BP_REG_SETMODE1_TEX6 = 0xA6
    GX_BP_REG_SETMODE1_TEX7 = 0xA7

    GX_BP_REG_SETIMAGE0_TEX4 = 0xA8
    GX_BP_REG_SETIMAGE0_TEX5 = 0xA9
    GX_BP_REG_SETIMAGE0_TEX6 = 0xAA
    GX_BP_REG_SETIMAGE0_TEX7 = 0xAB
    GX_BP_REG_SETIMAGE1_TEX4 = 0xAC
    GX_BP_REG_SETIMAGE1_TEX5 = 0xAD
    GX_BP_REG_SETIMAGE1_TEX6 = 0xAE
    GX_BP_REG_SETIMAGE1_TEX7 = 0xAF
    GX_BP_REG_SETIMAGE2_TEX4 = 0xB0
    GX_BP_REG_SETIMAGE2_TEX5 = 0xB1
    GX_BP_REG_SETIMAGE2_TEX6 = 0xB2
    GX_BP_REG_SETIMAGE2_TEX7 = 0xB3
    GX_BP_REG_SETIMAGE3_TEX4 = 0xB4
    GX_BP_REG_SETIMAGE3_TEX5 = 0xB5
    GX_BP_REG_SETIMAGE3_TEX6 = 0xB6
    GX_BP_REG_SETIMAGE3_TEX7 = 0xB7

    GX_BP_REG_SETTLUT_TEX4 = 0xB8
    GX_BP_REG_SETTLUT_TEX5 = 0xB9
    GX_BP_REG_SETTLUT_TEX6 = 0xBA
    GX_BP_REG_SETTLUT_TEX7 = 0xBB

    GX_BP_REG_TEVCOLORCOMBINER0 = 0xC0
    GX_BP_REG_TEVALPHACOMBINER0 = 0xC1
    GX_BP_REG_TEVCOLORCOMBINER1 = 0xC2
    GX_BP_REG_TEVALPHACOMBINER1 = 0xC3
    GX_BP_REG_TEVCOLORCOMBINER2 = 0xC4
    GX_BP_REG_TEVALPHACOMBINER2 = 0xC5
    GX_BP_REG_TEVCOLORCOMBINER3 = 0xC6
    GX_BP_REG_TEVALPHACOMBINER3 = 0xC7
    GX_BP_REG_TEVCOLORCOMBINER4 = 0xC8
    GX_BP_REG_TEVALPHACOMBINER4 = 0xC9
    GX_BP_REG_TEVCOLORCOMBINER5 = 0xCA
    GX_BP_REG_TEVALPHACOMBINER5 = 0xCB
    GX_BP_REG_TEVCOLORCOMBINER6 = 0xCC
    GX_BP_REG_TEVALPHACOMBINER6 = 0xCD
    GX_BP_REG_TEVCOLORCOMBINER7 = 0xCE
    GX_BP_REG_TEVALPHACOMBINER7 = 0xCF
    GX_BP_REG_TEVCOLORCOMBINER8 = 0xD0
    GX_BP_REG_TEVALPHACOMBINER8 = 0xD1
    GX_BP_REG_TEVCOLORCOMBINER9 = 0xD2
    GX_BP_REG_TEVALPHACOMBINER9 = 0xD3
    GX_BP_REG_TEVCOLORCOMBINER10 = 0xD4
    GX_BP_REG_TEVALPHACOMBINER10 = 0xD5
    GX_BP_REG_TEVCOLORCOMBINER11 = 0xD6
    GX_BP_REG_TEVALPHACOMBINER11 = 0xD7
    GX_BP_REG_TEVCOLORCOMBINER12 = 0xD8
    GX_BP_REG_TEVALPHACOMBINER12 = 0xD9
    GX_BP_REG_TEVCOLORCOMBINER13 = 0xDA
    GX_BP_REG_TEVALPHACOMBINER13 = 0xDB
    GX_BP_REG_TEVCOLORCOMBINER14 = 0xDC
    GX_BP_REG_TEVALPHACOMBINER14 = 0xDD
    GX_BP_REG_TEVCOLORCOMBINER15 = 0xDE
    GX_BP_REG_TEVALPHACOMBINER15 = 0xDF

    GX_BP_REG_TEVREG0LO = 0xE0
    GX_BP_REG_TEVREG0HI = 0xE1
    GX_BP_REG_TEVREG1LO = 0xE2
    GX_BP_REG_TEVREG1HI = 0xE3
    GX_BP_REG_TEVREG2LO = 0xE4
    GX_BP_REG_TEVREG2HI = 0xE5
    GX_BP_REG_TEVREG3LO = 0xE6
    GX_BP_REG_TEVREG3HI = 0xE7

    GX_BP_REG_FOGRANGE = 0xE8
    GX_BP_REG_FOGRANGEK0 = 0xE9
    GX_BP_REG_FOGRANGEK1 = 0xEA
    GX_BP_REG_FOGRANGEK2 = 0xEB
    GX_BP_REG_FOGRANGEK3 = 0xEC
    GX_BP_REG_FOGRANGEK4 = 0xED
    GX_BP_REG_FOGPARAM0 = 0xEE
    GX_BP_REG_FOGPARAM1 = 0xEF
    GX_BP_REG_FOGPARAM2 = 0xF0
    GX_BP_REG_FOGPARAM3 = 0xF1
    GX_BP_REG_FOGCOLOR = 0xF2

    GX_BP_REG_ALPHACOMPARE = 0xF3
    GX_BP_REG_ZTEXTURE0 = 0xF4
    GX_BP_REG_ZTEXTURE1 = 0xF5

    GX_BP_REG_TEVKSEL0 = 0xF6
    GX_BP_REG_TEVKSEL1 = 0xF7
    GX_BP_REG_TEVKSEL2 = 0xF8
    GX_BP_REG_TEVKSEL3 = 0xF9
    GX_BP_REG_TEVKSEL4 = 0xFA
    GX_BP_REG_TEVKSEL5 = 0xFB
    GX_BP_REG_TEVKSEL6 = 0xFC
    GX_BP_REG_TEVKSEL7 = 0xFD

    GX_BP_REG_SSMASK = 0xFE


class GXXFRegs(Enum):
    GX_XF_REG_ERROR = 0x1000
    GX_XF_REG_DIAGNOSTICS = 0x1001
    GX_XF_REG_STATE0 = 0x1002
    GX_XF_REG_STATE1 = 0x1003
    GX_XF_REG_CLOCK = 0x1004
    GX_XF_REG_CLIPDISABLE = 0x1005
    GX_XF_REG_PERF0 = 0x1006
    GX_XF_REG_PERF1 = 0x1007
    GX_XF_REG_INVERTEXSPEC = 0x1008
    GX_XF_REG_NUMCOLORS = 0x1009
    GX_XF_REG_AMBIENT0 = 0x100A
    GX_XF_REG_AMBIENT1 = 0x100B
    GX_XF_REG_MATERIAL0 = 0x100C
    GX_XF_REG_MATERIAL1 = 0x100D
    GX_XF_REG_COLOR0CNTRL = 0x100E
    GX_XF_REG_COLOR1CNTRL = 0x100F
    GX_XF_REG_ALPHA0CNTRL = 0x1010
    GX_XF_REG_ALPHA1CNTRL = 0x1011
    GX_XF_REG_DUALTEXTRAN = 0x1012
    GX_XF_REG_MATRIXINDEX0 = 0x1018
    GX_XF_REG_MATRIXINDEX1 = 0x1019
    GX_XF_REG_SCALEX = 0x101A
    GX_XF_REG_SCALEY = 0x101B
    GX_XF_REG_SCALEZ = 0x101C
    GX_XF_REG_OFFSETX = 0x101D
    GX_XF_REG_OFFSETY = 0x101E
    GX_XF_REG_OFFSETZ = 0x101F
    GX_XF_REG_PROJECTIONA = 0x1020
    GX_XF_REG_PROJECTIONB = 0x1021
    GX_XF_REG_PROJECTIONC = 0x1022
    GX_XF_REG_PROJECTIOND = 0x1023
    GX_XF_REG_PROJECTIONE = 0x1024
    GX_XF_REG_PROJECTIONF = 0x1025
    GX_XF_REG_PROJECTORTHO = 0x1026
    GX_XF_REG_NUMTEX = 0x103F
    GX_XF_REG_TEX0 = 0x1040
    GX_XF_REG_TEX1 = 0x1041
    GX_XF_REG_TEX2 = 0x1042
    GX_XF_REG_TEX3 = 0x1043
    GX_XF_REG_TEX4 = 0x1044
    GX_XF_REG_TEX5 = 0x1045
    GX_XF_REG_TEX6 = 0x1046
    GX_XF_REG_TEX7 = 0x1047
    GX_XF_REG_DUALTEX0 = 0x1050
    GX_XF_REG_DUALTEX1 = 0x1051
    GX_XF_REG_DUALTEX2 = 0x1052
    GX_XF_REG_DUALTEX3 = 0x1053
    GX_XF_REG_DUALTEX4 = 0x1054
    GX_XF_REG_DUALTEX5 = 0x1055
    GX_XF_REG_DUALTEX6 = 0x1056
    GX_XF_REG_DUALTEX7 = 0x1057


class GXCPRegs(Enum):
    GX_CP_REG_MTXIDXA = 0x30
    GX_CP_REG_MTXIDXB = 0x40
    GX_CP_REG_VCD_LO = 0x50
    GX_CP_REG_VCD_HI = 0x60
    GX_CP_REG_VAT_GRP0 = 0x70
    GX_CP_REG_VAT_GRP1 = 0x80
    GX_CP_REG_VAT_GRP2 = 0x90
    GX_CP_REG_ARRAYBASE = 0xA0
    GX_CP_REG_ARRAYSTRIDE = 0xB0


def convert_binary_to_matDL_c_source(src_path, dest_path, symbol_name, scope):
    # Load data
    macro_name = os.path.splitext(os.path.basename(src_path))[0]
    with open(src_path, "rb") as binary_file, open(dest_path, "w") as c_file:
        c_file.write("#ifndef LOAD_BP_REG\n")
        c_file.write("#define U32_AS_U8(v) (((v) >> 24) & 0xFF), (((v) >> 16) & 0xFF), (((v) >> 8) & 0xFF), (((v) >> 0) & 0xFF)\n")
        c_file.write("#define U24_AS_U8(v) (((v) >> 16) & 0xFF), (((v) >> 8) & 0xFF), (((v) >> 0) & 0xFF)\n")
        c_file.write("#define U16_AS_U8(v) (((v) >> 8) & 0xFF), (((v) >> 0) & 0xFF)\n")
        c_file.write("#define IMAGE_ADDR(addr) (u32)(addr) >> 5\n")
        c_file.write("#define LOAD_BP_REG(reg, value) GX_CMD_LOAD_BP_REG, reg, U24_AS_U8(value)\n")
        c_file.write("#define LOAD_XF_REG(reg, num_args, ...) GX_CMD_LOAD_XF_REG, U16_AS_U8(num_args-1), U16_AS_U8(reg), __VA_ARGS__\n")
        c_file.write("#define LOAD_CP_REG(reg, value) GX_CMD_LOAD_CP_REG, reg, U32_AS_U8(value)\n")
        c_file.write("#endif\n\n")
        
        def write_macro_line(line):
            line_with_escape = line + (" "*(99-len(line))) + "\\\n"
            c_file.write(line_with_escape)
        
        if scope == "local":
            var_def_prefix = "static "
        else:
            var_def_prefix = ""
        
        write_macro_line(f"#define {macro_name}(TEX_NAME)")
        write_macro_line(f"{var_def_prefix}u8 {symbol_name}[] ATTRIBUTE_ALIGN(32) = {{")
        
        while True:
            try:
                command_type = read_u8(binary_file)
            except EOFError:
                break
            
            line_elements = []
            def add_byte_to_line(byte):
                line_elements.append(f"0x{byte:02X}")
            def add_string_to_line(string):
                line_elements.append(string)
            
            if command_type == 0x61:
                # BP Command
                register = GXBPRegs(read_u8(binary_file))
                value = read_u8(binary_file) << 16 | read_u8(binary_file) << 8 | read_u8(binary_file) << 0
                if register == GXBPRegs.GX_BP_REG_SETIMAGE3_TEX0:
                    value_string = f"IMAGE_ADDR(TEX_NAME)"
                elif register == GXBPRegs.GX_BP_REG_SU_SSIZE0:
                    value_string = f"TEX_NAME ## __width"
                elif register == GXBPRegs.GX_BP_REG_SU_TSIZE0:
                    value_string = f"TEX_NAME ## __height"
                else:
                    value_string = f"0x{value:06X}"
                add_string_to_line(f"LOAD_BP_REG({register.name}, {value_string})")
            
            elif command_type == 0x10:
                # XF Command
                num_args = 1 + read_u16(binary_file)
                assert num_args <= 0x10
                register = GXXFRegs(read_u16(binary_file))
                arg_bytes = []
                for i in range(4 * num_args):
                    arg_bytes.append(read_u8(binary_file))
                args_string = ", ".join(f"0x{byte:02X}" for byte in arg_bytes)
                add_string_to_line(f"LOAD_XF_REG({register.name}, {num_args}, {args_string})")
            
            elif command_type == 0x08:
                # CP Command
                register = GXCPRegs(read_u8(binary_file))
                value = read_u8(binary_file) << 24 | read_u8(binary_file) << 16 | read_u8(binary_file) << 8 | read_u8(binary_file) << 0
                add_string_to_line(f"LOAD_CP_REG({register.name}, 0x{value:08X})")
            
            elif command_type == 0x00:
                add_byte_to_line(command_type)
                
                while len(line_elements) < 0x10:
                    byte = read_u8(binary_file)
                    assert byte == 0x00
                    add_byte_to_line(byte)
            
            else:
                raise Exception(f"Unknown command type: {command_type:02X}")

            write_macro_line("    " + ", ".join(line_elements) + ",")
        
        c_file.write("};\n")


def main():
    parser = ArgumentParser(
        description="Converts a binary file containing a material display list to a macro for defining an array of u8 type"
    )
    parser.add_argument("src_path", type=str, help="Binary source file path")
    parser.add_argument("dest_path", type=str, help="Destination C include file path")
    parser.add_argument(
        "--symbol",
        type=str,
        help="Symbol name",
    )
    parser.add_argument(
        "--scope",
        choices=["global", "local"],
        default="local",
        help="The scope of the symbol",
    )

    args = parser.parse_args()
    convert_binary_to_matDL_c_source(args.src_path, args.dest_path, args.symbol, args.scope)


if __name__ == "__main__":
    main()
