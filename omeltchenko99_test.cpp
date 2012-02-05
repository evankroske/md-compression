#include <iostream>
#include <cstring>
#include "omeltchenko99.h"
#include "bin.h"

using namespace std;

void e_assert (bool statement, char const *description)
{
	if (statement)
	{
		printf("%s: Passed\n", description);
	}
	else
	{
		printf("%s: Failed\n", description);
	}
}

int main ()
{
	e_assert(false, "test test (should fail)");

	e_assert(make_bitmask<unsigned char>(1, 3) == 14, "make_bitmask test");
	e_assert(msb_bitmask<unsigned char>(4) == 240, "msb_bitmask test");
	e_assert(lsb_bitmask<unsigned char>(5) == 31, "lsb_bitmask test");

	e_assert(str_to_bin<unsigned char>("00000011") == 3, "0b00000011 == 3");
	e_assert(str_to_bin<unsigned char>("11000000") == 192, "0b11000000 == 192");
	e_assert(str_to_bin<unsigned int>("101010101010") == 2730, "0b101010101010 == 2730");

	{
		Coordinate c(0, 1, 2);
		OctreeIndexParams p(2, 2, 2);
		e_assert(octree_index(&c, &p) == str_to_bin<unsigned int>("001010"),
			"test octree_index");
	}
	{
		WriteableBitArray dst;
		BitArray src(7, 3);
		bit_array_append(&dst, &src); 
		e_assert(dst.data[dst.active_byte] == msb_bitmask<unsigned char>(3), 
			"data 1 appended");
	}
	{
		WriteableBitArray dst;
		BitArray src(str_to_bin<int>("110011001010"), 12);
		bit_array_append(&dst, &src);
		e_assert(dst.data[dst.active_byte + 1] == 
			str_to_bin<unsigned char>("11001100"),
			"First byte of data 2");
		e_assert(dst.data[dst.active_byte] ==
			str_to_bin<unsigned char>("10100000"),
			"Second byte of data 2");
	}

	return 0;
}
