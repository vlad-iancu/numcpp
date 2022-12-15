#ifndef NUMCPP_ARRAY_H
#define NUMCPP_ARRAY_H

#include <numcpp/defs.hpp>
#include <numcpp/refcount.hpp>

#include <cstdlib>
#include <cstdarg>
#include <iostream>

namespace npp
{
	template<array_order order, u64 dimension, u64... args>
	struct stride
	{
	};

	template<u64 last_dimension>
	struct stride<array_order::C_CONTIGUOUS, last_dimension>
	{
		static constexpr u64 value = 1;
	};

	template<u64 dimension, u64 next_dimension, u64... args>
	struct stride<array_order::C_CONTIGUOUS, dimension, next_dimension, args...>
	{
		using next_stride = stride<array_order::C_CONTIGUOUS, next_dimension, args...>;
		static constexpr u64 value = next_dimension * next_stride::value;
		static constexpr next_stride next = next_stride();
	};


	template<u64 current_stride, u64 dimension, u64... args>
	struct stride<array_order::F_CONTIGUOUS, current_stride, dimension, args...>
	{
		using next_stride = stride<array_order::F_CONTIGUOUS, current_stride * dimension, args...>;
		static constexpr u64 value = current_stride;
		static constexpr next_stride next = next_stride();
	};


	template<u64 current_stride, u64 dimension>
	struct stride<array_order::F_CONTIGUOUS, current_stride, dimension>
	{
		static constexpr u64 value = current_stride;
	};

	template<array_order order, u64... args>
	struct stride_decl
	{
	};

	template<u64... args>
	struct stride_decl<array_order::C_CONTIGUOUS, args...>
	{
		using stride_type = stride<array_order::C_CONTIGUOUS, args...>;
	};

	template<u64... args>
	struct stride_decl<array_order::F_CONTIGUOUS, args...>
	{
		using stride_type = stride<array_order::F_CONTIGUOUS, 1, args...>;
	};

	template<u64... args>
	struct multiply;

	template<>
	struct multiply<>
	{
		static constexpr u64 value = 1;
	};

	template<u64 x, u64... args>
	struct multiply<x, args...>
	{
		static constexpr u64 value = x * multiply<args...>().value;
	};

	template<u64 dimensions, typename stride_type>
	struct index_calculator;
	
	template<u64 dimensions, typename stride_type>
	struct index_calculator
	{
		static constexpr stride_type stride = stride_type();

		static constexpr inline u64 index(u64 i, u64 idx...) 
		{
			return i * stride.value + index_calculator<dimensions - 1, typename stride_type::next_stride>::index(idx);
		}
	};

	template<typename stride_type>
	struct index_calculator<1, stride_type>
	{
		static constexpr stride_type stride = stride_type();
		static constexpr inline u64 index(u64 index)
		{
			return index * stride.value;
		}
	};

	template<typename number_type, array_order order, u64... dimensions>
	class array
	{
	public:

		using strides_type = typename stride_decl<order, dimensions...>::stride_type;
		using indexer = index_calculator<sizeof...(dimensions), strides_type>;
		static constexpr strides_type strides = strides_type();
		static constexpr u64 ndims = sizeof...(dimensions);

		explicit array()
			: ref(nullptr)
		{
			u8 *data = new u8[multiply<dimensions...>().value * sizeof(number_type)];
			a = (number_type*)data;
			ref = refcount(data); 
		}

		array(const array &other)
			: ref(nullptr)
		{
			a = other.a;
			ref = other.ref;
		}

		template<typename... args>
		inline number_type get(args... idx)
		{
			return indexer::index(idx...);
		}

		number_type *a;

	private:
		refcount ref;
	};


}

#endif
