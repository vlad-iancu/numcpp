#include <numcpp/refcount.hpp>

using namespace npp;

refcount::refcount(void *data)
{
	ref_count = new u64(1);
	this->data = data;
}

refcount::refcount(const refcount &other)
	: data(other.data)
{
	ref_count = other.ref_count;
	data = other.data;
	increment();
}

 refcount::~refcount()
{
	decrement();
	if(!is_valid())
	{
		delete[] data;
		delete ref_count;
	}
}

bool refcount::is_valid() const
{
	return (*ref_count) > 0;
}

void refcount::increment()
{
	(*ref_count)++;
}

void refcount::decrement()
{
	(*ref_count)--;
}

refcount& refcount::operator=(const refcount &other)
{
	if(&other != this)
	{
		data = other.data;
		ref_count = other.ref_count;
		increment();
	}
	return *this;
}

u64 refcount::count() const
{
	return *ref_count;
}
