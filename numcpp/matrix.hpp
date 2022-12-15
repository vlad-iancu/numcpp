#ifndef NUMCPP_MATRIX_H
#define NUMCPP_MATRIX_H

#include <numcpp/defs.hpp>
#include <numcpp/refcount.hpp>

namespace npp
{

	class matrix
	{
	public:
		
		explicit matrix(u64 n, u64 m, u8 dtype)
			: ref(nullptr)
		{
			this->n = n;
			this->m = m;
			this->n_view = n;
			this->m_view = m;
			this->i_view = 0;
			this->j_view = 0;
			this->dtype = dtype;
			a = new u8[n * m * dtype];
			ref = refcount(a);
		}

		explicit matrix(u64 n)
			: matrix(n, n, 4)
		{}

		matrix(const matrix &other)
			: ref(nullptr)
		{
			this->n = other.n;
			this->m = other.m;
			this->n_view = other.n_view;
			this->m_view = other.m_view;
			this->i_view = other.i_view;
			this->j_view = other.j_view;
			this->ref= other.ref;
			this->dtype = other.dtype;
			a = other.a;
		}

	protected:
		explicit matrix
		(
			u64 n,
			u64 m,
			u64 i_view, 
			u64 j_view, 
			u64 n_view, 
			u64 m_view,
			u8 *a,
			refcount ref
		)
			: ref(nullptr)
		{
			this->a = a;
			this->n = n;
			this->m = m;
			this->i_view = i_view;
			this->j_view = j_view;
			this->n_view = n_view;
			this->m_view = m_view;
			this->a = a;
			this->ref = ref;
		}

	protected:
		u64 n;
		u64 m;
		u64 i_view;
		u64 j_view;
		u64 n_view;
		u64 m_view;
		refcount ref;

	public:
		u8 *a;
		u8 dtype;

	public:
		u64 lines() const
		{
			return n_view;
		}

		u64 columns() const
		{
			return m_view;
		}

		template <typename T>
		inline T get(u64 i, u64 j) const
		{
			return ((T*)a)[n * (j_view + j) + (i_view + i)];
		}

		template <typename T>
		inline void set(u64 i, u64 j, T value)
		{
			((T*)a)[n * (j_view + j) + (i_view + i)] = value;
		}

		matrix slice(u64 start_line, u64 end_line, u64 start_column, u64 end_column) const
		{
			return matrix(
					n,
					m,
					i_view + start_line,
					j_view + start_column,
					end_line - start_line + 1,
					end_column - start_column + 1,
					a,
					ref
					);
		}

	};
}
#endif
