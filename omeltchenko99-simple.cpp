#include "omeltchenko99-simple.h"
#include "omeltchenko99.h"
#include <cassert>
#include <algorithm>

using namespace std;

static const VarEncodingParams DEFAULT_VAR_ENCODING_PARAMS(60, 2);
static const OctreeIndexParams DEFAULT_OCTREE_INDEX_PARAMS(21, 21, 21);

void list_write_bit_array (list<unsigned char> &out, WriteableBitArray *b);
void list_read_bit_array (list<unsigned char> &in, ReadableBitArray *b);

void compress (list<unsigned char> &compressed, list<Coordinate> &coordinates)
{
	vector<RTreeIndex> indexes;
	for (list<Coordinate>::iterator i = coordinates.begin();
		i != coordinates.end();
		i++)
	{
		indexes.push_back(get_rtree_index(*i));
	}
	sort(indexes.begin(), indexes.end());
	compute_differences(&indexes[0], indexes.size());
	varlength_encode(compressed, indexes);
}

void uncompress (list<Coordinate> &coordinates, list<unsigned char> &compressed, int num_compressed)
{
	vector<RTreeIndex> indexes;
	varlength_decode(indexes, compressed, num_compressed);
	compute_sums(&indexes[0], indexes.size());
	for (vector<RTreeIndex>::iterator i = indexes.begin();
		i != indexes.end();
		i++)
	{
		coordinates.push_back(get_coordinate(*i));
	}
}

void varlength_encode (list<unsigned char> &compressed, vector<RTreeIndex> &index_diffs)
{
	VarEncodingParams v(DEFAULT_VAR_ENCODING_PARAMS);
	BitArray encoded_index;
	WriteableBitArray b;
	for (vector<RTreeIndex>::iterator i = index_diffs.begin();
		i != index_diffs.end();
		i++)
	{
		encoded_index = var_encode_index(*i, v);
		bit_array_append(&b, encoded_index);
		list_write_bit_array(compressed, &b);
	}
	list_write_bit_array(compressed, &b);
}

void varlength_decode (vector<RTreeIndex> &index_diffs, list<unsigned char> &compressed, int num_compressed)
{
	VarEncodingParams v(DEFAULT_VAR_ENCODING_PARAMS);
	ReadableBitArray b;
	for (int i = 0; i < num_compressed; i++)
	{
		list_read_bit_array(compressed, &b);
		index_diffs.push_back(var_decode_index(&b, v));
	}
}

RTreeIndex get_rtree_index(Coordinate coordinate)
{
	return octree_index(coordinate, DEFAULT_OCTREE_INDEX_PARAMS);
}

Coordinate get_coordinate(RTreeIndex index)
{
	return un_octree_index((OctreeIndex)index, DEFAULT_OCTREE_INDEX_PARAMS);
}

void list_write_bit_array (list<unsigned char> &out, WriteableBitArray *in)
{
	assert(in->active_byte < in->size);
	if (in->active_byte > 0)
	{
		// fwrite(in->data, sizeof(char), in->active_byte, out);
		for (int i = 0; i < in->active_byte; i++)
		{
			out.push_back(in->data[i]);
		}
		unsigned char *tmp = new unsigned char[in->size]();
		memcpy(tmp, in->data + in->active_byte, in->size - in->active_byte);
		memcpy(in->data, tmp, in->size);
		in->active_byte = 0;
		delete[] tmp;
	}
	else
	{
		// fwrite(in->data, sizeof(char), 1, out);
		out.push_back(in->data[0]);
		in->data[0] = 0x0;
		in->bits_available = 8;
	}
}

void list_read_bit_array (list<unsigned char> &in, ReadableBitArray *out)
{
	if (out->active_byte > 0)
	{
		unsigned char tmp[9];
		int bytes_full = out->size - out->active_byte;
		//fread(tmp + bytes_full, sizeof(char), out->active_byte, in);
		for (int i = 0; i < out->active_byte && in.size() > 0; i++)
		{
			tmp[i + bytes_full] = in.front();
			in.pop_front();
		}
		memcpy(tmp, out->data + out->active_byte, bytes_full);
		memcpy(out->data, tmp, out->size);
		out->active_byte = 0;
	}
}
