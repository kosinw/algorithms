#include "pathfind.hh"

#include <vector>
#include <array>
#include <memory>
#include <algorithm>
#include <stdlib.h>

#include "min_heap.hh"

using A::Node;
using A::Vertex;

float A::Heurisitics::Manhattan(Vertex a, Vertex b)
{
	uint32_t dx = abs(a.X - b.X);
	uint32_t dy = abs(a.Y - b.Y);

	return (float)(dx + dy);
}

float A::Heurisitics::Euclidean(Vertex a, Vertex b)
{
	uint32_t dx = abs(a.X - b.X);
	uint32_t dy = abs(a.Y - b.Y);

	return (float)sqrt(dx * dx + dy * dy);
}

A::Pathfinder::Pathfinder()
{
}

void A::Pathfinder::BlockVertex(Vertex v)
{
	blockedVertices_.push_back(v);
}

void A::Pathfinder::UnblockVertex(Vertex v)
{
	blockedVertices_.push_back(v);
}

std::vector<Vertex> A::Pathfinder::CalculateShortestPath(Vertex start, Vertex goal)
{
	if (!function_)
		throw std::exception("function_ [HeuristicsFunction] is nullptr");

	auto comparer = [](std::shared_ptr<Node> a, std::shared_ptr<Node> b) -> int32_t {
		auto result = a->D() - b->D();

		if (result < 0)
		{
			return (int32_t)floor(result);
		}

		return (int32_t)ceil(result);
	};

	A::MinHeap<std::shared_ptr<Node>> mh(comparer);

	{
		std::shared_ptr<Node> startNode = std::make_shared<Node>();
		startNode->V = start;
		startNode->Founder = nullptr;
		startNode->H = function_(start, goal);
		startNode->G = 0;
		mh.PushBack(startNode);
	}

	std::array<Vertex, 8> directions{
		0, 1,
		1, 0,
		-1, 0,
		0, -1,
		1, 1,
		1, -1,
		-1, 1,
		-1, -1};

	std::vector<Vertex> visited;

	std::shared_ptr<Node> current;

	while (mh.size() > 0)
	{
		current = mh.Extract();

		for (auto vertex : directions)
		{
			Vertex neighbor = {current->V.X + vertex.X, current->V.Y + vertex.Y};

			bool notBlacklisted = IsNodeUnblocked(neighbor);

			bool notVisited = true;

			for (size_t i = 0; i < visited.size(); ++i)
			{
				if (visited[i].X == neighbor.X && visited[i].Y == neighbor.Y)
				{
					notVisited = false;
					break;
				}
			}

			if (notBlacklisted && notVisited)
			{
				bool wasQueued = false;

				std::shared_ptr<Node> next = std::make_shared<Node>();
				next->V = neighbor;
				next->Founder = current;
				next->H = function_(neighbor, goal);
				next->G = current->G + 1;

				for (size_t i = 0; i < mh.size(); ++i)
				{
					auto currentValue = mh[i];

					if (currentValue->V.X == neighbor.X && currentValue->V.Y == neighbor.Y)
					{
						next = currentValue;
						wasQueued = true;
						break;
					}
				}

				if ((current->G + 1 + current->H) < next->D())
				{
					next->G = current->G + 1;
					next->Founder = current;
				}

				if (!wasQueued)
				{
					mh.PushBack(next);
				}
			}
		}

		visited.push_back(current->V);

		if (current->V.X == goal.X && current->V.Y == goal.Y)
		{
			break;
		}
	}

	std::vector<Vertex> ret;

	if (current->V.X != goal.X && current->V.Y != goal.Y)
	{
		return std::vector<Vertex>();
	}

	while (current != nullptr)
	{
		ret.push_back(current->V);
		current = current->Founder;
	}

	return ret;
}

void A::Pathfinder::SetFunction(HeuristicsFunction funct)
{
	function_ = funct;
}

void A::Pathfinder::SetGraphSize(uint32_t X, uint32_t Y)
{
	size_ = {(int32_t)X, (int32_t)Y};
}

bool A::Pathfinder::IsNodeUnblocked(Vertex v)
{
	if (v.X < 0 || v.X > size_.X || v.Y < 0 || v.Y > size_.Y)
	{
		return false;
	}

	for (size_t i = 0; i < blockedVertices_.size(); ++i)
	{
		auto current = blockedVertices_[i];

		if (current.X == v.X && current.Y == v.Y)
		{
			return false;
		}
	}

	return true;
}
