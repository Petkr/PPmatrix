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
			if (offset <= count)
				count -= offset;
			return *this;
		}
		template <iterator OtherIterator>
		constexpr auto operator!=(OtherIterator i) const
		{
			return compare_iterator(base_iterator, i) && count != 0;
		}
	};

	struct take
	{
		std::size_t count;
		constexpr take(std::size_t count)
			: count(count)
		{}
	};

	template <iterator Iterator>
	constexpr auto take_view(Iterator i, std::size_t n)
	{
		return take_iterator(i, n) ^ unbounded;
	}
	template <view View>
	constexpr auto take_view(View&& v, std::size_t n)
	{
		return take_iterator(begin(v), n) ^ end(v);
	}

	template <typename Iterator>
	constexpr auto operator&(Iterator i, take t)
	{
		return take_iterator(i, t.count);
	}
	template <view View>
	constexpr auto operator||(View&& v, take t)
	{
		return take_view(begin(v), t.count);
	}
	template <view View>
	constexpr auto operator|(View&& v, take t)
	{
		return take_view(std::forward<View>(v), t.count);
	}
}
