/* @(#)s_ldexp.c 1.2 95/01/04 */
/* $Id: s_ldexp.c,v 1.3.14.1 2002/01/31 15:24:14 ceciliar Exp $ */
/*
 * ====================================================
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunPro, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice
 * is preserved.
 * ====================================================
 */

#include "global.h"

#if PLATFORM_GCN
#include <cmath> /* for isfinite macro */
#else
#include <fdlibm.h>
#define isfinite(x) ((__fpclassifyd(x) > 2))
#endif

static const double

    two54
    = 1.80143985094819840000e+16,        /* 0x43500000, 0x00000000 */
    twom54 = 5.55111512312578270212e-17, /* 0x3C900000, 0x00000000 */
    big = 1.0e+300, tiny = 1.0e-300;

double ldexp(double x, int n)
{
	long k, hx, lx; /*- cc 020130 -*/
	if (!isfinite(x) || x == 0.0)
		return x;

	hx = __HI(x);
	lx = __LO(x);
	k  = (hx & 0x7ff00000) >> 20; /* extract exponent */
	if (k == 0) {                 /* 0 or subnormal x */
		if ((lx | (hx & 0x7fffffff)) == 0)
			return x; /* +-0 */
		x *= two54;
		hx = __HI(x);
		k  = ((hx & 0x7ff00000) >> 20) - 54;
		if (n < -50000)
			return tiny * x; /*underflow*/
	}
	if (k == 0x7ff)
		return x + x; /* NaN or Inf */
	k = k + n;
	if (k > 0x7fe)
		return big * copysign(big, x); /* overflow  */
	if (k > 0)                         /* normal result */
	{
		__HI(x) = (hx & 0x800fffff) | (k << 20);
		return x;
	}
	if (k <= -54)
		if (n > 50000)                     /* in case integer overflow in n+k */
			return big * copysign(big, x); /*overflow*/
		else
			return tiny * copysign(tiny, x); /*underflow*/
	k += 54;                                 /* subnormal result */
	__HI(x) = (hx & 0x800fffff) | (k << 20);
	return x * twom54;
}
