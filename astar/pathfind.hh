#pragma once

#include <stdint.h>
#include <vector>
#include <functional>

using std::vector;

namespace A
{
	using heuristics_function = std::function<float(vertex, vertex)>;

	struct vertex
	{
		int32_t X;
		int32_t Y;
	};

	struct node
	{
		vertex position;
		size_t founder;

		float known_distance;
		float estimated_distance;
	};

	vector<vertex> calculate_shortest_path(vertex begin, vertex end, vertex size, vector<vertex> black_list, heuristics_function funct);

	namespace heuristics
	{
		float manhattan_distance(vertex node, vertex goal);
		float diagonal_distance(vertex node, vertex goal);
		float euclidean_distance(vertex node, vertex goal);
	}
}