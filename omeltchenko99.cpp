#include "omeltchenko99.h"

#define COPY_BIT_AND_INC(dst, src, dst_i, src_i) dst |= ((src >> src_i++) & 1) << dst_i++
using namespace std;

typedef unsigned int uint;

const uint MSB = 1 << (sizeof(uint) * 8 - 1);

long long octree_index (Coordinate &c, OctreeIndexParams &p)
{
	long long octree_index = 0;
	for (int i = sizeof(long long) * 8 - 1; i >= 0; i--)
	{
		if (i < p.x_width) enqueue_to_lsb(c.x, i, &octree_index);
		if (i < p.y_width) enqueue_to_lsb(c.y, i, &octree_index);
		if (i < p.z_width) enqueue_to_lsb(c.z, i, &octree_index);
	}

	return octree_index;
}

Coordinate un_octree_index (long long octree_index, OctreeIndexParams &p)
{
	Coordinate c;
	int index_bits_used = p.x_width + p.y_width + p.z_width;
	for (int i = 0, x_i = 0, y_i = 0, z_i = 0; i < index_bits_used; )
	{
		// If there are bits of x left in the octree index, copy the least-
		// significant uncopied bit from octree_index to the least-significant 
		// unfilled bit of c.x
		if (x_i < p.x_width) COPY_BIT_AND_INC(c.x, octree_index, x_i, i);
		if (y_i < p.y_width) COPY_BIT_AND_INC(c.y, octree_index, y_i, i);
		if (z_i < p.z_width) COPY_BIT_AND_INC(c.z, octree_index, z_i, i);
	}
	return c;
}

void enqueue_to_lsb (int src, int bit_index, long long *dst)
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

void read_md_data (vector<Coordinate> &coordinates, FILE *f)
{
	while (true)
	{
		int x, y, z;
		fscanf(f, "%d %d %d", &x, &y, &z);
		if (feof(f))
		{
			break;
		}
		Coordinate c(x, y, z);
		coordinates.push_back(c);
	}
}

void print_coordinate (Coordinate c)
{
	printf("Coordinate(x=%u, y=%u, z=%u)\n", c.x, c.y, c.z);
}

