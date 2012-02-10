#ifndef OMELTCHENKO99
#define OMELTCHENKO99
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <vector>

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
unsigned int x;
unsigned int y;
unsigned int z;
Coordinate (): x(0), y(0), z(0) {};
Coordinate(int _x, int _y, int _z): x(_x), y(_y), z(_z) {};
bool operator== (Coordinate const &other)
{
	return x == other.x && y == other.y && z == other.z;
};
bool operator!= (Coordinate const &other)
{
	return !(*this == other);
};
};

struct OctreeIndexParams{
int x_width;
int y_width;
int z_width;
OctreeIndexParams (int _x_width, int _y_width, int _z_width): 
	x_width(_x_width), y_width(_y_width), z_width(_z_width) {};
};

struct VarEncodingParams {
int l;
int d_l;
VarEncodingParams (int _l, int _d_l): l(_l), d_l(_d_l) {};
};

long long octree_index (Coordinate &c, OctreeIndexParams &p);
Coordinate un_octree_index (long long octree_index, OctreeIndexParams &p);
void enqueue_to_lsb (int src, int bit_index, long long *dst);
void bit_array_append (WriteableBitArray *a, BitArray *b);
void read_md_data (std::vector<Coordinate> &coordinates, FILE *f);
void print_coordinate (FILE *f, Coordinate &c);

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
