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

struct typed_obj {};
struct member_base : public typed_obj { 
	virtual void set( const char* ) = 0; 
};

template<typename _Ty>
struct memvar : public member_base
{
	virtual void set( const char* _str ) override {
		//v = arg::str_to_T<_Ty>( _str );
		printf( "%s\n", _str );
	}
};

template<typename _Ty, auto _Ty::* ..._Members>
struct reflected_struct : typed_obj
{
	reflected_struct( const std::array<const char*, sizeof...( _Members )>& _names ) {
		std::vector<const char*> types{ arg::member_name<_Ty, _Members>()... };
		//std::vector<member_base*> membs{ new memvar<arg::typename_of<decltype( _Members )>::Ty>()... };
		std::vector<arg::typeval> realTypes{ arg::typeval_of<decltype( _Members )>::ty... };
		
		for( size_t i = 0; i < sizeof...( _Members ); i++ )
			printf( "%s %s::%s\n", types[ i ], typeid( _Ty ).name(), _names[ i ] );

	}

	void* create( void* _pBase = nullptr ) { return _create( (_Ty*)_pBase ); }
	void set( const char* _member, const char* _value );

private:
	_Ty* _create( _Ty* _pBase ) {
		if( _pBase )
			return new( _pBase ) _Ty{};
		else
			return new _Ty{};
	}

	_Ty* m_pObj{ nullptr };

};

int test2() { return 0; }

typedef reflected_struct<
		test_struct,
		&test_struct::member_int,
		&test_struct::member_float,
		&test_struct::member_char,
		&test_struct::dothing2> 
	test_struct_refl;
#define test_struct_refl_names { "member_int", "member_float", "member_char", "dothing2" }

int main()
{
	test::test_unordered_array();
	test::test_strong_type();
	test::test_reflected_function();
	test::test_typeval_of();

	printf( " ::---- reflected_struct test ---::\n" );

	std::vector<arg::typeval> tps{
		arg::typeval_of<decltype( &test_struct::dothing2 )>::ty,
		arg::typeval_of<decltype( &test_struct::member_char )>::ty,
		arg::typeval_of<int>::ty,
		arg::typeval_of<decltype( &test2 )>::ty
	};

	test_struct_refl mystruct( test_struct_refl_names );
	
	std::vector<test_struct*> testStructs;

	testStructs.push_back( (test_struct*)mystruct.create() );
	testStructs.push_back( (test_struct*)mystruct.create() );
	testStructs.push_back( (test_struct*)mystruct.create() );

	printf( " ::------------------------------::\n\n" );

	return 0;
}
