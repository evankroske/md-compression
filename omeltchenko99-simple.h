#include <vector>
#include <list>

using namespace std;

struct Coordinate;
typedef unsigned long RTreeIndex;

void compress (list<char> &compressed, vector<Coordinate> &coordinates);
void uncompress (list<Coordinate> &coordinates, list<char> &compressed);

void varlength_encode (list<char> &compressed, list<RTreeIndex> &indexes);
void varlength_decode (list<RTreeIndex &indexes, list<char> &compressed, int num_compressed);

RTreeIndex get_rtree_index(Coordinate coordinate);
Coordinate get_coordinate(RTreeIndex index);

