struct WriteableBitArray {
const static int size = sizeof(unsigned long) + 1;
unsigned char data[size];
int active_byte_index;
int active_bit_index;
WriteableBitArray (): data(), active_byte_index(size - 1), 
	active_bit_index(7) {};
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
unsigned int lsb_bitmask (int l);
void enqueue_to_lsb (unsigned int src, int bit_index, unsigned int *dst);
void bit_array_append (WriteableBitArray *a, BitArray *b);
unsigned char msb_bit_mask (int n);
