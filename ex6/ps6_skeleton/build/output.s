.section .rodata
intout: .string "%ld "
strout: .string "%s "
errout: .string "Wrong number of arguments"
STR0: .string "Greatest common divisor of"
STR1: .string "and"
STR2: .string "is"
STR3: .string "and"
STR4: .string "are relative primes"
.section .data
.globl main
.section .text
main:
	pushq %rbp
	movq %rsp, %rbp
	subq $1, %rdi
	cmpq	$2,%rdi
	jne ABORT
	cmpq $0, %rdi
	jz SKIP_ARGS
	movq %rdi, %rcx
	addq $16, %rsi
PARSE_ARGV:
	pushq %rcx
	pushq %rsi
	movq (%rsi), %rdi
	movq $0, %rsi
	movq $10, %rdx
	call strtol
	popq %rsi
	popq %rcx
	pushq %rax
	subq $8, %rsi
	loop PARSE_ARGV
	popq	%rdi
	popq	%rsi
SKIP_ARGS:
	call	_euclid
	jmp END
ABORT:
	movq $errout, %rdi
	call puts
END:
	movq %rax, %rdi
	call exit
_gcd:
	pushq	%rbp
	movq	%rsp, %rbp
	subq	$32, %rsp
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	cmpq	$0, -16(%rbp)
	jle	.L1
	movq	-16(%rbp), %rdi
	movq	-8(%rbp), %rax
	pushq	%rax
	pushq	%rdx
	movq	-16(%rbp), %rax
	pushq	%rax
	pushq	%rdx
	movq	-16(%rbp), %rax
	pushq	%rax
	movq	-8(%rbp), %rax
	cqo
	idivq	(%rsp)
	popq	%rdx
	popq	%rdx
	cqo
	imulq	(%rsp)
	popq	%rdx
	popq	%rdx
	subq	%rax, (%rsp)
	popq	%rax
	movq	%rax, %rsi
	call	_gcd
	movq	%rax, -24(%rbp)
	jmp	.L2
.L1:
	movq	-8(%rbp), %rax
	movq	%rax, -24(%rbp)
.L2:
	movq	-24(%rbp), %rax
	leave
	ret
_euclid:
	pushq	%rbp
	movq	%rsp, %rbp
	subq	$16, %rsp
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	cmpq	$0, -8(%rbp)
	jge	.L3
	movq	-8(%rbp), %rax
	negq	%rax
	movq	%rax, -8(%rbp)
.L3:
	cmpq	$0, -16(%rbp)
	jge	.L4
	movq	-16(%rbp), %rax
	negq	%rax
	movq	%rax, -16(%rbp)
.L4:
	movq	-8(%rbp), %rdi
	movq	-16(%rbp), %rsi
	call	_gcd
	cmpq	$1, %rax
	jle	.L5

	movq	$STR0, %rsi
	movq	$strout, %rdi
	call	printf

	movq	-8(%rbp), %rsi
	movq	$intout, %rdi
	call	printf

	movq	$STR1, %rsi
	movq	$strout, %rdi
	call	printf

	movq	-16(%rbp), %rsi
	movq	$intout, %rdi
	call	printf

	movq	$STR2, %rsi
	movq	$strout, %rdi
	call	printf

	movq	-8(%rbp), %rdi
	movq	-16(%rbp), %rsi
	call	_gcd
	movq	%rax, %rsi
	movq	$intout, %rdi
	call	printf
	movq	$'\n', %rdi
	call	putchar

	jmp	.L6
.L5:

	movq	-8(%rbp), %rsi
	movq	$intout, %rdi
	call	printf

	movq	$STR3, %rsi
	movq	$strout, %rdi
	call	printf

	movq	-16(%rbp), %rsi
	movq	$intout, %rdi
	call	printf

	movq	$STR4, %rsi
	movq	$strout, %rdi
	call	printf
	movq	$'\n', %rdi
	call	putchar

.L6:
	movq	$0, %rax
	leave
	ret
