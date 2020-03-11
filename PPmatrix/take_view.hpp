#pragma once
#include <cstddef>
#include "view.hpp"

namespace PPmatrix
{
	template <iterator BaseIterator>
	class take_iterator
	{
		BaseIterator base_iterator;
		std::size_t count;
	public:
		constexpr take_iterator(BaseIterator base_iterator, std::size_t count)
			: base_iterator(base_iterator)
			, count(count)
		{}

		constexpr decltype(auto) operator*() const
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
		constexpr auto operator==(iterator auto other) const
		{
			return count == 0 || base_iterator == other;
		}
	};

	struct take
	{
		std::size_t count;
		constexpr take(std::size_t count)
			: count(count)
		{}
	};

	constexpr view auto take_view(iterator auto i, std::size_t n)
	{
		return take_iterator(i, n) ^ unbounded;
	}
	constexpr view auto take_view(view auto&& v, std::size_t n)
	{
		return take_iterator(begin(v), n) ^ end(v);
	}

	constexpr iterator auto operator&(iterator auto i, take t)
	{
		return take_iterator(i, t.count);
	}
	constexpr view auto operator||(view auto&& v, take t)
	{
		return take_view(begin(v), t.count);
	}
	constexpr view auto operator|(view auto&& v, take t)
	{
		return take_view(v, t.count);
	}
}
