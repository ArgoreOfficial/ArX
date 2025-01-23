#include "test.hpp"

#include <arx/unordered_array.hpp>
#include <arx/strong_type.hpp>
#include <arx/reflected_function.hpp>
#include <arx/typename_of.hpp>
#include <arx/detour.hpp>
#include <arx/ptr_reloc.hpp>

#include <stdio.h>

void type_test_func() {}

struct type_test_struct
{
	void type_test_mfunc() {}
	int type_test_mint;
};

const char* typeval2str( arx::typeval _tval ) {

	switch( _tval )
	{
	case arx::type:            return "type      "; break;
	case arx::type_pointer:    return "type*     "; break;
	case arx::function:        return "function  "; break;
	case arx::member_variable: return "m_variable"; break;
	case arx::member_function: return "m_function"; break;
	}

	return "unknown";
}

template<typename _Ty, arx::typeval _Tval = arx::typeval_of<_Ty>::ty>
struct type_test
{
	static void print() {
		printf( "%s :-: %s\n", typeval2str( _Tval ), typeid( _Ty ).name() );
	}
};

void arxTest::test_typeval_of()
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
