#include <numcpp/narray.hpp>
#include <algorithm>

using namespace npp;

narray::narray(): narray(0)
{}

narray::narray(std::initializer_list<u64> dims)
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

narray::narray(u64 n)
{
	val = new u64[n];
	this->n = n;
	for(u64 i = 0;i < n; i++)
	{
	 val[i] = 0;
	}
}

narray::narray(const narray &other)
{
	n = other.n;
	val = new u64[n];
	for(u64 i = 0; i < n; i++)
	{
	val[i] = other.val[i];
	}
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
	delete[] val;
	val = new_arr;
	this->n = n;
}


narray& narray::operator=(const narray &other)
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

u64& narray::operator[](u64 i)
{
	return val[i];
}

narray::~narray()
{
	delete[] val;
}
