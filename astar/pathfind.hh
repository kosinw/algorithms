#pragma once

#include <stdint.h> 
#include <vector>
#include <memory>

namespace A
{
	struct Vertex
	{
		int32_t X;
		int32_t Y;
	};

	struct Node
	{
		Vertex V;

		std::shared_ptr<Node> Founder;

		float G; // Distance value
		float H; // Heuristics value

		inline float D() { return G + H; }		
	};

	class Pathfinder
	{
		using HeuristicsFunction = float(*)(Vertex, Vertex);

	public:
		Pathfinder();
		void BlockVertex(Vertex v);
		void UnblockVertex(Vertex v);

		std::vector<Vertex> CalculateShortestPath(Vertex start, Vertex goal);

		void SetFunction(HeuristicsFunction funct);
		void SetGraphSize(uint32_t X, uint32_t Y);
	private:
		bool IsNodeUnblocked(Vertex v);
		std::vector<Vertex> blockedVertices_;
		HeuristicsFunction function_ = nullptr;
		Vertex size_;
	};

	namespace Heurisitics
	{
		float Manhattan(Vertex a, Vertex b);
		float Euclidean(Vertex a, Vertex b);
	}
}