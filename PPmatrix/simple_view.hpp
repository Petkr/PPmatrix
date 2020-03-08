#pragma once
#include <utility>
#include "view.hpp"
#include "compressed_pair.hpp"
#include "unbounded_sentinel_t.hpp"

namespace PPmatrix
{
	template <iterator Iterator, sentinel<Iterator> Sentinel>
	class simple_view
	{
		compressed_pair<Iterator, Sentinel> pair;

	public:
		template <view View>
		constexpr simple_view(View&& v)
			: pair{ PPmatrix::begin(v), PPmatrix::end(v) }
		{}
		constexpr simple_view(Iterator begin, Sentinel end)
			: pair{ begin, end }
		{}
		constexpr iterator begin() const
		{
			return pair.first;
		}
		constexpr iterator end() const
		{
			return pair.second;
		}
	};
	template <view View>
	simple_view(View&&) -> simple_view<begin_t<View>, end_t<View>>;

	template <iterator Iterator, sentinel<Iterator> Sentinel>
	constexpr auto operator^(Iterator begin, Sentinel end)
	{
		return simple_view(begin, end);
	}

	template <view View>
	constexpr auto operator|(View&& view, unbounded_t)
	{
		return begin(view) ^ unbounded;
	}
}
