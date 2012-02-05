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

void print_coordinate (Coordinate c);
unsigned int octree_index (Coordinate *c, OctreeIndexParams *p);
void enqueue_to_lsb (int src, int bit_index, unsigned int *dst);
void bit_array_append (WriteableBitArray *a, BitArray *b);

template <typename T>
T str_to_bin (char const *n_str)
{
	T b = 0;
	int n_len = strlen(n_str);
	for (int i = 0; i < n_len; i++)
	{
		if (n_str[i] == '1')
		{
			b |= 1 << n_len - 1 - i;
		}
	}
	return b;
}

template <typename T>
void bin_to_str (char *dst, T n)
{
	for (int i = sizeof(T) * 8 - 1; i >= 0; i--)
	{
		int j = sizeof(T) * 8 - 1 - i;
		if ((n >> i) & 1)
		{
			dst[j] = '1';
		}
		else
		{
			dst[j] = '0';
		}
	}
}

template <typename T>
void puts_bin (T n)
{
	char *str = (char *)malloc(sizeof(T));
	bin_to_str(str, n);
	puts(str);
	free(str);
}

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


