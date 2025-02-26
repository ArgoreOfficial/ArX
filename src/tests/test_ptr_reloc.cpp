#include "test_ptr_reloc.hpp"

#include <stdio.h>
#include <wv/ptr_reloc.hpp>

void fptr_reloc( int _i )
{
	printf( "Relocated function called.\n" );
	printf( "    _i   == %i\n", _i );
	printf( "    fptr == [%p]\n", &fptr_reloc );
}

void wv::test_ptr_reloc()
{
	printf( " ::------- ptr_reloc test -------::\n" );

	wv::ptr_reloc<int> int_ptr{ 0x0 };
	printf( "int at [%p] is %d\n", int_ptr.ptr, *int_ptr );

	wv::ptr_reloc<decltype( fptr_reloc )> fptr{ 0x0 };
	fptr.fptr = &fptr_reloc; // quick hack for testing
	fptr( 4 );

	printf( " ::------------------------------::\n\n" );
}
