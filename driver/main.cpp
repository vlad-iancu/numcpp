#include <iostream>

#include <numcpp/matrix.hpp>
#include <numcpp/ndarray.hpp>

using namespace npp;

int main()
{
	ndarray cube({4, 4, 4}, 8, array_order::C_CONTIGUOUS);
	std::cout << "Array strides (main): (";
	for(u64 i = 0;i < cube.stride.n; i++)
	{
		std::cout << " " << cube.stride[i];
	}
	std::cout << " )" << std::endl;
	i64 index = 0;
	for(u64 j = 0; j < 4; j++)
		for(u64 i = 0; i < 4; i++)
			for(u64 k = 0; k < 4; k++)
			{
				cube.set<i64>({k, i, j}, index);
				index++;
			}

	
	for(u64 i = 0;i < 64 * cube.dtype; i += cube.dtype)
	{
		std::cout << *((i64*)(cube.a + i)) << " ";
	}
	ndarray cube_slice = cube.slice({0, 1, 0}, {0, 1, 4});
	std::cout << "Slice strides (main): (";
	for(u64 i = 0;i < cube_slice.stride.n; i++)
	{
		std::cout << " " << cube_slice.stride[i];
	}
	std::cout << " )" << std::endl;
	std::cout << "Slice shape (main): (";
	for(u64 i = 0;i < cube_slice.s.n; i++)
	{
		std::cout << " " << cube_slice.s[i];
	}
	std::cout << " )" << std::endl;
	std::cout << "Finished program" << std::endl;
}
