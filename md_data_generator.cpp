#include <iostream>
#include <ctime>
#include "omeltchenko99.h"

const int MAX_BITS = 21;
int main (int argc, char **argv)
{
	int num_coordinates = 100;
	if (argc > 1) {
		puts(argv[1]);
		sscanf(argv[1], "%d", &num_coordinates);
	}
	int max_bits = MAX_BITS;

	srand(time(NULL));
	for (int i = 0; i < num_coordinates; i++)
	{
		printf("%u\t%u\t%u\n", rand() & lsb_bitmask<int>(max_bits), rand() & lsb_bitmask<int>(max_bits), rand() & lsb_bitmask<int>(max_bits));
	}

	return 0;
}
