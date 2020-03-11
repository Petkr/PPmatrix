#pragma once

namespace PPmatrix
{
	constexpr void traverse(view auto&& view)
	{
		for (auto&& x : view)
			(void)x;
	}
}
