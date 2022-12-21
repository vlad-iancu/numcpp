#ifndef NUMCPP_ARRAY_H
#define NUMCPP_ARRAY_H

#include <numcpp/defs.hpp>
#include <numcpp/refcount.hpp>
#include <numcpp/narray.hpp>
#include <numcpp/ndarray.hpp>

#include <cstdlib>
#include <cstdarg>
#include <iostream>

namespace npp
{
	template<array_order order, u8 size, u64 dimension, u64... args>
	struct stride;

	template<u8 size, u64 last_dimension>
	struct stride<array_order::C_CONTIGUOUS, size, last_dimension>
	{
		static constexpr u64 value = size;
	};

	template<u8 size, u64 dimension, u64 next_dimension, u64... args>
	struct stride<array_order::C_CONTIGUOUS, size, dimension, next_dimension, args...>
	{
		using next_stride = stride<array_order::C_CONTIGUOUS, size, next_dimension, args...>;
		static constexpr u64 value = next_dimension * next_stride::value;
	};

	template<u64 current_stride, u64 dimension, u64... args>
	struct stride<array_order::F_CONTIGUOUS, 1, current_stride, dimension, args...>
	{
		using next_stride = stride<array_order::F_CONTIGUOUS, 1, current_stride * dimension, args...>;
		static constexpr u64 value = current_stride;
	};


	template<u64 current_stride, u64 dimension>
	struct stride<array_order::F_CONTIGUOUS, 1, current_stride, dimension>
	{
		static constexpr u64 value = current_stride;
	};

	template<array_order order, u8 size, u64... args>
	struct stride_decl;

	template<u8 size, u64... args>
	struct stride_decl<array_order::C_CONTIGUOUS, size, args...>
	{
		using stride_type = stride<array_order::C_CONTIGUOUS, size, args...>;
	};

	template<u8 size, u64... args>
	struct stride_decl<array_order::F_CONTIGUOUS, size, args...>
	{
		using stride_type = stride<array_order::F_CONTIGUOUS, 1, size, args...>;
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

	template<typename stride_type, typename... index_types>
	struct index_calculator;
	
	template<typename stride_type, typename index_type, typename next_type, typename... index_types>
	struct index_calculator<stride_type, index_type, next_type, index_types...>
	{
		static constexpr inline index_type index(index_type i, next_type j,  index_types... idx) 
		{
			return i * stride_type::value + index_calculator<typename stride_type::next_stride, next_type, index_types...>::index(j, idx...);
		}
	};

	template<typename stride_type, typename index_type>
	struct index_calculator<stride_type, index_type>
	{
		static constexpr inline index_type index(index_type idx)
		{
			return stride_type::value * idx;
		}
	};

	template<u64 remaining_dimensions, typename stride_type>
	struct strides_to_narray;

	template<typename stride_type>
	struct strides_to_narray<1, stride_type>
	{
		static constexpr inline void convert(narray &arr, u64 idx)
		{
			arr.val[idx] = stride_type::value;
		}
	};

	template<u64 remaining_dimensions, typename stride_type>
	struct strides_to_narray
	{
		using next = strides_to_narray<remaining_dimensions - 1, typename stride_type::next_stride>;
		static constexpr inline void convert(narray &arr, u64 idx)
		{
			arr.val[idx] = stride_type::value;
			next::convert(arr, idx + 1);
		}
	};

	template<u64... dimensions>
	struct dimension_list_to_narray;

	template<u64 dim, u64 next_dim, u64... dimensions>
	struct dimension_list_to_narray<dim, next_dim, dimensions...>
	{
		using next = dimension_list_to_narray<next_dim, dimensions...>;
		static inline void convert(narray &arr, u64 idx)
		{
			arr.val[idx] = dim;
			next::convert(arr, idx + 1);
		}
	};

	template<u64 last_dim, u64... dimensions>
	struct dimension_list_to_narray<last_dim, dimensions...>
	{
		static inline void convert(narray &arr, u64 idx)
		{
			arr.val[idx] = last_dim;
		}
	};

	template<u64... args>
	struct index;
	
	template<u64 idx, u64 next_idx, u64... args>
	struct index<idx, next_idx, args...>
	{
		static constexpr u64 value = idx;
		using next = index<next_idx, args...>;
	};

	template<u64 idx>
	struct index<idx>
	{
		static constexpr u64 value = idx;
	};

	template<typename number_type, array_order order, u64... dimensions>
	class array
	{
	public:

		using strides_type = typename stride_decl<order, sizeof(number_type), dimensions...>::stride_type;
		static constexpr u64 ndims = sizeof...(dimensions);

		explicit array()
			: ref(nullptr)
		{
			u8 *data = new u8[multiply<dimensions...>().value * sizeof(number_type)];
			a = data;
			ref = refcount(data); 
		}

		array(const array &other)
			: ref(nullptr)
		{
			a = other.a;
			ref = other.ref;
		}

		template<typename... args>
		inline u64 get(args... idx)
		{
			return *((number_type*)(a + index_calculator<strides_type, args...>::index(idx...)));
		}

		template<typename... args>
		inline void set(number_type value, args... idx)
		{
			*((number_type*)(a + index_calculator<strides_type, args...>::index(idx...))) = value;
		}

		ndarray slice(narray lower, narray upper) const
		{
			narray current_dims(ndims);
			dimension_list_to_narray<dimensions...>::convert(current_dims, 0);
			u8 dtype = sizeof(number_type);
			narray shape(ndims);
			for(u64 i = 0; i < ndims; ++i)
			{
				shape.val[i] = current_dims.val[i];
			}
			array_order ord = order;
			narray slice_strides(ndims);
			narray current_strides(ndims);
			narray slice_shape(ndims);
			strides_to_narray<sizeof...(dimensions), strides_type>::convert(current_strides, 0);
			
			u64 nstrides = 0;
			u8 *offset = (u8*)a;
			i64 *notoffset = (i64*)offset;
			for(u64 i = 0; i < lower.n; ++i)
			{
				offset += current_strides.val[i] * lower.val[i];
				if(upper.val[i] > lower.val[i])
				{
					slice_strides.val[nstrides] = current_strides.val[i];
					slice_shape.val[nstrides] = upper[i] - lower[i];
					nstrides++;
				}
			}
			
			slice_strides.resize(nstrides);
			slice_shape.resize(nstrides);
			return ndarray(offset, slice_shape, dtype, ord, slice_strides, ref);
		}

		u8 *a;

		template<typename lower, typename upper>
		void slice()
		{
			
		}

	private:
		refcount ref;
	};


}


#endif
