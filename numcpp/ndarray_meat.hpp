#ifndef NUMCPP_NDARRAY_MEAT_H
#define NUMCPP_NDARRAY_MEAT_H

#include <numcpp/ndarray_bones.hpp>

#include <type_traits>

namespace npp
{
	template<u64 number>
	struct log2
	{
		static_assert(number % 2 == 0, "Number must be a power of two in order to calculate its logarithm");
		static constexpr u64 value = log2<number / 2>::value + 1;
	};

	template<>
	struct log2<1>
	{
		static constexpr u64 value = 0;
	};

	template<typename number_type, array_order order>
	inline
	ndarray<number_type, order>::ndarray(const shape& array_shape)
		: 
			s(array_shape),
			ref(nullptr)
	{
		s.n = array_shape.n;
		strides stride(s.n);
		u64 current_stride = 1;
		u64 j = 0;
		u64 size = 1;
		u64 start;
		u64 end;
		u64 step;
		if(order == array_order::F_CONTIGUOUS)
		{
			start = 0;
			end = array_shape.n;
			step = 1;
		}
		if(order == array_order::C_CONTIGUOUS)
		{
			start = array_shape.n - 1;
			end = -1;
			step = -1;
		}
		for(u64 i = start; i != end; i += step)
		{
			size = size * array_shape.val[i];
			stride[i] = current_stride * dtype;
			++j;
			current_stride = current_stride * array_shape.val[i];
		}
		a = new number_type[size];
		ref = refcount(a);
		this->stride = stride;
	}
	
	template<typename number_type, array_order order>
	template<typename other_number_type, array_order other_order>
	ndarray<number_type, order>::ndarray(const ndarray<other_number_type, other_order> &arr)
	{
		static_assert(std::is_convertible_v<other_number_type, number_type>, "ndarray number types not convertible");	

		s = arr.s;
		strides stride(s.n);
		u64 current_stride = 1;
		u64 j = 0;
		u64 size = 1;
		u64 start;
		u64 end;
		u64 step;
		if(order == array_order::F_CONTIGUOUS)
		{
			start = 0;
			end = arr.s.n;
			step = 1;
		}
		if(order == array_order::C_CONTIGUOUS)
		{
			start = arr.s.n - 1;
			end = -1;
			step = -1;
		}
		for(u64 i = start; i != end; i += step)
		{
			size = size * arr.s.val[i];
			stride[i] = current_stride * dtype;
			++j;
			current_stride = current_stride * arr.s.val[i];
		}
		this->stride = stride;
		a = new number_type[size];
		ref = refcount(a);

		for(u64 i = 0; i < size; ++i)
		{
			narray idx_in_other = get_index(i);
			a[i] = (number_type)arr.get(idx_in_other);
		}

	}

	template<typename number_type, array_order order>
	inline
	ndarray<number_type, order>::~ndarray()
	{

	}

	template<typename number_type, array_order order>
	inline
	ndarray<number_type, order>::ndarray(number_type *data, shape s, strides stride, refcount ref)
	{
		this->s = s;
		this->stride = stride;
		this->ref = ref;
		this->a = data;
	}

	template<typename number_type, array_order order>
	inline
	number_type ndarray<number_type, order>::get(std::initializer_list<u64> index) const
	{
		number_type* result_index = a;
		const auto size = index.size();
		u64 * __restrict__ current_stride = stride.val;
		for(const auto idx : index)
		{
			result_index += (*current_stride >> log2<sizeof(number_type)>::value) * idx;
			++current_stride;
		}
		return *result_index;
	}

	template<typename number_type, array_order order>
	inline
	number_type ndarray<number_type, order>::get(const narray& index) const
	{
		number_type* result_index = a;
		const auto size = index.n;
		u64 * __restrict__ current_stride = stride.val;
		for(u64 i = 0; i < index.n; ++i)
		{
			result_index += (*current_stride >> log2<sizeof(number_type)>::value) * index.val[i];
			++current_stride;
		}
		return *result_index;
	}

	template<typename number_type, array_order order>
	inline
	void ndarray<number_type, order>::set(std::initializer_list<u64> index, number_type value)
	{
		number_type* result_index = a;
		const auto size = index.size();
		u64 * __restrict__ current_stride = stride.val;
		for(const auto idx : index)
		{
			result_index += (*current_stride >> log2<sizeof(number_type)>::value) * idx;
			++current_stride;
		}
		*result_index = value;
	}

	template<typename number_type, array_order order>
	inline
	void ndarray<number_type, order>::set(narray index, number_type value)
	{
		number_type* result_index = a;
		const auto size = index.n;
		u64 * current_stride = stride.val;
		for(u64 i = 0; i < index.n; ++i)
		{
			result_index += ( (*current_stride) >> log2<sizeof(number_type)>::value) * index.val[i];

			current_stride++;
		}
		*result_index = value;
	}

	template<typename number_type, array_order order>
	inline	
	narray ndarray<number_type, order>::get_index(u64 index) const
	{
		narray idx(s.n);
		u64 step;
		u64 start;
		u64 end;
		if constexpr (order == array_order::F_CONTIGUOUS)
		{
			step = -1;
			start = s.n - 1;
		    end = 0;	
		}
		if constexpr (order == array_order::C_CONTIGUOUS)
		{
			step = 1;
			start = 0;
			end = s.n - 1;
		}

		u64 index_iter = start;
		for(u64 i = start; i != end; i += step, index_iter += step)
		{
			idx.val[index_iter] = index / (stride.val[i] >> log2<sizeof(number_type)>::value);
			index = index % (stride.val[i] >> log2<sizeof(number_type)>::value);
		}
		idx.val[index_iter] = index;
		return idx;
	}

	template<typename number_type, array_order order>
	u8 *ndarray<number_type, order>::get_data() const
	{
		return (u8*)a;
	}
	
	template<typename number_type, array_order order>
	u8 *ndarray<number_type, order>::get_element(std::initializer_list<u64> indices) const
	{
		number_type* result_index = a;
		const auto size = indices.size();
		u64 * __restrict__ current_stride = stride.val;
		for(const auto idx : indices)
		{
			result_index += (*current_stride >> log2<sizeof(number_type)>::value) * idx;
			current_stride++;
		}
		return (u8*)result_index;
	}

	template<typename number_type, array_order order>
	u64 ndarray<number_type, order>::get_ndims() const
	{
		return s.n;
	}

	template<typename number_type, array_order order>
	u64 ndarray<number_type, order>::get_flags() const
	{
		return 0x0;
	}

	template<typename number_type, array_order order>
	array_order ndarray<number_type, order>::get_order() const
	{
		return order;
	}

	template<typename number_type, array_order order>
	u8 ndarray<number_type, order>::get_dtype() const
	{
		return order;
	}

	template<typename number_type, array_order order>
	shape ndarray<number_type, order>::get_shape() const
	{
		return s;
	}

	template<typename number_type, array_order order>
	ndarray<number_type, order> ndarray<number_type, order>::slice(index_bound lower, index_bound upper)
	{
		u64 ndims = 0;
		u8 *offset = (u8*)a;
		strides slice_strides(lower.n);
		shape slice_shape(lower.n);
		for(u64 i = 0; i < lower.n; ++i)
		{
			offset += stride[i] * lower[i]; 
			if(upper.val[i] > lower.val[i])
			{
				slice_strides[ndims] = stride[i];
				slice_shape[ndims] = upper[i] - lower[i];
				ndims++;
			}
		}

		slice_strides.resize(ndims);
		slice_shape.resize(ndims);
		return ndarray((number_type*)offset, slice_shape, slice_strides, ref);
	}
}

#endif
