#include <stdio.h>

#include "test.hpp"
#include <stdint.h>

int main()
{

	test::test_unordered_array();
	test::test_strong_type();
	test::test_reflected_function();

	return 0;
}

