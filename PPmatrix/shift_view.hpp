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

	template <view View>
	constexpr view operator<<(View&& view, shift s)
	{
		return begin(view) - s.count ^ end(view) - s.count;
	}
	template <view View>
	constexpr view operator<<(shift s, View&& view)
	{
		return std::forward<View>(view) << s;
	}
	template <view View>
	constexpr view operator<(View&& view, shift s)
	{
		return begin(view) ^ end(view) - s.count;
	}
	template <view View>
	constexpr view operator<(shift s, View&& view)
	{
		return begin(view) - s.count ^ end(view);
	}
	template <view View>
	constexpr view operator>(View&& view, shift s)
	{
		return begin(view) ^ end(view) + s.count;
	}
	template <view View>
	constexpr view operator>(shift s, View&& view)
	{
		return begin(view) + s.count ^ end(view);
	}
}
