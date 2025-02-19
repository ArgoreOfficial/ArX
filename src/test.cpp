#include "test.hpp"

#include <wv/unordered_array.hpp>
#include <wv/strong_type.hpp>
#include <wv/reflected_function.hpp>
#include <wv/typename_of.hpp>
#include <wv/detour.hpp>
#include <wv/ptr_reloc.hpp>

#include <stdio.h>

void type_test_func() {}

struct type_test_struct
{
	void type_test_mfunc() {}
	int type_test_mint;
};

const char* typeval2str( wv::typeval _tval ) {

	switch( _tval )
	{
	case wv::type:            return "type      "; break;
	case wv::type_pointer:    return "type*     "; break;
	case wv::function:        return "function  "; break;
	case wv::member_variable: return "m_variable"; break;
	case wv::member_function: return "m_function"; break;
	}

	return "unknown";
}

template<typename _Ty, wv::typeval _Tval = wv::typeval_of<_Ty>::ty>
struct type_test
{
	static void print() {
		printf( "%s :-: %s\n", typeval2str( _Tval ), typeid( _Ty ).name() );
	}
};

void wv::test_typeval_of()
{
	printf( " ::------- typeval_of test ------::\n" );
	int test_int = 0;

	type_test<int>::print();
	type_test<decltype( &type_test_func )>::print();
	type_test<decltype( &test_int )>::print();

	type_test<type_test_struct>::print();
	type_test<decltype( &type_test_struct::type_test_mint )>::print();
	type_test<decltype( &type_test_struct::type_test_mfunc )>::print();
	printf( " ::------------------------------::\n\n" );
}
