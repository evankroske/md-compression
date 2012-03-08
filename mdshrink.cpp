#include <cstdio>
#include <vector>
#include "omeltchenko99.h"
#include <algorithm>
#include "bin.h"

using namespace std;

int compress (FILE *in_file, FILE *out_file, OctreeIndexParams &p, VarEncodingParams &v)
{
	vector<Coordinate> coordinates;
	read_md_data(coordinates, in_file);
	unsigned long *octree_indexes = new unsigned long[coordinates.size()];
	if (octree_indexes == NULL)
	{
		fprintf(stderr, "Memory allocation error\n");
		exit(1);
	}

	{
		int i = 0;
		for (vector<Coordinate>::iterator c = coordinates.begin();
			c != coordinates.end();
			c++)
		{
			octree_indexes[i] = octree_index(*c, p);
			Coordinate test = un_octree_index(octree_indexes[i], p);
			if (test != *c)
			{
				fprintf(stderr, "Octree index overflow\n", i);
				print_coordinate(stderr, *c);
				exit(1);
			}
			i++;
		}
	}
	sort(octree_indexes, octree_indexes + coordinates.size());

	compute_differences(octree_indexes, coordinates.size());

	fwrite(&p, sizeof(OctreeIndexParams), 1, out_file);
	fwrite(&v, sizeof(VarEncodingParams), 1, out_file);

	int num_indexes = coordinates.size();
	fwrite(&num_indexes, sizeof(int), 1, out_file);

	{
		BitArray encoded_index;
		WriteableBitArray b;
		for (int i = 0; i < num_indexes; i++)
		{
			// printf("%lx\n", octree_indexes[i]);
			encoded_index = var_encode_index(octree_indexes[i], v);
			/*
			if (i == 45)
			{
				printf("%lx %lx %d\n", octree_indexes[i], encoded_index.data, encoded_index.bits_used);
			}
			*/
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

int extract (FILE *in_file, FILE *out_file)
{
	OctreeIndexParams p;
	VarEncodingParams v;
	fread(&p, sizeof(OctreeIndexParams), 1, in_file);
	fread(&v, sizeof(VarEncodingParams), 1, in_file);

	int num_indexes = 0;
	fread(&num_indexes, sizeof(int), 1, in_file);

	unsigned long *octree_indexes = new unsigned long[num_indexes];

	{
		ReadableBitArray a;
		for (int i = 0; i < num_indexes; i++)
		{
			read_bit_array(in_file, &a);
			octree_indexes[i] = var_decode_index(&a, v);
			// printf("%lx\n", octree_indexes[i]);
		}
	}
	compute_sums(octree_indexes, num_indexes);
	for (int i = 0; i < num_indexes; i++)
	{
		Coordinate c = un_octree_index(octree_indexes[i], p);
		print_coordinate(out_file, c);
	}
	delete[] octree_indexes;
	fclose(in_file);
	fclose(out_file);
	return 0;
}

int main (int argc, char **argv)
{
	if (argc < 2)
	{
		printf("%d\n", argc);
		printf("Usage: %s OP [IN_FILE OUT_FILE]\n", argv[0]);
		return 1;
	}
	char *op = argv[1];
	VarEncodingParams v(63, 2);
	OctreeIndexParams p(21, 21, 21);

	int argi = 2;
	if (argc > 2)
	{
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
	}

	FILE *in = stdin;
	FILE *out = stdout;

	if (strcmp(op, "-c") == 0)
	{
		if (argi < argc)
		{
			in = fopen(argv[argi++], "r");
			out = fopen(argv[argi++], "wb");
		}
		return compress(in, out, p, v);
	}
	else if (strcmp(op, "-x") == 0)
	{
		if (argi < argc)
		{
			in = fopen(argv[argi++], "rb");
			out = fopen(argv[argi++], "w");
		}
		return extract(in, out);
	}
	else
	{
		puts("Unrecognized option: use -c or -x");
		return 1;
	}
}
