#ifndef JMESSAGE_H
#define JMESSAGE_H

#include <dolphin/dolphin.h>

// Struct definitions might be wrong
typedef struct bmg_header_t {
    /* 0x00 */ char magic[8];            // "MESGbmg1"
    /* 0x08 */ u32 size;                 // total size of file not including FLW1/FLI1 sections
    /* 0x0C */ u32 n_sections;           // number of sections
    /* 0x10 */ u8 encoding;              // text encoding
    /* 0x11 */ u8 padding[0x20 - 0x11];  // padding
} bmg_header_t;

typedef struct bmg_section_t {
    /* 0x0 */ u32 magic;  // four character magic string
    /* 0x4 */ u32 size;   // total size of the section
} bmg_section_t;

typedef struct inf1_entry_t {
    /* 0x0 */ u32 string_offset;      // offset into DAT1 section entries, pointing to where message text starts
    /* 0x4 */ // attributes           // attribute data. size fills up the rest of defined INF1 "entry_size"
} inf1_entry_t;

typedef struct inf1_section_t {
    /* 0x00 */ bmg_section_t header;  // section header
    /* 0x08 */ u16 entry_num;         // number of entries in this section
    /* 0x0A */ u16 entry_size;        // size of an entry
    /* 0x0C */ u8 padding[4];         // padding
} inf1_section_t;

typedef struct str1_entry_t {
    char str[0];
} str1_entry_t;

typedef struct str1_section_t {
    /* 0x00 */ bmg_section_t header;  // section header
    /* 0x08 */ str1_entry_t entries[0];
} str1_section_t;

#endif /* JMESSAGE_H */
