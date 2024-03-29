#include <iostream>
#include <cstring>
#include "omeltchenko99.h"
#include "bin.h"

using namespace std;

void e_assert (bool statement, char const *description)
{
	if (statement)
	{
		printf("%-60s Passed\n", description);
	}
	else
	{
		printf("%-60s Failed\n", description);
	}
}

int main ()
{
	e_assert(make_bitmask<unsigned char>(1, 3) == 14, "make_bitmask test");
	e_assert(msb_bitmask<unsigned char>(4) == 240, "msb_bitmask test");
	e_assert(lsb_bitmask<unsigned char>(5) == 31, "lsb_bitmask test");
	e_assert(msb_bitmask<int>(32) == -1, "msb_bitmask negative test 1");
	e_assert(msb_bitmask<int>(31) == -2, "msb_bitmask negative test 2");

	e_assert(str_to_bin<unsigned char>("00000011") == 3, "0b00000011 == 3");
	e_assert(str_to_bin<unsigned char>("11000000") == 192, "0b11000000 == 192");
	e_assert(str_to_bin<unsigned int>("101010101010") == 2730, "0b101010101010 == 2730");

	{
		unsigned long a = str_to_bin<unsigned 
			long>("11111111111111111111111111111111");
		e_assert(a == 0xffffffff, "str_to_bin unsigned long");
	}

	{
		Coordinate x(3, 4, 6);
		Coordinate y(3, 4, 6);
		Coordinate z(5, 3, 6);
		e_assert(x == y, "Coordinate equality");
		e_assert(x != z, "Coordinate inequality");
	}

	{
		Coordinate c(0, 1, 2);
		OctreeIndexParams p(3, 3, 3);
		Coordinate d = un_octree_index(octree_index(c, p), p);
		e_assert(c == d,
			"test small octree_index");
	}

	{
		Coordinate c(0, -1, 2);
		OctreeIndexParams p(3, 3, 3);
		Coordinate d = un_octree_index(octree_index(c, p), p);
		e_assert(c == d,
			"Small negative octree_index test");
	}

	{
		Coordinate c(0xfad13, 0xf1239, 0xf9802);
		OctreeIndexParams p(21, 21, 21);
		Coordinate d = un_octree_index(octree_index(c, p), p);
		print_coordinate(stdout, c);
		print_coordinate(stdout, d);
		e_assert(c == d,
			"test octree_index with big numbers");
	}

	{
		Coordinate c(739195, -699465, -95248);
		OctreeIndexParams p(21, 21, 21);
		Coordinate d = un_octree_index(octree_index(c, p), p);
		e_assert(c == d,
			"test octree_index with more big numbers");
	}

	{
		OctreeIndex i = str_to_bin<OctreeIndex>("111111");
		OctreeIndexParams p(3, 3, 3);
		Coordinate d = un_octree_index(i, p);
		OctreeIndex j = octree_index(d, p);
		e_assert(i == j,
			"un_octree_index with small numbers");
	}

	{
		Coordinate c(1, -1, 3);
		OctreeIndexParams p(2, 2, 3);
		Coordinate d = un_octree_index(octree_index(c, p), p);
		e_assert(d == c, 
			"un_octree_index small negative numbers test");
	}

	{
		Coordinate c(739195, -699465, -95248);
		OctreeIndexParams p(21, 21, 21);
		Coordinate d = un_octree_index(octree_index(c, p), p);
		e_assert(d == c, 
			"un_octree_index with large negative numbers");
	}

	{
		unsigned long a = str_to_bin<unsigned 
			long>("11111111111111111111111111111111");
		int n = count_used_bits(a);
		e_assert(n == 32, "count_used_bits 32 bit number");
	}

	{
		WriteableBitArray dst;
		BitArray b(0x7, 3);
		bit_array_append(&dst, b); 
		e_assert(dst.data[dst.active_byte] == msb_bitmask<unsigned char>(3), 
			"data 1 appended");
	}

	{
		WriteableBitArray dst;
		BitArray b(str_to_bin<int>("110011001010"), 12);
		bit_array_append(&dst, b);
		e_assert(dst.data[dst.active_byte - 1] == 
			str_to_bin<unsigned char>("11001100"),
			"First byte of data 2");
		e_assert(dst.data[dst.active_byte] ==
			str_to_bin<unsigned char>("10100000"),
			"Second byte of data 2");
	}

	{
		unsigned long a = 1;
		VarEncodingParams v(3, 2);
		BitArray a_encoded = var_encode_index(a, v);
		e_assert(a_encoded.data == 4, "var_encode_index tiny number");
		e_assert(v.L == -2, "var_encode_index L increase");
	}

	{
		OctreeIndex index = str_to_bin<unsigned long>("00011101");
		VarEncodingParams v(3, 2);
		BitArray encoded_index = var_encode_index(index, v);
		e_assert(encoded_index.data == str_to_bin<unsigned long>("1101011"),
			"var_encode_index small number");
		e_assert(v.L == 1, 
			"var_encode_index L increase");
	}

	{
		unsigned long index = str_to_bin<unsigned long>("001101");
		VarEncodingParams v(3, 3);
		var_encode_index(index, v);
		e_assert(v.d_L == -2, 
			"var_encode_index d_L decrease");
	}

	{
		unsigned long index = str_to_bin<unsigned long>("1001101");
		VarEncodingParams v(3, 2);
		var_encode_index(index, v);
		e_assert(v.d_L == 1, 
			"var_encode_index d_L increase");
	}

	{
		unsigned long index = str_to_bin<unsigned long>("10101010101");
		VarEncodingParams v(3, 2, 0, 0, 2, 2);
		var_encode_index(index, v);
		e_assert(v.d_l == 3 && v.d_L == 0, 
			"var_encode_index d_l increased in response to d_L");
	}

	{
		ReadableBitArray a;
		a.data[a.active_byte] = 0x50;
		VarEncodingParams p(3, 3);
		unsigned long decoded_index = var_decode_index(&a, p);
		e_assert(decoded_index == str_to_bin<unsigned long>("101"),
			"var_decode_index decode trivial index");
	}

	{
		ReadableBitArray a;
		a.data[a.active_byte] = 0xd1;
		VarEncodingParams p(3, 3);
		unsigned long decoded_index = var_decode_index(&a, p);
		e_assert(decoded_index == str_to_bin<unsigned long>("100101"),
			"var_decode_index decode short index");
	}

	{
		ReadableBitArray a;
		a.data[a.active_byte] = 0xbc;
		a.data[a.active_byte + 1] = 0x4d;
		a.data[a.active_byte + 2] = 0x10;
		VarEncodingParams p(3, 3);
		unsigned long decoded_index_1 = var_decode_index(&a, p);
		e_assert(decoded_index_1 == str_to_bin<unsigned long>("1001110"),
			"var_decode_index decode longer index");
		unsigned long decoded_index_2 = var_decode_index(&a, p);
		e_assert(decoded_index_2 == str_to_bin<unsigned long>("100101"),
			"var_decode_index decode consecutive index");
	}

	{
		ReadableBitArray a;
		a.data[a.active_byte] = 0xca;
		a.data[a.active_byte + 1] = 0x40;
		VarEncodingParams p(3, 2);
		unsigned long decoded_index = var_decode_index(&a, p);
		e_assert(p.d_L == 1,
			"var_decode_index d_L increase");
		e_assert(decoded_index == 0x51,
			"var_decode_index different l and d_l values");
	}

	{
		ReadableBitArray a;
		a.data[a.active_byte] = 0xd1;
		VarEncodingParams p(3, 3);
		unsigned long decoded_index = var_decode_index(&a, p);
		e_assert(p.L == 1,
			"var_decode_index L increase");
	}

	{
		FILE *test_file = tmpfile();
		WriteableBitArray a;
		ReadableBitArray b;
		a.data[a.active_byte] = 0xd1;

		write_bit_array(test_file, &a);
		e_assert(a.active_byte == 0 && a.data[a.active_byte] == 0x0,
			"write_bit_array write partial byte");
		rewind(test_file);
		read_bit_array(test_file, &b);
		e_assert(b.data[0] == 0xd1,
			"read_bit_array read byte");
		fclose(test_file);
	}

	{
		ReadableBitArray a;
		a.data[0] = 0xdd;
		a.data[1] = 0xdd;
		a.data[2] = 0x60;
		a.active_byte = 0;
		VarEncodingParams p(3, 3);
		OctreeIndex i = var_decode_index(&a, p);
		e_assert(i == str_to_bin<OctreeIndex>("011101101101101"),
			"var_decode_index long index");
	}

	{
		VarEncodingParams p1(62, 0);
		VarEncodingParams p2(p1);
		OctreeIndex i = str_to_bin<OctreeIndex>(
			"01101101101101101101101101101101101101101101101101101101101101");
		BitArray j = var_encode_index(i, p1);
		WriteableBitArray a;
		bit_array_append(&a, j);
		FILE *tmp = tmpfile();
		write_bit_array(tmp, &a);
		write_bit_array(tmp, &a);
		unsigned char junk[30] = {'A'};
		fwrite(junk, sizeof(unsigned char), 30, tmp);
		rewind(tmp);
		ReadableBitArray b;
		read_bit_array(tmp, &b);
		OctreeIndex k = var_decode_index(&b, p2);
		e_assert(k == i, "integration test");
		fclose(tmp);
	}

	{
		BitArray b(0x1ca15b6b85a31440, 63);
		VarEncodingParams v(62, 0);
		VarEncodingParams v2(v);
		WriteableBitArray a;
		bit_array_append(&a, b);
		FILE *tmp = tmpfile();
		write_bit_array(tmp, &a);
		bit_array_append(&a, b);
		write_bit_array(tmp, &a);
		write_bit_array(tmp, &a);
		rewind(tmp);
		ReadableBitArray c;
		read_bit_array(tmp, &c);
		OctreeIndex j = var_decode_index(&c, v2);
		
	}

	{
		Coordinate c(-999376, -595378, 1445828);
		Coordinate d(345231, 196796, -413505);
		OctreeIndexParams p(21, 21, 21);
		OctreeIndex i_c = octree_index(c, p);
		OctreeIndex i_d = octree_index(d, p);
		OctreeIndex a[2];
		if (i_c < i_d)
		{
			a[0] = i_c;
			a[1] = i_d - i_c;
		}
		else
		{
			a[0] = i_d;
			a[1] = i_c - i_d;
		}
	}

	{
		OctreeIndex i = 0x7;
		e_assert(count_leading_zeros(i, 5, 3) == 2,
			"count_leading_zeros easy");
	}

	{
		OctreeIndex i = 0x3ffffffffL;
		int j = count_leading_zeros(i, 36, 4);
		e_assert(j == 2,
			"count_leading_zeros medium");
	}

	{
		OctreeIndex a[] = {1, 2, 3, 4, 5};
		OctreeIndex b[] = {1, 2, 3, 4, 5};
		compute_differences(b, 5);
		compute_sums(b, 5);
		bool good = true;
		for (int i = 0; i < 5; i++)
		{
			good = good && a[i] == b[i];
		}
		e_assert(good, "compute_differences and compute_sums easy");
	}

	{
		OctreeIndex a[] = {1, 5, 10, 12, 13};
		OctreeIndex b[] = {1, 5, 10, 12, 13};
		compute_differences(b, 5);
		compute_sums(b, 5);
		bool good = true;
		for (int i = 0; i < 5; i++)
		{
			good = good && a[i] == b[i];
		}
		e_assert(good, "compute_differences and compute_sums medium");
	}

	{
		OctreeIndex a[] = {0x1ffffffffL, 0x2ffffffffL, 0x3ffffffffL, 0x4ffffffffL, 0x5ffffffffL};
		OctreeIndex b[] = {0x1ffffffffL, 0x2ffffffffL, 0x3ffffffffL, 0x4ffffffffL, 0x5ffffffffL};
		compute_differences(b, 5);
		compute_sums(b, 5);
		bool good = true;
		for (int i = 0; i < 5; i++)
		{
			good = good && a[i] == b[i];
		}
		e_assert(good, "compute_differences and compute_sums hard");
	}

	{
		Coordinate a(1, 2, 3);
		Coordinate b(1, 1, -1);
		OctreeIndexParams p(21, 21, 21);
		OctreeIndex a_i = octree_index(a, p);
		OctreeIndex b_i = octree_index(b, p);
		Coordinate c = un_octree_index(b_i, p);
		e_assert((a_i > b_i) ? a_i - b_i : b_i - a_i < 100, "octree_index positive-negative difference");
	}

/*
	{
		int a = 1, b = -1;
		int bias = 1 << 20;
		unsigned int c, d;
		c = a + bias;
		d = b + bias;
		c &= lsb_bitmask<unsigned int>(21);
		d &= lsb_bitmask<unsigned int>(21);
		int e, f;
		e = c - bias;
		f = d - bias;
		printf("%x %x\n", a, e);
		printf("%x %x\n", b, f);
		printf("%x %x\n", c, d);
		printf("%x\n", bias + 1);
	}
*/

	return 0;
}
