#include <wcstoul.h>

#include <errno.h>
#include <wctype_api.h>
#include <wscanf.h>
#include <climits>
#include <locale>

enum scan_states {
    start = 0x01,
    check_for_zero = 0x02,
    leading_zero = 0x04,
    need_digit = 0x08,
    digit_loop = 0x10,
    finished = 0x20,
    failure = 0x40
};

#define final_state(scan_state) (scan_state & (finished | failure))
#define success(scan_state) (scan_state & (leading_zero | digit_loop | finished))
#define fetch() (count++, (*read_proc)(read_proc_arg, 0, 0))

static size_t __wcstoul(int base, int max_width,
                 wchar_t (*read_proc)(wString* src, wchar_t param_1, wchar_t param_2),
                 void* read_proc_arg, int* chars_scanned, int* negative, int* overflow) {
    int scan_state;
    int count;
    int spaces;
    unsigned int value;
    unsigned int value_max;
    wchar_t c;

    count = 0;
    scan_state = 1;
    spaces = 0;
    value = 0;
    value_max = 0;

    *negative = *overflow = 0;

    if (base < 0 || base == 1 || base > 36 || max_width < 1) {
        scan_state = failure;
    } else {
        c = fetch();
    }

    if (base != 0) {
        value_max = ULONG_MAX / base;
    }

    // NOTE: c is uninitialized if the parameter validation above fails. In practice, this doesn't
    //       matter because the third condition will always fail in this scenario anyway.
    while (count <= max_width && c != -1 && !final_state(scan_state)) {
        switch (scan_state) {
        case start:
            if (c >= 0x100 ?
                         0 :
                         _current_locale.ctype_cmpt_ptr->wctype_map_ptr[c] & wctype_space) {
                c = read_proc(read_proc_arg, 0, 0);
                spaces++;
            } else {
                if (c == L'+') {
                    c = fetch();
                } else if (c == L'-') {
                    c = fetch();
                    *negative = 1;
                }
                scan_state = check_for_zero;
            }
            break;
        case check_for_zero:
            if ((base == 0 || base == 16) && c == L'0') {
                scan_state = leading_zero;
                c = fetch();
            } else {
                scan_state = need_digit;
            }
            break;
        case leading_zero:
            if (c == L'X' || c == L'x') {
                base = 16;
                scan_state = need_digit;
                c = fetch();
            } else {
                if (base == 0) {
                    base = 8;
                }
                scan_state = digit_loop;
            }
            break;
        case need_digit:
        case digit_loop:
            if (base == 0) {
                base = 10;
            }

            if (value_max == 0) {
                value_max = ULONG_MAX / base;
            }

            if (c >= 0x100 ? 0 : _current_locale.ctype_cmpt_ptr->wctype_map_ptr[c] & wctype_digit) {
                c -= L'0';
                if (c >= base) {
                    if (scan_state == digit_loop) {
                        scan_state = finished;
                    } else {
                        scan_state = failure;
                    }
                    c += L'0';
                    break;
                }
            } else {
                int temp;
                if (!(c >= 0x100 ?
                         0 :
                        _current_locale.ctype_cmpt_ptr->wctype_map_ptr[c] & wctype_alpha)) {
                    goto label;
                }

                temp = 1;
                if (c <= 0xFF) {
                    temp = 0;
                }
                if ((temp ? c :
                                  _current_locale.ctype_cmpt_ptr->upper_map_ptr[c]) -
                        0x37 >=
                    base) {
                    label:
                    if (scan_state == digit_loop) {
                        scan_state = finished;
                    } else {
                        scan_state = failure;
                    }
                    continue;
                } else {
                    c = (wchar_t)(c >= 0x100 ?
                                           c :
                                           _current_locale.ctype_cmpt_ptr->wupper_map_ptr[c]) -
                             0x37;
                }
            }

            if (value > value_max) {
                *overflow = 1;
            }

            value *= base;
            if (c > ULONG_MAX - value) {
                *overflow = 1;
            }

            scan_state = digit_loop;
            value += c;
            c = fetch();

            break;
        }
    }

    if (!success(scan_state)) {
        value = 0;
        *chars_scanned = 0;
    } else {
        *chars_scanned = count + spaces - 1;
    }

    read_proc(read_proc_arg, c, 1);

    return value;
}

size_t wcstoul(wchar_t* param_1, wchar_t** param_2, int param_3) {
    wString sp18;
    size_t retval;
    int sp10[2]; // not sure if this should be an array, but the stack doesn't match otherwise
    int sp0C;
    int sp08;
    sp18.buffer = param_1;
    sp18.field_0x4 = 0;

    retval = __wcstoul(param_3, 0x7fffffff, __wStringRead, &sp18, sp10, &sp0C, &sp08);

    if (param_2 != NULL) {
        *param_2 = param_1 + sp10[0];
    }

    if (sp08 != 0) {
        errno = 0x22;
        return -1;
    }

    if (sp0C != 0) {
        return -retval;
    } else {
        return retval;
    }
}

size_t wcstol(wchar_t* param_1, wchar_t** param_2, int param_3) {
    size_t retval;
    wString sp18;
    int sp10[2]; // not sure if this should be an array, but the stack doesn't match otherwise
    int sp0C;
    int sp08;
    sp18.buffer = param_1;
    sp18.field_0x4 = 0;

    retval = __wcstoul(param_3, 0x7fffffff, __wStringRead, &sp18, sp10, &sp0C, &sp08);

    if (param_2 != NULL) {
        *param_2 = param_1 + sp10[0];
    }

    if (sp08 != 0 || (sp0C == 0 && retval > 0x7fffffff) || (sp0C != 0 && retval > 0x80000000)) {
        errno = 0x22;
        return sp0C != 0 ? 0x80000000 : 0x7fffffff;
    }

    if (sp0C != 0) {
        return -retval;
    } else {
        return retval;
    }
}
