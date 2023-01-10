#ifndef NUMCPP_MATRIX_BONES_H
#define NUMCPP_MATRIX_BONES_H

#include <numcpp/defs.hpp>
#include <numcpp/refcount.hpp>
#include <numcpp/matrix_layout.hpp>

namespace npp
{
	
	template<typename number_type, array_order order = array_order::F_CONTIGUOUS>
	class matrix
	{
	public:
		
		using layout = matrix_layout<number_type, order>;
		static constexpr u8 dtype = sizeof(number_type);

		inline explicit matrix(u64 n, u64 m);

		inline explicit matrix(u64 n);

		matrix(const matrix &other);

	protected:
		inline explicit matrix
		(
			u64 n,
			u64 m,
			number_type *a,
			refcount ref
		);


	public:
		inline u64 lines() const;

		inline u64 columns() const;

		inline number_type get(u64 i, u64 j) const;

		inline void set(u64 i, u64 j, number_type value);

		matrix slice(u64 start_row, u64 end_row, u64 start_column, u64 end_column) const;

	public:
		u64 n;
		u64 m;
		refcount ref;
		number_type *a;

	};
}
#endif
