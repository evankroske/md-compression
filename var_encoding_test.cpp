#include <iostream>
#include "omeltchenko99.h"

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

unsigned char bin (char const *number)
{
	unsigned char b = 0;
	for (int i = 0; i < 8; i++)
	{
		if (number[i] == '1')
		{
			b |= 1 << (7 - i);
		}
	}
	return b;
}

int main ()
{
	e_assert(false, "test test (should fail)");
	e_assert(msb_bitmask(4) == 240, "msb_bit_mask test");
	e_assert(bin("00000011") == 3, "0b00000011 == 3");
	e_assert(bin("11000000") == 192, "0b11000000 == 192");

	printf("size = %lu\n", sizeof(unsigned long));
	WriteableBitArray dst;
	BitArray src(7, 3);
	bit_array_append(&dst, &src); 
	e_assert(dst.data[dst.active_byte] == msb_bitmask(3), 
		"data 1 appended");

	return 0;
}
