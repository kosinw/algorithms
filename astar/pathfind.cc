#include "pathfind.hh"

#include <map>
#include <set>

#include "../shared/min_heap.hh"

using std::vector;
using std::map;
using std::set;

using A::vertex;
using A::node;
using A::heuristics_function;

using shared::structs::min_heap;

vector<vertex> A::calculate_shortest_path(vertex begin, vertex end, vertex size, vector<vertex> black_list, heuristics_function funct)
{	
	auto comparer = [](node a, node b) -> int32_t {
		float cumulativeA =  a.known_distance + a.estimated_distance;
		float cumulativeB = b.known_distance + b.estimated_distance;

		return cumulativeA - cumulativeB;
	};

	min_heap<node> open_set(comparer);
	set<vertex> in_open_set;
	set<int32_t> closed_set;	

	return vector<vertex>();
}