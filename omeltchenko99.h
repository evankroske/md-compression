#ifndef OMELTCHENKO99
#define OMELTCHENKO99
#include <cstdlib>
#include <cstdio>
#include <cstring>

struct WriteableBitArray {
const static int size = sizeof(unsigned long);
unsigned char data[size];
int active_byte;
int bits_available;
WriteableBitArray (): data(), active_byte(size - 1), 
	bits_available(8) {};
};

struct BitArray {
unsigned long data;
int bits_used;
BitArray (): data(0), bits_used(0) {};
BitArray (unsigned long _data, int _bits_used): data(_data), bits_used(_bits_used) {};
};

struct Coordinate {
int x;
int y;
int z;
Coordinate(int _x, int _y, int _z): x(_x), y(_y), z(_z) {};
};

struct OctreeIndexParams{
int x_width;
int y_width;
int z_width;
OctreeIndexParams (int _x_width, int _y_width, int _z_width): 
	x_width(_x_width), y_width(_y_width), z_width(_z_width) {};
};

unsigned int octree_index (Coordinate *c, OctreeIndexParams *p);
void enqueue_to_lsb (int src, int bit_index, unsigned int *dst);
void bit_array_append (WriteableBitArray *a, BitArray *b);
void read_md_data (Coordinate *c, FILE *f, int c_size, int *num_read);
void print_coordinate (Coordinate c);

template <typename T>
T make_bitmask (int l, int n)
{
	T m = 0;
	for (int i = 0; i < n; i++)
	{
		m <<= 1;
		m |= 1;
	}
	m <<= l;
	return m;
}

template <typename T>
T lsb_bitmask (int n)
{
	return make_bitmask<T>(0, n);
}

template <typename T>
T msb_bitmask (int n)
{
	return make_bitmask<T>(sizeof(T) * 8 - n, n);
}
#endif
