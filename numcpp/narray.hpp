#ifndef NUMCPP_NARRAY_H
#define NUMCPP_NARRAY_H

#include <numcpp/defs.hpp>
#include <numcpp/refcount.hpp>

#include <initializer_list>

namespace npp
{
	//defines a positive n-sized array, mainly used for indices and shapes of ndarrays
	struct narray
	{
		u64 n;
		u64 *val;

		explicit narray();
		narray(std::initializer_list<u64> dims);

		explicit narray(u64 n);

		narray(const narray &other);
		
		void resize(u64 n, u64 value = 0);

		narray& operator=(const narray &other);

		u64& operator[](u64 i);
		u64& operator[](u64 i) const;

		~narray();
		
		refcount ref;
	};

	typedef narray shape;
	typedef narray index_bound;
	typedef narray strides;
}

#endif
