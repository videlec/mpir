/*============================================================================

    Copyright (C) 2007, 2009 William Hart

    This file is part of MPIR.

    MPIR is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    MPIR is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with MPIR; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA

===============================================================================*/

#include <mpir.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "profiler.h"

#define MAX_WORDS 1000000

extern void
mpn_mul_fft_full_a (mp_ptr op, mp_srcptr n, mp_size_t nl, mp_srcptr m, mp_size_t ml, int a);

/* Runs FFT_aux through random data.

num_trials = number of trials to perform.
coeff_bits = number of bits to use in each coefficient. 
k = parameter we are tweaking
multiplication is mod 2^(coeff_limbs*B) + b where b is +/- 1
*/

void run_FFT_full_a(unsigned long num_trials, mp_size_t coeff_limbs, int a, int sq)
{
   unsigned long i;
   
   // alloc some space
   mp_limb_t * data1;
   mp_limb_t * data2;
   mp_limb_t * data3;
   
	data1 = (mp_limb_t*) malloc(sizeof(mp_limb_t)*coeff_limbs);
   if (!sq) data2 = (mp_limb_t*) malloc(sizeof(mp_limb_t)*coeff_limbs);
   data3 = (mp_limb_t*) malloc(sizeof(mp_limb_t)*2*coeff_limbs);
    
   for (i = 0; i < num_trials; i++)
   {
      // make up random polys
      if (i%20==0)
      {
         mpn_random(data1, coeff_limbs);
         if (!sq) mpn_random(data2, coeff_limbs);
      }
     
      ulong j;
      for (j = 0; j < 2*coeff_limbs; j++) data3[j] = 0L;
		start_clock(0);
      if (!sq) mpn_mul_fft_full_a (data3, data1, coeff_limbs, data2, coeff_limbs, a);
      else mpn_mul_fft_full_a (data3, data1, coeff_limbs, data1, coeff_limbs, a);
		stop_clock(0);
   }   
   
   // clean up
   free(data1);
   if (!sq) free(data2);
   free(data3);
}


int main (int argc, const char * argv[])
{
   double time1, time2;
   unsigned long bestk;
   double best;
   int k, besti, first;
   mp_size_t words2;
	ulong trials;
		 
   k = 5;
	first = 1;
	printf("#define MUL_FFT_FULL_TABLE2 {");
	mp_size_t words;
    for (words = 1000UL; words < MAX_WORDS; words=floor(words*pow(2.0,1.0/32.0))+1) 
   {
       best = 10000.0;
       int i;
		 for (i = -4; i < 4; i++)
       {
          init_clock(0);
			 while (k + i < 1) i++;
          if ((words < 10000) && (k + i > 6)) continue;
			 if (words < 64000) trials = 10;
			 else trials = 4;
			 run_FFT_full_a(trials, words, k + i, 0);
          time1 = get_clock(0) / 1000000000.0;
          if (time1 < best) 
          {
             best = time1;
             besti = i;
          }
       }
		 if (first) 
		 {
			 printf("{%ld, %d}, ", 16, k + besti);
			 first = 0;
		 }
		 else if (besti) printf("{%ld, %d}, ", words, k + besti);
		 fflush(stdout);
		 k = k + besti;
   }
   printf("{MP_SIZE_T_MAX,0}}\n\n");

	first = 1;
   printf("#define SQR_FFT_FULL_TABLE2 {");
	for (words = 1000UL; words < MAX_WORDS; words=floor(words*pow(2.0,1.0/32.0))+1) 
   {
       best = 10000.0;
       int i;
		 for (i = -4; i < 4; i++)
       {
          init_clock(0);
			 while (k + i < 1) i++;
          if ((words < 10000) && (k + i > 6)) continue;
			 if (words < 64000) trials = 10;
			 else trials = 4;
			 run_FFT_full_a(trials, words, k + i, 1);
          time1 = get_clock(0) / 1000000000.0;
          if (time1 < best) 
          {
             best = time1;
             besti = i;
          }
       }
		 if (first) 
		 {
			 printf("{%ld, %d}, ", 16, k + besti);
			 first = 0;
		 }
		 else if (besti) printf("{%ld, %d}, ", words, k + besti);
		 fflush(stdout);
		 k = k + besti;
   }
   printf("{MP_SIZE_T_MAX,0}}\n\n");

   return 0;
}
