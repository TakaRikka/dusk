.include "macros.inc"
# .file "targsupp.s"

.text
.balign 16

.fn TRKAccessFile, global
    twui r0, 0x0
    blr
.endfn TRKAccessFile

.fn TRKOpenFile, global
    twui r0, 0x0
    blr
.endfn TRKOpenFile

.fn TRKCloseFile, global
    twui r0, 0x0
    blr
.endfn TRKCloseFile

.fn TRKPositionFile, global
    twui r0, 0x0
    blr
.endfn TRKPositionFile
