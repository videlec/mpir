/*
dnl  Copyright 2009 Jason Moxham

dnl  This file is part of the MPIR Library.

dnl  The MPIR Library is free software; you can redistribute it and/or modify
dnl  it under the terms of the GNU Lesser General Public License as published
dnl  by the Free Software Foundation; either version 2.1 of the License, or (at
dnl  your option) any later version.

dnl  The MPIR Library is distributed in the hope that it will be useful, but
dnl  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
dnl  or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
dnl  License for more details.

dnl  You should have received a copy of the GNU Lesser General Public License
dnl  along with the MPIR Library; see the file COPYING.LIB.  If not, write
dnl  to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
dnl  Boston, MA 02110-1301, USA.
*/

#include "mpir.h"
#include "gmp-impl.h"

#define ADD ADDC_LIMB
#define SUB SUBC_LIMB

#define ADC(co,w,x,y,ci) do{mp_limb_t _c1,_c2,_t;ADD(_c1,_t,x,y);ADD(_c2,w,_t,ci);co=(_c1|_c2);}while(0)
#define SBB(co,w,x,y,ci) do{mp_limb_t _c1,_c2,_t;SUB(_c1,_t,x,y);SUB(_c2,w,_t,ci);co=(_c1|_c2);}while(0)

// NOTE: compaired with addmul_1 or addmul_2 this does one more add
// addmul_basecase      (rp,xn+yn) = (rp,xn+yn) + (xp,xn)*(yp,yn)
mp_limb_t
mpn_addmul_basecase (mp_ptr rp, mp_srcptr xp, mp_size_t xn, mp_srcptr yp,
		     mp_size_t yn)
{
  mp_size_t i;
  mp_limb_t t, carry;

  ASSERT (xn != 0);
  ASSERT (yn != 0);
  ASSERT_MPN (xp, xn);
  ASSERT_MPN (yp, yn);
  ASSERT_MPN (rp, xn + yn);
  ASSERT (!MPN_OVERLAP_P (rp, xn + yn, xp, xn));
  ASSERT (!MPN_OVERLAP_P (rp, xn + yn, yp, yn));
  carry = 0;
  for (i = 0; i < yn; i++)
    {
      t = mpn_addmul_1 (rp + i, xp, xn, yp[i]);
      ADC (carry, rp[xn + i], rp[xn + i], t, carry);
    }
  return carry;
}


#if 0
// standard mul_basecase    (rp,xn+yn) = (xp,xn)*(yp,yn)
// first iteration is a mul rather than an addmul
void
mpn_mul_basecase (mp_ptr rp, mp_srcptr xp, mp_size_t xn, mp_srcptr yp,
		  mp_size_t yn)
{
  mp_size_t i;

  ASSERT (xn != 0);
  ASSERT (yn != 0);
  ASSERT_MPN (xp, xn);
  ASSERT_MPN (yp, yn);
  ASSERT (!MPN_OVERLAP_P (rp, xn + yn, xp, xn));
  ASSERT (!MPN_OVERLAP_P (rp, xn + yn, yp, yn));
  rp[xn + 0] = mpn_mul_1 (rp + 0, xp, xn, yp[0]);
  for (i = 1; i < yn; i++)
    rp[xn + i] = mpn_addmul_1 (rp + i, xp, xn, yp[i]);
  return;
}
#endif

#if 0
// halfaddmul_basecase      (rp,xn+yn) = (rp,xn) + (xp,xn)*(yp,yn)
// replace first mul in basecase with addmul
void
mpn_halfaddmul_basecase (mp_ptr rp, mp_srcptr xp, mp_size_t xn, mp_srcptr yp,
			 mp_size_t yn)
{
  mp_size_t i;

  ASSERT (xn != 0);
  ASSERT (yn != 0);
  ASSERT_MPN (xp, xn);
  ASSERT_MPN (yp, yn);
  ASSERT_MPN (rp, xn);
  ASSERT (!MPN_OVERLAP_P (rp, xn + yn, xp, xn));
  ASSERT (!MPN_OVERLAP_P (rp, xn + yn, yp, yn));
  for (i = 0; i < yn; i++)
    rp[xn + i] = mpn_addmul_1 (rp + i, xp, xn, yp[i]);
  return;
}
#endif
