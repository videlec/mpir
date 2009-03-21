/*
     Copyright 2009 Jason Moxham

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


#include <stdio.h>
#include <stdlib.h>
#include "gmpextra.h"
#include "gmpextra-1.0.5/gmp-impl.h"
#include "gmpextra-1.0.5/longlong.h"
#include "rdtsc.h"

#define DISP(xp,xn)  do{int __t;printf("%s=",#xp);for(__t=(xn)-1;__t>=0;__t--)printf("%lX ",(xp)[__t]);printf("\n");}while(0)


// d is mormalized
#define udiv_inverse(i,d)  do{mp_limb_t __X;udiv_qrnnd(i,__X,~(d),GMP_LIMB_MAX,d);}while(0)

#define udiv_qrnnd_gmp(q, r, nh, nl, d, di)                         \
  do {                                                                  \
      mp_limb_t _n2, _n10, _nmask, _nadj, _q1;                            \
      mp_limb_t _xh, _xl;                                                 \
      _n2 = (nh);                                                         \
      _n10 = (nl);                                                        \
      _nmask = LIMB_HIGHBIT_TO_MASK (_n10);                               \
      _nadj = _n10 + (_nmask & (d));                                      \
      umul_ppmm (_xh, _xl, di, _n2 - _nmask);                             \
      add_ssaaaa (_xh, _xl, _xh, _xl, _n2, _nadj);                        \
      _q1 = ~_xh;                                                         \
      umul_ppmm (_xh, _xl, _q1, d);                                       \
      add_ssaaaa (_xh, _xl, _xh, _xl, nh, nl);                            \
      _xh -= (d);                                 /* xh = 0 or -1 */      \
      (r) = _xl + ((d) & _xh);                                            \
      (q) = _xh - _q1;                                                    \
    } while (0)
                                                            



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


//basic div
mp_limb_t	inlinebasicdiv_1(mp_ptr qp,mp_srcptr xp,mp_size_t n,mp_limb_t d)
{int j;mp_limb_t r=0,s=0,h,l,q,i;

ASSERT(n>0);ASSERT(d!=0);ASSERT_MPN(xp,n);ASSERT(MPN_SAME_OR_SEPARATE_P(qp,xp,n));
count_leading_zeros(s,d);d=d<<s;
invert_limb(i,d);r=0;
for(j=n-1;j>=0;j--)
   {
    
    mov (%rsi),%r10
    mov %r10,%r9
    shr %cl,%r10   // cl=64-s
    add %rax,%r10
    neg %rcx
    shl %cl,%r9    // cl=s
    neg %rcx
    //UDIV(q=r11,r=rax,h=r10,l=r9,d=r8,i=r12);
      mov %r9,%r11
      sar $63,%r11
      mov %r10,%rax
      sub %r11,%rax
      and %r8,%r11
      add %r9,%r11
      mul %r12
      add %r11,%rax
      adc %r10,%rdx
      not %rdx
      mov %rdx,%r11
      mov %r8,%rax
      mul %rdx
      sub %r8,%r10
      add %r9,%rax
      adc %r10,%rdx
      sub %rdx,%r11
      and %r8,%rdx
      add %rdx,%rax
      neg %r11
      mov %r11,(%rdi)
    #endif
   }
#if ! STORE_QUOTIENT
h=r>>(GMP_LIMB_BITS-1-s)>>1;l=r<<s;
UDIV(q,r,h,l,d,i);
#endif
r>>=s;
return r;}    // so (xp,n) = (qp,n)*d +r   and 0 <= r < d





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

// basic divexact
mp_limb_t	jaydivexact(mp_ptr qp,mp_ptr xp,mp_size_t n,mp_limb_t d,mp_limb_t m)
{int j;mp_limb_t c,h,q,dummy,h1,t;

ASSERT(n>0);ASSERT(d!=0);ASSERT_MPN(xp,n);ASSERT(MPN_SAME_OR_SEPARATE_P(qp,xp,n));
ASSERT(d%2==1);
// if d is even then either shift the input xp or the output qp
// output is better as not on depandant path , and for div test dont need to do it
c=0;h=0;t=0;
for(j=0;j<=n-1;j++)
   {h1=xp[j];
    if(t>h1){h1=h1-t;c=1;}else{h1=h1-t;c=0;}
    q=h1*m;
    qp[j]=q;
    umul_ppmm(h,dummy,q,d);
    t=h+c;
  }
return t;}   // so  (xp,n) = (qp,n)*d -ret*B^n    and 0 <= ret < d


ASSERT(d is normalized);
udiv_qrnnd(i,t1,~d,B-1,d);


neg=sgn(nl);if(neg==1)neg=0;
na=nl+(neg&d);
umul_ppmm(th,tl,i,nh-neg);
add_ssaaaa(q1,tl,th,tl,nh,na);
q1=~q1;
umul_ppmm(th,tl,q1,d);
add_ssaaaa(th,tl,th,tl,nh-d,nl);
r=tl+(d&th);
q=th-q1;




neg=sgn(nl);if(neg==1)neg=0;// could use cqo if neg=DX nl=AX  else sar 63,nl and save nl

na=nl+(neg&d);
mul i,nh-neg
add na+AX=AX
adc nh+DX=q1
not q1
mul q1,d
add nl+AX=tl
adc nh-d+DX=th

r=tl+(d&th);
q=th-q1;





*/

