#   Copyright (C) 2006-2006 Fujitsu Technology Solutions
#   Thilo Cestonaro
#   This file is part of sobbapi
#
#   sobbapi is free software; you can redistribute it and/or modify it
#   under the terms of the GNU General Public License as published by
#   the Free Software Foundation; either version 2 of the License,
#   or (at your option) any later version.
#
#   sobbapi is distributed in the hope that it will be useful, but WITHOUT
#   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
#   FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
#   for more details.
#
#   You should have received a copy of the GNU General Public License
#   along with sobbapi; see the file COPYING. If not, write to the
#   Free Software Foundation, Inc., 59 Temple Place - Suite 330,
#   Boston, MA 02111-1307, USA.
#
#************************************************************************************
#                                  A P I C . A S M
# 
#    Project:    deskflash for Linux
#    Created:    Fujitsu Technology Solutions / PT                            
#    Purpose:    BIOS update utility
#    Platform:   SuSE Linux 10.1
#    Version:    1.30
#    Implements: 
#    Changes:    
#******************************************+******************************************

.text
        .global ApicCheckEnabled
#
#   BOOL ApicCheckEnabled();
#

ApicCheckEnabled: 
#if defined(__LP64__)
	push %rdx
	push %rcx
	push %rbx

	mov $0x0, %rax
	mov $0x1b, %rcx
	rdmsr               # read Cache Latency Override register

	mov %rax, %rbx    # Save eax for evt later usage of base address

	and $0x800, %rax        # Bit 11 set (APIC enabled)
	jz ApicDisabled
	mov $0x1, %rax

ApicDisabled: 
	pop %rbx
	pop %rcx
	pop %rdx
	ret
#else
	pushl %edx
	pushl %ecx
	pushl %ebx
	# push eax - do not save cause this contains the return value

	movl $0x0, %eax
	movl $0x1b, %ecx
	rdmsr               # read Cache Latency Override register

	movl %eax, %ebx    # Save eax for evt later usage of base address

	andl $0x800, %eax        # Bit 11 set (APIC enabled)
	jz ApicDisabled
	movl $0x1, %eax

ApicDisabled: 
	# pop eax - do not restore cause this contains the return value
	popl %ebx
	popl %ecx
	popl %edx
	ret
# END ApicCheckEnabled
#endif
