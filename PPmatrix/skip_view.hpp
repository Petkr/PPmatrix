#pragma once
#include <cstddef>
#include "simple_view.hpp"

namespace PPmatrix
{
	template <iterator BaseIterator>
	class skip_iterator
	{
		BaseIterator base_iterator;
		size_t skip_length;
	public:
		constexpr skip_iterator(BaseIterator base_iterator, size_t skip_length)
			: base_iterator(base_iterator)
			, skip_length(skip_length)
		{}
		constexpr decltype(auto) operator*() const
		{
			return *base_iterator;
		}
		constexpr auto& operator+=(size_t offset)
		{
			base_iterator += offset * skip_length;
			return *this;
		}
		constexpr auto& operator-=(size_t offset)
		{
			base_iterator -= offset * skip_length;
			return *this;
		}
		constexpr auto operator==(iterator auto other) const
		{
			return base_iterator == other;
		}
		constexpr auto& base()
		{
			return base_iterator;
		}
		constexpr auto base() const
		{
			return base_iterator;
		}
	};

	struct skip
	{
		size_t skip_length;
		skip(size_t skip_length)
			: skip_length(skip_length)
		{}

		constexpr iterator auto aligned_end(view auto&& v)
		{
			return end(v) + skip_length - PPmatrix::size(v) % skip_length;
		}
	};

	constexpr iterator auto operator&(iterator auto i, skip s)
	{
		return skip_iterator(i, s.skip_length);
	}
	constexpr view auto operator||(view auto&& v, skip s)
	{
		return begin(v) & s ^ s.aligned_end(v);
	}
	constexpr view auto operator|(view auto&& v, skip s)
	{
		return begin(v) & s ^ s.aligned_end(v) & s;
	}
}
