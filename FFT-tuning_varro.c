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

extern int
_mpn_mul_fft_aux (mp_ptr op, mp_size_t pl,
                 mp_srcptr n, mp_size_t nl,
                 mp_srcptr m, mp_size_t ml,
                 mp_size_t k, int b);
/* 
   Returns smallest possible number of limbs >= pl for a fft of size 2^k,
   i.e. smallest multiple of 2^k >= pl.
*/

mp_size_t
mpn_fft_next_size (mp_size_t pl, int k)
{
  pl = 1 + ((pl - 1) >> k); /* ceil (pl/2^k) */
  return pl << k;
}

/* Runs FFT_aux through random data.

num_trials = number of trials to perform.
coeff_bits = number of bits to use in each coefficient. 
k = parameter we are tweaking
multiplication is mod 2^(coeff_limbs*B) + b where b is +/- 1
*/

void run_FFT_aux(unsigned long num_trials, mp_size_t coeff_limbs, mp_size_t k, mp_size_t b, int sq)
{
   unsigned long i;
   
   // alloc some space
   mp_limb_t * data1;
   mp_limb_t * data2;
   mp_limb_t * data3;
	data1 = (mp_limb_t*) malloc(sizeof(mp_limb_t)*coeff_limbs);
   if (!sq) data2 = (mp_limb_t*) malloc(sizeof(mp_limb_t)*coeff_limbs);
   data3 = (mp_limb_t*) malloc(sizeof(mp_limb_t)*coeff_limbs);
    
   for (i = 0; i < num_trials; i++)
   {
      // make up random polys
      if (i%20==0)
      {
         mpn_random(data1, coeff_limbs);
         if (!sq) mpn_random(data2, coeff_limbs);
      }
		start_clock(0);
      if (!sq) mpn_mul_fft_aux (data3, coeff_limbs, data1, coeff_limbs, data2, coeff_limbs, k, b);
      else mpn_mul_fft_aux (data3, coeff_limbs, data1, coeff_limbs, data1, coeff_limbs, k, b);
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
   mp_size_t k, besti, first;
   mp_size_t words2;
	ulong trials;
		 
   k = 4;
	first = 1;
	printf("#define MUL_FFT_TABLE2 {");
	mp_size_t words;
    for (words = 200UL; words < MAX_WORDS; words=floor(words*pow(2.0,1.0/32.0))+1) 
   {
       best = 10000.0;
       mp_size_t i;
		 for (i = -2; i < 2; i++)
       {
          init_clock(0);
			 while (k + i < 2) i++;
          words2 = mpn_fft_next_size(words, k + i);
          //printf("words = %ld, words2 = %ld, k + i = %ld\n", words, words2, k + i);
			 if (words2 < 64000) trials = 10;
			 else trials = 4;
			 run_FFT_aux(trials, words2, k + i, 1, 0);
          time1 = get_clock(0) / 1000000000.0;
          if (time1 < best) 
          {
             best = time1;
             besti = i;
          }
       }
		 if (first) 
		 {
			 printf("{%ld, %d}, ", 1, k + besti);
			 first = 0;
		 }
		 else if (besti) printf("{%ld, %d}, ", words, k + besti);
		 fflush(stdout);
		 k = k + besti;
   }
   printf("{MP_SIZE_T_MAX,0}}\n\n");

   k = 4;
	first = 1;
	printf("#define MUL_FFTM_TABLE2 {");
	for (words = 200UL; words < MAX_WORDS; words=floor(words*pow(2.0,1.0/32.0))+1) 
   {
       best = 10000.0;
       int i;
		 for (i = -2; i < 2; i++)
       {
          init_clock(0);
          while (k + i < 2) i++;
          words2 = mpn_fft_next_size(words, k + i);
          //printf("words = %ld, words2 = %ld, k + i = %ld\n", words, words2, k + i);
			 if (words2 < 64000) trials = 10;
			 else trials = 4;
			 run_FFT_aux(trials, words2, k + i, -1, 0);
          time1 = get_clock(0) / 1000000000.0;
          if (time1 < best) 
          {
             best = time1;
             besti = i;
          }
       }
		 if (first) 
		 {
			 printf("{%ld, %d}, ", 1, k + besti);
			 first = 0;
		 }
		 else if (besti) printf("{%ld, %d}, ", words, k + besti);
		 fflush(stdout);
		 k = k + besti;
   }
   printf("{MP_SIZE_T_MAX,0}}\n\n");

   k = 4;
	first = 1;
	printf("#define SQR_FFT_TABLE2 {");
	for (words = 200UL; words < MAX_WORDS; words=floor(words*pow(2.0,1.0/32.0))+1) 
   {
       best = 10000.0;
       int i;
		 for (i = -2; i < 2; i++)
       {
          init_clock(0);
          while (k + i < 2) i++;
          words2 = mpn_fft_next_size(words, k + i);
          //printf("words = %ld, words2 = %ld, k + i = %ld\n", words, words2, k + i);
			 if (words2 < 64000) trials = 10;
			 else trials = 4;
			 run_FFT_aux(trials, words2, k + i, 1, 1);
          time1 = get_clock(0) / 1000000000.0;
          if (time1 < best) 
          {
             best = time1;
             besti = i;
          }
       }
		 if (first) 
		 {
			 printf("{%ld, %d}, ", 1, k + besti);
			 first = 0;
		 }
		 else if (besti) printf("{%ld, %d}, ", words, k + besti);
		 fflush(stdout);
		 k = k + besti;
   }
   printf("{MP_SIZE_T_MAX,0}}\n\n");

   k = 4;
	first = 1;
	printf("#define SQR_FFTM_TABLE2 {");
	for (words = 200UL; words < MAX_WORDS; words=floor(words*pow(2.0,1.0/32.0))+1) 
   {
       best = 10000.0;
       int i;
		 for (i = -2; i < 2; i++)
       {
          init_clock(0);
          while (k + i < 2) i++;
          words2 = mpn_fft_next_size(words, k + i);
          //printf("words = %ld, words2 = %ld, k + i = %ld\n", words, words2, k + i);
			 if (words2 < 64000) trials = 10;
			 else trials = 4;
			 run_FFT_aux(trials, words2, k + i, -1, 1);
          time1 = get_clock(0) / 1000000000.0;
          if (time1 < best) 
          {
             best = time1;
             besti = i;
          }
       }
		 if (first) 
		 {
			 printf("{%ld, %d}, ", 1, k + besti);
			 first = 0;
		 }
		 else if (besti) printf("{%ld, %d}, ", words, k + besti);
		 fflush(stdout);
		 k = k + besti;
   }
   printf("{MP_SIZE_T_MAX,0}}\n\n");

   return 0;
}
