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

template<typename _Ty, arg::typeval _Tval = arg::typeval_of<_Ty>::ty>
struct ttest
{
	static const int a = 0;
};

template<typename _Ty>
struct ttest<_Ty, arg::typeval::member_function>
{
	static const int a = 1;
};


template<typename _Ty, auto _Ty::* ..._Members>
struct reflected_struct : typed_obj
{
	reflected_struct( const std::array<const char*, sizeof...( _Members )>& _names ) {
		std::vector<const char*> types{ arg::member_name<_Ty, _Members>()... };
		//std::vector<member_base*> membs{ new memvar<arg::typename_of<decltype( _Members )>::Ty>()... };
		std::vector<arg::typeval> realTypes{ arg::typeval_of<decltype( _Members )>::ty... };
		std::vector<int> realTypes2{ ttest<decltype( _Members )>::a... };

		for( size_t i = 0; i < sizeof...( _Members ); i++ )
			printf( "%s %s::%s\n", types[ i ], typeid( _Ty ).name(), _names[ i ] );

	}

	// void set( const char* _member, const char* _value );
	void* create( void* _pBase ) { _create( _pBase ) }

private:
	_Ty* _create( _Ty* _pBase = nullptr ) {
		if( _pBase )
			return new( _pBase ) _Ty{};
		else
			return new _Ty{};
	}

	_Ty* m_pObj{ nullptr };

};

int test2() { return 0; }

int main()
{
	std::vector<arg::typeval> tps{
		arg::typeval_of<decltype( &test_struct::dothing2 )>::ty,
		arg::typeval_of<decltype( &test_struct::member_char )>::ty,
		arg::typeval_of<int>::ty,
		arg::typeval_of<decltype( &test2 )>::ty
	};

	reflected_struct<
		test_struct, 
		&test_struct::member_int, 
		&test_struct::member_float, 
		&test_struct::member_char,
		&test_struct::dothing2> 
	mystruct( { "member_int", "member_float", "member_char", "dothing2" });
	
	/*
	test::test_unordered_array();
	test::test_strong_type();
	test::test_reflected_function();
	*/

	return 0;
}
