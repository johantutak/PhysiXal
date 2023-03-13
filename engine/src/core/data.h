#pragma once

#include <atomic>

// Atomic
typedef std::atomic<int8_t> SAFE_I8; // Signed char	- 1 byte signed integer	(8 bit)
typedef std::atomic<int16_t> SAFE_I16; // Short	- 2 byte signed integer	(16 bit)         
typedef std::atomic<int32_t> SAFE_I32; // Int - 4 byte signed integer (32 bit)
typedef std::atomic<int64_t> SAFE_I64; // Long long	- 8 byte signed integer	(64 bit)

typedef std::atomic<uint8_t> SAFE_U8; // Unsigned char - 1 byte unsigned integer (8 bit)
typedef std::atomic<uint16_t> SAFE_U16; // Unsigned short - 2 byte unsigned integer (16 bit)
typedef std::atomic<uint32_t> SAFE_U32; // Unsigned int	- 4 byte unsigned integer (32 bit)
typedef std::atomic<uint64_t> SAFE_U64; // Unsigned long long - 8 byte unsigned integer	(64 bit)

typedef std::atomic<float> SAFE_F32; // Float - 4 byte floating point (32 bit)
typedef std::atomic<double>	SAFE_F64; // Double	- 8 byte double floating point (64 bit)
typedef std::atomic<long double> SAFE_F128; // Long double	- 16/8 byte double floating point (128 bit)

typedef std::atomic<char> SAFE_C8; // Character	- 1 byte character (8 bit)
typedef std::atomic<wchar_t> SAFE_C16; // Wide character - 2 byte character	(16 bit)

typedef std::atomic<bool> SAFE_B8; // Boolean - 1 byte boolean (8 bit)

// Non atomic
typedef int8_t I8; // Signed char - 1 byte signed integer (8 bit)
typedef int16_t	I16; // Short - 2 byte signed integer (16 bit)         
typedef int32_t	I32; // Int	- 4 byte signed integer	(32 bit)
typedef int64_t	I64; // Long long - 8 byte signed integer (64 bit)

typedef uint8_t U8; // Unsigned char - 1 byte unsigned integer (8 bit)
typedef uint16_t U16; // Unsigned short	- 2 byte unsigned integer (16 bit)
typedef uint32_t U32; // Unsigned int - 4 byte unsigned integer (32 bit)
typedef uint64_t U64; // Unsigned long long - 8 byte unsigned integer (64 bit)

typedef float F32; // Float	 - 4 byte floating point (32 bit)
typedef double F64; // Double - 8 byte double floating point (64 bit)
typedef long double	F128; // Long double - 16/8 byte double floating point (128 bit)

typedef char C8; // Character - 1 byte character (8 bit)
typedef wchar_t C16; // Wide character - 2 byte character (16 bit)

typedef bool B8; // Boolean	- 1 byte boolean (8 bit)

// Macros
#define pnew new
#define pdelete delete
