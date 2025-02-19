#include "test_registry.hpp"

#include <stdio.h>
#include <wv/registry.hpp>

class base_class : public wv::registry<base_class>
{
public:
	int base = 0;
};

struct child_data
{
	int value = 0;
};

class child_class : wv::registar<child_class, base_class>
{
public:
	static std::string get_name() { return "some_child"; }
	static base_class* alloc( void* _pUserData ) {
		child_data* userData = reinterpret_cast<child_data*>( _pUserData );

		printf( "allocated child_class with value: %i\n", userData->value );
		return new child_class{};
	}

	float deriv = 2.0f;
};

void wv::test_registry()
{
	printf( " ::-------- registry test -------::\n" );

	child_data data;
	data.value = 6;
	base_class* reg = base_class::alloc( "some_child", &data );

	printf( " ::------------------------------::\n\n" );
}
