#include <iostream>
#include <cstdlib>

#include <numcpp/defs.hpp>
#include <diagnostics.hpp>

#define ITER 10000

using namespace npp;
extern "C"
{

	void arithmetic_on_locals(u64 iter)
	{
		u64 i = 4;
		u64 j = 5;
		u64 k = i * j;
	}

	void arithmetic_on_heap(u64 *i, u64 *j, u64 *k,  u64 iter)
	{
		*i = 4;
		*j = 5;
		*k = (*i) * (*j);
	}

	void one_exp(u64 a1, u64 a2, u64 a3, u64 a4, u64 a5, u64 a6, u64 a7, u64 iter)
	{
		u64 i = (a1 * a2) / (a3 * a4) + a5 * (a6 / a7);
	}

	void multiple_exp(u64 a1, u64 a2, u64 a3, u64 a4, u64 a5, u64 a6, u64 a7, u64 iter)
	{
		u64 i1 = a1 * a2;
		u64 i2 = a3 * a4;
		u64 i3 = i1 / i2;
		u64 i4 = a6 / a7;
		u64 i5 = a5 * i4;
		u64 i6 = i3 + i5;
	}
}
/*
multiple_exp:
.LFB3177:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -56(%rbp)
	movq	%rsi, -64(%rbp)
	movq	%rdx, -72(%rbp)
	movq	%rcx, -80(%rbp)
	movq	%r8, -88(%rbp)
	movq	%r9, -96(%rbp)
	movq	-56(%rbp), %rax
	imulq	-64(%rbp), %rax
	movq	%rax, -8(%rbp)
	movq	-72(%rbp), %rax
	imulq	-80(%rbp), %rax
	movq	%rax, -16(%rbp)
	movq	-8(%rbp), %rax
	movl	$0, %edx
	divq	-16(%rbp)
	movq	%rax, -24(%rbp)
	movq	-96(%rbp), %rax
	movl	$0, %edx
	divq	16(%rbp)
	movq	%rax, -32(%rbp)
	movq	-88(%rbp), %rax
	imulq	-32(%rbp), %rax
	movq	%rax, -40(%rbp)
	movq	-24(%rbp), %rdx
	movq	-40(%rbp), %rax
	addq	%rdx, %rax
	movq	%rax, -48(%rbp)
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc



	one_exp:
	.LFB3176:
		.cfi_startproc
		pushq	%rbp
		.cfi_def_cfa_offset 16
		.cfi_offset 6, -16
		movq	%rsp, %rbp
		.cfi_def_cfa_register 6
		movq	%rdi, -24(%rbp)
		movq	%rsi, -32(%rbp)
		movq	%rdx, -40(%rbp)
		movq	%rcx, -48(%rbp)
		movq	%r8, -56(%rbp)
		movq	%r9, -64(%rbp)
		movq	-24(%rbp), %rax
		imulq	-32(%rbp), %rax
		movq	-40(%rbp), %rdx
		movq	%rdx, %rsi
		imulq	-48(%rbp), %rsi
		movl	$0, %edx
		divq	%rsi
		movq	%rax, %rcx
		movq	-64(%rbp), %rax
		movl	$0, %edx
		divq	16(%rbp)
		imulq	-56(%rbp), %rax
		addq	%rcx, %rax
		movq	%rax, -8(%rbp)
		nop
		popq	%rbp
		.cfi_def_cfa 7, 8
		ret
		.cfi_endproc

*/
int main()
{
	u64 *i = new u64[100000];
	u64 *j = new u64[100000];
	u64 *k = new u64[100000];
	double heap = run("arithmetic on heap", ITER, std::bind(&arithmetic_on_heap, i, j, k, std::placeholders::_1));
	double locals = run("arithmetic on locals", ITER, &arithmetic_on_locals);
	double multipleexp = run("6 local variables: ", ITER, std::bind(&multiple_exp, 10, 8, 3, 7, 4, 8, 2, std::placeholders::_1));
	double singleexp = run("6 local variables: ", ITER, std::bind(&one_exp, 10, 8, 3, 7, 4, 8, 2, std::placeholders::_1));
	
	std::cout << "locals (avg): " << locals << std::endl;
	std::cout << "heap (avg): " << heap << std::endl;
	std::cout << "multiple locals (avg): " << multipleexp << std::endl;
	std::cout << "single local (avg): " << singleexp << std::endl;
	return 0;
}
