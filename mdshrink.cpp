#include <cstdio>
#include "omeltchenko99.h"
#include <vector>

using namespace std;

int compress (char *in_filename, char *out_filename)
{
	FILE *in_file = fopen(in_filename, "r");
	FILE *out_file = fopen(out_filename, "wb");
	vector<Coordinate> coordinates;
	read_md_data(coordinates, in_file);
	unsigned long *octree_indexes = new unsigned long[coordinates.size()];
	OctreeIndexParams p(21, 21, 21);
	int i = 0;
	for (vector<Coordinate>::iterator c = coordinates.begin();
		c != coordinates.end();
		c++)
	{
		octree_indexes[i] = octree_index(*c, p);
		i++;
	}
	fwrite(octree_indexes, sizeof(unsigned long), coordinates.size(), out_file);
	fclose(in_file);
	fclose(out_file);
	return 0;
}

int extract (char *in_filename, char *out_filename)
{
	return 0;
}

int main (int argc, char **argv)
{
	char *out_filename = NULL;
	char *in_filename = NULL;
	char *op = NULL;
	if (argc == 4)
	{
		op = argv[1];
		in_filename = argv[2];
		out_filename = argv[3];
	}
	else
	{
		printf("%d\n", argc);
		printf("Usage: %s OP IN_FILE OUT_FILE\n", argv[0]);
		return 1;
	}
	if (strcmp(op, "-c") == 0)
	{
		return compress(in_filename, out_filename);
	}
	else if (strcmp(op, "-x") == 0)
	{
		return extract(in_filename, out_filename);
	}
	else
	{
		puts("Unrecognized option: use -c or -x");
		return 1;
	}
}
