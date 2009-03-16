/* mpn_mul_tc4 -- Internal routine to multiply two natural numbers
   of length n.

   THIS IS AN INTERNAL FUNCTION WITH A MUTABLE INTERFACE.  IT IS ONLY
   SAFE TO REACH THIS FUNCTION THROUGH DOCUMENTED INTERFACES.


/* Implementation of the Bodrato-Zanoni algorithm for Toom-Cook 4-way.

Copyright 2006 Free Software Foundation, Inc.
Copyright 2009 William Hart

This file is part of the GNU MP Library.

The GNU MP Library is free software; you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation; either version 2.1 of the License, or (at your
option) any later version.

The GNU MP Library is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
License for more details.

You should have received a copy of the GNU Lesser General Public License
along with the GNU MP Library; see the file COPYING.LIB.  If not, write to
the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
MA 02110-1301, USA. */
*/

/*
   This implementation is based on that of Paul Zimmmermann, which is available
	for mpz_t's at http://www.loria.fr/~zimmerma/software/toom4.c
*/

#include "mpir.h"
#include "gmp-impl.h"

#define TC4_THRESHOLD 550 /* must be >= 3 */

void mpn_mul_tc4 (mp_ptr rp, mp_srcptr up,
		              mp_srcptr vp, mp_size_t n);

void tc4_add(mp_ptr rp, mp_size_t * rn, mp_ptr r1, mp_size_t r1n, mp_ptr r2, mp_size_t r2n)
{
   mp_limb_t cy;
   mp_size_t s1 = ABS(r1n);
   mp_size_t s2 = ABS(r2n);
   
   if (s1 < s2) 
   {
      MPN_PTR_SWAP(r1, r1n, r2, r2n);
      MP_SIZE_T_SWAP(s1, s2);
   } 
   
   if (!s1)
   {
      *rn = 0;
   } else if (!s2)
   {
      if (rp != r1) MPN_COPY(rp, r1, s1);
		rn = r1n;
   } else if ((r1n ^ r2n) >= 0)
   {
      rn = r1n;
      cy = mpn_add(rp, r1, s1, r2, s2);
      if (cy) 
      {
         rp[s1] = cy;
         if (rn < 0) rn--;
         else rn++;
      }
   } else
   {
      if (s1 != s2) cy = CNST_LIMB(1);
      else cy = (mp_limb_t) MPN_CMP(r1, r2, s1); 
          
      if (!cy) rn = 0;
      else if (cy > CNST_LIMB(0)) 
      {
         mpn_sub(rp, r1, s1, r2, s2);
         rn = s1;
         MPN_NORMALIZE(rp, rn);
			if (r1 < 0) rn = -rn;
      }
      else
      {
         mpn_sub_n(rp, r2, r1, s1);
         rn = s1;
         MPN_NORMALISE(rp, rn);
			if (r1 > 0) rn = -rn;
      }
   }
}

tc4_add_unsigned(mp_ptr rp, mp_size_t * rn, mp_srcptr up, mp_size_t un, 
			               mp_srcptr vp, mp_size_t vn)
{
	mp_limb_t cy;
	mp_size_t len;
		
	if (un >= vn)
	{
		cy = mpn_add(rp, up, un, vp, vn);
		if (cy) 
		{
			rp[un] = cy;
			len = un + 1;
		} else
			len = un;
		MPN_NORMALIZE(rp, len);
		*rn = len;
	} else
	{
		cy = mpn_add(rp, vp, vn, up, un);
		if (cy) 
		{
			rp[vn] = cy;
			len = vn + 1;
		} else
			len = vn;
		MPN_NORMALIZE_NOT_ZERO(rp, len);
		*rn = len;
	}
}
 
tc4_sub_unsigned(mp_ptr rp, mp_size_t * rn, mp_srcptr up, mp_size_t un, 
			               mp_srcptr vp, mp_size_t vn)
{
	mp_size_t len;
		
	if (un == vn)
	{
		int sign;
		if (un == 0) return 0;
		MPN_CMP(sign, up, vp, un);
		if (sign == 0) return 0;
		if (sign > 0)
		{
			mpn_sub_n(rp, up, vp, un);
			len = un;
		   MPN_NORMALIZE_NOT_ZERO(rp, len);
			*rn = len;
		} else
		{
			mpn_sub_n(rp, vp, up, un);
			len = un;
		   MPN_NORMALIZE_NOT_ZERO(rp, len);
			*rn = -len;
		}
	} else if (un > vn)
	{
		mpn_sub(rp, up, un, vp, vn);
		len = un;
		MPN_NORMALIZE_NOT_ZERO(rp, len);
		*rn = len;
	} else if (vn > un)
	{
		mpn_sub(rp, vp, vn, up, un);
		len = vn;
		MPN_NORMALIZE_NOT_ZERO(rp, len);
		*rn = -len;
	}
}
	
#define MUL_TC4_UNSIGNED(r3xx, n3xx, r1xx, n1xx, r2xx, n2xx) \
   do \ 
   { \
      if (n1xx == n2xx) \
		{ \
			if (n1xx > TC4_THRESHOLD) mpn_mul_tc4(r3, r1xx, r2xx, n1xx); \
			else mpn_mul_n(r3, r1xx, r2xx, n1xx); \
		} else if (n1xx > n2xx) \
		   mpn_mul(r3, r1xx, n1xx, r2xx, n2xx); \
		else \
		   mpn_mul(r3, r2xx, n2xx, r1xx, n1xx); \
	   mp_limb_t len = n1xx + n2xx; \
		MPN_NORMALIZE(r3, len); \
		n3xx = len; \
   } while (0)

#define MUL_TC4(r3xx, n3xx, r1xx, n1xx, r2xx, n2xx) \
	do \
	{ \
	   mp_size_t sign = n1xx ^ n2xx; \
	   mp_size_t un1 = ABS(n1xx); \
	   mp_size_t un2 = ABS(n2xx); \
		MUL_TC4(r3xx, n3xx, r1xx, un1, r2xx, un2); \
		if (sign < 0) n3xx = -n3xx; \
	} while (0)

#define TC4_NORM(rxx, nxx, sxx) \
	do \
	{ \
	   nxx = sxx; \
	   MPN_NORMALISE(rxx, nxx); \
	} while(0)

/* Multiply {up, n} by {vp, n} and write the result to
   {prodp, 2n}.

   Note that prodp gets 2n limbs stored, even if the actual result
   only needs 2n - 1.
*/

mp_limb_t
mpn_mul_tc4 (mp_ptr rp, mp_srcptr up,
		          mp_srcptr vp, mp_size_t n)
{
  ASSERT (n >= 1);
  ASSERT (!MPN_OVERLAP_P(rp, 2*n, up, n));
  ASSERT (!MPN_OVERLAP_P(rp, 2*n, vp, n));

  mp_limb_t cy;
  mp_ptr tp;
  mp_size_t len1, len2, len3, len4, len5, len6, len7;
  mp_size_t a0n, a1n, a2n, a3n, b0n, b1n, b2n, b3n;
  
  sn = (n - 1) / 4 + 1;
  
#define a0 (up)
#define a1 (up + sn)
#define a2 (up + 2*sn)
#define a3 (up + 3*sn)
#define b0 (vp)
#define b1 (vp + sn)
#define b2 (vp + 2*sn)
#define b3 (vp + 3*sn)

   TC_NORM(a0, a0n, sn);
	TC_NORM(a1, a1n, sn);
	TC_NORM(a2, a2n, sn);
	TC_NORM(a3, a3n, n - 3*sn); 
   TC_NORM(b0, b0n, sn);
	TC_NORM(b1, b1n, sn);
	TC_NORM(b2, b2n, sn);
	TC_NORM(b3, b3n, n - 3*sn); 

#define t4 (2*sn+2) // allows mult of 2 integers of n4 + 1 limbs

   tp = __GMP_ALLOCATE_FUNC_LIMBS(7*t4);

#define r1 (tp)
#define r2 (tp + t4)
#define r3 (tp + 2*t4)
#define r4 (tp + 3*t4)
#define r5 (tp + 4*t4)
#define r6 (tp + 5*t4)
#define r7 (tp + 6*t4)

   tc4_add(r6, &n6, a3, a3n, a1, a1n); 
   tc4_add(r5, &n5, a2, a2n, a0, a0n); 
	tc4_add(r3, &n3, r5, n5, r6, n6); 
   tc4_sub(r4, &n4, r5, n5, r6, n6);
   tc4_add(r6, &n6, b3, b3n, b1, b1n);
   tc4_add(r5, &n5, b2, b2n, b0, b0n);
   tc4_add(r7, &n7, r5, n5, r6, n6); 
   tc4_sub(r5, &n5, r5, n5, r6, n6);
   
	MUL_TC4_UNSIGNED(r3, n3, r3, n3, r7, n7);
	MUL_TC4(r4, n4, r4, n4, r5, n5);

	tc4_lshift(r1, &n1, a0, a0n, 3);
	tc4_addlsh1(r1, &n1, a2, a2n);
	tc4_lshift(r7, &n7, a1, a1n, 2);
   tc4_add(r7, &n7, r7, n7, a3, a3n);
   tc4_add(r5, &n5, r1, n1, r7, n7);
   tc4_add(r6, &n6, r1, n1, r7, n7);
   tc4_lshift(r1, &n1, b0, b0n, 3);
	tc4_addlsh1(r1, &n1, b2, b2n);
   tc4_lshift(r7, &n7, b1, b1n, 2);
   tc4_add(r7, &n7, r7, n7, b3, b3n);
   tc4_add(r2, &n2, r1, n1, r7, n7);
   tc4_sub(r7, &n7, r1, n1, r7, n7);
   
	MUL_TC4_UNSIGNED(r5, n5, r5, n5, r2, n2);
   MUL_TC4(r6, n6, r6, n6, r7, n7);

   tc4_lshift(r2, &n2, a3, a3n, 3);
   tc4_addmul_1(r2, &n2, a2, a2n, 4);
   tc4_addlsh1(r2, &n2, a1, a1n);
	tc4_add(r2, &n2, r2, n2, a0, a0n);
   tc4_lshift(r7, &n7, b3, b3n, 3);
   tc4_addmul_1(r7, &n7, b2, b2n, 4);
   tc4_addlsh1(r7, &n7, b1, b1n);
	tc4_add(r7, &n7, r7, n7, b0, b0n);

	MUL_TC4_UNSIGNED(r2, n2, r2, n2, r7, n7);
   MUL_TC4_UNSIGNED(r1, n1, a3, a3n, b3, b3n);
   MUL_TC4_UNSIGNED(r7, n7, a0, a0n, b0, b0n);

	tc4_add(r2, &n2, r2, n2, r5, n5);
   tc4_sub(r6, &n6, r5, n5, r6, n6);
	tc4_sub(r4, &n4, r3, n3, r4, n4);
	
	tc4_sub(r5, &n5, r5, n5, r1, n1);
	tc4_submul_1(r5, &n5, r7, n7, 64);
   tc4_rshift(r4, &n4, r4, n4, 1);
	
	tc4_sub(r3, &n3, r3, n3, r4, n4);
	tc4_lshift(r5, &n5, r5, n5, 1);
	tc4_sub(r5, &n5, r5, n5, r6, n6);

   tc4_submul_1(r2, &n2, r3, n3, 65);
	tc4_sub(r3, &n3, r3, n3, r7, n7);
   tc4_sub(r3, &n3, r3, n3, r1, n1);

   tc4_addmul_1(r2, &n2, r3, n3, 45);
   tc4_submul_1(r5, &n5, r3, n3, 8);

	tc4_divexact_ui(r5, &n5, r5, n5, 24);

	tc4_sub(r6, &n6, r6, n6, r2, n2);
	tc4_submul_1(r2, &n2, r4, n4, 16);

   tc4_divexact_ui(r2, &n2, r2, n2, 18);

   tc4_sub(r3, &n3, r3, n3, r5, n5);
	tc4_sub(r4, &n4, r4, n4, r2, n2);
	
	tc4_divexact_ui(r6, &n6, r6, n6, 30);

	tc4_add(r6, &n6, r6, n6, r2, n2);
	tc4_rshift(r6, &n6, r6, n6, 1);

	tc4_sub(r2, &n2, r2, n2, r6, n6);

   rpn = 0;
	tc4_copy(rp, &rpn, 0, r6, n6);
   tc4_copy(rp, &rpn, n4, r6, n6);
   tc4_copy(rp, &rpn, 2*n4, r5, n5);
   tc4_copy(rp, &rpn, 3*n4, r4, n4);
   tc4_copy(rp, &rpn, 4*n4, r3, n3);
   tc4_copy(rp, &rpn, 5*n4, r2, n2);
   tc4_copy(rp, &rpn, 6*n4, r1, n1);

	if (rpn != 2*n) rp[rpn] = 0;

   __GMP_FREE_FUNC_LIMBS (tp, 7*t4);

	return rp[2*n - 1];
}
