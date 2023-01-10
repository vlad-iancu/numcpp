#include <numcpp/matrix.hpp>

#include <iostream>

void func(npp::matrix<npp::i32> m)
{
	std::cout << "Copy data address: " << static_cast<void*>(m.a) << std::endl;
	m.set(0, 0, 4);
}

int main()
{
	npp::matrix<npp::i32> mat(8, 5);
	npp::u64 idx = 0;
	for(npp::u64 j = 0; j < 5; ++j)
		for(npp::u64 i = 0; i < 8; ++i)
		{
			*(mat.a + idx * mat.dtype) = (npp::i32)idx;
			idx++;
		}

	for(npp::u64 j = 0; j < 5; ++j)
		for(npp::u64 i = 0;i < 8; ++i)
		{
			mat.set(i, j, 5);
		}

	for(npp::u64 j = 0; j < 5; ++j)
		for(npp::u64 i = 0;i < 8; ++i)
		{
			std::cout << " " << mat.get(i, j);
		}

	std::cout << std::endl;
	return 0;
}
