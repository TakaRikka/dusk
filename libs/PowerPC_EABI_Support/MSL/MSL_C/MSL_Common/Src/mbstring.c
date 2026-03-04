#include "mbstring.h"
#include <cstring>
#include <locale>

#include "errno.h"
#include "global.h"

extern void __msl_runtime_constraint_violation_s(const char* msg, void* ptr, int error);

static unsigned int is_utf8_complete(const char* buf, unsigned int len) {
    char start_byte;
    int checked_cnt;
    int conts;
    int rem_conts;

    if (len == 0) {
        return -1;
    }
    start_byte = buf[0];
    if (start_byte == 0) {
        return 0;
    }
    if ((start_byte & 0x80) == 0) {
        return 1;
    }
    if ((start_byte & 0xe0) == 0xc0) {
        conts = 1;
    } else if ((start_byte & 0xf0) == 0xe0) {
        conts = 2;
    } else if ((start_byte & 0xf8) == 0xf0) {
        conts = 3;
    } else if ((start_byte & 0xfc) == 0xf8) {
        conts = 4;
    } else if ((start_byte & 0xfe) == 0xfc) {
        conts = 5;
    } else {
        return 0xffffffff;
    }
    checked_cnt = 0;
    for (rem_conts = conts; rem_conts > 0; rem_conts--) {
        if ((buf[checked_cnt + 1] & 0xc0) != 0x80) {
            return 0xffffffff;
        }
        checked_cnt++;
    }
    if (len >= checked_cnt + 1U) {
        return conts + 1;
    }
    return 0xfffffffe;
}

static int __utf8_to_unicode(wchar_t* dst, const char* src, unsigned int len) {
    short byte_cnt;
    int remaining;
    int codepoint;

    codepoint = 0;
    if (src == NULL) {
        return 0;
    }

    if (len == 0) {
        return -1;
    }

    byte_cnt = is_utf8_complete(src, len);
    if (byte_cnt < 0) {
        return -1;
    }
    switch (byte_cnt) {
    case 6:
        codepoint = *src & ((1 << 2) - 1);
        break;
    case 5:
        codepoint = *src & ((1 << 3) - 1);
        break;
    case 4:
        codepoint = *src & ((1 << 4) - 1);
        break;
    case 3:
        codepoint = *src & ((1 << 5) - 1);
        break;
    case 2:
        codepoint = *src & ((1 << 6) - 1);
        break;
    case 1:
        codepoint = *src;
        break;
    }

    for (remaining = byte_cnt - 1; remaining > 0; remaining--) {
        codepoint <<= 6;
        codepoint |= *++src & 0x7f;
    }

    if (codepoint == 0) {
        remaining = 0;
    } else if (codepoint < 0 || codepoint < 0x80) {
        remaining = 1;
    } else if (codepoint < 0x800) {
        remaining = 2;
    } else if (codepoint < 0x10000) {
        remaining = 3;
    } else if (codepoint < 0x200000) {
        remaining = 4;
    } else if (codepoint < 0x4000000) {
        remaining = 5;
    } else {
        remaining = 6;
    }

    if (remaining != byte_cnt) {
        return -1;
    }

    if (dst != NULL) {
        *dst = codepoint;
    }

    return byte_cnt;
}

#if !PLATFORM_GCN
int mbtowc(wchar_t* pwc, const char* s, size_t n) {
    return _current_locale.ctype_cmpt_ptr->decode_mb(pwc, s, n);
}
#endif

inline static int unicode_to_UTF8(char* s, wchar_t wchar) {
    int number_of_bytes;
    wchar_t wide_char;
    char* target_ptr;
    char first_byte_mark[4] = { 0x00, 0x00, 0xc0, 0xe0 };

    if (!s)
        return (0);

    wide_char = wchar;
    if (wide_char < 0x0080)
        number_of_bytes = 1;
    else if (wide_char < 0x0800)
        number_of_bytes = 2;
    else
        number_of_bytes = 3;

    target_ptr = s + number_of_bytes;

    switch (number_of_bytes) {
    case 3:
        *--target_ptr = (wide_char & 0x003f) | 0x80;
        wide_char >>= 6;
    case 2:
        *--target_ptr = (wide_char & 0x003f) | 0x80;
        wide_char >>= 6;
    case 1:
        *--target_ptr = wide_char | first_byte_mark[number_of_bytes];
    }

    return number_of_bytes;
}

int __mbtowc_noconv(wchar_t* pwc, const char* s, size_t n) {
    if (!s)
        return 0;

    if (!n)
        return -1;

    if (pwc)
        *pwc = (unsigned char)*s;

    if (!*s)
        return(0);

    return 1;
}

int __wctomb_noconv(char* s, wchar_t wchar) {
    if (!s)
		return 0;

    *s = wchar;
    return 1;
}

int mbstowcs(wchar_t* param_0, const char* param_1, int param_2) {
    unsigned int i;
    int decoded_cnt;
    int remaining = strlen(param_1);
    if (param_0 != NULL) {
        for (i = 0; i < param_2; i++) {
            if (param_1[0] != '\0') {
                decoded_cnt =
                    _current_locale.ctype_cmpt_ptr->decode_mb(param_0++, param_1, remaining);
                if (decoded_cnt > 0) {
                    param_1 += decoded_cnt;
                    remaining -= decoded_cnt;
                    continue;
                }
                return -1;
            }
            param_0[0] = L'\0';
            break;
        }
    } else {
        i = 0;
    }

    return i;
}

int wctomb(char* s, wchar_t wchar) {
    #if PLATFORM_GCN
    return (unicode_to_UTF8(s, wchar));
    #else
    return _current_locale.ctype_cmpt_ptr->encode_wc(s, wchar);
    #endif
}

size_t wcstombs(char* s, const wchar_t* pwcs, size_t n) {
    int chars_written = 0;
    int result;
    char temp[3];
    wchar_t* source;

    if (!s || !pwcs)
        return (0);

    source = (wchar_t*)pwcs;
    while (chars_written <= n) {
        if (!*source) {
            *(s + chars_written) = '\0';
            break;
        } else {
            result = wctomb(temp, *source++);
            if ((chars_written + result) <= n) {
                strncpy(s + chars_written, temp, result);
                chars_written += result;
            } else
                break;
        }
    }

    return chars_written;
}

int mbsrtowcs_s(size_t* retval, wchar_t* dst, unsigned int dstsz, const char** param_4,
                size_t len, int param_6) {
    wchar_t* var_r27;
    const char* var_r24;
    unsigned int var_r25;
    int var_r26;
    wchar_t sp08[4];

    if (retval == NULL || param_4 == NULL || *param_4 == NULL || param_6 == 0 ||
        (dst != NULL && (len > 0x7fffffff || dstsz > 0x7fffffff || dstsz < len)))
    {
        __msl_runtime_constraint_violation_s(NULL, NULL, 0x22);
        if (retval != NULL) {
            *retval = 0xffffffff;
        }
        return 0x22;
    }

    var_r24 = *param_4;
    var_r26 = strlen(var_r24);
    for (var_r25 = 0; var_r25 < len; var_r25++) {
        if (*var_r24 != '\0') {
            int var_r3_1;

            if (dst == NULL) {
                var_r27 = sp08;
            } else {
                var_r27 = dst++;
            }

            if (var_r24 != NULL) {
                var_r3_1 = is_utf8_complete(var_r24, var_r26);
                if (var_r3_1 < 0xfffffffe) {
                    if (var_r27 != NULL) {
                        var_r3_1 = __utf8_to_unicode(var_r27, var_r24, var_r26);
                    } else if (var_r3_1 == 0xffffffff) {
                        errno = 0x58;
                    }
                }
            } else {
                if (var_r27 != NULL) {
                    *var_r27 = L'\0';
                }
                var_r3_1 = 0;
            }

            if (var_r3_1 <= 0) {
                break;
            }

            var_r24 += var_r3_1;
            var_r26 -= var_r3_1;
        } else {
            if (dst != NULL) {
                *dst = 0;
                *param_4 = 0;
            }
            break;
        }
    }
    *retval = var_r25;
    return 0;
}
