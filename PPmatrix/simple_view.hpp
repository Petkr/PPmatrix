#pragma once
#include <utility>
#include "view.hpp"
#include "compressed_pair.hpp"
#include "unbounded.hpp"

namespace PPmatrix
{
	template <iterator Iterator, sentinel<Iterator> Sentinel>
	class simple_view
	{
		compressed_pair<Iterator, Sentinel> pair;

	public:
		constexpr simple_view(view auto&& v)
			: pair{ PPmatrix::begin(v), PPmatrix::end(v) }
		{}
		constexpr simple_view(Iterator begin, Sentinel end)
			: pair{ begin, end }
		{}
		constexpr iterator auto begin() const
		{
			return pair.first;
		}
		constexpr auto end() const
		{
			return pair.second;
		}
	};
	template <view View>
	simple_view(View&&) -> simple_view<begin_t<View>, end_t<View>>;

	template <iterator Iterator>
	constexpr view auto operator^(Iterator begin, sentinel<Iterator> auto end)
	{
		return simple_view(begin, end);
	}

	constexpr view auto operator|(view auto&& v, unbounded_t)
	{
		return begin(v) ^ unbounded;
	}
}
