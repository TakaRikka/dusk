#include "printf.h"
#include "ansi_fp.h"
#include "critical_regions.h"
#include "ctype.h"
#include "scanf.h"
#include "stdio.h"
#include "string.h"
#include "wchar_io.h"

#define TARGET_FLOAT_BITS 64
#define TARGET_FLOAT_BYTES (TARGET_FLOAT_BITS / 8)
#define TARGET_FLOAT_MAX_EXP LDBL_MAX_EXP
#define TARGET_FLOAT_MANT_DIG LDBL_MANT_DIG
#define TARGET_FLOAT_IMPLICIT_J_BIT 1
#define TARGET_FLOAT_MANT_BITS (TARGET_FLOAT_MANT_DIG - TARGET_FLOAT_IMPLICIT_J_BIT)
#define TARGET_FLOAT_EXP_BITS (TARGET_FLOAT_BITS - TARGET_FLOAT_MANT_BITS - 1)

#define PTRDIFF __typeof__((char*)0 - (char*)0)
typedef PTRDIFF ptrdiff_t;

enum justification_options { left_justification, right_justification, zero_fill };

enum sign_options { only_minus, sign_always, space_holder };

enum argument_options {
    normal_argument,
    char_argument,
    short_argument,
    long_argument,
    long_long_argument,
    long_double_argument,
    wchar_argument
};

typedef struct {
    unsigned char justification_options;
    unsigned char sign_options;
    unsigned char precision_specified;
    unsigned char alternate_form;
    unsigned char argument_options;
    unsigned char conversion_char;
    int field_width;
    int precision;
} print_format;

static const char* parse_format(const char* format_string, va_list* arg, print_format* format) {
    print_format f;
    const char* s = format_string;
    int c;
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
        return ((const char*)s + 1);
    }

    for (;;) {
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
        while (isdigit(c)) {
            f.field_width = (f.field_width * 10) + (c - '0');
            c = *++s;
        }
    }

    if (f.field_width > 509) {
        f.conversion_char = 0xFF;
        *format = f;
        return ((const char*)s + 1);
    }

    if (c == '.') {
        f.precision_specified = 1;

        if ((c = *++s) == '*') {
            if ((f.precision = va_arg(*arg, int)) < 0) {
                f.precision_specified = 0;
            }

            c = *++s;
        } else {
            while (isdigit(c)) {
                f.precision = (f.precision * 10) + (c - '0');
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
    default:
        flag_found = 0;
        break;
    }

    if (flag_found) {
        c = *++s;
    }

    f.conversion_char = c;

    switch (c) {
    case 'd':
    case 'i':
    case 'u':
    case 'o':
    case 'x':
    case 'X':
        if (f.argument_options == long_double_argument) {
            f.conversion_char = 0xFF;
            break;
        }

        if (!f.precision_specified) {
            f.precision = 1;
        } else if (f.justification_options == zero_fill) {
            f.justification_options = right_justification;
        }
        break;

    case 'f':
    case 'F':
        if (f.argument_options == short_argument || f.argument_options == long_long_argument) {
            f.conversion_char = 0xFF;
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

        if (f.argument_options == short_argument || f.argument_options == long_long_argument ||
            f.argument_options == char_argument)
        {
            f.conversion_char = 0xFF;
        }

        break;

    case 'g':
    case 'G':
        if (!f.precision) {
            f.precision = 1;
        }

    case 'e':
    case 'E':
        if (f.argument_options == short_argument || f.argument_options == long_long_argument ||
            f.argument_options == char_argument)
        {
            f.conversion_char = 0xFF;
            break;
        }

        if (!f.precision_specified) {
            f.precision = 6;
        }
        break;

    case 'p':
        f.conversion_char = 'x';
        f.alternate_form = 1;
        f.argument_options = long_argument;
        f.precision = 8;
        break;

    case 'c':
        if (f.argument_options == long_argument) {
            f.argument_options = wchar_argument;
        } else {
            if (f.precision_specified || f.argument_options != normal_argument) {
                f.conversion_char = 0xFF;
            }
        }

        break;

    case 's':
        if (f.argument_options == long_argument) {
            f.argument_options = wchar_argument;
        } else {
            if (f.argument_options != normal_argument) {
                f.conversion_char = 0xFF;
            }
        }

        break;

    case 'n':
        if (f.argument_options == long_double_argument) {
            f.conversion_char = 0xFF;
        }

        break;

    default:
        f.conversion_char = 0xFF;
        break;
    }

    *format = f;
    return ((const char*)s + 1);
}

static char* long2str(signed long num, char* buff, print_format format) {
    unsigned long unsigned_num, base;
    char* p;
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
            unsigned_num = -unsigned_num;
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
        return (0);

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

static char* longlong2str(signed long long num, char* pBuf, print_format fmt) {
    unsigned long long unsigned_num, base;
    char* p;
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
            unsigned_num = -unsigned_num;
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

static char* double2hex(long double num, char* buff, print_format format) {
    int offset, what_nibble = 0;
    char* wrk_byte_ptr;
    char *p, *q;
    char working_byte;
    long double ld;
    signed short* sptr;
    signed short snum;
    signed long exp;
    print_format exp_format;
    int hex_precision;
    decform form;
    decimal dec;

    p = buff;
    ld = num;
    sptr = (signed short*)&ld;

    if (format.precision > 509) {
        return 0;
    }

    form.style = (char)0;
    form.digits = 0x20;
    __num2dec(&form, num, &dec);

    if (*dec.sig.text == 'I') {
        if (*sptr & 0x8000) {
            p = buff - 5;
            if (format.conversion_char == 'A')
                strcpy(p, "-INF");
            else
                strcpy(p, "-inf");
        } else {
            p = buff - 4;
            if (format.conversion_char == 'A')
                strcpy(p, "INF");
            else
                strcpy(p, "inf");
        }

        return p;
    } else if (*dec.sig.text == 'N') {
        if (*(char*)&num & 0x80) {
            p = buff - 5;
            if (format.conversion_char == 'A')
                strcpy(p, "-NAN");
            else
                strcpy(p, "-nan");
        } else {
            p = buff - 4;
            if (format.conversion_char == 'A')
                strcpy(p, "NAN");
            else
                strcpy(p, "nan");
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

    snum = (*sptr & 0x7ff0) >> 4;

    exp = snum - 0x3FF;

    p = long2str(exp, buff, exp_format);
    if (format.conversion_char == 'a')
        *--p = 'p';
    else
        *--p = 'P';

    q = (char*)&num;

    for (hex_precision = format.precision; hex_precision >= 1; hex_precision--) {
        working_byte = *(q + (hex_precision / 2) + 1);
        if (hex_precision % 2)
            working_byte = working_byte & 0x0f;
        else
            working_byte = (working_byte >> 4) & 0x0f;

        if (working_byte < 10) {
            working_byte += '0';
        } else {
            working_byte -= 10;

            if (format.conversion_char == 'a') {
                working_byte += 'a';
            } else {
                working_byte += 'A';
            }
        }

        *--p = working_byte;
    }

    if (format.precision || format.alternate_form) {
        *--p = '.';
    }

    *--p = '1';

    if (format.conversion_char == 'a') {
        *--p = 'x';
    } else {
        *--p = 'X';
    }

    *--p = '0';

    if (*sptr & 0x8000) {
        *--p = '-';
    } else if (format.sign_options == sign_always) {
        *--p = '+';
    } else if (format.sign_options == space_holder) {
        *--p = ' ';
    }

    return p;
}

static void round_decimal(decimal* dec, int new_length) {
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
        carry = (q == p) ? p[-1] & 1 : 1;
    } else {
        carry = (c > 5);
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

static char* float2str(long double num, char* buff, print_format format) {
    decimal dec;
    decform form;
    char* p;
    char* q;
    int n, digits, sign;
    int int_digits, frac_digits;
    int radix_marker;

    radix_marker = '.';

    if (format.precision > 509) {
        return 0;
    }

    form.style = 0;
    form.digits = 0x20;
    __num2dec(&form, num, &dec);
    p = (char*)dec.sig.text + dec.sig.length;

    while (dec.sig.length > 1 && *--p == '0') {
        --dec.sig.length;
        ++dec.exp;
    }

    switch (*dec.sig.text) {
    case '0':
        dec.exp = 0;
        break;
    case 'I':
        if (num < 0) {
            p = buff - 5;

            if (isupper(format.conversion_char)) {
                strcpy(p, "-INF");
            } else {
                strcpy(p, "-inf");
            }
        } else {
            p = buff - 4;
            if (isupper(format.conversion_char)) {
                strcpy(p, "INF");
            } else {
                strcpy(p, "inf");
            }
        }

        return p;

    case 'N':
        if (dec.sign) {
            p = buff - 5;

            if (isupper(format.conversion_char)) {
                strcpy(p, "-NAN");
            } else {
                strcpy(p, "-nan");
            }
        } else {
            p = buff - 4;
            if (isupper(format.conversion_char)) {
                strcpy(p, "NAN");
            } else {
                strcpy(p, "nan");
            }
        }

        return p;
    }

    dec.exp += dec.sig.length - 1;
    p = buff;
    *--p = 0;

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
            *--p = n % 10 + '0';
            n /= 10;
        }

        *--p = sign;
        *--p = format.conversion_char;

        if (buff - p + format.precision > 509) {
            return 0;
        }

        if (dec.sig.length < format.precision + 1) {
            for (n = format.precision + 1 - dec.sig.length + 1; --n;) {
                *--p = '0';
            }
        }

        for (n = dec.sig.length, q = (char*)dec.sig.text + dec.sig.length; --n;) {
            *--p = *--q;
        }

        if (format.precision || format.alternate_form) {
            *--p = radix_marker;
        }

        *--p = *dec.sig.text;

        if (dec.sign)
            *--p = '-';
        else if (format.sign_options == sign_always)
            *--p = '+';
        else if (format.sign_options == space_holder)
            *--p = ' ';

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

        q = (char*)dec.sig.text + dec.sig.length;

        for (digits = 0; digits < (format.precision - frac_digits); ++digits)
            *--p = '0';

        for (digits = 0; digits < frac_digits && digits < dec.sig.length; ++digits)
            *--p = *--q;

        for (; digits < frac_digits; ++digits)
            *--p = '0';

        if (format.precision || format.alternate_form)
            *--p = radix_marker;

        if (int_digits) {
            for (digits = 0; digits < int_digits - dec.sig.length; ++digits) {
                *--p = '0';
            }

            for (; digits < int_digits; ++digits) {
                *--p = *--q;
            }
        } else {
            *--p = '0';
        }

        if (dec.sign) {
            *--p = '-';
        } else if (format.sign_options == sign_always) {
            *--p = '+';
        } else if (format.sign_options == space_holder) {
            *--p = ' ';
        }

        break;
    }

    return p;
}

static int __pformatter(void* (*WriteProc)(void*, const char*, size_t), void* WriteProcArg,
                        const char* format_str, va_list arg) {
    int num_chars, chars_written, field_width;
    const char* format_ptr;
    const char* curr_format;
    print_format format;
    signed long long_num;
    signed long long long_long_num;
    long double long_double_num;
    char buff[512];
    char* buff_ptr;
    char* string_end;
    char fill_char = ' ';

    format_ptr = format_str;
    chars_written = 0;

    while (*format_ptr) {
        if (!(curr_format = strchr(format_ptr, '%'))) {
            num_chars = strlen(format_ptr);
            chars_written += num_chars;

            if (num_chars && !(*WriteProc)(WriteProcArg, format_ptr, num_chars)) {
                return -1;
            }

            break;
        }

        num_chars = curr_format - format_ptr;
        chars_written += num_chars;

        if (num_chars && !(*WriteProc)(WriteProcArg, format_ptr, num_chars)) {
            return -1;
        }

        format_ptr = curr_format;
        format_ptr = parse_format(format_ptr, (va_list*)arg, &format);

        switch (format.conversion_char) {
        case 'd':
        case 'i':
            if (format.argument_options == long_argument) {
                long_num = va_arg(arg, signed long);
            } else if (format.argument_options == long_long_argument) {
                long_long_num = va_arg(arg, signed long long);
            } else {
                long_num = va_arg(arg, int);
            }

            if (format.argument_options == short_argument) {
                long_num = (signed short)long_num;
            }

            if (format.argument_options == char_argument) {
                long_num = (signed char)long_num;
            }

            if ((format.argument_options == long_long_argument)) {
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
                long_num = va_arg(arg, unsigned long);
            } else if (format.argument_options == long_long_argument) {
                long_long_num = va_arg(arg, signed long long);
            } else {
                long_num = va_arg(arg, unsigned int);
            }

            if (format.argument_options == short_argument) {
                long_num = (unsigned short)long_num;
            }

            if (format.argument_options == char_argument) {
                long_num = (unsigned char)long_num;
            }

            if ((format.argument_options == long_long_argument)) {
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
                long_double_num = va_arg(arg, long double);
            } else {
                long_double_num = va_arg(arg, f64);
            }

            if (!(buff_ptr = float2str(long_double_num, buff + 512, format))) {
                goto conversion_error;
            }

            num_chars = buff + 512 - 1 - buff_ptr;
            break;

        case 'a':
        case 'A':
            if (format.argument_options == long_double_argument) {
                long_double_num = va_arg(arg, long double);
            } else {
                long_double_num = va_arg(arg, f64);
            }

            if (!(buff_ptr = double2hex(long_double_num, buff + 512, format))) {
                goto conversion_error;
            }

            num_chars = buff + 512 - 1 - buff_ptr;
            break;

        case 's':
            if (format.argument_options == wchar_argument) {
                wchar_t* wcs_ptr = va_arg(arg, wchar_t*);

                if (wcs_ptr == NULL) {
                    wcs_ptr = L"";
                }

                if ((num_chars = wcstombs(buff, wcs_ptr, sizeof(buff))) < 0) {
                    goto conversion_error;
                }

                buff_ptr = &buff[0];
            } else {
                buff_ptr = va_arg(arg, char*);
            }

            if (buff_ptr == NULL) {
                buff_ptr = "";
            }

            if (format.alternate_form) {
                num_chars = (unsigned char)*buff_ptr++;

                if (format.precision_specified && num_chars > format.precision) {
                    num_chars = format.precision;
                }
            } else if (format.precision_specified) {
                num_chars = format.precision;

                if ((string_end = (char*)memchr((unsigned char*)buff_ptr, 0, num_chars)) != 0) {
                    num_chars = string_end - buff_ptr;
                }
            } else {
                num_chars = strlen(buff_ptr);
            }

            break;

        case 'n':
            buff_ptr = va_arg(arg, char*);

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
            case long_long_argument:
                *(signed long long*)buff_ptr = chars_written;
                break;
            }

            continue;

        case 'c':
            buff_ptr = buff;
            *buff_ptr = va_arg(arg, int);
            num_chars = 1;
            break;

        case '%':
            buff_ptr = buff;
            *buff_ptr = '%';
            num_chars = 1;
            break;

        case 0xFF:
        default:
        conversion_error:
            num_chars = strlen(curr_format);
            chars_written += num_chars;

            if (num_chars && !(*WriteProc)(WriteProcArg, curr_format, num_chars)) {
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
                if ((*WriteProc)(WriteProcArg, buff_ptr, 1) == 0) {
                    return -1;
                }

                ++buff_ptr;
                num_chars--;
            }

            while (field_width < format.field_width) {
                if ((*WriteProc)(WriteProcArg, &fill_char, 1) == 0) {
                    return -1;
                }

                ++field_width;
            }
        }

        if (num_chars && !(*WriteProc)(WriteProcArg, buff_ptr, num_chars)) {
            return -1;
        }

        if (format.justification_options == left_justification) {
            while (field_width < format.field_width) {
                char blank = ' ';

                if ((*WriteProc)(WriteProcArg, &blank, 1) == 0) {
                    return -1;
                }

                ++field_width;
            }
        }

        chars_written += field_width;
    }

    return chars_written;
}

static void* __FileWrite(void* pFile, const char* pBuffer, size_t char_num) {
    return (fwrite(pBuffer, 1, char_num, (FILE*)pFile) == char_num ? pFile : 0);
}

static void* __StringWrite(void* pCtrl, const char* pBuffer, size_t char_num) {
    size_t chars;
    __OutStrCtrl* ctrl = (__OutStrCtrl*)pCtrl;
    void* res;

    chars = ((ctrl->CharsWritten + char_num) <= ctrl->MaxCharCount) ?
                char_num :
                ctrl->MaxCharCount - ctrl->CharsWritten;
    res = memcpy(ctrl->CharStr + ctrl->CharsWritten, pBuffer, chars);
    ctrl->CharsWritten += chars;
    return (void*)1;
}

int printf(const char* format, ...) {
    int res;

    if (fwide(stdout, -1) >= 0) {
        return -1;
    }

    __begin_critical_region(stdin_access);
    {
        va_list args;
        va_start(args, format);
        res = __pformatter(&__FileWrite, (void*)stdout, format, args);
    }
    __end_critical_region(stdin_access);
    return res;
}

int fprintf(FILE* file, const char* format, ...) {
    int res;

    if (fwide(file, -1) >= 0) {
        return -1;
    }

    __begin_critical_region(stdin_access);
    {
        va_list args;
        va_start(args, format);
        res = __pformatter(&__FileWrite, (void*)file, format, args);
    }
    __end_critical_region(stdin_access);
    return res;
}

int vprintf(const char* format, va_list arg) {
    int ret;

    if (fwide(stdout, -1) >= 0) {
        return -1;
    }

    __begin_critical_region(stdin_access);
    ret = __pformatter(&__FileWrite, (void*)stdout, format, arg);
    __end_critical_region(stdin_access);
    return ret;
}

int vsnprintf(char* s, size_t n, const char* format, va_list arg) {
    int end;
    __OutStrCtrl osc;
    osc.CharStr = s;
    osc.MaxCharCount = n;
    osc.CharsWritten = 0;

    end = __pformatter(&__StringWrite, &osc, format, arg);

    if (s) {
        s[(end < n) ? end : n - 1] = '\0';
    }

    return end;
}

int snprintf(char* s, size_t n, const char* format, ...) {
    va_list args;
    va_start(args, format);
    return vsnprintf(s, n, format, args);
}

int sprintf(char* s, const char* format, ...) {
    va_list args;
    va_start(args, format);
    return vsnprintf(s, 0xFFFFFFFF, format, args);
}
