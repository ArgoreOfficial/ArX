#include "test_strong_type.hpp"

#include <stdio.h>
#include <arx/strong_type.hpp>


typedef wv::strong_type<uint16_t, struct handle1_kt> handle1_t;
typedef wv::strong_type<uint16_t, struct handle2_kt> handle2_t;

static uint16_t get_strong_value( handle1_t _t ) {
	return _t.value;
}

void wv::test_strong_type()
{
	printf( " ::------ strong_type test ------::\n" );

	handle1_t someHandle1{ 1 };
	handle2_t someHandle2{ 2 };

	printf( "handle1_t: %i\n", get_strong_value( someHandle1 ) );
	// printf( "%i", get_strong_value( someHandle2 ) ); // strong type mismatch, compiler error
	// uint16_t u16 = someHandle1; // compiler error

	printf( " ::------------------------------::\n\n" );
}
