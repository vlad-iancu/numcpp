#include <numcpp/matrix.hpp>
#include <numcpp/ndarray.hpp>

#include <diagnostics.hpp>

#include <iostream>
#include <chrono>
#include <iomanip>
#include <functional>

#include <numcpp/array.hpp>

#define ITER 500
#define N 1000
#define M 1200
#define K1 60
#define K2 200
#define K3 100

using namespace npp;

void carray_for(i32 **arr, u64 iter)
{
	for(u64 i = 0; i < N; i++)
		for(u64 j = 0; j < M; j++)
		{
			arr[i][j] = 3;
		}
}

void ndarray_get(ndarray arr, u64 iter)
{
	for(u64 i = 0; i < N; i++)
	{
		for(u64 j = 0; j < M; j++)
		{
			arr.get<i64>({i, j});
		}
	}
}

void ndarray_set(ndarray arr, u64 iter)
{
	for(u64 i = 0; i < N; i++)
	{
		for(u64 j = 0;j < M; j++)
		{
			arr.set<i64>({i, j}, 3);
		}
	}

}

void matrix_get(matrix mat, u64 iter)
{

	for(u64 i = 0; i < N; i++)
	{
		for(u64 j = 0;j < M; j++)
		{
			mat.get<i32>(i, j);
		}
	}
}

void matrix_set(matrix mat, u64 iter)
{
	for(u64 i = 0; i < N; i++)
	{
		for(u64 j = 0;j < M; j++)
		{
			mat.set<i32>(i, j, 3);
		}
	}
}

void matrix_for(matrix mat, u64 iter)
{
	u8 *index = mat.a;
	volatile i32 x;
	for(u64 i = 0; i < 120000; ++i)
	{
		x = *((i32*)index);
		index += sizeof(i32);
	}

}

void cube_for(u8 *a, u64 iter)
{
	u8 *index = a;
	volatile i32 x;
	for(u64 j = 0; j < K3; j++)
	{
		for(u64 i = 0; i < K2; i++)
		{
			for(u64 k = 0; k < K1; k++)
			{
				x = *((i32*)index);
				index += sizeof(i32);		
			}
		}
	}
}

void big_for(u8 *a, u64 iter)
{
	u8 *index = a;
	volatile i32 x;
	for(u64 j = 0; j < M; j++)
	{
		for(u64 i = 0; i < N; i++)
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
	for(u64 j = 0; j < K3; j++)
	{
		for(u64 i = 0; i < K2; i++)
		{
			for(u64 k = 0; k < K1; k++)
			{
				x = *((i32*)index);
				index += sizeof(i32);		
			}
		}
	}
}

void static_array_loop(array<i32, array_order::F_CONTIGUOUS, N, M> arr, u64 iter)
{
	volatile i32 x;
	for(u64 i = 0; i < N; ++i)
		for(u64 j = 0; j < M; ++j)
		{
			x = arr.get(i, j);
		}
}

int main()
{
	ndarray arr({N, M}, 4, array_order::F_CONTIGUOUS);
	ndarray cube({K1, K2, K3}, 4, array_order::F_CONTIGUOUS); 
	npp::array<i32, array_order::F_CONTIGUOUS, N, M> static_array;
	for(u8 *i = cube.a; i < cube.a + K1 * K2 * K3; i += cube.dtype)
	{
		*((i32*)i) = (i - cube.a) / cube.dtype;
	}
	matrix mat(N, M, 4);
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
	double cubefor = run("classic for (cube)", ITER, std::bind(&cube_for, cube.a, std::placeholders::_1));
	double big_for_avg = run("big for (cube)", ITER * 5, std::bind(&big_for, cube.a, std::placeholders::_1));
	double small_for_avg = run("small for (cube)", ITER * 5, std::bind(&small_for, cube.a, std::placeholders::_1));
	double static_array_avg = run("static array", ITER, std::bind(&static_array_loop, static_array, std::placeholders::_1));
	

	std::cout << "ndarrayread (avg): " << std::fixed << ndarrayread << std::setprecision(9) << std::endl;
	std::cout << "ndarraywrite (avg): " << std::fixed << ndarraywrite << std::setprecision(9) << std::endl;
	std::cout << "matrixread (avg): " << std::fixed << matrixread << std::setprecision(9) << std::endl;
	std::cout << "matrixwrite (avg): " << std::fixed << matrixwrite << std::setprecision(9) << std::endl;
	std::cout << "matrixfor (avg): " << std::fixed << matrixfor << std::setprecision(9) << std::endl;
	std::cout << "array_write (avg): " << std::fixed << c_array_for << std::setprecision(9) << std::endl;
	std::cout << "classic_for cube (avg): " << std::fixed << cubefor << std::setprecision(9) << std::endl;
	std::cout << "big for (avg): " << std::fixed << big_for_avg << std::setprecision(9) << std::endl;
	std::cout << "small for (avg): " << std::fixed << small_for_avg << std::setprecision(9) << std::endl;
	std::cout << "static array (avg): " << std::fixed << static_array_avg << std::setprecision(9) << std::endl;
	return 0;	
}
