#include "omeltchenko99-simple.h"
#include "omeltchenko99.h"
#include <vector>
#include <list>

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
	Coordinate tmp[] = {
		Coordinate(2192, 3295, 52299),
		Coordinate(2174, 3201, 52328),
		Coordinate(2126, 3361, 52337),
		Coordinate(2331, 3317, 52342),
		Coordinate(2403, 3382, 52223),
		Coordinate(2335, 3303, 52110),
		Coordinate(2188, 3299, 52152),
		Coordinate(2352, 3399, 52469),
		Coordinate(2311, 3513, 52451),
		Coordinate(2424, 3346, 52566)
	};

	vector<Coordinate> coordinates(tmp, tmp + 10);
	list<char> compressed;
	compress(compressed, coordinates);
	list<Coordinate> uncompressed;
	uncompress(uncompressed, compressed);

	{
		Coordinate c(2192, 3295, 52299);
		Coordinate c2 = get_coordinate(get_rtree_index(c));
		e_assert(c == c2, "compress and uncompress");
	}

	return 0;
}
