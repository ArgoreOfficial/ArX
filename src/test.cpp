#include "test.hpp"

#include <arx/unordered_array.hpp>
#include <arx/strong_type.hpp>
#include <arx/reflected_function.hpp>
#include <arx/typename_of.hpp>
#include <arx/detour.hpp>
#include <arx/ptr_reloc.hpp>

#include <stdio.h>

struct abstract_car
{
	const char* strName;
	int numWheels;
	float weightInKg;
};

void arxTest::test_unordered_array()
{
	printf( " ::---- unordered_array test ----::\n" );
	
	{
		arx::unordered_array<uint16_t, const char*> names;

		printf( "size:   %zu\ncount:  %zu\n\n", names.size(), names.count() );

		uint16_t name1 = names.emplace( "car" );         // 1
		uint16_t name2 = names.emplace( "sail" "boat" ); // 2
		uint16_t name3 = names.emplace( "plane" );       // 3

		printf( "size:   %zu\ncount:  %zu\n\n", names.size(), names.count() );

		const char* name1str = names.at( name1 );      // "car"
		const char* name2str = names.at( name2 );      // "sailboat"
		const char* name3str = names.at( name3 );      // "plane"

		names.erase( name1 );                          // erase "car"
		name1 = 0;                                     // make sure to invalidate

		printf( "size:   %zu\ncount:  %zu\n\n", names.size(), names.count() );

		uint16_t name5 = names.emplace( "bicycle" );     // 1
		const char* name5str = names.at( name5 );     // "bicycle"

		printf( "size:   %zu\ncount:  %zu\n\n", names.size(), names.count() );

		names.clear();

		printf( "size:   %zu\ncount:  %zu\n\n", names.size(), names.count() );
	}

	arx::unordered_array<uint16_t, abstract_car> cars;
	uint16_t saabID  = cars.emplace( "Saab 95 v4", 4, 946.0f );
	uint16_t robinID = cars.emplace( "Reliant Robin", 3, 436.0f );

	printf( "%s weighs %fkg\n", cars.at( saabID ).strName, cars.at( saabID ).weightInKg );
	printf( "%s weighs %fkg\n", cars[ robinID ].strName, cars[ robinID ].weightInKg );

	printf( " ::------------------------------::\n\n" );
}

void arxTest::test_array_view()
{

}

typedef arx::strong_type<uint16_t, struct handle1_kt> handle1_t;
typedef arx::strong_type<uint16_t, struct handle2_kt> handle2_t;

static uint16_t get_strong_value( handle1_t _t ) { 
	return _t.value; 
}

void arxTest::test_strong_type()
{
	printf( " ::------ strong_type test ------::\n" );
	
	handle1_t someHandle1{ 1 };
	handle2_t someHandle2{ 2 };
	
	printf( "handle1_t: %i\n", get_strong_value( someHandle1 ) );
	// printf( "%i", get_strong_value( someHandle2 ) ); // strong type mismatch, compiler error
	// uint16_t u16 = someHandle1; // compiler error

	printf( " ::------------------------------::\n\n" );
}

struct testThing
{
	float f;
	int a;
};

float example( int32_t _i, float _f, testThing _ptr )
{
	return static_cast<float>( _i ) + _f;
}

void arxTest::test_reflected_function()
{
	printf( " ::--- reflected_function test --::\n" );

	arx::reflected_function<example> efunc( "example" );
	printf( "symbol: %s\n", efunc.symbol() );
	printf( "%zu parameters\n", efunc.arg_count() );
	printf( "name:  %s %s(", efunc.ret_name(), efunc.name() );

	auto names = efunc.arg_names();
	for( size_t i = 0; i < names.size(); i++ )
	{
		if( i != 0 )
			printf( ", " );
		printf( "%s", names[ i ] );
	}
	printf( ")\n" );

	printf( "%f\n", efunc( 3, 0.141592, testThing{} ) );

	printf( " ::------------------------------::\n\n" );
}


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

void fptr_reloc( int _i )
{
	printf( "Relocated function called.\n" );
	printf( "    _i   == %i\n", _i );
	printf( "    fptr == [%p]\n", &fptr_reloc );
}

void arxTest::test_ptr_reloc()
{
	printf( " ::------- ptr_reloc test -------::\n" );
	
	arx::ptr_reloc<int> int_ptr{ 0x0 };
	printf( "int at [%p] is %d\n", int_ptr.ptr, *int_ptr );

	arx::ptr_reloc<decltype( fptr_reloc )> fptr{ 0x0 };
	fptr.fptr = &fptr_reloc; // quick hack for testing
	fptr( 4 );

	printf( " ::------------------------------::\n\n" );
}
