#include <iostream>
#include "omeltchenko99.h"

using namespace std;

FILE *log_file;

int main ()
{
	log_file = fopen("log.txt", "w");
	// log_file = stdout;

	Coordinate c(2, 1, 1);
	OctreeIndexParams p(2, 2, 2);
	uint o_i = octree_index(c, &p);
	fprintf(log_file, "octree_index = %u\n", o_i);

	return 0;
}
