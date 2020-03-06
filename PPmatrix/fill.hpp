#pragma once

namespace PPmatrix
{
	template <view View, typename T>
	constexpr void fill(View&& view, const T& value)
	{
		for (auto&& x : view)
			x = value;
	}
}
