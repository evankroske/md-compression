#include <cstdio>
#include <vector>
#include "omeltchenko99.h"
#include <algorithm>
#include "bin.h"

using namespace std;

int compress (char *in_filename, char *out_filename, OctreeIndexParams &p, VarEncodingParams &v)
{
	FILE *in_file = fopen(in_filename, "r");
	FILE *out_file = fopen(out_filename, "wb");
	vector<Coordinate> coordinates;
	read_md_data(coordinates, in_file);
	unsigned long *octree_indexes = new unsigned long[coordinates.size()];
	int i = 0;
	for (vector<Coordinate>::iterator c = coordinates.begin();
		c != coordinates.end();
		c++)
	{
		octree_indexes[i] = octree_index(*c, p);
		i++;
	}
	sort(octree_indexes, octree_indexes + coordinates.size());
	compute_differences(octree_indexes, coordinates.size());
	int num_indexes = coordinates.size();
	fwrite(&num_indexes, sizeof(int), 1, out_file);
	fwrite(octree_indexes, sizeof(unsigned long), coordinates.size(), out_file);
	delete[] octree_indexes;
	fclose(in_file);
	fclose(out_file);
	return 0;
}

int extract (char *in_filename, char *out_filename, OctreeIndexParams &p, VarEncodingParams &v)
{
	FILE *in_file = fopen(in_filename, "rb");
	FILE *out_file = fopen(out_filename, "w");
	int num_indexes = 0;
	fread(&num_indexes, sizeof(int), 1, in_file);
	unsigned long *octree_indexes = new unsigned long[num_indexes];
	fread(octree_indexes, sizeof(unsigned long), num_indexes, in_file);
	compute_sums(octree_indexes, num_indexes);
	for (int i = 0; i < num_indexes; i++)
	{
		Coordinate c = un_octree_index(octree_indexes[i], p);
		print_coordinate(out_file, c);
	}
	fclose(in_file);
	fclose(out_file);
	delete[] octree_indexes;
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
