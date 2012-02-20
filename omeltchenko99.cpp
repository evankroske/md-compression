#include "omeltchenko99.h"
#include "bin.h"

#define COPY_BIT_AND_INC(dst, src, dst_i, src_i) dst |= ((src >> src_i++) & 1) << dst_i++
#define SIGN_EXTEND(x, sig_bits) x |= msb_bitmask<unsigned int>(sizeof(unsigned int) * 8 - sig_bits)
#define NEXT_BIT(a) ((a->data[a->active_byte] >> a->bits_not_read - 1) & 1)
#define DROP_BIT(a) if (--a->bits_not_read == 0) { (a->active_byte)++; a->bits_not_read = 8;}

using namespace std;

typedef unsigned int uint;

const uint MSB = 1 << (sizeof(uint) * 8 - 1);

int count_trailing_zeros (unsigned long a, int max);
int count_leading_zeros (unsigned int index, int bit_index, int max);

unsigned long octree_index (Coordinate &c, OctreeIndexParams &p)
{
	unsigned long octree_index = 0;
	for (int i = sizeof(unsigned long) * 8 - 1; i >= 0; i--)
	{
		if (i < p.x_width) octree_index = append_bit(c.x, octree_index, i);
		if (i < p.y_width) octree_index = append_bit(c.y, octree_index, i);
		if (i < p.z_width) octree_index = append_bit(c.z, octree_index, i);
	}

	return octree_index;
}

Coordinate un_octree_index (unsigned long octree_index, OctreeIndexParams &p)
{
	Coordinate c;
	int index_bits_used = p.x_width + p.y_width + p.z_width;
	for (int i = 0, x_i = 0, y_i = 0, z_i = 0; i < index_bits_used; )
	{
		// If there are bits of x left in the octree index, copy the least-
		// significant uncopied bit from octree_index to the least-significant 
		// unfilled bit of c.x
		if (z_i < p.z_width) COPY_BIT_AND_INC(c.z, octree_index, z_i, i);
		if (y_i < p.y_width) COPY_BIT_AND_INC(c.y, octree_index, y_i, i);
		if (x_i < p.x_width) COPY_BIT_AND_INC(c.x, octree_index, x_i, i);
	}
	// If the MSB of x is 1, pad x with 1s to interpret it as negative
	if ((c.x >> (p.x_width - 1)) & 1) SIGN_EXTEND(c.x, p.x_width);
	if ((c.y >> (p.y_width - 1)) & 1) SIGN_EXTEND(c.y, p.y_width);
	if ((c.z >> (p.z_width - 1)) & 1) SIGN_EXTEND(c.z, p.z_width);
	return c;
}

// Write used bits from index into a
void bit_array_append (WriteableBitArray *a, BitArray b)
{
	OctreeIndex index = b.data;
	int bits_used = b.bits_used;
	int bits_written = 0;
	int bits_not_written = bits_used;
	int bits_last_written = 0;
	while (bits_written < bits_used)
	{
		// If there's less than a byte of data left to write
		if (bits_not_written < a->bits_available)
		{
			a->data[a->active_byte] |= (index << a->bits_available - 
				bits_not_written);
			bits_last_written = bits_not_written;
			// Reduce bits_available but don't decrement active_byte
			a->bits_available -= bits_last_written;
		}
		else
		{
			a->data[a->active_byte] |= (index >> bits_not_written - 
				a->bits_available) & lsb_bitmask<unsigned char>(
				a->bits_available);
			bits_last_written = a->bits_available;
			// Reset bits_available and decrement byte_index
			a->bits_available = 8;
			a->active_byte++;
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

void print_coordinate (FILE *f, Coordinate &c)
{
	fprintf(f, "%10d %10d %10d\n", c.x, c.y, c.z);
}

void compute_differences (unsigned long *indexes, int n)
{
	for (int i = n - 1; i >= 1; i--)
	{
		indexes[i] -= indexes[i - 1];
	}
}

void compute_sums (unsigned long *indexes, int n)
{
	for (int i = 1; i < n; i++)
	{
		indexes[i] += indexes[i - 1];
	}
}

void transfer_bits (BitArray *out, unsigned long *index, int num_bits)
{
	for (int i = 0; i < num_bits; i++)
	{
		out->data <<= 1;
		out->data |= *index & 1;
		*index >>= 1;
	}
	out->bits_used += num_bits;
}

int count_trailing_zeros (unsigned long a, int max)
{
	int unused_bits;
	for (unused_bits = 0; 
		unused_bits < max && !((a >> unused_bits) & 1); 
		unused_bits++);
	return unused_bits;
}

BitArray var_encode_index (unsigned long index, VarEncodingParams &p)
{
	BitArray out;
	transfer_bits(&out, &index, p.l);
	if (index)
	{
		out.data |= 1 << p.l;
	}
	else
	{
		p.L -= count_trailing_zeros(out.data, p.l);
	}
	out.bits_used += 1;

	while (index)
	{
		out.data <<= 1;
		transfer_bits(&out, &index, p.d_l);
		if (index)
		{
			out.data |= 1 << p.d_l;
			p.d_L++;
		}
		else
		{
			p.d_L -= count_trailing_zeros(out.data, p.d_l);
		}
		p.L++;
	}
	adjust_var_encoding_params (p);
	return out;
}

unsigned long var_decode_index (ReadableBitArray *in, VarEncodingParams &p)
{
	unsigned long index = 0;
	int bit_index = 0;
	int more = NEXT_BIT(in);
	DROP_BIT(in);
	for (int i = 0; i < p.l; i++)
	{
		index |= NEXT_BIT(in) << bit_index++;
		DROP_BIT(in);
	}
	if (!more)
	{
		p.L -= count_leading_zeros(index, bit_index, p.l);
	}
	while (more)
	{
		more = NEXT_BIT(in);
		DROP_BIT(in);
		if (more)
		{
			p.d_L++;
		}
		p.L++;
		for (int i = 0; i < p.d_l; i++)
		{
			index |= NEXT_BIT(in) << bit_index++;
			DROP_BIT(in);
		}
	}
	adjust_var_encoding_params(p);

	return index;
}

int count_leading_zeros (unsigned int index, int bit_index, int max)
{
	int unused_bits;
	for (unused_bits = 0; 
		unused_bits < max && !((index >> bit_index - unused_bits - 1) & 1); 
		unused_bits++);
	return unused_bits;
}

void adjust_var_encoding_params (VarEncodingParams &p)
{
	if (p.L > p.max_L)
	{
		p.l++;
		p.L = 0;
	}
	else if (p.L < -p.max_L)
	{
		p.l--;
		p.L = 0;
	}
	if (p.d_L > p.max_d_L)
	{
		p.d_l++;
		p.d_L = 0;
	}
	else if (p.d_L < -p.max_d_L)
	{
		p.d_l--;
		p.d_L = 0;
	}
}

int count_used_bits (OctreeIndex index)
{
	int used_bits;
	for (used_bits = 0;
		index >> used_bits && used_bits < sizeof(OctreeIndex) * 8;
		used_bits++); //{ printf("%d %lx\n", used_bits, index >> used_bits);}
	return used_bits;
}

void write_bit_array (FILE *out, WriteableBitArray *in)
{
	if (in->active_byte > 0)
	{
		fwrite(in->data, sizeof(char), in->active_byte, out);
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

void read_bit_array (FILE *in, ReadableBitArray *out)
{
	if (out->active_byte > 0)
	{
		unsigned char *tmp = new unsigned char[out->size]();
		int bytes_full = out->size - out->active_byte;
		fread(tmp + bytes_full, sizeof(char), out->active_byte, in);
		memcpy(tmp, out->data + out->active_byte, bytes_full);
		memcpy(out->data, tmp, out->size);
		out->active_byte = 0;
		delete [] tmp;
	}
}
