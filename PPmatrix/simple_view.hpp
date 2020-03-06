#pragma once
#include <utility>
#include "view.hpp"
#include "compressed_pair.hpp"
#include "unbounded_sentinel_t.hpp"

namespace PPmatrix
{
	template <typename Iterator, typename Sentinel>
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
		constexpr auto begin() const
		{
			return pair.first;
		}
		constexpr auto end() const
		{
			return pair.second;
		}
	};
	template <typename View>
	simple_view(View&&) -> simple_view<begin_t<View>, end_t<View>>;

	template <typename Iterator, typename Sentinel>
	constexpr auto operator^(Iterator begin, Sentinel end)
	{
		return simple_view(begin, end);
	}

	template <typename View>
	constexpr auto operator|(View&& view, unbounded_t)
	{
		return begin(view) ^ unbounded;
	}
}
