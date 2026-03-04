.include "macros.inc"

.section .init, "ax"  # 0x80003100 - 0x80005600

.global gTRKInterruptVectorTable
gTRKInterruptVectorTable:
.asciz "Metrowerks Target Resident Kernel for PowerPC"
.balign 4
.fill 0xD0

#############################################
# Interrupt vector slot 0x0000 is reserved. #
#############################################

# Slot 0x0100: System Reset Exception
    b __TRK_reset
.fill 0xFC

# Slot 0x0200: Machine Check Exception
/* 80003354 00000354  7C 51 43 A6 */	mtspr 0x111, r2
/* 80003358 00000358  7C 5A 02 A6 */	mfspr r2, 0x1a
/* 8000335C 0000035C  7C 00 17 AC */	icbi 0, r2
/* 80003360 00000360  7C 53 02 A6 */	mfdar r2
/* 80003364 00000364  7C 00 13 AC */	dcbi 0, r2
/* 80003368 00000368  7C 51 42 A6 */	mfspr r2, 0x111
/* 8000336C 0000036C  7C 51 43 A6 */	mtspr 0x111, r2
/* 80003370 00000370  7C 72 43 A6 */	mtspr 0x112, r3
/* 80003374 00000374  7C 93 43 A6 */	mtspr 0x113, r4
/* 80003378 00000378  7C 5A 02 A6 */	mfspr r2, 0x1a
/* 8000337C 0000037C  7C 9B 02 A6 */	mfspr r4, 0x1b
/* 80003380 00000380  7C 60 00 A6 */	mfmsr r3
/* 80003384 00000384  60 63 00 30 */	ori r3, r3, 0x30
/* 80003388 00000388  7C 7B 03 A6 */	mtspr 0x1b, r3
/* 8000338C 0000038C  3C 60 80 0B */	lis r3, TRKInterruptHandler@h
/* 80003390 00000390  60 63 DE F4 */	ori r3, r3, TRKInterruptHandler@l
/* 80003394 00000394  7C 7A 03 A6 */	mtspr 0x1a, r3
/* 80003398 00000398  38 60 02 00 */	li r3, 0x200
/* 8000339C 0000039C  4C 00 00 64 */	rfi 
.fill 0xB4

# Slot 0x0300: DSI Exception
/* 80003454 00000454  7C 51 43 A6 */	mtspr 0x111, r2
/* 80003458 00000458  7C 72 43 A6 */	mtspr 0x112, r3
/* 8000345C 0000045C  7C 93 43 A6 */	mtspr 0x113, r4
/* 80003460 00000460  7C 5A 02 A6 */	mfspr r2, 0x1a
/* 80003464 00000464  7C 9B 02 A6 */	mfspr r4, 0x1b
/* 80003468 00000468  7C 60 00 A6 */	mfmsr r3
/* 8000346C 0000046C  60 63 00 30 */	ori r3, r3, 0x30
/* 80003470 00000470  7C 7B 03 A6 */	mtspr 0x1b, r3
/* 80003474 00000474  3C 60 80 0B */	lis r3, TRKInterruptHandler@h
/* 80003478 00000478  60 63 DE F4 */	ori r3, r3, TRKInterruptHandler@l
/* 8000347C 0000047C  7C 7A 03 A6 */	mtspr 0x1a, r3
/* 80003480 00000480  38 60 03 00 */	li r3, 0x300
/* 80003484 00000484  4C 00 00 64 */	rfi 
.fill 0xCC

# Slot 0x0400: ISI Exception
/* 80003554 00000554  7C 51 43 A6 */	mtspr 0x111, r2
/* 80003558 00000558  7C 72 43 A6 */	mtspr 0x112, r3
/* 8000355C 0000055C  7C 93 43 A6 */	mtspr 0x113, r4
/* 80003560 00000560  7C 5A 02 A6 */	mfspr r2, 0x1a
/* 80003564 00000564  7C 9B 02 A6 */	mfspr r4, 0x1b
/* 80003568 00000568  7C 60 00 A6 */	mfmsr r3
/* 8000356C 0000056C  60 63 00 30 */	ori r3, r3, 0x30
/* 80003570 00000570  7C 7B 03 A6 */	mtspr 0x1b, r3
/* 80003574 00000574  3C 60 80 0B */	lis r3, TRKInterruptHandler@h
/* 80003578 00000578  60 63 DE F4 */	ori r3, r3, TRKInterruptHandler@l
/* 8000357C 0000057C  7C 7A 03 A6 */	mtspr 0x1a, r3
/* 80003580 00000580  38 60 04 00 */	li r3, 0x400
/* 80003584 00000584  4C 00 00 64 */	rfi 
.fill 0xCC

# Slot 0x0500: External Interrupt Exception
/* 80003654 00000654  7C 51 43 A6 */	mtspr 0x111, r2
/* 80003658 00000658  7C 72 43 A6 */	mtspr 0x112, r3
/* 8000365C 0000065C  7C 93 43 A6 */	mtspr 0x113, r4
/* 80003660 00000660  7C 5A 02 A6 */	mfspr r2, 0x1a
/* 80003664 00000664  7C 9B 02 A6 */	mfspr r4, 0x1b
/* 80003668 00000668  7C 60 00 A6 */	mfmsr r3
/* 8000366C 0000066C  60 63 00 30 */	ori r3, r3, 0x30
/* 80003670 00000670  7C 7B 03 A6 */	mtspr 0x1b, r3
/* 80003674 00000674  3C 60 80 0B */	lis r3, TRKInterruptHandler@h
/* 80003678 00000678  60 63 DE F4 */	ori r3, r3, TRKInterruptHandler@l
/* 8000367C 0000067C  7C 7A 03 A6 */	mtspr 0x1a, r3
/* 80003680 00000680  38 60 05 00 */	li r3, 0x500
/* 80003684 00000684  4C 00 00 64 */	rfi 
.fill 0xCC

# Slot 0x0600: Alignment Exception
/* 80003754 00000754  7C 51 43 A6 */	mtspr 0x111, r2
/* 80003758 00000758  7C 72 43 A6 */	mtspr 0x112, r3
/* 8000375C 0000075C  7C 93 43 A6 */	mtspr 0x113, r4
/* 80003760 00000760  7C 5A 02 A6 */	mfspr r2, 0x1a
/* 80003764 00000764  7C 9B 02 A6 */	mfspr r4, 0x1b
/* 80003768 00000768  7C 60 00 A6 */	mfmsr r3
/* 8000376C 0000076C  60 63 00 30 */	ori r3, r3, 0x30
/* 80003770 00000770  7C 7B 03 A6 */	mtspr 0x1b, r3
/* 80003774 00000774  3C 60 80 0B */	lis r3, TRKInterruptHandler@h
/* 80003778 00000778  60 63 DE F4 */	ori r3, r3, TRKInterruptHandler@l
/* 8000377C 0000077C  7C 7A 03 A6 */	mtspr 0x1a, r3
/* 80003780 00000780  38 60 06 00 */	li r3, 0x600
/* 80003784 00000784  4C 00 00 64 */	rfi 
.fill 0xCC

# Slot 0x0700: Program Exception
/* 80003854 00000854  7C 51 43 A6 */	mtspr 0x111, r2
/* 80003858 00000858  7C 72 43 A6 */	mtspr 0x112, r3
/* 8000385C 0000085C  7C 93 43 A6 */	mtspr 0x113, r4
/* 80003860 00000860  7C 5A 02 A6 */	mfspr r2, 0x1a
/* 80003864 00000864  7C 9B 02 A6 */	mfspr r4, 0x1b
/* 80003868 00000868  7C 60 00 A6 */	mfmsr r3
/* 8000386C 0000086C  60 63 00 30 */	ori r3, r3, 0x30
/* 80003870 00000870  7C 7B 03 A6 */	mtspr 0x1b, r3
/* 80003874 00000874  3C 60 80 0B */	lis r3, TRKInterruptHandler@h
/* 80003878 00000878  60 63 DE F4 */	ori r3, r3, TRKInterruptHandler@l
/* 8000387C 0000087C  7C 7A 03 A6 */	mtspr 0x1a, r3
/* 80003880 00000880  38 60 07 00 */	li r3, 0x700
/* 80003884 00000884  4C 00 00 64 */	rfi 
.fill 0xCC

# Slot 0x0800: Floating Point Unavailable Exception
/* 80003954 00000954  7C 51 43 A6 */	mtspr 0x111, r2
/* 80003958 00000958  7C 72 43 A6 */	mtspr 0x112, r3
/* 8000395C 0000095C  7C 93 43 A6 */	mtspr 0x113, r4
/* 80003960 00000960  7C 5A 02 A6 */	mfspr r2, 0x1a
/* 80003964 00000964  7C 9B 02 A6 */	mfspr r4, 0x1b
/* 80003968 00000968  7C 60 00 A6 */	mfmsr r3
/* 8000396C 0000096C  60 63 00 30 */	ori r3, r3, 0x30
/* 80003970 00000970  7C 7B 03 A6 */	mtspr 0x1b, r3
/* 80003974 00000974  3C 60 80 0B */	lis r3, TRKInterruptHandler@h
/* 80003978 00000978  60 63 DE F4 */	ori r3, r3, TRKInterruptHandler@l
/* 8000397C 0000097C  7C 7A 03 A6 */	mtspr 0x1a, r3
/* 80003980 00000980  38 60 08 00 */	li r3, 0x800
/* 80003984 00000984  4C 00 00 64 */	rfi 
.fill 0xCC

# Slot 0x0900: Decrementer Exception
/* 80003A54 00000A54  7C 51 43 A6 */	mtspr 0x111, r2
/* 80003A58 00000A58  7C 72 43 A6 */	mtspr 0x112, r3
/* 80003A5C 00000A5C  7C 93 43 A6 */	mtspr 0x113, r4
/* 80003A60 00000A60  7C 5A 02 A6 */	mfspr r2, 0x1a
/* 80003A64 00000A64  7C 9B 02 A6 */	mfspr r4, 0x1b
/* 80003A68 00000A68  7C 60 00 A6 */	mfmsr r3
/* 80003A6C 00000A6C  60 63 00 30 */	ori r3, r3, 0x30
/* 80003A70 00000A70  7C 7B 03 A6 */	mtspr 0x1b, r3
/* 80003A74 00000A74  3C 60 80 0B */	lis r3, TRKInterruptHandler@h
/* 80003A78 00000A78  60 63 DE F4 */	ori r3, r3, TRKInterruptHandler@l
/* 80003A7C 00000A7C  7C 7A 03 A6 */	mtspr 0x1a, r3
/* 80003A80 00000A80  38 60 09 00 */	li r3, 0x900
/* 80003A84 00000A84  4C 00 00 64 */	rfi 
.fill 0xCC

######################################################
# Interrupt vector slots 0x0A00 & 0x0B00 are reserved.
.fill 0x100 
.fill 0x100 
######################################################

# Slot 0x0C00: System Call Exception
/* 80003D54 00000D54  7C 51 43 A6 */	mtspr 0x111, r2
/* 80003D58 00000D58  7C 72 43 A6 */	mtspr 0x112, r3
/* 80003D5C 00000D5C  7C 93 43 A6 */	mtspr 0x113, r4
/* 80003D60 00000D60  7C 5A 02 A6 */	mfspr r2, 0x1a
/* 80003D64 00000D64  7C 9B 02 A6 */	mfspr r4, 0x1b
/* 80003D68 00000D68  7C 60 00 A6 */	mfmsr r3
/* 80003D6C 00000D6C  60 63 00 30 */	ori r3, r3, 0x30
/* 80003D70 00000D70  7C 7B 03 A6 */	mtspr 0x1b, r3
/* 80003D74 00000D74  3C 60 80 0B */	lis r3, TRKInterruptHandler@h
/* 80003D78 00000D78  60 63 DE F4 */	ori r3, r3, TRKInterruptHandler@l
/* 80003D7C 00000D7C  7C 7A 03 A6 */	mtspr 0x1a, r3
/* 80003D80 00000D80  38 60 0C 00 */	li r3, 0xc00
/* 80003D84 00000D84  4C 00 00 64 */	rfi 
.fill 0xCC

# Slot 0x0D00: Trace Exception
/* 80003E54 00000E54  7C 51 43 A6 */	mtspr 0x111, r2
/* 80003E58 00000E58  7C 72 43 A6 */	mtspr 0x112, r3
/* 80003E5C 00000E5C  7C 93 43 A6 */	mtspr 0x113, r4
/* 80003E60 00000E60  7C 5A 02 A6 */	mfspr r2, 0x1a
/* 80003E64 00000E64  7C 9B 02 A6 */	mfspr r4, 0x1b
/* 80003E68 00000E68  7C 60 00 A6 */	mfmsr r3
/* 80003E6C 00000E6C  60 63 00 30 */	ori r3, r3, 0x30
/* 80003E70 00000E70  7C 7B 03 A6 */	mtspr 0x1b, r3
/* 80003E74 00000E74  3C 60 80 0B */	lis r3, TRKInterruptHandler@h
/* 80003E78 00000E78  60 63 DE F4 */	ori r3, r3, TRKInterruptHandler@l
/* 80003E7C 00000E7C  7C 7A 03 A6 */	mtspr 0x1a, r3
/* 80003E80 00000E80  38 60 0D 00 */	li r3, 0xd00
/* 80003E84 00000E84  4C 00 00 64 */	rfi 
.fill 0xCC

############################################################################
# Slot 0x0E00 is usually for the Floating Point Assist Exception Handler,  #
# however that exception is not implemented in the PPC 750CL architecture. #
############################################################################

# Slot 0x0F00: Performance Monitor Exception
/* 80003F54 00000F54  7C 51 43 A6 */	mtspr 0x111, r2
/* 80003F58 00000F58  7C 72 43 A6 */	mtspr 0x112, r3
/* 80003F5C 00000F5C  7C 93 43 A6 */	mtspr 0x113, r4
/* 80003F60 00000F60  7C 5A 02 A6 */	mfspr r2, 0x1a
/* 80003F64 00000F64  7C 9B 02 A6 */	mfspr r4, 0x1b
/* 80003F68 00000F68  7C 60 00 A6 */	mfmsr r3
/* 80003F6C 00000F6C  60 63 00 30 */	ori r3, r3, 0x30
/* 80003F70 00000F70  7C 7B 03 A6 */	mtspr 0x1b, r3
/* 80003F74 00000F74  3C 60 80 0B */	lis r3, TRKInterruptHandler@h
/* 80003F78 00000F78  60 63 DE F4 */	ori r3, r3, TRKInterruptHandler@l
/* 80003F7C 00000F7C  7C 7A 03 A6 */	mtspr 0x1a, r3
/* 80003F80 00000F80  38 60 0E 00 */	li r3, 0xe00
/* 80003F84 00000F84  4C 00 00 64 */	rfi 
.fill 0xCC

##################################################################################
# Interrupt vector slots 0x1000 through 0x1200 are not implemented in the 750CL. #
##################################################################################

# Slot 0x1300: Instruction Address Breakpoint Exception
/* 80004054 00001054  48 00 00 54 */	b .L_800040A8
.fill 0x1C
/* 80004074 00001074  7C 51 43 A6 */	mtspr 0x111, r2
/* 80004078 00001078  7C 72 43 A6 */	mtspr 0x112, r3
/* 8000407C 0000107C  7C 93 43 A6 */	mtspr 0x113, r4
/* 80004080 00001080  7C 5A 02 A6 */	mfspr r2, 0x1a
/* 80004084 00001084  7C 9B 02 A6 */	mfspr r4, 0x1b
/* 80004088 00001088  7C 60 00 A6 */	mfmsr r3
/* 8000408C 0000108C  60 63 00 30 */	ori r3, r3, 0x30
/* 80004090 00001090  7C 7B 03 A6 */	mtspr 0x1b, r3
/* 80004094 00001094  3C 60 80 0B */	lis r3, TRKInterruptHandler@h
/* 80004098 00001098  60 63 DE F4 */	ori r3, r3, TRKInterruptHandler@l
/* 8000409C 0000109C  7C 7A 03 A6 */	mtspr 0x1a, r3
/* 800040A0 000010A0  38 60 0F 20 */	li r3, 0xf20
/* 800040A4 000010A4  4C 00 00 64 */	rfi 
.L_800040A8:
/* 800040A8 000010A8  7C 51 43 A6 */	mtspr 0x111, r2
/* 800040AC 000010AC  7C 72 43 A6 */	mtspr 0x112, r3
/* 800040B0 000010B0  7C 93 43 A6 */	mtspr 0x113, r4
/* 800040B4 000010B4  7C 5A 02 A6 */	mfspr r2, 0x1a
/* 800040B8 000010B8  7C 9B 02 A6 */	mfspr r4, 0x1b
/* 800040BC 000010BC  7C 60 00 A6 */	mfmsr r3
/* 800040C0 000010C0  60 63 00 30 */	ori r3, r3, 0x30
/* 800040C4 000010C4  7C 7B 03 A6 */	mtspr 0x1b, r3
/* 800040C8 000010C8  3C 60 80 0B */	lis r3, TRKInterruptHandler@h
/* 800040CC 000010CC  60 63 DE F4 */	ori r3, r3, TRKInterruptHandler@l
/* 800040D0 000010D0  7C 7A 03 A6 */	mtspr 0x1a, r3
/* 800040D4 000010D4  38 60 0F 00 */	li r3, 0xf00
/* 800040D8 000010D8  4C 00 00 64 */	rfi 
.fill 0x78

# Slot 0x1400: System Management Interrupt Exception
/* 80004154 00001154  7C 51 43 A6 */	mtspr 0x111, r2
/* 80004158 00001158  7C 40 00 26 */	mfcr r2
/* 8000415C 0000115C  7C 52 43 A6 */	mtspr 0x112, r2
/* 80004160 00001160  7C 40 00 A6 */	mfmsr r2
/* 80004164 00001164  74 42 00 02 */	andis. r2, r2, 2
/* 80004168 00001168  41 82 00 1C */	beq .L_80004184
/* 8000416C 0000116C  7C 40 00 A6 */	mfmsr r2
/* 80004170 00001170  6C 42 00 02 */	xoris r2, r2, 2
/* 80004174 00001174  7C 00 04 AC */	sync 0
/* 80004178 00001178  7C 40 01 24 */	mtmsr r2
/* 8000417C 0000117C  7C 00 04 AC */	sync 0
/* 80004180 00001180  7C 51 43 A6 */	mtspr 0x111, r2
.L_80004184:
/* 80004184 00001184  7C 52 42 A6 */	mfspr r2, 0x112
/* 80004188 00001188  7C 4F F1 20 */	mtcrf 0xff, r2
/* 8000418C 0000118C  7C 51 42 A6 */	mfspr r2, 0x111
/* 80004190 00001190  7C 51 43 A6 */	mtspr 0x111, r2
/* 80004194 00001194  7C 72 43 A6 */	mtspr 0x112, r3
/* 80004198 00001198  7C 93 43 A6 */	mtspr 0x113, r4
/* 8000419C 0000119C  7C 5A 02 A6 */	mfspr r2, 0x1a
/* 800041A0 000011A0  7C 9B 02 A6 */	mfspr r4, 0x1b
/* 800041A4 000011A4  7C 60 00 A6 */	mfmsr r3
/* 800041A8 000011A8  60 63 00 30 */	ori r3, r3, 0x30
/* 800041AC 000011AC  7C 7B 03 A6 */	mtspr 0x1b, r3
/* 800041B0 000011B0  3C 60 80 0B */	lis r3, TRKInterruptHandler@h
/* 800041B4 000011B4  60 63 DE F4 */	ori r3, r3, TRKInterruptHandler@l
/* 800041B8 000011B8  7C 7A 03 A6 */	mtspr 0x1a, r3
/* 800041BC 000011BC  38 60 10 00 */	li r3, 0x1000
/* 800041C0 000011C0  4C 00 00 64 */	rfi 
.fill 0x90


##############################################################################
# Interrupt vector slots 0x1500 and 0x1600 are not implemented in the 750CL. #
##############################################################################

# Slot 0x1700: Thermal-Management Interrupt Exception
/* 80004254 00001254  7C 51 43 A6 */	mtspr 0x111, r2
/* 80004258 00001258  7C 40 00 26 */	mfcr r2
/* 8000425C 0000125C  7C 52 43 A6 */	mtspr 0x112, r2
/* 80004260 00001260  7C 40 00 A6 */	mfmsr r2
/* 80004264 00001264  74 42 00 02 */	andis. r2, r2, 2
/* 80004268 00001268  41 82 00 1C */	beq .L_80004284
/* 8000426C 0000126C  7C 40 00 A6 */	mfmsr r2
/* 80004270 00001270  6C 42 00 02 */	xoris r2, r2, 2
/* 80004274 00001274  7C 00 04 AC */	sync 0
/* 80004278 00001278  7C 40 01 24 */	mtmsr r2
/* 8000427C 0000127C  7C 00 04 AC */	sync 0
/* 80004280 00001280  7C 51 43 A6 */	mtspr 0x111, r2
.L_80004284:
/* 80004284 00001284  7C 52 42 A6 */	mfspr r2, 0x112
/* 80004288 00001288  7C 4F F1 20 */	mtcrf 0xff, r2
/* 8000428C 0000128C  7C 51 42 A6 */	mfspr r2, 0x111
/* 80004290 00001290  7C 51 43 A6 */	mtspr 0x111, r2
/* 80004294 00001294  7C 72 43 A6 */	mtspr 0x112, r3
/* 80004298 00001298  7C 93 43 A6 */	mtspr 0x113, r4
/* 8000429C 0000129C  7C 5A 02 A6 */	mfspr r2, 0x1a
/* 800042A0 000012A0  7C 9B 02 A6 */	mfspr r4, 0x1b
/* 800042A4 000012A4  7C 60 00 A6 */	mfmsr r3
/* 800042A8 000012A8  60 63 00 30 */	ori r3, r3, 0x30
/* 800042AC 000012AC  7C 7B 03 A6 */	mtspr 0x1b, r3
/* 800042B0 000012B0  3C 60 80 0B */	lis r3, TRKInterruptHandler@h
/* 800042B4 000012B4  60 63 DE F4 */	ori r3, r3, TRKInterruptHandler@l
/* 800042B8 000012B8  7C 7A 03 A6 */	mtspr 0x1a, r3
/* 800042BC 000012BC  38 60 11 00 */	li r3, 0x1100
/* 800042C0 000012C0  4C 00 00 64 */	rfi 
.fill 0x90

# Slot 0x1800(?)
/* 80004354 00001354  7C 51 43 A6 */	mtspr 0x111, r2
/* 80004358 00001358  7C 40 00 26 */	mfcr r2
/* 8000435C 0000135C  7C 52 43 A6 */	mtspr 0x112, r2
/* 80004360 00001360  7C 40 00 A6 */	mfmsr r2
/* 80004364 00001364  74 42 00 02 */	andis. r2, r2, 2
/* 80004368 00001368  41 82 00 1C */	beq .L_80004384
/* 8000436C 0000136C  7C 40 00 A6 */	mfmsr r2
/* 80004370 00001370  6C 42 00 02 */	xoris r2, r2, 2
/* 80004374 00001374  7C 00 04 AC */	sync 0
/* 80004378 00001378  7C 40 01 24 */	mtmsr r2
/* 8000437C 0000137C  7C 00 04 AC */	sync 0
/* 80004380 00001380  7C 51 43 A6 */	mtspr 0x111, r2
.L_80004384:
/* 80004384 00001384  7C 52 42 A6 */	mfspr r2, 0x112
/* 80004388 00001388  7C 4F F1 20 */	mtcrf 0xff, r2
/* 8000438C 0000138C  7C 51 42 A6 */	mfspr r2, 0x111
/* 80004390 00001390  7C 51 43 A6 */	mtspr 0x111, r2
/* 80004394 00001394  7C 72 43 A6 */	mtspr 0x112, r3
/* 80004398 00001398  7C 93 43 A6 */	mtspr 0x113, r4
/* 8000439C 0000139C  7C 5A 02 A6 */	mfspr r2, 0x1a
/* 800043A0 000013A0  7C 9B 02 A6 */	mfspr r4, 0x1b
/* 800043A4 000013A4  7C 60 00 A6 */	mfmsr r3
/* 800043A8 000013A8  60 63 00 30 */	ori r3, r3, 0x30
/* 800043AC 000013AC  7C 7B 03 A6 */	mtspr 0x1b, r3
/* 800043B0 000013B0  3C 60 80 0B */	lis r3, TRKInterruptHandler@h
/* 800043B4 000013B4  60 63 DE F4 */	ori r3, r3, TRKInterruptHandler@l
/* 800043B8 000013B8  7C 7A 03 A6 */	mtspr 0x1a, r3
/* 800043BC 000013BC  38 60 12 00 */	li r3, 0x1200
/* 800043C0 000013C0  4C 00 00 64 */	rfi 
.fill 0x90

# Slot 0x1900(?)
/* 80004454 00001454  7C 51 43 A6 */	mtspr 0x111, r2
/* 80004458 00001458  7C 72 43 A6 */	mtspr 0x112, r3
/* 8000445C 0000145C  7C 93 43 A6 */	mtspr 0x113, r4
/* 80004460 00001460  7C 5A 02 A6 */	mfspr r2, 0x1a
/* 80004464 00001464  7C 9B 02 A6 */	mfspr r4, 0x1b
/* 80004468 00001468  7C 60 00 A6 */	mfmsr r3
/* 8000446C 0000146C  60 63 00 30 */	ori r3, r3, 0x30
/* 80004470 00001470  7C 7B 03 A6 */	mtspr 0x1b, r3
/* 80004474 00001474  3C 60 80 0B */	lis r3, TRKInterruptHandler@h
/* 80004478 00001478  60 63 DE F4 */	ori r3, r3, TRKInterruptHandler@l
/* 8000447C 0000147C  7C 7A 03 A6 */	mtspr 0x1a, r3
/* 80004480 00001480  38 60 13 00 */	li r3, 0x1300
/* 80004484 00001484  4C 00 00 64 */	rfi 
.fill 0xCC

# Slot 0x1A00(?)
/* 80004554 00001554  7C 51 43 A6 */	mtspr 0x111, r2
/* 80004558 00001558  7C 72 43 A6 */	mtspr 0x112, r3
/* 8000455C 0000155C  7C 93 43 A6 */	mtspr 0x113, r4
/* 80004560 00001560  7C 5A 02 A6 */	mfspr r2, 0x1a
/* 80004564 00001564  7C 9B 02 A6 */	mfspr r4, 0x1b
/* 80004568 00001568  7C 60 00 A6 */	mfmsr r3
/* 8000456C 0000156C  60 63 00 30 */	ori r3, r3, 0x30
/* 80004570 00001570  7C 7B 03 A6 */	mtspr 0x1b, r3
/* 80004574 00001574  3C 60 80 0B */	lis r3, TRKInterruptHandler@h
/* 80004578 00001578  60 63 DE F4 */	ori r3, r3, TRKInterruptHandler@l
/* 8000457C 0000157C  7C 7A 03 A6 */	mtspr 0x1a, r3
/* 80004580 00001580  38 60 14 00 */	li r3, 0x1400
/* 80004584 00001584  4C 00 00 64 */	rfi 
.fill 0x1CC

# Slot 0x1B00(?)
/* 80004754 00001754  7C 51 43 A6 */	mtspr 0x111, r2
/* 80004758 00001758  7C 72 43 A6 */	mtspr 0x112, r3
/* 8000475C 0000175C  7C 93 43 A6 */	mtspr 0x113, r4
/* 80004760 00001760  7C 5A 02 A6 */	mfspr r2, 0x1a
/* 80004764 00001764  7C 9B 02 A6 */	mfspr r4, 0x1b
/* 80004768 00001768  7C 60 00 A6 */	mfmsr r3
/* 8000476C 0000176C  60 63 00 30 */	ori r3, r3, 0x30
/* 80004770 00001770  7C 7B 03 A6 */	mtspr 0x1b, r3
/* 80004774 00001774  3C 60 80 0B */	lis r3, TRKInterruptHandler@h
/* 80004778 00001778  60 63 DE F4 */	ori r3, r3, TRKInterruptHandler@l
/* 8000477C 0000177C  7C 7A 03 A6 */	mtspr 0x1a, r3
/* 80004780 00001780  38 60 16 00 */	li r3, 0x1600
/* 80004784 00001784  4C 00 00 64 */	rfi 
.fill 0xCC

# Slot 0x1C00(?)
/* 80004854 00001854  7C 51 43 A6 */	mtspr 0x111, r2
/* 80004858 00001858  7C 72 43 A6 */	mtspr 0x112, r3
/* 8000485C 0000185C  7C 93 43 A6 */	mtspr 0x113, r4
/* 80004860 00001860  7C 5A 02 A6 */	mfspr r2, 0x1a
/* 80004864 00001864  7C 9B 02 A6 */	mfspr r4, 0x1b
/* 80004868 00001868  7C 60 00 A6 */	mfmsr r3
/* 8000486C 0000186C  60 63 00 30 */	ori r3, r3, 0x30
/* 80004870 00001870  7C 7B 03 A6 */	mtspr 0x1b, r3
/* 80004874 00001874  3C 60 80 0B */	lis r3, TRKInterruptHandler@h
/* 80004878 00001878  60 63 DE F4 */	ori r3, r3, TRKInterruptHandler@l
/* 8000487C 0000187C  7C 7A 03 A6 */	mtspr 0x1a, r3
/* 80004880 00001880  38 60 17 00 */	li r3, 0x1700
/* 80004884 00001884  4C 00 00 64 */	rfi 
.fill 0x4CC

# Slot 0x1D00(?)
/* 80004D54 00001D54  7C 51 43 A6 */	mtspr 0x111, r2
/* 80004D58 00001D58  7C 72 43 A6 */	mtspr 0x112, r3
/* 80004D5C 00001D5C  7C 93 43 A6 */	mtspr 0x113, r4
/* 80004D60 00001D60  7C 5A 02 A6 */	mfspr r2, 0x1a
/* 80004D64 00001D64  7C 9B 02 A6 */	mfspr r4, 0x1b
/* 80004D68 00001D68  7C 60 00 A6 */	mfmsr r3
/* 80004D6C 00001D6C  60 63 00 30 */	ori r3, r3, 0x30
/* 80004D70 00001D70  7C 7B 03 A6 */	mtspr 0x1b, r3
/* 80004D74 00001D74  3C 60 80 0B */	lis r3, TRKInterruptHandler@h
/* 80004D78 00001D78  60 63 DE F4 */	ori r3, r3, TRKInterruptHandler@l
/* 80004D7C 00001D7C  7C 7A 03 A6 */	mtspr 0x1a, r3
/* 80004D80 00001D80  38 60 1C 00 */	li r3, 0x1c00
/* 80004D84 00001D84  4C 00 00 64 */	rfi 
.fill 0xCC

# Slot 0x1E00(?)
/* 80004E54 00001E54  7C 51 43 A6 */	mtspr 0x111, r2
/* 80004E58 00001E58  7C 72 43 A6 */	mtspr 0x112, r3
/* 80004E5C 00001E5C  7C 93 43 A6 */	mtspr 0x113, r4
/* 80004E60 00001E60  7C 5A 02 A6 */	mfspr r2, 0x1a
/* 80004E64 00001E64  7C 9B 02 A6 */	mfspr r4, 0x1b
/* 80004E68 00001E68  7C 60 00 A6 */	mfmsr r3
/* 80004E6C 00001E6C  60 63 00 30 */	ori r3, r3, 0x30
/* 80004E70 00001E70  7C 7B 03 A6 */	mtspr 0x1b, r3
/* 80004E74 00001E74  3C 60 80 0B */	lis r3, TRKInterruptHandler@h
/* 80004E78 00001E78  60 63 DE F4 */	ori r3, r3, TRKInterruptHandler@l
/* 80004E7C 00001E7C  7C 7A 03 A6 */	mtspr 0x1a, r3
/* 80004E80 00001E80  38 60 1D 00 */	li r3, 0x1d00
/* 80004E84 00001E84  4C 00 00 64 */	rfi 
.fill 0xCC

# Slot 0x1F00(?)
/* 80004F54 00001F54  7C 51 43 A6 */	mtspr 0x111, r2
/* 80004F58 00001F58  7C 72 43 A6 */	mtspr 0x112, r3
/* 80004F5C 00001F5C  7C 93 43 A6 */	mtspr 0x113, r4
/* 80004F60 00001F60  7C 5A 02 A6 */	mfspr r2, 0x1a
/* 80004F64 00001F64  7C 9B 02 A6 */	mfspr r4, 0x1b
/* 80004F68 00001F68  7C 60 00 A6 */	mfmsr r3
/* 80004F6C 00001F6C  60 63 00 30 */	ori r3, r3, 0x30
/* 80004F70 00001F70  7C 7B 03 A6 */	mtspr 0x1b, r3
/* 80004F74 00001F74  3C 60 80 0B */	lis r3, TRKInterruptHandler@h
/* 80004F78 00001F78  60 63 DE F4 */	ori r3, r3, TRKInterruptHandler@l
/* 80004F7C 00001F7C  7C 7A 03 A6 */	mtspr 0x1a, r3
/* 80004F80 00001F80  38 60 1E 00 */	li r3, 0x1e00
/* 80004F84 00001F84  4C 00 00 64 */	rfi 
.fill 0xCC

# Slot 0x2000(?)
/* 80005054 00002054  7C 51 43 A6 */	mtspr 0x111, r2
/* 80005058 00002058  7C 72 43 A6 */	mtspr 0x112, r3
/* 8000505C 0000205C  7C 93 43 A6 */	mtspr 0x113, r4
/* 80005060 00002060  7C 5A 02 A6 */	mfspr r2, 0x1a
/* 80005064 00002064  7C 9B 02 A6 */	mfspr r4, 0x1b
/* 80005068 00002068  7C 60 00 A6 */	mfmsr r3
/* 8000506C 0000206C  60 63 00 30 */	ori r3, r3, 0x30
/* 80005070 00002070  7C 7B 03 A6 */	mtspr 0x1b, r3
/* 80005074 00002074  3C 60 80 0B */	lis r3, TRKInterruptHandler@h
/* 80005078 00002078  60 63 DE F4 */	ori r3, r3, TRKInterruptHandler@l
/* 8000507C 0000207C  7C 7A 03 A6 */	mtspr 0x1a, r3
/* 80005080 00002080  38 60 1F 00 */	li r3, 0x1f00
/* 80005084 00002084  4C 00 00 64 */	rfi 
.global gTRKInterruptVectorTableEnd
gTRKInterruptVectorTableEnd:
