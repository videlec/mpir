dnl  X86_64 mpn_divrem_euclidean_1

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

include(`../config.m4')

C	(rdi,rdx)=(rsi,rdx) / rcx     #inverse is r8 , no ,calc internel
C	rax=remainder

C	This is divrem_euclidean_1
C	This function "replaces" divrem_1 mod_1 

ASM_START()
PROLOGUE(mpn_divrem_euclidean_qr_1)
push %r15
push %r14
push %r13
push %r12
push %rbp
mov %rdx,%r14
mov %rcx,%r8
xor %r15,%r15	
# x15 is zero
#count_leading_zeros(cl,%r8);
bsr %r8,%rcx
# could use not but would this affect below ??
xor $63,%rcx
shl %cl,%r8
xor %r11,%r11
#   r11=h
#invert_limb(%r9,%r8);
mov %r8,%rdx
not %rdx
xor %rax,%rax
not %rax
div %r8
mov %rax,%r9
xor %rax,%rax
ALIGN(16)
loop:
    mov -8(%rsi,%r14,8),%r13
    # r12=l   r13=t
    mov %r13,%r12
    neg %rcx
    cmovnc %r15,%r13
    shr %cl,%r13
    neg %rcx
    shl %cl,%r12
    mov %r12,%r10
    sar $63,%r10
    add %r13,%rax
    sub %r10,%rax
    add %r11,%rax
    add %r13,%r11
    add %rbp,%r11
#r10=mask,adj
    mul %r9
    mov %r11,%r13
    and %r8,%r10
    add %r12,%r10
    add %r10,%rax
    mov %r11,%r10
#r10=q1
    adc %rdx,%r10
    not %r10
    mov %r8,%rax
    mul %r10
    sub %r8,%r13
    add %r12,%rax
    mov %r8,%r11
    adc %r13,%rdx
    and %rdx,%r11
    mov %rax,%rbp
    sub %r10,%rdx
    mov %rdx,-8(%rdi,%r14,8)
    dec %r14
    jnz loop
pop %rbp
pop %r12
pop %r13
pop %r14
pop %r15
shr %cl,%r11
mov %r11,%rax
ret
EPILOGUE()