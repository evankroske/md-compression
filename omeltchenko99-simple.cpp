#include "omeltchenko99-simple.h"
#include "omeltchenko99.h"

using namespace std;

static const VarEncodingParams DEFAULT_VAR_ENCODING_PARAMS(40, 10);
static const OctreeIndexParams DEFAULT_OCTREE_INDEX_PARAMS(21, 21, 21);

void list_write_bit_array (deque<char> out, WriteableBitArray *b);
void list_read_bit_array (deque<char> in, ReadableBitArray *b);

void compress (list<char> &compressed, vector<Coordinate> &coordinates)
{
}

void uncompress (list<Coordinate> &coordinates, list<char> &compressed)
{
	
}

void varlength_encode (list<char> &compressed, list<RTreeIndex> &indexes)
{
}

void varlength_decode (list<RTreeIndex> &indexes, list<char> &compressed, int num_compressed)
{
}

RTreeIndex get_rtree_index(Coordinate coordinate)
{
	return octree_index(coordinate, DEFAULT_OCTREE_INDEX_PARAMS);
}

Coordinate get_coordinate(RTreeIndex index)
{
	return un_octree_index((OctreeIndex)index, DEFAULT_OCTREE_INDEX_PARAMS);
}

void list_write_bit_array (deque<char> out, WriteableBitArray *in)
{
	assert(in->active_byte < in->size);
	if (in->active_byte > 0)
	{
		// fwrite(in->data, sizeof(char), in->active_byte, out);
		for (int i = 0; i < in->active_byte; i++)
		{
			out.push_back(b->data[i]);
		}
		unsigned char *tmp = new unsigned char[in->size]();
		memcpy(tmp, in->data + in->active_byte, in->size - in->active_byte);
		memcpy(in->data, tmp, in->size);
		in->active_byte = 0;
		delete[] tmp;
	}
	else
	{
		fwrite(in->data, sizeof(char), 1, out);
		in->data[0] = 0x0;
		in->bits_available = 8;
	}
}

void list_read_bit_array (deque<char> in, ReadableBitArray *out)
{
	if (out->active_byte > 0)
	{
		unsigned char *tmp = new unsigned char[out->size]();
		int bytes_full = out->size - out->active_byte;
		//fread(tmp + bytes_full, sizeof(char), out->active_byte, in);
		memcpy(tmp, out->data + out->active_byte, bytes_full);
		memcpy(out->data, tmp, out->size);
		out->active_byte = 0;
		delete [] tmp;
	}
}
