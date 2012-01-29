#include <iostream>

using namespace std;

typedef unsigned int uint;
typedef struct {
uint x;
uint y;
uint z;
} Coordinate;

typedef struct {
int x_width;
int y_width;
int z_width;
} OctreeIndexParams;

const uint MSB = 1 << (sizeof(uint) * 8 - 1);
FILE *log_file;

void print_coordinate (Coordinate c);
uint octree_index (Coordinate c, OctreeIndexParams *p);
uint top_bit (uint i);
void transfer_msb (uint *a, uint *b);
void var_encode_uint(uint a, int l, int d_l, char *bu);
uint lsb_bitmask (int l);
void enqueue_to_lsb (uint src, uint bitmask, uint *dst);

int main ()
{
	log_file = fopen("log.txt", "w");

	Coordinate c;
	c.x = 2;
	c.y = 1;
	c.z = 1;
	
	OctreeIndexParams p;
	p.x_width = 2;
	p.y_width = 2;
	p.z_width = 2;

	return 0;
}

void print_coordinate (Coordinate c)
{
	printf("Coordinate(x=%d, y=%d, z=%d)\n", c.x, c.y, c.z);
}

uint octree_index (Coordinate c, OctreeIndexParams *p)
{
	uint octree_index = 0;
	int i = 32;
	uint m = 1 << (sizeof(uint) * 8 - 1);
	for (; i != 0 && m != 0; i--, m >>= 1)
	{
		if (i <= p->x_width) enqueue_to_lsb(c.x, m, &octree_index);
		if (i <= p->y_width) enqueue_to_lsb(c.y, m, &octree_index);
		if (i <= p->z_width) enqueue_to_lsb(c.z, m, &octree_index);
	}

	return octree_index;
}

uint top_bit (uint i)
{
	return (i & MSB) ? 1 : 0;
}

void transfer_msb (uint *a, uint *b)
{
	fprintf(log_file, "transfer_msb(a = %u; b = %u)\n", *a, *b);
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

void enqueue_to_lsb (uint src, uint bitmask, uint *dst)
{
	fprintf(log_file, "enqueue_to_lsb(src = %d, bitmask = %u, dst = %u)\n", src, bitmask, *dst);
	*dst <<= 1;
	*dst |= (src & bitmask) ? 1 : 0;
}
