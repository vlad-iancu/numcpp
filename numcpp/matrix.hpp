#ifndef NUMCPP_MATRIX_H
#define NUMCPP_MATRIX_H

#include <numcpp/defs.hpp>

namespace npp
{

	template<typename T>
	class matrix
	{
	public:
		
		explicit matrix(u64 n, u64 m)
		{
			this->n = n;
			this->m = m;
			this->n_view = n;
			this->m_view = m;
			this->i_view = 0;
			this->j_view = 0;
			a = new T[n * m];
			ref_count = new u64(1);
		}

		explicit matrix(u64 n)
			: matrix(n, n)
		{}

		matrix(const matrix<T> &other)
		{
			this->n = other.n;
			this->m = other.m;
			this->n_view = other.n_view;
			this->m_view = other.m_view;
			this->i_view = other.i_view;
			this->j_view = other.j_view;
			this->ref_count = other.ref_count;
			a = other.a;
			*ref_count++;
		}

		~matrix()
		{
			(*ref_count)--;
			if(*ref_count <= 0)
			{
				delete[] a;
			}
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
			T *a,
			u64 *ref_count
		)
		{
			this->a = a;
			this->n = n;
			this->m = m;
			this->i_view = i_view;
			this->j_view = j_view;
			this->n_view = n_view;
			this->m_view = m_view;
			this->a = a;
			this->ref_count = ref_count;
			*ref_count++;
		}

	protected:
		u64 n;
		u64 m;
		u64 i_view;
		u64 j_view;
		u64 n_view;
		u64 m_view;
		u64 *ref_count;
		T *a;

	public:
		u64 lines() const
		{
			return n_view;
		}

		u64 columns() const
		{
			return m_view;
		}

		T get(u64 i, u64 j) const
		{
			return a[n * (j_view + j) + (i_view + i)];
		}

		void set(u64 i, u64 j, T value)
		{
			a[n * (j_view + j) + (i_view + i)] = value;
		}

		matrix<T> slice(u64 start_line, u64 end_line, u64 start_column, u64 end_column) const
		{
			return matrix<T>(
					n,
					m,
					i_view + start_line,
					j_view + start_column,
					end_line - start_line + 1,
					end_column - start_column + 1,
					a,
					ref_count
					);
		}

	};
}
#endif
