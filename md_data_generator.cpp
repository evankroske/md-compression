#include <iostream>
#include <ctime>
#include "omeltchenko99.h"

const int MAX_BITS = 8;

int signed_rand(int bits)
{
	unsigned int base = rand();
	if ((base >> (bits - 1)) & 1)
	{
		return base | msb_bitmask<unsigned int>(sizeof(int) * 8 - bits);
	}
	else 
	{
		return base & lsb_bitmask<unsigned int>(bits);
	}
}

int main (int argc, char **argv)
{
	int num_coordinates = 100;
	if (argc > 1) {
		puts(argv[1]);
		sscanf(argv[1], "%d", &num_coordinates);
	}
	int max_bits = MAX_BITS;
	int bits_x = max_bits;
	int bits_y = max_bits;
	int bits_z = max_bits;

	srand(time(NULL));
	for (int i = 0; i < num_coordinates; i++)
	{
		Coordinate c(signed_rand(bits_x), signed_rand(bits_y), 
			signed_rand(bits_z));
		print_coordinate(stdout, c);
	}

	return 0;
}
