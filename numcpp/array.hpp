#ifndef NUMCPP_ARRAY_H
#define NUMCPP_ARRAY_H

#include <numcpp/defs.hpp>
#include <numcpp/narray.hpp>

#include <initializer_list>

namespace npp 
{
	
	class array
	{
	public:
		virtual u8 *get_data() const = 0;
		virtual u8 *get_element(std::initializer_list<u64> indices) const = 0;
		virtual u64 get_ndims() const = 0;
		virtual u64 get_flags() const = 0;
		virtual array_order get_order() const = 0;
		virtual u8 get_dtype() const = 0;
		virtual shape get_shape() const = 0;

	};
}

#endif
