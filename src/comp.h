#pragma once

#include <stdint.h>
#include <math.h>

#define  INTREPOFFLOAT(f)   ( *(int32_t*)&(f) )
#define  INTREPOFDOUBLE(d)  ( *(int64_t*)&(d) )

#define  MAXULPS  5
#define  MAXULPSDOUBLE  5

uint64_t abs64(int64_t x) { return x<0 ? -x : x; }

int cmpDoubles(double a, double b) {
	int64_t aInt;
	int64_t bInt;

	if (isnan(a) || isnan(b))
		return -2;

	if (isinf(a) || isinf(b))
		return (a<b) ? -1 : (a>b) ? 1 : 0;

	aInt = INTREPOFDOUBLE(a);
	bInt = INTREPOFDOUBLE(b);

	if (aInt<0)   aInt = 0x8000000000000000LL - aInt;
	if (bInt<0)   bInt = 0x8000000000000000LL - bInt;
	return (abs64(aInt - bInt) <= MAXULPSDOUBLE) ? 0
		: (aInt<bInt) ? 1
		: -1;
}
