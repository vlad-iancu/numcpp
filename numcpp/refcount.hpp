#ifndef NUMCPP_REFCOUNT_H
#define NUMCPP_REFCOUNT_H

#include <numcpp/defs.hpp>

namespace npp
{
	// We won't use std::shared_ptr because it would add too much bloat on the
	// ndarray class
	struct refcount
	{
		u8 *data;
		u64 *ref_count;
		
		refcount(u8 *data);
		refcount(const refcount &other);
		~refcount();

		bool is_valid() const;
		
		void increment();
		void decrement();

		refcount& operator=(const refcount &other);

		u64 count() const;

	};
}

#endif
