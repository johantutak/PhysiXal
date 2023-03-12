#pragma once

#include <atomic>

// Atomic
typedef std::atomic<signed char>			 SAFE_I8; // Signed char			- 1 byte signed integer				  (8 bit)
typedef std::atomic<short>					SAFE_I16; // Short					- 2 byte signed integer			     (16 bit)         
typedef std::atomic<int>					SAFE_I32; // Int					- 4 byte signed integer			     (32 bit)
typedef std::atomic<long long>				SAFE_I64; // Long long				- 8 byte signed integer			     (64 bit)

typedef std::atomic<unsigned char>			 SAFE_U8; // Unsigned char			- 1 byte unsigned integer			  (8 bit)
typedef std::atomic<unsigned short>		    SAFE_U16; // Unsigned short			- 2 byte unsigned integer		     (16 bit)
typedef std::atomic<unsigned int>		    SAFE_U32; // Unsigned int			- 4 byte unsigned integer		     (32 bit)
typedef std::atomic<unsigned long long>		SAFE_U64; // Unsigned long long		- 8 byte unsigned integer			 (64 bit)

typedef std::atomic<float>					SAFE_F32; // Float					- 4 byte floating point				 (32 bit)
typedef std::atomic<double>					SAFE_F64; // Double					- 8 byte double floating point		 (64 bit)
typedef std::atomic<long double>		   SAFE_F128; // Long double			- 16/8 byte double floating point	(128 bit)

typedef std::atomic<char>					 SAFE_C8; // Character				- 1 byte character					  (8 bit)
typedef std::atomic<wchar_t>				SAFE_C16; // Wide character			- 2 byte character					 (16 bit)

typedef std::atomic<bool>					 SAFE_B8; // Boolean				- 1 byte boolean					  (8 bit)

// Non atomic
typedef signed char								  I8; // Signed char			- 1 byte signed integer				  (8 bit)
typedef short									 I16; // Short					- 2 byte signed integer				 (16 bit)         
typedef int										 I32; // Int					- 4 byte signed integer			     (32 bit)
typedef long long								 I64; // Long long				- 8 byte signed integer				 (64 bit)

typedef unsigned char							  U8; // Unsigned char			- 1 byte unsigned integer		      (8 bit)
typedef unsigned short							 U16; // Unsigned short			- 2 byte unsigned integer			 (16 bit)
typedef unsigned int							 U32; // Unsigned int			- 4 byte unsigned integer		     (32 bit)
typedef unsigned long long						 U64; // Unsigned long long		- 8 byte unsigned integer			 (64 bit)

typedef float							         F32; // Float					- 4 byte floating point				 (32 bit)
typedef double							         F64; // Double					- 8 byte double floating point		 (64 bit)
typedef long double								F128; // Long double			- 16/8 byte double floating point	(128 bit)

typedef char									  C8; // Character				- 1 byte character					  (8 bit)
typedef wchar_t								     C16; // Wide character			- 2 byte character					 (16 bit)

typedef bool									  B8; // Boolean				- 1 byte boolean					  (8 bit)

// Macros
#define pnew new
#define pdelete delete
