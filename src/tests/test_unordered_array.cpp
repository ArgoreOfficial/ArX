#include "test_unordered_array.hpp"

#include <stdio.h>
#include <wv/unordered_array.hpp>

struct abstract_car
{
	const char* strName;
	int numWheels;
	float weightInKg;
};

void wv::test_unordered_array()
{
	printf( " ::---- unordered_array test ----::\n" );

	{
		wv::unordered_array<uint16_t, const char*> names;

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

	wv::unordered_array<uint16_t, abstract_car> cars;
	uint16_t saabID  = cars.emplace( "Saab 95 v4",    4, 946.0f );
	uint16_t robinID = cars.emplace( "Reliant Robin", 3, 436.0f );

	printf( "cars does%s contain '%i'\n", cars.contains( saabID ) ? "" : " not", (int)saabID );
	printf( "cars does%s contain '%i'\n\n", cars.contains( 5 ) ? "" : " not", 5 );

	printf( "%s weighs %fkg\n", cars.at( saabID ).strName, cars.at( saabID ).weightInKg );
	printf( "%s weighs %fkg\n", cars[ robinID ].strName, cars[ robinID ].weightInKg );

	printf( " ::------------------------------::\n\n" );
}
