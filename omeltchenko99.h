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
int x;
int y;
int z;
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
int L;
int d_L;
int max_L;
int max_d_L;
VarEncodingParams (
	int _l, 
	int _d_l, 
	int _L = 0, 
	int _d_L = 0, 
	int _max_L = 32, 
	int _max_d_L = 32): 
	l(_l), d_l(_d_l), L(_L), d_L(_d_L), max_L(_max_L), max_d_L(_max_d_L) {};
};

unsigned long octree_index (Coordinate &c, OctreeIndexParams &p);
Coordinate un_octree_index (unsigned long octree_index, OctreeIndexParams &p);
void enqueue_to_lsb (int src, int bit_index, unsigned long *dst);
void bit_array_append (WriteableBitArray *a, BitArray *b);
void read_md_data (std::vector<Coordinate> &coordinates, FILE *f);
void print_coordinate (FILE *f, Coordinate &c);
void compute_differences (unsigned long *indexes, int n);
void compute_sums (unsigned long *indexes, int n);
BitArray var_encode_index (unsigned long index, VarEncodingParams &p);
unsigned long var_decode_index (BitArray in, VarEncodingParams &p);
void adjust_var_encoding_params (VarEncodingParams &p);

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

template <typename A, typename B>
B append_bit (A src, B dst, int bit_index)
{
	return (dst << 1) | ((src >> bit_index) & 1);
}

#endif
