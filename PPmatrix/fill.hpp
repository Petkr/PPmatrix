#pragma once

namespace PPmatrix
{
	template <typename View, typename T>
	constexpr void fill(View&& view, const T& value)
	{
		for (auto&& x : view)
			x = value;
	}
}
