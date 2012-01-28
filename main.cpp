#include <iostream>

using namespace std;

typedef unsigned int uint;
typedef struct {
uint x;
uint y;
uint z;
} Coordinate;

const uint MSB = 1 << (sizeof(uint) * 8 - 1);

void print_coordinate (Coordinate c);
uint octree_index (Coordinate c);
uint top_bit (uint i);
void transfer_msb (uint *a, uint *b);
void var_encode_uint(uint a, int l, int d_l, char *bu);
uint lsb_bitmask (int l);

int main ()
{
	Coordinate c;
	c.x = 2;
	c.y = 1;
	c.z = 1;
	print_coordinate(c);
	return 0;
}

void print_coordinate (Coordinate c)
{
	printf("Coordinate(x=%d, y=%d, z=%d)\n", c.x, c.y, c.z);
}

uint octree_index (Coordinate c)
{
	uint i = 0;
	while (c.x || c.y || c.z)
	{
		transfer_msb(&(c.x), &i);
		transfer_msb(&(c.y), &i);
		transfer_msb(&(c.z), &i);
	}
	return i;
}

uint top_bit (uint i)
{
	return (i & MSB) ? 1 : 0;
}

void transfer_msb (uint *a, uint *b)
{
	// printf("a = %u; b = %u\n", *a, *b);
	*b <<= 1;
	*b |= top_bit(*a);
	*a <<= 1;
}

void var_encode_uint(uint a, int l, int d_l, char *bu)
{
	
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
