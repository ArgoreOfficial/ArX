#pragma once

#include <string>

namespace arg {

template<typename _Ty> struct typename_of;

template<typename _Ty>
struct typename_of<_Ty*> { 
	using Ty = _Ty; 
};

template<typename _Ty, typename _Mty>
struct typename_of<_Ty _Mty::*> { 
	using Ty = _Ty; 
};

template<typename _Ty, auto _Ty::* ptr>
constexpr const char* member_name() {
	return typeid( typename_of<decltype( ptr )>::Ty ).name();
}


template<typename T>
T str_to_T( const char* _str ) {
	return static_cast<T>( str_to_T<int32_t>( _str ) );
}

template<> const char* str_to_T<const char*>( const char* _str ) { return _str; }
template<> int32_t     str_to_T<int32_t>    ( const char* _str ) { return std::stoi( _str ); }
template<> float       str_to_T<float>      ( const char* _str ) { return std::stof( _str ); }

}
