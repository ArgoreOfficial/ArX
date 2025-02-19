#include "test_reflected_function.hpp"

#include <stdio.h>
#include <wv/reflected_function.hpp>

struct testThing
{
	float f;
	int a;
};

float example( int32_t _i, float _f, testThing _ptr )
{
	return static_cast<float>( _i ) + _f;
}

void wv::test_reflected_function()
{
	printf( " ::--- reflected_function test --::\n" );

	wv::reflected_function<example> efunc( "example" );
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
