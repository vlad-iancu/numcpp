#ifndef NUMCPP_NDARRAY_BONES_H
#define NUMCPP_NDARRAY_BONES_H

#include <numcpp/defs.hpp>
#include <numcpp/narray.hpp>
#include <numcpp/array.hpp>
#include <numcpp/refcount.hpp>

#include <initializer_list>

namespace npp
{
	template<u64 number>
	struct log2;

	template<typename number_type, array_order order = array_order::F_CONTIGUOUS>
	class ndarray : public array
	{
	public:
		static constexpr u8 dtype = sizeof(number_type);

		inline explicit ndarray(const shape& array_shape);

		ndarray(const ndarray<number_type, order>& other) = default;

		template<typename other_number_type, array_order other_order>
		ndarray(const ndarray<other_number_type, other_order> &other);

		inline ~ndarray();

		inline ndarray(number_type *data, shape s, strides stride, refcount ref_count);

	public:
		inline number_type get(std::initializer_list<u64> index) const;

		inline number_type get(const narray& index) const;

		inline narray get_index(u64 index) const;

		inline void set(std::initializer_list<u64> index, number_type value);

		inline void set(narray index, number_type value);
		
		u8 *get_data() const;
	
		u8 *get_element(std::initializer_list<u64> indices) const;

		u64 get_ndims() const;

		u64 get_flags() const;

		array_order get_order() const;

		u8 get_dtype() const;

		shape get_shape() const;

		ndarray slice(index_bound lower, index_bound upper);

		shape s;
		strides stride;
		number_type *a;

	 protected:
		refcount ref;
	};
}

#endif
