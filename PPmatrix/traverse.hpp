#pragma once

namespace PPmatrix
{
	template <typename View>
	constexpr void traverse(View&& view)
	{
		for (auto&& x : view) (void)x;
	}
}
