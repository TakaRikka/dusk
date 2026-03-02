/* @(#)w_log10.c 1.2 95/01/04 */
/* $Id: w_log10.c,v 1.3 2004/02/26 17:03:30 ceciliar Exp $ */
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

/* 
 * wrapper log10(X)
 */

#include "fdlibm.h"

#ifdef __STDC__
	double log10(double x)		/* wrapper log10 */
#else
	double log10(x)			/* wrapper log10 */
	double x;
#endif
{
	return __ieee754_log10(x);
}
