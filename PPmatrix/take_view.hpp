#pragma once
#include <cstddef>
#include "view.hpp"

namespace PPmatrix
{
	template <typename BaseIterator>
	class take_iterator
	{
		BaseIterator base_iterator;
		std::size_t count;
	public:
		constexpr take_iterator(BaseIterator base_iterator, std::size_t count)
			: base_iterator(base_iterator)
			, count(count)
		{}

		constexpr decltype(auto) operator*()
		{
			return *base_iterator;
		}
		constexpr auto& operator+=(std::size_t offset)
		{
			base_iterator += offset;
			count -= offset;
			return *this;
		}
		template <typename OtherIterator>
		constexpr auto operator!=(OtherIterator) const
		{
			return *this != unbounded_sentinel;
		}
		constexpr auto operator!=(unbounded_sentinel_t) const
		{
			return count != 0;
		}
	};

	struct take
	{
		std::size_t count;
		constexpr take(std::size_t count)
			: count(count)
		{}
	};

	template <typename Iterator>
	constexpr auto operator&(Iterator i, take t)
	{
		return take_iterator(i, t.count);
	}
	template <typename View>
	constexpr auto operator|(View&& view, take t)
	{
		return begin(view) & t ^ unbounded_sentinel;
	}
}
