#include "scanf.h"
#include <cctype>
#include <cmath>
#include <cstdio>
#include <cstdint>
#include <strtoul.h>
#include <strtold.h>

enum argument_options {
    normal_argument,
    char_argument,
    short_argument,
    long_argument,
    intmax_argument,
    size_t_argument,
    ptrdiff_argument,
    long_long_argument,
    double_argument,
    long_double_argument,
    wchar_argument
};

typedef unsigned char char_map[32];

typedef struct {
    unsigned char suppress_assignment;
    unsigned char field_width_specified;
    unsigned char argument_options;
    unsigned char conversion_char;
    int           field_width;
    char_map char_set;
} scan_format;

#define set_char_map(map, ch) map[(unsigned char)ch >> 3] |= (1 << (ch & 7))
#define tst_char_map(map, ch) (map[(unsigned char)ch >> 3] &  (1 << (ch & 7)))

static const char* parse_format(const char* format_string, scan_format *format) {
    const char* s = format_string;
    int c;
    int flag_found, invert;
    scan_format f = { 0, 0, normal_argument, 0, 2147483647, { 0 } };

    if (((c = *++s) == '%')) {
        f.conversion_char = c;
        *format = f;
        return ((const char*)s + 1);
    }

    if (c == '*') {
        f.suppress_assignment = 1;
        c = *++s;
    }

    if (isdigit(c)) {
        f.field_width = 0;

        do {
            f.field_width = (f.field_width * 10) + (c - '0');
            c = *++s;
        } while (isdigit(c));

        if (f.field_width == 0) {
            f.conversion_char = 0xFF;
            *format = f;
            return ((const char*)s + 1);
        }

        f.field_width_specified = 1;
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
        case 'z':
            f.argument_options = size_t_argument;
            break;
        case 't':
            f.argument_options = ptrdiff_argument;
            break;
        default:
            flag_found = 0;
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

            break;

        case 'a':
        case 'A':
        case 'f':
        case 'F':
        case 'e':
        case 'E':
        case 'g':
        case 'G':
            if (f.argument_options == char_argument || f.argument_options == short_argument || f.argument_options == intmax_argument || f.argument_options == size_t_argument || f.argument_options == ptrdiff_argument || f.argument_options == long_long_argument) {
                f.conversion_char = 0xFF;
                break;
            }

            if (f.argument_options == long_argument) {
                f.argument_options = double_argument;
            }

            break;

        case 'p':
            f.argument_options = long_argument;
            f.conversion_char = 'x';
            break;

        case 'c':
            if (f.argument_options == long_argument) {
                f.argument_options = wchar_argument;
            }
            else {
                if (f.argument_options != normal_argument) {
                    f.conversion_char = 0xFF;
                }
            }

            break;

        case 's':
            if (f.argument_options == long_argument) {
                f.argument_options = wchar_argument;
            }
            else {
                if (f.argument_options != normal_argument) {
                    f.conversion_char = 0xFF;
                }
            }

            {
                int i;
                unsigned char* p;

                for (i = sizeof(f.char_set), p = f.char_set; i; --i) {
                    *p++ = 0xFF;
                }

                f.char_set[1] = 0xC1;
                f.char_set[4] = 0xFE;
            }

            break;

        case 'n':
            break;

        case '[':
            if (f.argument_options == long_argument) {
                f.argument_options = wchar_argument;
            }
            else {
                if (f.argument_options != normal_argument) {
                    f.conversion_char = 0xFF;
                }
            }

            c = *++s;
            invert = 0;

            if (c == '^') {
                invert = 1;
                c = *++s;
            }

            if (c == ']') {
                set_char_map(f.char_set, ']');
                c = *++s;
            }

            while (c && c != ']') {
                int d;
                set_char_map(f.char_set, c);

                if (*(s + 1) == '-' && (d = *(s + 2)) != 0 && d != ']') {
                    while (++c <= d) {
                        set_char_map(f.char_set, c);
                    }

                    c = *(s += 3);
                }
                else {
                    c = *++s;
                }
            }

            if (!c) {
                f.conversion_char = 0xFF;
                break;
            }

            if (invert) {
                int i;
                unsigned char* p;

                for (i = sizeof(f.char_set), p = f.char_set; i; --i, ++p) {
                    *p = ~*p;
                }

                break;
            }

            break;
        default:
            f.conversion_char = 0xFF;
            break;
    }

    *format = f;
    return ((const char*)s + 1);
}

static int __sformatter(int (*ReadProc)(void *, int, int), void * ReadProcArg, const char * format_str, va_list arg, int is_secure)
{
    int num_chars, chars_read, items_assigned, conversions;
    int base, negative, overflow;
    int rval;
    const char* format_ptr;
    char format_char;
    char c;
    scan_format format;
    long long_num;
    unsigned long u_long_num;
    long long long_long_num = 0;
    unsigned long long u_long_long_num;
    long double long_double_num;
    char * arg_ptr;
    int elem_valid;
    size_t elem_maxsize;
    int match_failure = 0;
    int terminate = 0;

    format_ptr = format_str;
    chars_read = 0;
    items_assigned = 0;
    conversions = 0;
    
    while (!terminate && (format_char = *format_ptr) != 0)
    {
        if (isspace(format_char))
        {
            do{
                format_char = *++format_ptr;
            } while (isspace(format_char));
            
            if (!match_failure)
            {
                while (isspace(c = (*ReadProc)(ReadProcArg, 0, __GetAChar)))
                    ++chars_read;
                
                (*ReadProc)(ReadProcArg, c, __UngetAChar);
            }
            continue;
        }
        
        if ((format_char != '%') && (!match_failure))
        {
            if ((c = (*ReadProc)(ReadProcArg, 0, __GetAChar)) != (unsigned char)format_char)
            {
                (*ReadProc)(ReadProcArg, c, __UngetAChar);
                if (!is_secure)
                    goto exit;
                else
                {
                    match_failure = 1;
                    ++format_ptr;
                    continue;
                }
            }
            
            chars_read++;
            format_ptr++;
            
            continue;
        }

        format_ptr = parse_format(format_ptr, &format);

        if (!format.suppress_assignment && format.conversion_char != '%')
        {
            arg_ptr = va_arg(arg, char *);
        }
        else
        {
            arg_ptr = 0;
        }
            
        if ((format.conversion_char != 'n') && (!match_failure) && (*ReadProc)(ReadProcArg, 0, __TestForError))
        {
            if (!is_secure)
                goto exit;
            else
                match_failure = 1;
        }
        
        switch (format.conversion_char)
        {
            case 'd':
                base = 10;
                goto signed_int;
            case 'i':
                base = 0;
            signed_int:
                
                if (match_failure)
                {
                    long_num = 0;
                    long_long_num = 0;
                }
                else
                {
                    if ((format.argument_options == long_long_argument) || (format.argument_options == intmax_argument))
                        u_long_long_num = __strtoull(base, format.field_width, ReadProc, ReadProcArg, &num_chars, &negative, &overflow);
                    else
                        u_long_num = __strtoul(base, format.field_width, ReadProc, ReadProcArg, &num_chars, &negative, &overflow);
                    
                    if (!num_chars)
                    {
                        if (!is_secure)
                            goto exit;
                        else
                        {
                            match_failure = 1;
                            long_num = 0;
                            long_long_num = 0;
                            goto signed_int_assign;
                        }
                    }
                    
                    chars_read += num_chars;
                    
                    if ((format.argument_options == long_long_argument) || (format.argument_options == intmax_argument))
                        long_long_num = (negative ? -u_long_long_num : u_long_long_num);
                    else
                        long_num = (negative ? -u_long_num : u_long_num);
                }
                
            signed_int_assign:

                if (arg_ptr)
                {
                    switch (format.argument_options)
                    {
                        case normal_argument:  * (int *)         arg_ptr = long_num; break;
                        case char_argument:    * (signed char *) arg_ptr = long_num; break;
                        case short_argument:   * (short *)       arg_ptr = long_num; break;
                        case long_argument:    * (long *)        arg_ptr = long_num; break;
                        case intmax_argument:  * (intmax_t *)    arg_ptr = long_long_num; break;
                        case size_t_argument:  * (size_t *)      arg_ptr = long_num; break;
                        case ptrdiff_argument: * (ptrdiff_t *)   arg_ptr = long_num; break;
                        case long_long_argument: * (long long *) arg_ptr = long_long_num; break;
                    }
                    if (!match_failure) items_assigned++;
                }

                conversions++;
                break;
            case 'o':
                base = 8;
                goto unsigned_int;
            case 'u':
                base = 10;
                goto unsigned_int;
            case 'x':
            case 'X':
                base = 16;
            unsigned_int:
                
                if (match_failure)
                {
                    u_long_num = 0;
                    u_long_long_num = 0;
                }
                else
                {
                    if ((format.argument_options == long_long_argument) || (format.argument_options == intmax_argument))
                       u_long_long_num = __strtoull(base, format.field_width, ReadProc, ReadProcArg, &num_chars, &negative, &overflow);
                    else
                        u_long_num = __strtoul(base, format.field_width, ReadProc, ReadProcArg, &num_chars, &negative, &overflow);
                    
                    if (!num_chars)
                    {
                        if (!is_secure)
                            goto exit;
                        else
                        {
                            match_failure = 1;
                            u_long_num = 0;
                            u_long_long_num = 0;
                            goto unsigned_int_assign;
                        }
                    }
                    
                    chars_read += num_chars;
                    
                    if (negative)
                    if (format.argument_options == long_long_argument)
                        u_long_long_num = -u_long_long_num;
                    else
                        u_long_num = -u_long_num;
                }
                
            unsigned_int_assign:

                if (arg_ptr)
                {
                    switch (format.argument_options)
                    {
                        case normal_argument:  * (unsigned int *)arg_ptr = u_long_num; break;
                        case char_argument:    * (unsigned char *)arg_ptr = u_long_num; break;
                        case short_argument:   * (unsigned short *)arg_ptr = u_long_num; break;
                        case long_argument:    * (unsigned long *)arg_ptr = u_long_num; break;
                        case intmax_argument:  * (intmax_t *)    arg_ptr = u_long_long_num; break;
                        case size_t_argument:  * (size_t *)      arg_ptr = u_long_num; break;
                        case ptrdiff_argument: * (ptrdiff_t *)   arg_ptr = u_long_num; break;
                        case long_long_argument: * (unsigned long long *) arg_ptr = u_long_long_num; break;


                    }
                    
                    if (!match_failure) items_assigned++;
                }
                
                conversions++;
                break;
            case 'a':
            case 'A':
            case 'f':
            case 'F':
            case 'e':
            case 'E':
            case 'g':
            case 'G':
            flt:
                if (match_failure) long_double_num = NAN;
                else
                {
                    long_double_num = __strtold(format.field_width, ReadProc, ReadProcArg, &num_chars, &overflow);
                    
                    if (!num_chars)
                    {
                        if (!is_secure)
                            goto exit;
                        else
                        {
                            match_failure = 1;
                            long_double_num = NAN;
                            goto assign_float;
                        }
                    }
                    
                    chars_read += num_chars;
                }
                
            assign_float:

                if (arg_ptr)
                {
                    switch (format.argument_options)
                    {
                        case normal_argument:      * (float *)       arg_ptr = long_double_num; break;
                        case double_argument:      * (double *)      arg_ptr = long_double_num; break;
                        case long_double_argument: * (long double *) arg_ptr = long_double_num; break;
                    }
                    
                    if (!match_failure) items_assigned++;
                }

                conversions++;
                break;

            case 'c':
                
                if (!format.field_width_specified) format.field_width = 1;

                if (arg_ptr)
                {
                    if (is_secure)
                    {
                        elem_valid = 1;
                        elem_maxsize = va_arg(arg, size_t);
                    }
                    
                    num_chars = 0;
                    
                    if (match_failure)
                    {
                        if (elem_maxsize > 0)
                            *arg_ptr = 0;
                        continue;
                    }
                    else
                    {
                        char * arg_start = arg_ptr;
                        
                        while (format.field_width-- && (!is_secure || ((elem_valid = (elem_maxsize > num_chars)) != 0)) && ((rval = ((*ReadProc)(ReadProcArg, 0, __GetAChar))) != -1))
                        {
                            c = rval;
                            
                            if (format.argument_options == wchar_argument)
                            {
                                mbtowc(((wchar_t*)arg_ptr), (char*)(&c), 1);
                                arg_ptr += sizeof(wchar_t);
                            }
                            else
                                *arg_ptr++ = c;
                            num_chars++;
                        }
                        
                        c = rval;
                        if (!num_chars || (is_secure && !elem_valid))
                        {
                            if (!is_secure)
                                goto exit;
                            else
                            {
                                match_failure = 1;
                                if (elem_maxsize > 0)
                                    *arg_start = 0;
                                continue;
                            }
                        }
                        
                        chars_read += num_chars;
                        
                        items_assigned++;
                    }
                }
                else
                {
                    num_chars = 0;
                    
                    while (format.field_width-- && ((rval = ((*ReadProc)(ReadProcArg, 0, __GetAChar))) != -1))
                    {
                        c = rval;
                        num_chars++;
                    }
                    c = rval;
                    if (!num_chars) goto exit;
                }
                
                conversions++;
                break;
            
            case '%':
                if (match_failure)
                    continue;
                else
                {
                    while (isspace(c = (*ReadProc)(ReadProcArg, 0, __GetAChar)))
                        chars_read++;
                    
                    if (c != '%')
                    {
                        (*ReadProc)(ReadProcArg, c, __UngetAChar);
                        if (!is_secure)
                            goto exit;
                        else
                        {
                            match_failure = 1;
                            continue;
                        }
                    }
                    
                    chars_read++;
                }
                break;
            
            case 's':
                if (!match_failure)
                {
                    c = (*ReadProc)(ReadProcArg, 0, __GetAChar);
                    while(isspace(c))
                    {
                        chars_read++;
                        c = (*ReadProc)(ReadProcArg, 0, __GetAChar);
                    }
                    
                    (*ReadProc)(ReadProcArg, c, __UngetAChar);
                }
            case '[':
                if (arg_ptr)
                {
                    if (is_secure)
                    {
                        elem_valid = 1;
                        elem_maxsize = va_arg(arg, size_t) - 1;
                    }
                    
                    num_chars = 0;
                    
                    if (match_failure)
                    {
                        if (elem_maxsize > 0)
                            *arg_ptr = 0;
                        continue;
                    }
                    else
                    {
                        char * arg_start = arg_ptr;
                        
                        while (format.field_width-- &&
                            (!is_secure || ((elem_valid = (elem_maxsize >= num_chars)) != 0)) &&
                                    ((rval = ((*ReadProc)(ReadProcArg, 0, __GetAChar))) != -1))
                        {
                            c = rval;
                            
                            if (!tst_char_map(format.char_set, c))
                                break;

                            if (format.argument_options == wchar_argument)
                            {
                                mbtowc(((wchar_t*)arg_ptr), (char*)&c, 1);
                                arg_ptr = (char*)((wchar_t*)arg_ptr + 1);
                            }
                            else
                                *arg_ptr++ = c;
                            num_chars++;
                        }
                        c = rval;
                        
                        if (!num_chars || (is_secure && !elem_valid))
                        {
                            (*ReadProc)(ReadProcArg, c, __UngetAChar);
                            if (!is_secure)
                                goto exit;
                            else
                            {
                                match_failure = 1;
                                if (elem_maxsize > 0)
                                    *arg_start = 0;
                                continue;
                            }
                        }
                        
                        chars_read += num_chars;
                        
                        if (format.argument_options == wchar_argument)
                            *(wchar_t*)arg_ptr = L'\0';
                        else
                            *arg_ptr = 0;
                        
                        items_assigned++;
                    }
                }
                else
                {
                    num_chars = 0;
                    
                    while (format.field_width-- &&
                        ((rval = ((*ReadProc)(ReadProcArg, 0, __GetAChar))) != -1))
                    {
                        c = rval;
                        if (!tst_char_map(format.char_set, c))
                            break;
                        
                        num_chars++;
                    }
                    c = rval;
                    
                    if (!num_chars)
                    {
                        (*ReadProc)(ReadProcArg, c, __UngetAChar);
                        break;
                    }
                    chars_read += num_chars;
                }
                
                if (format.field_width >= 0)
                    (*ReadProc)(ReadProcArg, c, __UngetAChar);
                
                conversions++;
                break;
            case 'n':
                if (arg_ptr)
                    switch (format.argument_options)
                    {
                        case normal_argument: * (int *)   arg_ptr = chars_read; break;
                        case short_argument:  * (short *) arg_ptr = chars_read; break;
                        case long_argument:   * (long *)  arg_ptr = chars_read; break;
                        case char_argument:   * (char *)  arg_ptr = chars_read; break;
                        case long_long_argument: * (long long *) arg_ptr = chars_read; break;
                    }
                continue;
            case 0xFF:
            default:
                goto exit;
        }
    }
    
exit:
    
    if ((*ReadProc)(ReadProcArg, 0, __TestForError) && conversions == 0)
        return -1;
    
    return items_assigned;
}

int __StringRead(void* pPtr, int ch, int act) {
    char ret;
    __InStrCtrl* Iscp = (__InStrCtrl*)pPtr;

    switch (act) {
    case __GetAChar:
        ret = *(Iscp->NextChar);

        if (ret == '\0') {
            Iscp->NullCharDetected = 1;
            return -1;
        } else {
            Iscp->NextChar++;
            return (unsigned char)ret;
        }

    case __UngetAChar:
        if (Iscp->NullCharDetected == 0) {
            Iscp->NextChar--;
        } else {
            Iscp->NullCharDetected = 0;
        }

        return ch;

    case __TestForError:
        return Iscp->NullCharDetected;
    }

    return 0;
}

int isspace_string(const char *s) {
    int i = 0;

    while(s[i] != '\0'){
       if(!isspace(s[i++])) return 0;
    }

    return 1;
}

int vsscanf(const char *s, const char *format, va_list arg) {
    __InStrCtrl isc;
    isc.NextChar = (char*)s;

    if ((s == 0) || (*isc.NextChar == '\0') || isspace_string(s)) {
        return -1;
    }

    isc.NullCharDetected = 0;
    return __sformatter(&__StringRead, (void*)&isc, format, arg, 0);
}

int sscanf(const char *s, const char *pFormat, ...) {
    int ret;
    va_list args;
    va_start(args, pFormat);
    return vsscanf(s, pFormat, args);
}
