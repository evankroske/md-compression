#include <iostream>
#include "omeltchenko99.h"

using namespace std;

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

void var_encode_uint(uint a, int l, int d_l, char *buf)
{
/*
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
*/	
}

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

// Enqueue n least-significant bits of b into bit array a
void bit_array_enqueue (char *a, int a_n, char b, int b_n)
{
	char c[100];
	for (int i = 0; i < a_n; i++)
	{
	}
		
}