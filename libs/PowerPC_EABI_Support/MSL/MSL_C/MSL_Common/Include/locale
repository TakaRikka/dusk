#ifndef _MSL_LOCALE_H
#define _MSL_LOCALE_H

#include <cstddef>

typedef int (*__decode_mbyte) (wchar_t*, const char*,  size_t);
typedef int (*__encode_mbyte) (char*, wchar_t);

struct lconv {
    char* decimal_point;
    char* thousands_sep;
    char* grouping;
    char* mon_decimal_point;
    char* mon_thousands_sep;
    char* mon_grouping;
    char* positive_sign;
    char* negative_sign;
    char* currency_symbol;
    char frac_digits;
    char p_cs_precedes;
    char n_cs_precedes;
    char p_sep_by_space;
    char n_sep_by_space;
    char p_sign_posn;
    char n_sign_posn;
    char* int_curr_symbol;
    char int_frac_digits;
    char int_p_cs_precedes;
    char int_n_cs_precedes;
    char int_p_sep_by_space;
    char int_n_sep_by_space;
    char int_p_sign_posn;
    char int_n_sign_posn;
};

struct _loc_mon_cmpt {
    char CmptName[8];
    char* mon_decimal_point;
    char* mon_thousands_sep;
    char* mon_grouping;
    char* positive_sign;
    char* negative_sign;
    char* currency_symbol;
    char frac_digits;
    char p_cs_precedes;
    char n_cs_precedes;
    char p_sep_by_space;
    char n_sep_by_space;
    char p_sign_posn;
    char n_sign_posn;
    char* int_curr_symbol;
    char int_frac_digits;
    char int_p_cs_precedes; 
    char int_n_cs_precedes; 
    char int_p_sep_by_space; 
    char int_n_sep_by_space; 
    char int_p_sign_posn; 
    char int_n_sign_posn; 
};

struct _loc_num_cmpt {
    char CmptName[8];
    char* decimal_point;
    char* thousands_sep;
    char* grouping;
};

struct _loc_time_cmpt {
    char CmptName[8];
    const char* am_pm;
    const char* DateTime_Format;
    const char* Twelve_hr_format;
    const char* Date_Format;
    const char* Time_Format;
    const char* Day_Names;
    const char* MonthNames;
    char* TimeZone;
};

struct _loc_coll_cmpt {
    char name[8];
    int char_start;
    int char_coll_tab_size;
    short char_spec;
    unsigned short* char_coll_table_ptr;
    unsigned short* wchar_coll_seq_ptr;
};

struct _loc_ctype_cmpt {
    char name[8];                               // 0x0
    const unsigned short* ctype_map_ptr;        // 0x8
    const unsigned char* upper_map_ptr;         // 0xC
    const unsigned char* lower_map_ptr;         // 0x10
    const unsigned short* wctype_map_ptr;       // 0x14
    const wchar_t* wupper_map_ptr;              // 0x18
    const wchar_t* wlower_map_ptr;              // 0x1C
    __decode_mbyte  decode_mb;
    __encode_mbyte  encode_wc;
};

struct __locale {
    struct __locale* next_locale;               // 0x0
    char name[0x30];                            // 0x4
    struct _loc_coll_cmpt* coll_cmpt_ptr;       // 0x34
    struct _loc_ctype_cmpt* ctype_cmpt_ptr;     // 0x38
    struct _loc_mon_cmpt* mon_cmpt_ptr;         // 0x3C
	struct _loc_num_cmpt* num_cmpt_ptr;         // 0x40
	struct _loc_time_cmpt* time_cmpt_ptr;       // 0x44
};

extern struct __locale _current_locale;
extern struct lconv __lconv;

#endif
