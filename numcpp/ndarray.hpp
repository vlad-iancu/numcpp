#ifndef NUMCPP_NDARRAY_H
#define NUMCPP_NDARRAY_H

#include <numcpp/defs.hpp>
#include <initializer_list>
namespace npp
{
	struct narray
	{
		u64 n;
		u64 *val;
		narray(): narray(0)
		{

		}
		narray(std::initializer_list<u64> dims)
		{
			n = dims.size();
			val = new u64[n];
			u64 i = 0;
			for(auto n : dims)
			{
				val[i] = n;
				i++;
			}
		}

		explicit narray(u64 n)
		{
			val = new u64[n];
			this->n = n;
			for(u64 i = 0;i < n; i++)
			{
			 val[i] = 0;
			}
		}
		
		narray(const narray &other)
		{
			n = other.n;
			val = new u64[n];
			for(u64 i = 0; i < n; i++)
			{
				val[i] = other.val[i];
			}
		}
		
		narray& operator=(const narray &other)
		{
			if(&other != this)
			{
				delete[] val;
				this->n = other.n;
				this->val = new u64[n];
				for(u64 i = 0;i < n; i++)
				{
					val[i] = other.val[i];
				}
			}
			return *this;
		}

		u64& operator[](u64 i)
		{
			return val[i];
		}

		~narray()
		{
			delete[] val;
		}

	};
	typedef narray shape;
	typedef narray ndarray_index;
	typedef narray index_bound;
	typedef narray strides;

	class ndarray
	{
	public:
		explicit ndarray(shape array_shape, u8 dtype, array_order order=array_order::F_CONTIGUOUS)
			: s(array_shape),
			  lower(array_shape.n),
			  upper(array_shape),
			  view_shape(array_shape)

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
				size = size * array_shape[i];
				stride[i] = current_stride * dtype;
				j++;
				current_stride = current_stride * array_shape[i];
			}
			size = size * dtype;
			a = new u8[size];
			ref_count = new u64(1);
			this->dtype = dtype;
			this->order = order;
			this->offset = 0;
			this->stride = stride;
		}
		~ndarray()
		{
			(*ref_count)--;
			if(*ref_count <= 0)
			{
				delete[] (u8*)a;
			}
		}

	protected:
		ndarray(shape s, index_bound upper, index_bound lower, u8 dtype, array_order order, strides stride, u64 offset, u64 *ref_count)
		{
			this->s = s;
			this->upper = upper;
			this->lower = lower;
			this->dtype = dtype;
			this->order = order;
			this->stride = stride;
			this->offset = offset;
			this->ref_count = ref_count;
			(*ref_count)++;
		}

	public:
		template<typename T>
		T get(ndarray_index index)
		{
		/*	u64 start;
			u64 stop;
			u64 step;
			u64 res;
			if(order == array_order::F_CONTIGUOUS)
			{
				start = index.n - 1;
				stop = 0;
				step = -1;
				res = index.val[index.n- 1];
			}
			if(order == array_order::C_CONTIGUOUS)
			{
				start = 1;
				stop = index.n;
				step = 1;
				res = index.val[1];
			}
			for(u64 i = start; i != stop; i += step)
			{
				res = res * s.val[i - 1] + index.val[i - 1];
			}
			return ((T*)a)[res]; */
			u8* result_index = a;
			result_index += offset;
			for(u64 i = 0;i < index.n; i++)
			{
				result_index += stride[i] * index[i];
			}
			return *((T*)result_index);
		}

		template<typename T>
		void set(ndarray_index index, T value)
		{
		 /* u64 start;
			u64 stop;
			u64 step;
			u64 res;
			if(order == array_order::F_CONTIGUOUS)
			{
				start = index.n- 1;
				stop = 0;
				step = -1;
				res = index.val[index.n- 1];
			}
			if(order == array_order::C_CONTIGUOUS)
			{
				start = 1;
				stop = index.n;
				step = 1;
				res = index.val[1];
			}
			for(u64 i = start; i != stop; i += step)
			{
				res = res * s.val[i - 1] + index.val[i - 1] - lower.val[i - 1];
			}
			((T*)a)[res] = value; */
			u8* result_index = a;
			result_index += offset;
			for(u64 i = 0;i < index.n; i++)
			{
				result_index += stride[i] * index[i];
			}
			*((T*)result_index) = value;
		}
		
		ndarray slice(index_bound lower, index_bound upper)
		{
			u64 ndims = 0;
			u64 offset = 0;
			strides slice_strides(lower.n);
			shape slice_shape(lower.n);
			for(u64 i = 0;i < lower.n; i++)
			{
				offset += stride[i] * (lower[i] - this->lower[i]);
				if(upper[i] - lower[i] > 0)
				{
					slice_strides[ndims] = stride[i];
					slice_shape[ndims] = upper[i] - lower[i];
					ndims++;
				}
			}

			slice_strides.n = ndims;
			slice_shape.n = ndims;
			return ndarray(slice_shape, upper, lower, dtype, order, slice_strides, offset, ref_count);
		}

		shape s;
		shape view_shape;
		index_bound lower;
		index_bound upper;
		strides stride;
		u64 offset;
		u8 *a;
		u8 dtype;
		array_order order;

	 protected:
		u64 *ref_count;
	};
}

#endif
