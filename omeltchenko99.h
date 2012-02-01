typedef unsigned int uint;

struct Coordinate {
uint x;
uint y;
uint z;
Coordinate(uint _x, uint _y, uint _z): x(_x), y(_y), z(_z) {};
};

struct OctreeIndexParams{
int x_width;
int y_width;
int z_width;
OctreeIndexParams (int _x_width, int _y_width, int _z_width): 
	x_width(_x_width), y_width(_y_width), z_width(_z_width) {};
};

const uint MSB = 1 << (sizeof(uint) * 8 - 1);

void print_coordinate (Coordinate c);
uint octree_index (Coordinate c, OctreeIndexParams *p);
void var_encode_uint(uint a, int l, int d_l, char *buf);
uint lsb_bitmask (int l);
void enqueue_to_lsb (uint src, int bit_index, uint *dst);
void bit_array_enqueue (char *a, char b);
unsigned char msb_bit_mask (int n);
