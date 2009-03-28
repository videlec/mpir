/* Test mpn_addmul_basecase  mpn_submul_basecase

  Copyright 2009 Jason Moxham

  This file is part of the MPIR Library.

  The MPIR Library is free software; you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as published
  by the Free Software Foundation; either version 2.1 of the License, or (at
  your option) any later version.

  The MPIR Library is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
  or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
  License for more details.

  You should have received a copy of the GNU Lesser General Public License
  along with the MPIR Library; see the file COPYING.LIB.  If not, write
  to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
  Boston, MA 02110-1301, USA.

*/

#include <stdio.h>
#include <stdlib.h>
#include "mpir.h"
#include "gmp-impl.h"
#include "longlong.h"
#include "tests.h"

#define DISP(xp,xn)  do{int __t;printf("%s=",#xp);for(__t=(xn)-1;__t>=0;__t--)printf("%lX ",(xp)[__t]);printf("\n");}while(0)

int
main (void)
{
  gmp_randstate_ptr rands;
  int xn, yn, c;
  mp_limb_t xp[100], yp[100], r1p[100], r2p[100], r1, r2;
  tests_start ();
  rands = RANDS;

  for (xn = 1; xn < 30; xn++)
    {
      for (yn = 1; yn < 30; yn++)
	{
	  for (c = 0; c < 10; c++)
	    {
	      mpn_random (xp, xn);
	      mpn_random (yp, yn);
	      mpn_random (r1p, xn + yn);
	      mpn_mul_basecase (r2p, xp, xn, yp, yn);
	      r2 = mpn_add_n (r2p, r1p, r2p, xn + yn);
	      r1 = mpn_addmul_basecase (r1p, xp, xn, yp, yn);
	      if (r1 != r2 || mpn_cmp (r1p, r2p, xn + yn) != 0)
		{
		  printf ("mpn_addmul_basecase error\n");
		  abort ();
		}
	      mpn_random (xp, xn);
	      mpn_random (yp, yn);
	      mpn_random (r1p, xn + yn);
	      mpn_mul_basecase (r2p, xp, xn, yp, yn);
	      r2 = mpn_sub_n (r2p, r1p, r2p, xn + yn);
	      r1 = mpn_submul_basecase (r1p, xp, xn, yp, yn);
	      if (r1 != r2 || mpn_cmp (r1p, r2p, xn + yn) != 0)
		{
		  printf ("mpn_submul_basecase error\n");
		  abort ();
		}
	    }
	}
    }

  tests_end ();
  exit (0);
}
