#include <stdio.h>

#include "test.hpp"
#include <stdint.h>

#include <typeinfo>
#include <vector>
#include <array>

struct test_struct
{
	uint16_t member_int;
	float member_float;
	char member_char;
};


template<typename _Ty> struct typename_of;
template<typename _Ty> struct typename_of<_Ty*> { using Ty = _Ty; };
template <typename _Ty, typename _Mty> struct typename_of<_Ty _Mty::*> { using Ty = _Ty; };

struct memvar_base { virtual void set( void* ) = 0; };
template<typename _Ty>
struct memvar : public memvar_base
{
	virtual void set( void* _data ) override { 
		printf( "%i\n", (_Ty*)( _data ) );
	}
};

template<typename _Ty, auto _Ty::*ptr>
void dothing(){
	printf("%s", typeid( typename_of<decltype( ptr )>::Ty ).name() );
}

template<typename _Ty, auto _Ty::*ptr>
const char* mname()
{
	return typeid( typename_of<decltype( ptr )>::Ty ).name();
}

template<typename _Ty, auto _Ty::* ..._Members>
void refl_struct( const std::array<const char*, sizeof...( _Members )>& _names )
{
	std::vector<const char*> types{ mname<_Ty, _Members>()... };
	for( size_t i = 0; i < sizeof...( _Members ); i++ )
		printf( "%s %s::%s\n", types[ i ], typeid( _Ty ).name(), _names[ i ] );
		
}

int main()
{
	refl_struct<
		test_struct, 
		&test_struct::member_int,
		&test_struct::member_float
	>( { 
		"member_int",
		"member_float"
	   });

	/*
	test::test_unordered_array();
	test::test_strong_type();
	test::test_reflected_function();
	*/

	return 0;
}

