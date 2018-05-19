#pragma once

#include <vector>
#include <stdexcept>
#include <initializer_list>
#include <stdint.h>

using std::vector;
using std::initializer_list;

namespace shared
{
	namespace structs
	{
		template<typename T>
		class min_heap : public vector<T>
		{
			using comparer = int32_t(*)(T, T);

		public:
			explicit min_heap() noexcept;
			explicit min_heap(comparer c, initializer_list<T> list);
			explicit min_heap(comparer c);
			void set_comparer(comparer c);
			void push_back(T item);
			T extract();
		private:
			comparer comparer_;
			void up_heap(size_t idx);
			void down_heap(size_t idx);
			void swap(size_t a, size_t b);
			size_t smallest_value(size_t parent, size_t a, size_t b);
			size_t child(size_t idx) const;
			size_t parent(size_t idx) const;
		};

		template<typename T>
		inline min_heap<T>::min_heap() noexcept
			:vector<T>::vector()
		{
			comparer_ = nullptr;
		}

		template<typename T>
		inline min_heap<T>::min_heap(comparer c, initializer_list<T> list)
			:comparer_(c)
		{
			for (auto a : list)
			{
				this->push_back(a);
			}
		}

		template<typename T>
		inline min_heap<T>::min_heap(comparer c)
			:comparer_(c)
		{
		}

		template<typename T>
		inline void min_heap<T>::set_comparer(comparer c)
		{
			comparer_ = c;
		}

		template<typename T>
		inline void min_heap<T>::push_back(T item)
		{
			vector<T>::push_back(item);
			up_heap(this->size() - 1);
		}

		template<typename T>
		inline T min_heap<T>::extract()
		{
			T first = this->operator[](0);

			this->operator[](0) = this->operator[](this->size() - 1);
			this->resize(this->size() - 1);
			down_heap(0);

			return first;
		}

		template<typename T>
		inline void min_heap<T>::up_heap(size_t idx)
		{
			if (!comparer_)
				throw std::exception("[min_heap] -> comparer_ is nullptr");

			if (idx <= 0)
				return;

			size_t parentIdx = parent(idx);

			if (comparer_(this->operator[](idx), this->operator[](parentIdx)) < 0)
			{
				swap(idx, parentIdx);

				up_heap(parentIdx);
			}

			return;
		}

		template<typename T>
		inline void min_heap<T>::down_heap(size_t idx)
		{
			if (!comparer_)
				throw std::exception("[min_heap] -> comparer_ is nullptr");

			if (idx >= this->size())
				return;

			size_t childrenInd = child(idx);

			size_t smallestIdx = smallest_value(idx, childrenInd + 1, childrenInd + 2);

			if (idx != smallestIdx)
			{
				swap(idx, smallestIdx);
				down_heap(smallestIdx);
			}

			else
			{
				return;
			}
		}

		template<typename T>
		inline void min_heap<T>::swap(size_t a, size_t b)
		{
			T val = this->operator[](a);
			this->operator[](a) = this->operator[](b);
			this->operator[](b) = val;
		}

		template<typename T>
		inline size_t min_heap<T>::smallest_value(size_t parent, size_t a, size_t b)
		{
			bool isLeftSmaller = false;

			if (this->operator[](a))
			{
				isLeftSmaller = comparer_(this->operator[](parent), this->operator[](a)) > 0;
			}

			bool rightExists = this->operator[](b);

			if (isLeftSmaller)
			{
				return a;
			}

			else if (rightExists && comparer_(this->operator[](parent), this->operator[](b)) > 0)
			{
				return b;
			}

			return parent;
		}

		template<typename T>
		inline size_t min_heap<T>::child(size_t idx) const
		{
			return idx * 2;
		}

		template<typename T>
		inline size_t min_heap<T>::parent(size_t idx) const
		{
			return (idx - 1) / 2;
		}
	}
}