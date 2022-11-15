#include <iostream>

#include <numcpp/matrix.hpp>

using namespace npp;

void fun(matrix<i32> mat)
{
	for(u64 j = 0; j < mat.columns(); j++)
	{
		for(u64 i = 0; i < mat.lines(); i++)
		{
			mat.set(i, j, mat.columns() * j + i);
		}
	}
}

int main()
{
	matrix<i32> mat(4, 4);

	for(u64 j = 0; j < 4; j++)
	{
		for(u64 i = 0; i < 4; i++)
		{
			mat.set(i, j, mat.columns() * j + i);
		}
	}

	matrix<i32> top_right(mat.slice(0, 1, 2, 3));
	fun(top_right);
	for(u64 i = 0;i < 4; i++)
	{
		for(u64 j = 0;j < 4; j++)
		{
			std::cout << mat.get(i, j) << " ";
		}
		std::cout << std::endl;
	}

	std::cout << std::endl;

	for(u64 i = 0; i < 2; i++)
	{
		for(u64 j = 0; j < 2; j++)
		{
			std::cout << top_right.get(i, j) << " ";
		}
		std::cout << std::endl;
	}
}
