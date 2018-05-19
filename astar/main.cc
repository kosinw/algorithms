#include <iostream>
#include <stdint.h>

#include "pathfind.hh"
#include "../shared/min_heap.hh"

int main(int argc, char** argv)
{	

	using std::cin;
	using std::cout;	

	auto comparer = [] (int32_t a, int32_t b) -> int32_t {
		return a - b;
	};

	shared::structs::min_heap<int32_t> mh(comparer, {
		9, 10, 4, 7, 81, 2, -14
	});

	mh.push_back(21);
	mh.push_back(43);
	mh.push_back(-123);
	mh.push_back(0);
	mh.push_back(18);	

	auto next = mh.extract();	
	next = mh.extract();
	next = mh.extract();

	cin.get();
}