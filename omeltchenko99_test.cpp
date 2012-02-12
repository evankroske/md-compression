#include <iostream>
#include <cstring>
#include "omeltchenko99.h"
#include "bin.h"

using namespace std;

void e_assert (bool statement, char const *description)
{
	if (statement)
	{
		printf("%-60s Passed\n", description);
	}
	else
	{
		printf("%-60s Failed\n", description);
	}
}

int main ()
{
	e_assert(make_bitmask<unsigned char>(1, 3) == 14, "make_bitmask test");
	e_assert(msb_bitmask<unsigned char>(4) == 240, "msb_bitmask test");
	e_assert(lsb_bitmask<unsigned char>(5) == 31, "lsb_bitmask test");
	e_assert(msb_bitmask<int>(32) == -1, "msb_bitmask negative test 1");
	e_assert(msb_bitmask<int>(31) == -2, "msb_bitmask negative test 2");

	e_assert(str_to_bin<unsigned char>("00000011") == 3, "0b00000011 == 3");
	e_assert(str_to_bin<unsigned char>("11000000") == 192, "0b11000000 == 192");
	e_assert(str_to_bin<unsigned int>("101010101010") == 2730, "0b101010101010 == 2730");
	{
		Coordinate x(3, 4, 6);
		Coordinate y(3, 4, 6);
		Coordinate z(5, 3, 6);
		e_assert(x == y, "Coordinate equality");
		e_assert(x != z, "Coordinate inequality");
	}
	{
		Coordinate c(0, 1, 2);
		OctreeIndexParams p(2, 2, 2);
		e_assert(octree_index(c, p) == str_to_bin<unsigned int>("001010"),
			"test small octree_index");
	}
	{
		Coordinate c(0, -1, 2);
		OctreeIndexParams p(3, 3, 3);
		e_assert(octree_index(c, p) == str_to_bin<unsigned int>("010011010"),
			"Small negative octree_index test");
	}
	{
		Coordinate c(1530854, 570703, 1802708);
		OctreeIndexParams p(21, 21, 21);
		e_assert(octree_index(c, p) == 0x572d7168a7be15f2,
			"test octree_index with big numbers");
	}
	{
		Coordinate c(739195, -699465, -95248);
		OctreeIndexParams p(21, 21, 21);
		e_assert(octree_index(c, p) == 0x3aee38233f77f8b6,
			"test octree_index with big numbers");
	}
	{
		long i = str_to_bin<long>("111111");
		Coordinate c(3, 3, 3);
		OctreeIndexParams p(3, 3, 3);
		e_assert(un_octree_index(i, p) == c, 
			"un_octree_index with small numbers");
	}
	{
		Coordinate c(1, -1, 3);
		long i = str_to_bin<long>("0011111");
		OctreeIndexParams p(2, 2, 3);
		Coordinate d = un_octree_index(i, p);
		e_assert(d == c, 
			"un_octree_index small negative numbers test");
	}
	{
		unsigned long i = 0x3aee38233f77f8b6;
		Coordinate c(739195, -699465, -95248);
		OctreeIndexParams p(21, 21, 21);
		e_assert(un_octree_index(i, p) == c, 
			"un_octree_index with large negative numbers");
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
