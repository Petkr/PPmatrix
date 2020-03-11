#pragma once
#include "zip.hpp"

namespace PPmatrix
{
	constexpr void swap_ranges(view auto&& view1, view auto&& view2)
	{
		zip(view1, view2, [](auto&& x, auto&& y) { std::swap(x, y); });
	}
}
