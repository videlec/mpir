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

// d is mormalized
#define udiv_inverse(i,d)  do{mp_limb_t __X;udiv_qrnnd(i,__X,~(d),GMP_LIMB_MAX,d);}while(0)

// note double lenght inverse can easily be calculated from single length inverse
// d is normalize
#define udiv_double_inverse(ih,il,d) \
  do{mp_limb_t __X;udiv_qrnnd(ih,__X,~(d),GMP_LIMB_MAX,d);udiv_qrnnd(il,__X,__X,GMP_LIMB_MAX,d);}while(0)



// set to 1=store or 0=not store
#define STORE_QUOTIENT 1
// set to 0=udiv  1=gmp-preinv   2-barrett
#define UDIV_METHOD 0

#if UDIV_NEEDS_NORMALIZATION==1 || UDIV_METHOD==1
#define NORMALIZE 1
#else
#define NORMALIZE 0
#endif

#if UDIV_METHOD==0
#define UDIV(q,r,h,l,d,i)	udiv_qrnnd(q,r,h,l,d)
#endif

#if UDIV_METHOD==1
#define UDIV	udiv_qrnnd_preinv
#endif

#if UDIV_METHOD==2
#define UDIV	udiv_qrnnd_barrett
#endif


//basic div
mp_limb_t	mpn_divrem_euclidean_1(mp_ptr qp,mp_srcptr xp,mp_size_t n,mp_limb_t d)
{int j;mp_limb_t r=0,s=0,h,l,q,i;

ASSERT(n>0);ASSERT(d!=0);ASSERT_MPN(xp,n);ASSERT(MPN_SAME_OR_SEPARATE_P(qp,xp,n));
#if NORMALIZE==1
count_leading_zeros(s,d);d=d<<s;
invert_limb(i,d);
#endif
for(j=n-1;j>=0;j--)
   {h=r;l=xp[j];
    #if STORE_QUOTIENT
    h=h+(l>>((GMP_LIMB_BITS-1-s))>>1);l=l<<s;
    UDIV(q,r,h,l,d,i);
    qp[j]=q;
    #else
    UDIV(q,r,h,l,d,i);
    #endif
   }
#if ! STORE_QUOTIENT
h=r>>(GMP_LIMB_BITS-1-s)>>1;l=r<<s;
UDIV(q,r,h,l,d,i);
#endif
r>>=s;
return r;}    // so (xp,n) = (qp,n)*d +r   and 0 <= r < d




int main(void)
{mp_limb_t  qp[2000],xp[2000],yp[2000],tp[2000],d,m,r1,r2;mp_size_t n,j,k;

for(k=0;k<64;k++){
for(j=0;j<100;j++){
for(n=1;n<100;n+=1)
   {
    do{mpn_random(&d,1);d>>=k;}while(d==0);
    mpn_random(xp,n);if(j>50)xp[n-1]=mpn_mul_1(xp,xp,n-1,d);

    r1=basicdiv_1(qp,xp,n,d);
    #if ! STORE_QUOTIENT
    mpn_divrem_1(qp,0,xp,n,d);
    #endif
    mpn_mul_1(tp,qp,n,d);
    mpn_add_1(tp,tp,n,r1);
    if(mpn_cmp(tp,xp,n)!=0){printf("error 2\n");abort();}
            
   }}}
  
return 0;}

/*


mp_limb_t	jaybidivexact(mp_ptr qp,mp_ptr xp,mp_size_t n,mp_limb_t d,mp_limb_t m)
{int j;mp_limb_t c,h,q,dummy,h1,t,k,hd,ld,qd;

ASSERT(n>0);ASSERT(d!=0);ASSERT_MPN(xp,n);ASSERT(MPN_SAME_OR_SEPARATE_P(qp,xp,n));
ASSERT(d%2==1);
// if d is even then either shift the input xp or the output qp
// output is better as not on depandant path , and for div test dont need to do it
ASSERT(n%2==0);k=n/2;
c=0;h=0;t=0;hd=0;
for(j=0;j<=k-1;j++)
   {h1=xp[j];ld=xp[n-1-j]
    if(t>h1){h1=h1-t;c=1;}else{h1=h1-t;c=0;}
    q=h1*m;
    udiv_qrnnd(qd,dummy,hd,ld,d);
    hd=ld-qd*d;
    qp[n-1-j]=qd;    
    qp[j]=q;
    umul_ppmm(h,dummy,q,d);
    t=h+c;
  }
return hd-t;}    //  so (xp,n) = (qp,n)*d +(hd-t)*B^k   and          d divides xp <=>  ret=0
*/
