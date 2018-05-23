#include <array>
#include <stdint.h>

#pragma once

enum class AStarState
{
	VISITED = 1,
	SOURCE = 2,
	GOAL = 3,
	NONE = 0
};

template <int32_t X, int32_t Y>
class Grid
{
	using GridArray = std::array<AStarState, Y * X>;

  public:
	explicit Grid();

	Grid Resize(int32_t newX, int32_t newY);
	int32_t Size();

  private:
	int32_t x_;
	int32_t y_;

	GridArray<X, Y> internal_;
};
