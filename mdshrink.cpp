#include <cstdio>
#include <vector>
#include "omeltchenko99.h"
#include <algorithm>
#include "bin.h"

using namespace std;

int compress (char *in_filename, char *out_filename, OctreeIndexParams &p, VarEncodingParams &v)
{
	FILE *in_file = stdin;
	FILE *out_file = fopen(out_filename, "wb");
	vector<Coordinate> coordinates;
	read_md_data(coordinates, in_file);
	unsigned long *octree_indexes = new unsigned long[coordinates.size()];
	{
		int i = 0;
		for (vector<Coordinate>::iterator c = coordinates.begin();
			c != coordinates.end();
			c++)
		{
			octree_indexes[i] = octree_index(*c, p);
			printf("Coordinate: (%d, %d, %d); Index: %lx\n", c->x, c->y, c->z,
				octree_indexes[i]);
			i++;
		}
	}
	sort(octree_indexes, octree_indexes + coordinates.size());
	compute_differences(octree_indexes, coordinates.size());

	fwrite(&p, sizeof(OctreeIndexParams), 1, out_file);
	fwrite(&v, sizeof(VarEncodingParams), 1, out_file);

	int num_indexes = coordinates.size();
	fwrite(&num_indexes, sizeof(int), 1, out_file);
	printf("num_indexes %d\n", num_indexes);
	{
		OctreeIndex encoded_index;
		WriteableBitArray b;
		for (int i = 0; i < num_indexes; i++)
		{
			encoded_index = var_encode_index(octree_indexes[i], v);
			bit_array_append(&b, encoded_index);
			write_bit_array(out_file, &b);
		}
		write_bit_array(out_file, &b);
	}
	delete[] octree_indexes;
	fclose(in_file);
	fclose(out_file);
	return 0;
}

int extract (char *in_filename, char *out_filename, OctreeIndexParams &p, VarEncodingParams &v)
{
	FILE *in_file = fopen(in_filename, "rb");
	FILE *out_file = fopen(out_filename, "w");

	OctreeIndexParams f_p;
	VarEncodingParams f_v;
	fread(&f_p, sizeof(OctreeIndexParams), 1, in_file);
	fread(&f_v, sizeof(VarEncodingParams), 1, in_file);
	printf("OctreeIndexParams(%d, %d, %d)\n", f_p.x_width, f_p.y_width, f_p.z_width);
	printf("VarEncodingParams(%d, %d, %d, %d)\n", f_v.l, f_v.d_l, f_v.L, f_v.d_L);

	int num_indexes = 0;
	fread(&num_indexes, sizeof(int), 1, in_file);
	printf("num_indexes %d\n", num_indexes);

	unsigned long *octree_indexes = new unsigned long[num_indexes];

	{
		ReadableBitArray a;
		for (int i = 0; i < num_indexes; i++)
		{
			read_bit_array(in_file, &a);
			octree_indexes[i] = var_decode_index(&a, f_v);
		}
	}
	compute_sums(octree_indexes, num_indexes);
	for (int i = 0; i < num_indexes; i++)
	{
		Coordinate c = un_octree_index(octree_indexes[i], f_p);
		print_coordinate(out_file, c);
	}
	delete[] octree_indexes;
	fclose(in_file);
	fclose(out_file);
	return 0;
}

int main (int argc, char **argv)
{
	if (argc < 4)
	{
		printf("%d\n", argc);
		printf("Usage: %s OP IN_FILE OUT_FILE\n", argv[0]);
		return 1;
	}
	char *op = argv[1];
	int argi = 2;
	VarEncodingParams v(3, 2);
	OctreeIndexParams p(21, 21, 21);
	bool more_flags = true;
	while (more_flags)
	{
		switch (argv[argi][1]) {
			case 'l':
				sscanf(argv[argi + 1], "%d", &v.l);
				sscanf(argv[argi + 2], "%d", &v.d_l);
				argi += 3;
				break;
			case 'w':
				sscanf(argv[argi + 1], "%d", &p.x_width);
				sscanf(argv[argi + 2], "%d", &p.y_width);
				sscanf(argv[argi + 3], "%d", &p.z_width);
				argi += 4;
				break;
			default:
				more_flags = false;
				break;
		}
	}
	char *in_filename = argv[argi];
	char *out_filename = argv[argi + 1];
	if (strcmp(op, "-c") == 0)
	{
		return compress(in_filename, out_filename, p, v);
	}
	else if (strcmp(op, "-x") == 0)
	{
		return extract(in_filename, out_filename, p, v);
	}
	else
	{
		puts("Unrecognized option: use -c or -x");
		return 1;
	}
}
