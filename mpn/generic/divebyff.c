/* mpn_divexact_byff -- mpn exact division by 2^GMP_NUMB_BITS - 1.

Copyright 2008 David Harvey

This file is part of the MPIR Library.

todo: need to figure out correct license text here

*/

#include "gmp.h"
#include "gmp-impl.h"


/*
  Divides {up, n} by B-1 modulo B^n, where B = 2^GMP_NUMB_BITS.

  Source and destination may be the same, but they may not partly overlap.
 */
void
mpn_divexact_byff (mp_ptr rp, mp_srcptr up, mp_size_t un)
{
  mp_limb_t accum = 0, borrow = 0, in;
  mp_size_t i = 0;

  ASSERT (un >= 1);
  ASSERT (MPN_SAME_OR_SEPARATE_P (rp, up, un));

  do
  {
     /*
       Claim: accum - borrow never overflows in line (A) below.
       Proof: on the first iteration, accum = borrow = 0.
       After that, the only way borrow can be 1 is if accum < in
       just before (B), in which case accum will be positive after (B).
     */

      in = up[i];
      accum -= borrow;      /* (A) */
      borrow = accum < in;
      accum -= in;          /* (B) */
      accum &= GMP_NUMB_MASK;
      rp[i] = accum;
   }
   while (++i < un);
}
