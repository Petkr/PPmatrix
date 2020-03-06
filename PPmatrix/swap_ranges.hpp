#pragma once
#include "zip.hpp"

namespace PPmatrix
{
	template <view View1, view View2>
	constexpr void swap_ranges(View1&& view1, View2&& view2)
	{
		zip(std::forward<View1>(view1), std::forward<View2>(view2),
			[](auto& x, auto& y) { std::swap(x, y); });
	}
}
