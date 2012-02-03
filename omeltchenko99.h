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
Coordinate(unsigned int _x, unsigned int _y, unsigned int _z): x(_x), y(_y), z(_z) {};
};

struct OctreeIndexParams{
int x_width;
int y_width;
int z_width;
OctreeIndexParams (int _x_width, int _y_width, int _z_width): 
	x_width(_x_width), y_width(_y_width), z_width(_z_width) {};
};

void print_coordinate (Coordinate c);
unsigned int octree_index (Coordinate c, OctreeIndexParams *p);
void enqueue_to_lsb (unsigned int src, int bit_index, unsigned int *dst);
void bit_array_append (WriteableBitArray *a, BitArray *b);
unsigned long make_bitmask (int l, int n);
unsigned long msb_bitmask (int n);
unsigned long lsb_bitmask (int n);
