dnl  X86_64 mpn_diveby (B-1)/f

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

C	(rdi,rdx)=(rsi,rdx)/rcx where r8=(B-1)/rcx
C	rax=carry out

C	The two imul's are only needed if want strict compatibility with
C	mpn_divexact_1 when the division is not exact

ASM_START()
PROLOGUE(mpn_divexact_byBm1of)
mov $3,%r10d
lea -24(%rsi,%rdx,8),%rsi
lea -24(%rdi,%rdx,8),%rdi
# r9 is our carry in
mov $0,%r9
#imul %r8,%r9 this is needed if we have non-zero carry in
sub %rdx,%r10
jnc skiploop
ALIGN(16)
loop:
	mov (%rsi,%r10,8),%rax
	mul %r8
	sub %rax,%r9
	mov %r9,(%rdi,%r10,8)
	sbb %rdx,%r9
	mov 8(%rsi,%r10,8),%rax
	mul %r8
	sub %rax,%r9
	mov %r9,8(%rdi,%r10,8)
	sbb %rdx,%r9
	mov 16(%rsi,%r10,8),%rax
	mul %r8
	sub %rax,%r9
	mov %r9,16(%rdi,%r10,8)
	sbb %rdx,%r9
	mov 24(%rsi,%r10,8),%rax
	mul %r8
	sub %rax,%r9
	mov %r9,24(%rdi,%r10,8)
	sbb %rdx,%r9
	add $4,%r10
	jnc loop
skiploop:
test $2,%r10
jnz skip
	mov (%rsi,%r10,8),%rax
	mul %r8
	sub %rax,%r9
	mov %r9,(%rdi,%r10,8)
	sbb %rdx,%r9
	mov 8(%rsi,%r10,8),%rax
	mul %r8
	sub %rax,%r9
	mov %r9,8(%rdi,%r10,8)
	sbb %rdx,%r9
	add $2,%r10
skip:
test $1,%r10
jnz end
	mov (%rsi,%r10,8),%rax
	mul %r8
	sub %rax,%r9
	mov %r9,(%rdi,%r10,8)
	sbb %rdx,%r9
end:
imul %rcx,%r9
mov %r9,%rax
neg %rax
ret
EPILOGUE()
