#pragma once
#include "zip.hpp"

template <typename View1, typename View2>
constexpr void swap_ranges(View1&& view1, View2&& view2)
{
	zip(std::forward<View1>(view1), std::forward<View2>(view2),
		[](auto& x, auto& y) { std::swap(x, y); });
}
