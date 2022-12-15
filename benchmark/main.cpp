#include <iostream>

#include <numcpp/matrix.hpp>
#include <numcpp/ndarray.hpp>

using namespace npp;

void changeFirstElementTo1(narray arr)
{
	arr[0] = 1;
}

int main()
{
	std::cout << "Entered main" << std::endl;
	ndarray cube({4, 4, 4}, 8, array_order::F_CONTIGUOUS);
	std::cout << "Allocated cube" << std::endl;
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
	std::cout << std::endl;
	ndarray cube_slice = cube.slice({0, 1, 0}, {0, 1, 4});
	std::cout << "Slice strides (main): (";
	for(u64 i = 0;i < cube_slice.stride.n; i++)
	{
		std::cout << " " << cube_slice.stride[i];
	}
	std::cout << " )" << std::endl;
	std::cout << "Slice shape (main): (";
	for(u64 i = 0;i < cube_slice.view_shape.n; i++)
	{
		std::cout << " " << cube_slice.view_shape[i];
	}
	std::cout << " )" << std::endl;
	std::cout << "Slice shape = " << cube_slice.view_shape[0] << std::endl;
	std::cout << "Slice = ["; 
	for(u64 i = 0;i < cube_slice.view_shape[0]; i++)
	{
		std::cout << " " << cube_slice.get<i64>({i});
	}
	std::cout << " ]" << std::endl;
	std::cout << "Finished program" << std::endl;
}
