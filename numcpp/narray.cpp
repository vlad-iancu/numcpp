#include <numcpp/narray.hpp>
#include <algorithm>

using namespace npp;

narray::narray(): narray(0)
{}

narray::narray(std::initializer_list<u64> dims)
	: val(new u64[dims.size()]),
	  n(dims.size()),
	  ref((u8*)val)
{
	n = dims.size();
	u64 i = 0;
	for(auto n : dims)
	{
		val[i] = n;
		i++;
	}
}

narray::narray(u64 n)
	: val(new u64[n]),
	  ref((u8*)val)
{
	this->n = n;
	for(u64 i = 0; i < n; i++)
	{
		val[i] = 0;
	}
}

narray::narray(const narray &other)
	: val(new u64[other.n]),
	  ref((u8*)val),
	  n(other.n)
{
	n = other.n;
	ref = other.ref;
	val = other.val;
}

void narray::resize(u64 n, u64 value)
{
	u64 *new_arr = new u64[n];
	u64 lim = std::min(n, this->n);
	for(u64 i = 0; i < lim; i++)
	{
		new_arr[i] = val[i];
	}
	u64 *continuation = new_arr + lim;
	for(u64 i = 0; i < n - lim; i++)
	{
		continuation[i] = value;
	}
	refcount new_ref((u8*)new_arr);
	ref = new_ref;
	val = new_arr;
	this->n = n;
}


narray& narray::operator=(const narray &other)
{
	if(&other != this)
	{
		this->ref = other.ref;
		this->val = other.val;
		this->n = other.n;
	}
	return *this;
}

u64& narray::operator[](u64 i)
{
	return val[i];
}

narray::~narray()
{
}
