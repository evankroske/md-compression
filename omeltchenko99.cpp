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

uint octree_index (Coordinate c, OctreeIndexParams *p)
{
	uint octree_index = 0;
	for (int i = sizeof(uint) * 8 - 1; i >= 0; i--)
	{
		if (i <= p->x_width) enqueue_to_lsb(c.x, i, &octree_index);
		if (i <= p->y_width) enqueue_to_lsb(c.y, i, &octree_index);
		if (i <= p->z_width) enqueue_to_lsb(c.z, i, &octree_index);
	}

	return octree_index;
}

/*
void var_encode_uint(uint a, int l, int d_l, char *buf)
{
	int byte_i = 0;
	char tmp = 0;
	for (int i = 0; i < l; i++)
	{
		tmp <<= 1;
		tmp |= a & 1;
		a >>= 1;
		a &= ^(1<<31)
	}
	if (a)
	{
	}
	else
	{
		buf[byte_i] <<= 1 + l;
		buf[byte_i] |= tmp
	}
	printf("%c", buf[byte_i]);
}
*/	

uint lsb_bitmask (int l)
{
	uint mask = 0;
	while (l > 0)
	{
		mask <<= 1;
		mask |= 1;
		l--;
	}
	return mask;
}

void enqueue_to_lsb (uint src, int bit_index, uint *dst)
{
	*dst <<= 1;
	*dst |= (src >> bit_index) & 1;
}

unsigned char msb_bit_mask (int n)
{
	unsigned char m = 0;
	for (int i = 0; i < n; i++)
	{
		m >>= 1;
		m |= 1 << 7;
	}
	return m;
}

// Write b into a
void bit_array_append (WriteableBitArray *a, BitArray *b)
{
	int bits_written = 0;
	while (bits_written != b->bits_used)
	{
		if (b->bits_used - bits_written <= 8)
		{
			
		}
		else
		{
			a->data[active_byte_index] |= 
	}
}
