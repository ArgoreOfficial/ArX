#include <stdio.h>
#include "../unordered_array/unordered_array.h"

void constCharTest()
{
	arg::unordered_array<uint16_t, const char*> names;

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

struct sCar
{
	sCar( const char* _strName, int _numWheels, float _weightInKg ) :
		strName{ _strName }, numWheels{ _numWheels }, weightInKg{ _weightInKg }
	{}

	const char* strName;
	int numWheels;
	float weightInKg;
};

int main()
{
	constCharTest();

	arg::unordered_array<uint16_t, sCar> cars;
	uint16_t saabID  = cars.emplace( sCar{ "Saab 95 v4", 4, 946.0f } );
	uint16_t robinID = cars.emplace( "Reliant Robin", 3, 436.0f );
	
	printf( "%s weighs %fkg\n", cars.at( saabID ).strName, cars.at( saabID ).weightInKg );
	printf( "%s weighs %fkg\n", cars.at( robinID ).strName, cars.at( robinID ).weightInKg );

	return 0;
}

