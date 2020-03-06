#pragma once

namespace PPmatrix
{
	template <view View>
	constexpr void traverse(View&& view)
	{
		for (auto&& x : view) (void)x;
	}
}
