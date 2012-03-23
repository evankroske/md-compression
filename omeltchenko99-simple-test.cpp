#include "omeltchenko99-simple.h"
#include "omeltchenko99.h"
#include <vector>
#include <list>
#include <algorithm>

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
		list<unsigned char> compressed;
		compress(compressed, coordinates);
		list<Coordinate> uncompressed;
		uncompress(uncompressed, compressed);
	}

	{
		Coordinate c(2192, 3295, 52299);
		Coordinate c2 = get_coordinate(get_rtree_index(c));
		e_assert(c == c2, "compress and uncompress");
	}

	{
		RTreeIndex tmp[] = {
			0x17293568923645,
			0x32874912039472,
			0x90709327419232,
			0x18374656347092,
			0x91872309479287,
			0x10973246509123,
			0x17039465090950,
			0x73409283947012,
			0x27304659283498,
			0x12983740651983
		};
		list<RTreeIndex> index_diffs(tmp, tmp + 10);
		list<unsigned char> compressed;
		varlength_encode(compressed, index_diffs);
		list<RTreeIndex> uncompressed;
		varlength_decode(uncompressed, compressed, 10);
		e_assert(equal(index_diffs.begin(), index_diffs.end(), uncompressed.begin()), "varlength_encode and varlength_decode");
	}

	return 0;
}
