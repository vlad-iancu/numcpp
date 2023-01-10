#ifndef NUMCPP_MATRIX_MEAT_H
#define NUMCPP_MATRIX_MEAT_H

#include <numcpp/matrix_bones.hpp>
#include <numcpp/defs.hpp>

using namespace npp;

template<typename number_type, array_order order>
inline
matrix<number_type, order>::matrix(u64 n, u64 m)
	: ref(nullptr)
{
	this->n = n;
	this->m = m;
	a = new number_type[n * m];
	ref = refcount(a);
}

template<typename number_type, array_order order>
inline
matrix<number_type, order>::matrix(u64 n)
		: matrix(n, n, 4)
{}

template<typename number_type, array_order order>
inline
matrix<number_type, order>::matrix(u64 n, u64 m, number_type * __restrict__ a, refcount ref)
	: ref(nullptr)
{
	this->a = a;
	this->n = n;
	this->m = m;
	this->a = a;
	this->ref = ref;
}

template<typename number_type, array_order order>
inline
matrix<number_type, order>::matrix(const matrix<number_type, order> &other)
	: ref(nullptr)
{
	this->a = other.a;
	this->n = other.n;
	this->m = other.m;
	this->a = other.a;
	this->ref = other.ref;

}

template<typename number_type, array_order order>
inline
u64 matrix<number_type, order>::lines() const
{
	return n;
}

template<typename number_type, array_order order>
inline
u64 matrix<number_type, order>::columns() const
{
	return m;
}

template<typename number_type, array_order order>
inline
number_type matrix<number_type, order>::get(u64 i, u64 j) const
{
	return layout::value(a, i, j, n, m);
}

template<typename number_type, array_order order>
inline
void matrix<number_type, order>::set(u64 i, u64 j, number_type value) 
{
	*layout::element(a, i, j, n, m) = value;
}

template<typename number_type, array_order order>
inline
matrix<number_type, order> matrix<number_type, order>::slice(u64 start_row, u64 end_row, u64 start_column, u64 end_column) const
{
	return matrix(
			end_row - start_row + 1,
			end_column - start_column + 1,
			layout::element(a, start_row, start_column, n, m, dtype),
			ref
			);
}

#endif
