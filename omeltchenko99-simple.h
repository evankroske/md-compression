#include <vector>
#include <list>
#include "omeltchenko99.h"

using namespace std;

typedef OctreeIndex RTreeIndex;

void compress (list<unsigned char> &compressed, list<Coordinate> &coordinates);
void uncompress (list<Coordinate> &coordinates, list<unsigned char> &compressed, int num_compressed);

void varlength_encode (list<unsigned char> &compressed, 
	vector<RTreeIndex> &indexes);
void varlength_decode (vector<RTreeIndex> &indexes, 
	list<unsigned char> &compressed, int num_compressed);

RTreeIndex get_rtree_index(Coordinate coordinate);
Coordinate get_coordinate(RTreeIndex index);
