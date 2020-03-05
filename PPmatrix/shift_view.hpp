#pragma once
#include "view.hpp"

namespace PPmatrix
{
	struct shift
	{
		std::size_t count;
		shift(std::size_t count)
			: count(count)
		{}
	};

	template <typename View>
	constexpr auto operator<<(View&& view, shift s)
	{
		return begin(view) - s.count ^ end(view) - s.count;
	}
	template <typename View>
	constexpr auto operator<<(shift s, View&& view)
	{
		return std::forward<View>(view) << s;
	}
	template <typename View>
	constexpr auto operator<(View&& view, shift s)
	{
		return begin(view) ^ end(view) - s.count;
	}
	template <typename View>
	constexpr auto operator<(shift s, View&& view)
	{
		return begin(view) - s.count ^ end(view);
	}
	template <typename View>
	constexpr auto operator>(View&& view, shift s)
	{
		return begin(view) ^ end(view) + s.count;
	}
	template <typename View>
	constexpr auto operator>(shift s, View&& view)
	{
		return begin(view) + s.count ^ end(view);
	}
}
