#ifndef NUMCPP_MATRIX_LAYOUT_H
#define NUMCPP_MATRIX_LAYOUT_H

#include <numcpp/defs.hpp>

namespace npp
{
	template<typename number_type, array_order order>
	struct matrix_layout;

	template<typename number_type>
	struct matrix_layout<number_type, array_order::F_CONTIGUOUS>
	{
		static constexpr inline number_type value(number_type * __restrict__ a, u64 i, u64 j, u64 n, u64 m)
		{
			return a[n * j + i];
		}

		static constexpr inline number_type *element(number_type * __restrict__ a, u64 i, u64 j, u64 n, u64 m)
		{
			return &a[n * j + i];
		}
	};

	template<typename number_type>
	struct matrix_layout<number_type, array_order::C_CONTIGUOUS>
	{
		static constexpr inline number_type value(number_type * __restrict__ a, u64 i, u64 j, u64 n, u64 m)
		{
			return a[m * i + j];
		}

		static constexpr inline number_type *element(number_type * __restrict__ a, u64 i, u64 j, u64 n, u64 m)
		{
			return &a[m * i + j];
		}
	};

	//Think of a way to cache the m / 2 and n / 2 values so that computation will be faster
}

#endif
