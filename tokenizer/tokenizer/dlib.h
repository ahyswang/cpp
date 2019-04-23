#ifndef DLIB_H
#define DLIB_H

#include <sstream>
#include <exception>
#include <new>

#define DLIBM_CASSERT(_exp,_message)                                              \
	{if ( !(_exp) )                                                         \
	{                                                                       \
		std::ostringstream dlib_o_out;                                       \
		dlib_o_out << "\n\nError detected at line " << __LINE__ << ".\n";    \
		dlib_o_out << "Error detected in file " << __FILE__ << ".\n";      \
		dlib_o_out << "Error detected in function " << __FUNCTION__ << ".\n\n";      \
		dlib_o_out << "Failing expression was " << #_exp << ".\n";           \
		dlib_o_out << std::boolalpha << _message << "\n";                    \
		throw std::exception(dlib_o_out.str().c_str());      \
	}}

typedef std::allocator<char> default_memory_manager;

#define std_allocator std::allocator


#endif /* DLIB_H */