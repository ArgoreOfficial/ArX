#include <stdio.h>
#include "../id_array/id_array.h"

namespace wv
{
	typedef uint16_t ID;
}

void constCharTest()
{
	wv::id_array<wv::ID, const char*> names;

	printf( "size:   %zu\ncount:  %zu\n\n", names.size(), names.count() );

	wv::ID name1 = names.emplace( "car" );         // 1
	wv::ID name2 = names.emplace( "sail" "boat" ); // 2
	wv::ID name3 = names.emplace( "plane" );       // 3
	
	printf( "size:   %zu\ncount:  %zu\n\n", names.size(), names.count() );

	const char* name1str = names.get( name1 );     // "car"
	const char* name2str = names.get( name2 );     // "sailboat"
	const char* name3str = names.get( name3 );     // "plane"
	
	names.erase( name1 );                          // erase "car"
	name1 = 0;                                     // make sure to invalidate

	printf( "size:   %zu\ncount:  %zu\n\n", names.size(), names.count() );

	wv::ID name5 = names.emplace( "bicycle" );     // 1
	const char* name5str = names.get( name5 );     // "bicycle"

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

	wv::id_array<wv::ID, sCar> cars;
	wv::ID saabID  = cars.emplace( sCar{ "Saab 95 v4", 4, 946.0f } );
	wv::ID robinID = cars.emplace( "Reliant Robin", 3, 436.0f );
	
	printf( "%s weighs %fkg\n", cars.get( saabID ).strName, cars.get( saabID ).weightInKg );
	printf( "%s weighs %fkg\n", cars.get( robinID ).strName, cars.get( robinID ).weightInKg );

	return 0;
}

