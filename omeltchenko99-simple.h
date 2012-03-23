#include <vector>
#include <list>
#include "omeltchenko99.h"

using namespace std;

typedef OctreeIndex RTreeIndex;

void compress (list<unsigned char> &compressed, vector<Coordinate> &coordinates);
void uncompress (list<Coordinate> &coordinates, list<unsigned char> &compressed);

void varlength_encode (list<unsigned char> &compressed, list<RTreeIndex> &indexes);
void varlength_decode (list<RTreeIndex> &indexes, list<unsigned char> &compressed, int num_compressed);

RTreeIndex get_rtree_index(Coordinate coordinate);
Coordinate get_coordinate(RTreeIndex index);
