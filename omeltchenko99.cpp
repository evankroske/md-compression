#include <iostream>
#include <cstring>
#include "omeltchenko99.h"

using namespace std;

typedef unsigned int uint;

const uint MSB = 1 << (sizeof(uint) * 8 - 1);

void print_coordinate (Coordinate c)
{
	printf("Coordinate(x=%d, y=%d, z=%d)\n", c.x, c.y, c.z);
}

uint octree_index (Coordinate *c, OctreeIndexParams *p)
{
	uint octree_index = 0;
	for (int i = sizeof(uint) * 8 - 1; i >= 0; i--)
	{
		if (i <= p->x_width) enqueue_to_lsb(c->x, i, &octree_index);
		if (i <= p->y_width) enqueue_to_lsb(c->y, i, &octree_index);
		if (i <= p->z_width) enqueue_to_lsb(c->z, i, &octree_index);
	}

	return octree_index;
}

void enqueue_to_lsb (int src, int bit_index, uint *dst)
{
	*dst <<= 1;
	*dst |= (src >> bit_index) & 1;
}

// Write bits from b into a
void bit_array_append (WriteableBitArray *a, BitArray *b)
{
	int bits_written = 0;
	int bits_not_written = b->bits_used;
	int bits_last_written = 0;
	while (bits_written != b->bits_used)
	{
		// If there's less than a byte of data left to write
		if (bits_not_written < a->bits_available)
		{
			/*
			char *b_data_shifted = bin_to_str(b->data << 1);
			puts(b_data_shifted);
			free(b_data_shifted);
			b_data_shifted = NULL;
			*/
			a->data[a->active_byte] |= (b->data << a->bits_available - 
				bits_not_written);
			bits_last_written = bits_not_written;
			// Reduce bits_available but don't decrement active_byte
			a->bits_available -= bits_last_written;
		}
		else
		{
			a->data[a->active_byte] |= (b->data >> bits_not_written - 
				a->bits_available) & lsb_bitmask<unsigned char>(
				a->bits_available);
			bits_last_written = a->bits_available;
			// Reset bits_available and decrement byte_index
			a->bits_available = 8;
			a->active_byte -= 1;
		}
		bits_written += bits_last_written;
		bits_not_written -= bits_last_written;
	}
}
