#ifndef NUMCPP_DEFS_H
#define NUMCPP_DEFS_H

#include <cstdint>

namespace npp
{

	typedef std::int64_t  i64;
	typedef std::uint64_t u64;
	typedef std::int32_t  i32;
	typedef std::uint32_t u32;
	typedef std::int64_t  i16;
	typedef std::uint16_t u16;
	typedef std::int8_t   i8;
	typedef std::uint8_t  u8;

	constexpr u8 int64 = 8; 
	constexpr u8 uint64 = 8;
	constexpr u8 int32 = 4;
	constexpr u8 uint32 = 4;
	constexpr u8 int16 = 2;
	constexpr u8 uint16 = 2;
	constexpr u8 int8 = 1;
	constexpr u8 uint8 = 1;

	typedef float         f4;
	typedef double        f8;

	enum array_order
	{
		C_CONTIGUOUS = 1,
		F_CONTIGUOUS = 2
	};
}

#endif
