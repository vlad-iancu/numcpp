#include <iostream>

#include <numcpp/matrix.hpp>
#include <numcpp/ndarray_meat.hpp>

using namespace npp;


int main()
{
	constexpr u64 N = 8;
	constexpr u64 M = 5;
	
	matrix<i32> mat(N, M);
	
	for(u64 i = 0; i < N * M; ++i)
	{
		mat.a[i] = i;
	}

	for(u64 i = 0; i < N; ++i)
	{
		for(u64 j = 0; j < M; ++j)
		{
			std::cout << mat.get(i, j) << " ";
		}
		std::cout << std::endl;
	}
	
	
}
