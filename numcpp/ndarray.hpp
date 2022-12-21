#ifndef NUMCPP_NDARRAY_H
#define NUMCPP_NDARRAY_H

#include <numcpp/defs.hpp>
#include <numcpp/narray.hpp>
#include <numcpp/refcount.hpp>

#include <initializer_list>

namespace npp
{
	typedef narray shape;
	typedef narray ndarray_index;
	typedef narray index_bound;
	typedef narray strides;

	class ndarray
	{
	public:
		explicit ndarray(shape array_shape, u8 dtype, array_order order=array_order::F_CONTIGUOUS);

		~ndarray();

		ndarray(u8 *data, shape s, u8 dtype, array_order order, strides stride, refcount ref_count);

	public:
		template<typename T>
		T get(ndarray_index index)
		{
			u8* result_index = a;
			for(u64 i = 0; i < index.n; i++)
			{
				result_index += stride[i] * index[i];
			}
			return *((T*)result_index);
		}

		template<typename T>
		void set(ndarray_index index, T value)
		{
			u8* result_index = a;
			for(u64 i = 0;i < index.n; i++)
			{
				result_index += stride[i] * index[i];
			}
			*((T*)result_index) = value;
		}
		
		ndarray slice(index_bound lower, index_bound upper);

		shape s;
		strides stride;
		u8 *a;
		u8 dtype;
		array_order order;

	 protected:
		refcount ref;
	};
}

#endif
