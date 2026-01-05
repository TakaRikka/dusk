#ifndef _MSL_COMMON_ARITH_H
#define _MSL_COMMON_ARITH_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    int quot;  /* quotient */
    int rem;   /* remainder */
} div_t;

int abs(int n);
long int labs(long int n);
div_t div(int numerator, int denominator);

#ifdef __cplusplus
}
#endif

#endif /* _MSL_COMMON_ARITH_H */
