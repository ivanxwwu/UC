	.file	"main.cpp"
	.section	.rodata
	.align 8
.LC0:
	.string	"backtrace() returned %d addresses\n"
.LC1:
	.string	"backtrace_symbols"
	.text
	.globl	_ZN4haha7myfunc3Ev
	.type	_ZN4haha7myfunc3Ev, @function
_ZN4haha7myfunc3Ev:
.LFB2:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$816, %rsp
	leaq	-800(%rbp), %rax
	movl	$100, %esi
	movq	%rax, %rdi
	call	backtrace
	movl	%eax, -812(%rbp)
	movl	-812(%rbp), %eax
	movl	%eax, %esi
	movl	$.LC0, %edi
	movl	$0, %eax
	call	printf
	movl	-812(%rbp), %edx
	leaq	-800(%rbp), %rax
	movl	%edx, %esi
	movq	%rax, %rdi
	call	backtrace_symbols
	movq	%rax, -808(%rbp)
	cmpq	$0, -808(%rbp)
	jne	.L2
	movl	$.LC1, %edi
	call	perror
	movl	$1, %edi
	call	exit
.L2:
	movl	$0, -816(%rbp)
	jmp	.L3
.L4:
	movl	-816(%rbp), %eax
	cltq
	leaq	0(,%rax,8), %rdx
	movq	-808(%rbp), %rax
	addq	%rdx, %rax
	movq	(%rax), %rax
	movq	%rax, %rdi
	call	puts
	addl	$1, -816(%rbp)
.L3:
	movl	-816(%rbp), %eax
	cmpl	-812(%rbp), %eax
	jl	.L4
	movq	-808(%rbp), %rax
	movq	%rax, %rdi
	call	free
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE2:
	.size	_ZN4haha7myfunc3Ev, .-_ZN4haha7myfunc3Ev
	.type	_ZN4hahaL7myfunc2Ev, @function
_ZN4hahaL7myfunc2Ev:
.LFB3:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	call	_ZN4haha7myfunc3Ev
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE3:
	.size	_ZN4hahaL7myfunc2Ev, .-_ZN4hahaL7myfunc2Ev
	.globl	_ZN4haha6myfuncEi
	.type	_ZN4haha6myfuncEi, @function
_ZN4haha6myfuncEi:
.LFB4:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movl	%edi, -4(%rbp)
	cmpl	$1, -4(%rbp)
	jle	.L7
	movl	-4(%rbp), %eax
	subl	$1, %eax
	movl	%eax, %edi
	call	_ZN4haha6myfuncEi
	jmp	.L9
.L7:
	call	_ZN4hahaL7myfunc2Ev
.L9:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE4:
	.size	_ZN4haha6myfuncEi, .-_ZN4haha6myfuncEi
	.section	.rodata
.LC2:
	.string	"%s num-calls\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB5:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movl	%edi, -4(%rbp)
	movq	%rsi, -16(%rbp)
	cmpl	$2, -4(%rbp)
	je	.L11
	movq	-16(%rbp), %rax
	movq	(%rax), %rdx
	movq	stderr(%rip), %rax
	movl	$.LC2, %esi
	movq	%rax, %rdi
	movl	$0, %eax
	call	fprintf
	movl	$1, %edi
	call	exit
.L11:
	movq	-16(%rbp), %rax
	addq	$8, %rax
	movq	(%rax), %rax
	movq	%rax, %rdi
	call	atoi
	movl	%eax, %edi
	call	_ZN4haha6myfuncEi
	movl	$0, %edi
	call	exit
	.cfi_endproc
.LFE5:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 4.8.4-2ubuntu1~14.04.4) 4.8.4"
	.section	.note.GNU-stack,"",@progbits
