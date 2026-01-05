#include "strtold.h"
#include <cstdio>
#include "ansi_fp.h"
#include <locale>
#include <ctype>
#include "float.h"
#include <cmath>
#include <climits>
#include <errno.h>

extern double nan(const char*);
extern double __dec2num(const decimal* d);

#define TARGET_FLOAT_BITS 64
#define TARGET_FLOAT_BYTES (TARGET_FLOAT_BITS / 8)
#define TARGET_FLOAT_MAX_EXP LDBL_MAX_EXP
#define TARGET_FLOAT_MANT_DIG LDBL_MANT_DIG
#define TARGET_FLOAT_IMPLICIT_J_BIT 1
#define TARGET_FLOAT_MANT_BITS (TARGET_FLOAT_MANT_DIG - TARGET_FLOAT_IMPLICIT_J_BIT)
#define TARGET_FLOAT_EXP_BITS (TARGET_FLOAT_BITS - TARGET_FLOAT_MANT_BITS - 1)

enum scan_states {
    start = 0x0001,
    sig_start = 0x0002,
    leading_sig_zeroes = 0x0004,
    int_digit_loop = 0x0008,
    frac_start = 0x0010,
    frac_digit_loop = 0x0020,
    sig_end = 0x0040,
    exp_start = 0x0080,
    leading_exp_digit = 0x0100,
    leading_exp_zeroes = 0x0200,
    exp_digit_loop = 0x0400,
    finished = 0x0800,
    failure = 0x1000,
    nan_state = 0x2000,
    infin_state = 0x4000,
    hex_state = 0x8000
};

enum hex_scan_states {
    not_hex = 0x0000,
    hex_start = 0x0001,
    hex_leading_sig_zeroes = 0x0002,
    hex_int_digit_loop = 0x0004,
    hex_frac_digit_loop = 0x0008,
    hex_sig_end = 0x0010,
    hex_exp_start = 0x0020,
    hex_leading_exp_digit = 0x0040,
    hex_leading_exp_zeroes = 0x0080,
    hex_exp_digit_loop = 0x0100
};

#define final_state(scan_state) (scan_state & (finished | failure))
#define success(scan_state) \
    (scan_state &           \
     (leading_sig_zeroes | int_digit_loop | frac_digit_loop | leading_exp_zeroes | exp_digit_loop | finished))
#define hex_success(count, scan_state)                                                                  \
    (count - 1 > 2 && scan_state & (hex_leading_sig_zeroes | hex_int_digit_loop | hex_frac_digit_loop | \
                                    hex_leading_exp_zeroes | hex_exp_digit_loop))

#define fetch() (count++, (*ReadProc)(ReadProcArg, 0, __GetAChar))
#define unfetch(c) (*ReadProc)(ReadProcArg, c, __UngetAChar)

long double __strtold(int max_width, int (*ReadProc)(void*, int, int), void* ReadProcArg, int* chars_scanned,
                      int* overflow) {
    int scan_state = start;
    int hex_scan_state = not_hex;
    int count = 0;
    int spaces = 0;
    int c;
    decimal d = {0, 0, 0, {0, ""}};
    int sig_negative = 0;
    int exp_negative = 0;
    long exp_value = 0;
    int exp_adjust = 0;
    register long double result = 0.0;
    int sign_detected = 0;
    int radix_marker = *(unsigned char*)(__lconv).decimal_point;

    unsigned char* chptr;
    unsigned char mantissa[TARGET_FLOAT_BYTES];
    unsigned mantissa_digits;
    unsigned long exponent = 0;

    int ui;
    unsigned char uch, uch1;
    int NibbleIndex;
    int expsign = 0;
    int exp_digits = 0;
    unsigned intdigits = 0;

    *overflow = 0;
    c = fetch();

    while (count <= max_width && c != -1 && !final_state(scan_state)) {
        switch (scan_state) {
            case start:
                if (isspace(c)) {
                    c = fetch();
                    count--;
                    spaces++;
                    break;
                }

                switch (toupper(c)) {
                    case '-':
                        sig_negative = 1;

                    case '+':
                        c = fetch();
                        sign_detected = 1;
                        break;
                    case 'I':
                        c = fetch();
                        scan_state = infin_state;
                        break;

                    case 'N':
                        c = fetch();
                        scan_state = nan_state;
                        break;

                    default:
                        scan_state = sig_start;
                        break;
                }
                break;

            case infin_state: {
                int i = 1;
                char model[] = "INFINITY";

                while ((i < 8) && (toupper(c) == model[i])) {
                    i++;
                    c = fetch();
                }

                if ((i == 3) || (i == 8)) {
                    if (sig_negative) {
                        result = (float)-HUGE_VALF;
                    } else {
                        result = HUGE_VALF;
                    }

                    *chars_scanned = spaces + i + sign_detected;
                    return result;
                } else {
                    scan_state = failure;
                }

                break;
            }

            case nan_state: {
                int i = 1, j = 0;
                char model[] = "NAN(";
                char nan_arg[32] = "";
                while ((i < 4) && (toupper(c) == model[i])) {
                    i++;
                    c = fetch();
                }

                if ((i == 3) || (i == 4)) {
                    if (i == 4) {
                        while ((j < 32) && (isdigit(c) || isalpha(c) || (c == radix_marker))) {
                            nan_arg[j++] = (char)c;
                            c = fetch();
                        }

                        if (c != ')') {
                            scan_state = failure;
                            break;
                        } else {
                            j++;
                        }
                    }
                    nan_arg[j] = '\0';

                    if (sig_negative) {
                        result = -nan(nan_arg);
                    } else {
                        result = nan(nan_arg);
                    }

                    *chars_scanned = spaces + i + j + sign_detected;
                    return result;
                } else {
                    scan_state = failure;
                }
                break;
            }

            case sig_start:
                if (c == radix_marker) {
                    scan_state = frac_start;
                    c = fetch();
                    break;
                }
                if (!isdigit(c)) {
                    scan_state = failure;
                    break;
                }

                if (c == '0') {
                    c = fetch();
                    if (toupper(c) == 'X') {
                        scan_state = hex_state;
                        hex_scan_state = hex_start;
                    } else {
                        scan_state = leading_sig_zeroes;
                    }
                    break;
                }

                scan_state = int_digit_loop;
                break;

            case leading_sig_zeroes:
                if (c == '0') {
                    c = fetch();

                    break;
                }
                scan_state = int_digit_loop;
                break;

            case int_digit_loop:
                if (!isdigit(c)) {
                    if (c == radix_marker) {
                        scan_state = frac_digit_loop;
                        c = fetch();
                    } else {
                        scan_state = sig_end;
                    }
                    break;
                }
                if (d.sig.length < 20) {
                    d.sig.text[d.sig.length++] = (unsigned char)c;
                } else {
                    exp_adjust++;
                }

                c = fetch();
                break;

            case frac_start:
                if (!isdigit(c)) {
                    scan_state = failure;
                    break;
                }

                scan_state = frac_digit_loop;
                break;

            case frac_digit_loop:
                if (!isdigit(c)) {
                    scan_state = sig_end;
                    break;
                }

                if (d.sig.length < 20) {
                    if (c != '0' || d.sig.length) {
                        d.sig.text[d.sig.length++] = (unsigned char)c;
                    }

                    exp_adjust--;
                }
                c = fetch();
                break;

            case sig_end:
                if (toupper(c) == 'E') {
                    scan_state = exp_start;
                    c = fetch();
                    break;
                }
                scan_state = finished;
                break;

            case exp_start:
                if (c == '+') {
                    c = fetch();
                } else if (c == '-') {
                    c = fetch();
                    exp_negative = 1;
                }

                scan_state = leading_exp_digit;
                break;

            case leading_exp_digit:
                if (!isdigit(c)) {
                    scan_state = failure;
                    break;
                }

                if (c == '0') {
                    scan_state = leading_exp_zeroes;
                    c = fetch();
                    break;
                }

                scan_state = exp_digit_loop;
                break;

            case leading_exp_zeroes:
                if (c == '0') {
                    c = fetch();
                    break;
                }

                scan_state = exp_digit_loop;
                break;

            case exp_digit_loop:
                if (!isdigit(c)) {
                    scan_state = finished;
                    break;
                }

                exp_value = exp_value * 10 + (c - '0');
                if (exp_value > DBL_MAX_10_EXP) {
                    *overflow = 1;
                }

                c = fetch();
                break;

            case hex_state: {
                switch (hex_scan_state) {
                    case hex_start:
                        memset(mantissa, 0, sizeof(mantissa));
                        chptr = mantissa;
                        mantissa_digits = (53 + 3) / 4;
                        intdigits = 0;
                        NibbleIndex = 0;
                        hex_scan_state = hex_leading_sig_zeroes;
                        c = fetch();
                        break;

                    case hex_leading_sig_zeroes:
                        if (c == '0') {
                            c = fetch();
                            break;
                        }

                        hex_scan_state = hex_int_digit_loop;
                        break;

                    case hex_int_digit_loop:
                        if (!isxdigit(c)) {
                            if (c == radix_marker) {
                                hex_scan_state = hex_frac_digit_loop;
                                c = fetch();
                            }

                            else {
                                hex_scan_state = hex_sig_end;
                            }
                            break;
                        }

                        if (intdigits < mantissa_digits) {
                            intdigits++;
                            uch = *(chptr + NibbleIndex / 2);

                            ui = toupper(c);
                            if (ui >= 'A') {
                                ui = ui - 'A' + 10;
                            } else {
                                ui -= '0';
                            }

                            uch1 = (unsigned char)ui;

                            if ((NibbleIndex % 2) != 0) {
                                uch |= uch1;
                            } else {
                                uch |= (unsigned char)(uch1 << 4);
                            }

                            *(chptr + NibbleIndex++ / 2) = uch;
                            c = fetch();
                        }

                        else {
                            c = fetch();
                        }

                        break;

                    case hex_frac_digit_loop:
                        if (!isxdigit(c)) {
                            hex_scan_state = hex_sig_end;
                            break;
                        }

                        if (intdigits < mantissa_digits) {
                            uch = *(chptr + NibbleIndex / 2);
                            ui = toupper(c);

                            if (ui >= 'A') {
                                ui = ui - 'A' + 10;
                            } else {
                                ui -= '0';
                            }

                            uch1 = (unsigned char)ui;

                            if ((NibbleIndex % 2) != 0) {
                                uch |= uch1;
                            } else {
                                uch |= (unsigned char)(uch1 << 4);
                            }

                            *(chptr + NibbleIndex++ / 2) = uch;
                            c = fetch();
                        } else {
                            c = fetch();
                        }
                        break;

                    case hex_sig_end:
                        if (toupper(c) == 'P') {
                            hex_scan_state = hex_exp_start;
                            exp_digits++;
                            c = fetch();
                        } else {
                            scan_state = finished;
                        }

                        break;

                    case hex_exp_start:
                        exp_digits++;
                        if (c == '-') {
                            expsign = 1;
                        } else if (c != '+') {
                            c = unfetch(c);
                            count--;
                            exp_digits--;
                        }

                        hex_scan_state = hex_leading_exp_digit;
                        c = fetch();
                        break;

                    case hex_leading_exp_digit:
                        if (!isdigit(c)) {
                            scan_state = failure;
                            break;
                        }

                        if (c == '0') {
                            exp_digits++;
                            hex_scan_state = hex_leading_exp_zeroes;
                            c = fetch();
                            break;
                        }

                        hex_scan_state = hex_exp_digit_loop;
                        break;

                    case hex_leading_exp_zeroes:
                        if (c == '0') {
                            c = fetch();
                            break;
                        }

                        hex_scan_state = hex_exp_digit_loop;
                        break;

                    case hex_exp_digit_loop:
                        if (!isdigit(c)) {
                            scan_state = finished;
                            break;
                        }

                        exponent = exponent * 10 + (c - '0');

                        if (exp_value > SHRT_MAX) {
                            *overflow = 1;
                        }

                        exp_digits++;
                        c = fetch();

                        break;
                }
            } break;
        }
    }

    if (scan_state != 32768 ? !success(scan_state) : !hex_success(count, hex_scan_state)) {
        count = 0;
        *chars_scanned = 0;
    } else {
        count--;
        *chars_scanned = count + spaces;
    }

    unfetch(c);

    if (hex_scan_state == not_hex) {
        if (exp_negative) {
            exp_value = -exp_value;
        }

        {
            int n = d.sig.length;
            unsigned char* p = &d.sig.text[n];

            while (n-- && *--p == '0') {
                exp_adjust++;
            }

            d.sig.length = (unsigned char)(n + 1);

            if (d.sig.length == 0) {
                d.sig.text[d.sig.length++] = '0';
            }
        }

        if (exp_value < DBL_MIN_10_EXP || exp_value > DBL_MAX_10_EXP) {
            if (!*overflow && d.sig.text[0] == '0' && d.sig.text[1] == 0) {
                return 0.0;
            }
            *overflow = 1;
        }

        exp_value += exp_adjust;

        if (*overflow) {
            if (exp_negative) {
                return 0.0;
            } else {
                return sig_negative ? -HUGE_VAL : HUGE_VAL;
            }
        }

        d.exp = (short)exp_value;

        result = __dec2num(&d);

        if (result != 0.0 && result < LDBL_MIN) {
            *overflow = 1;
        } else if (result > LDBL_MAX) {
            *overflow = 1;
            result = HUGE_VAL;
        }

        if (sig_negative && success(scan_state)) {
            result = -result;
        }

        return result;
    } else {
        unsigned mantissa_bit, dbl_bit;
        unsigned one_bit;
        long double dbl_bits_storage;
        unsigned char* dbl_bits = (unsigned char*)&dbl_bits_storage;

        if (expsign) {
            exponent = -exponent;
        }

        exponent += intdigits * 4;

        one_bit = 0;
        while (one_bit < 4 && !(mantissa[0] & (0x80 >> one_bit))) {
            one_bit++;
            exponent--;
        }
        exponent--;

        if (TARGET_FLOAT_IMPLICIT_J_BIT) {
            one_bit++;
        }

        if (one_bit) {
            unsigned char carry = 0;
            for (chptr = mantissa + sizeof(mantissa) - 1; chptr >= mantissa; chptr--) {
                unsigned char a = *chptr;
                *chptr = (unsigned char)((a << one_bit) | carry);
                carry = (unsigned char)(a >> (8 - one_bit));
            }
        }

        memset(dbl_bits, 0, sizeof(dbl_bits_storage));
        dbl_bit = (TARGET_FLOAT_BITS - TARGET_FLOAT_MANT_BITS);

        for (mantissa_bit = 0; mantissa_bit < TARGET_FLOAT_MANT_BITS; mantissa_bit += 8) {
            unsigned char ui = mantissa[mantissa_bit >> 3];
            int halfbits;

            if (mantissa_bit + 8 > TARGET_FLOAT_MANT_BITS) {
                ui &= 0xff << (TARGET_FLOAT_MANT_BITS - mantissa_bit);
            }

            halfbits = (dbl_bit & 7);
            dbl_bits[dbl_bit >> 3] |= (unsigned char)(ui >> halfbits);
            dbl_bit += 8;
            dbl_bits[dbl_bit >> 3] |= (unsigned char)(ui << (8 - halfbits));
        }

        exponent += (TARGET_FLOAT_MAX_EXP - 1) + exp_value;

        if ((exponent & ~(TARGET_FLOAT_MAX_EXP * 2 - 1))) {
            *overflow = 1;
            return 0.0;
        }

        exponent <<= 32 - TARGET_FLOAT_EXP_BITS;

        dbl_bits[0] |= exponent >> 25;

        if (TARGET_FLOAT_EXP_BITS > 7) {
            dbl_bits[1] |= exponent >> 17;
        }

        if (TARGET_FLOAT_EXP_BITS > 15) {
            dbl_bits[2] |= exponent >> 9;
        }

        if (TARGET_FLOAT_EXP_BITS > 23) {
            dbl_bits[3] |= exponent >> 1;
        }

        if (sig_negative) {
            dbl_bits[0] |= 0x80;
        }

        result = *(long double*)dbl_bits;

        return result;
    }
}

double strtod(const char* str, char** end) {
    double value, abs_value;
    int count, overflow;

    __InStrCtrl isc;
    isc.NextChar = (char *)str;
    isc.NullCharDetected = 0;

    value = __strtold(INT_MAX, &__StringRead, (void *)&isc, &count, &overflow);

    if (end)
        *end = (char*)str + count;

    abs_value = fabs(value);

    if (overflow)
        errno = ERANGE;

    return value;
}

double atof(const char* str) {
	return strtod(str, NULL);
}
