#pragma once
#include "view.hpp"

namespace PPmatrix
{
	constexpr void fill(view auto&& v, const auto& value)
	{
		for (auto&& x : v)
			x = value;
	}
}
