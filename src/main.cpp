#include <stdio.h>

#include "test.hpp"
#include <stdint.h>

#include <typeinfo>
#include <vector>
#include <array>
#include <string>

#include <typename_of.hpp>

struct test_struct
{
	uint16_t member_int;
	float member_float;
	char member_char;

	void dothing2() {}
};

struct memvar_base { virtual void set( const char* ) = 0; };
template<typename _Ty>
struct memvar : public memvar_base
{
	virtual void set( const char* _str ) override {
		v = arg::str_to_T<_Ty>( _str );
		printf( "%s\n", _str );
	}

	_Ty v;
};

template<typename _Ty>
struct reflected_struct
{
	void set( const char* _member, const char* _value );

private:
	_Ty* m_pObj;
};

template<typename _Ty, auto _Ty::* ..._Members>
void refl_struct( const std::array<const char*, sizeof...( _Members )>& _names ) 
{
	std::vector<const char*> types{ arg::member_name<_Ty, _Members>()... };
	//std::vector<memvar_base*> membs{ new memvar<arg::typename_of<decltype( _Members )>::Ty>()... };

	for( size_t i = 0; i < sizeof...( _Members ); i++ )
	{
		printf( "%s %s::%s\n", types[ i ], typeid( _Ty ).name(), _names[ i ] );

		//membs[ i ]->set( "3.3" );
	}
	
}

int main()
{
	refl_struct <
		test_struct,
		&test_struct::member_int,
		&test_struct::member_float,
		&test_struct::member_char,
		&test_struct::dothing2
	>( { 
		"member_int",
		"member_float",
		"member_char",
		"dothing2"
	   });

	/*
	test::test_unordered_array();
	test::test_strong_type();
	test::test_reflected_function();
	*/

	return 0;
}

