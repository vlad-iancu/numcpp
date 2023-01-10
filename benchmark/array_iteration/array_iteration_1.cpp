#include "numcpp/matrix_bones.hpp"
#include <numcpp/matrix.hpp>
#include <numcpp/ndarray.hpp>

#include <diagnostics.hpp>

#include <iostream>
#include <chrono>
#include <iomanip>
#include <functional>

#define ITER 500
#define N 1000
#define M 1200
#define K1 60
#define K2 200
#define K3 100

using namespace npp;

void carray_for(i32 **arr, u64 iter)
{
	for(u64 i = 0; i < N; ++i)
		for(u64 j = 0; j < M; ++j)
		{
			arr[i][j] = 3;
		}
}

void ndarray_get(ndarray<i32> arr, u64 iter)
{
	for(u64 i = 0; i < N; ++i)
	{
		for(u64 j = 0; j < M; ++j)
		{
			arr.get({i, j});
		}
	}
}

void ndarray_set(ndarray<i32> arr, u64 iter)
{
	for(u64 i = 0; i < N; ++i)
	{
		for(u64 j = 0;j < M; ++j)
		{
			arr.set({i, j}, 3);
		}
	}

}

void matrix_get(matrix<i32> mat, u64 iter)
{

	for(u64 i = 0; i < N; ++i)
	{
		for(u64 j = 0; j < M; ++j)
		{
			mat.get(i, j);
		}
	}
}

void matrix_set(matrix<i32> mat, u64 iter)
{
	for(u64 i = 0; i < N; ++i)
	{
		for(u64 j = 0;j < M; ++j)
		{
			mat.set(i, j, 3);
		}
	}
}

void matrix_for(matrix<i32> mat, u64 iter)
{
	i32 * __restrict__ index = mat.a;
	volatile i32 x;
	const u64 size = mat.m * mat.n;
	for(u64 i = 0; i < size; ++i)
	{
		x = *index;
		++index;
	}

}


void big_for(u8 *a, u64 iter)
{
	u8 *index = a;
	volatile i32 x;
	for(u64 j = 0; j < M; ++j)
	{
		for(u64 i = 0; i < N; ++i)
		{
			x = *((i32*)index);
			index += sizeof(i32);
		}
	}
}

void small_for(u8 *a, u64 iter)
{
	u8 *index = a;
	volatile i32 x;
	for(u64 j = 0; j < K3; ++j)
	{
		for(u64 i = 0; i < K2; ++i)
		{
			for(u64 k = 0; k < K1; ++k)
			{
				x = *((i32*)index);
				index += sizeof(i32);		
			}
		}
	}
}

int main()
{
	ndarray<i32> arr({N, M});
	ndarray<i32> cube({K1, K2, K3}); 
	for(i32 *i = cube.a; i < cube.a + K1 * K2 * K3; i += cube.dtype)
	{
		*i = (i - cube.a);
	}
	
	matrix<i32> mat(N, M);
	i32 **carr = new i32*[N];	
	for(u64 i = 0; i < N; i++)
	{
		carr[i] = new i32[M];
		for(u64 j = 0;j < M; j++)
		{
			mat.set(i, j, 5);
		}
	}
	
	i32 *carr3 = new i32[N * M];
	double ndarrayread = run("ndarray read", ITER / 5, std::bind(&ndarray_get, arr, std::placeholders::_1));
	double ndarraywrite = run("ndarray write", ITER / 5, std::bind(&ndarray_set, arr, std::placeholders::_1));
	double matrixread = run("matrix get", ITER, std::bind(&matrix_get, mat, std::placeholders::_1));
	double matrixwrite = run("matrix set", ITER, std::bind(&matrix_set, mat, std::placeholders::_1));
	double matrixfor = run("matrix for", ITER, std::bind(&matrix_for, mat, std::placeholders::_1));

	double c_array_for = run("C array read", ITER, std::bind(&carray_for, carr, std::placeholders::_1));
	double big_for_avg = run("big for (cube)", ITER * 5, std::bind(&big_for, (u8*)cube.a, std::placeholders::_1));
	double small_for_avg = run("small for (cube)", ITER * 5, std::bind(&small_for, (u8*)cube.a, std::placeholders::_1));
	

	std::cout << "ndarrayread (microseconds avg): " << std::fixed << ndarrayread / 1000 << std::setprecision(9) << std::endl;
	std::cout << "ndarraywrite (microseconds avg): " << std::fixed << ndarraywrite / 1000 << std::setprecision(9) << std::endl;
	std::cout << "matrixread (microseconds avg): " << std::fixed << matrixread / 1000 << std::setprecision(9) << std::endl;
	std::cout << "matrixwrite (microseconds avg): " << std::fixed << matrixwrite / 1000 << std::setprecision(9) << std::endl;
	std::cout << "matrixfor (microseconds avg): " << std::fixed << matrixfor / 1000 << std::setprecision(9) << std::endl;
	std::cout << "array_write (microseconds avg): " << std::fixed << c_array_for / 1000 << std::setprecision(9) << std::endl;
	std::cout << "big for (microseconds avg): " << std::fixed << big_for_avg / 1000 << std::setprecision(9) << std::endl;
	std::cout << "small for (microseconds avg): " << std::fixed << small_for_avg / 1000 << std::setprecision(9) << std::endl;
	return 0;	
}
