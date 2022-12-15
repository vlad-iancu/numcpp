#include <numcpp/ndarray.hpp>

using namespace npp;

ndarray::ndarray(shape array_shape, u8 dtype, array_order order)
	: 
		s(array_shape),
		lower(array_shape.n),
		upper(array_shape),
		view_shape(array_shape),
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
		size = size * array_shape[i];
		stride[i] = current_stride * dtype;
		j++;
		current_stride = current_stride * array_shape[i];
	}
	size = size * dtype;
	a = new u8[size];
	ref = refcount(a);
	this->dtype = dtype;
	this->order = order;
	this->offset = a;
	this->stride = stride;
}

ndarray::~ndarray()
{
}

 ndarray::ndarray(shape s, shape view_shape, index_bound upper, index_bound lower, u8 dtype, array_order order, strides stride, u8 *offset, refcount ref_count):
	 ref(ref_count)
{
	this->s = s;
	this->upper = upper;
	this->lower = lower;
	this->dtype = dtype;
	this->order = order;
	this->stride = stride;
	this->offset = offset;
	this->ref = ref_count;
	this->view_shape = view_shape;
	this->a = ref_count.data;
}
ndarray ndarray::slice(index_bound lower, index_bound upper)
{
	u64 ndims = 0;
	u8 *offset = a;
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

	slice_strides.resize(ndims);
	slice_shape.resize(ndims);
	return ndarray(s, slice_shape, upper, lower, dtype, order, slice_strides, offset, ref);
}
