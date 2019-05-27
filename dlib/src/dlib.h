#ifndef DLIB_H
#define DLIB_H

#include <sstream>
#include <exception>
#include <new>
#include <algorithm>

#define DLIBM_CASSERT(_exp,_message)												\
	{if ( !(_exp) )																	\
	{																				\
		std::ostringstream dlib_o_out;												\
		dlib_o_out << "\n\nError detected at line " << __LINE__ << ".\n";			\
		dlib_o_out << "Error detected in file " << __FILE__ << ".\n";				\
		dlib_o_out << "Error detected in function " << __FUNCTION__ << ".\n\n";     \
		dlib_o_out << "Failing expression was " << #_exp << ".\n";					\
		dlib_o_out << std::boolalpha << _message << "\n";							\
		throw std::runtime_error(dlib_o_out.str().c_str());								\
	}}

typedef std::allocator<char> default_memory_manager;

#define std_allocator std::allocator

#define TIMEOUT -1
#define WOULDBLOCK -2
#define OTHER_ERROR -3
#define SHUTDOWN -4
#define PORTINUSE -5

#ifdef __GNUC__
typedef unsigned long long uint64;
typedef long long int64;
#elif defined(__BORLANDC__)
typedef unsigned __int64 uint64;
typedef __int64 int64;
#elif defined(_MSC_VER)
typedef unsigned __int64 uint64;
typedef __int64 int64;
#else
typedef unsigned long long uint64;
typedef long long int64;
#endif

typedef unsigned short uint16;
typedef unsigned int   uint32;
typedef unsigned char  uint8;

typedef short int16;
typedef int   int32;
typedef char  int8;

template <typename T, size_t s = sizeof(T)>
struct unsigned_type;
template <typename T>
struct unsigned_type<T, 1> { typedef uint8 type; };
template <typename T>
struct unsigned_type<T, 2> { typedef uint16 type; };
template <typename T>
struct unsigned_type<T, 4> { typedef uint32 type; };
template <typename T>
struct unsigned_type<T, 8> { typedef uint64 type; };

// ----------------------------------------------------------------------------------------
// WIN32Ãÿ ‚¥¶¿Ì
// ----------------------------------------------------------------------------------------
#ifdef WIN32

#ifndef NOMINMAX
#define NOMINMAX // prevent windows from messing with std::min and std::max
#endif

// now just for good measure undefine min and max if they are defined
#ifdef min
#undef min
#endif

#ifdef max 
#undef max 
#endif

#endif 

#endif /* DLIB_H */