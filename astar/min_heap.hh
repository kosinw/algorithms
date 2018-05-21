#pragma once

#include <vector>
#include <algorithm>
#include <initializer_list>
#include <stdint.h>

using std::vector;
using std::initializer_list;

namespace Shared
{
	template<typename T>
	class MinHeap : public vector<T>
	{
		using Comparer = int32_t(*)(T, T);

	public:
		explicit MinHeap() noexcept;
		explicit MinHeap(Comparer c, initializer_list<T> list);
		explicit MinHeap(Comparer c);
		void SetComparer(Comparer c);
		void PushBack(T item);
		T Extract();
	private:
		Comparer comparer_;
		void UpHeap(size_t idx);
		void DownHeap(size_t idx);
		void Swap(size_t a, size_t b);
		size_t SmallestValue(size_t parent, size_t a, size_t b);
		size_t Child(size_t idx) const;
		size_t Parent(size_t idx) const;
	};

	template<typename T>
	inline MinHeap<T>::MinHeap() noexcept
		:vector<T>::vector()
	{
		comparer_ = nullptr;
	}

	template<typename T>
	inline MinHeap<T>::MinHeap(Comparer c, initializer_list<T> list)
		:comparer_(c)
	{
		for (auto a : list)
		{
			this->PushBack(a);
		}
	}

	template<typename T>
	inline MinHeap<T>::MinHeap(Comparer c)
		:comparer_(c)
	{
	}

	template<typename T>
	inline void MinHeap<T>::SetComparer(Comparer c)
	{
		comparer_ = c;
	}

	template<typename T>
	inline void MinHeap<T>::PushBack(T item)
	{
		vector<T>::push_back(item);
		UpHeap(this->size() - 1);
	}

	template<typename T>
	inline T MinHeap<T>::Extract()
	{
		T first = this->operator[](0);

		this->operator[](0) = this->operator[](this->size() - 1);
		this->resize(this->size() - 1);
		DownHeap(0);

		return first;
	}	

	template<typename T>
	inline void MinHeap<T>::UpHeap(size_t idx)
	{
		if (!comparer_)
			throw std::exception("[min_heap] -> comparer_ is nullptr");

		if (idx <= 0)
			return;

		size_t parentIdx = Parent(idx);

		auto compare = comparer_((*this)[idx], (*this)[parentIdx]);

		if (compare < 0)
		{
			Swap(idx, parentIdx);

			UpHeap(parentIdx);
		}

		return;
	}

	template<typename T>
	inline void MinHeap<T>::DownHeap(size_t idx)
	{
		if (!comparer_)
			throw std::exception("[min_heap] -> comparer_ is nullptr");

		if (idx >= this->size())
			return;

		size_t childrenInd = Child(idx);

		size_t smallestIdx = SmallestValue(idx, childrenInd + 1, childrenInd + 2);

		if (idx != smallestIdx)
		{
			Swap(idx, smallestIdx);
			DownHeap(smallestIdx);
		}

		else
		{
			return;
		}
	}

	template<typename T>
	inline void MinHeap<T>::Swap(size_t a, size_t b)
	{
		T val = (*this)[a];
		(*this)[a] = (*this)[b];
		(*this)[b] = val;
	}

	template<typename T>
	inline size_t MinHeap<T>::SmallestValue(size_t parent, size_t a, size_t b)
	{
		bool isLeftSmaller = false;

		if (a < this->size())
		{
			isLeftSmaller = comparer_((*this)[parent], (*this)[a]) > 0;
		}

		bool rightExists = b < this->size();

		if (isLeftSmaller)
		{
			return a;
		}		

		else if (rightExists && comparer_((*this)[parent], (*this)[b]) > 0)
		{
			return b;
		}

		return parent;
	}

	template<typename T>
	inline size_t MinHeap<T>::Child(size_t idx) const
	{
		return idx * 2;
	}

	template<typename T>
	inline size_t MinHeap<T>::Parent(size_t idx) const
	{
		return (idx - 1) / 2;
	}
}