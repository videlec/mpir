dnl  core2 mpn_addmul_1

dnl  Copyright 2008,2009 Jason Moxham

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

C	(rdi,rdx)=(rdi,rdx)+(rsi,rdx)*rcx
C	rax=carry

ASM_START()
PROLOGUE(mpn_addmul_1)
mov (%rsi),%rax
mov (%rdi),%r10
cmp $2,%rdx
je two
jb one
push %r12
push %r13
push %r14
mov $6,%r14
lea -48(%rsi,%rdx,8),%rsi
lea -48(%rdi,%rdx,8),%rdi
sub %rdx,%r14
	#mov (%rsi,%r14,8),%rax
	mul %rcx
	lea (%rax),%r8
	mov 8(%rsi,%r14,8),%rax
	#mov (%rdi,%r14,8),%r10
	lea (%rdx),%r9
	mul %rcx
	lea (%rax),%r11
	mov 16(%rsi,%r14,8),%rax
	mov 8(%rdi,%r14,8),%r13
	lea (%rdx),%r12
cmp $0,%r14
jge skiploop
ALIGN(16)
loop:
	mul %rcx
	add %r8,%r10
	lea (%rax),%r8
	mov 16+8(%rsi,%r14,8),%rax
	adc %r9,%r11
	mov %r10,16-16(%rdi,%r14,8)
	mov 16+0(%rdi,%r14,8),%r10
	adc $0,%r12
	lea (%rdx),%r9
	mul %rcx
	add %r11,%r13
	lea (%rax),%r11
	mov 16+16(%rsi,%r14,8),%rax
	adc %r12,%r8
	mov %r13,16+-8(%rdi,%r14,8)
	mov 16+8(%rdi,%r14,8),%r13
	adc $0,%r9
	lea (%rdx),%r12
	mul %rcx
	add %r8,%r10
	lea (%rax),%r8
	mov 16+24(%rsi,%r14,8),%rax
	adc %r9,%r11
	mov %r10,16+0(%rdi,%r14,8)
	mov 16+16(%rdi,%r14,8),%r10
	adc $0,%r12
	lea (%rdx),%r9
	mul %rcx
	add %r11,%r13
	lea (%rax),%r11
	mov 16+32(%rsi,%r14,8),%rax
	adc %r12,%r8
	mov %r13,16+8(%rdi,%r14,8)
	mov 16+24(%rdi,%r14,8),%r13
	adc $0,%r9
	lea (%rdx),%r12
	add $4,%r14
	jnc loop
ALIGN(16)
skiploop:
mul %rcx
# have 3-r14 left to do
cmp $2,%r14
jz next1
ja next0
jp next2
next3:
	add %r8,%r10
	lea (%rax),%r8
	mov 16+8(%rsi,%r14,8),%rax
	adc %r9,%r11
	mov %r10,16-16(%rdi,%r14,8)
	mov 16+0(%rdi,%r14,8),%r10
	adc $0,%r12
	lea (%rdx),%r9
	mul %rcx
	add %r11,%r13
	lea (%rax),%r11
	mov 16+16(%rsi,%r14,8),%rax
	adc %r12,%r8
	mov %r13,16-8(%rdi,%r14,8)
	mov 16+8(%rdi,%r14,8),%r13
	adc $0,%r9
	lea (%rdx),%r12
	mul %rcx
	add %r8,%r10
	lea (%rax),%r8
	mov 16+24(%rsi,%r14,8),%rax
	adc %r9,%r11
	mov %r10,16+0(%rdi,%r14,8)
	mov 16+16(%rdi,%r14,8),%r10
	adc $0,%r12
	lea (%rdx),%r9
	mul %rcx
	add %r11,%r13
	lea (%rax),%r11
	adc %r12,%r8
	mov %r13,16+8(%rdi,%r14,8)
	mov 16+24(%rdi,%r14,8),%r13
	adc $0,%r9
	lea (%rdx),%r12
	add %r8,%r10
	adc %r9,%r11
	mov %r10,16+16(%rdi,%r14,8)
	adc $0,%r12
	add %r13,%r11
	adc $0,%r12
	mov %r11,16+24(%rdi,%r14,8)
	mov %r12,%rax
	pop %r14
	pop %r13
	pop %r12
	ret
ALIGN(16)
next0:
	add %r8,%r10
	lea (%rax),%r8
	adc %r9,%r11
	mov %r10,16-16(%rdi,%r14,8)
	mov 16+0(%rdi,%r14,8),%r10
	adc $0,%r12
	lea (%rdx),%r9
	add %r11,%r13
	adc %r12,%r8
	mov %r13,16-8(%rdi,%r14,8)
	adc $0,%r9
	add %r8,%r10
	adc $0,%r9
	mov %r10,16+0(%rdi,%r14,8)
	mov %r9,%rax
	pop %r14
	pop %r13
	pop %r12
	ret	
ALIGN(16)
next1:
	add %r8,%r10
	lea (%rax),%r8
	mov 16+8(%rsi,%r14,8),%rax
	adc %r9,%r11
	mov %r10,16-16(%rdi,%r14,8)
	mov 16+0(%rdi,%r14,8),%r10
	adc $0,%r12
	lea (%rdx),%r9
	mul %rcx
	add %r11,%r13
	lea (%rax),%r11
	adc %r12,%r8
	mov %r13,16-8(%rdi,%r14,8)
	mov 16+8(%rdi,%r14,8),%r13
	adc $0,%r9
	lea (%rdx),%r12
	add %r8,%r10
	adc %r9,%r11
	mov %r10,16+0(%rdi,%r14,8)
	adc $0,%r12
	add %r11,%r13
	adc $0,%r12
	mov %r13,16+8(%rdi,%r14,8)
	mov %r12,%rax
	pop %r14
	pop %r13
	pop %r12
	ret
ALIGN(16)
one:
	mul %rcx
	add %rax,%r10
	adc $0,%rdx
	mov %r10,(%rdi)
	mov %rdx,%rax
	ret
ALIGN(16)
two:
	mul %rcx
	mov %rax,%r8
	mov 8(%rsi),%rax
	mov %rdx,%r9
	mul %rcx
	add %r8,(%rdi)
	adc %rax,%r9
	adc $0,%rdx
	add %r9,8(%rdi)
	adc $0,%rdx
	mov %rdx,%rax
	ret
ALIGN(16)
next2:
	add %r8,%r10
	lea (%rax),%r8
	mov 16+8(%rsi,%r14,8),%rax
	adc %r9,%r11
	mov %r10,16-16(%rdi,%r14,8)
	mov 16+0(%rdi,%r14,8),%r10
	adc $0,%r12
	lea (%rdx),%r9
	mul %rcx
	add %r11,%r13
	lea (%rax),%r11
	mov 16+16(%rsi,%r14,8),%rax
	adc %r12,%r8
	mov %r13,16-8(%rdi,%r14,8)
	mov 16+8(%rdi,%r14,8),%r13
	adc $0,%r9
	lea (%rdx),%r12
	mul %rcx
	add %r8,%r10
	lea (%rax),%r8
	adc %r9,%r11
	mov %r10,16+0(%rdi,%r14,8)
	mov 16+16(%rdi,%r14,8),%r10
	adc $0,%r12
	lea (%rdx),%r9
	add %r11,%r13
	adc %r12,%r8
	mov %r13,16+8(%rdi,%r14,8)
	adc $0,%r9
	add %r8,%r10
	adc $0,%r9
	mov %r10,16+16(%rdi,%r14,8)
	mov %r9,%rax
	pop %r14
	pop %r13
	pop %r12
	ret
EPILOGUE()
