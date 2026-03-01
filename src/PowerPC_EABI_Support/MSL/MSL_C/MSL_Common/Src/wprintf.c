#include <wprintf.h>

#include <ansi_fp.h>
#include <mbstring.h>
#include <wmem.h>
#include <wscanf.h>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <locale>
#include <cstdarg>
#include <cstddef>
#include <va_list>

#define LDBL_MANT_DIG 53

#define TARGET_FLOAT_BITS 64
#define TARGET_FLOAT_BYTES (TARGET_FLOAT_BITS / 8)
#define TARGET_FLOAT_MAX_EXP LDBL_MAX_EXP
#define TARGET_FLOAT_MANT_DIG LDBL_MANT_DIG
#define TARGET_FLOAT_IMPLICIT_J_BIT 1
#define TARGET_FLOAT_MANT_BITS (TARGET_FLOAT_MANT_DIG - TARGET_FLOAT_IMPLICIT_J_BIT)
#define TARGET_FLOAT_EXP_BITS (TARGET_FLOAT_BITS - TARGET_FLOAT_MANT_BITS - 1)

enum justification_options { left_justification, right_justification, zero_fill };

enum sign_options { only_minus, sign_always, space_holder };

enum argument_options {
    normal_argument,
    char_argument,
    short_argument,
    long_argument,
    long_long_argument,
    wchar_argument,

    intmax_argument,
    size_t_argument,
    ptrdiff_argument,

    long_double_argument,
};

typedef struct {
    unsigned char justification_options;
    unsigned char sign_options;
    unsigned char precision_specified;
    unsigned char alternate_form;
    unsigned char argument_options;
    wchar_t conversion_char;
    int field_width;
    int precision;
} print_format;

extern void __msl_runtime_constraint_violation_s(const char* msg, void* ptr, int error);

const wchar_t* parse_format(const wchar_t* format_string, va_list* arg, print_format* format) {
    print_format f;
    const wchar_t* s = format_string;
    wchar_t c;
    int flag_found;
    f.justification_options = right_justification;
    f.sign_options = only_minus;
    f.precision_specified = 0;
    f.alternate_form = 0;
    f.argument_options = normal_argument;
    f.field_width = 0;
    f.precision = 0;

    if ((c = *++s) == '%') {
        f.conversion_char = c;
        *format = f;
        return s + 1;
    }

    while (1) {
        flag_found = 1;

        switch (c) {
        case '-':
            f.justification_options = left_justification;
            break;
        case '+':
            f.sign_options = sign_always;
            break;
        case ' ':
            if (f.sign_options != sign_always) {
                f.sign_options = space_holder;
            }
            break;
        case '#':
            f.alternate_form = 1;
            break;
        case '0':
            if (f.justification_options != left_justification) {
                f.justification_options = zero_fill;
            }
            break;
        default:
            flag_found = 0;
            break;
        }

        if (flag_found) {
            c = *++s;
        } else {
            break;
        }
    }

    if (c == '*') {
        if ((f.field_width = va_arg(*arg, int)) < 0) {
            f.justification_options = left_justification;
            f.field_width = -f.field_width;
        }

        c = *++s;
    } else {
        while ((c >= 0x100 ? 0 : _current_locale.ctype_cmpt_ptr->wctype_map_ptr[c] & 0x8) != 0) {
            f.field_width = f.field_width * 10 + (c - '0');
            c = *++s;
        }
    }

    if (f.field_width > 509) {
        f.conversion_char = 0xFFFF;
        *format = f;
        return s + 1;
    }

    if (c == '.') {
        f.precision_specified = 1;

        if ((c = *++s) == '*') {
            if ((f.precision = va_arg(*arg, int)) < 0) {
                f.precision_specified = 0;
            }

            c = *++s;
        } else {
            while ((c >= 0x100 ? 0 : _current_locale.ctype_cmpt_ptr->wctype_map_ptr[c] & 0x8) != 0)
            {
                f.precision = f.precision * 10 + (c - '0');
                c = *++s;
            }
        }
    }

    flag_found = 1;

    switch (c) {
    case 'h':
        f.argument_options = short_argument;

        if (s[1] == 'h') {
            f.argument_options = char_argument;
            c = *++s;
        }

        break;

    case 'l':
        f.argument_options = long_argument;

        if (s[1] == 'l') {
            f.argument_options = long_long_argument;
            c = *++s;
        }
        break;
    case 'L':
        f.argument_options = long_double_argument;
        break;
    case 'j':
        f.argument_options = intmax_argument;
        break;
    case 't':
        f.argument_options = ptrdiff_argument;
        break;
    case 'z':
        f.argument_options = size_t_argument;
        break;
    default:
        flag_found = 0;
        break;
    }

    if (flag_found) {
        c = *++s;
    }

    switch (f.conversion_char = c) {
    case 'd':
    case 'i':
    case 'u':
    case 'o':
    case 'x':
    case 'X':
        if (f.argument_options == long_double_argument) {
            f.argument_options = 4;
        }

        if (!f.precision_specified) {
            f.precision = 1;
        } else if (f.justification_options == zero_fill) {
            f.justification_options = right_justification;
        }
        break;

    case 'f':
    case 'F':
        if (f.argument_options == short_argument || f.argument_options == intmax_argument ||
            f.argument_options == size_t_argument || f.argument_options == ptrdiff_argument ||
            f.argument_options == long_long_argument)
        {
            f.conversion_char = 0xFFFF;
            break;
        }

        if (!f.precision_specified) {
            f.precision = 6;
        }
        break;

    case 'a':
    case 'A':
        if (!f.precision_specified) {
            f.precision = 0xD;
        }

        if (f.argument_options == short_argument || f.argument_options == intmax_argument ||
            f.argument_options == size_t_argument || f.argument_options == ptrdiff_argument ||
            f.argument_options == long_long_argument || f.argument_options == char_argument)
        {
            f.conversion_char = 0xFFFF;
        }

        break;

    case 'g':
    case 'G':
        if (!f.precision) {
            f.precision = 1;
        }

    case 'e':
    case 'E':
        if (f.argument_options == short_argument || f.argument_options == intmax_argument ||
            f.argument_options == size_t_argument || f.argument_options == ptrdiff_argument ||
            f.argument_options == long_long_argument || f.argument_options == char_argument)
        {
            f.conversion_char = 0xFFFF;
            break;
        }

        if (!f.precision_specified) {
            f.precision = 6;
        }
        break;

    case 'p':
        f.argument_options = long_argument;
        f.alternate_form = 1;
        f.conversion_char = 'x';
        f.precision = 8;
        break;

    case 'c':
        if (f.argument_options == long_argument) {
            f.argument_options = wchar_argument;
        } else {
            if (f.precision_specified || f.argument_options != normal_argument) {
                f.conversion_char = 0xFFFF;
            }
        }

        break;

    case 's':
        if (f.argument_options == long_argument) {
            f.argument_options = wchar_argument;
        } else {
            if (f.argument_options != normal_argument) {
                f.conversion_char = 0xFFFF;
            }
        }

        break;

    case 'n':
        if (f.argument_options == long_double_argument) {
            f.argument_options = 0x4;
        }

        break;

    default:
        f.conversion_char = 0xFFFF;
        break;
    }

    *format = f;
    return s + 1;
}

wchar_t* long2str(signed long num, wchar_t* buff, print_format format) {
    unsigned long unsigned_num, base;
    wchar_t* p;
    int n, digits;
    int minus = 0;
    unsigned_num = num;
    minus = 0;

    p = buff;
    *--p = 0;
    digits = 0;

    if (!num && !format.precision && !(format.alternate_form && format.conversion_char == 'o')) {
        return p;
    }

    switch (format.conversion_char) {
    case 'd':
    case 'i':
        base = 10;

        if (num < 0) {
            if (num != 0x80000000L) {
                unsigned_num = -unsigned_num;
            }

            minus = 1;
        }
        break;

    case 'o':
        base = 8;
        format.sign_options = only_minus;
        break;

    case 'u':
        base = 10;
        format.sign_options = only_minus;
        break;

    case 'x':
    case 'X':
        base = 16;
        format.sign_options = only_minus;
        break;
    }

    do {
        n = unsigned_num % base;
        unsigned_num /= base;

        if (n < 10) {
            n += '0';
        } else {
            n -= 10;

            if (format.conversion_char == 'x') {
                n += 'a';
            } else {
                n += 'A';
            }
        }

        *--p = n;
        ++digits;
    } while (unsigned_num != 0);

    if (base == 8 && format.alternate_form && *p != '0') {
        *--p = '0';
        ++digits;
    }

    if (format.justification_options == zero_fill) {
        format.precision = format.field_width;

        if (minus || format.sign_options != only_minus)
            --format.precision;

        if (base == 16 && format.alternate_form)
            format.precision -= 2;
    }

    if (buff - p + format.precision > 509)
        return 0;

    while (digits < format.precision) {
        *--p = '0';
        ++digits;
    }

    if (base == 16 && format.alternate_form) {
        *--p = format.conversion_char;
        *--p = '0';
    }

    if (minus) {
        *--p = '-';
    } else if (format.sign_options == sign_always) {
        *--p = '+';
    } else if (format.sign_options == space_holder) {
        *--p = ' ';
    }

    return p;
}

wchar_t* longlong2str(signed long long num, wchar_t* pBuf, print_format fmt) {
    unsigned long long unsigned_num, base;
    wchar_t* p;
    int n, digits;
    int minus = 0;
    unsigned_num = num;
    minus = 0;
    p = pBuf;
    *--p = 0;
    digits = 0;

    if (!num && !fmt.precision && !(fmt.alternate_form && fmt.conversion_char == 'o')) {
        return p;
    }

    switch (fmt.conversion_char) {
    case 'd':
    case 'i':
        base = 10;

        if (num < 0) {
            if (num != 0x8000000000000000LL) {
                unsigned_num = -unsigned_num;
            }

            minus = 1;
        }
        break;
    case 'o':
        base = 8;
        fmt.sign_options = only_minus;
        break;
    case 'u':
        base = 10;
        fmt.sign_options = only_minus;
        break;
    case 'x':
    case 'X':
        base = 16;
        fmt.sign_options = only_minus;
        break;
    }

    do {
        n = unsigned_num % base;
        unsigned_num /= base;

        if (n < 10) {
            n += '0';
        } else {
            n -= 10;
            if (fmt.conversion_char == 'x') {
                n += 'a';
            } else {
                n += 'A';
            }
        }

        *--p = n;
        ++digits;
    } while (unsigned_num != 0);

    if (base == 8 && fmt.alternate_form && *p != '0') {
        *--p = '0';
        ++digits;
    }

    if (fmt.justification_options == zero_fill) {
        fmt.precision = fmt.field_width;

        if (minus || fmt.sign_options != only_minus) {
            --fmt.precision;
        }

        if (base == 16 && fmt.alternate_form) {
            fmt.precision -= 2;
        }
    }

    if (pBuf - p + fmt.precision > 509) {
        return 0;
    }

    while (digits < fmt.precision) {
        *--p = '0';
        ++digits;
    }

    if (base == 16 && fmt.alternate_form) {
        *--p = fmt.conversion_char;
        *--p = '0';
    }

    if (minus) {
        *--p = '-';
    } else if (fmt.sign_options == sign_always) {
        *--p = '+';
    } else if (fmt.sign_options == space_holder) {
        *--p = ' ';
    }

    return p;
}

wchar_t* double2hex(long double num, wchar_t* buff, print_format format) {
    wchar_t* p;
    wchar_t* q;
    unsigned char working_char;
    long double ld;
    int expbits, expmask;
    unsigned snum;
    long exp;
    print_format exp_format;
    int hex_precision;
    int mantissa_bit;
    decform form;
    decimal dec;
    int radix_marker;
    double tempF;

    radix_marker = *(unsigned char *)(__lconv).decimal_point;
    p = buff;
    ld = num;

    if (format.precision > 509) {
        return 0;
    }

    form.style = (char)0;
    form.digits = 0x20;
    __num2dec(&form, num, &dec);

    switch (*dec.sig.text) {
    case '0':
        dec.exp = 0;
        if (dec.sign) {
            p = buff - 5;
            if (format.conversion_char == 'A') {
                wcscpy(p, L"-0X0");
            } else {
                wcscpy(p, L"-0x0");
            }
        } else {
            p = buff - 4;
            if (format.conversion_char == 'A') {
                wcscpy(p, L"0X0");
            } else {
                wcscpy(p, L"0x0");
            }
        }

        return p;
    case 'I':
        if (dec.sign) {
            p = buff - 5;
            if (format.conversion_char == 'A') {
                wcscpy(p, L"-INF");
            } else {
                wcscpy(p, L"-inf");
            }
        } else {
            p = buff - 4;
            if (format.conversion_char == 'A') {
                wcscpy(p, L"INF");
            } else {
                wcscpy(p, L"inf");
            }
        }

        return p;
    case 'N':
        if (dec.sign) {
            p = buff - 5;
            if (format.conversion_char == 'A') {
                wcscpy(p, L"-NAN");
            } else {
                wcscpy(p, L"-nan");
            }
        } else {
            p = buff - 4;
            if (format.conversion_char == 'A') {
                wcscpy(p, L"NAN");
            } else {
                wcscpy(p, L"nan");
            }
        }

        return p;
    }

    exp_format.justification_options = right_justification;
    exp_format.sign_options = sign_always;
    exp_format.precision_specified = 0;
    exp_format.alternate_form = 0;
    exp_format.argument_options = normal_argument;
    exp_format.field_width = 0;
    exp_format.precision = 1;
    exp_format.conversion_char = 'd';

    expbits = 11;
    expmask = 0x7FF;

    snum = ((unsigned char *)&num)[0] << 25;
    if (TARGET_FLOAT_EXP_BITS > 7) {
        snum |= ((unsigned char *)&num)[1] << 17;
    }
    if (TARGET_FLOAT_EXP_BITS > 15) {
        snum |= ((unsigned char *)&num)[2] << 9;
    }
    if (TARGET_FLOAT_EXP_BITS > 23) {
        snum |= ((unsigned char *)&num)[3] << 1;
    }

    snum = (snum >> (32 - expbits)) & expmask;

    if(snum != 0)
        exp = snum - 0x3FF;
    else
        exp = 0;

    p = long2str(exp, buff, exp_format);
    if (format.conversion_char == 'a') {
        *--p = 'p';
    } else {
        *--p = 'P';
    }
    q = (wchar_t*)&num;

    if (TARGET_FLOAT_IMPLICIT_J_BIT) {
        mantissa_bit = (1 + expbits + format.precision * 4) - 1;
    } else {
        mantissa_bit = (1 + expbits + format.precision * 4) - 4;
    }

    for (hex_precision = format.precision; hex_precision >= 1; hex_precision--) {
        if (mantissa_bit < 64) {
            int mantissa_byte;

            mantissa_byte = mantissa_bit >> 3;
            working_char = (*((unsigned char*)q + mantissa_byte)) >> (0x7 - (mantissa_bit & 0x7));

            if ((mantissa_bit & ~0x7) != ((mantissa_bit - 4) & ~0x7)) {
                working_char |= (unsigned char)(((*((unsigned char*)q + (mantissa_byte - 1))) << 8) >> (0x7 - ((mantissa_bit) & 0x7)));
            }

            if (!TARGET_FLOAT_IMPLICIT_J_BIT) {
                if (mantissa_bit == 1 + expbits) {
                    *--p = radix_marker;
                    working_char &= 0x1;
                }
            }

            if ((working_char &= 0xF) < 10) {
                working_char += (unsigned char)'0';
            } else {
                if (format.conversion_char == 'a') {
                    working_char += (unsigned char)('a' - 10);
                } else {
                    working_char += (unsigned char)('A' - 10);
                }
            }
        }
        else {
            working_char = '0';
        }

        *--p = working_char;
        mantissa_bit -= 4;
    }

    if (TARGET_FLOAT_IMPLICIT_J_BIT){
        if (format.precision || format.alternate_form) {
            *--p = radix_marker;
        }

        tempF = __fabs(ld);
        if (tempF != 0.0) {
            *--p = '1';
        } else {
            *--p = '0';
        }
    }

    if (format.conversion_char == 'a') {
        *--p = 'x';
    } else {
        *--p = 'X';
    }

    *--p = '0';

    if (dec.sign) {
        *--p = '-';
    } else if (format.sign_options == sign_always) {
        *--p = '+';
    } else if (format.sign_options == space_holder) {
        *--p = ' ';
    }

    return p;
}

void round_decimal(decimal* dec, int new_length) {
    char c;
    char* p;
    int carry;

    if (new_length < 0) {
    return_zero:
        dec->exp = 0;
        dec->sig.length = 1;
        *dec->sig.text = '0';
        return;
    }

    if (new_length >= dec->sig.length) {
        return;
    }

    p = (char*)dec->sig.text + new_length + 1;
    c = *--p - '0';

    if (c == 5) {
        char* q = &((char*)dec->sig.text)[dec->sig.length];

        while (--q > p && *q == '0')
            ;
        carry = q == p ? p[-1] & 1 : 1;
    } else {
        carry = c > 5;
    }

    while (new_length != 0) {
        c = *--p - '0' + carry;

        if ((carry = (c > 9)) != 0 || c == 0) {
            --new_length;
        } else {
            *p = c + '0';
            break;
        }
    }

    if (carry != 0) {
        dec->exp += 1;
        dec->sig.length = 1;
        *dec->sig.text = '1';
        return;
    } else if (new_length == 0) {
        goto return_zero;
    }

    dec->sig.length = new_length;
}

wchar_t* float2str(double num, wchar_t* buff, print_format format) {
    wchar_t out_buf[0x100];
    decimal dec;
    decform form;
    wchar_t* p;
    char* p2;
    wchar_t c;
    char* p3;
    unsigned char* q;
    int n, digits, sign;
    int int_digits, frac_digits;
    int radix_marker;
    int used_bytes;

    radix_marker = *(unsigned char*)__lconv.decimal_point;

    if (format.precision > 509) {
        return 0;
    }

    form.style = 0;
    form.digits = 0x20;
    __num2dec(&form, num, &dec);
    p3 = (char*)(dec.sig.text + dec.sig.length);

    while (dec.sig.length > 1 && *--p3 == '0') {
        --dec.sig.length;
        ++dec.exp;
    }

    switch (*dec.sig.text) {
    case '0':
        dec.exp = 0;
        break;
    case 'I':
        if (num < 0) {
            c = format.conversion_char;
            p = buff - 5;

            if ((c >= 0x100 ? 0 : _current_locale.ctype_cmpt_ptr->wctype_map_ptr[c] & 0x200) != 0) {
                wcscpy(p, L"-INF");
            } else {
                wcscpy(p, L"-inf");
            }
        } else {
            c = format.conversion_char;
            p = buff - 4;
            if ((c >= 0x100 ? 0 : _current_locale.ctype_cmpt_ptr->wctype_map_ptr[c] & 0x200) != 0) {
                wcscpy(p, L"INF");
            } else {
                wcscpy(p, L"inf");
            }
        }

        return p;

    case 'N':
        if (dec.sign) {
            c = format.conversion_char;
            p = buff - 5;
            if ((c >= 0x100 ? 0 : _current_locale.ctype_cmpt_ptr->wctype_map_ptr[c] & 0x200) != 0) {
                wcscpy(p, L"-NAN");
            } else {
                wcscpy(p, L"-nan");
            }
        } else {
            c = format.conversion_char;
            p = buff - 4;
            if ((c >= 0x100 ? 0 : _current_locale.ctype_cmpt_ptr->wctype_map_ptr[c] & 0x200) != 0) {
                wcscpy(p, L"NAN");
            } else {
                wcscpy(p, L"nan");
            }
        }

        return p;
    }

    dec.exp += (short)(dec.sig.length - 1);

    p2 = (char*)&out_buf + sizeof(out_buf);
    *--p2 = 0;

    switch (format.conversion_char) {
    case 'g':
    case 'G':

        if (dec.sig.length > format.precision) {
            round_decimal(&dec, format.precision);
        }

        if (dec.exp < -4 || dec.exp >= format.precision) {
            if (format.alternate_form) {
                --format.precision;
            } else {
                format.precision = dec.sig.length - 1;
            }

            if (format.conversion_char == 'g') {
                format.conversion_char = 'e';
            } else {
                format.conversion_char = 'E';
            }

            goto e_format;
        }

        if (format.alternate_form) {
            format.precision -= dec.exp + 1;
        } else {
            if ((format.precision = dec.sig.length - (dec.exp + 1)) < 0) {
                format.precision = 0;
            }
        }

        goto f_format;

    case 'e':
    case 'E':
    e_format:

        if (dec.sig.length > format.precision + 1) {
            round_decimal(&dec, format.precision + 1);
        }

        n = dec.exp;
        sign = '+';

        if (n < 0) {
            n = -n;
            sign = '-';
        }

        for (digits = 0; n || digits < 2; ++digits) {
            *--p2 = n % 10 + '0';
            n /= 10;
        }

        *--p2 = sign;
        *--p2 = format.conversion_char;

        used_bytes = (intptr_t)out_buf - (intptr_t)p2;
        if (used_bytes + format.precision > 0x1FD) {
            return NULL;
        }

        if (dec.sig.length < format.precision + 1) {
            for (n = format.precision + 1 - dec.sig.length + 1; --n;) {
                *--p2 = '0';
            }
        }

        for (n = dec.sig.length, q = (dec.sig.text + dec.sig.length); --n;) {
            *--p2 = *--q;
        }

        if (format.precision || format.alternate_form) {
            *--p2 = radix_marker;
        }

        *--p2 = *dec.sig.text;

        if (dec.sign)
            *--p2 = '-';
        else if (format.sign_options == sign_always)
            *--p2 = '+';
        else if (format.sign_options == space_holder)
            *--p2 = ' ';

        break;

    case 'f':
    case 'F':
    f_format:

        if ((frac_digits = -dec.exp + dec.sig.length - 1) < 0)
            frac_digits = 0;

        if (frac_digits > format.precision) {
            round_decimal(&dec, dec.sig.length - (frac_digits - format.precision));

            if ((frac_digits = -dec.exp + dec.sig.length - 1) < 0)
                frac_digits = 0;
        }

        if ((int_digits = dec.exp + 1) < 0)
            int_digits = 0;

        if (int_digits + frac_digits > 509)
            return 0;

        q = dec.sig.text + dec.sig.length;

        for (digits = 0; digits < (format.precision - frac_digits); ++digits)
            *--p2 = '0';

        for (digits = 0; digits < frac_digits && digits < dec.sig.length; ++digits)
            *--p2 = *--q;

        for (; digits < frac_digits; ++digits)
            *--p2 = '0';

        if (format.precision || format.alternate_form)
            *--p2 = radix_marker;

        if (int_digits) {
            for (digits = 0; digits < int_digits - dec.sig.length; ++digits) {
                *--p2 = '0';
            }

            for (; digits < int_digits; ++digits) {
                *--p2 = *--q;
            }
        } else {
            *--p2 = '0';
        }

        if (dec.sign) {
            *--p2 = '-';
        } else if (format.sign_options == sign_always) {
            *--p2 = '+';
        } else if (format.sign_options == space_holder) {
            *--p2 = ' ';
        }

        break;
    }

    p = buff - strlen((char*)p2) - 1;
    mbstowcs(p, p2, strlen((char*)p2));

    return p;
}

int __wpformatter(int (*write_proc)(wString* dst, const void* data, size_t len),
                         void* proc_data, const wchar_t* fmt,
                         va_list args, int is_secure) {
    int num_chars, chars_written, field_width;
    const wchar_t* format_ptr;
    const wchar_t* curr_format;
    print_format format;
    signed long long_num;
    signed long long long_long_num;
    long double long_double_num;
    wchar_t buff[512];
    wchar_t* buff_ptr;
    const wchar_t* string_end;
    wchar_t fill_char = ' ';

    format_ptr = fmt;
    chars_written = 0;

    while (*format_ptr) {
        if (!(curr_format = wcschr(format_ptr, L'%'))) {
            num_chars = wcslen(format_ptr);
            chars_written += num_chars;

            if (num_chars && !(*write_proc)(proc_data, format_ptr, num_chars)) {
                return -1;
            }

            break;
        }

        num_chars = curr_format - format_ptr;
        chars_written += num_chars;

        if (num_chars && !(*write_proc)(proc_data, format_ptr, num_chars)) {
            return -1;
        }

        format_ptr = curr_format;
        format_ptr = parse_format(format_ptr, (va_list*)args, &format);

        switch (format.conversion_char) {
        case 'd':
        case 'i':
            if (format.argument_options == long_argument) {
                long_num = va_arg(args, signed long);
            } else if (format.argument_options == long_long_argument) {
                long_long_num = va_arg(args, signed long long);
            } else if (format.argument_options == intmax_argument) {
                long_long_num = va_arg(args, intmax_t);
            } else if (format.argument_options == size_t_argument) {
                long_num = va_arg(args, size_t);
            } else if (format.argument_options == ptrdiff_argument) {
                long_num = va_arg(args, ptrdiff_t);
            } else {
                long_num = va_arg(args, int);
            }

            if (format.argument_options == short_argument) {
                long_num = (signed short)long_num;
            }

            if (format.argument_options == long_long_argument ||
                format.argument_options == intmax_argument)
            {
                if (!(buff_ptr = longlong2str(long_long_num, buff + 512, format))) {
                    goto conversion_error;
                }
            } else {
                if (!(buff_ptr = long2str(long_num, buff + 512, format))) {
                    goto conversion_error;
                }
            }

            num_chars = buff + 512 - 1 - buff_ptr;
            break;

        case 'o':
        case 'u':
        case 'x':
        case 'X':
            if (format.argument_options == long_argument) {
                long_num = va_arg(args, unsigned long);
            } else if (format.argument_options == long_long_argument) {
                long_long_num = va_arg(args, signed long long);
            }
            else if (format.argument_options == intmax_argument) {
                long_long_num = va_arg(args, intmax_t);
            }
            else if (format.argument_options == size_t_argument) {
                long_num = va_arg(args, size_t);
            }
            else if (format.argument_options == ptrdiff_argument) {
                long_num = va_arg(args, ptrdiff_t);
            }
            else {
                long_num = va_arg(args, unsigned int);
            }

            if (format.argument_options == short_argument) {
                long_num = (unsigned short)long_num;
            }

            if ((format.argument_options == long_long_argument)
                || format.argument_options == intmax_argument
               )
            {
                if (!(buff_ptr = longlong2str(long_long_num, buff + 512, format))) {
                    goto conversion_error;
                }
            } else {
                if (!(buff_ptr = long2str(long_num, buff + 512, format))) {
                    goto conversion_error;
                }
            }

            num_chars = buff + 512 - 1 - buff_ptr;
            break;

        case 'f':
        case 'F':
        case 'e':
        case 'E':
        case 'g':
        case 'G':
            if (format.argument_options == long_double_argument) {
                long_double_num = va_arg(args, long double);
            } else {
                long_double_num = va_arg(args, double);
            }

            if (!(buff_ptr = float2str(long_double_num, buff + 512, format))) {
                goto conversion_error;
            }

            num_chars = buff + 512 - 1 - buff_ptr;
            break;

        case 'a':
        case 'A':
            if (format.argument_options == long_double_argument) {
                long_double_num = va_arg(args, long double);
            } else {
                long_double_num = va_arg(args, double);
            }

            if (!(buff_ptr = double2hex(long_double_num, buff + 512, format))) {
                goto conversion_error;
            }

            num_chars = buff + 512 - 1 - buff_ptr;
            break;

        case 's':
            if (format.argument_options == wchar_argument) {
                wchar_t* wcs_ptr = va_arg(args, wchar_t*);

                if (is_secure && wcs_ptr == NULL){
                    __msl_runtime_constraint_violation_s(0,0,-1);
                    return -1;
                }

                if (wcs_ptr == NULL) {
                    wcs_ptr = L"";
                }

                if (format.alternate_form) {
                    num_chars = (unsigned char)*wcs_ptr++;

                    if (format.precision_specified && num_chars > format.precision) {
                        num_chars = format.precision;
                    }
                } else if (format.precision_specified) {
                    num_chars = format.precision;

                    if ((string_end = wmemchr(wcs_ptr, 0, num_chars)) != 0) {
                        num_chars = string_end - wcs_ptr;
                    }

                } else {
                    num_chars = wcslen(wcs_ptr);
                }

                buff_ptr = wcs_ptr;
                break;
            } else {
                const char* wcs_ptr = (const char*)va_arg(args, wchar_t*);

                if (is_secure && wcs_ptr == NULL){
                    __msl_runtime_constraint_violation_s(0,0,-1);
                    return -1;
                }

                if (wcs_ptr == NULL) {
                    wcs_ptr = "";
                }

                if (format.alternate_form) {
                    num_chars = (unsigned char)*buff_ptr;

                    if (format.precision_specified && num_chars > format.precision) {
                        num_chars = format.precision;
                    }
                } else if (format.precision_specified) {
                    num_chars = format.precision;

                    if ((string_end = memchr(wcs_ptr, 0, num_chars)) != 0) {
                        num_chars = (intptr_t)string_end - (intptr_t)wcs_ptr;
                    }
                } else {
                    num_chars = strlen(wcs_ptr);
                }

                if ((num_chars = mbstowcs(buff, wcs_ptr, num_chars)) < 0) {
                    goto conversion_error;
                }

                buff_ptr = buff;
            }

            break;

        case 'n':
            if (is_secure) {
                __msl_runtime_constraint_violation_s(NULL, NULL, -1);
                return -1;
            }

            buff_ptr = va_arg(args, wchar_t*);

            switch (format.argument_options) {
            case normal_argument:
                *(int*)buff_ptr = chars_written;
                break;
            case short_argument:
                *(signed short*)buff_ptr = chars_written;
                break;
            case long_argument:
                *(signed long*)buff_ptr = chars_written;
                break;
            case intmax_argument:
                *(intmax_t*)buff_ptr = chars_written;
                break;
            case size_t_argument:
                *(size_t*)buff_ptr = chars_written;
                break;
            case ptrdiff_argument:
                *(ptrdiff_t*)buff_ptr = chars_written;
                break;
            case long_long_argument:
                *(signed long long*)buff_ptr = chars_written;
                break;
            }

            continue;

        case 'c':
            buff_ptr = buff;
            if (format.argument_options == wchar_argument) {
                *buff_ptr = va_arg(args, long);
                num_chars = 1;
            } else {
                char s;
                s = va_arg(args, int);
                num_chars = mbtowc(buff_ptr, &s, 1);
            }
            break;

        case '%':
            buff_ptr = buff;
            *buff_ptr = '%';
            num_chars = 1;
            break;

        case 0xFF:
        default:
        conversion_error:
            num_chars = wcslen(curr_format);
            chars_written += num_chars;

            if (num_chars && !(*write_proc)(proc_data, curr_format, num_chars)) {
                return -1;
            }

            return chars_written;
            break;
        }

        field_width = num_chars;

        if (format.justification_options != left_justification) {
            fill_char = (format.justification_options == zero_fill) ? '0' : ' ';

            if (((*buff_ptr == '+') || (*buff_ptr == '-') || (*buff_ptr == ' ')) &&
                (fill_char == '0'))
            {
                if ((*write_proc)(proc_data, buff_ptr, 1) == 0) {
                    return -1;
                }

                ++buff_ptr;
                num_chars--;
            }

            if ((format.justification_options == zero_fill) &&
                ((format.conversion_char == 'a') || (format.conversion_char == 'A')))
            {
                if (num_chars < 2) {
                    return -1;
                }

                if ((*write_proc)(proc_data, buff_ptr, 2) == 0) {
                    return -1;
                }

                num_chars -= 2;
                buff_ptr += 2;
            }

            while (field_width < format.field_width) {
                if ((*write_proc)(proc_data, (wchar_t*)&fill_char, 1) == 0) {
                    return -1;
                }

                ++field_width;
            }
        }

        if (num_chars && !(*write_proc)(proc_data, buff_ptr, num_chars)) {
            return -1;
        }

        if (format.justification_options == left_justification) {
            while (field_width < format.field_width) {
                wchar_t blank = L' ';

                if ((*write_proc)(proc_data, (wchar_t*)&blank, 1) == 0) {
                    return -1;
                }

                ++field_width;
            }
        }

        chars_written += field_width;
    }

    return chars_written;
}

static int __wStringWrite(wString* dst, const void* data, size_t len) {
    const __OutStrCtrl* osc = data;
    size_t cur_len = dst->field_0x8;
    size_t var_r7 = dst->field_0x4;
    size_t write_cnt = cur_len + len <= var_r7 ? len : var_r7 - cur_len;
    wmemcpy(dst->buffer + cur_len, (const wchar_t*)osc, write_cnt);
    dst->field_0x8 += write_cnt;
}

int swprintf(wchar_t *dst, size_t maxlen, const wchar_t *fmt, ...) {
    int written;
    va_list args;
    __OutStrCtrl ctx;

    ctx.WCharStr = dst;

    va_start(args, fmt);
    ctx.MaxCharCount = maxlen;
    ctx.CharsWritten = 0;
    written = __wpformatter(__wStringWrite, &ctx, fmt, args, 0);
    va_end(args);

    if (written >= 0) {
        if ((size_t)written < maxlen) {
            dst[written] = '\0';
            return written;
        }
        dst[maxlen - 1] = '\0';
        return -1;
    }
}

int vsnwprintf_s(wchar_t* str, size_t n, const wchar_t* fmt, va_list args) {
    __OutStrCtrl osc;
    int written_cnt;

    if (str == NULL || fmt == NULL || n == 0 || n > 0x7FFFFFFFU) {
        __msl_runtime_constraint_violation_s(0, 0, -1);
        if (str != NULL && n != 0 && n <= 0x7FFFFFFFU) {
            *str = 0;
        }
        return -1;
    }

    osc.MaxCharCount = n;
    osc.WCharStr = str;
    osc.CharsWritten = 0;
    written_cnt = __wpformatter(__wStringWrite, &osc, fmt, args, 1);
    if (written_cnt >= 0) {
        if (written_cnt < n) {
            str[written_cnt] = '\0';
            return written_cnt;
        }
        str[n - 1] = '\0';
        return written_cnt;
    }
    return written_cnt;
}
